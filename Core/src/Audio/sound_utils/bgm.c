#include <Audio/sound_utils/oslib.h>


typedef struct ADGlobals
{
	const unsigned char *data;
	int last_sample;
	int last_index;
} OSL_ADGlobals;

void oslStartAD(OSL_ADGlobals *ad, const unsigned char *data)
{
	ad->data = data;
	ad->last_sample = 0;
	ad->last_index = 0;
}

void oslRepriseAD(OSL_ADGlobals *ad, const unsigned char *data)
{
	ad->data = data;
}


static const signed char ima9_step_indices[16] =
{
  -1, -1, -1, -1, 2, 4, 7, 12,
  -1, -1, -1, -1, 2, 4, 7, 12
};

const unsigned short ima_step_table[89] =
{
      7,    8,    9,   10,   11,   12,   13,   14,   16,   17,
     19,   21,   23,   25,   28,   31,   34,   37,   41,   45,
     50,   55,   60,   66,   73,   80,   88,   97,  107,  118,
    130,  143,  157,  173,  190,  209,  230,  253,  279,  307,
    337,  371,  408,  449,  494,  544,  598,  658,  724,  796,
    876,  963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
   2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
   5894, 6484, 7132, 7845, 8630, 9493,10442,11487,12635,13899,
  15289,16818,18500,20350,22385,24623,27086,29794,32767
};

static inline int ima9_rescale(int step, unsigned int code)
{
  /* 0,1,2,3,4,5,6,9 */
  int diff = step >> 3;
  if(code & 1)
    diff += step >> 2;
  if(code & 2)
    diff += step >> 1;
  if(code & 4)
    diff += step;
  if((code & 7) == 7)
    diff += step >> 1;
  if(code & 8)
    diff = -diff;
  return diff;
}

short *oslDecodeADMono(OSL_ADGlobals *ad, short *dst, const unsigned char *src, unsigned int len, unsigned int samples, unsigned int streaming)
{
  int last_sample = ad->last_sample;
  int index = ad->last_index;
  unsigned int by = 0, i;
  unsigned char *streambuffer=NULL;
  if (streaming)		{
	streambuffer = (unsigned char*)alloca(len>>1);
	VirtualFileRead(streambuffer, len>>1, 1, (VIRTUAL_FILE*)src);
  }

  while(len > 0)
  {
    int step, diff;
    unsigned int code;

    if(index < 0)
      index = 0;
    if(index > 88)
      index = 88;
    step = ima_step_table[index];

    if(len & 1)
      code = by >> 4;
    else
    {
		if (streaming)
			by = *streambuffer++;
		else
			by = *src++;
		code = by & 0x0f;
    }

    diff = ima9_rescale(step, code);
    index += ima9_step_indices[code & 0x07];

    last_sample += diff;
    if(last_sample < -32768)
      last_sample = -32768;
    if(last_sample > 32767)
      last_sample = 32767;

	//Deux échantillons sur 2 voies
	for (i=0;i<samples;i++)		{
		*dst++ += last_sample;
	}

    len--;
  }

  ad->last_index = index;
  ad->last_sample = last_sample;
  ad->data = src;
  return dst;
}

/*
	Callbacks standard
*/
void oslAudioCallback_StopSound_BGM(OSL_SOUND *s)		{
	//Reset the file pointer
	if (s->isStreamed)
		VirtualFileSeek((VIRTUAL_FILE*)s->data, s->baseoffset, SEEK_SET);
}

void oslAudioCallback_PlaySound_BGM(OSL_SOUND *s)		{
	oslAudioCallback_StopSound_BGM(s);
	oslStartAD((OSL_ADGlobals*)s->dataplus, (unsigned char*)s->data);
}

int oslAudioCallback_AudioCallback_BGM(unsigned int i, void* buf, unsigned int length)			{
//	sample_t* ubuf = (sample_t*) buf;
	void *buf2;
	unsigned int l=0;
	OSL_ADGlobals *ad = (OSL_ADGlobals*)osl_audioVoices[i].dataplus;

	memset(buf, 0, length<<2);
	if (osl_audioVoices[i].size <= 0)
		return 1;

	l = length>>(osl_audioVoices[i].divider+1);
	if (l > osl_audioVoices[i].size)
		l = osl_audioVoices[i].size;
	buf2 = oslDecodeADMono(ad, (short*)buf, ad->data, l<<1, 1<<(osl_audioVoices[i].divider), osl_audioVoices[i].isStreamed);
	osl_audioVoices[i].size-=l;

	if (osl_audioVoices[i].size <= 0 && l)			{
		memset(buf2, 0, (u32)buf + (length << 1) - (u32)buf2);
/*		if (osl_audioVoices[i].sound->endCallback)		{
			if (osl_audioVoices[i].sound->endCallback(osl_audioVoices[i].sound, i))
				return;
		}
		oslAudioDeleteChannel(i);*/
		//Efface le channel
		return 0;
	}
	return 1;
}

VIRTUAL_FILE **oslAudioCallback_ReactiveSound_BGM(OSL_SOUND *s, VIRTUAL_FILE *f)			{
	VIRTUAL_FILE **w = (VIRTUAL_FILE**)&s->data;
	oslRepriseAD((OSL_ADGlobals*)s->dataplus, (unsigned char*)f);
	return w;
}

VIRTUAL_FILE *oslAudioCallback_StandBy_BGM(OSL_SOUND *s)		{
	VIRTUAL_FILE *f = (VIRTUAL_FILE*)s->data;
	return f;
}

void oslAudioCallback_DeleteSound_BGM(OSL_SOUND *s)		{
	if (s->isStreamed)		{
		VirtualFileClose((VIRTUAL_FILE*)s->data);
//		free(s->data);
	}
	else
		free(s->data);
}


OSL_SOUND *oslLoadSoundFileBGM(const char *filename, int stream)		{
	VIRTUAL_FILE *f;
	int debut, fin;
	OSL_SOUND *s;
	OSL_ADGlobals *ad=NULL;
	BGM_FORMAT_HEADER bfh;

	s = (OSL_SOUND*)malloc(sizeof(OSL_SOUND));
	if (!s)
		goto error;
	//Never forget that! If any member is added to OSL_SOUND, it is assumed to be zero!
	memset(s, 0, sizeof(OSL_SOUND));

	f = VirtualFileOpen((void*)filename, 0, VF_AUTO, VF_O_READ);
	if (!f)		{
		free(s);
		goto error;
	}
	VirtualFileRead(&bfh, sizeof(bfh), 1, f);
	//Vérifie l'en-tête
	if (strcmp(bfh.strVersion, "OSLBGM v01"))		{
		free(s);
		VirtualFileClose(f);
		goto error;
	}
	if (bfh.format == 1)		{
		//Pour l'adpcm, dataplus contient la structure définissant les données
		ad = (OSL_ADGlobals*)malloc(sizeof(OSL_ADGlobals));
		if (!ad)		{
			free(s);
			VirtualFileClose(f);
			goto error;
		}
		s->dataplus = ad;
	}
	debut = VirtualFileTell(f);
	s->baseoffset = debut;
	VirtualFileSeek(f, 0, SEEK_END);
	fin = VirtualFileTell(f);
	if (fin-debut <= 0)			{
		free(s);
		if (ad)	free(ad);
		VirtualFileClose(f);
		goto error;
	}
	VirtualFileSeek(f, 0, SEEK_SET);
	s->isStreamed = stream;
	if (s->isStreamed)			{
		if (strlen(filename) < sizeof(s->filename))
			strcpy(s->filename, filename);
		s->suspendNumber = osl_suspendNumber;
		s->data = (void*)f;
	}
	else		{
		s->data = malloc(fin-debut);
		if (!s->data)		{
			free(s);
			if (ad)	free(ad);
			VirtualFileClose(f);
			goto error;
		}
		VirtualFileRead(s->data, fin-debut, 1, f);
		VirtualFileClose(f);
	}
	s->endCallback = NULL;
	//Use the default value
	s->numSamples = 0;
	s->format = bfh.format;
	if (bfh.sampleRate == 44100)
		s->divider = OSL_FMT_44K;
	else if (bfh.sampleRate == 22050)
		s->divider = OSL_FMT_22K;
	else if (bfh.sampleRate == 11025)
		s->divider = OSL_FMT_11K;
	s->size = fin-debut;
//	if (bfh.format & OSL_FMT_STEREO)
//		s->mono = 0;				//OSL_AUDIO_FORMAT_STEREO
//	else
		s->mono = 0x10;				//OSL_AUDIO_FORMAT_MONO
	s->volumeLeft = s->volumeRight = OSL_VOLUME_MAX;

	s->audioCallback = oslAudioCallback_AudioCallback_BGM;
	s->playSound = oslAudioCallback_PlaySound_BGM;
	s->stopSound = oslAudioCallback_StopSound_BGM;
	s->standBySound = oslAudioCallback_StandBy_BGM;
	s->reactiveSound = oslAudioCallback_ReactiveSound_BGM;
	s->deleteSound = oslAudioCallback_DeleteSound_BGM;

	return s;

error:
	//oslHandleLoadNoFailError(filename);
	return NULL;
}
