NGIO Software Development Kit

The NGIO SDK is intended to provide software developers with the ability to access Vernier Software & Technology LabQuest and LabQuest Mini devices from their own applications.

The heart of the SDK is the NGIO_lib library.
The API to this library is documented in \NGIO_SDK\redist\include\NGIO_lib_interface.h. The sequence of function calls into the library is best illustrated by the NGIO_DeviceCheck sample code.

The Windows version of this library is NGIO_lib.dll. Since NGIO_lib.dll is a standard Microsoft Windows DLL, you can access the library from a variety of languages including C, C++, LabView, and Java. We have also written a .NET wrapper for NGIO_lib.dll called NGIOdotNET.dll. NGIOdotNET.dll allows .NET based applications to access the LabQuest and the LabQuest Mini.

The Mac OS version of this SDK ships with several library variants:
libNGIO.dylib is 32-bit only, ppc and Intel, for Mac OS 10.4 and higher.
NGIO.framework is 32-bit only, ppc and Intel, for Mac OS 10.4 and higher, packaged as a framework.
libNGIOUniversal.dylib is 32-bit and 64-bit, Intel only, for Mac OS 10.5 and higher.
NGIOUniversal.framework is 32-bit and 64-bit, Intel only, for Mac OS 10.5 and higher, packaged as a framework.
libNGIO64.dylib is 64-bit only, Intel only, for Mac OS 10.5 and higher.

libNGIO has been ported to Linux, but it is packaged separately.

Run VernierInterfaceDrivers_100.exe to install the Windows USB device drivers for the LabQuest, LabQuest2, and the LabQuest Mini.

====================================================================================================================

The NGIO SDK includes partial source code. The sample applications include complete source, but the source code to the NGIO_lib library is not included.

The NGIO SDK is currently distributed with a very permissive BSD style license. See the license.txt file located in the same folder as this readme.txt file.

Basically we encourage everyone to use the SDK, and to freely redistribute the NGIO_lib library binaries. If the restrictions set out in the license.txt file discourage you from using the SDK, please contact VST at http://www.vernier.com/tech/supportform.html .

VST does not guarantee that the code is bug free, but we try to make it so. If you find any bugs, please report them to http://www.vernier.com/tech/supportform.html .

====================================================================================================================

NGIO_DeviceCheck is a very simple command line sample application that opens the first LabQuest or LabQuest Mini device that it finds, 
scans the analog channels for sensors, takes some measurements from the sensors it finds, and printf's the results to STDOUT. It is written in C++(really just C) and its source code runs on Windows, MacOSX, and Linux. Looking at the sample code in \src\NoIO_DeviceCheck\NGIO_DeviceCheck.cpp is the best place to start if you are new to NGIO. Even if you are programming in a different language, eg. VB.NET or Java, NGIO_DeviceCheck.cpp is probably the first thing that you should look at.

====================================================================================================================

NGIO_Measure is a sample application that is coded to the NGIO_lib API. The Windows version source code was written in Microsoft Visual C++ and built with Visual Studio 2005.

To run this application, you need to plug a LabQuest or LabQuest Mini into a USB port, click on the Devices menu, and then click on one of the listed devices. The app will only automatically find a sensor if it is plugged into the ANALOG1 channel. You can take measurements from other channels, but you need to use the channels menu to do so.

NGIO_Measure is only ported to windows at this time.

====================================================================================================================

NGIO_lib.dll version 1.68(and later) is dependent on wdapi921.dll. This is only used on Windows systems.

Wdapi921.dll comes in 3 flavors:
wdapi921_WIN32forOS32.dll - used by 32 bit apps when running in a 32 bit operating system.
wdapi921_WIN32forOS64.dll - used by 32 bit apps when running in a 64 bit operating system.
wdapi921_WIN64forOS64.dll - used by 64 bit apps when running in a 64 bit operating system.

When you install NGIO_lib.dll on your system, you must copy the correct version of wdapi921_WINmmforOSnn.dll into the same folder where NGIO_lib.dll resides, and rename wdapi921_WINmmforOSnn.dll to wdapi921.dll.

To simplify installation of 32 bit applications, we have placed a very basic installation program in the redist folders called copy_win32_wdapi_dll.exe. This program assumes that wdapi921_WIN32forOS32.dll and wdapi921_WIN32forOS64.dll are located in the same folder as copy_win32_wdapi_dll.exe. When copy_win32_wdapi_dll.exe runs, it checks to see if it is running in a 32 bit operating system, or a 64 bit operating system. If the current OS is 32 bit, then the program copies wdapi921_WIN32forOS32.dll into wdapi921.dll in the local folder. If the current OS is 64 bit, then the program copies wdapi921_WIN32forOS64.dll into wdapi921.dll in the local folder.

====================================================================================================================

NGIO_VB_Form_Measure.exe is a Visual Basic .NET application that behaves very similarly to the NGIO_Measure sample application. This application is coded to the managed code API presented by NGIOdotNET.dll and VSTCoreDefsdotNET.dll. These .NET dll's provide a thin wrapper around the unmanaged code in NGIO_lib.dll.

To run this application, you need to plug a LabQuest or LabQuest Mini into a USB port, click on the Devices menu, and then click on one of the listed devices. The app will only automatically find a sensor if it is plugged into the ANALOG1 channel. You can take measurements from other channels, but you need to use the channels menu to do so.

====================================================================================================================

.NET discussion:

NGIOdotNET.dll provides P/Invoke methods that access the public entry points to the unmanaged code in NGIO_lib.dll.
VSTCoreDefsdotNET.dll provides some additional structure and constant definitions needed to work with NGIO_lib. The API's presented by these dll's show up in the .NET object browser in the NGIOdotNET and  VSTCoreDefsdotNET namespaces.

The XML documentation files for the NGIOdotNET and VSTCoreDefsdotNET assemblies provide a fair amount of documentation for the NGIO library API's that is accessible in the Object browser, but they are not complete. The most complete reference to the NGIO library API is in NGIO_lib_interface.h and its associated header files.

The following files need to be distributed with .NET applications that reference NGIOdotNET:
NGIOdotNET.dll
VSTCoreDefsdotNET.dll
NGIO_lib.dll
wdapi921.dll

We generally recommend that you just place these files in the same folder as the .exe file for your application.

NGIOdotNET.dll and VSTCoreDefsdotNET.dll are pure .NET assemblies that contain no unmanaged code, so the same instance of each file is used by both 32 bit and 64 bit applications. NGIO_lib.dll and wdapi921.dll are unmanaged code(aka native), so there are separate binary files for 32 bit apps versus 64 bit apps. When you click on Properties->Version in the Windows explorer for NGIO_lib.dll, the description string is either "NGIO_lib DLL (x86)" or "NGIO_lib DLL (x64)". 

If you build your .NET app for the x86 platform, then you should distribute 32 bit versions of NGIO_lib.dll and wdapi921.dll with your app. If you build your .NET app for the x64 platform, then you should distribute 64 bit versions of NGIO_lib.dll and wdapi921.dll with your app. If you build your .NET for the 'Any CPU' platform, then the situation is more complex. Such apps run as 32 bit processes in 32 bit windows, and they run as 64 bit processes in 64 bit windows. This means your installer needs figure out what operating system is being used, and install matching versions of NGIO_lib and wdapi921.

For maximum simplicity and portability, we recommend that you just build your .NET app for the x86 platform. 

====================================================================================================================

Release notes:
Version 1.97
Fix bug in 64 bit Mac implementation of NGIO_Device_Close() that caused subsequent invocation of NGIO_Device_Open() to hang.

Version 1.95
Support LabQuest2.

Version 1.84
64 bit version of NGIO lib is working on the Mac now(Mac OS 10.5 and higher).

Version 1.78
Add sensor map data for non smart sensors so DDS records are descriptive for all known sensors.
NGIO_Device_CalibrateData() supports non linear calibrations now, so stainless steel temperature is supported.
Support the +/-30 volt sensor.

Version 1.75
Added support for .NET languages.
Added support for 64 bit applications.
NGIO_Measure updated and ported to Visual Studio 2005.
Added NGIO_VB_Form_Measure sample app which is written in Visual Basic .NET.
Added NGIO_DeviceCheck sample app which is written in C++.

Version 1.68
The NGIO_Measure sample app has been ported to Visual Studio 2008. This code is built as a 32 bit binary, but it runs on both 32 bit and 64 bit windows. The LabQuest Mini is also supported in this version.

Version 1.35
NGIO_lib is reasonably mature, but NGIO_SDK still needs more sample code. Only windows sample code is currently available(NGIO_Measure).
