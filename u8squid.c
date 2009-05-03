#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;


#define le32 be32
#define le16 be16

u16 be16(u16 x)
{
    return (x>>8) |
   (x<<8);
}

u32 be32(u32 x)
{
    return (x>>24) |
   ((x<<8) & 0x00FF0000) |
   ((x>>8) & 0x0000FF00) |
   (x<<24);
}


#define pad(x, y) ((x) + ((y) - ((x) % (y))))

typedef struct
{
    u16 type; //this is 0x0000 for files and 0x0100 for folders
    u16 string_offset; //offset into the string table from the start of the string table
    u32 data_offset; // absolute offset from U.8- header but it is recursion for directories
    u32 size; // last included file num for directories
} u8node;


typedef struct
{
    u32 tag; // 0x55AA382D "U.8-"
    u32 rootnode_offset; // offset to root_node, always 0x20.
    u32 header_size; // size of header from root_node to end of string table.
    u32 data_offset; // offset to data -- this is rootnode_offset + header_size, aligned to 0x40.
    u8 zeroes[16]; //padding
} u8header;

typedef struct
{
    u8header header;
    u8node rootnode;
    u32 numnodes;
    u8node * nodes;
    u32 stringsz;
    u8 * string_table;
    u32 datasz;
    u8 * data;
} u8archive;


typedef struct
{
	char	src[256];	// Filename.
	char	dst[256];	// Filename.
	u32     size;
	u8 *    data;
} U8File;

typedef struct ud
{
	char	   name[256];	// Name.
	int	   filecnt;	// How many files.
	U8File *	   files;	// The files.
	int	   dircnt;	// How many subdirs.
	struct ud * dirs;	// The dirs.
} U8Dir;

typedef struct
{
	U8Dir	rootdir;	// The root dir.
} U8;


void U8_packdir(u8archive * arc, U8Dir dir, u32 recursion)
{
    u32 cnt, i, oldnumnodes;
    U8File curfile;
    U8Dir curdir;
    u8node curnode;

    oldnumnodes = arc->numnodes;

    arc->nodes = realloc(arc->nodes, arc->numnodes * sizeof(u8node));
    curnode = arc->nodes[arc->numnodes];

    curnode.type = be32(0x0100); //folder

    curnode.string_offset = be32(arc->stringsz);
    arc->string_table = realloc(arc->string_table, strlen(curdir.name) + 1);
    memcpy(arc->string_table, curdir.name, strlen(curdir.name) + 1);
    arc->stringsz += strlen(curdir.name) + 1;
    arc->numnodes++;

    for(i = 0; i < dir.filecnt; i++) //files
    {
        curfile = dir.files[i];

        arc->nodes = realloc(arc->nodes, arc->numnodes * sizeof(u8node));
        curnode = arc->nodes[arc->numnodes];

        curnode.type = 0; //file
        curnode.size = be32(curfile.size);

        curnode.string_offset = be32(arc->stringsz);
        arc->string_table = realloc(arc->string_table, strlen(curfile.dst) + 1);
        memcpy(arc->string_table, curfile.dst, strlen(curfile.dst) + 1);
        arc->stringsz += strlen(curfile.dst) + 1;

        curnode.data_offset = be32(arc->datasz);
        arc->data = realloc(arc->data, pad(curfile.size, 32));
        memcpy(arc->data, curfile.data, curfile.size);
        arc->datasz += pad(curfile.size, 32);

        arc->numnodes++;
    }

    for(i = 0; i < dir.dircnt; i++) //folders
    {
        curdir = dir.dirs[i];
        U8_packdir(arc, curdir, ++recursion);
    }

    arc->nodes[oldnumnodes].size = be32(arc->numnodes + 1); //add one for root node

    return;
}


u8 * U8_pack(U8 input)
{
    u8archive arc;
    u32 offset = 0, data_offset, i;
    u8 * outbuf;

    arc.string_table = calloc(1, 1); //Initial NULL
    arc.stringsz = 1; //Initial NULL
    arc.data = NULL;
    arc.nodes = NULL;

    U8_packdir(&arc, input.rootdir, 0);

    arc.rootnode.type = be16(0x0100);
    arc.rootnode.string_offset = 0;
    arc.rootnode.data_offset = 0;
    arc.rootnode.size = be32(arc.numnodes);

    arc.header.tag = be32(0x55AA382D);
    arc.header.rootnode_offset = be32(0x20);
    arc.header.header_size = be32((sizeof(u8node) * arc.numnodes) + arc.stringsz);
    data_offset = pad(0x20 + (sizeof(u8node) * arc.numnodes) + arc.stringsz, 0x40);
    arc.header.data_offset = be32(data_offset);
    memset(arc.header.zeroes, 0, 16);

    for(i = 0; i < arc.numnodes; i++) //fix the data offset
        if (le16(arc.nodes[i].type) == 0) //Checking if it is a file
             arc.nodes[i].data_offset = be32(le32(arc.nodes[i].data_offset) + data_offset);

    outbuf = calloc(1, sizeof(u8header) + sizeof(u8node) + (sizeof(u8node) * arc.numnodes) + arc.stringsz + arc.datasz);

    memcpy(outbuf + offset, &arc.header, sizeof(u8header));
        offset += sizeof(u8header);
    memcpy(outbuf + offset, &arc.rootnode, sizeof(u8node));
        offset += sizeof(u8node);
    memcpy(outbuf + offset, arc.nodes, sizeof(u8node) * arc.numnodes);
        offset += sizeof(u8node) * arc.numnodes;
    memcpy(outbuf + offset, arc.string_table, arc.stringsz);
        offset += arc.stringsz;
    memcpy(outbuf + offset, arc.data, arc.datasz);
        offset += arc.datasz;

    return outbuf;
}
