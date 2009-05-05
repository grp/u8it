/*****************************************************************************/
/*  Laichzeit                                                                */
/*  Part of the Benzin Project                                               */
/*  Copyright (c)2009 HACKERCHANNEL. Protected under the GNU GPLv2.          */
/* ------------------------------------------------------------------------- */
/*  types.h                                                                  */
/*  Typedefs for stuff. Keep this!                                           */
/*****************************************************************************/

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned long			u32;
typedef unsigned long long		u64;

typedef signed char			s8;
typedef signed short			s16;
typedef signed long			s32;
typedef signed long long		s64;

typedef float				f32;
typedef double				f64;


typedef volatile unsigned char		vu8;
typedef volatile unsigned short		vu16;
typedef volatile unsigned long		vu32;
typedef volatile unsigned long long	vu64;

typedef volatile signed char		vs8;
typedef volatile signed short		vs16;
typedef volatile signed long		vs32;
typedef volatile signed long long	vs64;

typedef volatile float			vf32;
typedef volatile double			vf64;


// Switch these if you're on a big endian machine.
#define LENDIAN
//#define BENDIAN

#define be16(x)	((u16)(						\
		((((u16)(x)) >> 8) & 0x00FF) |			\
		((((u16)(x)) << 8) & 0xFF00)			\
		))

#define be32(x)	((u32)(						\
		((((u32)(x)) << 24) & 0xFF000000) |		\
		((((u32)(x)) <<  8) & 0x00FF0000) |		\
		((((u32)(x)) >>  8) & 0x0000FF00) |		\
		((((u32)(x)) >> 24) & 0x000000FF)		\
		))

#define be64(x)	((u64)(						\
		((((u64)(x)) << 56) & 0xFF00000000000000LL) |	\
		((((u64)(x)) << 40) & 0x00FF000000000000LL) |	\
		((((u64)(x)) << 24) & 0x0000FF0000000000LL) |	\
		((((u64)(x)) <<  8) & 0x000000FF00000000LL) |	\
		((((u64)(x)) >>  8) & 0x00000000FF000000LL) |	\
		((((u64)(x)) >> 24) & 0x0000000000FF0000LL) |	\
		((((u64)(x)) >> 40) & 0x000000000000FF00LL) |	\
		((((u64)(x)) >> 56) & 0x00000000000000FFLL)	\
		))

#ifdef LENDIAN
#define htobs(x)	be16(x)
#define htobl(x)	be32(x)
#define htobll(x)	be64(x)
#endif //LENDIAN

#ifdef BENDIAN
#define htobs		(x)
#define htobl		(x)
#define htobll		(x)
#endif //BENDIAN

#define btohs		htobs
#define btohl		htobl
#define btohll		htobll

// Makes x a multiple of y, ceiling style.
#define align(x, y) ((x) + ((x) % (y)))

#endif //TYPEDEFS_H
