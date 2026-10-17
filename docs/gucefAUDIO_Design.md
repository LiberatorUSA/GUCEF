# gucefAUDIO: design

Status: **steps 0 to 3 of section 6 are implemented and tested, see section 9. The plugin parts (codec plugin API, driver plugin API, ALSA, streaming, push devices) are next and are the interfaces that need your review first.** Module: `platform/gucefAUDIO`, namespace `GUCEF::AUDIO`, files `gucefAUDIO_*.h/.cpp`. Backends and decoders are **plugins** (`plugins/AUDIO/...`), exactly as gucefINPUT drivers and gucefIMAGE codecs are. `platform/gucefAUDIO` holds no third party code.

## 1. Goals and scope

A cross platform audio abstraction that is on par with the structure of engine audio layers (device abstraction, mixer with buses and voices, streaming, codecs), small at first, but where everything that comes later is **additive**:

| now | later, without breaking the interface |
|---|---|
| playback on a device, pull model | capture (the device interface already has a direction) |
| voices: play / stop / pause, volume, pan, loop, playback rate | 3D position / doppler (extra voice parameters), effects (insert slots on buses), HRTF |
| buses (master, music, effects, any tree) with volume and mute | bus effects (reverb, EQ, compressor), ducking, metering |
| in memory clips and streamed sources | seekable / sample accurate sync, scheduled start times |
| decoders as plugins, WAV and raw PCM built in | Vorbis, Opus, FLAC, MP3, ... as plugins |
| native backends as plugins: ALSA, WASAPI, null, later CoreAudio, AAudio, WebAudio | PulseAudio / PipeWire, JACK, device hot plug |

Not goals now: DSP effects, 3D audio, capture, authoring tools.

## 2. Layers

```
 application ──► CAudioEngine ──► CAudioBus tree ──► CAudioVoice ──► IAudioSource ──► (CAudioClip | CAudioStream ◄── CIAudioCodec plugin)
                      │
                      └──► CAudioDevice ◄── CAudioDriver (plugin: ALSA, WASAPI, null, ...)
```

### 2.1 Format and buffers (runtime selectable sample type, as `CPixelMap`)

* `CAudioFormat`: `sampleRate`, `channelCount`, `sampleType` (`SAMPLE_UINT8`, `SAMPLE_INT16`, `SAMPLE_INT24` packed, `SAMPLE_INT32`, `SAMPLE_FLOAT32`, `SAMPLE_FLOAT64`), `channelLayout` (bit mask of speaker positions, default derived from the count). A class with accessors, not a bare struct, so fields can be added.
* `CAudioBuffer`: like `CPixelMap` it is **type erased with a runtime sample type**: interleaved frames in a `CORE::CDynamicBuffer` plus its `CAudioFormat`; `GetFrameCount()`, `GetFrameSizeInBytes()`, `GetDataPtr( frame )`, `ConvertFormatTo( ... )` in place or into a new shared buffer. No per sample variant: the type is decided per buffer, never per sample.
* `CAudioConverter`: sample type conversion, channel remix (pan law, up / down mix by layout) and rate conversion. All sample types convert through float (hub and spoke, so N converters not N x N); the rate converter is the interface `IAudioResampler` (linear first, better ones plug in later).
* **Mix format.** The industry standard for the internal mix is 32 bit float (FMOD, Wwise, Unity, OpenAL Soft, miniaudio, WebAudio, WASAPI shared mode, CoreAudio): headroom without inter stage clipping, trivial gain math, SIMD friendly, 24 bit mantissa covers all 16 / 24 bit sources. Fixed point only pays off on FPU-less targets (gucefARDUINO). So: the mixing kernels are written as templates on the sample type and selected at run time through `CAudioEngineParams::mixSampleType` (default `FLOAT32`; an `INT32` fixed point kernel can be added later without interface change). Clips stay in their decoded sample type in memory and convert on the fly when a voice reads them, so an 8 / 16 bit clip is not doubled in size. The **device** has its own negotiated format (ALSA often int16, WASAPI / CoreAudio / WebAudio float); the engine converts once at the device boundary. The mix format is therefore independent of the device format and of the platform.

### 2.2 Sources and codecs

* `IAudioSource`: pull model, PCM out. `GetFormat()`, `Read( buffer, frames )` returns frames produced (0 = end), `IsSeekable()`, `Seek( frame )`, `GetLengthInFrames()` (may be unknown), `Rewind()` for looping. Sources are used by one voice at a time.
* `CAudioClip`: decoded PCM held in memory, shared (`CTSharedPtr`) by many voices, created from a buffer or by decoding a whole `CIOAccess`.
* `CAudioStream`: wraps an `IAudioSource` made by a codec, reads ahead on a **task** (`CORE::CTaskConsumer` on the task manager) into a ring buffer so the render path never decodes or touches IO.
* `CIAudioCodec : CORE::CICodec`, modeled on `IMAGE::CIImageCodec`: `OpenSource( CIOAccess& encodedInput, CAudioSourcePtr& ) `, `DecodeMetaData( CIOAccess&, CValueList& )` (format, length, tags), optional `Encode` (capability flagged, not required). Registered in `CAudioCodecRegistry` (a `CTGlobalONRegistry`), reachable through `CAudioGlobal`.
* Plugin wrapper `CAudioCodecPlugin : CStdCodecPlugin` with a C API (`AUDIOCODECPLUG_*`: open, read, seek, close, metadata), as the image codec plugins have. **Built in, no plugin needed:** `wav` (RIFF PCM, own code) and `raw` (headerless PCM described by a `CValueList`: rate, channels, sample type; this is what games with legacy formats use).

### 2.3 Drivers and devices (the backend plugin API)

* `CAudioDriver` (modeled on `INPUT::CInputDriver`): `GetName()`, `GetVersion()`, `EnumerateDevices( CAudioDeviceInfoList&, direction )`, `GetDefaultDevice()`, `OpenDevice( CAudioDeviceParams, CAudioDevicePtr& )`. `CAudioDriverRegistry` holds drivers; a driver is either **built in** (`CAudioNullDriver`, deterministic, with manual `Render()` for tests and silent machines) or **loaded from a plugin** (`CAudioDriverPlugin : CAudioDriver, CIPlugin`, wrapping the C API below). Registering a C++ driver object directly is also supported, which is the hook the planned static plugin registry (Emscripten) needs.
* `CAudioDevice`: `GetFormat()` (the **negotiated** format, the engine adapts to it, the app does not have to guess), `Start()`, `Stop()`, `GetLatencyInFrames()`, `GetDirection()`. The driver pulls: it calls the device's render callback with a buffer to fill from its own audio thread (or, for backends without threads like WebAudio, from the platform callback). `CAudioDeviceParams`: device id (empty = default), direction (`DIRECTION_PLAYBACK` now, `DIRECTION_CAPTURE` defined now and implemented later), preferred format, buffer size hint in frames or ms, shared / exclusive hint, free `CValueList` for driver specific options.
* C plugin API (`AUDIODRIVERPLUG_*`, same shape as `INPUTDRIVERPLUG_*`): `Init`, `Shutdown`, `Name`, `Copyright`, `Version`, `EnumerateDevices`, `OpenDevice( params, callbacks, &deviceHandle, &negotiatedFormat )`, `StartDevice`, `StopDevice`, `CloseDevice`, `GetDeviceLatency`. Callbacks struct `TAudioDriverCallbacks { onRender, onCaptureData, onDeviceLost, onDeviceFormatChanged, userData }`. Every struct carries a `structSize` first member and new entry points are only ever appended, which is what keeps plugins built against an older header loadable.
* Device failure (unplugged, driver reset) is reported through `onDeviceLost`; the engine raises an event and, when a default device is in use, can reopen it (policy flag).

### 2.4 Engine, buses, voices

* `CAudioEngine`: owns the device, the bus tree, the voice list and the clip cache option. `Initialize( CAudioEngineParams )` (driver name or "best", device id, format hint), `Shutdown()`, `GetMasterBus()`, `CreateBus( name, parent )`, `Play( source or clip, CAudioVoiceParams, bus )` returns a `CAudioVoice` handle, `StopAll()`, `SetMasterVolume()`. One engine per device; several engines may coexist.
* `CAudioBus`: name, volume (linear gain with helpers for dB), mute, parent / children, and an (empty for now) **insert slot list** of `IAudioEffect` so effects are additive later.
* `CAudioVoice`: a playing instance. Handle type is a shared pointer to a small control object (safe to keep after the voice ended). `Stop()`, `Pause()`, `Resume()`, `SetVolume()`, `SetPan()`, `SetPlaybackRate()` (resampler ratio), `SetLooping( count / infinite )`, `GetState()` (`PLAYING`, `PAUSED`, `ENDED`), `GetPositionInFrames()`. 3D parameters will be added as further setters.
* Mixing: float32, per voice convert and resample to the device format, apply voice gain / pan, accumulate into the bus buffer, bus gain, sum into the parent, master out, clip protection (soft limit on the final stage, flag). Mixing is **deterministic** for a given input and render size (the null driver test relies on this).

### 2.5 Threading, async and events

**Principle: the task manager is the single point of knowledge for all threads** (pool threads, dedicated threads and others alike). gucefAUDIO therefore never creates a thread any other way: no `std::thread`, no raw `CActiveObject` in the library, and backend plugins do not create threads either. Where a thread is dedicated it is a `CTaskConsumer` started on a named pool with `StartTaskWithConsumer`, so it shows up in `GetAllThreadInfo()` and shuts down with `RequestAllThreadsToStop()`. The one case we do not control is a **callback only backend** (CoreAudio, AAudio, WebAudio) where the OS creates the render thread; those threads are foreign to the task manager. Decided generic CORE addition: a pseudo thread pool, **`CTaskManager::ExternalThreadPoolName`**, that does not schedule anything and only tracks threads created outside GUCEF. A foreign thread announces itself with an RAII `CExternalThreadScope( name, role )` (registers on construction, unregisters on destruction), after which it appears in `GetAllThreadInfo()` and the pool info like any other thread, flagged external. The render callback of a callback only backend opens such a scope on its first call (a one time registration, off the hot path afterwards). Backends that allow it (ALSA, WASAPI event mode, null) use a pool owned thread instead.

There are four kinds of execution contexts, each with a rule:

| context | owner | does | must never |
|---|---|---|---|
| **owner thread** (the application / pulse generator thread) | application | engine API, `Update()` / `OnPulse`, event dispatch, sync loads | be required to call per frame for audio to keep playing |
| **render context** | the driver | pulls the mix: drains commands, mixes, reads stream ring buffers, converts to the device format | allocate, lock a contended mutex, do IO, decode, log, notify |
| **engine thread** (optional) | gucefAUDIO, a dedicated `CTaskConsumer` started with `CTaskManager` `StartTaskWithConsumer` on the pool `"AudioEngine"` | the mix when the backend is push / blocking style (ALSA blocking write, the null driver in real time mode): a helper `CAudioPushDevice` in gucefAUDIO runs the pull loop so backend plugins only supply a blocking write and **never create threads themselves**; pull style backends (WASAPI event, CoreAudio, AAudio, WebAudio) call the render context directly from their own audio thread | |
| **streaming tasks** | `CORE::CTaskManager`, thread pool `"AudioStreaming"` (name configurable) | file reads, decoding, resampling of streamed sources into the ring buffers; async loads | touch voices or buses |

* **Threading mode** (`CAudioEngineParams::threadingMode`): `AUTO` (default: threads where the platform has them), `THREADED`, `COOPERATIVE`. Cooperative is for single threaded targets (Emscripten without pthreads): no engine thread, stream refills run in slices from `Update()`, same API. Nothing in the interface depends on the mode.
* **One rule: if the work runs on another thread (any thread: pool, dedicated, the render context, later a coroutine), the call returns a `CFutureResult`; if it completes inline, it does not.** Names follow: `LoadClip()` is inline; `LoadClipAsync()` runs on the pool. For *commands to the render context* the same rule applies: `voice->Start()`, `Stop()`, `Pause()`, `Seek()` return a `CFutureResult` that is ready once the render context has applied the command. `CreateVoice( clip, params, bus )` is inline and returns the `CAudioVoicePtr` handle (nothing audible yet), so the handle exists before the future; `Play( clip, params, bus, &voice )` is the convenience that does both and returns the future. Continuous parameters (`SetVolume`, `SetPan`, `SetPlaybackRate`, bus gains) are **not** delegated work: the call itself only publishes a value (an atomic store) that the render context reads on its own schedule, so they complete inline and return nothing. The end of a voice is `voice->GetEndedResult()`, a future ready when the voice has ended (lazily created, so voices nobody waits on cost nothing).
* **What completes a future for render context commands.** `CFutureResult` today is a handle on a `CTask` (a status polled by `Await()`), and `CTask::SetTaskStatus` is private to the task manager classes, so nothing outside the pool can complete one. Generic CORE addition needed (small, additive): a **promise** (`CFutureResultSource`, producer side) that creates a `CTask` in a running state and lets any owner `Complete( status )` / `Fail( status, info )` / set extra info, with `GetFuture()` handing out `CFutureResult`. Leaves room for completion callbacks later, which a coroutine awaiter needs (`await_suspend` registers a continuation), without changing `CFutureResult`. The render context never calls it (it locks, allocates and notifies): it enqueues a completion record and the **engine thread** (or `Update()` in cooperative mode) completes the promise, so `Await()` from any thread does not depend on the application pumping. Known cost: a `CTask` is a full `CNotifier` object, so one per `Start()`; fine at sound effect rates, and if measurements say otherwise the fix is making `CTask` construction lighter in CORE, not changing this API.
* **Cross thread communication**: owner to render context through a bounded command queue (single producer per engine, serialized by a short mutex among owner side callers, single consumer), render to owner through an event queue. These are **single consumer ring buffers on atomics**. GUCEF has no atomics facility today (nothing in gucefMT or gucefCORE), so the first step of the implementation is a small generic addition to gucefMT (`MT::CTAtomic< T >` with load / store / exchange / compare and swap / fetch add with explicit ordering, over `<atomic>` where available and the compiler or OS intrinsics elsewhere). It has landed (`gucefMT_CTAtomic.h`).
* **Events and the notifier.** `CNotifier::NotifyObservers` takes a notifier mutex, looks up the event, builds a std::set per dispatch and (in debug) formats log strings; that is right for control rate, cross system connectivity (audio to video, MVC style GUI updates) and wrong for anything on the render context. So: the render context only enqueues plain event records; the owner thread drains them in `OnPulse` and then (a) raises `CNotifier` events for loosely coupled consumers: `VoiceEnded`, `DeviceOpened`, `DeviceLost`, `DeviceChanged`, `StreamUnderrun`, and (b) calls optional **direct functor callbacks** registered per voice (`SetOnEndedHandler( functor )`) for consumers that want the cheap path. Neither runs on the audio thread. Latency critical consumers (for example video synchronization) do not use events at all: they **poll the playback clock**, `CAudioVoice::GetPositionInFrames()` and `CAudioEngine::GetOutputClockInFrames()`, which are single atomic reads of values the render context publishes.
* Underrun: output silence for the missing part, count it, raise `StreamUnderrun` through the event queue; never block.
* Shutdown order is defined: stop accepting commands, stop the device (render context quiet), join the engine thread, wait for stream tasks (`RequestTaskToStop`), drain events, release.

### 2.6 Module plumbing

`CAudioGlobal` singleton (`GetAudioDriverRegistry()`, `GetAudioCodecRegistry()`, plugin managers), `gucefAUDIO.h` umbrella, `CGUCEFAUDIOModule`, `gucefAUDIO_macros.h` / `_config.h` exports, `ModuleInfo.xml` (depends on gucefCORE, gucefMT), all as `gucefINPUT` does. Library prefix `AUDIO`, plugin export macro `GUCEF_AUDIO_PLUGIN_PUBLIC_C`.

## 3. Plugins (first set)

| plugin | what | notes |
|---|---|---|
| `audiodriverALSA` | Linux backend | `plugins/AUDIO/audiodriverALSA`. Rule: anything that is not an intrinsic part of the platform is a plugin, and an installed package such as ALSA is not intrinsic. Inside the plugin libasound is bound at run time (the few entry points are declared in the plugin and `dlopen`ed from `libasound.so.2`) so the plugin builds without the ALSA development package and simply fails to initialize on a machine without it. Uses `CAudioPushDevice`, so no thread code in the plugin |
| `audiodriverWASAPI` | Windows backend | written against the documented API, cannot be run in this environment |
| (built in) `null` | tests, headless | deterministic, manual `Render()`; no platform dependency, so built in |
| later | `audiodriverWEBAUDIO` (AudioWorklet), `audiodriverAAUDIO`, `audiodriverCOREAUDIO`, PulseAudio, codec plugins | |

Codec plugins for compressed formats are separate work: they may wrap open source decoders (allowed in plugins, not in `platform/`).

## 4. Tests (GUCEF test framework, `tests/gucefAUDIO_TestApp`)

Format and conversion (types, channel remix, resampling accuracy), clip and stream sources, WAV and raw codecs, codec registry and plugin loading, bus and voice behaviour (gain, pan, loop, pause, end event), mixing against computed expected PCM through the null driver, command queue ordering, underrun, device lost handling, driver registry and plugin API round trip with a test plugin, ALSA plugin open / render on a machine with a device (skips otherwise). No test needs a sound card.

## 5. What this replaces in Galaxy Unlimited

`guccAUDIO` (mixer and ALSA device) is removed once gucefAUDIO has the equivalent; `gucreUI::CSoundPlayer` switches to `AUDIO::CAudioEngine` (effects on an "effects" bus, music on a "music" bus, the original's volume curve stays game code). The original game's sound formats are described to the engine as `raw` sources; no game specific code enters GUCEF.

## 6. Implementation order

0. CORE / MT prerequisites: gucefMT `CTAtomic` (**done**: `gucefMT_CTAtomic.h`, backends std / GCC `__atomic` / MSVC / GCC `__sync` / lock, 35 tests per platform in gucefMT_TestApp, C++98 to C++20, verified with gcc, clang, -O2, TSan; MSVC backend type checked only); gucefCORE `CFutureResultSource` (**done**: promise side of `CFutureResult`, any thread can complete, first completion wins, result data, cancellation requests reach the worker; 7 tests); gucefCORE external thread tracking (**done**: `CTaskManager::RegisterExternalThread` / `ExternalThreadPoolName` ("external"), `CExternalThreadScope`, `CThreadInfo` gained name and external flag; 4 tests). `Fail()` only takes error statuses that are end states; throttle conditions such as `TASKSTATUS_RESOURCE_LIMIT_REACHED` are not end states on purpose (the condition may clear) and are set with `SetStatus()`, the caller sees them through `HasNoFuture()`.
1. Module skeleton, types, buffers, converter, resampler interface, tests.
2. Sources, clip, WAV and raw codecs, registry, codec plugin wrapper and C API.
3. Driver interface, registry, null driver, device, engine, buses, voices, mixing, events; tests.
4. Driver plugin wrapper and C API; ALSA plugin; manual listening check.
5. Streaming tasks and ring buffers, async API, `CAudioPushDevice`.
6. Switch Galaxy Unlimited, delete `guccAUDIO`.
7. WASAPI plugin (Windows), later the web and Android backends.

## 7. Decisions taken in review

1. Events: `CNotifier` for control rate and cross system events, direct functors for cheaper per voice callbacks, polled atomic clocks for latency critical consumers; the audio thread touches none of them. The notifier itself is left unchanged (its locking and per dispatch allocation are fine for its purpose).
2. Mix format: runtime selectable sample type in the style of `CPixelMap`, 32 bit float by default (industry standard), device format negotiated separately.
3. ALSA is a plugin (not intrinsic to the platform), `dlopen` of libasound inside the plugin.
4. Plugin directory `plugins/AUDIO`, names `audiodriver*` and `audiocodecplugin*`.

## 8. Open questions

1. `MT::CTAtomic` as a new generic gucefMT facility (step 0): OK to add? Alternative is mutex based queues only, which is simpler but not real time safe.
2. Default name of the streaming thread pool and its size (proposal: `AudioStreaming`, 1 thread, grows with concurrent streams up to 4), and `AudioEngine` for the dedicated engine thread.
3. CORE additions (decided in principle): `CFutureResultSource` (promise) and the external thread pseudo pool with `CExternalThreadScope`, see 2.5. Both generic, both additive; they get their own tests in gucefCORE_TestApp and come before the audio engine in the implementation order.

## 9. Implementation status (what exists, and where it differs from the proposal)

Everything below is in `platform/gucefAUDIO` with tests in `tests/gucefAUDIO_TestApp` (82 tests), plus the prerequisites in gucefMT (`CTAtomic`, `CTSpscQueue`) and gucefCORE (`CFutureResultSource`, external thread tracking).

| area | classes | notes |
|---|---|---|
| formats | `CAudioFormat`, `CAudioBuffer`, `CAudioConverter`, `IAudioResampler`, `CLinearResampler` | runtime sample type, conversion hub is float32 (so 32 bit integer and float64 keep 24 bits of precision through a conversion), channel remix matrices by speaker position with row normalisation |
| sources | `IAudioSource`, `CAudioClip` + `CAudioClipSource`, `CPcmStreamSource` | clips are shared and immutable, every source has its own position |
| codecs | `CIAudioCodec`, `CAudioCodecRegistry`, `CWavAudioCodec`, `CRawAudioCodec` | `OpenSource` (streaming), `DecodeAll`, `DecodeMetaData`, `Encode`; WAV reads PCM 8 to 32 bit, float 32 and 64, extensible with layouts, tolerates truncated files and unknown data sizes |
| devices | `CAudioDeviceInfo`, `CAudioDeviceParams`, `IAudioDeviceCallback`, `CAudioDevice`, `CAudioDriver`, `CAudioDriverRegistry`, `CNullAudioDriver` | the null device is driven by hand (`Render()`), so engine tests are deterministic. Driver priority selects the best driver when none is named |
| mixing | `CAudioMixer`, `CAudioBus`, `CAudioVoice` | no allocation or locks in `Mix()`; per piece gain ramps; constant power and balance pan laws; resampling for rate and pitch; looping; pause; limiting with a count of limited samples |
| engine | `CAudioEngine`, `CAudioEngineParams` | commands to the audio thread through `CTSpscQueue`, events back the same way, futures completed by `CFutureResultSource`, optional engine thread as a task manager task |

Differences from the proposal:

* `Play()` / `Start()` and the other commands return futures as agreed. `CreateVoice()` is separate and inline, so the handle exists before the future.
* Events to the application go through `CTSGNotifier::NotifyObserversFromThread`, so they arrive on the thread of the application's pulse generator, which is the GUCEF way. The futures do not depend on that pump.
* Pause and resume futures are completed by looking at the voice state in `Update()`, not by events from the audio thread.
* The engine bounds the number of voices that are started and not yet reported ended (a third of the event queue), so the audio thread can never overflow the event queue with events that must not be lost.
* Shutdown race found and fixed: the engine thread is told to stop through its own lock (`Disconnect()`). The cause was in CORE: a dedicated task's consumer was never linked to its task, so `RequestTaskToStop()` through the consumer or the pool did nothing for dedicated tasks. Fixed in `CThreadPool` (see below); `Disconnect()` stays, it is correct either way.
* CORE fixes that came out of this work, each with a test in gucefCORE_TestApp that fails without the fix:
  * `CThreadPool`: dedicated task consumers are linked to their task at setup ("DedicatedTasks: ... can request it to stop", "... The pool can stop ...").
  * `CActiveObject`: `OnThreadEnded()` is now delivered, exactly once, when a thread ends by itself, as its documentation says. Before, only `Deactivate()` delivered it, so finished dedicated tasks stayed in their pool and the task manager's thread count only grew ("DedicatedTasks: A dedicated task that finished is no longer counted ...").
  * `CThreadPool`: delegators of ended threads are retired instead of released inside the pool's own event handling, since a delegator can hold the last reference to an unregistered pool; they are released from calls made through a pool reference and by `CTaskManager::UnregisterThreadPool` ("DedicatedTasks: A pool which is no longer registered survives ...").
  * `CPumpedObserver`: a handler that destroys the notifier of the event being delivered no longer frees that mail under the pulse, which also dropped the next event ("Pumped observer handler destroying the notifier ...").
  * `CTFreeList`: freed slots were constructed or destroyed depending on how they got there, so `Clear()` destroyed released objects twice and `Acquire()` constructed new slots twice ("CTFreeList Test 7/8").
  * `CTFreeList` reuse via `Clear()` works now (it never did: the `has_clear` check ran on the accessor type and never matched an inherited `Clear()`). The trait detects inherited members (C++98 compatible), cleared objects get their shared pointer creator data re-armed, and `CTBasicSharedPtr::Unlink` releases the lock of shared data embedded in the object before the object is destroyed or recycled (it was left locked, which also caused TSan's "destroy of a locked mutex" reports). `CTask::Clear()` now readies a task for its next use, a fresh task id included, and `CTask::SetTaskData` no longer leaks the copy it makes of data it does not own ("CTFreeList Test 9/10/11", "DedicatedTasks: The copy a task makes ...").
  * `CDynamicBuffer`: `Append` honours the auto enlarge setting, and implicit growth follows a growth strategy (`GROWTHSTRATEGY_EXACT` default, `_GEOMETRIC`, `_BLOCKS`) set with `SetGrowthStrategy()`; explicit sizing stays exact ("DynamicBuffer: ..." tests).
  * `CActiveObject`: its state flags are `CTAtomic< bool >`, which orders the thread's last work before what an observer of `IsActive() == false` does next (TSan data race and vptr reports).
* Not done yet: streaming sources with read ahead on tasks (`CAudioStream`), `CAudioPushDevice`, the plugin wrappers and C APIs for codecs and drivers, the ALSA plugin, soft limiting, bus effects, 3D.

## 10. Plugins: C interface or C++ link back (for review before they are written)

GUCEF has two kinds of plugins:

* **C interface plugins** (`CODECPLUGIN_*`, `IMGCODECPLUGIN_*`, `INPUTDRIVERPLUG_*`): plain C functions and versioned structures. They can be written in any language that can export C functions, they do not depend on the compiler, standard library or GUCEF build they are loaded into, and the interface stays stable for a long time. The price is that everything has to fit through C: objects become handles, callbacks become function pointers with user data, and library services the plugin needs have to be offered to it as functions (a host function table).
* **C++ link back plugins** (`GUCEFPlugin_Load` / `GUCEFPlugin_Unload`, like `vfspluginISO9660`): the plugin links to the GUCEF libraries and registers C++ classes with them. They use everything GUCEF offers directly, but they have to be built with the same compiler, runtime and GUCEF version as what loads them, and an interface change in the library is a change for every plugin.

Considered per kind:

| | audio codecs | audio drivers |
|---|---|---|
| what a plugin does | parse a file format and hand out frames: a pure data transformation | run an OS audio API on a real time thread, with device life cycles and device loss |
| what it needs from GUCEF | read access to the data (`TIOAccess` already exists in C) and a place to put metadata (`TValueMapParserCallbacks` exists) | threads that the task manager knows (`CAudioPushDevice`, external thread registration), formats, logging |
| who writes them | mostly wrappers around C libraries (libvorbis, libFLAC, opus, minimp3): C is their natural interface, and other languages are a realistic source of codecs | platform specific code next to GUCEF itself, by the same people, built with GUCEF |
| how much of the API is callbacks into the library | none | the render callback plus thread services |

**Recommendation:**

* **Codecs: C interface.** It fits without strain (the image codecs prove the pattern), it keeps the door open for codecs in other languages, and a codec built once keeps working with later GUCEF versions. Section 10.2.
* **Drivers: C++ link back.** A driver is a `CAudioDriver` subclass registered in the `CAudioDriverRegistry` from `GUCEFPlugin_Load`. It uses `CAudioPushDevice`, `CExternalThreadScope` and the task manager directly, so the thread rule (the task manager knows every thread) is enforced by using the classes rather than by a host function table that a C plugin could ignore. Statically linked builds (WebAssembly) register the same class directly. The cost, building drivers with the same toolchain as GUCEF, is what happens for platform code anyway. A C interface for drivers is possible (the input drivers have one), and section 10.1 keeps the sketch of it in case you prefer it: it needs the host function table for threads, which is the part that makes it awkward.

### 10.1 Audio driver plugins (`audiodriver*`): the C interface it would take (not the recommendation, kept for comparison)

```c
/* version of this API, in the high 16 bits what breaks, in the low 16 bits what was added */
#define GUCEF_AUDIO_DRIVER_API_VERSION  0x00010000

typedef struct SAudioFormatInfo        /* version 1 */
{
    UInt32 version, structSize;
    UInt32 sampleRate;
    UInt32 channelCount;
    UInt32 sampleType;                 /* ESampleType, values never change */
    UInt32 channelLayout;              /* speaker mask, 0 = default for the count */
} TAudioFormatInfo;

typedef struct SAudioDeviceInfo        /* version 1 */
{
    UInt32 version, structSize;
    const char* id;                    /* stable id, as the driver wants it back */
    const char* name;                  /* for people */
    UInt32 direction;                  /* EAudioDirection */
    UInt32 isDefault;
    UInt32 maxChannelCount;
    TAudioFormatInfo preferredFormat;
} TAudioDeviceInfo;

typedef struct SAudioDeviceOpenParams  /* version 1 */
{
    UInt32 version, structSize;
    const char* deviceId;              /* NULL or empty = default device */
    UInt32 direction;
    TAudioFormatInfo preferredFormat;  /* zero fields = device decides */
    UInt32 bufferSizeInFrames;         /* 0 = driver decides */
    UInt32 shareMode;
    const char* options;               /* "name=value" pairs separated by ';', driver specific */
} TAudioDeviceOpenParams;

typedef struct SAudioDriverCallbacks   /* version 1; the library fills it in, the plugin calls it from its audio thread */
{
    UInt32 version, structSize;
    void ( *onRender )( void* userData, void* buffer, UInt32 frameCount );
    void ( *onCapture )( void* userData, const void* buffer, UInt32 frameCount );
    void ( *onDeviceLost )( void* userData );
    void* userData;
} TAudioDriverCallbacks;

/* life cycle and identity, as for the other plugin kinds */
UInt32      AUDIODRIVERPLUG_Init( void** plugdata, const int argc, const char** argv );    /* 0 = this machine cannot do it (no libasound) */
UInt32      AUDIODRIVERPLUG_Shutdown( void* plugdata );
const char* AUDIODRIVERPLUG_Name( void* plugdata );                                         /* "alsa" */
const char* AUDIODRIVERPLUG_Description( void* plugdata );
const char* AUDIODRIVERPLUG_Copyright( void* plugdata );
const TVersion* AUDIODRIVERPLUG_Version( void* plugdata );
UInt32      AUDIODRIVERPLUG_ApiVersion( void* plugdata );                                  /* GUCEF_AUDIO_DRIVER_API_VERSION the plugin was built with */
Int32       AUDIODRIVERPLUG_Priority( void* plugdata );

/* devices: iteration instead of a caller allocated array, no size negotiation */
UInt32 AUDIODRIVERPLUG_EnumerateDevicesBegin( void* plugdata, UInt32 direction, void** iterator );
UInt32 AUDIODRIVERPLUG_GetDeviceInfo( void* plugdata, void* iterator, TAudioDeviceInfo* info );
UInt32 AUDIODRIVERPLUG_EnumerateDevicesNext( void* plugdata, void* iterator );              /* 0 = no more */
UInt32 AUDIODRIVERPLUG_EnumerateDevicesEnd( void* plugdata, void* iterator );

/* a device */
UInt32 AUDIODRIVERPLUG_OpenDevice( void* plugdata, const TAudioDeviceOpenParams* params, const TAudioDriverCallbacks* callbacks, void** device, TAudioFormatInfo* negotiated, UInt32* periodFrames, UInt32* latencyFrames );
UInt32 AUDIODRIVERPLUG_StartDevice( void* plugdata, void* device );
UInt32 AUDIODRIVERPLUG_StopDevice( void* plugdata, void* device );                          /* returns when the callbacks have stopped */
UInt32 AUDIODRIVERPLUG_CloseDevice( void* plugdata, void* device );
```

Threads: the plugin may run the device on a thread of its own or be called by the OS on a thread the OS made. A thread the plugin creates itself is not allowed (the task manager must know every thread): plugins that need a thread for a blocking backend (ALSA) use the library helper `CAudioPushDevice`, exposed to plugins as `AUDIODRIVERPLUG_` host functions: `host->runPushLoop( device, writeFunction )`, so the loop runs on a task manager thread. Callback only backends call `host->enterExternalThread( "name" )` once from the first callback, which registers the OS thread in the external pool (design 2.5).

### 10.2 Audio codec plugins (`audiocodecplugin*`)

Modelled on the DStore codec plugins (`gucefCORE_c_dstore_plugin_api.h`), the most recent C plugin interface in CORE, and built from the generic C capabilities CORE already offers to C plugins, so a codec plugin reinvents none of them:

| need | reused from CORE (`include/c_api`) |
|---|---|
| logging | `TGucefCoreCApi` handed to `Init`, used through the `GUCEF_C_LOG( api, level, msg )` macro family of `gucefCORE_c_logging.h`, which proxies to the C++ log manager |
| reading and writing data | `TIOAccess` (`gucefCORE_c_ioaccess.h`), the C view of `CIOAccess`: files, VFS, memory, ISO alike |
| options in, metadata out | `TVariantMapApi` (`gucefCORE_c_variantmap.h`) over `TVariantData` keys and values; the host side wraps its containers with `CTVariantMapAdapterCpp2C`, so the plugin reads options from and writes tags (title, artist, loop points, ...) into C++ maps directly |
| identity | `const TVersion*`, and plugin owned static strings, as `DSTOREPLUG_Name` / `_Copyright` / `_Version` |

Shape, as for DStore: the plugin exports plain functions with a common prefix, the host resolves them by name into a function table (`TGucefAudioCodecPluginApi`, the counterpart of `TGucefCoreCDStorePluginApi`), and opaque state comes in levels: `plugdata` per plugin, `codecdata` per codec type the plugin offers, `sourcedata` per open stream (DStore's `filedata`).

```c
typedef struct SAudioFormatInfo        /* published once, never changed; a later need gets a new struct and new functions */
{
    UInt32 sampleRate;
    UInt32 channelCount;
    UInt32 sampleType;                 /* ESampleType, values never change */
    UInt32 channelLayout;              /* speaker mask, 0 = default for the count */
} TAudioFormatInfo;

/* plugin: life cycle and identity */
UInt32          AUDIOCODECPLUG_Init( void** plugdata, TGucefCoreCApi* libApi );               /* keeps libApi for logging */
void            AUDIOCODECPLUG_Shutdown( void** plugdata );
const char*     AUDIOCODECPLUG_Name( const void* plugdata );
const char*     AUDIOCODECPLUG_Copyright( const void* plugdata );
const TVersion* AUDIOCODECPLUG_Version( const void* plugdata );
UInt32          AUDIOCODECPLUG_Type_Count( const void* plugdata );                             /* a libsndfile wrapper offers many */
const char*     AUDIOCODECPLUG_Type( const void* plugdata, UInt32 index );                     /* "wav", "ogg", "flac", ... */

/* per codec type */
UInt32 AUDIOCODECPLUG_Create_Codec( void** plugdata, const char* codecType, void** codecdata );
void   AUDIOCODECPLUG_Destroy_Codec( void** plugdata, void** codecdata );

/* decoding, streaming */
UInt32 AUDIOCODECPLUG_Src_Open( void** plugdata, void** codecdata, void** sourcedata, TIOAccess* input, const TVariantMapApi* options, TAudioFormatInfo* format, UInt64* lengthInFrames );   /* length GUCEF_UINT64MAX = unknown */
UInt32 AUDIOCODECPLUG_Src_Read_Frames( void** plugdata, void** codecdata, void** sourcedata, void* destination, UInt32 frameCapacity, UInt32* framesRead );  /* 1 with 0 frames read = end */
UInt32 AUDIOCODECPLUG_Src_Seek( void** plugdata, void** codecdata, void** sourcedata, UInt64 frame );                                      /* optional: absent or 0 = not seekable */
void   AUDIOCODECPLUG_Src_Close( void** plugdata, void** codecdata, void** sourcedata );

/* optional */
UInt32 AUDIOCODECPLUG_Decode_Meta_Data( void** plugdata, void** codecdata, TIOAccess* input, TVariantMapApi* metaData );
UInt32 AUDIOCODECPLUG_Encode( void** plugdata, void** codecdata, const TAudioFormatInfo* format, const void* frames, UInt64 frameCount, TIOAccess* output, const TVariantMapApi* options );
```

The host side is one C++ class, `CAudioCodecCPlugin` (a `CIAudioCodec` per codec type, registered in `CAudioCodecRegistry`), so C plugins and built in codecs look the same to the rest of gucefAUDIO. The plugin links nothing from GUCEF; it needs only the headers, like the DStore plugins.

How this answers the earlier questions:

* Versioning: as in the DStore API, the interface grows by adding optional functions, which a plugin either exports or not (the host table entry stays `NULL`). Data structures are frozen once published. No `version` / `structSize` members.
* Strings: plugins hand out only static strings (identity, type names). Everything per stream that is text goes into host maps through `TVariantMapApi`, where the host copies it, so there are no lifetime rules to learn.
* No codec level callbacks into the library, so no host function table beyond `TGucefCoreCApi` is needed.

The driver C sketch in 10.1 would take the same building blocks (`TGucefCoreCApi`, `TVariantMapApi` for the options string, no struct versions), which removes some of its awkwardness but not the thread services, so the recommendation for drivers stays C++ link back.

### 10.3 The first drivers

* `audiodriverALSA` (Linux): blocking PCM write on a `CAudioPushDevice` thread, period and buffer size from the params, `dlopen("libasound.so.2")` inside the plugin, enumeration by `snd_device_name_hint`, negotiated format through `snd_pcm_hw_params`, xruns recovered with `snd_pcm_prepare` and counted.
* `audiodriverWASAPI` (Windows): event driven shared mode, the render callback runs on a thread made by WASAPI (registered as external).
* Later: `audiodriverWEBAUDIO` (AudioWorklet), `audiodriverAAUDIO`, `audiodriverCOREAUDIO`.

### 10.4 Questions

1. Decided: codecs as C interface plugins, drivers as C++ link back plugins.
2. Decided by following the DStore API: no struct versions, additive optional functions instead (10.2).
3. Decided by following the DStore API: static strings only, per stream text through `TVariantMapApi` (10.2).
4. Decided: `CTFreeList` reuses objects via `Clear()` whenever the type has one (section 9).
