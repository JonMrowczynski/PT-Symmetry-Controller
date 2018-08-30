/*********************************************************************************

Copyright (c) 2010, Vernier Software & Technology
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Vernier Software & Technology nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL VERNIER SOFTWARE & TECHNOLOGY BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************************/
#ifndef _NGIO_SOURCE_CMDS_H_
#define _NGIO_SOURCE_CMDS_H_

/***************************************************************************************************/
//
// This file contains declarations for parameter and response structures used by the function 
// GMBLBufferedMeasurementSource::SendCmdAndGetResponse().
//
/***************************************************************************************************/

//
// General NGIO commands include:
//
#define NGIO_CMD_ID_GET_STATUS 0x10
#define NGIO_CMD_ID_START_MEASUREMENTS 0x18
#define NGIO_CMD_ID_STOP_MEASUREMENTS 0x19
#define NGIO_CMD_ID_INIT 0x1A
#define NGIO_CMD_ID_SET_MEASUREMENT_PERIOD 0x1B
#define NGIO_CMD_ID_GET_MEASUREMENT_PERIOD 0x1C
#define NGIO_CMD_ID_SET_LED_STATE 0x1D
#define NGIO_CMD_ID_GET_LED_STATE 0x1E
//#define NGIO_CMD_ID_GET_SERIAL_NUMBER 0x20
#define NGIO_CMD_ID_SET_ANALOG_INPUT 0x21
#define NGIO_CMD_ID_GET_ANALOG_INPUT 0x22
#define NGIO_CMD_ID_WRITE_NV_MEM 0x26	
#define NGIO_CMD_ID_READ_NV_MEM 0x27
#define NGIO_CMD_ID_GET_SENSOR_ID 0x28
#define NGIO_CMD_ID_SET_SAMPLING_MODE 0x29
#define NGIO_CMD_ID_GET_SAMPLING_MODE 0x2A
#define NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK 0x2C
#define NGIO_CMD_ID_GET_SENSOR_CHANNEL_ENABLE_MASK 0x2D
#define NGIO_CMD_ID_SET_COLLECTION_PARAMS 0x2E
#define NGIO_CMD_ID_GET_COLLECTION_PARAMS 0x2F

#define NGIO_CMD_ID_GET_ARCHIVED_RUN_IDS 0x30
#define NGIO_CMD_ID_DELETE_ARCHIVED_RUNS 0x31
#define NGIO_CMD_ID_SET_DIGITAL_COUNTER 0x32
#define NGIO_CMD_ID_GET_DIGITAL_COUNTER 0x33
#define NGIO_CMD_ID_CLEAR_ERROR_FLAGS 0x34
#define NGIO_CMD_ID_ENABLE_SENSOR_ID_NOTIFICATIONS 0x35
#define NGIO_CMD_ID_DISABLE_SENSOR_ID_NOTIFICATIONS 0x36
#define NGIO_CMD_ID_WRITE_IO_CONFIG 0x37
#define NGIO_CMD_ID_READ_IO_CONFIG 0x38
#define NGIO_CMD_ID_WRITE_IO 0x39
#define NGIO_CMD_ID_READ_IO 0x3A
#define NGIO_CMD_ID_START_FRAME_COLLECTION 0x3B
#define NGIO_CMD_ID_STOP_FRAME_COLLECTION 0x3C

/***************************************************************************************************/

//#define NGIO_DEFAULT_CMD_RESPONSE_TIMEOUT_MS 2000

//LabQuest supports NGIO_CHANNEL_ID_ANALOG1 thru NGIO_CHANNEL_ID_BUILT_IN_TEMP.
//LabQuest Mini supports the same channels as LabQuest, except that NGIO_CHANNEL_ID_ANALOG4 and NGIO_CHANNEL_ID_BUILT_IN_TEMP are not supported.
#define NGIO_CHANNEL_ID_TIME 0
#define NGIO_CHANNEL_ID_ANALOG1 1
#define NGIO_CHANNEL_ID_ANALOG2 2
#define NGIO_CHANNEL_ID_ANALOG3 3
#define NGIO_CHANNEL_ID_ANALOG4 4
#define NGIO_CHANNEL_ID_DIGITAL1 5
#define NGIO_CHANNEL_ID_DIGITAL2 6
#define NGIO_CHANNEL_ID_BUILT_IN_TEMP 7
#define NGIO_CHANNEL_ID_ANALOG_EXT1 8
#define NGIO_CHANNEL_ID_ANALOG_EXT2 9

#define NGIO_MAX_NUM_CHANNELS 8
#define NGIO_MAX_NUM_CHANNELS_LQ 8
#define NGIO_MAX_NUM_CHANNELS_LQ_MINI 7

#define NGIO_MAX_NUM_DIGITAL_CHANNELS 2

//Following values are used with NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK lsbyteLsword_EnableSensorChannels:
#define NGIO_CHANNEL_MASK_TIME 1
#define NGIO_CHANNEL_MASK_ANALOG1 2
#define NGIO_CHANNEL_MASK_ANALOG2 4
#define NGIO_CHANNEL_MASK_ANALOG3 8
#define NGIO_CHANNEL_MASK_ANALOG4 0x10
#define NGIO_CHANNEL_MASK_DIGITAL1 0x20
#define NGIO_CHANNEL_MASK_DIGITAL2 0x40
#define NGIO_CHANNEL_MASK_BUILT_IN_TEMP 0x80

#define NGIO_CHANNEL_MASK_ALL_NORMAL_SENSORS 0xfe
#define NGIO_CHANNEL_MASK_ALL_NORMAL_ANALOG_SENSORS 0x1e

//Following values are used with NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK msbyteLsword_EnableSensorChannels:
#define NGIO_CHANNEL_MASK_ANALOG_EXT1 0x1
#define NGIO_CHANNEL_MASK_ANALOG_EXT2 0x2

//
/***************************************************************************************************/
//
// NGIO_DEVTYPE_LABQUEST_AUDIO devices use the following channels:

#define NGIO_CHANNEL_ID_AUDIO_INTERNAL 1
//NGIO_CHANNEL_ID_AUDIO_EXTERNAL_LEFT and NGIO_CHANNEL_ID_AUDIO_EXTERNAL_RIGHT are not supported yet.
#define NGIO_CHANNEL_ID_AUDIO_EXTERNAL_LEFT 2
#define NGIO_CHANNEL_ID_AUDIO_EXTERNAL_RIGHT 3

#define NGIO_MAX_NUM_AUDIO_CHANNELS 4

#define NGIO_CHANNEL_ID_MASK_AUDIO_INTERNAL 2
//NGIO_CHANNEL_ID_AUDIO_EXTERNAL_LEFT and NGIO_CHANNEL_ID_AUDIO_EXTERNAL_RIGHT are not supported yet.
#define NGIO_CHANNEL_ID_MASK_AUDIO_EXTERNAL_LEFT 4
#define NGIO_CHANNEL_ID_MASK_AUDIO_EXTERNAL_RIGHT 8

// NGIO_DEVTYPE_LABQUEST_AUDIO devices support the following commands:
//NGIO_CMD_ID_GET_STATUS
//NGIO_CMD_ID_START_MEASUREMENTS
//NGIO_CMD_ID_STOP_MEASUREMENTS
//NGIO_CMD_ID_SET_MEASUREMENT_PERIOD 
//NGIO_CMD_ID_GET_MEASUREMENT_PERIOD 
//NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK 
//NGIO_CMD_ID_GET_SENSOR_CHANNEL_ENABLE_MASK 

//
/***************************************************************************************************/
//
// NGIO_DEVTYPE_LABQUEST_BUILTIN_ACCELEROMETER devices use the following channels:

#define NGIO_CHANNEL_ID_BUILTIN_ACC_X 1
#define NGIO_CHANNEL_ID_BUILTIN_ACC_Y 2
#define NGIO_CHANNEL_ID_BUILTIN_ACC_Z 3

#define NGIO_MAX_NUM_BUILTIN_ACC_CHANNELS 4

#define NGIO_CHANNEL_ID_MASK_BUILTIN_ACC_X 2
#define NGIO_CHANNEL_ID_MASK_BUILTIN_ACC_Y 4
#define NGIO_CHANNEL_ID_MASK_BUILTIN_ACC_Z 8

// NGIO_DEVTYPE_LABQUEST_BUILTIN_ACCELEROMETER devices support the following commands:
//NGIO_CMD_ID_GET_STATUS
//NGIO_CMD_ID_START_MEASUREMENTS
//NGIO_CMD_ID_STOP_MEASUREMENTS
//NGIO_CMD_ID_SET_MEASUREMENT_PERIOD 
//NGIO_CMD_ID_GET_MEASUREMENT_PERIOD 
//NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK 
//NGIO_CMD_ID_GET_SENSOR_CHANNEL_ENABLE_MASK 

//
/***************************************************************************************************/
//
// NGIO_DEVTYPE_LABQUEST_BUILTIN_LIGHT_SENSOR devices use the following channels:

#define NGIO_CHANNEL_ID_BUILTIN_LUX 1

#define NGIO_MAX_NUM_BUILTIN_LUX_CHANNELS 2

#define NGIO_CHANNEL_ID_MASK_BUILTIN_LUX 2

// NGIO_DEVTYPE_LABQUEST_BUILTIN_LIGHT_SENSOR devices support the following commands:
//NGIO_CMD_ID_GET_STATUS
//NGIO_CMD_ID_START_MEASUREMENTS
//NGIO_CMD_ID_STOP_MEASUREMENTS
//NGIO_CMD_ID_SET_MEASUREMENT_PERIOD 
//NGIO_CMD_ID_GET_MEASUREMENT_PERIOD 
//NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK 
//NGIO_CMD_ID_GET_SENSOR_CHANNEL_ENABLE_MASK 

//
/***************************************************************************************************/
//

#if defined (TARGET_OS_WIN)
#pragma pack(push)
#pragma pack(1)
#endif

#ifdef TARGET_OS_MAC
#pragma pack(1)
#endif

//This is unfortunate, but gcc 3.x does not support pragma pack(gcc 4.x does!).
//We are stuck with gcc 3.x for now, so we use _XPACK1 .
//Note that some docs on the web mentioned problems with using typedefs and
//__attribute__, so we are putting the typedef on a separate line.
#ifndef _XPACK1

#ifdef TARGET_OS_LINUX
#define _XPACK1 __attribute__((__packed__))
#elif defined(TARGET_OS_DAQ)

#ifndef _X86
#define _XPACK1 __attribute__((packed))
#else
#define _XPACK1
#endif

#else
#define _XPACK1
#endif

#endif

// The structures below define the parameter and response blocks associated with the commands defined above.
// Pointers to these structures are passed in to SendCmdAndGetResponse().
// If no parameter structure is defined for a command, then the command does not take parameters.
// If no response structure is defined for a command, then the only response associated with the command is NGIODefaultCmdResponse.

#define NGIO_STATUS_SUCCESS 0
#define NGIO_STATUS_NOT_READY_FOR_NEW_CMD 0x30
#define NGIO_STATUS_CMD_NOT_SUPPORTED 0x31
#define NGIO_STATUS_INTERNAL_ERROR1 0x32
#define NGIO_STATUS_INTERNAL_ERROR2 0x33
#define NGIO_STATUS_ERROR_CANNOT_CHANGE_PERIOD_WHILE_COLLECTING 0x34
#define NGIO_STATUS_ERROR_CANNOT_READ_NV_MEM_BLK_WHILE_COLLECTING_FAST 0x35
#define NGIO_STATUS_ERROR_INVALID_PARAMETER 0x36
#define NGIO_STATUS_ERROR_CANNOT_WRITE_FLASH_WHILE_COLLECTING 0x37
#define NGIO_STATUS_ERROR_CANNOT_WRITE_FLASH_WHILE_HOST_FIFO_BUSY 0x38
#define NGIO_STATUS_ERROR_OP_BLOCKED_WHILE_COLLECTING 0x39
#define NGIO_STATUS_ERROR_CALCULATOR_CANNOT_MEASURE_WITH_NO_BATTERIES 0x3A
#define NGIO_STATUS_ERROR_OP_NOT_SUPPORTED_IN_CURRENT_MODE 0x3B
#define NGIO_STATUS_ERROR_AUDIO_CONTROL_FAILURE 0x3C
#define NGIO_STATUS_ERROR_AUDIO_STREAM_FAILURE 0x3D
#define NGIO_STATUS_ERROR_CANNOT_REALLOCATE_FRAME_BUFFERS 0x3E
#define NGIO_STATUS_ERROR_COMMUNICATION 0x41
#define NGIO_STATUS_INTERNAL_ERROR3 0x48
#define NGIO_STATUS_INTERNAL_ERROR4 0x49

//If SendCmdAndGetResponse() returns kResponse_Error and (1 == *pnRespBytes), then *pRespBuf contains
//NGIODefaultCmdResponse, even if a different response structure is defined for the command.
//Note that if you only need to know whether or not SendCmdAndGetResponse() fails, but you do not care about why,
//then you can always set the pRespBuf paramter to NULL.
struct tagNGIODefaultCmdResponse
{
	unsigned char status;//NGIO_STATUS_...
} _XPACK1;
typedef struct tagNGIODefaultCmdResponse NGIODefaultCmdResponse;

struct tagNGIO_LE_DWORD		//Little endian
{
	unsigned char lsbyteLsword;
	unsigned char msbyteLsword;
	unsigned char lsbyteMsword;
	unsigned char msbyteMsword;
} _XPACK1; 
typedef struct tagNGIO_LE_DWORD NGIO_LE_DWORD;

/***************************************************************************************************/
//NGIO_CMD_ID_SET_MEASUREMENT_PERIOD:
//NGIO_CMD_ID_GET_MEASUREMENT_PERIOD:
/*	The unit used for measurement periods and collection durations is a 'tick'. Each class of of hardware may define
	the meaning of a tick differently. For NGI, a 'tick' is one microsecond.										*/
struct tagNGIOSetMeasurementPeriodParams
{
	signed char channel;	/* -1 => all channels.	*/
	unsigned char lsbyteLswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char msbyteLswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char lsbyteMswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char msbyteMswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char lsbyteLswordMeasurementPeriod;
	unsigned char msbyteLswordMeasurementPeriod;
	unsigned char lsbyteMswordMeasurementPeriod;
	unsigned char msbyteMswordMeasurementPeriod;
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_CMD_ID_SET_MEASUREMENT_PERIOD.	*/
typedef struct tagNGIOSetMeasurementPeriodParams NGIOSetMeasurementPeriodParams;

struct tagNGIOGetMeasurementPeriodParams
{
	signed char channel;	/* -1 => all channels.	*/
	unsigned char lsbyteLswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char msbyteLswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char lsbyteMswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char msbyteMswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_CMD_ID_GET_MEASUREMENT_PERIOD.	*/
typedef struct tagNGIOGetMeasurementPeriodParams NGIOGetMeasurementPeriodParams;

typedef NGIOSetMeasurementPeriodParams NGIOGetMeasurementPeriodCmdResponsePayload;//This is the response payload returned by GetNextResponse() 
																					//after sending NGIO_CMD_ID_GET_MEASUREMENT_PERIOD.

/***************************************************************************************************/
// The LED commands are supported by the LabQuest Mini, but not the LabQuest.
//NGIO_CMD_ID_SET_LED_STATE:	
//NGIO_CMD_ID_GET_LED_STATE:

#define NGIO_LED_COLOR_BLACK 0xC0
#define NGIO_LED_COLOR_RED 0x40
#define NGIO_LED_COLOR_GREEN 0x80
#define NGIO_LED_COLOR_RED_GREEN 0
#define NGIO_LED_BRIGHTNESS_MIN 0
#define NGIO_LED_BRIGHTNESS_MAX 0x10
#define NGIO_LED_COLOR_ORANGE NGIO_LED_COLOR_RED_GREEN
#define NGIO_LED_BRIGHTNESS_ORANGE 4

struct tagNGIOSetLedStateParams
{
	unsigned char LEDchannel;		//This parameter is ignored by the LabQuest Mini.
	unsigned char color;
	unsigned char brightness;
} _XPACK1; //Parameter block passed into SendCmd() with NGIO_CMD_ID_SET_LED_STATE.
typedef struct tagNGIOSetLedStateParams NGIOSetLedStateParams;

struct tagNGIOGetLedStateParams
{
	unsigned char LEDchannel;
} _XPACK1; //Parameter block passed into SendCmd() with NGIO_CMD_ID_GET_LED_STATE.
typedef struct tagNGIOGetLedStateParams NGIOGetLedStateParams;

typedef NGIOSetLedStateParams NGIOGetLedStateCmdResponsePayload; //This is the response payload returned by GetNextResponse() after sending NGIO_CMD_ID_GET_LED_STATE.

/***************************************************************************************************/
//NGIO_CMD_ID_GET_STATUS:
#define NGIO_MASK_STATUS_ERROR_CMD_NOT_RECOGNIZED 1
#define NGIO_MASK_STATUS_ERROR_CMD_IGNORED 2
#define NGIO_MASK_STATUS_ADC_UNCALIBRATED 4
#define NGIO_MASK_STATUS_AUTOID_UNCALIBRATED 8
#define NGIO_MASK_STATUS_ERROR_INTERNAL_ERROR1 0X10
#define NGIO_MASK_STATUS_ERROR_AUDIO_CONTROL_FAILURE 0X10
#define NGIO_MASK_STATUS_ERROR_INTERNAL_ERROR2 0X20
#define NGIO_MASK_STATUS_ERROR_AUDIO_STREAM_FAILURE 0X20
#define NGIO_MASK_STATUS_ERROR_MASTER_FIFO_OVERFLOW 0X40
#define NGIO_MASK_STATUS_ERROR_DIGITAL_TRANSITION_LOST 0X80

struct tagNGIOGetStatusCmdResponsePayload
{
	unsigned char status;	//See NGIO_MASK_STATUS_*.
	unsigned char minorVersionMasterCPU;	//Binary coded decimal
	unsigned char majorVersionMasterCPU;	//Binary coded decimal
	unsigned char minorVersionSlaveCPU;		//Binary coded decimal - updated only by Skip and Cyclops, not by Jonah
	unsigned char majorVersionSlaveCPU;		//Binary coded decimal - updated only by Skip and Cyclops, not by Jonah
} _XPACK1; //This is the response payload returned by GetNextResponse() after sending NGIO_CMD_ID_GET_STATUS.
typedef struct tagNGIOGetStatusCmdResponsePayload NGIOGetStatusCmdResponsePayload;

/***************************************************************************************************/

/***************************************************************************************************/
//NGIO_CMD_ID_READ_NV_MEM:
//NGIO_CMD_ID_WRITE_NV_MEM:

#define NGIO_NVMEM_CHANNEL_ID1 100
#define NGIO_NVMEM_CHANNEL_ID2 101
#define NGIO_NVMEM_CHANNEL_ID1_MEMSIZE 250
#define NGIO_NVMEM_CHANNEL_ID2_MEMSIZE 750

struct tagNGIOSerialNumber
{
	unsigned char ww;			//week in year(starting at 1) in BCD format
	unsigned char yy;			//last two digits of year in BCD format
	unsigned char lsbyteLswordSerialCounter;
	unsigned char msbyteLswordSerialCounter;
	unsigned char lsbyteMswordSerialCounter;
	unsigned char msbyteMswordSerialCounter;
} _XPACK1;
typedef struct tagNGIOSerialNumber NGIOSerialNumber;

//Calibrated ADC value = ((rawADCValue*Multiplier)/Divisor) + Offset.
//ADC values, Multiplier, and Divisor are gtype_uint16(in range 0 to 0xffff).
//Offset is gtype_int16.
//gtype_int32 is used to hold calculated calibration value, which is then clipped to the range 0 to 0xffff .
struct tagNGIO_DAQ_channelCalibrationParams
{
	unsigned char lsbyteOffset;
	unsigned char msbyteOffset;
	unsigned char lsbyteMultiplier;
	unsigned char msbyteMultiplier;
	unsigned char lsbyteDivisor;
	unsigned char msbyteDivisor;
} _XPACK1;
typedef struct tagNGIO_DAQ_channelCalibrationParams NGIO_DAQ_channelCalibrationParams;

struct tagNGIO_DAQ_CalibrationParams
{
	//channelParams[0] corr. to NGIO_CHANNEL_ID_ANALOG1, channelParams[1] corr. to NGIO_CHANNEL_ID_ANALOG2, etc.
	NGIO_DAQ_channelCalibrationParams channelParams[NGIO_CHANNEL_ID_DIGITAL2 - NGIO_CHANNEL_ID_ANALOG1 + 1];
} _XPACK1;
typedef struct tagNGIO_DAQ_CalibrationParams NGIO_DAQ_CalibrationParams;

#define NGIO_CALIB_VERSION_FIRST 1
#define NGIO_CALIB_VERSION_10V 2
#define NGIO_CALIB_VERSION_REF_OHMS 3

struct tagNGIO_NVMEM_CHANNEL_ID1_rec
{
	unsigned char version;	//NGIO_CALIB_VERSION_...
	unsigned char spare1;
	unsigned char spare2;
	unsigned char spare3;
	NGIOSerialNumber serialNumber;
	NGIO_DAQ_CalibrationParams calibrationParams;
	NGIO_DAQ_CalibrationParams calibrationParams2;	//used for +/- 10 volt calibration.
	NGIO_DAQ_CalibrationParams calibrationParams3;	//used to calibrate resistive sensors, eg. stainless steel temperature.
} _XPACK1;
typedef struct tagNGIO_NVMEM_CHANNEL_ID1_rec NGIO_NVMEM_CHANNEL_ID1_rec;
//Use offsetof(NGIO_NVMEM_CHANNEL_ID1_rec, field) to calculate the address of field in NGIO_NVMEM_CHANNEL_ID1_rec.

struct tagNGIOWriteNVMemParams
{
	signed char channel;	//NGIO_CHANNEL_ID_ANALOGn => access DDS record on sensor. DDS addr range is 0 to 127.
							//NGIO_NVMEM_CHANNEL_IDn => access flash on DAQ. DAQ flash addr range is 0 to (NGIO_NVMEM_CHANNEL_IDn_MEMSIZE - 1).
	unsigned char lsbyteLswordAddr;
	unsigned char msbyteLswordAddr;
	unsigned char lsbyteMswordAddr;
	unsigned char msbyteMswordAddr;
	unsigned char count;
	unsigned char payload[1];	//The size of this payload varies. 
} _XPACK1; //Used with NGIO_CMD_ID_WRITE_NV_MEM.
typedef struct tagNGIOWriteNVMemParams NGIOWriteNVMemParams;

struct tagNGIOReadNVMemParams
{
	signed char channel;	//NGIO_CHANNEL_ID_ANALOGn => access DDS record on sensor. DDS addr range is 0 to 127.
							//NGIO_NVMEM_CHANNEL_IDn => access flash on DAQ. DAQ flash addr range is 0 to (NGIO_NVMEM_CHANNEL_IDn_MEMSIZE - 1).
	unsigned char lsbyteLswordAddr;
	unsigned char msbyteLswordAddr;
	unsigned char lsbyteMswordAddr;
	unsigned char msbyteMswordAddr;
	unsigned char lsByteCount;		//Reads can span many blobs.
	unsigned char msByteCount;
} _XPACK1; //Used with NGIO_CMD_ID_READ_NV_MEM.
typedef struct tagNGIOReadNVMemParams NGIOReadNVMemParams;

struct tagNGIOReadNVMemCmdResponsePayload
{
	unsigned char payload[1];	//The size of this payload varies. 
} _XPACK1; //Used with NGIO_CMD_ID_READ_NV_MEM.
typedef struct tagNGIOReadNVMemCmdResponsePayload NGIOReadNVMemCmdResponsePayload;

/***************************************************************************************************/
//NGIO_CMD_ID_GET_SENSOR_ID:

struct tagNGIOGetSensorIdParams
{
	signed char channel;	//NGIO_CHANNEL_ID_ANALOG1 .. NGIO_CHANNEL_ID_DIGITAL2
} _XPACK1;			//Parameter block passed into SendCmd() with NGIO_CMD_ID_GET_SENSOR_ID.
typedef struct tagNGIOGetSensorIdParams NGIOGetSensorIdParams;

struct tagNGIOGetSensorIdCmdResponsePayload
{
	unsigned char lsbyteLswordSensorId;
	unsigned char msbyteLswordSensorId;
	unsigned char lsbyteMswordSensorId;
	unsigned char msbyteMswordSensorId;
} _XPACK1; //This is the response payload returned by GetNextResponse() after sending NGIO_CMD_ID_GET_SENSOR_ID.

typedef struct tagNGIOGetSensorIdCmdResponsePayload NGIOGetSensorIdCmdResponsePayload;

struct tagNGIOGetSensorIdCmdResponseExtPayload
{
	unsigned char lsbyteLswordSensorId;
	unsigned char msbyteLswordSensorId;
	unsigned char lsbyteMswordSensorId;
	unsigned char msbyteMswordSensorId;
	unsigned char lsbyteLswordSIDSignature;//This increments every time the Sensor Id for the specified channel changes.
	unsigned char msbyteLswordSIDSignature;
	unsigned char lsbyteMswordSIDSignature;
	unsigned char msbyteMswordSIDSignature;
} _XPACK1; //This is the response payload returned by GetNextResponse() after sending NGIO_CMD_ID_GET_SENSOR_ID.

//NGIOGetSensorIdCmdResponsePayloadExt is only supported in DAQ firmware versions >= 1.26 .
typedef struct tagNGIOGetSensorIdCmdResponseExtPayload NGIOGetSensorIdCmdResponseExtPayload;

struct tagNGIOGetSensorIdCmdResponseExtListPayload
{
	NGIOGetSensorIdCmdResponseExtPayload sensorElement[NGIO_CHANNEL_ID_DIGITAL2 + 1];
} _XPACK1; //This is the response payload returned by GetNextResponse() after sending NGIO_CMD_ID_GET_SENSOR_ID with channel param == -1.

//NGIOGetSensorIdCmdResponsePayloadExtList is only supported in DAQ firmware versions >= 1.26 .
//NGIOGetSensorIdCmdResponseExtListPayload.sensorElement[0] is unused.
//NGIOGetSensorIdCmdResponseExtListPayload.sensorElement[NGIO_CHANNEL_ID_ANALOG1] is the first element with useful info.
typedef struct tagNGIOGetSensorIdCmdResponseExtListPayload NGIOGetSensorIdCmdResponseExtListPayload;

/***************************************************************************************************/
//NGIO_CMD_ID_SET_ANALOG_INPUT:
//NGIO_CMD_ID_GET_ANALOG_INPUT:

//NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC is the default value for all analog channels(set by NGIO_CMD_ID_INIT).
#define NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC 0
#define NGIO_ANALOG_INPUT_5V_BUILTIN_10BIT_ADC 1
#define NGIO_ANALOG_INPUT_5V_EXT_12BIT_ADC 2
#define NGIO_ANALOG_INPUT_5V_EXT_16BIT_ADC 3
#define NGIO_ANALOG_INPUT_PM10V_BUILTIN_12BIT_ADC 4
#define NGIO_ANALOG_INPUT_PM10V_BUILTIN_10BIT_ADC 5
#define NGIO_ANALOG_INPUT_PM10V_EXT_12BIT_ADC 6
#define NGIO_ANALOG_INPUT_PM10V_EXT_16BIT_ADC 7

struct tagNGIOSetAnalogInputParams
{
	signed char channel;	//NGIO_CHANNEL_ID_ANALOG1 ...
	unsigned char analogInput;	//NGIO_ANALOG_INPUT_... 
} _XPACK1;	//Parameter block passed into SendCmd() with NGIO_CMD_ID_SET_ANALOG_INPUT.
typedef struct tagNGIOSetAnalogInputParams NGIOSetAnalogInputParams;

typedef NGIOGetSensorIdParams NGIOGetAnalogInputParams;
typedef NGIOSetAnalogInputParams NGIOGetAnalogInputCmdResponsePayload;

/***************************************************************************************************/
//NGIO_CMD_ID_SET_SAMPLING_MODE:
//NGIO_CMD_ID_GET_SAMPLING_MODE:

#define NGIO_SAMPLING_MODE_PERIODIC_LEVEL_SNAPSHOT 0
#define NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT 1
#define NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT 2
#define NGIO_SAMPLING_MODE_PERIODIC_MOTION_DETECT 3
#define NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER 4
#define NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4 5
#define NGIO_SAMPLING_MODE_CUSTOM 6
//Note that on the LabQuest NGIO_CMD_ID_WRITE_IO_CONFIG and NGIO_CMD_ID_WRITE_IO are disabled unless sampling mode is NGIO_SAMPLING_MODE_CUSTOM.

struct tagNGIOSetSamplingModeParams
{
	signed char channel;	//NGIO_CHANNEL_ID_ANALOG1 ...
	unsigned char samplingMode;//NGIO_CMD_ID_INIT causes samplingMode to be NGIO_SAMPLING_MODE_PERIODIC_LEVEL_SNAPSHOT
							//for analog channels, and NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT for digital channels.
} _XPACK1;	//Parameter block passed into SendCmd() with NGIO_CMD_ID_SET_SAMPLING_MODE.
typedef struct tagNGIOSetSamplingModeParams NGIOSetSamplingModeParams;

typedef NGIOGetSensorIdParams NGIOGetSamplingModeParams;
typedef NGIOSetSamplingModeParams NGIOGetSamplingModeCmdResponsePayload;

/***************************************************************************************************/
//NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK:
//NGIO_CMD_ID_GET_SENSOR_CHANNEL_ENABLE_MASK:

struct tagNGIOSetSensorChannelEnableMaskParams
{
	unsigned char lsbyteLsword_EnableSensorChannels;//Enable bits for sensor channels 0 thru 7. Channel 0 is usually reserved for time.
	unsigned char msbyteLsword_EnableSensorChannels;//Enable bits for sensor channels 8 thru 15.
	unsigned char lsbyteMsword_EnableSensorChannels;//Enable bits for sensor channels 16 thru 23.
	unsigned char msbyteMsword_EnableSensorChannels;//Enable bits for sensor channels 24 thru 31.
} _XPACK1; //Used with NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK.
typedef struct tagNGIOSetSensorChannelEnableMaskParams NGIOSetSensorChannelEnableMaskParams;

//This is the response payload returned by GetNextResponse() after sending NGIO_CMD_ID_GET_SENSOR_CHANNEL_ENABLE_MASK.
typedef NGIOSetSensorChannelEnableMaskParams NGIOGetSensorChannelEnableMaskCmdResponsePayload;

/***************************************************************************************************/
//NGIO_CMD_ID_SET_DIGITAL_COUNTER:
//NGIO_CMD_ID_GET_DIGITAL_COUNTER:
//These commands only work when the sampling mode for the channel is set to NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT,
//NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER, or NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4.

struct tagNGIOSetDigitalCounterParams
{
	signed char channel;	//NGIO_CHANNEL_ID_DIGITAL1 or NGIO_CHANNEL_ID_DIGITAL2
	unsigned char lsbyteLsword_counter;
	unsigned char msbyteLsword_counter;
	unsigned char lsbyteMsword_counter;
	unsigned char msbyteMsword_counter;
} _XPACK1;	//Parameter block passed into SendCmd() with NGIO_CMD_ID_SET_DIGITAL_COUNTER.
typedef struct tagNGIOSetDigitalCounterParams NGIOSetDigitalCounterParams;

typedef NGIOGetSensorIdParams NGIOGetDigitalCounterParams;
typedef NGIOSetDigitalCounterParams NGIOGetDigitalCounterCmdResponsePayload;

/***************************************************************************************************/
//NGIO_CMD_ID_WRITE_IO_CONFIG:
//NGIO_CMD_ID_READ_IO_CONFIG:
//Note that on the LabQuest NGIO_CMD_ID_WRITE_IO_CONFIG and NGIO_CMD_ID_WRITE_IO are disabled unless sampling mode is NGIO_SAMPLING_MODE_CUSTOM.
//When sampling mode is configured as NGIO_SAMPLING_MODE_CUSTOM, all the IO lines are initially configured as inputs.
//Before configuring an IO line as an output with NGIO_CMD_ID_WRITE_IO_CONFIG, it is recommended that you first send
//a NGIO_CMD_ID_WRITE_IO command to set the state of the output to the level specified in the outputLatch parameter.
#define NGIO_MASK_DGX_LINE1 1
#define NGIO_MASK_DGX_LINE2 2
#define NGIO_MASK_DGX_LINE3 4
#define NGIO_MASK_DGX_LINE4 8

struct tagNGIOWriteIOConfigParams
{
	signed char channel;	//NGIO_CHANNEL_ID_DIGITAL1 or NGIO_CHANNEL_ID_DIGITAL2
	unsigned char mask;		//[NGIO_MASK_DGX_LINE1] | .. | [NGIO_MASK_DGX_LINE4]. Only configure line n if NGIO_MASK_DGX_LINEn set in mask.
	unsigned char IO;		//NGIO_MASK_DGX_LINEn bit set => line n is an input, else line n is an output.
	unsigned char open_collector;	//NGIO_MASK_DGX_LINEn bit set here => open collector if line is also configured as output.
} _XPACK1;	//Parameter block passed into SendCmd() with NGIO_CMD_ID_WRITE_IO_CONFIG.
typedef struct tagNGIOWriteIOConfigParams NGIOWriteIOConfigParams;

typedef NGIOGetSensorIdParams NGIOReadIOConfigParams;	//Use with NGIO_CMD_ID_READ_IO_CONFIG.
typedef NGIOWriteIOConfigParams NGIOReadIOConfigCmdResponsePayload;//All mask bits are always set when used with NGIO_CMD_ID_READ_IO_CONFIG.

/***************************************************************************************************/
//#define NGIO_CMD_ID_WRITE_IO:
//#define NGIO_CMD_ID_READ_IO:

struct tagNGIOWriteIOParams
{
	signed char channel;	//NGIO_CHANNEL_ID_DIGITAL1 or NGIO_CHANNEL_ID_DIGITAL2
	unsigned char mask;		//[NGIO_MASK_DGX_LINE1] | .. | [NGIO_MASK_DGX_LINE4]. Only modify line n if NGIO_MASK_DGX_LINEn set in mask.
	unsigned char outputLatch;	//This contains the logical output levels.
	unsigned char IOLive;	//This contains the actual level on the IO pin. Use this to read input levels. NGIO_CMD_ID_WRITE_IO ignores this param.
} _XPACK1;	//Parameter block passed into SendCmd() with NGIO_CMD_ID_WRITE_IO, response block for NGIO_CMD_ID_READ_IO.
typedef struct tagNGIOWriteIOParams NGIOWriteIOParams;

typedef NGIOGetSensorIdParams NGIOReadIOParams;	//Use with NGIO_CMD_ID_READ_IO.
typedef NGIOWriteIOParams NGIOReadIOCmdResponsePayload;//All mask bits are always set when used with NGIO_CMD_ID_READ_IO.

/***************************************************************************************************/
//NGIO_CMD_ID_SET_COLLECTION_PARAMS:
//This command is currently only supported with framed collection. See NGIO_Device_Frm_AllocateCollectionFrames().

#define NGIO_TRIGGER_MASK_BUTTON 1
#define NGIO_TRIGGER_MASK_CHANNEL 2
#define NGIO_TRIGGER_MASK_IMMEDIATE 4

#define NGIO_TRIGGER_CHAN_MODE_MEAS_ABOVE_THRESHOLD 0
#define NGIO_TRIGGER_CHAN_MODE_MEAS_BELOW_THRESHOLD 1
#define NGIO_TRIGGER_CHAN_MODE_LO_TO_HI_MEAS 2
#define NGIO_TRIGGER_CHAN_MODE_HI_TO_LO_MEAS 3

#define NGIO_COLLECTION_FLAGS_MASK_MULTIPLE_PERIODS 4 // not all channels collect data at the same rate.

/*	The unit used for measurement periods and collection durations is a 'tick'. Each class of of hardware may define
	the meaning of a tick differently. For LabQuest and LabQuest Mini, a 'tick' is one microsecond.										*/

struct tagNGIOSetCollectionParams
{
	unsigned char lsbyteLswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char msbyteLswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char lsbyteMswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char msbyteMswordDataRunId;	/* ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.	*/
	unsigned char triggerMask;				/* NGIO_TRIGGER_MASK_BUTTON | NGIO_TRIGGER_MASK_CHANNEL	... */
	unsigned char triggerChannelMode;		/* NGIO_TRIGGER_CHAN_MODE_...	*/
	unsigned char triggerChannel;			/* Used if NGIO_TRIGGER_MASK_CHANNEL is set in triggerMask.	*/
	unsigned char flags;					/* NGIO_COLLECTION_FLAGS_MASK_...	*/
	unsigned char lsbyteLswordTriggerThreshold;
	unsigned char msbyteLswordTriggerThreshold;
	unsigned char lsbyteMswordTriggerThreshold;
	unsigned char msbyteMswordTriggerThreshold;
	unsigned char lsbyteLswordTriggerHysteresis;//NGIO_TRIGGER_CHAN_MODE_LO_TO_HI_MEAS => signal must be < (threshold - hysteresis) before
	unsigned char msbyteLswordTriggerHysteresis;//	testing against threshold.
	unsigned char lsbyteMswordTriggerHysteresis;//NGIO_TRIGGER_CHAN_MODE_HI_TO_LO_MEAS => signal must be > (threshold + hysteresis) before
	unsigned char msbyteMswordTriggerHysteresis;//	testing against threshold.
	unsigned char lsbyteLswordLsdwordMinInterframeDelay;	/* Min time between successive frame triggers in 'ticks'. */
	unsigned char msbyteLswordLsdwordMinInterframeDelay;	/* Min time between successive frame triggers in 'ticks'. */
	unsigned char lsbyteMswordLsdwordMinInterframeDelay;	/* Min time between successive frame triggers in 'ticks'. */
	unsigned char msbyteMswordLsdwordMinInterframeDelay;	/* Min time between successive frame triggers in 'ticks'. */
	unsigned char lsbyteLswordMsdwordMinInterframeDelay;	/* Min time between successive frame triggers in 'ticks'. */
	unsigned char msbyteLswordMsdwordMinInterframeDelay;	/* Min time between successive frame triggers in 'ticks'. */
	unsigned char lsbyteMswordMsdwordMinInterframeDelay;	/* Min time between successive frame triggers in 'ticks'. */
	unsigned char msbyteMswordMsdwordMinInterframeDelay;	/* Min time between successive frame triggers in 'ticks'. */
	unsigned char lsbyteLswordLsdwordCollectionDuration;	/* Length of collection in 'ticks'.	*/
	unsigned char msbyteLswordLsdwordCollectionDuration;	/* Length of collection in 'ticks'.	*/
	unsigned char lsbyteMswordLsdwordCollectionDuration;	/* Length of collection in 'ticks'.	*/
	unsigned char msbyteMswordLsdwordCollectionDuration;	/* Length of collection in 'ticks'.	*/
	unsigned char lsbyteLswordMsdwordCollectionDuration;	/* Length of collection in 'ticks'.	*/
	unsigned char msbyteLswordMsdwordCollectionDuration;	/* Length of collection in 'ticks'.	*/
	unsigned char lsbyteMswordMsdwordCollectionDuration;	/* Length of collection in 'ticks'.	*/
	unsigned char msbyteMswordMsdwordCollectionDuration;	/* Length of collection in 'ticks'.	*/
	unsigned char lsbyteLswordLsdwordPreTriggerDuration;	/* Length of pretrigger collection in 'ticks'.	*/
	unsigned char msbyteLswordLsdwordPreTriggerDuration;	/* Length of pretrigger collection in 'ticks'.	*/
	unsigned char lsbyteMswordLsdwordPreTriggerDuration;	/* Length of pretrigger collection in 'ticks'.	*/
	unsigned char msbyteMswordLsdwordPreTriggerDuration;	/* Length of pretrigger collection in 'ticks'.	*/
	unsigned char lsbyteLswordMsdwordPreTriggerDuration;	/* Length of pretrigger collection in 'ticks'.	*/
	unsigned char msbyteLswordMsdwordPreTriggerDuration;	/* Length of pretrigger collection in 'ticks'.	*/
	unsigned char lsbyteMswordMsdwordPreTriggerDuration;	/* Length of pretrigger collection in 'ticks'.	*/
	unsigned char msbyteMswordMsdwordPreTriggerDuration;	/* Length of pretrigger collection in 'ticks'.	*/
	unsigned char lsbyteLsword_EnableSensorChannels;	/* Enable bits for sensor channels 0 thru 7. Channel 0 is usually reserved for time. */
	unsigned char msbyteLsword_EnableSensorChannels;	/* Enable bits for sensor channels 8 thru 15.	*/
	unsigned char lsbyteMsword_EnableSensorChannels;	/* Enable bits for sensor channels 16 thru 23.	*/
	unsigned char msbyteMsword_EnableSensorChannels;	/* Enable bits for sensor channels 24 thru 31.	*/
	unsigned char lsbyteLswordMeasurementPeriod;		/* base measurement period for all channels in ticks. */
	unsigned char msbyteLswordMeasurementPeriod;		/* (period == 0) => aperiodic collection */
	unsigned char lsbyteMswordMeasurementPeriod;		/* NGIO_COLLECTION_FLAGS_MASK_MULTIPLE_PERIODS set when */
	unsigned char msbyteMswordMeasurementPeriod;		/* different periods used - all periods are multiple of base period.*/
														/* Use NGIO_CMD_ID_GET_MEASUREMENT_PERIOD to determine periods when NGIO_COLLECTION_FLAGS_MASK_MULTIPLE_PERIODS set.*/
	unsigned char spare1;
	unsigned char spare2;
	unsigned char spare3;
	unsigned char spare4;
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_CMD_ID_SET_COLLECTION_PARAMS.	*/

typedef struct tagNGIOSetCollectionParams NGIOSetCollectionParams;

struct tagNGIOGetCollectionParams
{
	unsigned char lsbyteLswordDataRunId;//ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.
	unsigned char msbyteLswordDataRunId;//ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.
	unsigned char lsbyteMswordDataRunId;//ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.
	unsigned char msbyteMswordDataRunId;//ffffffffh => next non RealTime data run, 0 => RealTime, else id's archived run.
} _XPACK1; //Parameter block passed into SendCmd() with NGIO_CMD_ID_GET_COLLECTION_PARAMS.
typedef struct tagNGIOGetCollectionParams NGIOGetCollectionParams;

typedef NGIOSetCollectionParams NGIOGetCollectionParamsCmdResponsePayload;//This is the response payload returned by GetNextResponse() 
																	//after sending NGIO_CMD_ID_GET_COLLECTION_PARAMS.

#if defined (TARGET_OS_WIN)
#pragma pack(pop)
#endif

#ifdef TARGET_OS_MAC
#pragma pack()
#endif


#endif //_NGIO_SOURCE_CMDS_H_
