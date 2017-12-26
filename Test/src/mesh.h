#ifndef MESH_H__
#define MESH_H__

#include <sys/stat.h>
#include <iostream>
#include <string>
#include <math.h>
#include "mathutils.h"
#include "texture.h"
#include "model.h"
#include "math.h"
#include "array2d.h"
#include <vector>
#include <map>
#include "textureatlas.h"
#include "resource_manager.h"

class Mesh
{
public:
	Mesh(unsigned int atlasSize = 8);
	~Mesh();
	void AddToMesh(std::vector<Vertex>& verts, std::vector<GLuint>& inds, int biggestIndex, Vector3f pos, std::string tex, int index = -1);
	void Reset();
	void Finalize(std::string name);
	std::vector<Vertex>* GetMeshVertices();
	std::vector<GLuint>* GetMeshIndices();
	//std::vector<Transformation> GetMeshModels() { return m_models; };
	TextureAtlas* GetAtlas() { return &m_texAtlas; }
private:
	GLuint m_IBO;
	GLuint m_VBO;
	std::vector<GLuint> m_indices;
	//std::vector<Transformation> m_models;
	int m_indexProgress;
	TextureAtlas m_texAtlas;
	Texture* m_texture;
	int m_progess;
	std::map<std::string, unsigned int> m_textures;
	std::vector<Vertex> m_vertexList;
};

#endif