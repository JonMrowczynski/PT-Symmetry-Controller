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
#ifndef _NGIO_FGEN_SOURCE_CMDS_H_
#define _NGIO_FGEN_SOURCE_CMDS_H_

#define NGI_EPSILON 1e-09
#define NGI_ZERO    1e-10

#define FLOAT_IS_ZERO(__f_) (fabs (__f_) < NGI_EPSILON)
#define FLOAT_IS_EQUAL(__f_,__v_) (fabs ((__f_) - (__v_)) < NGI_EPSILON)
#define FLOAT_IS_LE(__f_,__v_) ( (__f_) < (__v_) || FLOAT_IS_EQUAL (__f_,__v_) )
#define FLOAT_IS_GE(__f_,__v_) ( (__f_) > (__v_) || FLOAT_IS_EQUAL (__f_,__v_) )

// commands:
#define NGIO_FGEN_CMD_ID_SET_CHANNEL_PARAMS	0x40
#define NGIO_FGEN_CMD_ID_GET_CHANNEL_PARAMS	0x41
#define NGIO_FGEN_CMD_ID_START_FGEN		0x42
#define NGIO_FGEN_CMD_ID_PAUSE_FGEN		0x43
#define NGIO_FGEN_CMD_ID_RESUME_FGEN		0x44
#define NGIO_FGEN_CMD_ID_STOP_FGEN		0x45

// channels:
#define NGIO_FGEN_CHANNEL_ID_LEFT	0
#define NGIO_FGEN_CHANNEL_ID_RIGHT	1
#define NGIO_FGEN_MAX_NUM_CHANNELS	2

// waveforms:
#define NGIO_FGEN_FUNCTION_SINE		0
#define NGIO_FGEN_FUNCTION_SQUARE	1
#define NGIO_FGEN_FUNCTION_TRIANGLE	2
#define NGIO_FGEN_FUNCTION_RAMPUP	3
#define NGIO_FGEN_FUNCTION_RAMPDN	4
#define NGIO_FGEN_FUNCTION_PWM		5
#define NGIO_FGEN_FUNCTION_NONE		6


#define NGIO_FGEN_FREQ_MIN      0.25
#define NGIO_FGEN_FREQ_MAX  10000.0

// FGEN-specific values for NGIODefaultCmdResponse {unsigned char status;}
#define FGEN_BAD_COMMAND		201	// the command code is wrong
#define FGEN_BAD_COMMAND_PARAMS		202	// one or more params are wrong
#define FGEN_COMMAND_REJECTED		203	// command illegal in sequence or FTL not ready for this command


#if defined (TARGET_OS_WIN)
#  pragma pack(push)
#  pragma pack(1)
#endif

#ifdef TARGET_OS_MAC
#  pragma pack(1)
#endif

// This is unfortunate, but gcc 3.x does not support pragma pack(gcc 4.x does!).
// We are stuck with gcc 3.x for now, so we use _XPACK1 .
// Note that some docs on the web mentioned problems with using typedefs and
// __attribute__, so we are putting the typedef on a separate line.
#ifndef _XPACK1
#  ifdef TARGET_OS_LINUX
#    define _XPACK1 __attribute__((__packed__))
#  elif defined(TARGET_OS_DAQ)
#    ifndef _X86
#      define _XPACK1 __attribute__((packed))
#    else
#      define _XPACK1
#    endif
#  else
#    define _XPACK1
#  endif
#endif

#ifndef GCHAR
#  define GCHAR char
#endif


// The structures below define the parameter and response blocks associated with the commands defined above.
// Pointers to these structures are passed in to SendCmdAndGetResponse().
// If no parameter structure is defined for a command, then the command does not take parameters.
// If no response structure is defined for a command, then the only response associated with the command is NGIODefaultCmdResponse.


/***************************************************************************************************/
// NGIO_FGEN_CMD_ID_SET_FGEN_CHANNEL_PARAMS:
// NGIO_FGEN_CMD_ID_GET_FGEN_CHANNEL_PARAMS:

/* Parameter block passed into SendCmd() with NGIO_FGEN_CMD_ID_SET_FGEN_CHANNEL_PARAMS.	*/
struct tagNGIOSetFgenChannelParams
{
	unsigned char Channel;			/* L or R (enum) */

	unsigned char Function;			/* sine, square, triangle, ramp_up, ramp_dn, pwm, none (enum) */

	unsigned char lsbyteLswordPhase;	/* starting phase offset in degrees [0..359] (signed int) */
	unsigned char msbyteLswordPhase;	/* (-1 = continue phase from current value) */
	unsigned char lsbyteMswordPhase;
	unsigned char msbyteMswordPhase;

	unsigned char Gain;			/* gain in percent [0..100] (unsigned int) */

	unsigned char lsbyteLswordFreq;		/* frequency in 1/1000 Hz (unsigned int) */
	unsigned char msbyteLswordFreq;
	unsigned char lsbyteMswordFreq;
	unsigned char msbyteMswordFreq;

	signed char   Offset;			/* pwm: offset voltage in ticks [-50..+50] (int) */
						/* sine, square, triangle, ramp_up, ramp_dn, none: not used */
} _XPACK1;
typedef struct tagNGIOSetFgenChannelParams NGIOSetFgenChannelParams;

/* Parameter block passed into SendCmd() with NGIO_FGEN_CMD_ID_GET_FGEN_CHANNEL_PARAMS.	*/
struct tagNGIOGetFgenChannelParams
{
	unsigned char channel;
} _XPACK1;
typedef struct tagNGIOGetFgenChannelParams NGIOGetFgenChannelParams;

/* Response payload returned by GetNextResponse() after sending NGIO_FGEN_CMD_ID_GET_FGEN_CHANNEL_PARAMS. */
typedef NGIOSetFgenChannelParams NGIOGetFgenChannelParamsCmdResponsePayload;

/***************************************************************************************************/

#if defined (TARGET_OS_WIN)
#  pragma pack(pop)
#endif

#ifdef TARGET_OS_MAC
#  pragma pack()
#endif

#endif

