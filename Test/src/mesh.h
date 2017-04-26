#ifndef MESH_H__
#define MESH_H__

#include <iostream>
#include <string>
#include <math.h>
#include "mathutils.h"
#include "texture.h"
#include "model.h"
#include "math.h"
#include "entity.h"
#include "array2d.h"
#include "define.h"
#include "queue.h"
#include "drawable.h"
#include <vector>
#include <map>
#include "textureatlas.h"
#include "resource_manager.h"

class Mesh
{
public:
	Mesh();
	~Mesh();
	void AddToMesh(std::vector<Vertex>& verts, std::vector<GLuint>& inds, int biggestIndex, Vector3f pos, std::string tex);
	void Reset();
	void Finalize(std::string name);
	std::vector<Vertex> GetMeshVertices();
	std::vector<GLuint> GetMeshIndices();
private:
	GLuint m_IBO;
	GLuint m_VBO;
	std::vector<GLuint> m_indices;
	int m_indexProgress;
	TextureAtlas m_texAtlas;
	Texture* m_texture;
	int m_progess;
	std::map<std::string, unsigned int> m_textures;
	std::vector<Vertex> m_vertexList;
};

#endif