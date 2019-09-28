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

// NGIO_DeviceCheck.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <string.h>
#include <memory.h>

#ifdef TARGET_OS_WIN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#pragma warning(disable: 4996)
#include <windows.h>
#endif
#ifdef TARGET_OS_LINUX
#include <sys/time.h>
#endif
#ifdef TARGET_OS_MAC
#include <Carbon/Carbon.h>
#endif

#include "NGIO_lib_interface.h"

#define MAX_NUM_MEASUREMENTS 200

static void OSSleep(unsigned int msToSleep);

NGIO_LIBRARY_HANDLE g_hNGIOlib = NULL;
NGIO_DEVICE_HANDLE g_hDevice = NULL;

int main(int argc, char* argv[])
{
	char tmpstring[300];
	gtype_uint16 majorVersion, minorVersion;
	gtype_int32 status = 0;
	gtype_uint32 sig, mask, deviceType;
	gtype_uint32 numDevices;
	NGIO_DEVICE_LIST_HANDLE hDeviceList;
	char deviceName[NGIO_MAX_SIZE_DEVICE_NAME];
	NGIOGetStatusCmdResponsePayload getStatusResponse;
	NGIO_NVMEM_CHANNEL_ID1_rec getNVMemResponse;
	gtype_uint32 nRespBytes;
	signed char channel;
	char units[20];

	gtype_int32 rawMeasurements[MAX_NUM_MEASUREMENTS];
	gtype_real32 volts[MAX_NUM_MEASUREMENTS];
	gtype_real32 calbMeasurements[MAX_NUM_MEASUREMENTS];
	gtype_int32 numMeasurements, i;
	gtype_real32 averageCalbMeasurement;

	g_hNGIOlib = NGIO_Init();
	NGIO_GetDLLVersion(g_hNGIOlib, &majorVersion, &minorVersion);

	printf("NGIO_DeviceCheck version 1.2 . \n");
	sprintf(tmpstring, "NGIO_DeviceCheck is linked to NGIO library version %02d.%02d .\n", majorVersion, minorVersion);
	printf(tmpstring);

#ifdef TARGET_OS_WIN
	OSSleep(500);//Give Jungo device driver time to find the LabQuest.
#endif

	if (g_hNGIOlib)
	{
		deviceType = NGIO_DEVTYPE_LABQUEST;
		NGIO_SearchForDevices(g_hNGIOlib, deviceType, NGIO_COMM_TRANSPORT_USB, NULL, &sig);

		hDeviceList = NGIO_OpenDeviceListSnapshot(g_hNGIOlib, deviceType, &numDevices, &sig);
		status = NGIO_DeviceListSnapshot_GetNthEntry(hDeviceList, 0, deviceName, sizeof(deviceName), &mask);
		NGIO_CloseDeviceListSnapshot(hDeviceList);

		if (0 != status)
		{
			//Couldn't find a LabQuest, so look for a LabQuest Mini.
			deviceType = NGIO_DEVTYPE_LABQUEST2;
			NGIO_SearchForDevices(g_hNGIOlib, deviceType, NGIO_COMM_TRANSPORT_USB, NULL, &sig);

			hDeviceList = NGIO_OpenDeviceListSnapshot(g_hNGIOlib, deviceType, &numDevices, &sig);
			status = NGIO_DeviceListSnapshot_GetNthEntry(hDeviceList, 0, deviceName, sizeof(deviceName), &mask);
			NGIO_CloseDeviceListSnapshot(hDeviceList);
		}

		if (0 != status)
		{
			//Couldn't find a LabQuest, so look for a LabQuest Mini.
			deviceType = NGIO_DEVTYPE_LABQUEST_MINI;
			NGIO_SearchForDevices(g_hNGIOlib, deviceType, NGIO_COMM_TRANSPORT_USB, NULL, &sig);

			hDeviceList = NGIO_OpenDeviceListSnapshot(g_hNGIOlib, deviceType, &numDevices, &sig);
			status = NGIO_DeviceListSnapshot_GetNthEntry(hDeviceList, 0, deviceName, sizeof(deviceName), &mask);
			NGIO_CloseDeviceListSnapshot(hDeviceList);
		}

		if (0 != status)
		{
			printf("NGIO_DeviceCheck cannot find a LabQuest, LabQuest2, or a LabQuest Mini.\n");
		}
		else
		{
			char deviceDesc[50];
			if (NGIO_DEVTYPE_LABQUEST == deviceType)
				strcpy(deviceDesc, "LabQuest");
			else if (NGIO_DEVTYPE_LABQUEST2 == deviceType)
				strcpy(deviceDesc, "LabQuest2");
			else
				strcpy(deviceDesc, "LabQuest Mini");
			g_hDevice = NGIO_Device_Open(g_hNGIOlib, deviceName, 0);
			if (!g_hDevice)
				printf("Failed to open %s device %s \n", deviceDesc, deviceName);
			else
				printf("Successfully opened %s device %s \n", deviceDesc, deviceName);

			if (g_hDevice)
			{
				if ((NGIO_DEVTYPE_LABQUEST == deviceType) || (NGIO_DEVTYPE_LABQUEST2 == deviceType))
				{
					//Wrest control of the LabQuest data acquisition subsystem(the DAQ) away from the GUI app running
					//down on the LabQuest.
					status = NGIO_Device_AcquireExclusiveOwnership(g_hDevice, NGIO_GRAB_DAQ_TIMEOUT);
					if (0 != status)
						printf("NGIO_Device_AcquireExclusiveOwnership() failed!\n");
				}

				if (0 == status)
				{
					memset(&getStatusResponse, 0, sizeof(getStatusResponse));
					nRespBytes = sizeof(getStatusResponse);
					status = NGIO_Device_SendCmdAndGetResponse(g_hDevice, NGIO_CMD_ID_GET_STATUS, NULL, 0, &getStatusResponse,
						&nRespBytes, NGIO_TIMEOUT_MS_DEFAULT);
				}

				if (0 == status)
				{
					printf("DAQ firmware version is %x.%02x .\n", (gtype_uint16) getStatusResponse.majorVersionMasterCPU, 
						(gtype_uint16) getStatusResponse.minorVersionMasterCPU);

					memset(&getNVMemResponse, 0, sizeof(getNVMemResponse));
					status = NGIO_Device_NVMemBlk_Read(g_hDevice, NGIO_NVMEM_CHANNEL_ID1, &getNVMemResponse, 0,
						sizeof(getNVMemResponse) - 1, NGIO_TIMEOUT_MS_DEFAULT);
				}

				if (0 == status)
				{
					unsigned int serialNum = getNVMemResponse.serialNumber.msbyteMswordSerialCounter;
					serialNum = (serialNum << 8) + getNVMemResponse.serialNumber.lsbyteMswordSerialCounter;
					serialNum = (serialNum << 8) + getNVMemResponse.serialNumber.msbyteLswordSerialCounter;
					serialNum = (serialNum << 8) + getNVMemResponse.serialNumber.lsbyteLswordSerialCounter;
					printf("LabQuest serial number(yy ww nnnnnnnn) is %02x %02x %08d \n", 
						(gtype_uint16) getNVMemResponse.serialNumber.yy, 
						(gtype_uint16) getNVMemResponse.serialNumber.ww, serialNum);
				}

				if (0 == status)
				{
					NGIOSetSensorChannelEnableMaskParams maskParams;
					NGIOSetAnalogInputParams analogInputParams;
					unsigned char sensorId = 0;
					unsigned char channelMask = NGIO_CHANNEL_MASK_ANALOG1;
					gtype_uint32 sig;
					memset(&maskParams, 0, sizeof(maskParams));
					for (channel = NGIO_CHANNEL_ID_ANALOG1; channel <= NGIO_CHANNEL_ID_ANALOG4; channel++)
					{
						NGIO_Device_DDSMem_GetSensorNumber(g_hDevice, channel, &sensorId, 1, &sig, NGIO_TIMEOUT_MS_DEFAULT);
						if (sensorId != 0)
						{
							maskParams.lsbyteLsword_EnableSensorChannels = maskParams.lsbyteLsword_EnableSensorChannels | channelMask;
							if (sensorId >= kSensorIdNumber_FirstSmartSensor)
								NGIO_Device_DDSMem_ReadRecord(g_hDevice, channel, 0, NGIO_TIMEOUT_MS_READ_DDSMEMBLOCK);

							if (kProbeTypeAnalog10V == NGIO_Device_GetProbeType(g_hDevice, channel))
								analogInputParams.analogInput = NGIO_ANALOG_INPUT_PM10V_BUILTIN_12BIT_ADC;
							else
								analogInputParams.analogInput = NGIO_ANALOG_INPUT_5V_BUILTIN_12BIT_ADC;
							analogInputParams.channel = channel;
							NGIO_Device_SendCmdAndGetResponse(g_hDevice, NGIO_CMD_ID_SET_ANALOG_INPUT, &analogInputParams, 
								sizeof(analogInputParams), NULL, NULL, NGIO_TIMEOUT_MS_DEFAULT);
						}
						channelMask = channelMask << 1;
					}

					if (0 == maskParams.lsbyteLsword_EnableSensorChannels)
						printf("No analog sensors found.\n");
					else
					{
						NGIO_Device_SendCmdAndGetResponse(g_hDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &maskParams, 
							sizeof(maskParams), NULL, NULL, NGIO_TIMEOUT_MS_DEFAULT);

						NGIO_Device_SetMeasurementPeriod(g_hDevice, -1, 0.010, NGIO_TIMEOUT_MS_DEFAULT);//10 milliseconds => 100 hz.

						NGIO_Device_SendCmdAndGetResponse(g_hDevice, NGIO_CMD_ID_START_MEASUREMENTS, NULL, 0, NULL, NULL, NGIO_TIMEOUT_MS_DEFAULT);

						OSSleep(1000); //Wait 1 second.

						for (channel = NGIO_CHANNEL_ID_ANALOG1; channel <= NGIO_CHANNEL_ID_ANALOG4; channel++)
						{
							NGIO_Device_DDSMem_GetSensorNumber(g_hDevice, channel, &sensorId, 0, &sig, 0);
							if (sensorId != 0)
							{
								char longname[30];
								longname[0] = 0;
								printf("Sensor id in channel ANALOG%d = %d", channel, sensorId);
								NGIO_Device_DDSMem_GetLongName(g_hDevice, channel, longname, sizeof(longname));
								if (strlen(longname) != 0)
									printf("(%s)", longname);

								int probeType = NGIO_Device_GetProbeType(g_hDevice, channel);
								numMeasurements = NGIO_Device_ReadRawMeasurements(g_hDevice, channel, rawMeasurements, NULL, MAX_NUM_MEASUREMENTS);
								if (numMeasurements > 0)
								{
									averageCalbMeasurement = 0.0;
									for (i = 0; i < numMeasurements; i++)
									{
										volts[i] = NGIO_Device_ConvertToVoltage(g_hDevice, channel, rawMeasurements[i], probeType);
										calbMeasurements[i] = NGIO_Device_CalibrateData(g_hDevice, channel, volts[i]);
										averageCalbMeasurement += calbMeasurements[i];
									}
									if (numMeasurements > 1)
										averageCalbMeasurement = averageCalbMeasurement/numMeasurements;

									gtype_real32 a, b, c;
									unsigned char activeCalPage = 0;
									NGIO_Device_DDSMem_GetActiveCalPage(g_hDevice, channel, &activeCalPage);
									NGIO_Device_DDSMem_GetCalPage(g_hDevice, channel, activeCalPage, &a, &b, &c, units, sizeof(units));

									printf("; average of %d measurements = %8.3f %s .", numMeasurements, averageCalbMeasurement, units);
								}
								printf("\n");
							} //if (sensorId != 0)
						}
					}
				}

				NGIO_Device_Close(g_hDevice);
				g_hDevice = NULL;
			}
		}

		NGIO_Uninit(g_hNGIOlib);
	}
	g_hNGIOlib = NULL;

	return 0;
}

void OSSleep(
	unsigned int msToSleep)//milliseconds
{
#ifdef TARGET_OS_WIN
	::Sleep(msToSleep);
#endif
#ifdef TARGET_OS_LINUX
  struct timeval tv;
  unsigned int usToSleep = msToSleep*1000;
  tv.tv_sec = usToSleep/1000000;
  tv.tv_usec = usToSleep % 1000000;
  select (0, NULL, NULL, NULL, &tv);
#endif
#ifdef TARGET_OS_MAC
	AbsoluteTime absTime = ::AddDurationToAbsolute(msToSleep * durationMillisecond, ::UpTime());
	::MPDelayUntil(&absTime);
#endif
}
