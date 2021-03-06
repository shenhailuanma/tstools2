/* vim: set tabstop=8 shiftwidth=8: */
//=============================================================================
// Name: tscrc.c
// Purpose: analyse certain character with ts file
// To build: gcc -o tscrc.exe tscrc.c
// Copyright (C) 2010 by ZHOU Cheng. All right reserved.
//=============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "crc.h"

//=============================================================================
// enum & struct definition:
//=============================================================================

//=============================================================================
// Variables definition:
//=============================================================================
static uint8_t buf32[] =
{
        //00    01    02    03    04    05    06    07
#if 0
        0x00, 0xB0, 0x0D, 0x00, 0x01, 0xC1, 0x00, 0x00, // PAT
        0x00, 0x01, 0xE0, 0x30, 0xEE, 0xD2, 0xF2, 0x31,
        0x02, 0xB0, 0x17, 0x00, 0x01, 0xC1, 0x00, 0x00, // PMT bad
        0xE0, 0x31, 0xF0, 0x06, 0x05, 0x04, 0x47, 0x41,
        0x39, 0x34, 0x1B, 0xE0, 0x32, 0xF0, 0xC6, 0xC8, 0xDB, 0xE6,
#endif
        0x42, 0xB0, 0x1D, 0x00, 0x01, 0xC1, 0x00, 0x00, // SDT
        0x12, 0x34, 0xFF, 0x00, 0x01, 0x00, 0x80, 0x0C,
        0x48, 0x0A, 0x01, 0x03, 0x44, 0x56, 0x54, 0x04,
        0x45, 0x4E, 0x43, 0x31, 0x8B, 0x11, 0xE0, 0xF6
};

//=============================================================================
// Sub-function declare:
//=============================================================================

//=============================================================================
// The main function:
//=============================================================================
int main()
{
        crc_init();
        //printf("crc32 is 0x%08X\n", crc32(buf32, 16));
        //printf("CRC is 0x%08X\n", CRC(buf32, 16, 32));
        printf("size is %d\n", sizeof(buf32));
        printf("CRC for TS is 0x%08X\n", CRC_for_TS(buf32, sizeof(buf32), 32));
        exit(0);
}

//=============================================================================
// Subfunctions definition:
//=============================================================================

//=============================================================================
// THE END.
//=============================================================================
