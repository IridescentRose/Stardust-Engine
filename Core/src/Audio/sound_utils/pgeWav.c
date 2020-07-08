#include <pspaudio.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <pspiofilemgr.h>
#include <pspthreadman.h>
#include <Audio/sound_utils/pgeWav.h>

#define PGE_WAV_MAX_SLOTS 16

static pgeWav pgeWavInfo[PGE_WAV_MAX_SLOTS];
static int pgeWavPlaying[PGE_WAV_MAX_SLOTS];
static int pgeWavId[PGE_WAV_MAX_SLOTS];

static short *pgeWavSamples;
static unsigned long pgeWavReq;
static int pgeWavIdFlag = 0;

static int pgeWavInitFlag = 0;

#define PGE_NUM_AUDIO_CHANNELS	4
#define PGE_NUM_AUDIO_SAMPLES	1024
#define PGE_VOLUME_MAX			0x8000

typedef void (* pgeAudioLibCallback)(void *buf, unsigned int reqn, void *pdata);

typedef struct
{
	int threadHandle;
	int handle;
	int volumeLeft;
	int volumeRight;
	pgeAudioLibCallback callback;
	void *data;

} pgeAudioLibChannelInfo;

static int pgeAudioLibReady = 0;
static short pgeAudioLibSoundBuffer[PGE_NUM_AUDIO_CHANNELS][2][PGE_NUM_AUDIO_SAMPLES][2];

static pgeAudioLibChannelInfo pgeAudioLibStatus[PGE_NUM_AUDIO_CHANNELS];

static volatile int pgeAudioLibTerminate = 0;

static void pgeAudioLibSetChannelCallback(int channel, pgeAudioLibCallback callback, void *data)
{
	volatile pgeAudioLibChannelInfo *pci = &pgeAudioLibStatus[channel];

	if (callback == 0)
		pci->callback = 0;
	else
	{
		pci->callback = callback;
		sceKernelWakeupThread(pci->threadHandle);
	}
}

static int pgeAudioLibOutBlocking(unsigned int channel, unsigned int left, unsigned int right, void *data)
{
	if (!pgeAudioLibReady)
		return(-1);

	if (channel >= PGE_NUM_AUDIO_CHANNELS)
		return(-1);

	if (left > PGE_VOLUME_MAX)
		left = PGE_VOLUME_MAX;

	if (right > PGE_VOLUME_MAX)
		right = PGE_VOLUME_MAX;

	return sceAudioOutputPannedBlocking(pgeAudioLibStatus[channel].handle, left, right, data);
}

static int pgeAudioLibChannelThread(int args, void *argp)
{
	volatile int bufidx = 0;

	int channel = *(int *) argp;

	while (pgeAudioLibTerminate == 0)
	{
		void *bufptr = &pgeAudioLibSoundBuffer[channel][bufidx];
		pgeAudioLibCallback callback;
		callback = pgeAudioLibStatus[channel].callback;

		if (callback)
		{
			callback(bufptr, PGE_NUM_AUDIO_SAMPLES, pgeAudioLibStatus[channel].data);
			pgeAudioLibOutBlocking(channel, pgeAudioLibStatus[channel].volumeLeft, pgeAudioLibStatus[channel].volumeRight, bufptr);
		}
		else
		{
			sceKernelSleepThread();
		}

	bufidx = (bufidx ? 0:1);
	}

	sceKernelExitThread(0);

	return(0);
}

static int pgeAudioLibInit(int priority)
{
	int i, ret;
	int failed = 0;
	char str[32];

	pgeAudioLibTerminate = 0;
	pgeAudioLibReady = 0;

	for (i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		pgeAudioLibStatus[i].handle = -1;
		pgeAudioLibStatus[i].threadHandle = -1;
		pgeAudioLibStatus[i].volumeRight = PGE_VOLUME_MAX;
		pgeAudioLibStatus[i].volumeLeft  = PGE_VOLUME_MAX;
		pgeAudioLibStatus[i].callback = 0;
		pgeAudioLibStatus[i].data = 0;
	}

	for (i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		if ((pgeAudioLibStatus[i].handle = sceAudioChReserve(-1, PGE_NUM_AUDIO_SAMPLES, 0)) < 0)
			failed = 1;
	}

	if (failed)
	{
		for (i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
		{
			if (pgeAudioLibStatus[i].handle != -1)
				sceAudioChRelease(pgeAudioLibStatus[i].handle);

			pgeAudioLibStatus[i].handle = -1;
		}

		return 0;
	}

	pgeAudioLibReady = 1;

	strcpy(str, "PgeAudioThread0");

	for (i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		str[14] = '0' + i;
		pgeAudioLibStatus[i].threadHandle = sceKernelCreateThread(str, (void*)&pgeAudioLibChannelThread, priority, 0x10000, PSP_THREAD_ATTR_USER, NULL);

		if (pgeAudioLibStatus[i].threadHandle < 0)
		{
			pgeAudioLibStatus[i].threadHandle = -1;
			failed = 1;
			break;
		}

		ret = sceKernelStartThread(pgeAudioLibStatus[i].threadHandle, sizeof(i), &i);

		if (ret != 0)
		{
			failed = 1;
			break;
		}
	}

	if (failed)
	{
		pgeAudioLibTerminate = 1;

		for (i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
		{
			if (pgeAudioLibStatus[i].threadHandle != -1)
			{
				sceKernelDeleteThread(pgeAudioLibStatus[i].threadHandle);
			}

			pgeAudioLibStatus[i].threadHandle = -1;
		}

		pgeAudioLibReady = 0;

		return 0;
	}

	return 1;
}

static void pgeAudioLibShutdown(void)
{
	int i;
	pgeAudioLibReady = 0;
	pgeAudioLibTerminate = 1;

	for (i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		if (pgeAudioLibStatus[i].threadHandle != -1)
		{
			sceKernelDeleteThread(pgeAudioLibStatus[i].threadHandle);
		}

		pgeAudioLibStatus[i].threadHandle = -1;
	}

	for (i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		if (pgeAudioLibStatus[i].handle != -1)
		{
			sceAudioChRelease(pgeAudioLibStatus[i].handle);
			pgeAudioLibStatus[i].handle = -1;
		}
	}
}

static void wavout_snd_callback(void *_buf, unsigned int _reqn, void *pdata)
{
	int i,slot;
	pgeWav *wi;
	unsigned long ptr, frac;
	short *buf = _buf;

	pgeWavSamples = _buf;
	pgeWavReq = _reqn;

	for(i = 0; i < _reqn; i++)
	{
		int outr = 0, outl = 0;

		for(slot = 0; slot < PGE_WAV_MAX_SLOTS; slot++)
		{
			if(!pgeWavPlaying[slot]) continue;

			wi = &pgeWavInfo[slot];
			frac = wi->playPtr_frac + wi->rateRatio;
			wi->playPtr = ptr = wi->playPtr + (frac>>16);
			wi->playPtr_frac = (frac & 0xffff);

			if(ptr >= wi->sampleCount)
			{
				if(wi->loop)
				{
					wi->playPtr = 0;
					wi->playPtr_frac = 0;
					ptr = 0;
				}
				else
				{
					pgeWavPlaying[slot] = 0;
					break;
				}
			}

			short *src16 = (short *)wi->data;
			unsigned char *src8 = (unsigned char *)wi->data;

			if(wi->channels == 1)
			{
				if(wi->bitPerSample == 8)
				{
					outl += (src8[ptr] * 256) - 32768 * wi->leftvolume;
					outr += (src8[ptr] * 256) - 32768 * wi->rightvolume;
				}
				else
				{
					outl += src16[ptr] * wi->leftvolume;
					outr += src16[ptr] * wi->rightvolume;
				}
			}
			else
			{
				if(wi->bitPerSample == 8)
				{
					outl += (src8[ptr*2] * 256) - 32768 * wi->leftvolume;
					outr += (src8[ptr*2+1] * 256) - 32768 * wi->rightvolume;
				}
				else
				{
					outl += src16[ptr*2] * wi->leftvolume;
					outr += src16[ptr*2+1] * wi->rightvolume;
				}
			}
		}

		if(outl < -32768)
			outl = -32768;
		else if (outl > 32767)
			outl = 32767;

		if(outr < -32768)
			outr = -32768;
		else if (outr > 32767)
			outr = 32767;

		*(buf++) = outl;
		*(buf++) = outr;
	}
}

int pgeWavInit(void)
{
	int i;

	pgeAudioLibInit(0x12);

	pgeAudioLibSetChannelCallback(0, wavout_snd_callback, 0);

	for(i = 0; i < PGE_WAV_MAX_SLOTS; i++)
		pgeWavPlaying[i] = 0;

	pgeWavInitFlag = 1;

	return(1);
}

void pgeWavShutdown(void)
{
	if(pgeWavInitFlag)
		pgeAudioLibShutdown();
}

void pgeWavStop(pgeWav *wav)
{
	int i;

	for(i = 0; i < PGE_WAV_MAX_SLOTS; i++)
	{
		if(wav->id == pgeWavId[i])
			pgeWavPlaying[i] = 0;
	}
}

void pgeWavStopAll(void)
{
	int i;

	for(i = 0; i < PGE_WAV_MAX_SLOTS; i++)
		pgeWavPlaying[i] = 0;
}

void pgeWavLoop(pgeWav *wav, unsigned int loop)
{
	wav->loop = loop;
}

void pgeWavVolume(pgeWav *wav, unsigned char leftvolume, unsigned char rightvolume)
{
	if(leftvolume > 100)
		leftvolume = 100;

	if(rightvolume > 100)
		rightvolume = 100;

	wav->leftvolume = leftvolume/100.0f;
	wav->rightvolume = rightvolume/100.0f;
}

void pgeWavPitch(pgeWav *wav, float pitch)
{
	wav->rateRatio = ((wav->sampleRate*0x4000)/11025) * pitch;

	if(wav->rateRatio < (2000*0x4000)/11025)
		wav->rateRatio = (2000*0x4000)/11025;
	else if(wav->rateRatio > (100000*0x4000)/11025)
		wav->rateRatio = (100000*0x4000)/11025;
}

int pgeWavPlay(pgeWav *wav)
{
	if(!pgeWavInitFlag)
		return(0);

	int i;

	pgeWav *wid;

	for(i = 0;i < PGE_WAV_MAX_SLOTS;i++)
	{
		if(pgeWavPlaying[i] == 0)
			break;
	}

	if(i == PGE_WAV_MAX_SLOTS)
		return(0);

	wid = &pgeWavInfo[i];
	wid->channels = wav->channels;
	wid->sampleRate = wav->sampleRate;
	wid->sampleCount = wav->sampleCount;
	wid->dataLength = wav->dataLength;
	wid->data = wav->data;
	wid->rateRatio = wav->rateRatio;
	wid->playPtr = 0;
	wid->playPtr_frac = 0;
	wid->loop = wav->loop;
	wid->id = wav->id;
    wid->leftvolume = wav->leftvolume;
	wid->rightvolume = wav->rightvolume;
	pgeWavPlaying[i] = 1;
	pgeWavId[i] = wav->id;
	wid->bitPerSample = wav->bitPerSample;

	return(1);
}

static pgeWav *pgeWavLoadInternal(pgeWav *wav, unsigned char *wavfile, int size)
{
	unsigned long channels;
	unsigned long samplerate;
	unsigned long blocksize;
	unsigned long bitpersample;
	unsigned long datalength;
	unsigned long samplecount;

	if(memcmp(wavfile, "RIFF", 4) != 0)
	{
		free(wav);
		return NULL;
	}

	channels = *(short *)(wavfile+0x16);
	samplerate = *(long *)(wavfile+0x18);
	blocksize = *(short *)(wavfile+0x20);
	bitpersample = *(short *)(wavfile+0x22);

	int i;

	for(i = 0; memcmp(wavfile + 0x24 + i, "data", 4) != 0; i++)
	{
		if(i == 0xFF)
		{
			free(wav);
			return NULL;
		}
	}

	datalength = *(unsigned long *)(wavfile + 0x28 + i);

	if(datalength + 0x2c > size)
	{
		free(wav);
		return NULL;
	}

	if(channels != 2 && channels != 1)
	{
		free(wav);
		return NULL;
	}

	if(samplerate > 100000 || samplerate < 2000)
	{
		free(wav);
		return NULL;
	}

	if(channels == 2)
	{
		samplecount = datalength/(bitpersample>>2);
	}
	else
	{
		samplecount = datalength/((bitpersample>>2)>>1);
	}

	if(samplecount <= 0)
	{
		free(wav);
		return NULL;
	}

	wav->channels = channels;
	wav->sampleRate = samplerate;
	wav->sampleCount = samplecount;
	wav->dataLength = datalength;
	wav->data = wavfile + 0x2c;
	wav->rateRatio = (samplerate*0x4000)/11025;
	wav->playPtr = 0;
	wav->playPtr_frac= 0;
	wav->loop = 0;
	pgeWavIdFlag++;
	wav->id = pgeWavIdFlag;
	wav->bitPerSample = bitpersample;
    wav->leftvolume = 1.0f;
	wav->rightvolume = 1.0f;

	return wav;
}

pgeWav *pgeWavLoad(const char *filename)
{
	unsigned long filelen;

	unsigned char *wavfile;
	pgeWav *wav;

	int fd = sceIoOpen(filename, PSP_O_RDONLY, 0777);

	if(fd < 0)
		return NULL;

	long lSize;

	lSize = sceIoLseek32(fd, 0, PSP_SEEK_END);
	sceIoLseek32(fd, 0, PSP_SEEK_SET);

	wav = malloc(lSize + sizeof(wav));
	wavfile = (unsigned char*)(wav) + sizeof(wav);

	filelen = sceIoRead(fd, wavfile, lSize);

	sceIoClose(fd);

	return(pgeWavLoadInternal(wav, wavfile, filelen));
}

pgeWav *pgeWavLoadMemory(const unsigned char *buffer, int size)
{
	unsigned char *wavfile;
	pgeWav *wav;

	wav = malloc(size + sizeof(wav));
	wavfile = (unsigned char*)(wav) + sizeof(wav);

	memcpy(wavfile, (unsigned char*)buffer, size);

	return(pgeWavLoadInternal(wav, wavfile, size));
}

void pgeWavUnload(pgeWav *wav)
{
	if(wav != NULL)
		free(wav);
}
