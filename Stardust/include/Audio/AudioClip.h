#pragma once
#include <Core/Core.h>
#include <Platform/Platform.h>

namespace Stardust::Audio {
	class AudioClip{
	public:
		AudioClip(std::string path, bool stream = false);
		~AudioClip();

		void Play(int channel = 0);
		void Pause();
		void Stop();
		void SetLoop(bool loop);

		void SetVol(float vol);
		void SetPan(float pan);

	private:
		Platform::detail::SoundEffect* snd;
	};
}