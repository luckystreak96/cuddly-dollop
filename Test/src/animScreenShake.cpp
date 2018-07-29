#include "animScreenShake.h"
#include "mathutils.h"

AnimScreenShake::AnimScreenShake(double intensity, double duration)
{
	_duration = duration;
	_intensity = intensity;
	_done = false;
	_progress = 0;
	_async = true;
	m_progress = 0;
}

void AnimScreenShake::Update()
{
	// Dont update if done
	if (_done)
		return;

	float x = pow((-1), m_progress) * (1 / (pow(m_progress, 2) - 5 * m_progress + 20));

	Camera::_currentCam->_extraTranslate.x = x;

	// Update percent
	m_progress++;
	_progress += 0.025;
	if (_progress >= _duration)
	{
		_done = true;
		Camera::_currentCam->_extraTranslate.x = 0;
	}
}
