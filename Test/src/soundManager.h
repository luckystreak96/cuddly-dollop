#ifndef SOUND_MANAGER_H__
#define SOUND_MANAGER_H__

//#include <stdlib.h>
#include <AL\al.h>
#include <AL\alc.h>
#include "Wave.h"
//#include <AL/sndfile.h>
#include <AL\alut.h>

class SoundManager
{
public:
	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	};
private:
	SoundManager();
	void CheckErrors();
};

#endif // !SOUND_MANAGER_H__
