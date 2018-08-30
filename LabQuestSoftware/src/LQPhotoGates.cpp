/* 
	Author: Jon Mrowczynski

	THIS FILE IS ONLY COMPILED AS A DLL WHICH IS NECESSARY TO CALL THE NATIVE FUNCTIONS
	FROM JAVA. 
	To compile this as a dll simply type the command:
		g++ -o LQPhotoGates.dll -shared -I"Path\To\Java\jdk1.8.0\include" 
		-I"Path\To\NGIO_SDK\redist\include" LQPhotoGates.cpp NGIO_lib.dll
	at the command prompt using mingw64's g++ or similar compiler. 
	Note that you will need the following dlls:
		-NGIO_lib.dll
		-wdapi921.dll
	to successfully compile this into a dll which can be copied and pasted into the current 
	directory from "Path\To\NGIO_SDK\redist\NGIO_lib\win64". Also note that I have made a
	batch file called "make" allowing one to simply enter 'make' at the command prompt
	so that this file compiles as a dll without you having to type the above command every time
	after you made a change to the file.
*/

#include "PTSymmetryController.h" // Include the header file created using javah.
#include <jni.h> // Also need to include the java native interface (jni) header file since we are using native code.
#include <stdio.h>
#include <memory.h>

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#pragma warning(disable: 4996)
#include <windows.h>
#ifdef TARGET_OS_MAC
#include <Carbon/Carbon.h>
#endif
#ifdef TARGET_OS_LINUX
#include <sys/time.h>
#endif

#include "NGIO_lib_interface.h"

#define MAX_NUM_MEASUREMENTS 200

// Prototypes for the helper function used by the native functions called from Java.
int findLabQuest2(void);
NGIO_DEVICE_HANDLE openLabQuest2(void);
int getExclusiveOwnership(void);
bool getPhotogates(void);

// Pauses the thread for a specified amount of time. Works on Windows, Mac and Linux.
void OSSleep(unsigned int msToSleep);

// Global variables. --> No side effects should be experienced unless one tries to make a
// really weird alteration to the code.
NGIO_LIBRARY_HANDLE g_hNGIOlib = NULL;
NGIO_DEVICE_HANDLE g_hDevice = NULL;
gtype_int32 status = 0;
char deviceName[NGIO_MAX_SIZE_DEVICE_NAME];
gtype_uint32 sig;
NGIO_DEVICE_LIST_HANDLE hDeviceList;

/******************************** Native Functions *********************************************/
/* 
	This is the one function that calls all of the helper functions. If any of the helper 
	functions experience any errors, the will return a value that prevents the if statement 
	evaluating to true.
*/
JNIEXPORT jboolean JNICALL Java_PTSymmetryController_initAll (JNIEnv * env, jobject jobj){
	g_hNGIOlib = NGIO_Init();
	if (g_hNGIOlib && findLabQuest2() && openLabQuest2() && getExclusiveOwnership() && getPhotogates()) {
		printf("\nInitialization Successful!\n\n");
		return true;
	} else {
		printf("\nInitialization was unsuccessful!\n\n");
		return false;
	}
}
/*
	If the photogate experiences a state change, then return the value of the state change.
	For example, if the photogate becomes blocked, then return 1, if the photogate becomes 
	unblocked then return 0. Return -1 otherwise which is normally due to some initialization 
	error.
*/
JNIEXPORT jint JNICALL Java_PTSymmetryController_pollPhotogate1 (JNIEnv * env, jobject jobj){
	gtype_int32 numMeasurements = 10;
	gtype_int32 rawMeasurements[MAX_NUM_MEASUREMENTS];

	numMeasurements = NGIO_Device_ReadRawMeasurements(g_hDevice, NGIO_CHANNEL_ID_DIGITAL1, rawMeasurements, NULL, MAX_NUM_MEASUREMENTS);
	if (numMeasurements > 0) {
		return rawMeasurements[0];
	} else{
		return -1;
	}
}

// Same thing as above except for the other photogate.
JNIEXPORT jint JNICALL Java_PTSymmetryController_pollPhotogate2 (JNIEnv * env, jobject jobj){
	gtype_int32 numMeasurements = 10;
	gtype_int32 rawMeasurements[MAX_NUM_MEASUREMENTS];
	
	numMeasurements = NGIO_Device_ReadRawMeasurements(g_hDevice, NGIO_CHANNEL_ID_DIGITAL2, rawMeasurements, NULL, MAX_NUM_MEASUREMENTS);
	if (numMeasurements > 0) {
		return rawMeasurements[0];
	} else
		return -1;
}

/*
	Cleans everything up.
*/
JNIEXPORT void JNICALL Java_PTSymmetryController_closeAll (JNIEnv * env, jobject jobj){
	NGIO_Device_Close(g_hDevice);
	g_hDevice = NULL;
	printf("g_hDevice was closed.\n");
	NGIO_Uninit(g_hNGIOlib);
	g_hNGIOlib = NULL;
	printf("g_hNGIOlib was closed.\n");
}

/*************************************** Helper Functions 
	Note that if any of these functions fail, then the program should not run. If any one of 
	these do fail however, modifications can be made such that these functions can help with 
	debugging. Refer to NGIO_lib_interface.h for said functions.
****************************************/

int findLabQuest2(void){
	gtype_uint32 deviceType, numDevices, mask;
	
	#ifdef TARGET_OS_WIN
		OSSleep(500); //Give Jungo device driver time to find the LabQuest 2.
	#endif
	
	deviceType = NGIO_DEVTYPE_LABQUEST2;
	NGIO_SearchForDevices(g_hNGIOlib, deviceType, NGIO_COMM_TRANSPORT_USB, NULL, &sig);

	hDeviceList = NGIO_OpenDeviceListSnapshot(g_hNGIOlib, deviceType, &numDevices, &sig);
	status = NGIO_DeviceListSnapshot_GetNthEntry(hDeviceList, 0, deviceName, sizeof(deviceName), &mask);
	NGIO_CloseDeviceListSnapshot(hDeviceList);
	
	if (status != 0)
		printf("***NGIO_DeviceCheck cannot find a LabQuest2.\n");
	else
		printf("LabQuest2 Device Found!\n");
	
	// NGIO_DeviceListSnapshot_GetNthEntry()returns 0 on success, so we want to return the 
	// negation of status so that the if statement evaluates to true.
	return !status;
}

/*
	Note that when NGIO_Device_Open() is called from a thread, this function locks the device
	so that only one thread is allowed to access the device. In order for multiple thread to 
	access the device, you have to unlock it by calling NGIO_Device_Unlock() which decrements
	the lock count by 1. Once the lock count is 0, the device has been unlocked for multithreaded
	usage. Note that calling NGIO_Device_Open() puts the lock count at 1.
*/
NGIO_DEVICE_HANDLE openLabQuest2(void){
	g_hDevice = NGIO_Device_Open(g_hNGIOlib, deviceName, 0);
	if (!g_hDevice)
		printf("***Failed to open LabQuest2 device!\n");
	else {
		printf("Successfully opened LabQuest2 device!\n");
		if(NGIO_Device_Unlock(g_hDevice) == 0) {
			printf("The LabQuest has been successfully unlocked for multithreading!\n");
			return g_hDevice;
		}
	}
	return 0;
}

/*
	Takes ownership away from the GUI on the LabQuest2 and gives it to the computer instead.
*/
int getExclusiveOwnership(void){
	NGIOGetStatusCmdResponsePayload getStatusResponse;
	gtype_uint32 nRespBytes;
	NGIO_NVMEM_CHANNEL_ID1_rec getNVMemResponse;

	status = NGIO_Device_AcquireExclusiveOwnership(g_hDevice, NGIO_GRAB_DAQ_TIMEOUT);
	if (0 != status) {
		printf("***NGIO_Device_AcquireExclusiveOwnership() failed!\n");
		return -1;
	} else {
		printf("Acquired exclusive ownership!\n");
		memset(&getStatusResponse, 0, sizeof(getStatusResponse));
		nRespBytes = sizeof(getStatusResponse);
		status = NGIO_Device_SendCmdAndGetResponse(g_hDevice, NGIO_CMD_ID_GET_STATUS, NULL, 0, &getStatusResponse,
			&nRespBytes, NGIO_TIMEOUT_MS_DEFAULT);
		if(status != 0)
			return -1;
		else
			memset(&getNVMemResponse, 0, sizeof(getNVMemResponse));
			status = NGIO_Device_NVMemBlk_Read(g_hDevice, NGIO_NVMEM_CHANNEL_ID1, &getNVMemResponse, 0,
				sizeof(getNVMemResponse) - 1, NGIO_TIMEOUT_MS_DEFAULT);
			if(status != 0)
				return -1;
			else 
				return !status;
	}
}
/*
	Gets both of the photogates, however if two are not found then this function returns false.
	You need both of the photogates in order to get the periods of the pendulums and to monitor 
	their movement so that you can pulse the electromagnets accordingly.
*/
bool getPhotogates(){
	signed char channel = NGIO_CHANNEL_ID_DIGITAL1;
	unsigned char sensorId = 0;
	unsigned char channelMask = NGIO_CHANNEL_MASK_DIGITAL1;
	NGIOSetSensorChannelEnableMaskParams maskParams;
	gtype_uint32 sig;
	gtype_int32 rawMeasurements[MAX_NUM_MEASUREMENTS];
	gtype_int32 numMeasurements;
	memset(&maskParams, 0, sizeof(maskParams));
	
	for (channel = NGIO_CHANNEL_ID_DIGITAL1; channel <= NGIO_CHANNEL_ID_DIGITAL2; channel++) {
		NGIO_Device_DDSMem_GetSensorNumber(g_hDevice, channel, &sensorId, 1, &sig, NGIO_TIMEOUT_MS_DEFAULT);
		printf("Channel ID: %i Sensor ID: %i\n",channel,sensorId);
		if (sensorId != 0) {
			maskParams.lsbyteLsword_EnableSensorChannels = maskParams.lsbyteLsword_EnableSensorChannels | channelMask;
		}
		channelMask = channelMask << 1;
	}
	
	if (0 == maskParams.lsbyteLsword_EnableSensorChannels){
		printf("***No photogates found.\n");
		return false;
	} else if (maskParams.lsbyteLsword_EnableSensorChannels > 64) {
		NGIO_Device_SendCmdAndGetResponse(g_hDevice, NGIO_CMD_ID_SET_SENSOR_CHANNEL_ENABLE_MASK, &maskParams,
										  sizeof(maskParams), NULL, NULL, NGIO_TIMEOUT_MS_DEFAULT);
		NGIO_Device_SetMeasurementPeriod(g_hDevice, -1, NGIO_Device_GetMinimumMeasurementPeriod(g_hDevice, -1), NGIO_TIMEOUT_MS_DEFAULT);
		NGIO_Device_SendCmdAndGetResponse(g_hDevice, NGIO_CMD_ID_START_MEASUREMENTS, NULL, 0, NULL, NULL, NGIO_TIMEOUT_MS_DEFAULT);

		OSSleep(200); //Wait 0.2 seconds

		for (channel = NGIO_CHANNEL_ID_DIGITAL1; channel <= NGIO_CHANNEL_ID_DIGITAL2; channel++)
		{
			NGIO_Device_DDSMem_GetSensorNumber(g_hDevice, channel, &sensorId, 0, &sig, 0);
			if (sensorId != 0)
			{
				char longname[30];
				longname[0] = 0;
				printf("Sensor id in channel DIGITAL%d = %d", channel, sensorId);
				NGIO_Device_DDSMem_GetLongName(g_hDevice, channel, longname, sizeof(longname));
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

/*********************************** Sleep Function ********************************************/

/*  Pause the native thread for the specified duration. This function will work on the operating
	systems listed below.
*/
void OSSleep(unsigned int msToSleep) {
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