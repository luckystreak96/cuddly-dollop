#ifndef MODEL_H__
#define MODEL_H__

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "utils.h"
#include "vertex.h"
#include <algorithm>//remove_if
#include <memory>

class Model
{
    public:
		static Model& GetInstance()
		{
			static Model instance;
			return instance;
		}
        void init(std::string filename);
        void loadModel(std::string modelname);
        void PrintLastModel();
        std::vector<float> getVertices();
        std::vector<GLuint> getIndices();
        std::vector<float> GetTex();
		std::string GetName();
    private:
        Model();
        std::string m_lastName;
        std::vector<float> m_lastVertices;
        std::vector<GLuint> m_lastIndices;
        std::vector<float> m_lastTex;

        std::string m_modelList;
};

#endif
