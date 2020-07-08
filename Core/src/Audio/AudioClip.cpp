#include <Audio/AudioClip.h>

namespace Stardust::Audio {
	AudioClip::AudioClip(std::string path, bool b)
	{
		snd = oslLoadSoundFile(path.c_str(), b ? OSL_FMT_STREAM : OSL_FMT_NONE);
	}
	AudioClip::~AudioClip()
	{
		oslDeleteSound(snd);
	}

	void AudioClip::Play(int c)
	{
		oslPlaySound(snd, c);
	}
	void AudioClip::Pause()
	{
		oslPauseSound(snd, -1);
	}
	void AudioClip::Stop()
	{
		oslStopSound(snd);
	}

	void AudioClip::SetLoop(bool loop)
	{
		oslSetSoundLoop(snd, loop);
	}

	void AudioClip::SetVol(float vol)
	{
		snd->volumeLeft = vol * 100;
		snd->volumeRight = vol * 100;
	}
	void AudioClip::SetPan(float pan)
	{
		float vol = (float)(snd->volumeLeft + snd->volumeRight) / 2.0f;
		snd->volumeLeft = vol * pan;
		snd->volumeRight = vol * (1.f - pan);
	}
}