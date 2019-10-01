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
#include "LabQuest2Photogates.h"
#include "ptsymmetrycontroller_hardware_LabQuest2.h" // Include the header file created using javac -h.
#include "NGIO_lib_interface.h"

static constexpr int maxNumMeasurements = 200;

// Prints to the Java console without having to wait for the termination of the program.

static void nativePrintf(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	fflush(stdout);
	va_end(args);
}

/******************************** Native Functions *********************************************/

/* 
	This function calls all of the helper functions. Returns false if any of the helper functions 
	experience any errors, true otherwise.
*/

JNIEXPORT jboolean JNICALL Java_ptsymmetrycontroller_hardware_LabQuest2_initLabQuest2AndPhotogates(JNIEnv * env, jobject jobj) {
	ngioLibraryHandle = NGIO_Init();
	if (ngioLibraryHandle && findLabQuest2() && openLabQuest2() && getExclusiveOwnership() && getPhotogates()) {
		nativePrintf("Initialization successful!\n\n");
		return true;
	} else {
		nativePrintf("Initialization was unsuccessful.\n\n");
		return false;
	}
}

// Closes and cleans everything up.

JNIEXPORT void JNICALL Java_ptsymmetrycontroller_hardware_LabQuest2_closeLabQuest2AndPhotogates(JNIEnv* env, jobject jobj) {
	NGIO_Device_Close(labQuest2Handle);
	labQuest2Handle = NULL;
	nativePrintf("Closed labQuest2Handle.\n");
	NGIO_Uninit(ngioLibraryHandle);
	ngioLibraryHandle = NULL;
	nativePrintf("Closed ngioLibraryHandle.\n");
}

// Polls the photogate on the given channel. Returns 1 if it is blocked, 0 if it is not blocked, or -1 if an error occured.

static int pollPhotogate(int channel) {
	gtype_int32 numMeasurements = 10;
	gtype_int32 rawMeasurements[maxNumMeasurements];

	numMeasurements = NGIO_Device_ReadRawMeasurements(labQuest2Handle, channel, rawMeasurements, NULL, maxNumMeasurements);
	return numMeasurements > 0 ? rawMeasurements[0] : -1;
}

/*
	Poll the current state of the photogate associated with the driving solenoid. 
	Returns 1 if it is blocked, 0 if it is not blocked, or -1 if an error occured.
*/

JNIEXPORT jint JNICALL Java_ptsymmetrycontroller_hardware_LabQuest2_pollDrivingPhotogate(JNIEnv * env, jobject jobj) {
	return pollPhotogate(NGIO_CHANNEL_ID_DIGITAL1);
}

/* 
	Poll the current state of the photogate associated with the dampening solenoid. 
	Returns 1 if it is blocked, 0 if it is not blocked, or -1 if an error occured.
*/

JNIEXPORT jint JNICALL Java_ptsymmetrycontroller_hardware_LabQuest2_pollDampeningPhotogate(JNIEnv * env, jobject jobj) {
	return pollPhotogate(NGIO_CHANNEL_ID_DIGITAL2);
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
	NGIO_SearchForDevices(ngioLibraryHandle, deviceType, NGIO_COMM_TRANSPORT_USB, NULL, &sig);

	NGIO_DEVICE_LIST_HANDLE hDeviceList = NGIO_OpenDeviceListSnapshot(ngioLibraryHandle, deviceType, &numDevices, &sig);
	gtype_int32 status = NGIO_DeviceListSnapshot_GetNthEntry(hDeviceList, 0, deviceName, sizeof(deviceName), &mask);
	NGIO_CloseDeviceListSnapshot(hDeviceList);
	
	if (status == 0) { 
		nativePrintf("Found LabQuest 2.\n");
		return true;
	}
	else { 
		nativePrintf("***Could not find LabQuest 2.\n");
		return false;
	}
}

bool openLabQuest2(void) {
	labQuest2Handle = NGIO_Device_Open(ngioLibraryHandle, deviceName, 0);
	if (!labQuest2Handle) { nativePrintf("***Failed to open LabQuest 2.\n"); }
	else {
		nativePrintf("Successfully opened LabQuest2.\n");
		if(NGIO_Device_Unlock(labQuest2Handle) == 0) {
			nativePrintf("The LabQuest 2 has been successfully unlocked for multithreading!\n");
			return true;
		}
	}
	return false;
}

bool getExclusiveOwnership(void) {
	NGIOGetStatusCmdResponsePayload getStatusResponse;
	gtype_uint32 nRespBytes;
	NGIO_NVMEM_CHANNEL_ID1_rec getNVMemResponse;

	nativePrintf("Acquiring exclusive ownership of LabQuest 2...\n");
	gtype_int32 status = NGIO_Device_AcquireExclusiveOwnership(labQuest2Handle, NGIO_GRAB_DAQ_TIMEOUT);
	if (0 != status) {
		nativePrintf("***Could not acquire exclusive ownership of LabQuest 2.\n");
		return false;
	} else {
		nativePrintf("Acquired exclusive ownership of LabQuest 2.\n");
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
		if (sensorId != 0) { maskParams.lsbyteLsword_EnableSensorChannels = maskParams.lsbyteLsword_EnableSensorChannels | channelMask; }
		channelMask = channelMask << 1;
	}
	
	if (0 == maskParams.lsbyteLsword_EnableSensorChannels){
		nativePrintf("***No photogates found.\n");
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
				nativePrintf("Sensor id in channel DIGITAL%d = %d", channel - 4, sensorId);
				NGIO_Device_DDSMem_GetLongName(labQuest2Handle, channel, longname, sizeof(longname));
				if (strlen(longname) != 0) { nativePrintf(" (%s)", longname); }
			} 
			nativePrintf("\n");
		}
		return true;
	} else {
		nativePrintf("***Did not acquire both photogates.");
		return false;
	}
}