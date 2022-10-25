#ifndef PATCH_XCOFF_H
#define PATCH_XCOFF_H

#define __XCOFF64__
#define __64BIT__
#include  <filehdr.h>
#include <scnhdr.h>
#include <loader.h>

#ifdef _AIX
#define be64toh(num) num
#define be32toh(num) num
#define htobe64(num) num
#define htobe32(num) num
#define be16toh(num) num
#endif 

#endif 