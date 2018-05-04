#include "menuBackgroundBox.h"

MenuBackgroundBox::MenuBackgroundBox(Vector3f position, Vector3f sizeWH) : FontGraphicsComponent()
{
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;
	float size = OrthoProjInfo::GetRegularInstance().Size;

	GraphComp_ptr topleft = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	topleft->SetPhysics(Vector3f((right * 0.2f + size / 2) / size, (top * 1.9f - size / 2) / size, -10.f));
	topleft->SetColorAll(Vector3f(0.12f, 0.3f, 0.8f), 1.0f);
	m_boxParts.push_back(topleft);

	GraphComp_ptr topright = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	topright->SetPhysics(Vector3f((right * 1.8f - size / 2) / size, (top * 1.9f - size / 2) / size, -10.f));
	topright->GetModelMat()->SetRotation(0, 0, -1.57f);// 1.57 is somehow 90 degrees
	topright->SetColorAll(Vector3f(0.12f, 0.3f, 0.8f), 1.0f);
	m_boxParts.push_back(topright);

	GraphComp_ptr topBG = GraphComp_ptr(new FontGraphicsComponent("TILE", "dialogue.png"));
	dynamic_cast<FontGraphicsComponent*>(topBG.get())->SetStatic(true);
	topBG->SetPhysics(Vector3f(right * 0.2f / size, top * 0.1f / size, -10.f));
	topBG->GetModelMat()->SetScale(right * 1.6f / size, top * 1.8f / size, 1);
	topBG->SetColorAll(Vector3f(1, 1, 0.5f), 1.2f);
	//topBG->Update();
	//topBG->ApplyModelToVertices();
	m_boxParts.push_back(topBG);

	m_MBO_instances = 1;

	for (auto& x : m_boxParts)
		m_mesh.AddToMesh(*x->GetVertices(), x->GetIndices(), x->GetHighestIndex(), Vector3f(), x->GetTexture());
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
			t.SetRotation(x->GetModelMat()->GetRotation());
			InsertMModels(t);
		}
	}

	FontGraphicsComponent::Update();
}