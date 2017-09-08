#ifndef HEIGHT_FOG_H_
#define HEIGHT_FOG_H_

#include "FBO.h"
#include "basicEffect.h"
//#include "bloomEffect.h"
#include "heightEffect.h"
#include "combineEffect.h"
#include "blurEffect.h"
#include "post_processing_screen.h"

class HeightFog {
public:
	HeightFog();

	void Begin();
	void End(Vector3f playerPos);
	void ResetTextureSizes();

private:
	//Decides how much to blur
	const float m_divisor = 2.0f;

	int m_width;
	int m_height;
	FBO m_fbo;
	FBO m_fog;
	FBO m_gaussV;
	FBO m_gaussH;
	Post_Processing_Screen pps;
};

#endif