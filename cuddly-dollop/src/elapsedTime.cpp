#include "elapsedTime.h"
#include <GLFW/glfw3.h>
#include <math.h>

//Return elapsed time in seconds
void ElapsedTime::CalculateElapsedTime()
{
	static double prev_time = 0;
	static double counter = 0;
	static double prev_point = 0;
	double uptime = glfwGetTime();
	if (floor(uptime) != floor(prev_time))
	{
		prev_time = floor(uptime);
		m_FPS = counter;
		m_secondPassed = true;
		counter = 0;
	}
	else
		m_secondPassed = false;

	m_elapsedTime = uptime - prev_point;

	if (m_elapsedTime > 0.03)
		m_elapsedTime = 0.03;

	//if (m_elapsedTime < .005f)
	//{
	//	m_elapsedTime = -1;
	//}
	//else
	{
		prev_point = uptime;
		counter++;
	}

	//static int prev_time = 0;
	//static int counter = 0;
	//static clock_t prev_point = 0;
	//clock_t uptime = clock() / (CLOCKS_PER_SEC / 1000);
	//if (uptime / 1000 != prev_time)
	//{
	//	prev_time = uptime / 1000;
	//	m_FPS = counter;
	//	counter = 0;
	//}
	//m_elapsedTime = (double)(uptime - prev_point) / 1000.0;
	//if (m_elapsedTime > 0.03)
	//	m_elapsedTime = 0.03;

	//if (m_elapsedTime < .005f)
	//{
	//	m_elapsedTime = -1;
	//}
	//else
	//{
	//	prev_point = uptime;
	//	counter++;
	//}
}
