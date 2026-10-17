# gucefAUDIO

Module `platform/gucefAUDIO`, namespace `GUCEF::AUDIO`, files `gucefAUDIO_*.h/.cpp`. Cross platform audio: formats and buffers, conversion, sources and codecs, devices through drivers, and an engine that mixes buses and voices. Backends and decoders are **plugins** (`plugins/AUDIO/...`), as gucefINPUT drivers and gucefIMAGE codecs are; `platform/gucefAUDIO` holds no third party code.

Status: the library, the codec C plugin host with a reference codec plugin, and drivers for Linux (ALSA, run on hardware), Windows (WASAPI), Android (AAudio) and the web (Web Audio). The Windows, Android and web drivers are compiled for their platforms and not yet run there. Tests: `tests/gucefAUDIO_TestApp`.

## 1. Scope

| supported | can be added without breaking the interface |
|---|---|
| playback on a device, pull model | capture (the device interface has a direction) |
| voices: play / stop / pause, volume, pan, loop, playback rate | 3D position, doppler, HRTF (extra voice parameters) |
| buses (master, music, effects, any tree) with volume and mute | bus effects (insert slots), ducking, metering |
| in memory clips and streamed sources | sample accurate scheduling |
| WAV and raw PCM built in, other formats as codec plugins | Vorbis, Opus, FLAC, MP3, ... as plugins |
| drivers as plugins: ALSA, WASAPI, AAudio, Web Audio, built in null | CoreAudio, PulseAudio / PipeWire, JACK, device hot plug |

## 2. Layers

```
 application ──► CAudioEngine ──► CAudioBus tree ──► CAudioVoice ──► IAudioSource ──► (CAudioClip | CAudioStream ◄── CIAudioCodec)
                      │
                      └──► CAudioDevice ◄── CAudioDriver (plugin: ALSA, WASAPI, AAudio, Web Audio; built in: null)
```

### 2.1 Formats and buffers

* `CAudioFormat`: sample rate, channel count, sample type (`SAMPLETYPE_UINT8`, `INT16`, `INT24` packed, `INT32`, `FLOAT32`, `FLOAT64`), channel layout (speaker mask `AUDIOSPEAKER_*`, the usual one for the count by default).
* `CAudioBuffer`: interleaved frames of a runtime selected sample type (as `IMAGE::CPixelMap` has a runtime pixel format), with conversion to another format in place or into a new buffer.
* `CAudioConverter`: sample type conversion through float32 (32 bit integer and float64 keep 24 bits of precision), channel remix by speaker position with normalised matrices, rate conversion through `IAudioResampler` (`CLinearResampler`).
* Mixing is done in 32 bit float. Clips stay in their own sample type and convert when a voice reads them. The device has its own negotiated format; the engine converts once at the device boundary.

### 2.2 Sources and codecs

* `IAudioSource`: pull model PCM. `GetFormat()`, `ReadFrames()` (fewer frames than asked: at the end or nothing more right now, `IsEndOfStream()` tells which), optional `SeekToFrame()`, `GetPositionInFrames()`, `GetLengthInFrames()` (may be unknown). A source is read by one consumer at a time.
* `CAudioClip`: decoded PCM in memory, shared by many voices (`CAudioClipSource` per voice). `CPcmStreamSource`: PCM read from a `CIOAccess` while it plays.
* `CAudioStream`: reads a source ahead into preallocated blocks on a task of the pool `"AudioStreaming"`, so the audio thread only copies (no IO, locks or allocations). Seeks from the audio thread, seamless looping, underruns counted. `Fill()` drives it without a task (single threaded targets, tests).
* `CIAudioCodec`: `OpenSource()` (streaming), `DecodeAll()`, `DecodeMetaData()`, optional `Encode()`. Registered by type in `CAudioCodecRegistry` (`CAudioGlobal::GetAudioCodecRegistry()`). Built in: `wav` (PCM 8 to 32 bit, float 32 / 64, extensible, tolerant of truncated files) and `raw` (headerless PCM described by options: rate, channels, sample type).
* `CToneSequenceSource`, `CAudioTestSignals`: generated audio for checking a setup: tone ladder, each speaker in turn, sweep, noise.

### 2.3 Drivers and devices

* `CAudioDriver`: `EnumerateDevices()`, `OpenDevice( CAudioDeviceParams, ... )`, a priority used to pick the best driver when none is named. Drivers are registered in `CAudioDriverRegistry`. Built in: `CNullAudioDriver`, whose device is driven by hand (`Render()`), so engine tests are deterministic; it can keep what it rendered (option `keepOutput`).
* `CAudioDevice`: the negotiated format, `Start()`, `Stop()`, latency. The device calls `IAudioDeviceCallback` to have frames rendered. `CAudioDeviceParams`: device id (empty = default), direction, preferred format, buffer size hint, driver specific options.
* `CAudioPushDevice`: the base for blocking backends. The render and write loop runs as a dedicated task on the pool `"AudioDevice"`, the backend only supplies `PushFrames()` and never makes a thread; `OnDeviceThreadStart()` / `OnDeviceThreadEnd()` set up per thread state (COM for WASAPI). `Stop()` returns once the callback cannot run any more; underruns are counted and a lost backend is reported once.

### 2.4 Engine, buses, voices

* `CAudioEngine`: `Initialize( CAudioEngineParams )` (driver name or the best one, device params, threading mode, limiter), `CreateBus()`, `CreateVoice()`, `Start()` / `Stop()` / `Pause()` / `Resume()` / `Play()`, `SetMasterVolume()`, `Update()`, `Shutdown()`. Events: `VoiceEndedEvent`, `DeviceLostEvent`.
* `CAudioBus`: volume, mute, parent and children.
* `CAudioVoice`: volume, pan (constant power or balance), playback rate, loop count, state, end reason, position in source frames.
* `CAudioMixer`: no allocation or locks while mixing, gain ramps per piece, resampling for rate and pitch. A peak limiter on the master output (instant attack, about 50 ms release, on by default in the engine) and a hard clamp behind it.

### 2.5 Threads, futures and events

The task manager knows every thread: gucefAUDIO and its plugins only use task manager threads (dedicated tasks on named pools). Threads made by an operating system audio API register with `CExternalThreadScope` in the pseudo pool `CTaskManager::ExternalThreadPoolName`.

| context | does | never |
|---|---|---|
| application thread | engine calls, `Update()`, events | is needed for audio to keep playing |
| audio thread (driver or `CAudioPushDevice`) | drains commands, mixes, copies from stream blocks, converts to the device format | allocates, waits on a lock, does IO, decodes, logs, notifies |
| engine thread (optional, pool `"AudioEngine"`) | completes futures and posts events as they happen | |
| streaming task (pool `"AudioStreaming"`) | reads and decodes streamed sources | touches voices or buses |

* A call whose work happens on another thread returns a `CORE::CFutureResult`; work that completes inline does not. Commands to the audio thread (`Start`, `Stop`, `Pause`, `Resume`) return futures that are ready once the audio thread applied them; `CreateVoice()` is inline. Settings such as volume and pan are published atomically and return nothing.
* Commands go to the audio thread and events come back through `MT::CTSpscQueue`; futures are completed through `CORE::CFutureResultSource`. Events reach the application through `CTSGNotifier`, on the thread of its pulse generator.
* Consumers that need timing (video) poll `CAudioVoice::GetPositionInFrames()`, an atomic read.
* Threading mode `AUTO`, `THREADED` or `COOPERATIVE` (no engine thread, everything from `Update()`; for targets without threads).
* The engine limits the voices started and not yet reported ended, so the event queue cannot overflow with events that must not be lost.

## 3. Plugins

| plugin | kind | notes |
|---|---|---|
| `audiodriverALSA` | driver, Linux | libasound bound at run time (builds without ALSA development files), a `CAudioPushDevice`, format negotiation, enumeration by name hints, underrun and suspend recovery |
| `audiodriverWASAPI` | driver, Windows | endpoints with friendly names, shared mode in the requested format (Windows converts), event driven writes on a `CAudioPushDevice`, an invalidated endpoint is a lost device |
| `audiodriverAAUDIO` | driver, Android | libaaudio bound at run time, blocking writes on a `CAudioPushDevice` |
| `audiodriverWEBAUDIO` | driver, browsers | a static library linked into the application (entry points prefixed with the module name); a script processor node renders on the browser's main thread, calls from workers are run on the main thread, the context resumes at the first user interaction |
| `audiocodecpluginAU` | codec, C | .au / .snd in 8 to 32 bit, float, double, mu-law and A-law, reading and writing; links nothing from GUCEF; the reference for the codec C interface |

Drivers are C++ link back plugins: a `CAudioDriver` subclass registered in `CAudioDriverRegistry` from `GUCEFPlugin_Load` (unregister with `TryUnregister` in `GUCEFPlugin_Unload`). Statically linked builds register the same class directly.

Codecs are C interface plugins, see section 4. Codec plugins for compressed formats may wrap open source decoders (allowed in plugins, not in `platform/`).

## 4. The codec plugin C interface

Header `include/c_api/gucefAUDIO_c_audiocodec_plugin_api.h`, host classes `CAudioCodecCPlugin`, `CAudioCodecCPluginCodec`, `CAudioCodecPluginManager` (plugin type `"GucefAudioCodecPlugin"`, `CAudioGlobal::GetAudioCodecPluginManager()`). It is built like the DStore plugin interface, from the C facilities of CORE:

| need | from CORE (`include/c_api`) |
|---|---|
| logging | `TGucefCoreCApi` given to `Init`, used with the `GUCEF_C_LOG` macros |
| reading and writing data | `TIOAccess` |
| options in, tags out | `TVariantMapApi` over host maps |
| identity | static strings and `const TVersion*` |

The plugin exports `AUDIOCODECPLUG_*` functions, which the host resolves by name: `Init`, `Shutdown`, `Name`, `Copyright`, `Version`, `Type_Count`, `Type`, `Create_Codec`, `Destroy_Codec`, `Src_Open`, `Src_Read_Frames`, `Src_Close`, and optionally `Src_Seek`, `Decode_Meta_Data`, `Encode`. State comes in levels: `plugdata` per plugin, `codecdata` per codec type, `sourcedata` per open stream. Rules:

* The interface grows by adding optional functions; published structures never change.
* Plugins hand out static strings only; per stream text goes into host maps.
* The host calls a plugin from one thread at a time (one call lock per plugin), so plugins need no locks.
* Every codec type of a plugin becomes a `CIAudioCodec` in the registry; types that are already taken stay with their owner. Unloading a plugin closes its open streams first, which then end instead of calling into the module.

## 5. How to use

```cpp
AUDIO::CAudioEngineParams params;                       // the best driver, its default device
AUDIO::CAudioEngine engine;
engine.Initialize( params );
AUDIO::CAudioBus* music = engine.CreateBus( "music" );

// a sound effect: decode once, play often
AUDIO::TAudioCodecPtr wav;
AUDIO::CAudioGlobal::Instance()->GetAudioCodecRegistry().TryFindCodec( "wav", wav );
AUDIO::CAudioBuffer pcm;
wav->DecodeAll( fileAccess, pcm );
engine.Play( AUDIO::TAudioClipPtr( GUCEF_NEW AUDIO::CAudioClip( pcm ) ) );

// music: streamed while it plays
AUDIO::TAudioSourcePtr source;
wav->OpenSource( fileAccessPtr, source );
AUDIO::TAudioVoicePtr voice = engine.CreateVoice( AUDIO::TAudioSourcePtr( GUCEF_NEW AUDIO::CAudioStream( source ) ), music );
engine.Start( voice );
```

Load driver and codec plugins with `CORE::CPluginControl` before `Initialize()`; without any, the null driver is used. Call `engine.Update()` regularly in cooperative mode.

## 6. Tests

`tests/gucefAUDIO_TestApp`: formats and conversion, clips, streams and codecs, the codec plugin host with `audiocodecpluginAU`, devices and the push device, the mixer (gain, pan, loop, pause, limiter), the engine on the null driver (deterministic, computed expected PCM), streaming, test signals, the ALSA driver (skips without a device) and the Web Audio driver (browser builds). Opt in listening check: `GUCEF_TEST_AUDIO_LISTEN=1`, with `GUCEF_TEST_AUDIO_DEVICE` to choose the device (`null` runs it silently). No test needs a sound card.

## 7. Not supported yet

Capture, bus effects, 3D audio, WASAPI exclusive mode, CoreAudio. On the web, the Web Audio tests wait on a known heap corruption in CORE under wasm.
