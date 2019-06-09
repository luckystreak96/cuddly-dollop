#include "model.h"

Model::Model()
{
}

void Model::init(std::string filename)
{
	m_modelList = Utils::ReadFile(filename, false);
}

void Model::PrintLastModel()
{
	std::cout << m_lastVertices[1] << std::endl;
}

void Model::loadModel(std::string model_name)
{
	//Don't repeat this if the correct model is already loaded
	if (m_lastName == model_name)
		return;

	//std::string result;
	//for (char c : m_modelList)
	//{
	//	if (c != ' ' && c != '\n')
	//		result.push_back(c);
	//}
	//m_modelList = result;

	std::stringstream ss(m_modelList);

	std::string model;//one line of text
	bool differs = false;//checks letter for letter if the names are the same

	//Read every line
	while (std::getline(ss, model, '#'))
	{
		if (model.length() < 2)
			continue;
		differs = false;

		model = std::string(Utils::RemoveWhiteSpace(model));

		//Does this model hold the desired name?
		for (int i = 0; i < model_name.length(); i++)
		{
			if (model_name[i] != model[i])
			{
				differs = true;//wrong model
				break;
			}

		}

		//Names are same length?
		if (!differs && model[model_name.length()] == '|')
		{
			m_lastName = model_name;

			//Find the vertices start point and indices start point
			int vertStart = 0;
			int indStart = 0;
			int texStart = 0;
			for (int i = 0; i < model.length(); i++)
			{
				if (model[i] == '|')
				{
					if (vertStart == 0)
					{
						vertStart = i + 1;
					}
					else if (indStart == 0)
					{
						indStart = i + 1;
					}
					else if (texStart == 0)
					{
						texStart = i + 1;
						break;
					}
				}
			}

			std::stringstream stream(model.substr(vertStart, indStart - vertStart - 2));
			std::string currentFloat;
			m_lastVertices.clear();
			m_lastVertexVertices.clear();
			m_lastIndices.clear();
			m_lastTex.clear();

			while (std::getline(stream, currentFloat, ','))
			{
				m_lastVertices.push_back(::atof(currentFloat.c_str()));//convert string to float and add
			}

			stream = std::stringstream(model.substr(indStart, texStart - indStart));

			while (std::getline(stream, currentFloat, ','))
			{
				m_lastIndices.push_back(::atoi(currentFloat.c_str()));//convert string to float and add
			}

			stream = std::stringstream(model.substr(texStart, model.length() - texStart));

			while (std::getline(stream, currentFloat, ','))
			{
				m_lastTex.push_back(::atof(currentFloat.c_str()));//convert string to float and add
			}

			return;
		}
	}
}

std::string Model::GetName()
{
	return m_lastName;
}

std::vector<float> Model::getVertices()
{
	return m_lastVertices;
}

std::vector<Vertex> Model::getVertexVertices()
{
	if (m_lastVertexVertices.size() != 0)
		return m_lastVertexVertices;

	for (int x = 0; x < m_lastVertices.size() / 3; x++)
	{
		m_lastVertexVertices.push_back(
			Vertex(Vector3f(m_lastVertices[x * 3], m_lastVertices[x * 3 + 1], m_lastVertices[x * 3 + 2]),
				Vector2f(x * 2 < m_lastTex.size() ? m_lastTex[x * 2] : 0, x * 2 + 1 < m_lastTex.size() ? m_lastTex[x * 2 + 1] : 0)));//TEXCOORD MOD HERE PLZ
	}
	return m_lastVertexVertices;
}

std::vector<GLuint> Model::getIndices()
{
	return m_lastIndices;
}

std::vector<float> Model::GetTex()
{
	return m_lastTex;
}

