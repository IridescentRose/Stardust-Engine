/*
 * pgeWav.h: Header for WAV audio playback
 * This file is part of the "Phoenix Game Engine".
 *
 * Copyright (C) 2007 Phoenix Game Engine
 * Copyright (C) 2007 InsertWittyName <tias_dp@hotmail.com>
 *
 */

#ifndef __PGEWAV_H__
#define __PGEWAV_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeWav WAV Library
 *  @{
 */

/**
 * A WAV file struct
 */
typedef struct
{
	unsigned long channels;		/**<  Number of channels */
	unsigned long sampleRate;	/**<  Sample rate */
	unsigned long sampleCount;	/**<  Sample count */
	unsigned long dataLength;	/**<  Data length */
	unsigned long rateRatio;	/**<  Rate ratio (sampleRate / 44100 * 0x10000) */
	unsigned long playPtr;		/**<  Internal */
	unsigned long playPtr_frac;	/**<  Internal */
	unsigned int loop;			/**<  Loop flag */
	unsigned char *data;		/**< A pointer to the actual WAV data */
	unsigned long id;			/**<  The ID of the WAV */
	unsigned int bitPerSample;	/**<  The bit rate of the WAV */
    float leftvolume;			/**<  The left volume of the WAV */
	float rightvolume;			/**<  The right volume of the WAV */

} pgeWav;

/**
 * Initialise the WAV playback
 *
 * @returns 1 on success.
 */
int pgeWavInit(void);

/**
 * Shutdown WAV playback
 */
void pgeWavShutdown(void);

/**
 * Load a WAV file
 *
 * @param filename - Path of the file to load.
 *
 * @returns A pointer to a ::pgeWav struct or NULL on error.
 */
pgeWav *pgeWavLoad(const char *filename);

/**
 * Load a WAV file from memory
 *
 * @param buffer - Buffer that contains the WAV data.
 *
 * @param size - Size of the buffer.
 *
 * @returns A pointer to a ::pgeWav struct or NULL on error.
 */
pgeWav *pgeWavLoadMemory(const unsigned char *buffer, int size);

/**
 * Unload a previously loaded WAV file
 *
 * @param wav - A valid ::pgeWav
 */
void pgeWavUnload(pgeWav *wav);

/**
 * Start playing a loaded WAV file
 *
 * @param wav A pointer to a valid ::pgeWav struct.
 *
 * @returns 1 on success
 */
int pgeWavPlay(pgeWav *wav);

/**
 * Stop playing a loaded WAV
 *
 * @param wav A pointer to a valid ::pgeWav struct.
 *
 * @returns 1 on success
 */
void pgeWavStop(pgeWav *wav);

/**
 * Stop playing all WAVs
 */
void pgeWavStopAll(void);

/**
 * Set the loop of the WAV playback
 *
 * @param wav - A pointer to a valid ::pgeWav struct.
 *
 * @param loop - Set to 1 to loop, 0 to playback once.
 */
void pgeWavLoop(pgeWav *wav, unsigned int loop);

/**
 * Set the volume of the WAV
 *
 * @param wav - A pointer to a valid ::pgeWav struct.
 *
 * @param leftvolume - Set to 0 for no volume, 100 for full volume.
 *
 * @param rightvolume - Set to 0 for no volume, 100 for full volume.
 */
void pgeWavVolume(pgeWav *wav, unsigned char leftvolume, unsigned char rightvolume);

/**
 * Set the pitch of the WAV
 *
 * @param wav - A pointer to a valid ::pgeWav struct.
 *
 * @param pitch - The new pitch.
 */
void pgeWavPitch(pgeWav *wav, float pitch);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEWAV_H__
