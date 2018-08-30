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
#ifndef _NGIO_FTL_SOURCE_CMDS_H_
#define _NGIO_FTL_SOURCE_CMDS_H_

// File related commands
#define NGIO_FTL_CMD_ID_GET_FOLDER_CONTENTS 0xA4
#define NGIO_FTL_CMD_ID_OPEN_FILE 0xA5
#define NGIO_FTL_CMD_ID_READ_FILE 0xA6
#define NGIO_FTL_CMD_ID_WRITE_FILE 0xA7
#define NGIO_FTL_CMD_ID_CLOSE_FILE 0xA8
#define NGIO_FTL_CMD_ID_DELETE_FOLDER_OR_FILE 0xA9
#define NGIO_FTL_CMD_ID_CREATE_FOLDER 0xAA

// Misc. commands
#define NGIO_FTL_CMD_ID_SET_TIME 0xB0
#define NGIO_FTL_CMD_ID_EXECUTE 0xB1
#define NGIO_FTL_CMD_ID_GET_EXECUTE_RESULTS 0xB2
#define NGIO_FTL_CMD_ID_CANCEL_EXECUTE 0xB3

#define NGIO_FTL_MAX_PATH 256 // Max size (in GCHAR) of paths in FTL
#define NGIO_FTL_MAX_RETURN_BUFFER_SIZE 4096 // Max size (in bytes) of return buffers

#define NGIO_FTL_OPEN_FILE_READ 0x01 // Flags used when opening files
#define NGIO_FTL_OPEN_FILE_WRITE 0x02

// For the bitfield charcater that defines the file/folder state
#define NGIO_FTL_FOLDER_CONTENTS_HIDDEN 0x01 // file/folder is hidden if this bit is set (Unused.  Assume first character is '.' for hidden folders/files)
#define NGIO_FTL_FOLDER_CONTENTS_FILE 0x02
#define NGIO_FTL_FOLDER_CONTENTS_FOLDER 0x04
#define NGIO_FTL_FOLDER_CONTENTS_OTHER 0x08

// File/Folder permissions -- these are bits for a permissions flag in the open/create structs
#define NGIO_FTL_FILE_PERMISSIONS_READ 0x10
#define NGIO_FTL_FILE_PERMISSIONS_WRITE 0x20
#define NGIO_FTL_FILE_PERMISSIONS_EXECUTE 0x40 // when applied to folders, this means "Browse"
#define NGIO_FTL_FILE_PERMISSIONS_ALL (NGIO_FTL_FILE_PERMISSIONS_READ | NGIO_FTL_FILE_PERMISSIONS_WRITE | NGIO_FTL_FILE_PERMISSIONS_EXECUTE)

#define TIME_ZONE_NAME_LENGTH 16 // 3 characters should take care of it, but make it larger just in case

// FTL-specific values for NGIODefaultCmdResponse{unsigned char status;}
#define FTL_BAD_COMMAND			201	// the command code is wrong
#define FTL_BAD_COMMAND_PARAMS		202	// one or more params are wrong
#define FTL_COMMAND_REJECTED		203	// command illegal in sequence or FTL not ready for this command

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

#ifndef GCHAR
#define GCHAR char
#endif

// The structures below define the parameter and response blocks associated with the commands defined above.
// Pointers to these structures are passed in to SendCmdAndGetResponse().
// If no parameter structure is defined for a command, then the command does not take parameters.
// If no response structure is defined for a command, then the only response associated with the command is NGIODefaultCmdResponse.

/////////////////////////////////////////////////////////
// Get Folder Contents

struct tagNGIOFTLGetFolderContentsParams
{
	GCHAR sFolderName[NGIO_FTL_MAX_PATH]; /* Name of folder to get contents of */
	unsigned char lsbyteLswordFolderContentsOffset;	// Offset (as GCHAR) into larger array.  0 the first time, only non-zero if folder contents are larger than response packet size and this is being called a subsequent time
	unsigned char msbyteLswordFolderContentsOffset;	
	unsigned char lsbyteMswordFolderContentsOffset;	
	unsigned char msbyteMswordFolderContentsOffset;	
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_GET_FODLER_CONTENTS	*/
typedef struct tagNGIOFTLGetFolderContentsParams NGIOFTLGetFolderContentsParams;

#define NGIOFTLGetFolderContentsCmdResponsePayload_HeaderSize 4 // So we can subtract this and get the array
struct tagNGIOFTLGetFolderContentsCmdResponsePayload
{
	unsigned char lsbyteLswordFolderContentsSize;	// Total remaining size (in GCHAR) of bytes to read (not including what in this struct) -- if > 0 caller needs to make repeated calls to get remaining bytes
	unsigned char msbyteLswordFolderContentsSize;	
	unsigned char lsbyteMswordFolderContentsSize;	
	unsigned char msbyteMswordFolderContentsSize;	
    GCHAR sArray[NGIO_FTL_MAX_RETURN_BUFFER_SIZE / sizeof(GCHAR)]; // The first character of each entry is a single GCHAR: NGIO_FTL_FOLDER_CHAR ==> subfolder, NGIO_FTL_FILE_CHAR ==> a file.  The entry name then follows, ending with a null.  
} _XPACK1; //This is the response payload returned by GetNextResponse() after sending NGIO_FTL_CMD_ID_GET_FODLER_CONTENTS.
typedef struct tagNGIOFTLGetFolderContentsCmdResponsePayload NGIOFTLGetFolderContentsCmdResponsePayload;

/////////////////////////////////////////////////////////
// Open File

struct tagNGIOFTLOpenFileParams
{
	GCHAR sFileName[NGIO_FTL_MAX_PATH]; /* Fully Qualified path and name of file to open */
	unsigned char openFileFlags;	// read/write etc.  NGIO_FTL_OPEN_FILE_READ or NGIO_FTL_OPEN_FILE_WRITE
	unsigned char createFilePermissions; // bit field of read/write/execute bits (defined above).  Used only when the file is created.
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_OPEN_FILE */
typedef struct tagNGIOFTLOpenFileParams NGIOFTLOpenFileParams;

/////////////////////////////////////////////////////////
// Close File

/////////////////////////////////////////////////////////
// Read File

struct tagNGIOFTLReadFileParams
{
	unsigned char lsbyteLswordFileReadOffset;	// Offset into larger block of values to read.  Ignored for now.
	unsigned char msbyteLswordFileReadOffset;	
	unsigned char lsbyteMswordFileReadOffset;	
	unsigned char msbyteMswordFileReadOffset;	
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_READ_FILE	*/
typedef struct tagNGIOFTLReadFileParams NGIOFTLReadFileParams;

#define NGIOFTLReadFileResponsePayload_HeaderSize 4 // So we can subtract this and get the array
struct tagNGIOFTLReadFileResponsePayload
{
	unsigned char lsbyteLswordFileReadSize;	// Size of following array of bytes (we've reached the eof when this is 0)
	unsigned char msbyteLswordFileReadSize;	
	unsigned char lsbyteMswordFileReadSize;	
	unsigned char msbyteMswordFileReadSize;	
    unsigned char contents[NGIO_FTL_MAX_RETURN_BUFFER_SIZE]; // File Contents that have been read
} _XPACK1; //This is the response payload returned by GetNextResponse() after sending NGIO_FTL_CMD_ID_READ_FILE.
typedef struct tagNGIOFTLReadFileResponsePayload NGIOFTLReadFileResponsePayload;

/////////////////////////////////////////////////////////
// Write File

struct tagNGIOFTLWriteFileParams
{
	unsigned char lsbyteLswordFileWriteSize;	// Size of following array of bytes to write
	unsigned char msbyteLswordFileWriteSize;	
	unsigned char lsbyteMswordFileWriteSize;	
	unsigned char msbyteMswordFileWriteSize;	
    unsigned char contents[NGIO_FTL_MAX_RETURN_BUFFER_SIZE]; // File contents to write
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_WRITE_FILE	*/
typedef struct tagNGIOFTLWriteFileParams NGIOFTLWriteFileParams;

/////////////////////////////////////////////////////////
// Delete Folder or File

struct tagNGIOFTLDeleteFolderOrFileParams
{
	unsigned char bFolder; // true (non-zero) ==> folder; false ==> file
	GCHAR sFolderOrFileName[NGIO_FTL_MAX_PATH]; /* Fully qualified Name of folder or file to delete */
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_DELETE_FOLDER_OR_FILE	*/
typedef struct tagNGIOFTLDeleteFolderOrFileParams NGIOFTLDeleteFolderOrFileParams;

/////////////////////////////////////////////////////////
// Create Sub Folder 

struct tagNGIOFTLCreateFolderParams
{
	unsigned char createFolderPermissions; // bit field of read/write/browse bits (defined above)
	GCHAR sParentFolderName[NGIO_FTL_MAX_PATH]; /* Name of folder to create new sub folder in */
	GCHAR sSubFolderName[NGIO_FTL_MAX_PATH]; /* Name of new sub folder */
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_CREATE_FOLDER	*/
typedef struct tagNGIOFTLCreateFolderParams NGIOFTLCreateFolderParams;

/////////////////////////////////////////////////////////
// Set Time
struct tagNGIOFTLSetTimeParams
{
	unsigned char lsbyteLswordSecondsSince111970;	// Local time: Seconds since 1/1/70
	unsigned char msbyteLswordSecondsSince111970;	
	unsigned char lsbyteMswordSecondsSince111970;	
	unsigned char msbyteMswordSecondsSince111970;	
	unsigned char daylightSavings;	// 1 ==> daylight savings is in effect; 0 ==> standard time
	unsigned char lsbyteLswordSecondsDiffFromUTC;	// seconds difference from UTC (i.e. UTC - local time so PST is a positive number)
	unsigned char msbyteLswordSecondsDiffFromUTC;	// Also -- doen't take into account daylight savings time (above flag can be used to adjust)
	unsigned char lsbyteMswordSecondsDiffFromUTC;	
	unsigned char msbyteMswordSecondsDiffFromUTC;	
	unsigned char sTZName[TIME_ZONE_NAME_LENGTH]; // Name of the time zone (e.g. "PST")
	unsigned char sDaylightTZName[TIME_ZONE_NAME_LENGTH]; // Name of the daylight savings time zone (e.g. "PDT")
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_SET_TIME	*/
typedef struct tagNGIOFTLSetTimeParams NGIOFTLSetTimeParams;

// This is a new struct for specifying the timezone in a post 1.3 world.

#define STANDARD_TIMEZONE_LENGTH TIME_ZONE_NAME_LENGTH * 2
struct tagNGIOFTLSetTimeParams2
{
	unsigned char lsbyteLswordSecondsSince111970;	// Local time: Seconds since 1/1/70
	unsigned char msbyteLswordSecondsSince111970;	
	unsigned char lsbyteMswordSecondsSince111970;	
	unsigned char msbyteMswordSecondsSince111970;	
	unsigned char daylightSavings;					// This flag is deprecated. Posix-date can figure it out from the tz name and the seconds.
	unsigned char lsbyteLswordSecondsDiffFromUTC;	// seconds difference from UTC (i.e. UTC - local time so PST is a positive number)
	unsigned char msbyteLswordSecondsDiffFromUTC;	// Also -- doen't take into account daylight savings time (above flag can be used to adjust)
	unsigned char lsbyteMswordSecondsDiffFromUTC;	
	unsigned char msbyteMswordSecondsDiffFromUTC;	
	unsigned char sStandardTZName[STANDARD_TIMEZONE_LENGTH]; // Name of the time zone (e.g. "America/Los_Angeles")
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_SET_TIME	*/
typedef struct tagNGIOFTLSetTimeParams2 NGIOFTLSetTimeParams2;


/* 
** Execute (Asynchronous command to start the execution of an arbitrary command line command) 
*/
struct tagNGIOFTLExecuteFileParams
{
	GCHAR sFileName[NGIO_FTL_MAX_PATH];	/* Fully Qualified path and name of file to execute */
	GCHAR sParameters[NGIO_FTL_MAX_PATH];	/* list of parameters for command (string) */
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_EXECUTE	*/
typedef struct tagNGIOFTLExecuteFileParams NGIOFTLExecuteFileParams;

struct tagNGIOFTLExecuteFileResponse
{
	unsigned char id; /* Use this id with NGIO_FTL_CMD_ID_GET_EXECUTE_RESULTS to get the response */
} _XPACK1;	/* response block from NGIO_FTL_CMD_ID_EXECUTE */
typedef struct tagNGIOFTLExecuteFileResponse NGIOFTLExecuteFileResponse;

/* 
**  Get Execution results
*/
struct tagNGIOFTLGetExecuteResultsParams
{
	unsigned char id; /* id identifying which execute command we're interested in getting response for */
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_GET_EXECUTE_RESULTS	*/
typedef struct tagNGIOFTLGetExecuteResultsParams NGIOFTLGetExecuteResultsParams;

#define NGIOFTLGetExecuteResultsResponse_HeaderSize 6 /* So we can subtract this and get the array */
struct tagNGIOFTLGetExecuteResultsResponse
{
	unsigned char error;					/* 0 ==> no error, otherwise some sort of error code */
	unsigned char running;					/* 0 ==> command has finished executing;  1 ==> command is still running */
	unsigned char lsbyteLswordGetExecuteResultsSize;	/* Size of following array of bytes */
	unsigned char msbyteLswordGetExecuteResultsSize;	
	unsigned char lsbyteMswordGetExecuteResultsSize;	
	unsigned char msbyteMswordGetExecuteResultsSize;	
    unsigned char contents[NGIO_FTL_MAX_RETURN_BUFFER_SIZE];	/* Response contents */
} _XPACK1;	/* response block from NGIO_FTL_CMD_ID_GET_EXECUTE_RESULTS */
typedef struct tagNGIOFTLGetExecuteResultsResponse NGIOFTLGetExecuteResultsResponse;

/* 
**  Cancel Execution 
*/
struct tagNGIOFTLCancelExecuteFile
{
	unsigned char id; /* id returned by ExecuteFile -- cancel this execution */
} _XPACK1;	/* Parameter block passed into SendCmd() with NGIO_FTL_CMD_ID_CANCEL_EXECUTE */
typedef struct tagNGIOFTLCancelExecuteFile NGIOFTLCancelExecuteFile;



/***************************************************************************************************/

#if defined (TARGET_OS_WIN)
#pragma pack(pop)
#endif

#ifdef TARGET_OS_MAC
#pragma pack()
#endif


#endif 
