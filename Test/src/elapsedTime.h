#ifndef ELAPSED_TIME_H__
#define ELAPSED_TIME_H__

#include <time.h>

#define BUFFER_ELAPSED_TIME 0.016

class ElapsedTime
{
public:
	static ElapsedTime& GetInstance()
	{
		static ElapsedTime instance;
		return instance;
	}

	double GetElapsedTime() { return m_elapsedTime; }
	unsigned int GetFPS() { return m_FPS; }
	void CalculateElapsedTime();
	void SetBufferElapsedTime(float fps) { m_elapsedTime = 1.f / fps; }

private:
	ElapsedTime() {}
	double m_elapsedTime;
	unsigned int m_FPS;
};

#endif // !ELAPSED_TIME_H__
