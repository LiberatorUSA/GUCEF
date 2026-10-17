# gucefVIDEO

Module `platform/gucefVIDEO`, namespace `GUCEF::VIDEO`, files `gucefVIDEO_*.h/.cpp`. Plays movies with their sound, the picture kept in step with the sound. Decoders are **plugins** (`plugins/VIDEO/videocodecplugin*`) with a C interface, as the audio codecs have (`gucefAUDIO_Design.md` section 4). `platform/gucefVIDEO` holds no third party code. Depends on gucefMT, gucefCORE, gucefIMAGE and gucefAUDIO.

Status: implemented and tested on Linux, compiled for Windows. Codec plugins cannot be loaded on the web yet (that needs a static plugin registry). Tests: `tests/gucefVIDEO_TestApp`.

## 1. Scope

| supported | can be added without breaking the interface |
|---|---|
| decoding a movie frame by frame: the pictures and the audio tracks in it | encoding, capture, hardware decoding |
| pictures as `IMAGE::CPixelMap`: palette indices with a palette, or RGB / RGBA / BGRA | YUV formats (an added pixel format value), textures for a render system |
| a player: start, pause, resume, stop (skip), the end as an event and as a state | seeking in the player, playback rate, looping |
| the sound through a `gucefAUDIO` engine, the picture following its clock | choosing between audio tracks, subtitles |
| decoders as C plugins | demuxers for containers with separate codecs |

## 2. Layers

```
 application ──► CVideoPlayer ──► CVideoDemuxer ──► IVideoSource ◄── CIVideoCodec (registry) ◄── C plugin (videocodecplugin*)
                      │                 │
                      │                 └──► audio track source (AUDIO::IAudioSource) ──► AUDIO::CAudioStream ──► voice in an AUDIO::CAudioEngine
                      └──► current picture (CVideoFrame), clock = the position of that voice
```

### 2.1 Formats and frames

* `CVideoFormat`: width, height, pixel format (`IMAGE::PSF_PALETTE_INDICES` with 8 bit indices, `PSF_RGB`, `PSF_RGBA`, `PSF_BGRA`, 8 bits per component), frame rate as a fraction, frame count (`GUCEF_VIDEO_FRAMECOUNT_UNKNOWN` when the file does not tell). Frame times are computed from the fraction in integers, so they do not drift over long movies.
* `CVideoFrame`: frame number, presentation time in microseconds, the picture (`IMAGE::CPixelMap`), and for palette pictures the palette (256 RGB entries) with a flag that it changed with this frame. `ConvertTo32Bit()` writes RGBA or BGRA into memory of the caller or into a pixel map that is reused. Frames are reused: decoding into the same frame does not allocate.

### 2.2 Sources and codecs

* `IVideoSource`: `GetFormat()`, `GetAudioTrackCount()`, `GetAudioTrackFormat()`, `DecodeNextFrame( CVideoFrame&, IVideoAudioSink* )`, `IsEndOfStream()`, `HasFailed()`, `GetPositionInFrames()`, optional `SeekToFrame()`. Movie files interleave pictures and sound and are mostly read in order, so one call gives one frame: the picture into the frame, the sound stored with it to the sink (`OnVideoAudio( track, frames, frameCount )`).
* `CIVideoCodec`: `OpenSource()` (from a `CIOAccess`, or an access pointer the source keeps alive) and `DecodeMetaData()` ("width", "height", "pixelFormat", "frameRate", "frames", "durationInMs", "audioTracks", per track "audioTrack<n>.sampleRate", ".channels", ".sampleType", plus plugin tags). Registered by type in `CVideoCodecRegistry` (`CVideoGlobal::GetVideoCodecRegistry()`), found by type or by file name.
* There are no built in codecs; formats come from plugins.

### 2.3 Demuxer

The picture is taken by the application, the sound by the audio streaming task; both come from one sequential decoder. `CVideoDemuxer` owns the source:

* `ReadFrame()` gives the oldest queued picture or decodes the next frame; `SkipFrame()` passes one over and tells whether it changed the palette.
* `CreateAudioTrackSource( demuxer, track )` gives an `AUDIO::IAudioSource` for a track. It reads from the queue of its track and decodes further frames when the queue is empty, which queues their pictures for the picture side.
* One lock covers the source and the queues; neither consumer is the real time audio thread.
* The queued pictures are bounded (`SetMaxQueuedFrames()`, 64 by default): when the sound is that far ahead it waits for the pictures to be taken. Tracks without a source are not queued; `SetPictureConsumer( false )` plays only the sound.

### 2.4 Player

`CVideoPlayer` (a `CORE::CNotifier`, used from one thread):

* `Open( TVideoSourcePtr )`, or `Open( codecType, IOAccessPtr )` through the registry. `Close()`.
* `Start( engine, bus, audioTrack )`: with an engine and a track the sound plays as a streamed voice and its position is the clock. Without (no engine, `NoAudioTrack`, a video without sound) the high resolution timer is the clock. When the sound stops moving for a second while playing (a device that stopped), the timer takes over from where the sound was. A movie plays once per `Open()`.
* `Update()`: call it every frame the application draws. It decodes up to the frame that is due; late frames are decoded into the current frame, only the last one is shown, and its palette change flag covers the frames passed over. Returns whether the picture changed.
* `Pause()`, `Resume()`, `Stop()`. The end is `IsEnded()` / `GetEndReason()` (`VIDEOEND_FINISHED`, `VIDEOEND_STOPPED`, `VIDEOEND_FAILED`) and the event `VideoEndedEvent` with the reason. A movie of known length ends after its last frame has been shown for its full time and the sound has played.
* With a cooperative audio engine the player fills the audio stream from `Update()`.

### 2.5 Threads

gucefVIDEO makes no threads. Pictures are decoded on the caller of `Update()`, the sound inside the audio streaming task of gucefAUDIO.

## 3. The codec plugin C interface

Header `include/c_api/gucefVIDEO_c_videocodec_plugin_api.h`, host classes `CVideoCodecCPlugin`, `CVideoCodecCPluginCodec`, `CVideoCodecPluginManager` (plugin type `"GucefVideoCodecPlugin"`, `CVideoGlobal::GetVideoCodecPluginManager()`). It uses the same pieces as the audio codec interface: `TGucefCoreCApi` for logging, `TIOAccess` for the input, `TVariantMapApi` for options and tags, static strings, functions resolved by name, optional functions absent, structures that never change once published. Audio track formats are `TAudioFormatInfo` of the audio codec interface.

```c
#define GUCEF_VIDEOCODEC_PIXELFORMAT_PALETTE8   1   /* 8 bit palette indices, palette of 256 RGB entries */
#define GUCEF_VIDEOCODEC_PIXELFORMAT_RGB24      2
#define GUCEF_VIDEOCODEC_PIXELFORMAT_RGBA32     3
#define GUCEF_VIDEOCODEC_PIXELFORMAT_BGRA32     4
#define GUCEF_VIDEOCODEC_FRAMECOUNT_UNKNOWN     ( ~( (UInt64) 0 ) )

typedef struct SVideoFormatInfo
{
    UInt32 width;
    UInt32 height;
    UInt32 pixelFormat;              /* GUCEF_VIDEOCODEC_PIXELFORMAT_* */
    UInt32 frameRateNumerator;       /* frames per second as a fraction */
    UInt32 frameRateDenominator;
    UInt32 audioTrackCount;
} TVideoFormatInfo;

typedef struct SVideoPictureBuffer   /* the host owns the memory, the plugin writes into it */
{
    void* pixels;                    /* height lines of 'pitch' bytes */
    UInt32 pitch;
    UInt8* palette;                  /* 256 x 3 bytes RGB for PALETTE8, else NULL */
    UInt32 paletteChanged;           /* set by the plugin: the palette changed with this frame */
} TVideoPictureBuffer;

typedef struct SVideoAudioOutput     /* the plugin hands the sound of a frame to the host through this */
{
    TVIDEOCODECPLUGFPTR_OnAudio onAudio;     /* ( userData, track, frames, frameCount ) */
    void* userData;
} TVideoAudioOutput;

UInt32 VIDEOCODECPLUG_Init( void** plugdata, TGucefCoreCApi* libApi );
void   VIDEOCODECPLUG_Shutdown( void** plugdata );
const char* VIDEOCODECPLUG_Name( const void* plugdata );
const char* VIDEOCODECPLUG_Copyright( const void* plugdata );
const TVersion* VIDEOCODECPLUG_Version( const void* plugdata );
UInt32 VIDEOCODECPLUG_Type_Count( const void* plugdata );
const char* VIDEOCODECPLUG_Type( const void* plugdata, UInt32 index );
UInt32 VIDEOCODECPLUG_Create_Codec( void** plugdata, const char* codecType, void** codecdata );
void   VIDEOCODECPLUG_Destroy_Codec( void** plugdata, void** codecdata );

UInt32 VIDEOCODECPLUG_Src_Open( void** plugdata, void** codecdata, void** sourcedata, TIOAccess* input, const TVariantMapApi* options, TVideoFormatInfo* format, UInt64* frameCount );
UInt32 VIDEOCODECPLUG_Src_Get_Audio_Track_Format( void** plugdata, void** codecdata, void** sourcedata, UInt32 track, TAudioFormatInfo* format );
UInt32 VIDEOCODECPLUG_Src_Decode_Frame( void** plugdata, void** codecdata, void** sourcedata, TVideoPictureBuffer* picture, const TVideoAudioOutput* audio, UInt32* endOfStream );
UInt32 VIDEOCODECPLUG_Src_Seek_Frame( void** plugdata, void** codecdata, void** sourcedata, UInt64 frame );     /* optional */
void   VIDEOCODECPLUG_Src_Close( void** plugdata, void** codecdata, void** sourcedata );
UInt32 VIDEOCODECPLUG_Decode_Meta_Data( void** plugdata, void** codecdata, TIOAccess* input, TVariantMapApi* metaData );   /* optional */
```

* `Src_Decode_Frame` returns 1 with `*endOfStream = 1` and nothing written at the end, 1 with a frame otherwise, 0 on an error (the host then ends the source).
* The host keeps the previous picture in the buffer between calls, so formats with delta frames only write what changed.
* Sound comes in the track's format, in pieces of any size, through `onAudio` during the call.
* After a successful `Src_Seek_Frame` the next decoded frame is that frame, completely drawn.
* The host calls a plugin from one thread at a time. Unloading a plugin closes its open sources first; they then end instead of calling into the module.

## 4. Plugins

| plugin | format | notes |
|---|---|---|
| `videocodecpluginSMK` | Smacker (`SMK2`, `SMK4`), type `smk` | C, links nothing from GUCEF, written from the published format description. Palette pictures from 4x4 blocks with Huffman coded block types and colours, palette changes per frame (including copies from the previous palette), the ring frame, doubled and interlaced lines; up to 7 audio tracks, 8 or 16 bit, mono or stereo, compressed (DPCM) or plain. Seeking decodes from the start up to the frame. Not supported: the two extra full block modes of `SMK4` (reported as a decode error) and audio tracks with the later Bink compression (left out with a log line). Verified frame by frame (indices and palette) and on all sound against an independent decoder on a set of 11 commercial movies |

## 5. How to use

```cpp
// load the codec plugins first, e.g. CORE::CPluginControl::AddPluginFromDir( "$MODULEDIR$/videocodecpluginSMK", group, true )
VIDEO::CVideoPlayer player;
player.Open( "smk", movieAccessPtr );                  // or Open( source ) with a source from a codec
player.Start( &audioEngine, movieBus );                // or Start() without sound: the timer is the clock

while ( player.IsPlaying() )                           // once per drawn frame
{
    if ( player.Update() )
        player.GetCurrentFrame().ConvertTo32Bit( IMAGE::PSF_RGBA, screenPixels, screenPitch );
    // draw, handle input: player.Stop() skips
}
```

`VideoEndedEvent` reports the end to observers of the player. To decode without playing, open a source with a codec from `CVideoCodecRegistry` and call `DecodeNextFrame()` with a frame and an optional `IVideoAudioSink`.

## 6. Tests

`tests/gucefVIDEO_TestApp`, clean under ASan and UBSan:

* formats and frames;
* the demuxer and the player with a scripted source: timer clock with a test timer, audio clock on the null audio driver, late frames, pause, stop, failure, the end event, the stall fallback;
* the Smacker plugin on files the test writes with its own encoder: every block type, the caches of the Huffman trees and their reset per frame, palette changes with copies from the previous palette, compressed 8 / 16 bit mono / stereo and plain sound, the ring frame, doubled lines, `SMK4`, damaged and truncated files, meta data, playing through the player, unloading the plugin with a source open.

Opt in: `GUCEF_TEST_SMK_FILE` decodes a real file to its end. With `GUCEF_TEST_SMK_REFERENCE`, every frame and the sound are compared with a reference file: one line per frame, `F <frame> <crc32 of the indices> <crc32 of the palette>`, then one line per audio track, `A <track> <crc32 of the sound> <frames of sound>`.

## 7. Not supported yet

Loading codec plugins on the web, seeking in the player, the Bink audio variant and the extra full block modes of `SMK4`.
