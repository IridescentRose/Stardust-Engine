#include <Audio/AudioClip.h>

namespace Stardust::Audio {
	AudioClip::AudioClip(std::string path, bool b)
	{
		snd = Platform::detail::loadSound(path.c_str(), b);
	}
	AudioClip::~AudioClip()
	{
		#if CURRENT_PLATFORM == PLATFORM_VITA
			Platform::detail::deleteSound(&snd);
		#else
			Platform::detail::deleteSound(snd);
		#endif
	}

	void AudioClip::Play(int c)
	{
		Platform::detail::playSoundEffect(snd, c);
	}
	void AudioClip::Pause()
	{
		Platform::detail::pauseSoundEffect(snd);
	}
	void AudioClip::Stop()
	{
		Platform::detail::stopSoundEffect(snd);
	}

	void AudioClip::SetLoop(bool loop)
	{
		Platform::detail::setSoundLoop(snd, loop);
	}

	void AudioClip::SetVol(float vol)
	{

	}
	void AudioClip::SetPan(float pan)
	{

	}
}