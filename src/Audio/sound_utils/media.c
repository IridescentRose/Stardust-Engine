#include <Audio/sound_utils/oslib.h>
#include <pspaudiocodec.h>
#include <pspsdk.h>
#include <pspmpeg.h>

typedef struct AT3_INFO		{
	VIRTUAL_FILE *handle;
	unsigned long *codecBuffer;
	u8* dataBuffer;
	u32 sample_per_frame;
	u32 data_start_init;

	//Codec specific
	u16 at3_type;
	u16 at3_data_align;
	u16 at3_channel_mode;
	u8 at3_at3plus_flagdata[2];
	u32 at3_data_size;
	u8 at3_getEDRAM;
	u32 at3_channels;
	u32 at3_samplerate;
} AT3_INFO;

#define TYPE_ATRAC3 0x270
#define TYPE_ATRAC3PLUS 0xFFFE


typedef struct MP3_INFO		{
	VIRTUAL_FILE *handle;
	unsigned long *codecBuffer;
	//u8* dataBuffer;
	unsigned char dataBuffer[2889]__attribute__((aligned(64)));
	u32 sample_per_frame;
	u32 data_start_init;

	//Codec specific
	u32 channels;
	u32 samplerate;
	u32 data_start;
	u8 getEDRAM;
} MP3_INFO;


static int samplerates[4][3] =
{
    {11025, 12000, 8000,},//mpeg 2.5
    {0, 0, 0,}, //reserved
    {22050, 24000, 16000,},//mpeg 2
    {44100, 48000, 32000}//mpeg 1
};
static int bitrates[] = {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 };
static int bitrates_v2[] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 };

#define NUMBLOCKS 4

//Load a module:
SceUID LoadStartAudioModule(char *modname, int partition){
    SceKernelLMOption option;
    SceUID modid;

    memset(&option, 0, sizeof(option));
    option.size = sizeof(option);
    option.mpidtext = partition;
    option.mpiddata = partition;
    option.position = 0;
    option.access = 1;

    modid = sceKernelLoadModule(modname, 0, &option);
    if (modid < 0)
        return modid;

    return sceKernelStartModule(modid, 0, NULL, NULL, NULL);
}

int GetID3TagSize(char *fname)
{
    SceUID fd;
    char header[10];
    int size = 0;
    fd = sceIoOpen(fname, PSP_O_RDONLY, 0777);
    if (fd < 0)
        return 0;

    sceIoRead(fd, header, sizeof(header));
    sceIoClose(fd);

    if (!strncmp((char*)header, "ea3", 3) || !strncmp((char*)header, "EA3", 3)
      ||!strncmp((char*)header, "ID3", 3))
    {
        //get the real size from the syncsafe int
        size = header[6];
        size = (size<<7) | header[7];
        size = (size<<7) | header[8];
        size = (size<<7) | header[9];

        size += 10;

        if (header[5] & 0x10) //has footer
            size += 10;
         return size;
    }
    return 0;
}


int SeekNextFrameMP3(VIRTUAL_FILE *fd)
{
    int offset = 0;
    unsigned char buf[1024];
    unsigned char *pBuffer;
    int i;
    int size = 0;

    VirtualFileSeek(fd, 0, PSP_SEEK_CUR);
	offset = VirtualFileTell(fd);
    VirtualFileRead(buf, sizeof(buf), 1, fd);
    if (!strncmp((char*)buf, "ID3", 3) || !strncmp((char*)buf, "ea3", 3)) //skip past id3v2 header, which can cause a false sync to be found
    {
        //get the real size from the syncsafe int
        size = buf[6];
        size = (size<<7) | buf[7];
        size = (size<<7) | buf[8];
        size = (size<<7) | buf[9];

        size += 10;

        if (buf[5] & 0x10) //has footer
            size += 10;
    }

    VirtualFileSeek(fd, offset, PSP_SEEK_SET); //now seek for a sync
    while(1)
    {
        VirtualFileSeek(fd, 0, PSP_SEEK_CUR);
		offset = VirtualFileTell(fd);
        size = VirtualFileRead(buf, sizeof(buf), 1, fd);

        if (size <= 2)//at end of file
            return -1;

        if (!strncmp((char*)buf, "EA3", 3))//oma mp3 files have non-safe ints in the EA3 header
        {
            VirtualFileSeek(fd, (buf[4]<<8)+buf[5], PSP_SEEK_CUR);
            continue;
        }

        pBuffer = buf;
        for( i = 0; i < size; i++)
        {
            //if this is a valid frame sync (0xe0 is for mpeg version 2.5,2+1)
            if ( (pBuffer[i] == 0xff) && ((pBuffer[i+1] & 0xE0) == 0xE0))
            {
                offset += i;
                VirtualFileSeek(fd, offset, PSP_SEEK_SET);
                return offset;
            }
        }
       //go back two bytes to catch any syncs that on the boundary
        VirtualFileSeek(fd, -2, PSP_SEEK_CUR);
    }
}


void initME(){
    if (sceKernelDevkitVersion() == 0x01050001)
    {
        LoadStartAudioModule("flash0:/kd/me_for_vsh.prx", PSP_MEMORY_PARTITION_KERNEL);
        LoadStartAudioModule("flash0:/kd/audiocodec.prx", PSP_MEMORY_PARTITION_KERNEL);
    }
    else
    {
        sceUtilityLoadAvModule(PSP_AV_MODULE_AVCODEC);
    }
}

static int osl_at3Inited = 0, osl_mp3Inited = 0;

static int osl_at3Init()		{
	int result = 0;
	if (!osl_at3Inited)
        initME();

	osl_at3Inited = 1;

	return result;
}

static int osl_mp3Init()		{
	int result = 0;

	if (!osl_mp3Inited)
        initME();

	osl_mp3Inited = 1;
	return result;
}

static void osl_mp3DestroyInfo(MP3_INFO *info)		{

	if (info)			{
		if (info->codecBuffer)
			free(info->codecBuffer);

		//if (info->dataBuffer)
		//	free(info->dataBuffer);

		if (info->handle)
			VirtualFileClose(info->handle);

		if (info->getEDRAM)
			sceAudiocodecReleaseEDRAM(info->codecBuffer);

		free(info);
	}
}

static MP3_INFO *osl_mp3CreateInfo()		{
	int success = 0;
	MP3_INFO *info;

	info = (MP3_INFO*)malloc(sizeof(MP3_INFO));

	if (info)		{
		memset(info, 0, sizeof(MP3_INFO));

		//Allocate memory for the codec param buffer
		info->codecBuffer = memalign(64, 65 * sizeof(unsigned long));

		if (info->codecBuffer)		{
			memset(info->codecBuffer, 0, 65 * sizeof(unsigned long));
			success = 1;
		}
	}

	//Clean up if failed
	if (!success)		{
		osl_mp3DestroyInfo(info);
		info = NULL;
	}

	return info;
}

static void osl_at3DestroyInfo(AT3_INFO *info)		{
	//Does the same
	osl_mp3DestroyInfo((MP3_INFO*)info);
}


static int osl_mp3Load(const char *fileName, MP3_INFO *info)		{
	int success = 0;

	//Try to load the file
	info->handle = VirtualFileOpen((void*)fileName, 0, VF_AUTO, VF_O_READ);
	if (info->handle)			{
		info->channels = 2;
		info->samplerate = 44100; //this is mp3 file's samplerate, also can be 48000,....
		info->sample_per_frame = 1152;

        info->data_start_init = GetID3TagSize((char*)fileName);
        VirtualFileSeek(info->handle, info->data_start_init, PSP_SEEK_SET);

		info->data_start_init = VirtualFileTell(info->handle);

		//Initialize the codec
		if (sceAudiocodecCheckNeedMem(info->codecBuffer, 0x1002) >= 0)			{
			if (sceAudiocodecGetEDRAM(info->codecBuffer, 0x1002) >= 0)			{
				info->getEDRAM = 1;

				if (sceAudiocodecInit(info->codecBuffer, 0x1002) >= 0)
					success = 1;
			}
		}
	}
	return success;
}

static AT3_INFO *osl_at3CreateInfo()		{
	int success = 0;
	AT3_INFO *info;

	info = (AT3_INFO*)malloc(sizeof(AT3_INFO));

	if (info)		{
		memset(info, 0, sizeof(AT3_INFO));

		//Allocate memory for the codec param buffer
		info->codecBuffer = memalign(64, 65 * sizeof(unsigned long));

		if (info->codecBuffer)		{
			memset(info->codecBuffer, 0, 65 * sizeof(unsigned long));
			success = 1;
		}
	}

	//Clean up if failed
	if (!success)		{
		osl_at3DestroyInfo(info);
		info = NULL;
	}

	return info;
}

static int osl_at3Load(const char *fileName, AT3_INFO *info)		{
	int success = 0;

	//Try to load the file
	info->handle = VirtualFileOpen((void*)fileName, 0, VF_AUTO, VF_O_READ);
    info->data_start_init = GetID3TagSize((char*)fileName);
    VirtualFileSeek(info->handle, info->data_start_init, PSP_SEEK_SET);

	if (info->handle)			{
		u32 riff_header[2];
		if (VirtualFileRead(riff_header, 8, 1, info->handle) != 8)
			goto end;

		//RIFF
		if (riff_header[0] != 0x46464952)
			goto end;

		u32 wavefmt_header[3];

		if (VirtualFileRead(wavefmt_header, 12, 1, info->handle) != 12)
			goto end;

		if (wavefmt_header[0] != 0x45564157 || wavefmt_header[1] != 0x20746D66)
			goto end;

		u8* wavefmt_data = (u8*)malloc(wavefmt_header[2]);
		if (wavefmt_data == NULL)
			goto end;

		if (VirtualFileRead(wavefmt_data, wavefmt_header[2], 1, info->handle) != wavefmt_header[2] ) {
			free(wavefmt_data);
			goto end;
		}

		info->at3_type = *((u16*)wavefmt_data);
		info->at3_channels = *((u16*)(wavefmt_data+2));
		info->at3_samplerate = *((u32*)(wavefmt_data+4));
		info->at3_data_align = *((u16*)(wavefmt_data+12));

		if (info->at3_type == TYPE_ATRAC3PLUS)		{
			info->at3_at3plus_flagdata[0] = wavefmt_data[42];
			info->at3_at3plus_flagdata[1] = wavefmt_data[43];
		}

		free(wavefmt_data);

		u32 data_header[2];
		if (VirtualFileRead(data_header, 8, 1, info->handle) != 8)
			goto end;

		//data
		while (data_header[0] != 0x61746164)		{
			VirtualFileSeek(info->handle, data_header[1], PSP_SEEK_CUR);
			if (VirtualFileRead(data_header, 8, 1, info->handle) != 8)
				goto end;
		}

		info->data_start_init = VirtualFileTell(info->handle);
		info->at3_data_size = data_header[1];

		if (info->at3_data_size % info->at3_data_align != 0)
			goto end;

		if (info->at3_type == TYPE_ATRAC3)		{
			info->at3_channel_mode = 0x0;
			if (info->at3_data_align == 0xC0)				// atract3 have 3 bitrate, 132k,105k,66k, 132k align=0x180, 105k align = 0x130, 66k align = 0xc0
				info->at3_channel_mode = 0x1;
			info->sample_per_frame = 1024;
			info->dataBuffer = (u8*)memalign(64, 0x180);
			if (info->dataBuffer == NULL)
				goto end;
			info->codecBuffer[26] = 0x20;
			if ( sceAudiocodecCheckNeedMem(info->codecBuffer, 0x1001) < 0 )
				goto end;
			if ( sceAudiocodecGetEDRAM(info->codecBuffer, 0x1001) < 0 )
				goto end;
			info->at3_getEDRAM = 1;
			info->codecBuffer[10] = 4;
			info->codecBuffer[44] = 2;
			if (info->at3_data_align == 0x130 )
				info->codecBuffer[10] = 6;
			if (sceAudiocodecInit(info->codecBuffer, 0x1001) < 0 ) {
				goto end;
			}
			success = 1;
		}
		else if (info->at3_type == TYPE_ATRAC3PLUS)		{
			info->sample_per_frame = 2048;
			int temp_size = info->at3_data_align + 8;
			int mod_64 = temp_size & 0x3f;
			if (mod_64 != 0) temp_size += 64 - mod_64;
			info->dataBuffer = (u8*)memalign(64, temp_size);
			if (info->dataBuffer == NULL)
				goto end;
			info->codecBuffer[5] = 0x1;
			info->codecBuffer[10] = info->at3_at3plus_flagdata[1];
			info->codecBuffer[10] = (info->codecBuffer[10] << 8 ) | info->at3_at3plus_flagdata[0];
			info->codecBuffer[12] = 0x1;
			info->codecBuffer[14] = 0x1;
			if (sceAudiocodecCheckNeedMem(info->codecBuffer, 0x1000) < 0)
				goto end;
			if (sceAudiocodecGetEDRAM(info->codecBuffer, 0x1000) < 0)
				goto end;
			info->at3_getEDRAM = 1;
			if (sceAudiocodecInit(info->codecBuffer, 0x1000) < 0) {
				goto end;
			}
			success = 1;
		}
		else
			goto end;
	}

end:
	return success;
}


/*
	Callbacks standard
*/
void oslAudioCallback_StopSound_ME(OSL_SOUND *s)		{
	MP3_INFO *info = (MP3_INFO*)s->data;
	info->data_start = info->data_start_init;
	VirtualFileSeek(info->handle, info->data_start, PSP_SEEK_SET);
}

void oslAudioCallback_PlaySound_ME(OSL_SOUND *s)		{
	oslAudioCallback_StopSound_ME(s);
}

int oslAudioCallback_AudioCallback_MP3(unsigned int i, void* buf, unsigned int length)			{
	int eof = 0;
	MP3_INFO *info = (MP3_INFO*)osl_audioVoices[i].data;
	unsigned long decode_type = 0x1002;
	unsigned char mp3_header_buf[4];

	info->data_start = SeekNextFrameMP3(info->handle);

start:
	if (VirtualFileRead(mp3_header_buf, 4, 1, info->handle) != 4)			{
         eof = 1;
         goto end;
	}

	int mp3_header = mp3_header_buf[0];
	mp3_header = (mp3_header<<8) | mp3_header_buf[1];
	mp3_header = (mp3_header<<8) | mp3_header_buf[2];
	mp3_header = (mp3_header<<8) | mp3_header_buf[3];

	int bitrate = (mp3_header & 0xf000) >> 12;
	int padding = (mp3_header & 0x200) >> 9;
	int version = (mp3_header & 0x180000) >> 19;
	int samplerate = samplerates[version][ (mp3_header & 0xC00) >> 10 ];

	if ((bitrate > 14) || (version == 1) || (samplerate == 0) || (bitrate == 0))//invalid frame, look for the next one
	{
		info->data_start = SeekNextFrameMP3(info->handle);
		if(info->data_start < 0)
		{
			eof = 1;
			goto end;
		}
		goto start;
	}

	int frame_size = 0;
	if (version == 3) //mpeg-1
	{
		info->sample_per_frame = 1152;
		frame_size = 144000*bitrates[bitrate]/samplerate + padding;
	}else{
		info->sample_per_frame = 576;
		frame_size = 72000*bitrates_v2[bitrate]/samplerate + padding;
	}
	//int frame_size = 144000*bitrates[bitrate]/info->samplerate + padding;

	//if (info->dataBuffer)
	//	free(info->dataBuffer);
	//info->dataBuffer = (u8*)memalign(64, frame_size);

	VirtualFileSeek(info->handle, info->data_start, PSP_SEEK_SET); //seek back

	if ( VirtualFileRead(info->dataBuffer, frame_size, 1, info->handle) != frame_size)			{
		eof = 1;
		goto end;
	}

	info->data_start += frame_size;

	info->codecBuffer[7] = info->codecBuffer[10] = frame_size;
	info->codecBuffer[9] = info->sample_per_frame * 4;

	info->codecBuffer[6] = (unsigned long)info->dataBuffer;
	info->codecBuffer[8] = (unsigned long)buf;

	int res = sceAudiocodecDecode(info->codecBuffer, decode_type);
	if ( res < 0 ){
		info->data_start = SeekNextFrameMP3(info->handle);
		if(info->data_start < 0)
		{
			eof = 1;
			goto end;
		}
		goto start;
	}

end:
	if (eof)		{
//		osl_mp3CleanUp();
		//Efface le channel
		return 0;
	}

	return 1;
}

VIRTUAL_FILE **oslAudioCallback_ReactiveSound_ME(OSL_SOUND *s, VIRTUAL_FILE *f)			{
	MP3_INFO *info = (MP3_INFO*)s->data;
	VIRTUAL_FILE **w = &info->handle;
	return w;
}

VIRTUAL_FILE *oslAudioCallback_StandBy_ME(OSL_SOUND *s)		{
	MP3_INFO *info = (MP3_INFO*)s->data;
	return info->handle;
}

void oslAudioCallback_DeleteSound_ME(OSL_SOUND *s)		{
	osl_mp3DestroyInfo((MP3_INFO*)s->data);
	s->data = NULL;
}

int oslAudioCallback_AudioCallback_AT3(unsigned int i, void* buf, unsigned int length)			{
	AT3_INFO *info = (AT3_INFO*)osl_audioVoices[i].data;
	int eof = 0;
	unsigned long decode_type;

	if (info->at3_type == TYPE_ATRAC3)		{
		memset(info->dataBuffer, 0, 0x180);
		if (VirtualFileRead(info->dataBuffer, info->at3_data_align, 1, info->handle) != info->at3_data_align) {
			eof = 1;
			goto end;
		}
		if (info->at3_channel_mode) {
			memcpy(info->dataBuffer + info->at3_data_align, info->dataBuffer, info->at3_data_align);
		}
		decode_type = 0x1001;
	}
	else {
		memset(info->dataBuffer, 0, info->at3_data_align + 8);
		info->dataBuffer[0] = 0x0F;
		info->dataBuffer[1] = 0xD0;
		info->dataBuffer[2] = info->at3_at3plus_flagdata[0];
		info->dataBuffer[3] = info->at3_at3plus_flagdata[1];
		if (VirtualFileRead(info->dataBuffer + 8, info->at3_data_align, 1, info->handle) != info->at3_data_align) {
			eof = 1;
			goto end;
		}
		decode_type = 0x1000;
	}

	info->codecBuffer[6] = (unsigned long)info->dataBuffer;
	info->codecBuffer[8] = (unsigned long)buf;

	int res = sceAudiocodecDecode(info->codecBuffer, decode_type);
	if ( res < 0 )
		eof = 1;

end:
	if (eof)		{
//		osl_mp3CleanUp();
		//Efface le channel
		return 0;
	}

	return 1;
}


static void soundInit(const char *filename, OSL_SOUND *s, MP3_INFO *info)		{
	s->data = (void*)info;

	s->endCallback = NULL;
	s->volumeLeft = s->volumeRight = OSL_VOLUME_MAX;
	//No special format
	s->format = 0;
	//Always stereo output
	s->mono = 0;
	s->divider = OSL_FMT_44K;
	//MP3 files are always streamed for now
	s->isStreamed = 1;
	s->numSamples = info->sample_per_frame;

	//Streaming special information
	if (s->isStreamed)			{
		if (strlen(filename) < sizeof(s->filename))
			strcpy(s->filename, filename);
		s->suspendNumber = osl_suspendNumber;
	}

	s->playSound = oslAudioCallback_PlaySound_ME;
	s->stopSound = oslAudioCallback_StopSound_ME;
	s->standBySound = oslAudioCallback_StandBy_ME;
	s->reactiveSound = oslAudioCallback_ReactiveSound_ME;
	s->deleteSound = oslAudioCallback_DeleteSound_ME;
}

OSL_SOUND *oslLoadSoundFileMP3(const char *filename, int stream)		{
	OSL_SOUND *s = NULL;
	MP3_INFO *info;
	int success = 0;

//	osl_mp3Init();

	//Mp3 must be streamed for now
	if (stream & OSL_FMT_STREAM)			{

		s = (OSL_SOUND*)malloc(sizeof(OSL_SOUND));
		if (s)			{
			//Never forget that! If any member is added to OSL_SOUND, it is assumed to be zero!
			memset(s, 0, sizeof(OSL_SOUND));

			//Allocate the size of a mp3 info structure
			info = osl_mp3CreateInfo();

			if (info)	{
				if (osl_mp3Load(filename, info))			{
					soundInit(filename, s, info);
					s->audioCallback = oslAudioCallback_AudioCallback_MP3;
					success = 1;
				}
				else	{
					osl_mp3DestroyInfo(info);
					info = NULL;
				}
			}
		}

		if (!success)		{
			if (s)
				free(s);
			s = NULL;
		}
	}

	if (!s)
	{
		//oslHandleLoadNoFailError(filename);
		
	}
	return s;
}

OSL_SOUND *oslLoadSoundFileAT3(const char *filename, int stream)		{
	OSL_SOUND *s = NULL;
	AT3_INFO *info;
	int success = 0;

	//At3 & Mp3 must be streamed for now
	if (stream & OSL_FMT_STREAM)			{

		s = (OSL_SOUND*)malloc(sizeof(OSL_SOUND));
		if (s)			{
			//Never forget that! If any member is added to OSL_SOUND, it is assumed to be zero!
			memset(s, 0, sizeof(OSL_SOUND));

			//Allocate the size of a mp3 info structure
			info = osl_at3CreateInfo();

			if (info)	{
				if (osl_at3Load(filename, info))			{
					soundInit(filename, s, (MP3_INFO*)info);
					s->audioCallback = oslAudioCallback_AudioCallback_AT3;
					success = 1;
				}
				else	{
					osl_at3DestroyInfo(info);
					info = NULL;
				}
			}
		}

		if (!success)		{
			if (s)
				free(s);
			s = NULL;
		}
	}

	if (!s)
	{
		//oslHandleLoadNoFailError(filename);
		}
	return s;
}

void oslInitAudioME(int formats)		{
	if (formats & OSL_FMT_AT3)
		osl_at3Init();
	if (formats & OSL_FMT_MP3)
		osl_mp3Init();
}

