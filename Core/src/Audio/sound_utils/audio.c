/*
 * Using parts of pspaudiolib.
 *
 * Copyright (c) 2005 Adresd
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 *
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 */

	#include <pspthreadman.h>
	#include <pspaudio.h>

#include <Audio/sound_utils/oslib.h>
#include <Audio/sound_utils/audio.h>

/*
	*************************
*/

int (*osl_powerCallback)(int, int, void*)=NULL;
int (*osl_audioOldPowerCallback)(int, int, void*)=NULL;
OSL_AUDIO_VOICE osl_audioVoices[OSL_NUM_AUDIO_CHANNELS];

//int OSL_AUDIOSTREAM_BUFFER_SIZE=256;
int osl_audioDefaultNumSamples=512;
int osl_suspendNumber=0;
static int audio_ready=0;
static u32 *audio_sndbuf[OSL_NUM_AUDIO_CHANNELS];
long osl_filesave[OSL_NUM_AUDIO_CHANNELS];
int osl_audioStandBy;

static osl_audio_channelinfo AudioStatus[OSL_NUM_AUDIO_CHANNELS];

volatile int osl_audioActive[OSL_NUM_AUDIO_CHANNELS];

/*void oslSetChannelVolume(int channel, int left, int right)
{
  osl_audioVoices[channel].volumeRight = right;
  osl_audioVoices[channel].volumeLeft  = left;
}*/

void oslAudioChannelThreadCallback(int channel, void *buf, unsigned int reqn)
{
	void (*callback)(unsigned int channel, void *buf, unsigned int reqn);
	callback=AudioStatus[channel].callback;
}


void oslAudioSetChannelCallback(int channel, void *callback)
{
	AudioStatus[channel].callback = (void(*)(unsigned int, void*, unsigned int))callback;
}

int oslAudioOutBlocking(unsigned int channel, unsigned int vol1, unsigned int vol2, void *buf)
{
	if (!audio_ready) return -1;
//	if (channel>=OSL_NUM_AUDIO_CHANNELS) return -1;
	if (vol1>OSL_VOLUME_MAX) vol1=OSL_VOLUME_MAX;
	if (vol2>OSL_VOLUME_MAX) vol2=OSL_VOLUME_MAX;
	return sceAudioOutputPannedBlocking(AudioStatus[channel].handle,vol1,vol2,buf);
}

static int oslAudioChannelThread(int args, void *argp)
{
	volatile int bufidx=0;
	int channel=*(int *)argp;
//	int sample = 0;

	//Allocate 2 buffers of num_samples * 2 channels * 2 bytes = num_samples * 8
	audio_sndbuf[channel] = (u32*)malloc(osl_audioVoices[channel].numSamples << 3);

	if (audio_sndbuf[channel])			{
		memset(audio_sndbuf[channel], 0, osl_audioVoices[channel].numSamples << 3);

		while (osl_audioActive[channel] > 0)			{
			//Get a pointer to our actual buffer (we do double buffering)
			void *bufptr = audio_sndbuf[channel] + bufidx * osl_audioVoices[channel].numSamples;
			//Our callback function
			void (*callback)(unsigned int channel, void *buf, unsigned int reqn) = AudioStatus[channel].callback;
			//NE SERT A RIEN, NE FONCTIONNE PAS (pourquoi j'ai écrit ça moi?)
			AudioStatus[channel].inProgress = 1;
			if (callback && osl_audioActive[channel]==1)
				callback(channel, bufptr, osl_audioVoices[channel].numSamples);
			else
				memset(bufptr, 0, osl_audioVoices[channel].numSamples << 2);
			AudioStatus[channel].inProgress = 0;
//			if (sample++ <= 2)
//				memset(bufptr, 0, osl_audioVoices[channel].numSamples << 2);
			oslAudioOutBlocking(channel,osl_audioVoices[channel].sound->volumeLeft,osl_audioVoices[channel].sound->volumeRight,bufptr);
			bufidx = (bufidx ? 0 : 1);
		}
	}

	osl_audioActive[channel]=-2;				//Signale qu'on a terminé. Si le recreatechannel se produit juste entre le test (>0) et ceci, on n'aura pas de son!
	//Fin

	free(audio_sndbuf[channel]);

	AudioStatus[channel].threadhandle = -1;
	AudioStatus[channel].callback=NULL;
	if (AudioStatus[channel].handle != -1) {
		sceAudioChRelease(AudioStatus[channel].handle);
		AudioStatus[channel].handle = -1;
	}
	osl_audioVoices[channel].sound = NULL;
	osl_audioActive[channel]=0;
	sceKernelExitDeleteThread(0);
	return 0;
}



/******************************************************************************/

/*void oslClearAudio()
{
	int i;
	audio_ready=0;

	for (i=0; i<OSL_NUM_AUDIO_CHANNELS; i++) {
		osl_audioActive[i]=0;
		if (AudioStatus[i].threadhandle != -1) {
			sceKernelWaitThreadEnd(AudioStatus[i].threadhandle,NULL);
			sceKernelDeleteThread(AudioStatus[i].threadhandle);
		}
		AudioStatus[i].threadhandle = -1;
		osl_audioVoices[i].sound = NULL;
	}

	for (i=0; i<OSL_NUM_AUDIO_CHANNELS; i++) {
		if (AudioStatus[i].handle != -1) {
			sceAudioChRelease(AudioStatus[i].handle);
			AudioStatus[i].handle = -1;
		}
	}
}*/

void oslAudioDeleteChannel(int i)
{
	osl_audioActive[i]=-1;
/*	if (AudioStatus[i].threadhandle != -1) {
		sceKernelWaitThreadEnd(AudioStatus[i].threadhandle,NULL);
		sceKernelDeleteThread(AudioStatus[i].threadhandle);
	}
	AudioStatus[i].threadhandle = -1;

	AudioStatus[i].callback=NULL;
	if (AudioStatus[i].handle != -1) {
		sceAudioChRelease(AudioStatus[i].handle);
		AudioStatus[i].handle = -1;
	}
	osl_audioVoices[i].sound = NULL;*/
}

static void setChannelSound(int voice, OSL_SOUND *s)		{
	//Default sample number?
	if (s->numSamples == 0)
		osl_audioVoices[voice].numSamples = osl_audioDefaultNumSamples;
	else
		osl_audioVoices[voice].numSamples = s->numSamples;
	osl_audioVoices[voice].data = s->data;
	osl_audioVoices[voice].format = s->format;
	osl_audioVoices[voice].size = s->size;
	osl_audioVoices[voice].divider = s->divider;
	osl_audioVoices[voice].mono = s->mono;
	osl_audioVoices[voice].dataplus = s->dataplus;
	osl_audioVoices[voice].isStreamed = s->isStreamed;
//	osl_audioVoices[voice].volumeLeft = s->volumeLeft;
//	osl_audioVoices[voice].volumeRight = s->volumeRight;

	//Les deux sont liés
	osl_audioVoices[voice].sound = s;
}

int oslAudioCreateChannel(int i, int format, int numSamples, OSL_SOUND *s)
{
	int ret;
	int failed=0;
	char str[32];

	AudioStatus[i].handle = -1;
    AudioStatus[i].threadhandle = -1;
//    AudioStatus[i].volumeright = OSL_VOLUME_MAX;
//    AudioStatus[i].volumeleft  = OSL_VOLUME_MAX;
    AudioStatus[i].callback = 0;

	//Définit le bon son pour la channel
	setChannelSound(i, s);

	if ((AudioStatus[i].handle = sceAudioChReserve(i, osl_audioVoices[i].numSamples, format)) < 0)			{
		failed=1;
	}

	if (failed) {
		if (AudioStatus[i].handle != -1)
			sceAudioChRelease(AudioStatus[i].handle);
		AudioStatus[i].handle = -1;
		return -1;
	}

	audio_ready = 1;
	osl_audioActive[i]=1;
	strcpy(str,"audiot0");
	str[6]='0'+i;
	AudioStatus[i].threadhandle = sceKernelCreateThread(str,(SceKernelThreadEntry)&oslAudioChannelThread,0x10,0x10000,0,NULL);
	if (AudioStatus[i].threadhandle < 0) {
		AudioStatus[i].threadhandle = -1;
		failed=1;
	}
	ret=sceKernelStartThread(AudioStatus[i].threadhandle,sizeof(i),&i);
	if (ret!=0) {
		failed=1;
	}

	if (failed) {
		osl_audioActive[i]=0;
		if (AudioStatus[i].threadhandle != -1) {
			sceKernelWaitThreadEnd(AudioStatus[i].threadhandle,NULL);
			sceKernelDeleteThread(AudioStatus[i].threadhandle);
		}
		AudioStatus[i].threadhandle = -1;
		return -1;
	}
	return 0;
}

int oslAudioRecreateChannel(int i, int format, int numSamples, OSL_SOUND *s)
{
/*	int failed=0;
	if (AudioStatus[i].handle != -1) {
		sceAudioChRelease(AudioStatus[i].handle);
		AudioStatus[i].handle = -1;
	}

	if ((AudioStatus[i].handle = sceAudioChReserve(-1, OSL_NUM_AUDIO_SAMPLES, format))<0)			{
		failed=1;
	}

	if (failed) {
		if (AudioStatus[i].handle != -1)
			sceAudioChRelease(AudioStatus[i].handle);
		AudioStatus[i].handle = -1;
		return -1;
	}*/

	//Méthode originale, ne permet pas de redéfinir le nombre de samples :(
	if (osl_audioVoices[i].numSamples == numSamples && osl_audioVoices[i].sound == s)		{
		sceAudioChangeChannelConfig(i, format);
		//Les deux sont liés
		setChannelSound(i, s);
		osl_audioActive[i]=1;
	}
	else	{
		oslAudioDeleteChannel(i);
		//Attend qu'elle ait bien été désactivée
		while (osl_audioActive[i] != 0);
		sceKernelWaitThreadEnd(AudioStatus[i].threadhandle, NULL);
		oslAudioCreateChannel(i, format, numSamples, s);
	}

	return 0;
}

int oslGetSoundChannel(OSL_SOUND *s)		{
	int i;
	for (i=0; i<OSL_NUM_AUDIO_CHANNELS; i++)		{
		if (osl_audioVoices[i].sound == s)
			return i;
	}
	return -1;
}

// ------------------------------------------
#include <Audio/sound_utils/readwav.h>

void oslDecodeWav(unsigned int i, void* buf, unsigned int length)		{
	unsigned int j, k, samples=1<<(osl_audioVoices[i].divider);
	unsigned short *data = (unsigned short*)buf, cur1, cur2;
	WAVE_SRC *wav = (WAVE_SRC*)osl_audioVoices[i].dataplus;
	unsigned char *streambuffer;
	int len;
	if (wav->stream)		{
		//Longueur du segment à lire sur la MS -> 16 bits = *2
		len = (length * wav->fmt.bits_sample) >> 3;
		if (samples == 1)
			len <<= 1;
		else	{
			len>>=osl_audioVoices[i].divider;
		}
		if (osl_audioVoices[i].mono == 0)			//Stéréo
			len <<= 1;
		streambuffer = (unsigned char*)alloca(len);
		VirtualFileRead(streambuffer, len, 1, wav->fp);
		wav->streambuffer = streambuffer;
	}


	if (samples == 1)			{
		if (osl_audioVoices[i].mono == 0)
			length<<=1;
		for (j=0;j<length;j++)			{
			*data++ = get_next_wav_sample(wav);
		}
	}
	else		{
		length>>=osl_audioVoices[i].divider;
		if (osl_audioVoices[i].mono == 0)		{
			//Stéréo
			for (j=0;j<length;j++)			{
				cur1 = get_next_wav_sample(wav);
				cur2 = get_next_wav_sample(wav);
				for (k=0;k<samples;k++)			{
					*data++ = cur1;
					*data++ = cur2;
				}
			}
		}
		else		{
			//Mono
			for (j=0;j<length;j++)			{
				cur1 = get_next_wav_sample(wav);
				for (k=0;k<samples;k++)
					*data++ = cur1;
			}
		}
	}
	//Terminé, les poteaux
	if (wav->chunk_left <= 0)			{
		if (osl_audioVoices[i].sound->endCallback)		{
			if (osl_audioVoices[i].sound->endCallback(osl_audioVoices[i].sound, i))
				return;
		}
		oslAudioDeleteChannel(i);
	}
}

/* Fonction utilisée pour remplir le buffer audio (44'100 Hz, 16 bits, Mono) */
void oslAudioCallback(unsigned int i, void* buf, unsigned int length) {
	if (!osl_audioVoices[i].sound->audioCallback(i, buf, length))		{
		//Fin du channel
		if (osl_audioVoices[i].sound->endCallback)		{
			if (osl_audioVoices[i].sound->endCallback(osl_audioVoices[i].sound, i))
				return;
		}
		oslAudioDeleteChannel(i);
	}
}

#ifdef PSP
int oslAudioPowerCallback(int unknown, int pwrflags,void *common)			{
	static signed char audio[OSL_NUM_AUDIO_CHANNELS];
	OSL_SOUND *s;
	VIRTUAL_FILE *f = NULL;
	int i;

	if ((pwrflags & PSP_POWER_CB_POWER_SWITCH))		{
		osl_audioStandBy = 1;
		for (i=0;i<OSL_NUM_AUDIO_CHANNELS;i++)			{
			s = osl_audioVoices[i].sound;
			if (s)			{
				if (s->isStreamed)		{
					audio[i] = osl_audioActive[i];
					osl_audioActive[i] = 3;					//3 = suspendu/invalide
					f = s->standBySound(s);
					osl_filesave[i] = VirtualFileTell(f);
				}
			}
/*			s = osl_audioVoices[i].sound;
			if (s)			{
				if (s->isStreamed)		{
					if (s->format == OSL_FMT_ADPCM)
						f = (FILE*)s->data;
					else if (s->format == OSL_FMT_WAV)
						f = (FILE*)((WAVE_SRC*)s->dataplus)->fp;
					files[i] = ftell(f);
					fclose(f);
				}
			}*/
//			oslAudioDeleteChannel(i);
		}
		for (i=0;i<1000000;i--)
			i+=2;
//		fcloseall();
		osl_suspendNumber++;
	}
	else if(pwrflags & PSP_POWER_CB_RESUME_COMPLETE)		{
		osl_audioStandBy = 0;
	}

	if (osl_audioOldPowerCallback)
		return osl_audioOldPowerCallback(unknown, pwrflags, common);
	else
		return 0;
}
#endif

int oslInitAudio()			{
	int i;

	for (i=0;i<OSL_NUM_AUDIO_CHANNELS;i++)			{
		osl_audioActive[i]=0;
		osl_audioVoices[i].sound = NULL;
	}
	audio_ready=0;

	for (i=0; i<OSL_NUM_AUDIO_CHANNELS; i++) {
		AudioStatus[i].handle = -1;
		AudioStatus[i].threadhandle = -1;
//		AudioStatus[i].volumeright = OSL_VOLUME_MAX;
//		AudioStatus[i].volumeleft  = OSL_VOLUME_MAX;
		AudioStatus[i].callback = 0;
	}
	memset(osl_audioVoices, 0, sizeof(osl_audioVoices));
	osl_suspendNumber = 0;				//Peut-être ne pas le refaire à chaque fois...
#ifdef PSP
	osl_audioOldPowerCallback = osl_powerCallback;
	osl_powerCallback = oslAudioPowerCallback;
#endif
	osl_audioStandBy = 0;
	return 0;
}

//Supprime le système son, mais vous devriez supprimer tous vos sons avant!
void oslDeinitAudio()		{
	int i;
	//Stoppe toutes les voies actives
	for (i=0;i<OSL_NUM_AUDIO_CHANNELS;i++)
		oslAudioDeleteChannel(i);
	osl_powerCallback = osl_audioOldPowerCallback;
	audio_ready=0;
}

OSL_SOUND *oslLoadSoundFile(const char *filename, int stream)		{
	//Fichier BGM
	if (!strcmp(filename + strlen(filename) - 4, ".bgm"))
		return oslLoadSoundFileBGM(filename, stream);
	else if (!strcmp(filename + strlen(filename) - 4, ".wav"))
		return oslLoadSoundFileWAV(filename, stream);
	return NULL;
}

void oslDeleteSound(OSL_SOUND *s)			{
	//Vérifie que le son n'est pas en train d'être joué!
	oslStopSound(s);
	s->deleteSound(s);
	free(s);
}

//Retour: 0=pas réactivé, 1=réactivé, 2=réactivé, mais il faut recommencer le son depuis le début
int oslAudioReactiveSound(OSL_SOUND *s)		{
	//Le son est en streaming et la PSP a été mise en veille -> on ne peut pas jouer le son directement puisque le kernel ferme tous les fichiers.
	if (s->isStreamed && s->suspendNumber < osl_suspendNumber)		{
		VIRTUAL_FILE *f, **w=NULL;
		int i;
		//On réouvre le fichier - ATTENTION!!!! SI LES PARAMETRES DU SYSTEME DE FICHIERS VIRTUEL ONT CHANGÉ, CA PEUT MERDER!
		f = VirtualFileOpen(s->filename, 0, VF_AUTO, VF_O_READ);
		if (!f)
			return 0;
		w = s->reactiveSound(s, f);
		i = oslGetSoundChannel(s);
		if (i >= 0)
			VirtualFileSeek(f, osl_filesave[i], SEEK_SET);
		*w = f;
		s->suspendNumber = osl_suspendNumber;
		if (i >= 0)
			return 1;
		else
			return 2;
	}
	return 0;
}

int oslSoundLoopFunc(OSL_SOUND *s, int voice)		{
	oslPlaySound(s, voice);
	return 1;
}

//A appeler en boucle lorsqu'on a du streaming et que la PSP peut passer en veille
void oslAudioVSync()		{
	int i;
	if (osl_audioStandBy)
		return;
	for (i=0;i<OSL_NUM_AUDIO_CHANNELS;i++)		{
		if (osl_audioActive[i]!=3)
			continue;
		if (osl_audioVoices[i].sound)		{
			if (oslAudioReactiveSound(osl_audioVoices[i].sound))
				osl_audioActive[i]=1;
		}
	}
}

void oslPlaySound(OSL_SOUND *s, int voice)			{
	//Can't play a sound on a wrong voice
	if (voice < 0 || voice >= 8)
		return;

	//A VERIFIER: SI ELLE EST OCCUPEE, ON ATTEND UN PEU
	while(osl_audioActive[voice]==-2);

	//Crée la channel si elle n'existe pas déjà
	if (!osl_audioActive[voice])
		oslAudioCreateChannel(voice, s->mono, osl_audioVoices[voice].numSamples, s);
	else
		oslAudioRecreateChannel(voice, s->mono, osl_audioVoices[voice].numSamples, s);

	//Essaie de réactiver le son (après une mise en veille), sinon recommence à zéro
	if (oslAudioReactiveSound(s) != 1)		{
		s->playSound(s);
	}
	oslAudioSetChannelCallback(voice, oslAudioCallback);
}

void oslStopSound(OSL_SOUND *s)			{
	int voice = oslGetSoundChannel(s);
	s->stopSound(s);
	//Vérifie qu'il est bien en train d'être joué...
	if (voice >= 0)
		oslAudioDeleteChannel(voice);
}

void oslPauseSound(OSL_SOUND *s, int pause)			{
	int voice = oslGetSoundChannel(s);
	//Vérifie qu'il est bien en train d'être joué...
	if (voice >= 0)		{
		if (pause == -1)
			osl_audioActive[voice] = 3 - osl_audioActive[voice];
		else
			osl_audioActive[voice] = pause?2:1;
	}
}


/*
	Callbacks standard
*/
void oslAudioCallback_PlaySound_WAV(OSL_SOUND *s)			{
//	if (s->format == OSL_FMT_WAV)			{
		if (s->isStreamed)
			VirtualFileSeek(((WAVE_SRC*)s->dataplus)->fp, ((WAVE_SRC*)s->dataplus)->basefp, SEEK_SET);
		else
			((WAVE_SRC*)s->dataplus)->data = ((WAVE_SRC*)s->dataplus)->database;
		((WAVE_SRC*)s->dataplus)->chunk_left = ((WAVE_SRC*)s->dataplus)->chunk_base;
//	}
}

void oslAudioCallback_StopSound_WAV(OSL_SOUND *s)		{
	//Do nothing
}

int oslAudioCallback_AudioCallback_WAV(unsigned int i, void* buf, unsigned int length)			{
	oslDecodeWav(i, buf, length);
	return 1;
}

VIRTUAL_FILE **oslAudioCallback_ReactiveSound_WAV(OSL_SOUND *s, VIRTUAL_FILE *f)			{
	VIRTUAL_FILE **w = (VIRTUAL_FILE**)&((WAVE_SRC*)s->dataplus)->fp;
	return w;
}

VIRTUAL_FILE *oslAudioCallback_StandBy_WAV(OSL_SOUND *s)		{
	VIRTUAL_FILE *f = (VIRTUAL_FILE*)((WAVE_SRC*)s->dataplus)->fp;
	return f;
}

void oslAudioCallback_DeleteSound_WAV(OSL_SOUND *s)		{
	if (s->isStreamed)
		close_wave_src((WAVE_SRC*)s->dataplus);
	else
		free(((WAVE_SRC*)s->dataplus)->database);
	free(s->dataplus);
}

OSL_SOUND *oslLoadSoundFileWAV(const char *filename, int stream)			{
	OSL_SOUND *s;
	WAVE_SRC *wav;
	s = (OSL_SOUND*)malloc(sizeof(OSL_SOUND));
	if (!s)
		goto error;
	//Never forget that! If any member is added to OSL_SOUND, it is assumed to be zero!
	memset(s, 0, sizeof(OSL_SOUND));

	//Structure wav
	wav = (WAVE_SRC*)malloc(sizeof(WAVE_SRC));
	if(!wav)
	{
		free(s);
		goto error;
	}

	//Ouvre le fichier wav
	if (open_wave_src(wav, filename) < 0)				{
		free(s);
		free(wav);
		goto error;
	}
	if (wav->fmt.channels == 1)
		s->mono = 0x10;					//OSL_AUDIO_FORMAT_MONO
	else
		s->mono = 0x00;					//OSL_AUDIO_FORMAT_STEREO
	wav->basefp = VirtualFileTell(wav->fp);
	wav->chunk_base = wav->chunk_left;
	s->size = (int)wav->chunk_left;
	s->endCallback = NULL;
	s->volumeLeft = s->volumeRight = OSL_VOLUME_MAX;
	s->format = 0;
	//Use the default value
	s->numSamples = 0;
	if (wav->fmt.sample_rate >= 44100)
		s->divider = OSL_FMT_44K;
	else if (wav->fmt.sample_rate >= 22050)
		s->divider = OSL_FMT_22K;
	else
		s->divider = OSL_FMT_11K;
	s->isStreamed = stream;
	s->dataplus = wav;
	wav->stream = s->isStreamed;
	if (s->isStreamed)			{
		if (strlen(filename) >= sizeof(s->filename))
		{
			//oslFatalError("Sound file name too long!");
		}
		s->suspendNumber = osl_suspendNumber;
		strcpy(s->filename, filename);
	}
	else		{
		wav->database = (unsigned char*)malloc(s->size);
		if (!wav->database)		{
			free(s);
			close_wave_src(wav);
			free(wav);
			goto error;
		}
		VirtualFileRead(wav->database, s->size, 1, wav->fp);
		wav->data = wav->database;
		close_wave_src(wav);
	}

	s->audioCallback = oslAudioCallback_AudioCallback_WAV;
	s->playSound = oslAudioCallback_PlaySound_WAV;
	s->stopSound = oslAudioCallback_StopSound_WAV;
	s->standBySound = oslAudioCallback_StandBy_WAV;
	s->reactiveSound = oslAudioCallback_ReactiveSound_WAV;
	s->deleteSound = oslAudioCallback_DeleteSound_WAV;

	return s;

error:
	//oslHandleLoadNoFailError(filename);
	return NULL;
}
