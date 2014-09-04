/**************************************************************************
*
*             eCS RexxUtil Library Replacement Project
*
*  Contains helper functions functions:
*       SysOS2Ver
*       SysECSVer      // new
*       SysVersion
*       SysUtilVersion
*
*  Michael Greene, December 2007
*
------------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/******************************************************************************/

#define INCL_DOSMISC

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <alloca.h>

#include <rexxdefs.h>  // rexxsaa.h include in this header
#include <version.h>

#pragma pack(push,1)

typedef struct _SYSLEVELDATA {
   unsigned char d_reserved1[2];
   unsigned char d_kind;
   unsigned char d_version[2];
   unsigned char d_reserved2[2];
   unsigned char d_clevel[7];
   unsigned char d_reserved3;
   unsigned char d_plevel[7];
   unsigned char d_reserved4;
   unsigned char d_title[80];
   unsigned char d_cid[9];
   unsigned char d_revision;
   unsigned char d_type[1];
} SYSLEVELDATA;

typedef struct _SYSLEVELHEADER {
   unsigned char h_magic[2];
   unsigned char h_name[9];
   unsigned char h_reserved1[4];
   unsigned char h_updated;
   unsigned char h_reserved2[17];
   unsigned long h_data;
} SYSLEVELHEADER;

#pragma pack(pop)

RexxFunctionHandler SysOS2Ver;
RexxFunctionHandler SysECSVer;
RexxFunctionHandler SysVersion;
RexxFunctionHandler SysUtilVersion;


/*************************************************************************
* Function:  SysOS2Ver                                                   *
*                                                                        *
* Syntax:    call SysOS2Ver                                              *
*                                                                        *
* Return:    eCS-OS/2 Version                                            *
*************************************************************************/

unsigned long SysOS2Ver(unsigned char *name,
                           unsigned long numargs,
                           RXSTRING args[],
                           char *queuename,
                           RXSTRING *retstr)
{
    unsigned long Versions[2];                  /* Major version number       */

#ifdef DLOGGING
    logmessage(__func__);
#endif

    if (numargs != 0) return INVALID_ROUTINE;

    DosQuerySysInfo(QSV_VERSION_MAJOR, QSV_VERSION_MINOR,
                        Versions, sizeof(Versions));

    retstr->strlength = sprintf(retstr->strptr,
                                "OS/2 %lu.%02lu",
                                Versions[0]/10, Versions[1]);
    return VALID_ROUTINE;
}


/*************************************************************************
* Function:  SysECSVer                                                   *
*                                                                        *
* Syntax:    call SysOS2Ver                                              *
*                                                                        *
* Return:    eCS-OS/2 Version                                            *
*************************************************************************/

unsigned long SysECSVer(unsigned char *name,
                           unsigned long numargs,
                           RXSTRING args[],
                           char *queuename,
                           RXSTRING *retstr)
{
    int handle;

    unsigned long BootDrive[1];
    unsigned long Versions[2];

    char lvlpath[_MAX_PATH] = {0};
    char *buffer = NULL;

    SYSLEVELHEADER *lvlheader = NULL;
    SYSLEVELDATA   *lvldata   = NULL;

#ifdef DLOGGING
    logmessage(__func__);
#endif

    DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, BootDrive, sizeof(BootDrive));

    sprintf(lvlpath, "%c:\\OS2\\INSTALL\\SYSLEVEL.ECS", (char)(BootDrive[0]+64));

    if (access(lvlpath, F_OK) == -1) {
        return SysOS2Ver(name, numargs, args, queuename, retstr);
    }

    buffer = alloca(filelength(handle));

    handle = open(lvlpath, O_RDONLY | O_BINARY);
    read( handle, buffer, filelength(handle));
    close(handle);

    lvlheader = (SYSLEVELHEADER *)buffer;
    lvldata = (SYSLEVELDATA *)&(buffer[lvlheader->h_data]);

    Versions[0] = lvldata->d_version[0] >> 4;
    Versions[1] = lvldata->d_version[0] & 15;

    retstr->strlength = sprintf(retstr->strptr, "eCS %lu.%02lu", Versions[0], Versions[1]);

    return VALID_ROUTINE;
}


/*************************************************************************
* Function:  SysVersion                                                  *
*                                                                        *
* Syntax:    Say  SysVersion                                             *
*                                                                        *
* Return:    Operating System and Version                                *
*                                                                        *
* NOTE: if called with a second arg return the eCS version               *
*************************************************************************/

unsigned long SysVersion(unsigned char *name,
                           unsigned long numargs,
                           RXSTRING args[],
                           char *queuename,
                           RXSTRING *retstr)
{
#ifdef DLOGGING
    logmessage(__func__);
#endif

    if (numargs == 1 && !stricmp(args[0].strptr, "ECS")) {
        return SysECSVer(name, numargs, args, queuename, retstr);
    } else {
        return SysOS2Ver(name, numargs, args, queuename, retstr);
    }
}


/*************************************************************************
* Function:  SysUtilVersion                                              *
*                                                                        *
* Syntax:    Say  SysUtilVersion                                         *
*                                                                        *
* Return:    RexxUtil Library Version                                    *
*************************************************************************/

unsigned long SysUtilVersion(unsigned char *name,
                           unsigned long numargs,
                           RXSTRING args[],
                           char *queuename,
                           RXSTRING *retstr)
{

#ifdef DLOGGING
    logmessage(__func__);
#endif

    if (numargs != 0) return INVALID_ROUTINE;

    retstr->strlength = sprintf(retstr->strptr,
                               "%s.%s",
                                LIBVERSION_MAJOR,
                                LIBVERSION_MINOR );

    return VALID_ROUTINE;
}

