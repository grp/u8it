#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "banner.h"


int main()
{
	U8Dir root;
	U8Dir stuff;
	U8 * arc;	
	u8 * dst;
	u32 sz;
	FILE * file;
	
	arc = LaichMeta_StartU8(&root);
	printf("created\n");
	
	stuff = *(LaichMeta_CreateDirInDirU8(&root, "arc"));
	printf("create dir\n");
	LaichMeta_CreateFileInDirU8(stuff, "testfile", "banner.bin");
	printf("created file\n");
	dst = LaichMeta_CompileU8(arc, &sz);
	printf("compiled");
	
	file = fopen("out.u8", "wb");
	fwrite(dst, 1, sz, file);
	fclose(file);
	
	free(dst);
	
	printf("\nDone!\n");
	
	return 0;
}

