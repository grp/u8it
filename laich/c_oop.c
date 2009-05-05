/*****************************************************************************/
/*  Laichzeit                                                                */
/*  Part of the Benzin Project                                               */
/*  Copyright (c)2009 HACKERCHANNEL. Protected under the GNU GPLv2.          */
/* ------------------------------------------------------------------------- */
/*  c_oop.c                                                                  */
/*  Object orientation shit for C. Compile this!                             */
/* ------------------------------------------------------------------------- */
/*  Code contributed by:                                                     */
/*          SquidMan                                                         */
/*****************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "c_oop.h"

int AddObjectToList(u8** listv, u32* listcnt, u8* object, u32 size)
{
	*listcnt++;
	u8* list = *(u8**)listv;
	u8* entries = calloc(size, *listcnt);
	if(list != NULL) {
		memcpy(entries, list, size * (*listcnt - 1));
		free(list);
	}
	memcpy(&entries[(*listcnt - 1) * size], object, size);
	*(u8**)listv = entries;
	return *listcnt - 1;
}

