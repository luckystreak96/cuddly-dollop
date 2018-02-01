#ifndef SOUND_UTILS_H__
#define SOUND_UTILS_H__

class SoundUtils
{
	public:
		static void SoundUtils::Format(std::string filename, ALenum& format);
		static void SoundUtils::Size(std::string filename, ALsizei& size);
		static void SoundUtils::Freq(std::string filename, ALsizei& freq);
		static void SoundUtils::Data(std::string filename, ALvoid* data);
};

#endif
