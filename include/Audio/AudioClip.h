#pragma once
#include <Core/Core.h>
#include <Audio/sound_utils/audio.h>

namespace Stardust::Audio {
	class AudioClip{
	public:
		AudioClip(std::string path, bool stream = false);
		~AudioClip();

		void Play(int channel = 0);
		void Pause();
		void Stop();

		void SetVol(float vol);
		void SetPan(float pan);

	private:
		OSL_SOUND* snd;
	};
}