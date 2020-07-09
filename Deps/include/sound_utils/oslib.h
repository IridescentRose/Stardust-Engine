#ifndef _OSLIB_H_
#define _OSLIB_H_


#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
//#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <pspctrl.h>
#include <psputility.h>
#include <pspgu.h>
#include <pspgum.h>
#include <psppower.h>
#include <pspiofilemgr.h>
#include <psphprm.h>

#define oslMin(x, y)				(((x)<(y))?(x):(y))
#define oslMax(x, y)				(((x)>(y))?(x):(y))




#include "VirtualFile.h"
#include "audio.h"
#include "bgm.h"

#endif
