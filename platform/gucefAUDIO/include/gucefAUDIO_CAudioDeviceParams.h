/*
 *  gucefAUDIO: GUCEF module providing audio playback, mixing and decoding
 *
 *  Copyright (C) 1998 - 2026.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef GUCEF_AUDIO_CAUDIODEVICEPARAMS_H
#define GUCEF_AUDIO_CAUDIODEVICEPARAMS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_CVALUELIST_H
#include "CValueList.h"
#define GUCEF_CORE_CVALUELIST_H
#endif /* GUCEF_CORE_CVALUELIST_H ? */

#ifndef GUCEF_AUDIO_CAUDIOFORMAT_H
#include "gucefAUDIO_CAudioFormat.h"
#define GUCEF_AUDIO_CAUDIOFORMAT_H
#endif /* GUCEF_AUDIO_CAUDIOFORMAT_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace AUDIO {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

/** How the device is shared with other programs */
enum EAudioShareMode
{
    AUDIOSHAREMODE_SHARED = 0 ,     /**< the system mixes this with the other programs, the default */
    AUDIOSHAREMODE_EXCLUSIVE        /**< this program has the device to itself, for the lowest latency where the platform has it */
};
typedef enum EAudioShareMode TAudioShareMode;

/**
 *  What is asked of a device that is opened. Everything has a default that means 'whatever works best',
 *  so a program that does not care sets nothing. The driver reports what it actually got through the
 *  device it hands back.
 */
class GUCEF_AUDIO_PUBLIC_CPP CAudioDeviceParams
{
    public:

    CAudioDeviceParams( void );

    CAudioDeviceParams( const CAudioDeviceParams& src );

    CAudioDeviceParams& operator=( const CAudioDeviceParams& src );

    ~CAudioDeviceParams();

    /** The device to open as the driver knows it (CAudioDeviceInfo::GetId()), empty for the default device */
    const CString& GetDeviceId( void ) const;
    void SetDeviceId( const CString& deviceId );

    TAudioDirection GetDirection( void ) const;
    void SetDirection( TAudioDirection direction );

    /** The format wanted. An invalid format (the default) or invalid parts of it (rate 0) leave the choice to the device */
    const CAudioFormat& GetPreferredFormat( void ) const;
    void SetPreferredFormat( const CAudioFormat& format );

    /** Size of the pieces the device asks for audio in, in frames. 0 lets the driver pick. Smaller is lower latency and less safe */
    UInt32 GetBufferSizeInFrames( void ) const;
    void SetBufferSizeInFrames( UInt32 frames );

    TAudioShareMode GetShareMode( void ) const;
    void SetShareMode( TAudioShareMode shareMode );

    /** Settings for one specific driver, as name / value pairs. Drivers ignore what they do not know */
    const CORE::CValueList& GetOptions( void ) const;
    CORE::CValueList& GetOptions( void );

    private:

    CString m_deviceId;
    TAudioDirection m_direction;
    CAudioFormat m_preferredFormat;
    UInt32 m_bufferSizeInFrames;
    TAudioShareMode m_shareMode;
    CORE::CValueList m_options;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace AUDIO */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_AUDIO_CAUDIODEVICEPARAMS_H ? */
