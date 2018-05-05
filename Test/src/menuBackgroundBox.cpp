#include "menuBackgroundBox.h"

MenuBackgroundBox::MenuBackgroundBox(Vector3f position, int width, int height) : FontGraphicsComponent()
{
	float alpha = 0.95f;
	Vector3f color = Vector3f(0.12f, 0.3f, 0.8f);
	width--;
	height--;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;
	float size = OrthoProjInfo::GetRegularInstance().Size;

	float leftPos = (right * position.x + size / 2) / size;
	float bottomPos = (top * 0.1f + size / 2) / size;

	// Center the menu
	if (position == -1)
	{
		leftPos = right / size - ((float)width + 1.0f) / 2.0f;
		bottomPos = top / size - ((float)height + 1.0f) / 2.0f;
		leftPos += 0.5f;
		bottomPos += 0.5f;
	}

	// Top + bottom rows
	for (int i = 1; i < width; i++)
	{
		GraphComp_ptr x = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_top.png"));
		x->SetPhysics(Vector3f(leftPos + i, bottomPos + height, -10.f));
		x->SetColorAll(color, alpha);
		m_boxParts.push_back(x);

		GraphComp_ptr y = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_top.png"));
		y->SetPhysics(Vector3f(leftPos + i, bottomPos, -10.f));
		y->GetModelMat()->SetRotation(0, 0, 3.141592f);// 1.57 is somehow 90 degrees
		y->SetColorAll(color, alpha);
		m_boxParts.push_back(y);
	}

	// Left + right columns
	for (int i = 1; i < height; i++)
	{
		GraphComp_ptr x = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_top.png"));
		x->SetPhysics(Vector3f(leftPos, bottomPos + i, -10.f));
		x->GetModelMat()->SetRotation(0, 0, 1.57f);// 1.57 is somehow 90 degrees
		x->SetColorAll(color, alpha);
		m_boxParts.push_back(x);

		GraphComp_ptr y = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_top.png"));
		y->SetPhysics(Vector3f(leftPos + width, bottomPos + i, -10.f));
		y->GetModelMat()->SetRotation(0, 0, -1.57f);// 1.57 is somehow 90 degrees
		y->SetColorAll(color, alpha);
		m_boxParts.push_back(y);
	}

	// Center
	for (int w = 1; w < width; w++)
	{
		for (int h = 1; h < height; h++)
		{
			GraphComp_ptr x = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_center.png"));
			x->SetPhysics(Vector3f(leftPos + w, bottomPos + h, -10.f));
			x->SetColorAll(color, alpha);
			m_boxParts.push_back(x);

			GraphComp_ptr y = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_center.png"));
			y->SetPhysics(Vector3f(leftPos + w, bottomPos + h, -10.f));
			y->SetColorAll(color, alpha);
			m_boxParts.push_back(y);
		}
	}

	// Corners
	GraphComp_ptr topleft = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	topleft->SetPhysics(Vector3f(leftPos, bottomPos + height, -10.f));
	topleft->SetColorAll(color, alpha);
	m_boxParts.push_back(topleft);

	GraphComp_ptr topright = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	topright->SetPhysics(Vector3f(leftPos + width, bottomPos + height, -10.f));
	topright->GetModelMat()->SetRotation(0, 0, -1.57f);// 1.57 is somehow 90 degrees
	topright->SetColorAll(color, alpha);
	m_boxParts.push_back(topright);

	GraphComp_ptr bottomleft = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	bottomleft->SetPhysics(Vector3f(leftPos, bottomPos, -10.f));
	bottomleft->GetModelMat()->SetRotation(0, 0, 1.57f);// 1.57 is somehow 90 degrees
	bottomleft->SetColorAll(color, alpha);
	m_boxParts.push_back(bottomleft);

	GraphComp_ptr bottomright = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	bottomright->SetPhysics(Vector3f(leftPos + width, bottomPos, -10.f));
	bottomright->GetModelMat()->SetRotation(0, 0, 3.141592f);// 1.57 is somehow 90 degrees
	bottomright->SetColorAll(color, alpha);
	m_boxParts.push_back(bottomright);

	m_MBO_instances = 1;

	for (auto& x : m_boxParts)
		m_mesh.AddToMesh(*x->GetVertices(), x->GetIndices(), x->GetHighestIndex(), Vector3f(), x->GetTexture());

	m_texture = "res/tiles.png";
	SetStatic(true);

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