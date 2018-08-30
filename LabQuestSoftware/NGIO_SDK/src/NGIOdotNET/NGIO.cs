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
using VSTCoreDefsdotNET;

namespace NGIOdotNET
{
	/// <summary>
	/// 	The NGIO class provides an application program with full access to the data acquisition capabilities built
	/// 	into the LabQuest and the LabQuest Mini. This class is a very thin managed code wrapper around the unmanaged NGIO library.
	/// 	The NGIO class API is fairly broad, so knowing where to start is hard. The documentation for the 
	/// 	NGIO_Device_Open() and the NGIO_Device_SendCmdAndGetResponse() functions are a good starting place.
	/// <para>
	///		Refer to the NGIO_ParmBlk class for the command and response data structures passed into NGIO_Device_SendCmdAndGetResponse().
	/// </para>
	/// <para>
	/// The NGIOdotNet XML docs are a work in progress. More complete documentation can be found in the NGIO_lib_interface.h file.
	/// </para>
	/// </summary>
	public class NGIO
	{
		public const UInt32 MAX_SIZE_DEVICE_NAME = 220;
		public const byte COMM_TRANSPORT_USB = 1;
		public const byte COMM_TRANSPORT_SERIAL = 2;
		public const byte COMM_TRANSPORT_BLUETOOTH = 3;
		public const byte COMM_TRANSPORT_BLUETOOTH_ALT1 = 4;
		public const byte COMM_TRANSPORT_WIRELESS_80211_X = 5;
		public const byte COMM_TRANSPORT_HARDWIRE_LAN = 6;

		public const byte DEVTYPE_LABPRO = 1;
		public const byte DEVTYPE_GO_TEMP = 2;
		public const byte DEVTYPE_GO_LINK = 3;
		public const byte DEVTYPE_GO_MOTION = 4;
		public const byte DEVTYPE_LABQUEST = 5;
		public const byte DEVTYPE_WDSS = 6;
		public const byte DEVTYPE_NI_SENSORDAQ = 7;
		public const byte DEVTYPE_LABQUEST_FILE_SERVER = 8;
		public const byte DEVTYPE_LABQUEST_AUDIO = 9;
		public const byte DEVTYPE_DAISYCHAIN_TEST = 10;
		public const byte DEVTYPE_LABQUEST_FUNC_GENERATOR = 11;
		public const byte DEVTYPE_LABQUEST_MINI = 12;
        public const byte DEVTYPE_LABQUEST2 = 14;
        public const byte DEVTYPE_LABQUEST_BUILTIN_ACCELEROMETER = 15;
        public const byte DEVTYPE_LABQUEST_BUILTIN_LIGHT_SENSOR = 16;

		/// <summary>
		/// TIMEOUT_MS_DEFAULT is the recommended timeout in milliseconds for most commands sent to the hardware 
		/// via NGIO_DeviceSendCmdAndGetResponse(). Note that this timeout is much longer than the expected execution time
		/// of NGIO_DeviceSendCmdAndGetResponse() for most commands, which is about 40 milliseconds.
		/// </summary>
		public const UInt32 TIMEOUT_MS_DEFAULT = 2000;
		public const UInt32 INIT_CMD_RESPONSE_TIMEOUT_MS = 2000;
		/// <summary>
		/// GRAB_DAQ_TIMEOUT is so long because the LabQuest needs to save data when it shuts down remote collection.
		/// </summary>
		public const UInt32 GRAB_DAQ_TIMEOUT_MS = 12000;
		public const UInt32 READ_DDSMEMBLOCK_TIMEOUT_MS = 2000;
		public const UInt32 WRITE_DDSMEMBLOCK_TIMEOUT_MS = 4000;

		public const UInt32 TCPIP_PORT_LABQUEST = 0x9500;

		/// <summary>
		/// NGIO_Diags_SetDebugTraceThreshold() threshold parameter value. Setting debug trace threshold to 
		/// TRACE_SEVERITY_LOWEST causes the most messages to be sent to the debug console.
		/// </summary>
		public const byte TRACE_SEVERITY_LOWEST = 1;
		public const byte TRACE_SEVERITY_LOW = 10;
		public const byte TRACE_SEVERITY_MEDIUM = 50;
		public const byte TRACE_SEVERITY_HIGH = 100;

		/// <summary>
		/// Measurement value reported from NGIO_Device_ReadRawMeasurements() for channel CHANNEL_ID_DIGITAL1 or
		/// CHANNEL_ID_DIGITAL2 when the channel is configured with SAMPLING_MODE_APERIODIC_EDGE_DETECT.
		/// </summary>
		public const Int32 MEAS_PHOTOGATE_BLOCKED = 1;
		/// <summary>
		/// Measurement value reported from NGIO_Device_ReadRawMeasurements() for channel CHANNEL_ID_DIGITAL1 or
		/// CHANNEL_ID_DIGITAL2 when the channel is configured with SAMPLING_MODE_APERIODIC_EDGE_DETECT.
		/// </summary>
		public const Int32 MEAS_PHOTOGATE_UNBLOCKED = 0;
		/// <summary>
		/// Measurement value reported from NGIO_Device_ReadRawMeasurements() for channel CHANNEL_ID_DIGITAL1 or
		/// CHANNEL_ID_DIGITAL2 when the channel is configured with SAMPLING_MODE_PERIODIC_MOTION_DETECT.
		/// This value indicates when the detector transmitted a ping.
		/// </summary>
		public const Int32 MEAS_MOTION_DETECTOR_PING = 0;
		/// <summary>
		/// Measurement value reported from NGIO_Device_ReadRawMeasurements() for channel CHANNEL_ID_DIGITAL1 or
		/// CHANNEL_ID_DIGITAL2 when the channel is configured with SAMPLING_MODE_PERIODIC_MOTION_DETECT.
		/// This value indicates when the detector received an echo.
		/// Distance can be calcuted as (echo time - ping time)*speed_of_sound/2.0.
		/// </summary>
		public const Int32 MEAS_MOTION_DETECTOR_ECHO = 1;
		/// Measurement value reported from NGIO_Device_ReadRawMeasurements() for channel CHANNEL_ID_DIGITAL1 or
		/// CHANNEL_ID_DIGITAL2 when the channel is configured with SAMPLING_MODE_PERIODIC_MOTION_DETECT.
		/// This value indicates when the detector reported a false echo because a real echo was not detected.
		public const Int32 MEAS_MOTION_DETECTOR_FALSE_ECHO = 2;

		/// <summary>
		/// Call NGIO_Init() once before any other NGIO_ calls are made.
		/// NGIO_Init() and NGIO_Uninit() should be called from the same thread.
		/// <para>
		/// Currently, only one application at a time may successfully communicate with LabQuests.
		/// If separate apps call NGIO_Init() before calling NGIO_Uninit(), generally only the first one to 
		/// invoke NGIO_Init() will find devices when it calls NGIO_SearchForDevices() and NGIO_OpenDeviceListSnapshot().
		/// </para>
		/// <para>
		/// On Windows systems, NGIO_SearchForDevices() will not find LabQuests until about 200 milliseconds
		/// after NGIO_Init() is called because of device driver issues. The NGIO_DeviceCheck sample program invokes 
		/// Sleep() between NGIO_Init() and NGIO_SearchForDevices() to cope with this feature.
		/// </para>
		/// 
		/// </summary>
		/// <returns> Handle to NGIO library if successful, else NULL. </returns>
		[DllImport("NGIO_lib.dll", EntryPoint="NGIO_Init", CallingConvention=CallingConvention.Cdecl)]
			public static extern IntPtr Init();

		/// <summary>
		/// Call NGIO_Uninit() once to 'undo' NGIO_Init().
		/// NGIO_Init() and NGIO_Uninit() should be called from the same thread.
		/// </summary>
		/// <param name="hLib"></param>
			/// <returns>0 iff successful, else -1.</returns>
		[DllImport("NGIO_lib.dll", EntryPoint="NGIO_Uninit", CallingConvention=CallingConvention.Cdecl)]
			public static extern Int32 Uninit(IntPtr hLib);

		/// <summary>
		/// NGIO_Diags_SetDebugTraceThreshold().
		/// </summary>
		/// <param name="hLIb"></param>
		/// <param name="threshold">Only trace messages marked with a severity >= threshold(NGIO.TRACE_SEVERITY_) are actually sent to the debug output.</param>
		/// <returns></returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Diags_SetDebugTraceThreshold", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Diags_SetDebugTraceThreshold(
			IntPtr hLIb,
			Int32 threshold);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Diags_GetDebugTraceThreshold", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Diags_GetDebugTraceThreshold(
			IntPtr hLIb,
			out Int32 threshold);

		/// <summary>
		/// This routine returns the major and minor version numbers for the instance of the NGIO library that is
		/// currently running.
		/// 
		/// If a function is not guaranteed to be present in all supported versions of the NGIO library, then the line
		/// "Added in version 'major.minor'" will appear in the function description in this file.
		/// 
		/// It is our intention that all versions of the NGIO library created subsequent to a given version, will be
		/// backwards compatible with the older version. You should be able to replace an old version of the NGIO library
		/// with a newer version and everything should still work without rebuilding your application.
		/// 
		/// Note that version major2.minor2 is later than version major1.minor1 
		/// iff. ((major2 > major1) || ((major2 == major1) && (minor2 > minor1))).
		/// 
		/// Backwards compatibility is definitely our intention, but we do not absolutely guarantee it. If you think
		/// that you have detected a backwards compatibility bug, then please report it to Vernier Software & Technology.
		/// Calling NGIO_GetDLLVersion() from your application is a way to identify precisely which version of
		/// the NGIO library you are actually using.
		/// </summary>
		/// <param name="hLib"></param>
		/// <param name="MajorVersion"></param>
		/// <param name="MinorVersion"></param>
		/// <returns></returns>
		[DllImport("NGIO_lib.dll", EntryPoint="NGIO_GetDLLVersion", CallingConvention=CallingConvention.Cdecl)]
			public static extern Int32 GetDLLVersion(IntPtr hLib, 
												out UInt16 MajorVersion, 
												out UInt16 MinorVersion);

		/// <summary>
		/// The NGIO library maintains a separate list of available devices for each supported device type.
		/// NGIO_SearchForDevices() updates the list for the specified device type.
		/// 
		/// For each device list a signature is maintained that is incremented every time the list changes. The signature
		/// is implemented as a simple 32 bit counter that wraps from 0xffffffff to 0.
		/// 
		/// Let int delta = sig2 - sig1. 
		/// If delta > 0, then sig2 is newer than sig1.
		/// If delta is less than 0, then sig2 is older than sig1.
		/// </summary>
			/// <param name="hLib">[in] handle returned by NGIO_Init()</param>
			/// <param name="deviceType">[in] NGIO_DEVTYPE_...</param>
			/// <param name="commTransportId">[in] NGIO_COMM_TRANSPORT_...</param>
			/// <param name="pParams">[in] ptr to context specific search params. Not currently used.</param>
		/// <param name="DeviceListSignature">[out] Device list signature. Signatures are not fully implemented yet.</param>
			/// <returns>0 iff successful, else -1.</returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_SearchForDevices", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 SearchForDevices(IntPtr hLib,
											UInt32 deviceType,
											UInt32 commTransportId,
											IntPtr pParams,
											out UInt32 DeviceListSignature);

		/// <summary>
		/// The NGIO library maintains a separate list of available devices for each supported device type.
		/// NGIO_SearchForDevices() updates the list for the specified device type.
		/// Eventually, when NGIO_EnableAutoDeviceDiscoveryAcrossCommTransport() is fully implemented, device lists
		/// will be fairly volatile. Until that happens, the only way to update a device list is to call
		/// NGIO_SearchForDevices(). NGIO_OpenDeviceListSnapshot() creates a snapshot copy of the device list for
		/// the specified device type. Use the handle returned by NGIO_OpenDeviceListSnapshot() to walk the device
		/// list.
		/// 
		/// A device is placed in the list snapshot even if it has already been opened.
		/// </summary>
		/// <param name="hLib">[in] handle returned by NGIO_Init()</param>
		/// <param name="deviceType">[in] NGIO_DEVTYPE_...</param>
		/// <param name="numDevices">[out] number of devices in the list</param>
		/// <param name="DeviceListSignature">device list signature. Signatures are not fully implemented yet.</param>
		/// <returns>handle to device list snapshot iff successful, else NULL.</returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_OpenDeviceListSnapshot", CallingConvention = CallingConvention.Cdecl)]
		public static extern IntPtr OpenDeviceListSnapshot(
			IntPtr hLib,
			UInt32 deviceType,
			out UInt32 numDevices,
			out UInt32 DeviceListSignature);

		/// <summary>
		/// Close the list created by NGIO_OpenDeviceListSnapshot().
		/// </summary>
		/// <param name="hList"></param>
		/// <returns>0 iff successful, else -1.</returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_CloseDeviceListSnapshot", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 CloseDeviceListSnapshot(
			IntPtr hList);

		/// <summary>
		/// Return the Nth entry in the list created by NGIO_OpenDeviceListSnapshot().
		/// A device is placed in the list snapshot even if it has already been opened.
		/// Eventually, the bits returned in *pDeviceStatusMask will tell you if the device is already open,
		/// but these status bits are not supported yet.
		/// 
		/// Pass the device name string placed in devnameBuf to NGIO_Device_Open() to open the device. Each
		/// device name string uniquely identifies the device, so you can determine if a device is already open
		/// by comparing devnameBuf with the string returned by NGIO_Device_GetOpenDeviceName() for each open
		/// device handle.
		/// </summary>
		/// <param name="hList">[in] handle returned by NGIO_OpenDeviceListSnapshot()</param>
		/// <param name="N">[in] index into list of known devices, 0 => first device in list.</param>
		/// <param name="devnameBuf">[out] buffer to store device name string. Allocate this with a capacity of NGIO.MAX_SIZE_DEVICE_NAME.</param>
		/// <param name="bufSize">[in] Set this to the capacity of devnameBuf.</param>
		/// <param name="deviceStatusMask">[in] deviceStatusMask is not implemented yet. </param>
		/// <returns>0 iff successful, else -1.</returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_DeviceListSnapshot_GetNthEntry", CallingConvention = CallingConvention.Cdecl, CharSet=CharSet.Ansi)]
		public static extern Int32 DeviceListSnapshot_GetNthEntry(
			IntPtr hList,
			UInt32 N,
			StringBuilder devnameBuf,
			UInt32 bufSize,
			out UInt32 deviceStatusMask);

		/// <summary>
		/// <para>
		/// Open a device with the name returned by NGIO_DeviceListSnapshot_GetNthEntry.
		/// Note that you must almost always call NGIO_Device_AcquireExclusiveOwnership() immediately after calling
		/// NGIO_Device_Open(deviceName) if deviceName corresponds to a basic LabQuest device.
		/// </para>
		/// <para>
		/// </para>
		/// <para>
		/// NGIO_Device_Open() currently opens devices of the types NGIO_DEVTYPE_LABQUEST, NGIO_DEVTYPE_LABQUEST_MINI,
		/// NGIO_DEVTYPE_LABQUEST_FILE_SERVER, NGIO_DEVTYPE_LABQUEST_AUDIO, and NGIO_DEVTYPE_LABQUEST_FUNC_GENERATOR.
		/// NGIO_DEVTYPE_LABQUEST refers to the basic LabQuest device. Standard Vernier analog and digital sensors
		/// are accessed via the NGIO_DEVTYPE_LABQUEST and NGIO_DEVTYPE_LABQUEST_MINI devices. 
		/// The NGIO_DEVTYPE_LABQUEST_AUDIO device is used to access the audio devices(internal and external) that are 
		/// built in to the LabQuest. The NGIO_DEVTYPE_LABQUEST_FILE_SERVER device is a special device that provides the 
		/// caller with access to the Linux file system that is internal to the LabQuest. 
		/// The NGIO_DEVTYPE_LABQUEST_FUNC_GENERATOR device allows the caller to drive the LabQuest audio output.
		/// </para>
		/// <para>
		/// </para>
		/// <para>
		/// For each open NGIO_DEVTYPE_LABQUEST device, there is a corresponding NGIO_DEVTYPE_LABQUEST_FILE_SERVER,
		/// NGIO_DEVTYPE_LABQUEST_AUDIO, and a NGIO_DEVTYPE_LABQUEST_FUNC_GENERATOR device that is a logical 'child' of 
		/// the NGIO_DEVTYPE_LABQUEST device. NGIO_SearchForDevices() will not find any of these child devices until at 
		/// least one parent NGIO_DEVTYPE_LABQUEST device has been opened.
		/// </para>
		/// <para>
		/// Each device type has a command protocol that is unique to that device type. The command protocol used by
		/// NGIO_DEVTYPE_LABQUEST_AUDIO devices is a subset of the protocol used by NGIO_DEVTYPE_LABQUEST and 
		/// NGIO_DEVTYPE_LABQUEST_MINI devices. This protocol is documented in NGIOSourceCmds class.
		/// </para>
		/// <para>
		/// The command protocol used by NGIO_DEVTYPE_LABQUEST_FILE_SERVER devices is documented in NGIOFTLSourceCmds.h 
		/// The command protocol used by NGIO_DEVTYPE_LABQUEST_FUNC_GENERATOR devices is documented in NGIOFGenSourceCmds.h 
		/// </para>
		/// <para>
		/// The following discussion generally only applies to NGIO_DEVTYPE_LABQUEST and NGIO_DEVTYPE_LABQUEST_MINI devices. 
		/// The behaviour of the NGIO_DEVTYPE_LABQUEST_AUDIO devices types is similar, but not always identical. 
		/// NGIO_DEVTYPE_LABQUEST_FILE_SERVER and NGIO_DEVTYPE_LABQUEST_FUNC_GENERATOR devices are very different. The one 
		/// thing that all device types share is support for NGIO_Device_SendCmdAndGetResponse().
		/// </para>
		/// <para>
		/// After establishing basic communication, NGIO_Device_Open() sends a NGIO_CMD_ID_INIT command to the 
		/// NGIO_DEVTYPE_LABQUEST[_MINI] device. In response to NGIO_CMD_ID_INIT, all measurement buffers are cleared and each 
		/// channel is configured with default settings. For NGIO_CHANNEL_ID_ANALOG1 .. NGIO_CHANNEL_ID_ANALOG4,
		/// sampling mode defaults to NGIO_SAMPLING_MODE_PERIODIC_LEVEL_SNAPSHOT, and analog input defaults to 
		/// NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC. For NGIO_CHANNEL_ID_DIGITAL1 and NGIO_CHANNEL_ID_DIGITAL2,
		/// the sampling mode defaults to NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT. No measurements will be taken until
		/// a NGIO_CMD_ID_START_MEASUREMENTS command is sent to the device.
		/// </para>
		/// <para>
		/// Currently, NGIO_Device_Open() does not automatically detect what kinds of sensors are plugged in and it does
		/// not automatically read the DDS memory on the sensor. The application program has to do this manually.
		/// </para>
		/// <para>
		/// After calling NGIO_Device_Open(), an application needs to perform the following operations to take 
		/// measurements:
		/// </para>
		/// <para>
		/// For each channel:
		/// </para>
		/// <para>
		///Send a NGIO_CMD_ID_GET_SENSOR_ID command to identify type of sensor connected on a specified channel.
		/// </para>
		/// <para>
		/// 		----	If the sensor is smart(id >= kSensorIdNumber_FirstSmartSensor) then call 
		/// 				NGIO_Device_DDSMem_ReadRecord() to read the DDS memory from the sensor.
		/// 				Note that NGIO_Device_DDSMem_ReadRecord() reads in the calibration coefficients used by
		/// 				NGIO_Device_CalibrateData(), so NGIO_Device_CalibrateData() will not work correctly unless
		/// 				NGIO_Device_DDSMem_ReadRecord() succeeds.
		/// </para>
		/// <para>
		/// 		----	If the channel is analog, then set the analog input to NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC
		/// 				for most sensors, and set analog input to NGIO_ANALOG_INPUT_PM10V_BUILTIN_12BIT_ADC for
		/// 				+/- 10 volt sensors. 
		/// 				Note that analog sensors default to NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC and
		/// 				NGIO_SAMPLING_MODE_PERIODIC_LEVEL_SNAPSHOT.
		/// </para>
		/// <para>
		///			----	If the channel is digital, then set the sampling mode to a mode appropriate for the connected sensor.
		/// </para>
		/// <para>
		///			----	Call NGIO_Device_SetMeasurementPeriod(channel) to set the sampling period used for periodic measurements.
		///					(channel == -1) => set the sampling period for all channels.
		/// </para>
		/// <para>
		///		--	Send a NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK command to define which channels will take measurements in
		///				the next collection.
		/// </para>
		/// <para>
		///		--	Send a NGIO_CMD_ID_START_MEASUREMENTS command to start taking measurements.
		/// </para>
		/// <para>
		///		--	Call NGIO_Device_ReadRawMeasurements() to get raw binary measurements. Call NGIO_Device_ConvertToVoltage()
		///				and NGIO_Device_CalibrateData() to convert raw measurements to cooked values. NGIO library maintains a large
		///				(>= 10000 measurements) circular measurement buffer for each channel. If a collection exceeds 10000
		///				measurements, then the application needs to pull the measurements out of the buffer by calling
		///				NGIO_Device_ReadRawMeasurements() before the buffer fills.
		/// </para>
		/// <para>
		///		--	Send a NGIO_CMD_ID_STOP_MEASUREMENTS command to stop taking measurements. Once NGIO_CMD_ID_STOP_MEASUREMENTS
		///				has been sent, the app can take its time about calling NGIO_Device_ReadRawMeasurements() to pull measurements
		///				out of the measurement buffers. However, the app must empty the measurement buffers before sending
		///				NGIO_CMD_ID_START_MEASUREMENTS again.
		/// </para>
		/// <para>
		///		At any given time, a device is 'owned' by only one thread. The thread that calls this routine is the
		///		initial owner of the device. If an NGIO() call is made from a thread that does not own the device object
		///		that is passed in, then the call will generally fail. To allow another thread to access a device,
		///		the owning thread must call NGIO_Device_Unlock(), and then the new thread should call NGIO_Device_Lock().
		/// </para>
		/// <para>
		///		We currently have some additional restrictions on what threads can open and close a LabQuest device.
		///		A device must be closed on the same thread that initially opened it.
		///		Furthermore, the audio, function generator, and file server child devices of a LabQuest must be opened 
		///		and closed on the same thread that initially opened the LabQuest. Once opened, you can transfer ownership of 
		///		the devices to other threads and operate on the devices. However, before you close the devices, you must 
		///		transfer ownership back to the single thread that the LabQuest and all its children were opened on. 
		///		Eventually, we plan to relax these restrictions, but for now, that is the way things are.
		/// </para>
		/// </summary>
		/// <param name="hLib"></param>
		/// <param name="deviceName"></param>
		/// <param name="bDemandExclusiveOwnership"> LabQuest requires that this be set to 0, and call
		/// NGIO_Device_AcquireExclusiveOwnership() before doing anything else. </param>
		/// <returns>handle to open device if successful, else NULL.</returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Open", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern IntPtr Device_Open(
			IntPtr hLib,
			String deviceName,
			byte bDemandExclusiveOwnership);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Close", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Close(
			IntPtr hDevice);

		/// <summary>
		/// NGIO_Device_AcquireExclusiveOwnership()
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="timeoutMs">NGIO_GRAB_DAQ_TIMEOUT is recommended.</param>
		/// <returns></returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_AcquireExclusiveOwnership", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_AcquireExclusiveOwnership(
			IntPtr hDevice,
			UInt32 timeoutMs);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_GetParentDeviceHandleFromChildName", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern Int32 GetParentDeviceHandleFromChildName(
			String childDeviceName,
			out IntPtr parentDeviceHandle);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_GetDeviceTypeFromDeviceName", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern Int32 GetDeviceTypeFromDeviceName(
			String deviceName,
			out UInt32 deviceType);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_IsRemoteCollectionActive", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_IsRemoteCollectionActive(
			IntPtr hDevice,
			out byte remoteCollectionActiveFlag,
			UInt32 timeoutMs);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_IsDeviceExclusivelyOurs", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 IsDeviceExclusivelyOurs(
			IntPtr hDevice,
			out byte exclusiveOwnershipFlag);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetOpenDeviceName", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern Int32 Device_GetOpenDeviceName(
			IntPtr hDevice,
			StringBuilder devnameBuf,
			UInt32 bufSize);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Lock", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Lock(
			IntPtr hDevice,
			UInt32 timeoutMs);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Unlock", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Unlock(
			IntPtr hDevice);

		/// <summary>
		/// NGIO_Device_ClearIO()
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="channel">-1 implies all channels</param>
		/// <returns></returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_ClearIO", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_ClearIO(
			IntPtr hDevice,
			sbyte channel);

		/// <summary>
		/// NGIO_Device_SendCmdAndGetResponse() is a low level function that most .NET code should not use directly.
		/// Use one of the NGIO_Device_SendCmdAndGetResponseN() helper functions instead.
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="cmd"></param>
		/// <param name="parameters"></param>
		/// <param name="nParamBytes"></param>
		/// <param name="response"></param>
		/// <param name="nRespBytes">size of of response buffer on input, # of bytes copied into response buffer on output</param>
		/// <param name="timeoutMs"></param>
		/// <returns>0 if successful, else -1.</returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_SendCmdAndGetResponse", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_SendCmdAndGetResponse(
			IntPtr hDevice,
			byte cmd,
			IntPtr parameters,
			UInt32 nParamBytes,
			IntPtr response,
			ref UInt32 nRespBytes,
			UInt32 timeoutMs);

		/// <summary>
		/// Send a command to the specified device hardware and wait for a response. 
		/// <para>
		/// Each device type has a command protocol that is unique to that device type. The command protocol used by
		/// NGIO_DEVTYPE_LABQUEST_AUDIO devices is a subset of the protocol used by NGIO_DEVTYPE_LABQUEST devices. This
		/// protocol is documented in the NGIOSourceCmds class. Additional documentation can be found in NGIOSourceCmds.h.
		/// </para>
		/// <para>
		/// Note that NGIO_Device_SendCmdAndGetResponse() will fail if you send a NGIO_CMD_ID_START_MEASUREMENTS
		/// command while NGIO_Device_GetNumMeasurementsAvailable() says measurements are available. 
		/// </para>
		/// <para>
		/// So...if you are restarting measurements, you should clear any old measurements in the NGIO Measurement 
		/// Buffer first by reading in the measurements until the Buffer is empty, or you should call NGIO_Device_ClearIO().
		/// </para>
		/// <para>
		/// Every command supported by NGIO_Device_SendCmdAndGetResponse() has an associated response. If no response
		/// specific to a command is defined, then the format of the response is NGIODefaultCmdResponse. Some commands
		/// have associated parameter blocks defined for them. 
		/// </para>
		/// <para>
		/// If NGIO_Device_SendCmdAndGetResponse() returns -1, additional information about a NGIO_Device_SendCmdAndGetResponse() 
		/// error may be obtained by calling NGIO_Device_GetLastCmdResponseStatus().
		/// </para>
		/// </summary>
		/// <param name="hDevice">[in] handle to open device.</param>
		/// <param name="cmd">[in] command code.</param>
		/// <param name="parameters">[in] cmd specific parameter block</param>
		/// <param name="response">[out] response block</param>
		/// <param name="nRespBytes">[out] num of bytes passed back in the response block. 
		/// Caller does not need to initialize this with the size of response block(as unmanaged code does) because the dotNET
		/// wrapper does so.
		/// </param>
		/// <param name="timeoutMs">[in] # of milliseconds to wait for a reply before giving up. Most devices should reply to 
		/// almost all the currently defined commands within NGIO.TIMEOUT_MS_DEFAULT milliseconds. In fact, typical response
		/// times are less than 50 milliseconds. </param>
		/// <returns>0 if successful, else -1.</returns>
		public static Int32 Device_SendCmdAndGetResponse1(
			IntPtr hDevice,
			byte cmd,
			object parameters,
			ref object response,
			ref UInt32 nRespBytes,
			UInt32 timeoutMs)
		{
			IntPtr paramPtr = Marshal.AllocHGlobal(Marshal.SizeOf(parameters));
			Marshal.StructureToPtr(parameters, paramPtr, true);
			IntPtr responsePtr = Marshal.AllocHGlobal(Marshal.SizeOf(response));
			nRespBytes = (UInt32)Marshal.SizeOf(response);
			Int32 status = Device_SendCmdAndGetResponse(hDevice, cmd, paramPtr, (UInt32)Marshal.SizeOf(parameters), 
				responsePtr, ref nRespBytes, timeoutMs);
			if (0 == status)
				response = Marshal.PtrToStructure(responsePtr, response.GetType());
			else if ((status < 0) && (1 == nRespBytes) && (1 == Marshal.SizeOf(response)))
				response = Marshal.PtrToStructure(responsePtr, response.GetType());
			Marshal.FreeHGlobal(responsePtr);
			Marshal.FreeHGlobal(paramPtr);
			return status;
		}

		/// <summary>
		/// Device_SendCmdAndGetResponse2() is the same as Device_SendCmdAndGetResponse1() except that no response block is
		/// returned by the function(even though some sort of response always comes back from the device unless there is a 
		/// communication failure). This is reasonable if the caller only cares about when the function succeeds or fails, 
		/// which can be determined from the return value.
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="cmd"></param>
		/// <param name="parameters"></param>
		/// <param name="timeoutMs"></param>
		/// <returns>0 if successful, else -1.</returns>
		public static Int32 Device_SendCmdAndGetResponse2(
			IntPtr hDevice,
			byte cmd,
			object parameters,
			UInt32 timeoutMs)
		{
			UInt32 respLen = 0;
			IntPtr paramPtr = Marshal.AllocHGlobal(Marshal.SizeOf(parameters));
			Marshal.StructureToPtr(parameters, paramPtr, true);
			Int32 status = Device_SendCmdAndGetResponse(hDevice, cmd, paramPtr, (UInt32)Marshal.SizeOf(parameters),
				IntPtr.Zero, ref respLen, timeoutMs);
			Marshal.FreeHGlobal(paramPtr);
			return status;
		}

		/// <summary>
		/// Device_SendCmdAndGetResponse3() is the same as Device_SendCmdAndGetResponse1() except that no parameter block is sent
		/// to the device. That works fine for commands that do not require parameters.
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="cmd"></param>
		/// <param name="response"></param>
		/// <param name="nRespBytes"></param>
		/// <param name="timeoutMs"></param>
		/// <returns>0 if successful, else -1.</returns>
		public static Int32 Device_SendCmdAndGetResponse3(
			IntPtr hDevice,
			byte cmd,
			ref object response,
			ref UInt32 nRespBytes,
			UInt32 timeoutMs)
		{
			IntPtr responsePtr = Marshal.AllocHGlobal(Marshal.SizeOf(response));
			nRespBytes = (UInt32)Marshal.SizeOf(response);
			Int32 status = Device_SendCmdAndGetResponse(hDevice, cmd, IntPtr.Zero, 0, responsePtr, ref nRespBytes, timeoutMs);
			if (0 == status)
				response = Marshal.PtrToStructure(responsePtr, response.GetType());
			else if ((status < 0) && (1 == nRespBytes) && (1 == Marshal.SizeOf(response)))
				response = Marshal.PtrToStructure(responsePtr, response.GetType());
			Marshal.FreeHGlobal(responsePtr);
			return status;
		}

		/// <summary>
		/// Device_SendCmdAndGetResponse4() is the same as Device_SendCmdAndGetResponse1() except that no parameter block is sent
		/// to the device and no response block is returned by the function. That works fine for commands that do not require 
		/// parameters when the caller is only interested in success or failure.
		/// 
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="cmd"></param>
		/// <param name="timeoutMs"></param>
		/// <returns>0 if successful, else -1.</returns>
		public static Int32 Device_SendCmdAndGetResponse4(
			IntPtr hDevice,
			byte cmd,
			UInt32 timeoutMs)
		{
			UInt32 respLen = 0;
			Int32 status = Device_SendCmdAndGetResponse(hDevice, cmd, IntPtr.Zero, 0, IntPtr.Zero, ref respLen, timeoutMs);
			return status;
		}

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetMeasurementTick", CallingConvention = CallingConvention.Cdecl)]
		public static extern double Device_GetMeasurementTick(
			IntPtr hDevice,
			sbyte channel);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetMinimumMeasurementPeriod", CallingConvention = CallingConvention.Cdecl)]
		public static extern double Device_GetMinimumMeasurementPeriod(
			IntPtr hDevice,
			sbyte channel);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetMaximumMeasurementPeriod", CallingConvention = CallingConvention.Cdecl)]
		public static extern double Device_GetMaximumMeasurementPeriod(
			IntPtr hDevice,
			sbyte channel);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_SetMeasurementPeriod", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_SetMeasurementPeriod(
			IntPtr hDevice,
			sbyte channel,
			Double desiredPeriod,
			UInt32 timeoutMs);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetMeasurementPeriod", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_GetMeasurementPeriod(
			IntPtr hDevice,
			sbyte channel,
			out Double period,
			UInt32 timeoutMs);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetNumMeasurementsAvailable", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_GetNumMeasurementsAvailable(
			IntPtr hDevice,
			sbyte channel);

		/// <summary>
		/// Retrieve measurements from the NGIO Measurement Buffer for a specified channel. The measurements reported
		/// by this routine are actually removed from the NGIO Measurement Buffer.
		/// <para>
		/// After NGIO_CMD_ID_START_MEASUREMENTS has been sent to the device, the device starts
		/// sending measurements to the host computer. These measurements are stored in the 
		/// NGIO Measurement Buffer. A separate NGIO Measurement Buffer is maintained for each
		/// channel of the device.
		/// </para>
		/// <para>
		/// See the description of NGIO_Device_GetNumMeasurementsAvailable().
		/// </para>
		/// <para>
		/// If the current sampling mode is periodic with period Tp, then the first measurement in a
		/// channel's NGIO Measurement Buffer occurred at time 0, the next measurement occurred at time Tp,
		/// then time 2*Tp, 3*Tp, and so on. If several different channels are sampled at the same rate, then
		/// at times n*Tp, the separate channels are sampled concurrently. In fact, if the hardware is configured
		/// two measure two different channels at the same time, the measurements occur several microseconds apart,
		/// but we generally report these measurements to have occurred at the same time.
		/// </para>
		/// <para>
		/// The timestamps array reports when measurements are taken. Timestamps have a 1 'tick' resolution, which is 1 
		/// microsecond for the LabQuest. Timestamps are reported for all device types on all channels with one 
		/// exception: NGIO_DEVTYPE_LABQUEST_AUDIO.
		/// Because the measurement period for the LabQuest audio device is generally not an integer multiple of the
		/// microsecond tick, timestamps are not reported for this device. Instead, the caller must recreate the
		/// audio timestamps by incrementing the gtype_real64 period reported by NGIO_Device_GetMeasurementPeriod().
		/// </para>
		/// <para>
		/// NGIO_SAMPLING_MODE_PERIODIC_LEVEL_SNAPSHOT:
		/// </para>
		/// <para>
		/// This sampling mode is used with NGIO_CHANNEL_ID_ANALOG1 thru NGIO_CHANNEL_ID_ANALOG4.
		/// The values returned are generally 16 bit integers obtained from an analog to digital converter.
		/// Use NGIO_Device_ConvertToVoltage() to convert the raw value to volts, and use 
		/// NGIO_Device_CalibrateData() to convert volts to sensor specific units.
		/// Use NGIO_Device_DDSMem_GetCalPage to identify calibration units.
		/// </para>
		/// <para>
		/// NGIO_SAMPLING_MODE_APERIODIC_EDGE_DETECT:
		/// </para>
		/// <para>
		/// Used with photogates and drop counters in NGIO_CHANNEL_ID_DIGITAL1 and NGIO_CHANNEL_ID_DIGITAL2.
		/// Values returned are NGIO_MEAS_PHOTOGATE_BLOCKED or NGIO_MEAS_PHOTOGATE_UNBLOCKED.
		/// </para>
		/// <para>
		/// NGIO_SAMPLING_MODE_PERIODIC_MOTION_DETECT:
		/// </para>
		/// <para>
		/// Used with motion detectors in NGIO_CHANNEL_ID_DIGITAL1 and NGIO_CHANNEL_ID_DIGITAL2.
		/// Values returned are NGIO_MEAS_MOTION_DETECTOR_PING, NGIO_MEAS_MOTION_DETECTOR_ECHO,
		/// or NGIO_MEAS_MOTION_DETECTOR_FALSE_ECHO. A false echo is reported if no echo is detected after a ping.
		/// Distance is calculated = 0.5*(echo time - previous ping time)*(speed of sound) since the delta
		/// time is how long the ping audio pulse takes to travel from the detector to the object and then back
		/// to the detector.
		/// </para>
		/// <para>
		/// NGIO_SAMPLING_MODE_PERIODIC_PULSE_COUNT:
		/// </para>
		/// <para>
		/// Used with radiation counters in NGIO_CHANNEL_ID_DIGITAL1 and NGIO_CHANNEL_ID_DIGITAL2.
		/// Values returned are cumulative counts that range from 0 to 2147483647.
		/// The count is set to 0 when the mode is configured, and the device starts keeping track at
		/// mode configuration time. The first measurement returned is the number of counts detected between
		/// initial configuration and when NGIO_CMD_ID_START_MEASUREMENTS is processed. You can reset the count
		/// using NGIO_CMD_ID_SET_DIGITAL_COUNTER. If the application wants to act like a geiger counter, it needs
		/// to calculate the differences between successive measurement counts.
		/// </para>
		/// <para>
		/// NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER:
		/// </para>
		/// <para>
		/// NGIO_SAMPLING_MODE_PERIODIC_ROTATION_COUNTER_X4:
		/// </para>
		/// <para>
		/// Used with rotary motion sensors in NGIO_CHANNEL_ID_DIGITAL1 and NGIO_CHANNEL_ID_DIGITAL2.
		/// Values returned are net cumulative counts that range from -2147483648 to 2147483647.
		/// Convert from counts to CCW wheel rotations by dividing by 360 or (360x4).
		/// The count is set to 0 when the mode is configured, and the device starts keeping track at
		/// mode configuration time. The first measurement returned is the number of counts detected between
		/// initial configuration and when NGIO_CMD_ID_START_MEASUREMENTS is processed. You can reset the count
		/// using NGIO_CMD_ID_SET_DIGITAL_COUNTER. Note that these counts are 'net' counts, so spin the wheel CCW to
		/// increase the count, spin the wheel CW to decrease the count.
		/// </para>
		/// </summary>
		/// <param name="hDevice">[in] Handle to open device.</param>
		/// <param name="channel">[in]</param>
		/// <param name="measurements">[out]Loc to store measurements.</param>
		/// <param name="timestamps">[out] Loc to store 'tick' resolution timestamps. Timestamp is 0 at time of 
		/// trigger. For normal real time mode, measurements are triggered when NGIO_CMD_ID_START_MEASUREMENTS is 
		/// received.</param>
		/// <param name="maxCount">Maximum number of measurements to copy to measurements array. The measurements and timestamps 
		/// arrays passed in as parameters must be allocated with a length of at least maxCount elements.</param>
		/// <returns>Number of measurements retrieved from the NGIO Measurement Buffer. This routine returns 
		/// immediately, so the return value may be less than maxCount. Return value less than 0 implies error.</returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_ReadRawMeasurements", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_ReadRawMeasurements(
			IntPtr hDevice,
			sbyte channel,
			Int32[] measurements,
			Int64[] timestamps,
			UInt32 maxCount);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetLatestRawMeasurement", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_GetLatestRawMeasurement(
			IntPtr hDevice,
			sbyte channel);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_ConvertToVoltage", CallingConvention = CallingConvention.Cdecl)]
		public static extern Single Device_ConvertToVoltage(
			IntPtr hDevice,
			sbyte channel,
			Int32 rawMeasurement,
			Int32 probeType);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_CalibrateData", CallingConvention = CallingConvention.Cdecl)]
		public static extern Single Device_CalibrateData(
			IntPtr hDevice,
			sbyte channel,
			Single volts);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_CalibrateData2", CallingConvention = CallingConvention.Cdecl)]
		public static extern Single Device_CalibrateData2(
			IntPtr hDevice,
			sbyte channel,
			Int32 rawMeasurement);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_UncalibrateData", CallingConvention = CallingConvention.Cdecl)]
		public static extern Single Device_UncalibrateData(
			IntPtr hDevice,
			sbyte channel,
			Single measurement);

		/// <summary>
		/// NGIO_Device_GetProbeType()
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="channel"></param>
		/// <returns>VSTSensorDDSMemRec.kProbeType...</returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetProbeType", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_GetProbeType(
			IntPtr hDevice,
			sbyte channel);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_GetChannelRange", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_GetChannelRange(
			IntPtr hDevice,
			out UInt32 minChannel,
			out UInt32 maxChannel);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_NVMemBlk_GetAddrRange", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_NVMemBlk_GetAddrRange(
			IntPtr hDevice,
			sbyte channel,
			out UInt32 minAddr,
			out UInt32 maxAddr);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_WriteRecord", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_WriteRecord(
			IntPtr hDevice,
			sbyte channel,
			UInt32 timeoutMs);

		/// <summary>
		/// NGIO_Device_DDSMem_ReadRecord().
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="channel"></param>
		/// <param name="strictDDSValidationFlag">insist on exactly valid checksum if 1, else use a more lax validation test.</param>
		/// <param name="timeoutMs"></param>
		/// <returns></returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_ReadRecord", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_ReadRecord(
			IntPtr hDevice,
			sbyte channel,
			byte strictDDSValidationFlag,
			UInt32 timeoutMs);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetRecord", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetRecord(
			IntPtr hDevice,
			sbyte channel,
			ref VSTSensorDDSMemRec rec);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetRecord", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetRecord(
			IntPtr hDevice,
			sbyte channel,
			out VSTSensorDDSMemRec rec);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_ClearRecord", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_ClearRecord(
			IntPtr hDevice,
			sbyte channel);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_CalculateChecksum", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_CalculateChecksum(
			IntPtr hDevice,
			sbyte channel,
			out byte checksum);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetMemMapVersion", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetMemMapVersion(
			IntPtr hDevice,
			sbyte channel,
			byte MemMapVersion);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetMemMapVersion", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetMemMapVersion(
			IntPtr hDevice,
			sbyte channel,
			out byte MemMapVersion);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetSensorNumber", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetSensorNumber(
			IntPtr hDevice,
			sbyte channel,
			byte SensorNumber);

		/// <summary>
		/// NGIO_Device_DDSMem_GetSensorNumber().
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="channel"></param>
		/// <param name="SensorNumber"></param>
		/// <param name="sendQueryToHardwareflag">If sendQueryToHardwareflag != 0, then send a NGIO_CMD_ID_GET_SENSOR_ID to the sensor hardware.</param>
		/// <param name="channelSignature"></param>
		/// <param name="timeoutMs"># of milliseconds to wait for a reply before giving up. NGIO.TIMEOUT_MS_DEFAULT is recommended.</param>
		/// <returns></returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetSensorNumber", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetSensorNumber(
			IntPtr hDevice,
			sbyte channel,
			out byte SensorNumber,
			byte sendQueryToHardwareflag,
			out UInt32 channelSignature,
			UInt32 timeoutMs);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetSerialNumber", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetSerialNumber(
			IntPtr hDevice,
			sbyte channel,
			byte leastSigByte_SerialNumber,
			byte midSigByte_SerialNumber,
			byte mostSigByte_SerialNumber);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetSerialNumber", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetSerialNumber(
			IntPtr hDevice,
			sbyte channel,
			out byte leastSigByte_SerialNumber,
			out byte midSigByte_SerialNumber,
			out byte mostSigByte_SerialNumber);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetLotCode", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetLotCode(
			IntPtr hDevice,
			sbyte channel,
			byte YY_BCD,
			byte WW_BCD);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetLotCode", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetLotCode(
			IntPtr hDevice,
			sbyte channel,
			out byte YY_BCD,
			out byte WW_BCD);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetManufacturerID", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetManufacturerID(
			IntPtr hDevice,
			sbyte channel,
			byte ManufacturerID);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetManufacturerID", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetManufacturerID(
			IntPtr hDevice,
			sbyte channel,
			out byte ManufacturerID);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetLongName", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern IntPtr Device_DDSMem_SetLongName(
			IntPtr hDevice,
			sbyte channel,
			String longName);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetLongName", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern IntPtr Device_DDSMem_GetLongName(
			IntPtr hDevice,
			sbyte channel,
			StringBuilder longName,
			UInt16 maxNumBytesToCopy);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetShortName", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern IntPtr Device_DDSMem_SetShortName(
			IntPtr hDevice,
			sbyte channel,
			String shortName);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetShortName", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern IntPtr Device_DDSMem_GetShortName(
			IntPtr hDevice,
			sbyte channel,
			StringBuilder shortName,
			UInt16 maxNumBytesToCopy);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetUncertainty", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetUncertainty(
			IntPtr hDevice,
			sbyte channel,
			byte Uncertainty);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetUncertainty", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetUncertainty(
			IntPtr hDevice,
			sbyte channel,
			out byte Uncertainty);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetSignificantFigures", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetSignificantFigures(
			IntPtr hDevice,
			sbyte channel,
			byte SignificantFigures);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetSignificantFigures", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetSignificantFigures(
			IntPtr hDevice,
			sbyte channel,
			out byte SignificantFigures);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetCurrentRequirement", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetCurrentRequirement(
			IntPtr hDevice,
			sbyte channel,
			byte CurrentRequirement);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetCurrentRequirement", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetCurrentRequirement(
			IntPtr hDevice,
			sbyte channel,
			out byte CurrentRequirement);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetAveraging", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetAveraging(
			IntPtr hDevice,
			sbyte channel,
			byte Averaging);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetAveraging", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetAveraging(
			IntPtr hDevice,
			sbyte channel,
			out byte Averaging);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetMinSamplePeriod", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetMinSamplePeriod(
			IntPtr hDevice,
			sbyte channel,
			Single MinSamplePeriod);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetMinSamplePeriod", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetMinSamplePeriod(
			IntPtr hDevice,
			sbyte channel,
			out Single MinSamplePeriod);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetTypSamplePeriod", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetTypSamplePeriod(
			IntPtr hDevice,
			sbyte channel,
			Single TypSamplePeriod);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetTypSamplePeriod", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetTypSamplePeriod(
			IntPtr hDevice,
			sbyte channel,
			out Single TypSamplePeriod);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetTypNumberofSamples", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetTypNumberofSamples(
			IntPtr hDevice,
			sbyte channel,
			UInt16 TypNumberofSamples);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetTypNumberofSamples", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetTypNumberofSamples(
			IntPtr hDevice,
			sbyte channel,
			out UInt16 TypNumberofSamples);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetWarmUpTime", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetWarmUpTime(
			IntPtr hDevice,
			sbyte channel,
			UInt16 WarmUpTime);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetWarmUpTime", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetWarmUpTime(
			IntPtr hDevice,
			sbyte channel,
			out UInt16 WarmUpTime);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetExperimentType", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetExperimentType(
			IntPtr hDevice,
			sbyte channel,
			byte ExperimentType);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetExperimentType", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetExperimentType(
			IntPtr hDevice,
			sbyte channel,
			out byte ExperimentType);

		/// <summary>
		/// NGIO_Device_DDSMem_SetOperationType() can change the probe type. See NGIO_Device_GetProbeType().
		/// </summary>
		/// <param name="hDevice"></param>
		/// <param name="channel"></param>
		/// <param name="OperationType"></param>
		/// <returns></returns>
		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetOperationType", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetOperationType(
			IntPtr hDevice,
			sbyte channel,
			byte OperationType);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetOperationType", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetOperationType(
			IntPtr hDevice,
			sbyte channel,
			out byte OperationType);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetCalibrationEquation", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetCalibrationEquation(
			IntPtr hDevice,
			sbyte channel,
			sbyte CalibrationEquation);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetCalibrationEquation", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetCalibrationEquation(
			IntPtr hDevice,
			sbyte channel,
			out sbyte CalibrationEquation);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetYminValue", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetYminValue(
			IntPtr hDevice,
			sbyte channel,
			Single YminValue);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetYminValue", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetYminValue(
			IntPtr hDevice,
			sbyte channel,
			out Single YminValue);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetYmaxValue", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetYmaxValue(
			IntPtr hDevice,
			sbyte channel,
			Single YmaxValue);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetYmaxValue", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetYmaxValue(
			IntPtr hDevice,
			sbyte channel,
			out Single YmaxValue);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetYscale", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetYscale(
			IntPtr hDevice,
			sbyte channel,
			byte Yscale);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetYscale", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetYscale(
			IntPtr hDevice,
			sbyte channel,
			out byte Yscale);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetHighestValidCalPageIndex", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetHighestValidCalPageIndex(
			IntPtr hDevice,
			sbyte channel,
			byte HighestValidCalPageIndex);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetHighestValidCalPageIndex", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetHighestValidCalPageIndex(
			IntPtr hDevice,
			sbyte channel,
			out byte HighestValidCalPageIndex);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetActiveCalPage", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetActiveCalPage(
			IntPtr hDevice,
			sbyte channel,
			byte ActiveCalPage);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetActiveCalPage", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetActiveCalPage(
			IntPtr hDevice,
			sbyte channel,
			out byte ActiveCalPage);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetCalPage", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern Int32 Device_DDSMem_SetCalPage(
			IntPtr hDevice,
			sbyte channel,
			byte CalPageIndex,
			float CalibrationCoefficientA,
			float CalibrationCoefficientB,
			float CalibrationCoefficientC,
			String units);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetCalPage", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern Int32 Device_DDSMem_GetCalPage(
			IntPtr hDevice,
			sbyte channel,
			byte CalPageIndex,
			out float CalibrationCoefficientA,
			out float CalibrationCoefficientB,
			out float CalibrationCoefficientC,
			StringBuilder units,
			UInt16 maxNumBytesToCopy);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_SetChecksum", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_SetChecksum(
			IntPtr hDevice,
			sbyte channel,
			byte Checksum);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_DDSMem_GetChecksum", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_DDSMem_GetChecksum(
			IntPtr hDevice,
			sbyte channel,
			byte Checksum);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_AllocateCollectionFrames", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_AllocateCollectionFrames(
			IntPtr hDevice,
			UInt16 numFrames,
			ref NGIOSetCollectionParams collectionParams);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_FreeCollectionFrames", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_FreeCollectionFrames(
			IntPtr hDevice);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_GetNumCollectionFramesAllocated", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_GetNumCollectionFramesAllocated(
			IntPtr hDevice,
			out UInt16 numFrames);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_GetCapturedFrameRange", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_GetCapturedFrameRange(
			IntPtr hDevice,
			out UInt32 firstFrameId,
			out UInt16 numCapturedFrames);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_GetQuickAndDirtyCapturedFrameRange", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_GetQuickAndDirtyCapturedFrameRange(
			IntPtr hDevice,
			out UInt32 firstFrameId,
			out UInt16 numCapturedFrames);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_RetireCapturedFrame", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_RetireCapturedFrame(
			IntPtr hDevice,
			UInt32 frameId);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_GetInterframeDelay", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_GetInterframeDelay(
			IntPtr hDevice,
			UInt32 firstFrameId,
			UInt32 secondFrameId,
			out Int64 interFrameDelay,
			out Int64 minInterFrameDelay);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_GetNumMeasurementsAvailable", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_GetNumMeasurementsAvailable(
			IntPtr hDevice,
			UInt32 frameId,
			sbyte channel);

		[DllImport("NGIO_lib.dll", EntryPoint = "NGIO_Device_Frm_CopyRawMeasurements", CallingConvention = CallingConvention.Cdecl)]
		public static extern Int32 Device_Frm_CopyRawMeasurements(
			IntPtr hDevice,
			UInt32 frameId,
			sbyte channel,
			UInt32 firstMeasurementIndex,
			Int32[] measurements,
			Int64[] timestamps,
			UInt32 maxCount);

	}
}
