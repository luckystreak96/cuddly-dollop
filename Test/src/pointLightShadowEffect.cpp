#include "pointLightShadowEffect.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define INVALID_UNIFORM_LOCATION 0xffffffff
#ifdef WIN32
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#if (_MSC_VER == 1900)
#elif (_MSC_VER == 1800)
#else
#endif
#else
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

PointLightShadowEffect::PointLightShadowEffect()
{
	Init();
	Enable();
	m_pointLights = new PointLight();
	m_pointLights->AmbientIntensity = 0.1f;
	m_pointLights->Color = Vector3f(1.f, 0.8f, 0.8f);
	m_pointLights->DiffuseIntensity = 0.1f;
	m_pointLights->Position = Vector3f(3, 2, -5);
	m_pointLights->Attenuation.Constant = 0.0f;
	m_pointLights->Attenuation.Exp = 0.01f;
	m_pointLights->Attenuation.Linear = 0.01f;
	SetPointLights(1, m_pointLights);
	DirectionalLight light = DirectionalLight();
	light.Color = Vector3f(1.0f, 1.0f, 1.0f);
	light.AmbientIntensity = 0.0f;
	light.DiffuseIntensity = 0.5f;
	light.Direction = Vector3f(0.5f, -0.5, -0.5);
	glUniform3f(m_dirLightLocation.Color, light.Color.x, light.Color.y, light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, light.AmbientIntensity);
	Vector3f Direction = light.Direction;
	Direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, light.DiffuseIntensity);
}

bool PointLightShadowEffect::Init()
{
	const char* vs = "shaders/pointLight.vs";
	const char* fs = "shaders/pointLight.fs";

	AddShader(vs, GL_VERTEX_SHADER);
	AddShader(fs, GL_FRAGMENT_SHADER);

	bool isFinalized = Finalize();
	assert(isFinalized == true);

	Enable();

	m_samplerLocation = GetUniformLocation("gColorMap");
	m_shadowMapLocation = GetUniformLocation("gShadowMap");
	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	m_numPointLightsLocation = GetUniformLocation("gNumPointLights");

	for (unsigned int i = 0; i < 1; i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		SNPRINTF(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	return true;
}

Vector3f PointLightShadowEffect::GetPointLightLocation(int index)
{
	return m_pointLights[index].Position;
}

void PointLightShadowEffect::Move(Mat4f* mat)
{
	static float pos = 0.0f;
	pos += 0.005f;
	SetPointLights(1, m_pointLights);
	Vector3f LULPos = Vector3f();
	LULPos = m_pointLights->Position;
	glLineWidth(4.0f);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(LULPos.x, LULPos.y, LULPos.z - 15);
	glVertex3f(LULPos.x, LULPos.y, LULPos.z);
	glEnd();
}

void PointLightShadowEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void PointLightShadowEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void PointLightShadowEffect::SetEyeWorldPos(const Vector3f& EyeWorldPos)
{
	glUniform3f(GetUniformLocation("gEyeWorldPos"), EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void PointLightShadowEffect::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
	glUniform1i(m_numPointLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void PointLightShadowEffect::SetLightWVP(const Mat4f& LightWVP)
{
	glUniformMatrix4fv(m_LightWVPLocation, 1, GL_TRUE, (const GLfloat*)LightWVP.m);
}

void PointLightShadowEffect::SetTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_textureLocation, TextureUnit);
}

void PointLightShadowEffect::SetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_samplerLocation, TextureUnit);
}

void PointLightShadowEffect::SetShadowMapTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_shadowMapLocation, TextureUnit);
}