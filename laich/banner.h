/*****************************************************************************/
/*  Laichzeit                                                                */
/*  Part of the Benzin Project                                               */
/*  Copyright (c)2009 HACKERCHANNEL. Protected under the GNU GPLv2.          */
/* ------------------------------------------------------------------------- */
/*  banner.h                                                                 */
/*  The header file for all the final banner stuff. Include this!            */
/* ------------------------------------------------------------------------- */
/*  Compilation code contributed by:                                         */
/*          icefire                                                          */
/*  Intermediation code contributed by:                                      */
/*          SquidMan                                                         */
/*****************************************************************************/

#ifndef BANNER_H
#define BANNER_H

#include "types.h"

#define USE_BANNER

typedef struct
{
	u8	zeroes[128];	// padding
	char	imet[4];	// `IMET'
	u64	unk;		// 0x0000060000000003 fixed, unknown purpose
	u32	sizes[3];	// icon.bin, banner.bin, sound.bin
	u32	flag1;		// ???
	u16	names[7][42];	// JP, EN, DE, FR, ES, IT, NL
	u8	zeroes_2[840];	// Padding
	u8	crypto[16];	// MD5 of 0x40 to 0x640 in header. 
				// Should be all 0's when calculating MD5.
} IMET;

typedef struct
{
	char	imd5[4];	// `IMD5'
	u32	filesize;	// Size of rest of file
	u8	zeroes[8];	// Padding
	u8	crypto[16];	// MD5 of rest of file
} IMD5;

typedef struct
{
	char	src[256];	// Filename.
	char	dst[256];	// Filename.
	u32	size;		// Size of the data.
	u8*	data;		// The data.
} U8File;

typedef struct ud
{
	char	   name[256];	// Name.
	int	   filecnt;	// How many files.
	U8File*	   files;	// The files.
	int	   dircnt;	// How many subdirs.
	struct ud* dirs;	// The dirs.
} U8Dir;

typedef struct
{
	U8Dir	rootdir;	// The root dir.
} U8;

IMET*  LaichMeta_IMET(char** names, \
		      u32 iconsize, u32 bannersize, u32 soundsize);
IMD5*  LaichMeta_IMD5(u8* file, u32 size);
U8*    LaichMeta_StartU8(U8Dir* root);
U8Dir* LaichMeta_CreateDirInDirU8(U8Dir* arc, char* name);
void   LaichMeta_CreateFileInDirU8(U8Dir* arc, char* src, char* dst);
u8*    LaichMeta_CompileU8(U8* input, u32* size);

#endif //BANNER_H
