

#pragma once

#include <stdio.h>
#include "NGIO_lib_interface.h"

// Handle to NGIO library.

NGIO_LIBRARY_HANDLE ngioLibraryHandle = NULL;

// Handle to LabQuest2

NGIO_DEVICE_HANDLE labQuest2Handle = NULL;

// The name of the acquired device. In our case, the name of the LabQuest 2.

char deviceName[NGIO_MAX_SIZE_DEVICE_NAME];

// Pauses the native thread for the specified duration. Works on Windows, Mac, and Linux.

void OSSleep(unsigned int msToSleep);

/*************************************** Helper Functions ****************************************
			If any of these functions return false, then the program should not run.
*************************************************************************************************/

// Returns true if there is a LabQuest 2 connected to the computer, false otherwise.

bool findLabQuest2(void);

/*
	Returns true if the LabQuest 2 has been opened, false otherwise.

	Note that when NGIO_Device_Open() is called from a thread, the acquired device is locked
	so that only that thread can access it. In order for multiple threads to access the device, 
	it has to be unlocked by calling NGIO_Device_Unlock() which decrements the lock count by 1 
	(calling NGIO_Device_Open() puts the lock count at 1.). Once the lock count is 0, the device 
	has been unlocked for multithreaded usage. 
*/

bool openLabQuest2(void);

// Returns true if ownership was taken away from the GUI on the LabQuest2 and given to the computer instead, false otherwise.

bool getExclusiveOwnership(void);

// Returns true if both photogates are connected to the LabQuest2 and acquired by the computer, false otherwise.

bool getPhotogates(void);