/* 
	Author: Jon Mrowczynski

	This file is compiled as a DLL so that the native functions can be called from Java.
*/

#include <stdio.h>
#include <string.h>
#include <memory.h>

#ifdef TARGET_OS_WIN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#pragma warning(disable: 4996)
#include <windows.h>
#endif
#ifdef TARGET_OS_MAC
#include <Carbon/Carbon.h>
#endif
#ifdef TARGET_OS_LINUX
#include <sys/time.h>
#endif

#include <jni.h> // Include the java native interface (jni) header file since we are using native code.
#include "LQPhotoGates.h"
#include "ptsymmetrycontroller_hardware_LabQuest2.h" // Include the header file created using javac -h.
#include "NGIO_lib_interface.h"

static constexpr int maxNumMeasurements = 200;

/******************************** Native Functions *********************************************/

/* 
	This function calls all of the helper functions. Returns false if any of the helper functions 
	experience any errors, true otherwise.
*/

JNIEXPORT jboolean JNICALL Java_PTSymmetryController_initAll(JNIEnv * env, jobject jobj) {
	libraryHandle = NGIO_Init();
	if (libraryHandle && findLabQuest2() && openLabQuest2() && getExclusiveOwnership() && getPhotogates()) {
		printf("\nInitialization Successful!\n\n");
		return true;
	} else {
		printf("\nInitialization was unsuccessful!\n\n");
		return false;
	}
}

// Polls the photogate on the given channel. Returns 1 if it is blocked, returns 0 if it is not blocked.

static int pollPhotogate(int channel) {
	gtype_int32 numMeasurements = 10;
	gtype_int32 rawMeasurements[maxNumMeasurements];

	numMeasurements = NGIO_Device_ReadRawMeasurements(labQuest2Handle, channel, rawMeasurements, NULL, maxNumMeasurements);
	return numMeasurements > 0 ? rawMeasurements[0] : -1;
}

// Poll the current state of photogate 1. Returns 1 if it is blocked, returns 0 if it is not blocked.

JNIEXPORT jint JNICALL Java_PTSymmetryController_pollPhotogate1 (JNIEnv * env, jobject jobj) {
	return pollPhotogate(NGIO_CHANNEL_ID_DIGITAL1);
}

// Poll the current state of photogate 2. Returns 1 if it is blocked, returns 0 if it is not blocked.

JNIEXPORT jint JNICALL Java_PTSymmetryController_pollPhotogate2 (JNIEnv * env, jobject jobj) {
	return pollPhotogate(NGIO_CHANNEL_ID_DIGITAL2);
}

// Closes and cleans everything up.

JNIEXPORT void JNICALL Java_PTSymmetryController_closeAll (JNIEnv * env, jobject jobj) {
	NGIO_Device_Close(labQuest2Handle);
	labQuest2Handle = NULL;
	printf("g_hDevice was closed.\n");
	NGIO_Uninit(libraryHandle);
	libraryHandle = NULL;
	printf("g_hNGIOlib was closed.\n");
}

void OSSleep(unsigned int msToSleep) {
#ifdef TARGET_OS_WIN
	::Sleep(msToSleep);
#endif
#ifdef TARGET_OS_LINUX
	struct timeval tv;
	unsigned int usToSleep = msToSleep * 1000;
	tv.tv_sec = usToSleep / 1000000;
	tv.tv_usec = usToSleep % 1000000;
	select(0, NULL, NULL, NULL, &tv);
#endif
#ifdef TARGET_OS_MAC
	AbsoluteTime absTime = ::AddDurationToAbsolute(msToSleep * durationMillisecond, ::UpTime());
	::MPDelayUntil(&absTime);
#endif
}

bool findLabQuest2(void) {
	gtype_uint32 deviceType, numDevices, mask;
	
	#ifdef TARGET_OS_WIN
		OSSleep(500); //Give Jungo device driver time to find the LabQuest 2.
	#endif
	
	deviceType = NGIO_DEVTYPE_LABQUEST2;
	gtype_uint32 sig;
	NGIO_SearchForDevices(libraryHandle, deviceType, NGIO_COMM_TRANSPORT_USB, NULL, &sig);

	NGIO_DEVICE_LIST_HANDLE hDeviceList = NGIO_OpenDeviceListSnapshot(libraryHandle, deviceType, &numDevices, &sig);
	gtype_int32 status = NGIO_DeviceListSnapshot_GetNthEntry(hDeviceList, 0, deviceName, sizeof(deviceName), &mask);
	NGIO_CloseDeviceListSnapshot(hDeviceList);
	
	if (status == 0) { 
		printf("LabQuest2 Device Found!\n"); 
		return true;
	}
	else { 
		printf("***NGIO_DeviceCheck cannot find a LabQuest2.\n"); 
		return false;
	}
}

bool openLabQuest2(void) {
	labQuest2Handle = NGIO_Device_Open(libraryHandle, deviceName, 0);
	if (!labQuest2Handle) { printf("***Failed to open LabQuest2 device!\n"); }
	else {
		printf("Successfully opened LabQuest2 device!\n");
		if(NGIO_Device_Unlock(labQuest2Handle) == 0) {
			printf("The LabQuest has been successfully unlocked for multithreading!\n");
			return true;
		}
	}
	return false;
}

bool getExclusiveOwnership(void) {
	NGIOGetStatusCmdResponsePayload getStatusResponse;
	gtype_uint32 nRespBytes;
	NGIO_NVMEM_CHANNEL_ID1_rec getNVMemResponse;

	gtype_int32 status = NGIO_Device_AcquireExclusiveOwnership(labQuest2Handle, NGIO_GRAB_DAQ_TIMEOUT);
	if (0 != status) {
		printf("***NGIO_Device_AcquireExclusiveOwnership() failed!\n");
		return false;
	} else {
		printf("Acquired exclusive ownership!\n");
		memset(&getStatusResponse, 0, sizeof(getStatusResponse));
		nRespBytes = sizeof(getStatusResponse);
		status = NGIO_Device_SendCmdAndGetResponse(labQuest2Handle, NGIO_CMD_ID_GET_STATUS, NULL, 0, &getStatusResponse,
			&nRespBytes, NGIO_TIMEOUT_MS_DEFAULT);
		if (status != 0) { return false; }
		else {
			memset(&getNVMemResponse, 0, sizeof(getNVMemResponse));
			status = NGIO_Device_NVMemBlk_Read(labQuest2Handle, NGIO_NVMEM_CHANNEL_ID1, &getNVMemResponse, 0,
				sizeof(getNVMemResponse) - 1, NGIO_TIMEOUT_MS_DEFAULT);
			return status == 0;
		}
	}
}

bool getPhotogates() {
	signed char channel = NGIO_CHANNEL_ID_DIGITAL1;
	unsigned char sensorId = 0;
	unsigned char channelMask = NGIO_CHANNEL_MASK_DIGITAL1;
	NGIOSetSensorChannelEnableMaskParams maskParams;
	gtype_uint32 sig;
	memset(&maskParams, 0, sizeof(maskParams));
	
	for (channel = NGIO_CHANNEL_ID_DIGITAL1; channel <= NGIO_CHANNEL_ID_DIGITAL2; channel++) {
		NGIO_Device_DDSMem_GetSensorNumber(labQuest2Handle, channel, &sensorId, 1, &sig, NGIO_TIMEOUT_MS_DEFAULT);
		printf("Channel ID: %i Sensor ID: %i\n",channel,sensorId);
		if (sensorId != 0) { maskParams.lsbyteLsword_EnableSensorChannels = maskParams.lsbyteLsword_EnableSensorChannels | channelMask; }
		channelMask = channelMask << 1;
	}
	
	if (0 == maskParams.lsbyteLsword_EnableSensorChannels){
		printf("***No photogates found.\n");
		return false;
	} else if (maskParams.lsbyteLsword_EnableSensorChannels > 64) {
		NGIO_Device_SendCmdAndGetResponse(labQuest2Handle, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &maskParams,
										  sizeof(maskParams), NULL, NULL, NGIO_TIMEOUT_MS_DEFAULT);
		NGIO_Device_SetMeasurementPeriod(labQuest2Handle, -1, NGIO_Device_GetMinimumMeasurementPeriod(labQuest2Handle, -1), NGIO_TIMEOUT_MS_DEFAULT);
		NGIO_Device_SendCmdAndGetResponse(labQuest2Handle, NGIO_CMD_ID_START_MEASUREMENTS, NULL, 0, NULL, NULL, NGIO_TIMEOUT_MS_DEFAULT);

		OSSleep(200); //Wait 0.2 seconds

		for (channel = NGIO_CHANNEL_ID_DIGITAL1; channel <= NGIO_CHANNEL_ID_DIGITAL2; ++channel)
		{
			NGIO_Device_DDSMem_GetSensorNumber(labQuest2Handle, channel, &sensorId, 0, &sig, 0);
			if (sensorId != 0)
			{
				char longname[30];
				longname[0] = 0;
				printf("Sensor id in channel DIGITAL%d = %d", channel, sensorId);
				NGIO_Device_DDSMem_GetLongName(labQuest2Handle, channel, longname, sizeof(longname));
				if (strlen(longname) != 0)
					printf(" (%s)", longname);
			} 
			printf("\n");
		}
		return true;
	} else {
		printf("***Did not acquire both photogates.");
		return false;
	}
}