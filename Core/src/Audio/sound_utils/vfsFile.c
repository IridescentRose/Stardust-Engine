#include <Audio/sound_utils/oslib.h>

/*
	SOURCE VFS: file
*/

#define FLAG_EOF 1

int VF_FILE = -1;

#define _file_			((SceUID)f->ioPtr)

int vfsFileOpen(void *param1, int param2, int type, int mode, VIRTUAL_FILE* f)			{
	int stdMode = PSP_O_RDONLY;
	if (mode == VF_O_WRITE)
		stdMode = PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC;
	else if (mode == VF_O_READWRITE)
		stdMode = PSP_O_RDWR;
	
	f->ioPtr = (void*)sceIoOpen((char*)param1, stdMode, 0777);
	return (s32)f->ioPtr >= 0;
}

int vfsFileClose(VIRTUAL_FILE *f)				{
	sceIoClose(_file_);
	return 1;
}

int vfsFileWrite(const void *ptr, size_t size, size_t n, VIRTUAL_FILE* f)			{
	return sceIoWrite(_file_, ptr, size * n);
}

int vfsFileRead(void *ptr, size_t size, size_t n, VIRTUAL_FILE* f)			{
	int readSize = sceIoRead(_file_, ptr, size * n);
	if (readSize < size * n)
		f->userData |= FLAG_EOF;
	return readSize;
}

int vfsFileGetc(VIRTUAL_FILE *f)		{
/*	unsigned char car = -1;
	sceIoRead(_file_, &car, sizeof(car));
	return (int)car;*/
	return vfsMemGetc(f);
}

int vfsFilePutc(int caractere, VIRTUAL_FILE *f)		{
/*	unsigned char car = (unsigned char)caractere;
	return sceIoWrite(_file_, &car, sizeof(car));*/
	return vfsMemPutc(caractere, f);
}

char *vfsFileGets(char *str, int maxLen, VIRTUAL_FILE *f)			{
	return vfsMemGets(str, maxLen, f);
}

void vfsFilePuts(const char *s, VIRTUAL_FILE *f)		{
	return vfsMemPuts(s, f);
}

void vfsFileSeek(VIRTUAL_FILE *f, int offset, int whence)		{
//	int oldOffset = sceIoLseek32(_file_, 0, SEEK_CUR);
//	if (!(offset == 0 && whence == SEEK_CUR))
//		sceIoLseek32(_file_, offset, whence);
	sceIoLseek32(_file_, offset, whence);
	f->userData &= ~FLAG_EOF;
}

int vfsFileTell(VIRTUAL_FILE *f)		{
	return sceIoLseek32(_file_, 0, SEEK_CUR);
}

int vfsFileEof(VIRTUAL_FILE *f)		{
	return f->userData & FLAG_EOF;
}

VIRTUAL_FILE_SOURCE vfsFile =		{
	vfsFileOpen,
	vfsFileClose,
	vfsFileRead,
	vfsFileWrite,
	vfsFileGetc,
	vfsFilePutc,
	vfsFileGets,
	vfsFilePuts,
	vfsFileSeek,
	vfsFileTell,
	vfsFileEof,
};

int oslInitVfsFile()		{
   VF_FILE = VirtualFileRegisterSource(&vfsFile);
   return VF_FILE;
}


