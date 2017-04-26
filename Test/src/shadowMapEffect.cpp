/*

Copyright 2011 Etay Meiri

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "shadowMapEffect.h"


ShadowMapEffect::ShadowMapEffect()
{
	Init();

}

bool ShadowMapEffect::Init()
{
	const char* vs = "shaders/shadow_map.vs";
	const char* fs = "shaders/shadow_map.fs";

	AddShader(vs, GL_VERTEX_SHADER);
	AddShader(fs, GL_FRAGMENT_SHADER);

	if (!Finalize()) {
		return false;
	}

	m_ModelLocation = GetUniformLocation("gModel");
	m_WorldMatrixLocation = GetUniformLocation("gWorld");
	m_lightWorldPosLoc = GetUniformLocation("gLightWorldPos");

	return true;
}


void ShadowMapEffect::SetModel(const Mat4f& WVP)
{
	glUniformMatrix4fv(m_ModelLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}


void ShadowMapEffect::SetWorld(const Mat4f& World)
{
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)World.m);
}

void ShadowMapEffect::SetLightWorldPos(const Vector3f& Pos)
{
	glUniform3f(m_lightWorldPosLoc, Pos.x, Pos.y, Pos.z);
}