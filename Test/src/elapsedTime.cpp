#include "elapsedTime.h"

//Return elapsed time in seconds
void ElapsedTime::CalculateElapsedTime()
{
	static int prev_time = 0;
	static int counter = 0;
	static clock_t prev_point = 0;
	clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);
	if (uptime / 1000 != prev_time)
	{
		prev_time = uptime / 1000;
		m_FPS = counter;
		counter = 0;
	}
	m_elapsedTime = (double)(uptime - prev_point) / 1000.0;
	if (m_elapsedTime > 0.03)
		m_elapsedTime = 0.03;

	if (m_elapsedTime < .005f)
	{
		m_elapsedTime = -1;
	}
	else
	{
		prev_point = uptime;
		counter++;
	}
}