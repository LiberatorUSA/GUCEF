/*
 *  gucefIMAGE: GUCEF module providing image utilities
 *
 *  Copyright (C) 1998 - 2024.  Dinand Vanvelzen
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

#ifndef GUCEF_IMAGE_IMAGETAGS_H
#define GUCEF_IMAGE_IMAGETAGS_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_IMAGE_ETYPES_H
#include "gucefIMAGE_ETypes.h"      /* simple types used */
#define GUCEF_IMAGE_ETYPES_H
#endif /* GUCEF_IMAGE_ETYPES_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
namespace GUCEF {
namespace IMAGE {
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      CONSTANTS                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

/* ==========================================================
// Exif JPEG tags
// ========================================================*/

/* ----------------------------------------------------------
// TIFF Rev. 6.0 Attribute Information Used in Exif
// --------------------------------------------------------*/

/* Tags relating to image data structure */

#define GUCEF_IMAGE_TAG_IMAGE_WIDTH					0x0100
#define GUCEF_IMAGE_TAG_IMAGE_HEIGHT				0x0101
#define GUCEF_IMAGE_TAG_BITS_PER_SAMPLE				0x0102
#define GUCEF_IMAGE_TAG_COMPRESSION					0x0103
#define GUCEF_IMAGE_TAG_PHOTOMETRIC_INTERPRETATION	0x0106
#define GUCEF_IMAGE_TAG_ORIENTATION					0x0112
#define GUCEF_IMAGE_TAG_SAMPLES_PER_PIXEL			0x0115
#define GUCEF_IMAGE_TAG_PLANAR_CONFIGURATION		0x011C
#define GUCEF_IMAGE_TAG_YCBCR_SUBSAMPLING			0x0212
#define GUCEF_IMAGE_TAG_YCBCR_POSITIONING			0x0213
#define GUCEF_IMAGE_TAG_X_RESOLUTION				0x011A
#define GUCEF_IMAGE_TAG_Y_RESOLUTION				0x011B
#define GUCEF_IMAGE_TAG_RESOLUTION_UNIT				0x0128

/* Tags relating to recording offset */

#define GUCEF_IMAGE_TAG_STRIP_OFFSETS					0x0111
#define GUCEF_IMAGE_TAG_ROWS_PER_STRIP					0x0116
#define GUCEF_IMAGE_TAG_STRIP_BYTE_COUNTS				0x0117
#define GUCEF_IMAGE_TAG_JPEG_INTERCHANGE_FORMAT			0x0201
#define GUCEF_IMAGE_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH	0x0202

/* Tags relating to image data characteristics */

#define GUCEF_IMAGE_TAG_TRANSFER_FUNCTION		0x012D
#define GUCEF_IMAGE_TAG_WHITE_POINT				0x013E
#define GUCEF_IMAGE_TAG_PRIMARY_CHROMATICITIES	0x013F
#define GUCEF_IMAGE_TAG_YCBCR_COEFFICIENTS		0x0211
#define GUCEF_IMAGE_TAG_REFERENCE_BLACK_WHITE	0x0214

/* Other tags */

#define GUCEF_IMAGE_TAG_DATETIME 				0x0132
#define GUCEF_IMAGE_TAG_IMAGE_DESCRIPTION 		0x010E
#define GUCEF_IMAGE_TAG_MAKE 					0x010F
#define GUCEF_IMAGE_TAG_MODEL 					0x0110
#define GUCEF_IMAGE_TAG_SOFTWARE 				0x0131
#define GUCEF_IMAGE_TAG_ARTIST 					0x013B
#define GUCEF_IMAGE_TAG_COPYRIGHT 				0x8298

/* ----------------------------------------------------------
// Exif IFD Attribute Information
// --------------------------------------------------------*/

/* Tags relating to version */

#define GUCEF_IMAGE_TAG_EXIF_VERSION 			0x9000
#define GUCEF_IMAGE_TAG_FLASHPIX_VERSION 		0xA000

/* Tag relating to image data characteristics */

#define TGUCEF_IMAGE_TAG_COLOR_SPACE 			0xA001

/* Tags relating to image configuration */

#define GUCEF_IMAGE_TAG_COMPONENTS_CONFIGURATION	0x9101
#define GUCEF_IMAGE_TAG_COMPRESSED_BITS_PER_PIXEL	0x9102
#define GUCEF_IMAGE_TAG_PIXEL_X_DIMENSION			0xA002
#define GUCEF_IMAGE_TAG_PIXEL_Y_DIMENSION			0xA003

/* Tags relating to user information */

#define GUCEF_IMAGE_TAG_MARKER_NOTE		0x927C
#define GUCEF_IMAGE_TAG_USER_COMMENT	0x9286
    
/* Tag relating to related file information */

#define GUCEF_IMAGE_TAG_RELATED_SOUND_FILE			0xA004

/* Tags relating to date and time */

#define GUCEF_IMAGE_TAG_DATETIME_ORIGINAL			0x9003
#define GUCEF_IMAGE_TAG_DATETIME_DIGITIZED			0x9004
#define GUCEF_IMAGE_TAG_SUBSECOND_TIME				0x9290
#define GUCEF_IMAGE_TAG_SUBSECOND_TIME_ORIGINAL		0x9291
#define GUCEF_IMAGE_TAG_SUBSECOND_TIME_DIGITIZED	0x9292

/* Tags relating to picture-taking conditions */

#define GUCEF_IMAGE_TAG_EXPOSURE_TIME				0x829A
#define GUCEF_IMAGE_TAG_FNUMBER						0x829D
#define GUCEF_IMAGE_TAG_EXPOSURE_PROGRAM			0x8822
#define GUCEF_IMAGE_TAG_SPECTRAL_SENSITIVITY		0x8824
#define GUCEF_IMAGE_TAG_ISO_SPEED_RATINGS 			0x8827
#define GUCEF_IMAGE_TAG_OECF						0x8828
#define GUCEF_IMAGE_TAG_SHUTTER_SPEED_VALUE 		0x9201
#define GUCEF_IMAGE_TAG_APERTURE_VALUE 				0x9202
#define GUCEF_IMAGE_TAG_BRIGHTNESS_VALUE			0x9203
#define GUCEF_IMAGE_TAG_EXPOSURE_BIAS_VALUE 		0x9204
#define GUCEF_IMAGE_TAG_MAX_APERTURE_VALUE 			0x9205
#define GUCEF_IMAGE_TAG_SUBJECT_DISTANCE			0x9206
#define GUCEF_IMAGE_TAG_METERING_MODE				0x9207
#define GUCEF_IMAGE_TAG_LIGHT_SOURCE				0x9208
#define GUCEF_IMAGE_TAG_FLASH						0x9209
#define GUCEF_IMAGE_TAG_FOCAL_LENGTH				0x920A
#define GUCEF_IMAGE_TAG_SUBJECT_AREA				0x9214
#define GUCEF_IMAGE_TAG_FLASH_ENERGY				0xA20B
#define GUCEF_IMAGE_TAG_SPATIAL_FREQ_RESPONSE 		0xA20C
#define GUCEF_IMAGE_TAG_FOCAL_PLANE_X_RES			0xA20E
#define GUCEF_IMAGE_TAG_FOCAL_PLANE_Y_RES			0xA20F
#define GUCEF_IMAGE_TAG_FOCAL_PLANE_UNIT			0xA210
#define GUCEF_IMAGE_TAG_SUBJECT_LOCATION 			0xA214
#define GUCEF_IMAGE_TAG_EXPOSURE_INDEX				0xA215
#define GUCEF_IMAGE_TAG_SENSING_METHOD				0xA217
#define GUCEF_IMAGE_TAG_FILE_SOURCE					0xA300
#define GUCEF_IMAGE_TAG_SCENE_TYPE					0xA301
#define GUCEF_IMAGE_TAG_CFA_PATTERN					0xA302
#define GUCEF_IMAGE_TAG_CUSTOM_RENDERED				0xA401
#define GUCEF_IMAGE_TAG_EXPOSURE_MODE				0xA402
#define GUCEF_IMAGE_TAG_WHITE_BALANCE				0xA403
#define GUCEF_IMAGE_TAG_DIGITAL_ZOOM_RATIO			0xA404
#define GUCEF_IMAGE_TAG_FOCAL_LENGTH_IN_35MM_FILM	0xA405
#define GUCEF_IMAGE_TAG_SCENE_CAPTURE_TYPE			0xA406
#define GUCEF_IMAGE_TAG_GAIN_CONTROL				0xA407
#define GUCEF_IMAGE_TAG_CONTRAST					0xA408
#define GUCEF_IMAGE_TAG_SATURATION					0xA409
#define GUCEF_IMAGE_TAG_SHARPNESS					0xA40A
#define GUCEF_IMAGE_TAG_DEVICE_SETTING_DESCRIPTION	0xA40B
#define GUCEF_IMAGE_TAG_SUBJECT_DISTANCE_RANGE		0xA40C

/* Other tags */

#define GUCEF_IMAGE_TAG_IMAGE_UNIQUE_ID				0xA420

/* ----------------------------------------------------------
// GPS Attribute Information
// --------------------------------------------------------*/

#define GUCEF_IMAGE_TAG_GPS_VERSION_ID				0x0000
#define GUCEF_IMAGE_TAG_GPS_LATITUDE_REF			0x0001
#define GUCEF_IMAGE_TAG_GPS_LATITUDE				0x0002
#define GUCEF_IMAGE_TAG_GPS_LONGITUDE_REF			0x0003
#define GUCEF_IMAGE_TAG_GPS_LONGITUDE				0x0004
#define GUCEF_IMAGE_TAG_GPS_ALTITUDE_REF			0x0005
#define GUCEF_IMAGE_TAG_GPS_ALTITUDE				0x0006
#define GUCEF_IMAGE_TAG_GPS_TIME_STAMP				0x0007
#define GUCEF_IMAGE_TAG_GPS_SATELLITES				0x0008
#define GUCEF_IMAGE_TAG_GPS_STATUS					0x0009
#define GUCEF_IMAGE_TAG_GPS_MEASURE_MODE			0x000A
#define GUCEF_IMAGE_TAG_GPS_DOP						0x000B
#define GUCEF_IMAGE_TAG_GPS_SPEED_REF				0x000C
#define GUCEF_IMAGE_TAG_GPS_SPEED					0x000D
#define GUCEF_IMAGE_TAG_GPS_TRACK_REF				0x000E
#define GUCEF_IMAGE_TAG_GPS_TRACK					0x000F
#define GUCEF_IMAGE_TAG_GPS_IMG_DIRECTION_REF		0x0010
#define GUCEF_IMAGE_TAG_GPS_IMG_DIRECTION			0x0011
#define GUCEF_IMAGE_TAG_GPS_MAP_DATUM				0x0012
#define GUCEF_IMAGE_TAG_GPS_DEST_LATITUDE_REF		0x0013
#define GUCEF_IMAGE_TAG_GPS_DEST_LATITUDE			0x0014
#define GUCEF_IMAGE_TAG_GPS_DEST_LONGITUDE_REF		0x0015
#define GUCEF_IMAGE_TAG_GPS_DEST_LONGITUDE			0x0016
#define GUCEF_IMAGE_TAG_GPS_DEST_BEARING_REF		0x0017
#define GUCEF_IMAGE_TAG_GPS_DEST_BEARING			0x0018
#define GUCEF_IMAGE_TAG_GPS_DEST_DISTANCE_REF		0x0019
#define GUCEF_IMAGE_TAG_GPS_DEST_DISTANCE			0x001A
#define GUCEF_IMAGE_TAG_GPS_PROCESSING_METHOD		0x001B
#define GUCEF_IMAGE_TAG_GPS_AREA_INFORMATION		0x001C
#define GUCEF_IMAGE_TAG_GPS_DATE_STAMP				0x001D
#define GUCEF_IMAGE_TAG_GPS_DIFFERENTIAL			0x001E

/* ==========================================================
// IPTC/NAA tags
// ========================================================*/

#define GUCEF_IMAGE_TAG_RECORD_VERSION					0x0200
#define GUCEF_IMAGE_TAG_CAPTION							0x0278
#define GUCEF_IMAGE_TAG_WRITER							0x027A
#define GUCEF_IMAGE_TAG_HEADLINE						0x0269
#define GUCEF_IMAGE_TAG_SPECIAL_INSTRUCTIONS			0x0228
#define GUCEF_IMAGE_TAG_BY_LINE							0x0250
#define GUCEF_IMAGE_TAG_BY_LINE_TITLE					0x0255
#define GUCEF_IMAGE_TAG_CREDIT							0x026E
#define GUCEF_IMAGE_TAG_SOURCE							0x0273
#define GUCEF_IMAGE_TAG_OBJECT_NAME						0x0205
#define GUCEF_IMAGE_TAG_DATE_CREATED					0x0237
#define GUCEF_IMAGE_TAG_CITY							0x025A
#define GUCEF_IMAGE_TAG_PROVINCE_OR_STATE				0x025F
#define GUCEF_IMAGE_TAG_COUNTRY_OR_PRIMARY_LOCATION		0x0265
#define GUCEF_IMAGE_TAG_ORIGINAL_TRANSMISSION_REFERENCE 0x0267
#define GUCEF_IMAGE_TAG_CATEGORY						0x020F
#define GUCEF_IMAGE_TAG_SUPPLEMENTAL_CATEGORIES			0x0214
#define GUCEF_IMAGE_TAG_URGENCY							0x020A
#define GUCEF_IMAGE_TAG_KEYWORDS						0x0219
#define GUCEF_IMAGE_TAG_COPYRIGHT_NOTICE				0x0274
#define GUCEF_IMAGE_TAG_RELEASE_DATE					0x021E
#define GUCEF_IMAGE_TAG_RELEASE_TIME					0x0223
#define GUCEF_IMAGE_TAG_TIME_CREATED					0x023C
#define GUCEF_IMAGE_TAG_ORIGINATING_PROGRAM				0x0241

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifdef __cplusplus
}; /* namespace IMAGE */
}; /* namespace GUCEF */
#endif /* __cplusplus ? */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_IMAGE_IMAGETAGS_H ? */
