/*
	Based on PSPAUDIOLIB
 */
#ifndef AUDIO_H
#define AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif
#include "VirtualFile.h"

/** @defgroup audio Audio

	Audio in OSLib.
	@{
*/

/*
	Ressources allou�es � un son:
	Commun
	s (OSL_SOUND)
	
	Fichier WAV
		Commun
			s->dataplus (WAV_SRC)
		Stream�
			s->dataplus (WAV_SRC)
				fp (FILE)
		Normal
			s->dataplus (WAV_SRC)
				database (malloc: s->size)
	Fichier ADPCM
		Commun
			s->dataplus (OSL_ADGlobals)
		Stream�
			s->data (FILE)
		Normal
			s->data (malloc: s->size)
*/

#include <stdint.h>
typedef uint16_t u16;
typedef uint8_t u8;

/** Structure of a sound in OSLib. You can make your own "drivers" for other sound types but I can't guarantee they will be forward compatible, so please release your source when creating one ;-) */
typedef struct OSL_SOUND		{
	char filename[64];					//!< Original file name, so that the file can be reopen after a resume from stand-by. Used only if the song is streamed.
	void *data;							//!< User sound data (depends on the sound file format).
	void *dataplus;						//!< Extended user sound data.
	int baseoffset;						//!< Offset from the beginning of the file (without the header). Used to resume the sound after stand-by.
	int format;							//!< -
	int divider;						//!< -
	int size;							//!< -
	int mono;							//!< 0x10 for Mono, 0 for Stereo output
	int isStreamed;						//!< Holds whether the sound is streamed or not.
	u16 volumeLeft;						//!< Volume of the left channel (set volumeLeft == volumeRight for normal operation, another value creates a panning effect)
	u16 volumeRight;					//!< Volume of the right channel
	int suspendNumber;					//!< -
	int (*endCallback)(struct OSL_SOUND*, int);			//!< Function called when the sound has finished to play
	u8 userdata[32];					//!< Custom user data
	int numSamples;						//!< Number of samples per read. Default is osl_audioDefaultNumSamples, that is, 512.
	
	void (*playSound)(struct OSL_SOUND*);		//!< Custom function called when the sound must be played
	void (*stopSound)(struct OSL_SOUND*);		//!< Custom function called when the sound must be stopped (not paused, completely stopped)
	int (*audioCallback)(unsigned int, void*, unsigned int);		//!< Custom function which must feed a buffer with a certain number of samples
	VIRTUAL_FILE* (*standBySound)(struct OSL_SOUND*);				//!< Function called when the PSP enters in stand by mode
	VIRTUAL_FILE** (*reactiveSound)(struct OSL_SOUND*, VIRTUAL_FILE*);		//!< Function called when the sound must be reactivated (after a stand by).
	void (*deleteSound)(struct OSL_SOUND*);		//!< Custom function called to destroy the sound.
} OSL_SOUND;

/** Currently playing channel status, system part. Don't access it */
typedef struct {
  int active;
  int threadhandle;
  int handle;
//  int volumeleft;
//  int volumeright;
  void (*callback)(unsigned int channel, void *buf, unsigned int reqn);
  int inProgress;
} osl_audio_channelinfo;

/** Currently playing channel, user part. Only sound drivers should play with this, the user will only work with OSL_SOUND. */
typedef struct		{
	void *data, *dataplus;
	int format, divider, size, mono, isStreamed;
//	int volumeLeft, volumeRight;
	int numSamples;
	OSL_SOUND *sound;
} OSL_AUDIO_VOICE;


/** @defgroup audio_general General

	General tasks for audio.
	@{
*/

/** Initializes the audio system. Don't forget to call this before trying to play any song. Maybe your application will not crash if you forget it, but weird things can happen randomly! */
extern int  oslInitAudio();
/** Deinitializes the audio system. No sound will output anymore. Call this for clean-up (saves memory).

However, before deinitializing audio, you should delete all sounds by yourself (oslDeleteSound). */
extern void oslDeinitAudio();

/** Initializes Media Engine audio (need to call this before loading an at3 or mp3 file).

	\params formats
		One or more of the #oslInitAudioME_formats values, added together.

\b Important: Requires to be executed in kernel mode! Look for kernel mode programs in the documentation. */
extern void oslInitAudioME(int formats);

//extern void oslClearAudio();

/** Number of audio channels. No more than 8 sounds can be played at once! */
#define OSL_NUM_AUDIO_CHANNELS 8
/** This is the default volume for audio channels. Though the real maximum value is 0xffff, this value is the maximum value before distorsion may happen. */
#define OSL_VOLUME_MAX 0x8000

/** Sets the default number of samples per read. If you generate more samples at once, OSLib will need less calls, making it faster. But more data will be read at once, and the CPU will be blocked for
a longer time, which may be too much and cause screen tearing. It's only an advanced command, let it to default (512) if you don't know exactly what you are doing. */
#define oslAudioSetDefaultSampleNumber(num)			(osl_audioDefaultNumSamples = num)

//Don't access these
extern int osl_audioDefaultNumSamples;

/** @} */ // end of audio_general


/** @defgroup audio_load Loading

	Loading sounds.
	@{
*/

/** Loads a sound type and determines its format with its extension.
	\param filename
		Name of the file to be loaded. If the song must be streamed, always use files that are stored on the Memory Stick, alternate file sources have not been tested and may not always work properly.
	\param stream
		Either OSL_FMT_STREAM (sound is streamed) or OSL_FMT_NONE (sound is entierly loaded in memory). Streamed sounds use less memory but require more CPU power.

This remark applies for every format: the biggest the sample rate, the more CPU time it will need to be played back.
*/
extern OSL_SOUND *oslLoadSoundFile(const char *filename, int stream);
/** Loads a WAV sound file. See oslLoadSoundFile for more information. */
extern OSL_SOUND *oslLoadSoundFileWAV(const char *filename, int stream);
/** Loads a BGM sound file. See oslLoadSoundFile for more information. BGM is an audio format specific to OSLib. It stores "true" sound, taking less room than WAV, but is only mono. 

You can find an encoder in the distribution.

Some other formats are available in the OSTools extension library, take a look to it. */
extern OSL_SOUND *oslLoadSoundFileBGM(const char *filename, int stream);

/** Loads a MOD sound file. Requires to link with the mod library (-lmikmod in the library list). Currently supports the following formats: .mod, .it, .s3m and .xm.

You can only play one MOD song at a time! I didn't try to do it on 2 voices but I imagine it would play at a double speed and louder. Do NOT think this is normal and most important do not use it in your
game! This behavior is very likely to change in a future version of OSLib.

\b Note: Streaming is not supported for MOD playback. Files will always be loaded from RAM, so don't give OSL_FMT_STREAM as an argument for \e stream, else you may experience problems in a future
OSLib version! Set it to OSL_FMT_NONE for the moment.

\b Warning: Note that MOD playback is excessively heavy as much in terms of required memory (increases the size of your EBOOT) as speed (watch out the CPU usage when or without MOD playback:
with some songs it may increase up to 50%%!).
To reduce this problem, you can define the sample rate (#oslSetModSampleRate) which can help a bit. But it's still not recommended if you need a lot of CPU power for your game. However if your game does
not require a high CPU load but only a high GPU (graphic) load, this is not a problem, as sound decoding will mostly be done during periods where the CPU is waiting on the GPU to finish its drawing, and
thus not affecting performance very much. */
OSL_SOUND *oslLoadSoundFileMOD(const char *filename, int stream);

/** Loads an MP3 file. It is necessary to call #oslInitAudioME in kernel mode before, else your program will crash! */
OSL_SOUND *oslLoadSoundFileMP3(const char *filename, int stream);
/** Loads an AT3 file. It is necessary to call #oslInitAudioME in kernel mode before, else your program will crash! */
OSL_SOUND *oslLoadSoundFileAT3(const char *filename, int stream);


/** Sets the sample rate for the MOD player (it does not affect other formats!). Decreasing it reduces the audio quality but can reduce the CPU load required to decode MOD audio.
	\param freq
		Sample rate (in samples per second). The higher it is, the more CPU load is needed. Allowed values are 44100 (good quality), 22050 (medium quality), 11025 (low quality).
		The default is 44100, but if you need to use less CPU power, 22050 is a good choice. 11025 uses less CPU but it's not worth the big loss of quality IMHO, but it depends from the track in question.
	\param stereo
		The only allowed value is 1 currently. Do NOT put any other value!
	\param shift
		The shift sets the ratio for playback. Values are a power of two. 0 means 1x, 1 means 2x, 2 means 4x, etc.
		This parameter is necessary if you use another frequency than 44100, because the audio stream will always be played at 44100, due to the internal PSP audio capabilities. If you play a 22050 stream
		at 44100 Hz it will play twice as fast! To reduce this problem, set a shift of 1 (meaning 2x). This parameter is here so that you can make an accelerated music effect.
		For example if you set \e shift to 0 (44100 Hz) and \e freq to 22050, the sound will play twice as fast. Note that in this case (and only in this case!) the \e freq parameter can have another value
		than 11025, 22050 or 44100.

\b Note: it is unrecommended to set any other sample rate than 44100 (with a shift of 0), 22050 (with a shift of 1), 11025 (with a shift of 2) as it's tricky and not heavily tested. Also, it is currently
not sure that this behavior will be kept in the future.
\code
//The 3 "normal" possibilities, you can copy them as is if you need them.
oslSetModSampleRate(11025, 1, 2);			//Very low CPU, bad sound
oslSetModSampleRate(22050, 1, 1);			//Low CPU, medium sound
oslSetModSampleRate(44100, 1, 0);			//Normal CPU usage, good sound

//Extended possibilities, not recommended as said above.
oslSetModSampleRate(22050, 1, 0);			//Twice the normal speed
oslSetModSampleRate(33075, 1, 0);			//3/2 speed
oslSetModSampleRate(11025, 1, 1);			//2x speed, lower quality
\endcode */
void oslSetModSampleRate(int freq, int stereo, int shift);


/** @} */ // end of audio_load


/** @defgroup audio_play Player

	Audio player.
	@{
*/

/** Plays a sound on the specified channel. The channel is the channel number from 0 to 7. Playing a sound on an active channel stops the currently playing sound. The advantage of having 8 channels is that
you can play up to 8 sounds at the same time.

\code
//These must of course be loaded, but I skipped this step as it's not the goal of this sample.
OSL_SOUND *coin, *jump, *stomp, *music;
//Play the music on an own voice reserved for it (0)
oslPlaySound(music, 0);
[...]
//Play the "coin" sound on another channel than the music else it will replace it
oslPlaySound(coin, 1);
[...]
//Play a second time the "coin" sound. Putting it on the same channel will replace the currently playing coin sound.
//This is useful as sometimes you don't want or need two specific sounds to be played together.
//Especially, imagine your character is in a field of coins, and you take 10 coins in a short period.
//Having 10 times the coin sound played in a different channel would be very noisy.
//So it's good to stop the old sound to play the new one.
oslPlaySound(coin, 1);
[...]
//The jump sound have its own channel, the coin sound will be playing together with the jump and the music.
oslPlaySound(jump, 2);
[...]
//The stomp sound will replace the coin sound as it's played on the same channel.
//But the jump sound and the music are left unaffected.
oslPlaySound(stomp, 1);
\endcode */
extern void oslPlaySound(OSL_SOUND *s, int voice);
/** Stops a sound currently playing. */
extern void oslStopSound(OSL_SOUND *s);
/** Pauses a sound.
	\param s
		Sound to pause or resume.
	\param pause
		- 1: Pause the sound
		- 0: Resume the sound (where it was paused)
		- -1: Toggle pause / play
*/
extern void oslPauseSound(OSL_SOUND *s, int pause);

/** Deletes a sound, freeing associated memory. If the sound is currently being played, it will be stopped. */
extern void oslDeleteSound(OSL_SOUND *s);

/** Call this in your loop if a file is streamed and the PSP can go in stand-by mode. If you forget to call this often enough, the sound will not resume after a return from stand-by.

\b Important: This function is called by #oslEndFrame, so you don't need to do it twice if you've added a #oslEndFrame call to your code. Consider using #oslEndFrame instead of oslAudioVSync for your
new projects, as it's cleaner. */
extern void oslAudioVSync();

/** Sets the function to be called after a sound has finished to play.

The function has the following parameters: OSL_SOUND* (pointer to the sound), int (number of the voice). It must return the following value: 0 to destroy the channel, 1 to continue playback
(imagine you play something else or replay the same song).

\code
OSL_SOUND *s;

int myFunction(OSL_SOUND *s, int voice)		{
	//We replay the same sound on the same voice
	oslPlaySound(s, voice);
	//Return 1 because we continue sound playback
	return 1;
}

oslSetSoundEndCallback(sound, myFunction);
oslPlaySound(sound, 0);
[...] \endcode */
#define oslSetSoundEndCallback(s, fct)  (s->endCallback = (fct))

/** Sets whether sound is looped or not. This is done by defining the same callback as in the example above. */
#define oslSetSoundLoop(s,loop)			oslSetSoundEndCallback(s, (loop)?oslSoundLoopFunc:NULL)


//Do not use this.
enum {OSL_FMT_NONE=0};
enum {OSL_FMT_MASK=0xff};
//enum {OSL_FMT_WAV=0x100, OSL_FMT_ADPCM};									//Fichier wav
enum {OSL_FMT_MONO=0, OSL_FMT_STEREO=0x200, OSL_FMT_STREAM=0x400};
enum {OSL_FMT_44K=0, OSL_FMT_22K=1, OSL_FMT_11K=2};

/** Formats de fichier � initialiser pour #oslInitAudioME. */
enum oslInitAudioME_formats		{
	OSL_FMT_AT3 = 1,				//!< Atrac3 and Atrac3+
	OSL_FMT_MP3 = 2,				//!< Mpeg Audio-Layer 3
	OSL_FMT_ALL = 3,				//!< All formats
};


/** @} */ // end of audio_play



/** @defgroup audio_adv Advanced

	Lower level work with active audio channels.
	@{
*/

/** Returns the channel on which a specific sound is being played, or -1 if the sound is not being played currently. */
extern int oslGetSoundChannel(OSL_SOUND *s);

//Internal
extern int oslAudioCreateChannel(int i, int format, int numSamples, OSL_SOUND *s);
extern int oslAudioRecreateChannel(int i, int format, int numSamples, OSL_SOUND *s);
extern void oslAudioDeleteChannel(int i);
extern int  oslAudioOutBlocking(unsigned int channel, unsigned int vol1, unsigned int vol2, void *buf);
typedef int (* oslAudioThreadfunc_t)(int args, void *argp);

extern volatile int osl_audioActive[OSL_NUM_AUDIO_CHANNELS], osl_audioBusy[OSL_NUM_AUDIO_CHANNELS];
extern int osl_suspendNumber;


/** Represents the currently active voices properties. Especially, you can find a 'sound' member, which holds a pointer to the currently playing sound in this voice. */
extern OSL_AUDIO_VOICE osl_audioVoices[OSL_NUM_AUDIO_CHANNELS];

/** Standard callback function that loops a sound. Set as the sound end callback by oslSetSoundLoop. */
extern int oslSoundLoopFunc(OSL_SOUND *s, int voice);

/** @} */ // end of audio_adv


/** @} */ // end of audio


#ifdef __cplusplus
}
#endif

#endif

