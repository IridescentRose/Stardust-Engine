#include <Audio/AudioClip.h>

namespace Stardust::Audio {
	AudioClip::AudioClip(std::string path, bool b)
	{
		snd = oslLoadSoundFile(path.c_str, b ? OSL_FMT_STREAM : OSL_FMT_NONE);
	}
	AudioClip::~AudioClip()
	{
		oslDeleteSound(snd);
	}

	void AudioClip::Play(int c)
	{
		oslPlaySound(snd, 0);
	}
	void AudioClip::Pause()
	{
		oslPauseSound(snd, -1);
	}
	void AudioClip::Stop()
	{
		oslStopSound(snd);
	}

	void AudioClip::SetVol(float vol)
	{
		
	}
	void AudioClip::SetPan(float pan)
	{

	}
}