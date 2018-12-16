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

	//float leftPos = (right * position.x + size / 2) / size;
	//float bottomPos = (top * 0.1f + size / 2) / size;
	float leftPos = position.x;
	float bottomPos = position.y;

	// Center the menu
	{
		if (position.x == -1)
			leftPos = right - ((float)width + 1.0f) / 2.0f * size;

		if (position.y == -1)
			bottomPos = top - ((float)height + 1.0f) / 2.0f * size;
	}

	leftPos += 0.5f * size;
	bottomPos += 0.5f * size;

	// Top + bottom rows
	for (int i = 1 * size; i < width * size; i += size)
	{
		GraphComp_ptr x = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_top.png"));
		x->SetPhysics(Vector3f(leftPos + i, bottomPos + height * size, -10.f));
		//std::cout << leftPos + i << std::endl;
		//std::cout << bottomPos + height * 64 << std::endl;
		x->SetColorAll(color, alpha);
		m_boxParts.push_back(x);

		GraphComp_ptr y = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_top.png"));
		y->SetPhysics(Vector3f(leftPos + i, bottomPos, -10.f));
		y->GetModelMat()->SetRotation(0, 0, 3.141592f);// 1.57 is somehow 90 degrees
		y->SetColorAll(color, alpha);
		m_boxParts.push_back(y);
	}

	// Left + right columns
	for (int i = 1 * size; i < height * size; i += size)
	{
		GraphComp_ptr x = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_top.png"));
		x->SetPhysics(Vector3f(leftPos, bottomPos + i, -10.f));
		x->GetModelMat()->SetRotation(0, 0, 1.57f);// 1.57 is somehow 90 degrees
		x->SetColorAll(color, alpha);
		m_boxParts.push_back(x);

		GraphComp_ptr y = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_top.png"));
		y->SetPhysics(Vector3f(leftPos + width * size, bottomPos + i, -10.f));
		y->GetModelMat()->SetRotation(0, 0, -1.57f);// 1.57 is somehow 90 degrees
		y->SetColorAll(color, alpha);
		m_boxParts.push_back(y);
	}

	// Center
	for (int w = 1 * size; w < width * size; w+=size)
	{
		for (int h = 1 * size; h < height * size; h+=size)
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
	topleft->SetPhysics(Vector3f(leftPos, bottomPos + height * size, -10.f));
	topleft->SetColorAll(color, alpha);
	m_boxParts.push_back(topleft);
	_topLeft = Vector3f(leftPos - 0.5f * size, bottomPos + height * size + 0.5f * size, -10);

	GraphComp_ptr topright = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	topright->SetPhysics(Vector3f(leftPos + width * size, bottomPos + height * size, -10.f));
	topright->GetModelMat()->SetRotation(0, 0, -1.57f);// 1.57 is somehow 90 degrees
	topright->SetColorAll(color, alpha);
	m_boxParts.push_back(topright);

	GraphComp_ptr bottomleft = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	bottomleft->SetPhysics(Vector3f(leftPos, bottomPos, -10.f));
	bottomleft->GetModelMat()->SetRotation(0, 0, 1.57f);// 1.57 is somehow 90 degrees
	bottomleft->SetColorAll(color, alpha);
	m_boxParts.push_back(bottomleft);

	GraphComp_ptr bottomright = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "menu_corner.png"));
	bottomright->SetPhysics(Vector3f(leftPos + width * size, bottomPos, -10.f));
	bottomright->GetModelMat()->SetRotation(0, 0, 3.141592f);// 1.57 is somehow 90 degrees
	bottomright->SetColorAll(color, alpha);
	m_boxParts.push_back(bottomright);
	_bottomRight = Vector3f(leftPos + width * size + 1 * size - 0.5f * size, bottomPos + 1 * size - 0.5f * size, -10.f);

	m_MBO_instances = 1;

	m_mesh.add_instanced_base_to_mesh_static_atlas("CENTERED_TILE");
	_instancedDraw = true;
	_instanced_tex_coord_draw = true;

	for (auto& x : m_boxParts)
	{
		m_mesh.add_tex_offset_static_atlas(x->GetTexture());
		//m_mesh.AddToMesh(*x->GetVertices(), x->GetIndices(), x->GetHighestIndex(), Vector3f(), x->GetTexture());
	}

	m_texture = "res/tiles.png";
	m_tex_ptr = ResourceManager::GetInstance().GetTexture(m_texture);
	SetStatic(true);

	m_vertices = std::vector<Vertex>(*m_mesh.GetMeshVertices());
	m_indices = std::vector<GLuint>(*m_mesh.GetMeshIndices());
	ClearMModels();
	Update();
	set_tex_coord_offsets(m_mesh.get_tex_coords());
	SetNewBuffers(&m_vertices, &m_indices);
	LoadGLResources();
}

void MenuBackgroundBox::Update()
{
	if (GetMModels().size() == 0 || OrthoProjInfo::GetRegularInstance().changed)
	{
		ClearMModels();
		for (auto& x : m_boxParts)
		{
			Vector3f pos = x->GetPosRef();
			Transformation t;
			t.SetTranslation(pos);
			t.SetScale(x->GetModelMat()->GetScale());
			t.SetRotation(x->GetModelMat()->GetRotation());
			InsertMModels(t);
		}
	}

	//FontGraphicsComponent::Update();
}