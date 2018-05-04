#include "menuBackgroundBox.h"

MenuBackgroundBox::MenuBackgroundBox(Vector3f position, Vector3f sizeWH) : FontGraphicsComponent()
{
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;
	float size = OrthoProjInfo::GetRegularInstance().Size;

	GraphComp_ptr topBG = GraphComp_ptr(new FontGraphicsComponent("TILE", "dialogue.png"));
	dynamic_cast<FontGraphicsComponent*>(topBG.get())->SetStatic(true);
	topBG->SetPhysics(Vector3f(right * 0.2f / size, top * 0.1f / size, 0.5f));
	topBG->GetModelMat()->SetScale(right * 1.6f / size, top * 1.8f / size, 1);
	topBG->SetColorAll(Vector3f(1, 1, 0.5f), 1.1f);
	topBG->Update();
	//topBG->ApplyModelToVertices();
	m_boxParts.push_back(topBG);

	m_MBO_instances = 1;

	m_mesh.AddToMesh(*topBG->GetVertices(), topBG->GetIndices(), topBG->GetHighestIndex(), Vector3f(), topBG->GetTexture());
	//m_mesh.Finalize("res/tiles.png");

	m_texture = "res/tiles.png";

	//m_modelMat.SetScale(Vector3f(10));
	SetStatic(true);

	//FullReset(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices());

	m_vertices = std::vector<Vertex>(*m_mesh.GetMeshVertices());
	m_indices = std::vector<GLuint>(*m_mesh.GetMeshIndices());
	ClearMModels();
	Update();
	LoadGLResources();
}

void MenuBackgroundBox::Update()
{
	if (GetMModels().size() == 0)
	{
		ClearMModels();
		for (auto x : m_boxParts)
		{
			Vector3f pos = x->GetPosRef();
			Transformation t;
			t.SetTranslation(pos);
			t.SetScale(x->GetModelMat()->GetScale());
			InsertMModels(t);
		}
	}

	FontGraphicsComponent::Update();
}