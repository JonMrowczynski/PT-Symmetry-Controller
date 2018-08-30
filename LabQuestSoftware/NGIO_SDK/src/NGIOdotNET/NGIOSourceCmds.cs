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
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace NGIOdotNET
{
	/// <summary>
	/// NGIO_ParmBlk defines the NGIO.Device_SendCmdAndGetResponse() protocol used with LabQuest, LabQuest Audio, 
	/// and LabQuest Mini devices.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack=1)]
	public class NGIO_ParmBlk
	{
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIOGetStatusCmdResponsePayload
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_STATUS = 0x10;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_START_MEASUREMENTS = 0x18;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_STOP_MEASUREMENTS = 0x19;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// Direct use of this command by application programs is not recommended. This command gets sent automatically by the NGIO
		/// library by NGIO_Device_Open().
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_INIT = 0x1A;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// Direct use of this command by application programs is not recommended. Instead, call
		/// NGIO_Device_SetMeasurementPeriod().
		/// <para>
		/// parameters: NGIOSetMeasurementPeriodParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_SET_MEASUREMENT_PERIOD = 0x1B;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// Direct use of this command by application programs is not recommended. Instead, call
		/// NGIO_Device_GetMeasurementPeriod().
		/// <para>
		/// parameters: NGIOGetMeasurementPeriodParams
		/// </para>
		/// <para>
		/// response: NGIOSetMeasurementPeriodParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_MEASUREMENT_PERIOD = 0x1C;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// The LED commands are supported by the LabQuest Mini, but not the LabQuest.
		/// <para>
		/// parameters: NGIOSetLedStateParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_SET_LED_STATE = 0x1D;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// The LED commands are supported by the LabQuest Mini, but not the LabQuest.
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIOSetLedStateParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_LED_STATE = 0x1E;
		//public const byte CMD_ID_GET_SERIAL_NUMBER = 0x20;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOSetAnalogInputParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_SET_ANALOG_INPUT = 0x21;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOChannelIdParameter
		/// </para>
		/// <para>
		/// response: NGIOSetAnalogInputParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_ANALOG_INPUT = 0x22;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOWriteNVMemParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_WRITE_NV_MEM = 0x26;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOReadNVMemParams
		/// </para>
		/// <para>
		/// response: NGIO_NVMEM_CHANNEL_ID1_rec, or byte[]
		/// </para>
		/// </summary>
		public const byte CMD_ID_READ_NV_MEM = 0x27;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// Direct use of this command by application programs is not recommended. 
		/// Instead, call NGIO_Device_DDSMem_GetSensorNumber(sendQueryToHardwareflag=1).
		/// <para>
		/// parameters: NGIOChannelIdParameter
		/// </para>
		/// <para>
		/// response: NGIOGetSensorIdCmdResponsePayload
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_SENSOR_ID = 0x28;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOSetSamplingModeParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_SET_SAMPLING_MODE = 0x29;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOChannelIdParameter
		/// </para>
		/// <para>
		/// response: NGIOSetSamplingModeParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_SAMPLING_MODE = 0x2A;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOSetSensorChannelEnableMaskParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK = 0x2C;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIOSetSensorChannelEnableMaskParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_SENSOR_CHANNEL_ENABLE_MASK = 0x2D;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// This command is currently only supported with framed collection. See NGIO_Device_Frm_AllocateCollectionFrames().
		/// <para>
		/// parameters: NGIOSetCollectionParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_SET_COLLECTION_PARAMS = 0x2E;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// This command is currently only supported with framed collection. See NGIO_Device_Frm_AllocateCollectionFrames().
		/// <para>
		/// parameters: NGIOGetCollectionParams
		/// </para>
		/// <para>
		/// response: NGIOSetCollectionParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_COLLECTION_PARAMS = 0x2F;

		const byte CMD_ID_GET_ARCHIVED_RUN_IDS = 0x30;
		const byte CMD_ID_DELETE_ARCHIVED_RUNS = 0x31;

		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// This command only works when the sampling mode for the channel is set to NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT,
		/// NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER, or NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4.
		/// <para>
		/// parameters: NGIOSetDigitalCounterParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_SET_DIGITAL_COUNTER = 0x32;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// This command only works when the sampling mode for the channel is set to NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT,
		/// NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER, or NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4.
		/// <para>
		/// parameters: NGIOChannelIdParameter
		/// </para>
		/// <para>
		/// response: NGIOSetDigitalCounterParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_GET_DIGITAL_COUNTER = 0x33;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// This command clears error bits reported in the status field reported by NGIO_CMD_ID_GET_STATUS.
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_CLEAR_ERROR_FLAGS = 0x34;
		const byte CMD_ID_ENABLE_SENSOR_ID_NOTIFICATIONS = 0x35;
		const byte CMD_ID_DISABLE_SENSOR_ID_NOTIFICATIONS = 0x36;

		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// NGIO_CMD_ID_WRITE_IO_CONFIG and NGIO_CMD_ID_WRITE_IO are disabled unless sampling mode is NGIO_SAMPLING_MODE_CUSTOM.
		/// When sampling mode is configured as NGIO_SAMPLING_MODE_CUSTOM, all the IO lines are initially configured as inputs.
		/// Before configuring an IO line as an output with NGIO_CMD_ID_WRITE_IO_CONFIG, it is recommended that you first send
		/// a NGIO_CMD_ID_WRITE_IO command to set the state of the output to the level specified in the outputLatch parameter.
		/// <para>
		/// parameters: NGIOWriteIOConfigParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_WRITE_IO_CONFIG = 0x37;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOChannelIdParameter
		/// </para>
		/// <para>
		/// response: NGIOWriteIOConfigParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_READ_IO_CONFIG = 0x38;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// NGIO_CMD_ID_WRITE_IO_CONFIG and NGIO_CMD_ID_WRITE_IO are disabled unless sampling mode is NGIO_SAMPLING_MODE_CUSTOM.
		/// When sampling mode is configured as NGIO_SAMPLING_MODE_CUSTOM, all the IO lines are initially configured as inputs.
		/// Before configuring an IO line as an output with NGIO_CMD_ID_WRITE_IO_CONFIG, it is recommended that you first send
		/// a NGIO_CMD_ID_WRITE_IO command to set the state of the output to the level specified in the outputLatch parameter.
		/// <para>
		/// parameters: NGIOWriteIOParams
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_WRITE_IO = 0x39;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// <para>
		/// parameters: NGIOChannelIdParameter
		/// </para>
		/// <para>
		/// response: NGIOWriteIOParams
		/// </para>
		/// </summary>
		public const byte CMD_ID_READ_IO = 0x3A;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// This command is currently only supported with framed collection. See NGIO_Device_Frm_AllocateCollectionFrames().
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_START_FRAME_COLLECTION = 0x3B;
		/// <summary>
		/// Used with NGIO.Device_SendCmdAndGetResponseN().
		/// This command is currently only supported with framed collection. See NGIO_Device_Frm_AllocateCollectionFrames().
		/// <para>
		/// parameters: none
		/// </para>
		/// <para>
		/// response: NGIODefaultCmdResponse
		/// </para>
		/// </summary>
		public const byte CMD_ID_STOP_FRAME_COLLECTION = 0x3C;

		public const sbyte CHANNEL_ID_TIME = 0;
		public const sbyte CHANNEL_ID_ANALOG1 = 1;
		public const sbyte CHANNEL_ID_ANALOG2 = 2;
		public const sbyte CHANNEL_ID_ANALOG3 = 3;
		public const sbyte CHANNEL_ID_ANALOG4 = 4;
		public const sbyte CHANNEL_ID_DIGITAL1 = 5;
		public const sbyte CHANNEL_ID_DIGITAL2 = 6;
		public const sbyte CHANNEL_ID_BUILT_IN_TEMP = 7;
		/// <summary>
		/// CHANNEL_ID_ANALOG_EXT1 is not currently supported.
		/// </summary>
		public const sbyte CHANNEL_ID_ANALOG_EXT1 = 8;
		/// <summary>
		/// CHANNEL_ID_ANALOG_EXT2 is not currently supported.
		/// </summary>
		public const sbyte CHANNEL_ID_ANALOG_EXT2 = 9;

		//public const byte MAX_NUM_CHANNELS = 10;
		public const byte MAX_NUM_CHANNELS = 8;

		public const byte MAX_NUM_DIGITAL_CHANNELS = 2;

		//Following values are used with NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK lsbyteLsword_EnableSensorChannels:
		public const byte CHANNEL_MASK_TIME = 1;
		public const byte CHANNEL_MASK_ANALOG1 = 2;
		public const byte CHANNEL_MASK_ANALOG2 = 4;
		public const byte CHANNEL_MASK_ANALOG3 = 8;
		public const byte CHANNEL_MASK_ANALOG4 = 0x10;
		public const byte CHANNEL_MASK_DIGITAL1 = 0x20;
		public const byte CHANNEL_MASK_DIGITAL2 = 0x40;
		public const byte CHANNEL_MASK_BUILT_IN_TEMP = 0x80;

		public const byte CHANNEL_MASK_ALL_NORMAL_SENSORS = 0xfe;
		public const byte CHANNEL_MASK_ALL_NORMAL_ANALOG_SENSORS = 0x1e;

		//Following values are used with NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK msbyteLsword_EnableSensorChannels:
		/// <summary>
		/// CHANNEL_MASK_ANALOG_EXT1 is not currently supported.
		/// </summary>
		public const byte CHANNEL_MASK_ANALOG_EXT1 = 0x1;
		/// <summary>
		/// CHANNEL_MASK_ANALOG_EXT2 is not currently supported.
		/// </summary>
		public const byte CHANNEL_MASK_ANALOG_EXT2 = 0x2;

//
/***************************************************************************************************/
//

// NGIO_DEVTYPE_LABPRO2_AUDIO devices use the following channels:

		public const byte CHANNEL_ID_AUDIO_INTERNAL = 1;
		public const byte CHANNEL_ID_AUDIO_EXTERNAL_LEFT = 2;
		public const byte CHANNEL_ID_AUDIO_EXTERNAL_RIGHT = 3;

		public const byte MAX_NUM_AUDIO_CHANNELS = 4;

		public const byte CHANNEL_ID_MASK_AUDIO_INTERNAL = 2;
		public const byte CHANNEL_ID_MASK_AUDIO_EXTERNAL_LEFT = 4;
		public const byte CHANNEL_ID_MASK_AUDIO_EXTERNAL_RIGHT = 8;
//
/***************************************************************************************************/
//
	
	}

	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIODefaultCmdResponse
	{
		public const byte STATUS_SUCCESS = 0;
		public const byte STATUS_NOT_READY_FOR_NEW_CMD = 0x30;
		public const byte STATUS_CMD_NOT_SUPPORTED = 0x31;
		public const byte STATUS_INTERNAL_ERROR1 = 0x32;
		public const byte STATUS_INTERNAL_ERROR2 = 0x33;
		public const byte STATUS_ERROR_CANNOT_CHANGE_PERIOD_WHILE_COLLECTING = 0x34;
		public const byte STATUS_ERROR_CANNOT_READ_NV_MEM_BLK_WHILE_COLLECTING_FAST = 0x35;
		public const byte STATUS_ERROR_INVALID_PARAMETER = 0x36;
		public const byte STATUS_ERROR_CANNOT_WRITE_FLASH_WHILE_COLLECTING = 0x37;
		public const byte STATUS_ERROR_CANNOT_WRITE_FLASH_WHILE_HOST_FIFO_BUSY = 0x38;
		public const byte STATUS_ERROR_OP_BLOCKED_WHILE_COLLECTING = 0x39;
		public const byte STATUS_ERROR_CALCULATOR_CANNOT_MEASURE_WITH_NO_BATTERIES = 0x3A;
		public const byte STATUS_ERROR_OP_NOT_SUPPORTED_IN_CURRENT_MODE = 0x3B;
		public const byte STATUS_ERROR_AUDIO_CONTROL_FAILURE = 0x3C;
		public const byte STATUS_ERROR_AUDIO_STREAM_FAILURE = 0x3D;
		public const byte STATUS_ERROR_CANNOT_REALLOCATE_FRAME_BUFFERS = 0x3E;
		/// <summary>
		/// NGIO.GetErrorSentOverTheWire() returns NGIODefaultCmdResponse.NO_ERROR_SENT_OVER_THE_WIRE if the
		/// most recent call to NGIO.SendCmdAndGetResponse() did not result in an error code being sent back from
		/// the device.
		/// </summary>
		public const byte NO_ERROR_SENT_OVER_THE_WIRE = 0xFE;

		/// <summary>
		/// NGIODefaultCmdResponse.STATUS_...
		/// </summary>
		public byte status;
	} 

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_SET_MEASUREMENT_PERIOD.
	/// This is also the response payload returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_MEASUREMENT_PERIOD.
	/// Direct use of the NGIOSetMeasurementPeriodParams struct is not recommended. 
	/// Use NGIO.Device_SetMeasurementPeriod() instead.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOSetMeasurementPeriodParams
	{
		/// <summary>
		/// -1 => all channels.
		/// </summary>
		public sbyte channel;
		/// <summary>
		/// Set this to 0, which indicates RealTime data collection.
		/// </summary>
		public byte lsbyteLswordDataRunId;
		public byte msbyteLswordDataRunId;
		public byte lsbyteMswordDataRunId;
		public byte msbyteMswordDataRunId;
		/// <summary>
		/// Measurement period in device 'ticks', which are microseconds for the LabQuest.
		/// </summary>
		public byte lsbyteLswordMeasurementPeriod;
		public byte msbyteLswordMeasurementPeriod;
		public byte lsbyteMswordMeasurementPeriod;
		public byte msbyteMswordMeasurementPeriod;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_GET_MEASUREMENT_PERIOD.
	/// Direct use of the NGIOGetMeasurementPeriodParams struct is not recommended. 
	/// Use NGIO.Device_GetMeasurementPeriod() instead.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOGetMeasurementPeriodParams 
	{
		/// <summary>
		/// -1 => all channels.
		/// </summary>
		public sbyte channel;
		/// <summary>
		/// Set this to 0, which indicates RealTime data collection.
		/// </summary>
		public byte lsbyteLswordDataRunId;
		public byte msbyteLswordDataRunId;
		public byte lsbyteMswordDataRunId;
		public byte msbyteMswordDataRunId;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_SET_LED_STATE.
	/// This is also the response payload returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_LED_STATE.
	/// NGIO_CMD_ID_SET_LED_STATE is supported by the LabQuest Mini, but not the LabQuest.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOSetLedStateParams
	{
		public const byte LED_COLOR_BLACK = 0xC0;
		public const byte LED_COLOR_RED = 0x40;
		public const byte LED_COLOR_GREEN = 0x80;
		public const byte LED_COLOR_RED_GREEN = 0;
		public const byte LED_BRIGHTNESS_MIN = 0;
		public const byte LED_BRIGHTNESS_MAX = 0x10;
		public const byte LED_COLOR_ORANGE = 0;
		public const byte LED_BRIGHTNESS_ORANGE = 4;
		/// <summary>
		/// LEDchannel is ignored by the LabQuest Mini.
		/// </summary>
		public byte LEDchannel;
		public byte color;
		public byte brightness;
	}

	/// <summary>
	/// This is the response payload returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_STATUS.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOGetStatusCmdResponsePayload
	{
		public const byte MASK_STATUS_ERROR_CMD_NOT_RECOGNIZED = 1;
		public const byte MASK_STATUS_ERROR_CMD_IGNORED = 2;
		public const byte MASK_STATUS_ADC_UNCALIBRATED = 4;
		public const byte MASK_STATUS_AUTOID_UNCALIBRATED = 8;
		public const byte MASK_STATUS_ERROR_INTERNAL_ERROR1 = 0X10;
		public const byte MASK_STATUS_ERROR_AUDIO_CONTROL_FAILURE = 0X10;
		public const byte MASK_STATUS_ERROR_INTERNAL_ERROR2 = 0X20;
		public const byte MASK_STATUS_ERROR_AUDIO_STREAM_FAILURE = 0X20;
		public const byte MASK_STATUS_ERROR_MASTER_FIFO_OVERFLOW = 0X40;
		public const byte MASK_STATUS_ERROR_DIGITAL_TRANSITION_LOST = 0X80;

		/// <summary>
		/// See MASK_STATUS_ .
		/// </summary>
		public byte status;
		/// <summary>
		/// Binary coded decimal.
		/// </summary>
		public byte minorVersionMasterCPU;
		/// <summary>
		/// Binary coded decimal.
		/// </summary>
		public byte majorVersionMasterCPU;
		/// <summary>
		/// Binary coded decimal - updated by LabQuest Mini, but not by the LabQuest.
		/// </summary>
		public byte minorVersionSlaveCPU;
		/// <summary>
		/// Binary coded decimal - updated by LabQuest Mini, but not by the LabQuest.
		/// </summary>
		public byte majorVersionSlaveCPU;
	}

	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOSerialNumber
	{
		/// <summary>
		/// week in year(starting at 1) in BCD format
		/// </summary>
		public byte ww;
		/// <summary>
		/// last two digits of year in BCD format
		/// </summary>
		public byte yy;
		public byte lsbyteLswordSerialCounter;
		public byte msbyteLswordSerialCounter;
		public byte lsbyteMswordSerialCounter;
		public byte msbyteMswordSerialCounter;
	}

	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIO_NVMEM_CHANNEL_ID1_rec
	{
		/// <summary>
		/// First valid version is 1 .
		/// </summary>
		public byte version;
		public byte spare1;
		public byte spare2;
		public byte spare3;
		public NGIOSerialNumber serialNumber;
//		public NGIO_DAQ_CalibrationParams calibrationParams;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_READ_NV_MEM.
	/// In general, response payload returned by SendCmdAndGetResponse() for NGIO_CMD_ID_READ_NV_MEM is just a byte array.
	/// However, depending on what parameters are sent, the response can vary, eg NGIO_NVMEM_CHANNEL_ID1_rec .
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOReadNVMemParams
	{
		public const byte NGIO_NVMEM_CHANNEL_ID1 = 100;
		public const byte NGIO_NVMEM_CHANNEL_ID2 = 101;
		public const byte NGIO_NVMEM_CHANNEL_ID1_MEMSIZE = 250;
		public const int NGIO_NVMEM_CHANNEL_ID2_MEMSIZE = 750;
		/// <summary>
		/// NGIO_CHANNEL_ID_ANALOGn => access DDS record on sensor. DDS addr range is 0 to 127.
		/// NGIO_NVMEM_CHANNEL_IDn => access flash on DAQ. DAQ flash addr range is 0 to (NGIO_NVMEM_CHANNEL_IDn_MEMSIZE - 1).
		/// </summary>
		public sbyte channel;
		public byte lsbyteLswordAddr;
		public byte msbyteLswordAddr;
		public byte lsbyteMswordAddr;
		public byte msbyteMswordAddr;
		public byte lsByteCount;
		public byte msByteCount;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_WRITE_NV_MEM.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public unsafe struct NGIOWriteNVMemParams
	{
		/// <summary>
		/// NGIO_CHANNEL_ID_ANALOGn => access DDS record on sensor. DDS addr range is 0 to 127.
		/// NGIO_NVMEM_CHANNEL_IDn => access flash on DAQ. DAQ flash addr range is 0 to (NGIO_NVMEM_CHANNEL_IDn_MEMSIZE - 1).
		/// </summary>
		public sbyte channel;
		public byte lsbyteLswordAddr;
		public byte msbyteLswordAddr;
		public byte lsbyteMswordAddr;
		public byte msbyteMswordAddr;
		public byte count;
		/// <summary>
		/// Useful size of payload field is given by NGIOWriteNVMemParams.count .
		/// </summary>
		public fixed byte payload[128]; 
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with 
	/// NGIO_CMD_ID_GET_SENSOR_ID,
	/// NGIO_CMD_ID_GET_ANALOG_INPUT,
	/// NGIO_CMD_ID_GET_SAMPLING_MODE,
	/// NGIO_CMD_ID_GET_DIGITAL_COUNTER,
	/// NGIO_CMD_ID_READ_IO_CONFIG,
	/// NGIO_CMD_ID_READ_IO
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOChannelIdParameter
	{
		/// <summary>
		/// NGIO_CHANNEL_ID_ANALOG1 .. NGIO_CHANNEL_ID_DIGITAL2
		/// </summary>
		public sbyte channel;
	}

	/// <summary>
	/// This is the response payload returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_SENSOR_ID.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOGetSensorIdCmdResponsePayload
	{
		public byte lsbyteLswordSensorId;
		public byte msbyteLswordSensorId;
		public byte lsbyteMswordSensorId;
		public byte msbyteMswordSensorId;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_SET_ANALOG_INPUT.
	/// This is also the response returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_ANALOG_INPUT.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOSetAnalogInputParams
	{
		/// <summary>
		/// ANALOG_INPUT_5V_BUILTIN_12BIT_ADC is the default value for all analog channels(set by NGIO_CMD_ID_INIT).
		/// </summary>
		public const byte ANALOG_INPUT_5V_BUILTIN_12BIT_ADC = 0;
		public const byte ANALOG_INPUT_5V_BUILTIN_10BIT_ADC = 1;
		public const byte ANALOG_INPUT_5V_EXT_12BIT_ADC = 2;
		public const byte ANALOG_INPUT_5V_EXT_16BIT_ADC = 3;
		public const byte ANALOG_INPUT_PM10V_BUILTIN_12BIT_ADC = 4;
		public const byte ANALOG_INPUT_PM10V_BUILTIN_10BIT_ADC = 5;
		public const byte ANALOG_INPUT_PM10V_EXT_12BIT_ADC = 6;
		public const byte ANALOG_INPUT_PM10V_EXT_16BIT_ADC = 7;
		/// <summary>
		/// NGIO_CHANNEL_ID_ANALOG1 ...
		/// </summary>
		public sbyte channel;
		/// <summary>
		/// ANALOG_INPUT_... 
		/// </summary>
		public byte analogInput;	//NGIO_ANALOG_INPUT_... 
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_SET_SAMPLING_MODE.
	/// This is also the response returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_SAMPLING_MODE.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOSetSamplingModeParams
	{
		/// <summary>
		/// SAMPLING_MODE_PERIODIC_LEVEL_SNAPSHOT is the default value for all analog channels(set by NGIO_CMD_ID_INIT).
		/// </summary>
		public const byte SAMPLING_MODE_PERIODIC_LEVEL_SNAPSHOT = 0;
		/// <summary>
		/// SAMPLING_MODE_APERIODIC_EDGE_DETECT is the default value for all digital channels(set by NGIO_CMD_ID_INIT).
		/// </summary>
		public const byte SAMPLING_MODE_APERIODIC_EDGE_DETECT = 1;
		public const byte SAMPLING_MODE_PERIODIC_PULSE_COUNT = 2;
		public const byte SAMPLING_MODE_PERIODIC_MOTION_DETECT = 3;
		public const byte SAMPLING_MODE_PERIODIC_ROTATION_COUNTER = 4;
		public const byte SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4 = 5;
		/// <summary>
		/// Note that for LabQuest family devices NGIO_CMD_ID_WRITE_IO_CONFIG and NGIO_CMD_ID_WRITE_IO are disabled 
		/// unless sampling mode is NGIO_SAMPLING_MODE_CUSTOM.
		/// </summary>
		public const byte SAMPLING_MODE_CUSTOM = 6;
		/// <summary>
		/// NGIO_CHANNEL_ID_ANALOG1 ...
		/// </summary>
		public sbyte channel;
		/// <summary>
		/// SAMPLING_MODE_... 
		/// </summary>
		public byte samplingMode;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK.
	/// This is also the response returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_SENSOR_CHANNEL_ENABLE_MASK.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOSetSensorChannelEnableMaskParams
	{
		/// <summary>
		/// Enable bits for sensor channels 0 thru 7. Channel 0 is usually reserved for time.
		/// See NGIO_ParmBlk.CHANNEL_MASK_... and NGIO_ParmBlk.CHANNEL_ID_MASK_AUDIO_...
		/// </summary>
		public byte lsbyteLsword_EnableSensorChannels;
		/// <summary>
		/// Enable bits for sensor channels 8 thru 15 - not supported by LabQuest currently.
		/// </summary>
		public byte msbyteLsword_EnableSensorChannels;
		public byte lsbyteMsword_EnableSensorChannels;
		public byte msbyteMsword_EnableSensorChannels;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_SET_DIGITAL_COUNTER.
	/// This is also the response returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_DIGITAL_COUNTER.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOSetDigitalCounterParams
	{
		/// <summary>
		/// NGIO_CHANNEL_ID_DIGITAL1 or NGIO_CHANNEL_ID_DIGITAL2
		/// </summary>
		public sbyte channel;
		public byte lsbyteLsword_counter;
		public byte msbyteLsword_counter;
		public byte lsbyteMsword_counter;
		public byte msbyteMsword_counter;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_WRITE_IO_CONFIG.
	/// This is also the response returned by SendCmdAndGetResponse() for NGIO_CMD_ID_READ_IO_CONFIG.
	/// NGIO_CMD_ID_WRITE_IO_CONFIG and NGIO_CMD_ID_WRITE_IO are disabled unless sampling mode is NGIO_SAMPLING_MODE_CUSTOM.
	/// When sampling mode is configured as NGIO_SAMPLING_MODE_CUSTOM, all the IO lines are initially configured as inputs.
	/// Before configuring an IO line as an output with NGIO_CMD_ID_WRITE_IO_CONFIG, it is recommended that you first send
	/// a NGIO_CMD_ID_WRITE_IO command to set the state of the output to the level specified in the outputLatch parameter.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOWriteIOConfigParams
	{
		/// <summary>
		/// Each digital channel has 4 digital IO lines associated with it.
		/// Mask bit identifying digital line 1.
		/// </summary>
		public const byte MASK_DGX_LINE1 = 1;
		/// <summary>
		/// Each digital channel has 4 digital IO lines associated with it.
		/// Mask bit identifying digital line 2.
		/// </summary>
		public const byte MASK_DGX_LINE2 = 2;
		/// <summary>
		/// Each digital channel has 4 digital IO lines associated with it.
		/// Mask bit identifying digital line 3.
		/// </summary>
		public const byte MASK_DGX_LINE3 = 4;
		/// <summary>
		/// Each digital channel has 4 digital IO lines associated with it.
		/// Mask bit identifying digital line 4.
		/// </summary>
		public const byte MASK_DGX_LINE4 = 8;
		/// <summary>
		/// NGIO_CHANNEL_ID_DIGITAL1 or NGIO_CHANNEL_ID_DIGITAL2
		/// </summary>
		public sbyte channel;
		/// <summary>
		/// [MASK_DGX_LINE1] | .. | [MASK_DGX_LINE4]. Only configure line n if MASK_DGX_LINEn set in mask.
		/// </summary>
		public byte mask;
		/// <summary>
		/// MASK_DGX_LINEn bit set => line n is an input, else line n is an output.
		/// </summary>
		public byte IO;
		/// <summary>
		/// MASK_DGX_LINEn bit set => open collector if line is also configured as output.
		/// </summary>
		public byte open_collector;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_WRITE_IO.
	/// This is also the response returned by SendCmdAndGetResponse() for NGIO_CMD_ID_READ_IO.
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOWriteIOParams
	{
		/// <summary>
		/// NGIO_CHANNEL_ID_DIGITAL1 or NGIO_CHANNEL_ID_DIGITAL2
		/// </summary>
		public sbyte channel;
		/// <summary>
		/// [MASK_DGX_LINE1] | .. | [MASK_DGX_LINE4]. Only modify line n if MASK_DGX_LINEn set in mask.
		/// </summary>
		public byte mask;
		/// <summary>
		/// MASK_DGX_LINEn bit set => line n is driven high, else line n is driven low when line is configured as output.
		/// </summary>
		public byte outputLatch;
		/// <summary>
		/// This contains the actual level on the IO pins. 
		/// Use this to read input levels. NGIO_CMD_ID_WRITE_IO ignores this param.
		/// </summary>
		public byte IOLive;
	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_SET_COLLECTION_PARAMS.
	/// This is also the response returned by SendCmdAndGetResponse() for NGIO_CMD_ID_GET_COLLECTION_PARAMS.
	/// These commands are currently only supported with framed collection. See NGIO_Device_Frm_AllocateCollectionFrames().
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOSetCollectionParams
	{
		public const byte TRIGGER_MASK_BUTTON = 1;
		public const byte TRIGGER_MASK_CHANNEL = 2;
		public const byte TRIGGER_MASK_IMMEDIATE = 4;
		public const byte TRIGGER_CHAN_MODE_MEAS_ABOVE_THRESHOLD = 0;
		public const byte TRIGGER_CHAN_MODE_MEAS_BELOW_THRESHOLD = 1;
		public const byte TRIGGER_CHAN_MODE_LO_TO_HI_MEAS = 2;
		public const byte TRIGGER_CHAN_MODE_HI_TO_LO_MEAS = 3;
		public const byte COLLECTION_FLAGS_MASK_MULTIPLE_PERIODS = 4;

		/// <summary>
		/// Set this to 0 for the LabQuest and LabQuest Mini.
		/// </summary>
		public byte lsbyteLswordDataRunId;
		/// <summary>
		/// Set this to 0 for the LabQuest and LabQuest Mini.
		/// </summary>
		public byte msbyteLswordDataRunId;
		/// <summary>
		/// Set this to 0 for the LabQuest and LabQuest Mini.
		/// </summary>
		public byte lsbyteMswordDataRunId;
		/// <summary>
		/// Set this to 0 for the LabQuest and LabQuest Mini.
		/// </summary>
		public byte msbyteMswordDataRunId;

		/// <summary>
		/// NGIO_TRIGGER_MASK_BUTTON | NGIO_TRIGGER_MASK_CHANNEL...
		/// </summary>
		public byte triggerMask;
		/// <summary>
		/// NGIO_TRIGGER_CHAN_MODE_...
		/// </summary>
		public byte triggerChannelMode;
		/// <summary>
		/// Used if NGIO_TRIGGER_MASK_CHANNEL is set in triggerMask.
		/// </summary>
		public byte triggerChannel;
		/// <summary>
		/// NGIO_COLLECTION_FLAGS_MASK_...
		/// </summary>
		public byte flags;
		public byte lsbyteLswordTriggerThreshold;
		public byte msbyteLswordTriggerThreshold;
		public byte lsbyteMswordTriggerThreshold;
		public byte msbyteMswordTriggerThreshold;
		/// <summary>
		/// NGIO_TRIGGER_CHAN_MODE_LO_TO_HI_MEAS => signal must be less than (threshold - hysteresis) before
		/// testing against threshold.
		/// NGIO_TRIGGER_CHAN_MODE_HI_TO_LO_MEAS => signal must be > (threshold + hysteresis) before
		/// testing against threshold.
		/// </summary>
		public byte lsbyteLswordTriggerHysteresis;
		public byte msbyteLswordTriggerHysteresis;
		public byte lsbyteMswordTriggerHysteresis;
		public byte msbyteMswordTriggerHysteresis;
		/// <summary>
		/// Min time between successive frame triggers in 'ticks'.
		/// </summary>
		public byte lsbyteLswordLsdwordMinInterframeDelay;
		public byte msbyteLswordLsdwordMinInterframeDelay;
		public byte lsbyteMswordLsdwordMinInterframeDelay;
		public byte msbyteMswordLsdwordMinInterframeDelay;
		public byte lsbyteLswordMsdwordMinInterframeDelay;
		public byte msbyteLswordMsdwordMinInterframeDelay;
		public byte lsbyteMswordMsdwordMinInterframeDelay;
		public byte msbyteMswordMsdwordMinInterframeDelay;
		/// <summary>
		/// Length of collection in 'ticks'.
		/// </summary>
		public byte lsbyteLswordLsdwordCollectionDuration;
		public byte msbyteLswordLsdwordCollectionDuration;
		public byte lsbyteMswordLsdwordCollectionDuration;
		public byte msbyteMswordLsdwordCollectionDuration;
		public byte lsbyteLswordMsdwordCollectionDuration;
		public byte msbyteLswordMsdwordCollectionDuration;
		public byte lsbyteMswordMsdwordCollectionDuration;
		public byte msbyteMswordMsdwordCollectionDuration;
		/// <summary>
		/// Length of pretrigger collection in 'ticks'.
		/// </summary>
		public byte lsbyteLswordLsdwordPreTriggerDuration;
		public byte msbyteLswordLsdwordPreTriggerDuration;
		public byte lsbyteMswordLsdwordPreTriggerDuration;
		public byte msbyteMswordLsdwordPreTriggerDuration;
		public byte lsbyteLswordMsdwordPreTriggerDuration;
		public byte msbyteLswordMsdwordPreTriggerDuration;
		public byte lsbyteMswordMsdwordPreTriggerDuration;
		public byte msbyteMswordMsdwordPreTriggerDuration;
		/// <summary>
		/// Enable bits for sensor channels 0 thru 7. Channel 0 is usually reserved for time.
		/// </summary>
		public byte lsbyteLsword_EnableSensorChannels;
		/// <summary>
		/// Enable bits for sensor channels 8 thru 15.
		/// </summary>
		public byte msbyteLsword_EnableSensorChannels;
		/// <summary>
		/// Enable bits for sensor channels 16 thru 23.
		/// </summary>
		public byte lsbyteMsword_EnableSensorChannels;
		/// <summary>
		/// Enable bits for sensor channels 24 thru 31.
		/// </summary>
		public byte msbyteMsword_EnableSensorChannels;
		/// <summary>
		/// Base measurement period for all channels in ticks.
		/// (period == 0) => aperiodic collection.
		/// NGIO_COLLECTION_FLAGS_MASK_MULTIPLE_PERIODS set when different periods used - all periods are multiple of base period.
		/// Use NGIO_CMD_ID_GET_MEASUREMENT_PERIOD to determine periods when NGIO_COLLECTION_FLAGS_MASK_MULTIPLE_PERIODS set.
		/// </summary>
		public byte lsbyteLswordMeasurementPeriod;	
		public byte msbyteLswordMeasurementPeriod;	
		public byte lsbyteMswordMeasurementPeriod;	
		public byte msbyteMswordMeasurementPeriod;	
													
		public byte spare1;
		public byte spare2;
		public byte spare3;
		public byte spare4;

	}

	/// <summary>
	/// Parameter block passed into SendCmdAndGetResponse() with NGIO_CMD_ID_GET_COLLECTION_PARAMS.
	/// This command is currently only supported with framed collection. See NGIO_Device_Frm_AllocateCollectionFrames().
	/// </summary>
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct NGIOGetCollectionParams
	{
		/// <summary>
		/// Set this to 0 for the LabQuest and LabQuest Mini.
		/// </summary>
		public byte lsbyteLswordDataRunId;
		/// <summary>
		/// Set this to 0 for the LabQuest and LabQuest Mini.
		/// </summary>
		public byte msbyteLswordDataRunId;
		/// <summary>
		/// Set this to 0 for the LabQuest and LabQuest Mini.
		/// </summary>
		public byte lsbyteMswordDataRunId;
		/// <summary>
		/// Set this to 0 for the LabQuest and LabQuest Mini.
		/// </summary>
		public byte msbyteMswordDataRunId;
	}

	/*
 */

}

/***************************************************************************************************/
/***************************************************************************************************/

