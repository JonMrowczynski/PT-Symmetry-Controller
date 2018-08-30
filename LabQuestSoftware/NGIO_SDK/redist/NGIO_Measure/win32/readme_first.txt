Please run copy_win32_wdapi_dll.exe once from this folder before the first time you run NGIO_Measure.exe.

This will put the correct 32 bit version of wdapi921.dll in the folder.

Wdapi921.dll comes in 3 flavors:
wdapi921_WIN32forOS32.dll - used by 32 bit apps when running in a 32 bit operating system.
wdapi921_WIN32forOS64.dll - used by 32 bit apps when running in a 64 bit operating system.
wdapi921_WIN64forOS64.dll - used by 64 bit apps when running in a 64 bit operating system.

When you install NGIO_lib.dll on your system, you must copy the correct version of wdapi921_WINmmforOSnn.dll into the same folder where NGIO_lib.dll resides, and rename wdapi921_WINmmforOSnn.dll to wdapi921.dll.

To simplify installation of 32 bit applications, we have placed a very basic installation program in the redist folders called copy_win32_wdapi_dll.exe. This program assumes that wdapi921_WIN32forOS32.dll and wdapi921_WIN32forOS64.dll are located in the same folder as copy_win32_wdapi_dll.exe. When copy_win32_wdapi_dll.exe runs, it checks to see if it is running in a 32 bit operating system, or a 64 bit operating system. If the current OS is 32 bit, then the program copies wdapi921_WIN32forOS32.dll into wdapi921.dll in the local folder. If the current OS is 64 bit, then the program copies wdapi921_WIN32forOS64.dll into wdapi921.dll in the local folder.