#ifndef _OSL_BGM_H_
#define _OSL_BGM_H_

#ifdef __cplusplus
extern "C" {
#endif

//Le format BGM
typedef struct			{
	char strVersion[11];				// "OSLBGM v01"
	int format;							// Toujours 1
	int sampleRate;						// Taux d'échantillonnage
	unsigned char nbChannels;			// Mono ou stéréo
	unsigned char reserved[32];			// Réservé
} BGM_FORMAT_HEADER;

#ifdef __cplusplus
}
#endif

#endif

