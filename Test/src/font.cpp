//#include "font.h"
//
//Font::Font(std::string path)
//{
//	int bitmapWidth = 16;
//	m_width = 0.5f;
//	m_height = 0.5f;
//
//	CreateHash();
//	ResourceManager::GetInstance().LoadTexture(path);
//	for (int i = 0; i < Math.pow((texS / texU), 2); i++) {//256*256 = bitmap texture size
//
//		float col = i / bitmapWidth;
//		float row = i % bitmapWidth;
//
//		float u0 = row / bitmapWidth;
//		float u1 = (row + 1) / bitmapWidth;
//
//		float v0 = col / bitmapWidth;
//		float v1 = (col + 1) / bitmapWidth;
//
//		float z = 0.2f;
//		float[] vertices = new float[] {//changes between different textures
//			-m_width / 2.0f, -m_height / 2.0f, z,//last value is the z value??
//				-m_width / 2.0f, m_height / 2.0f, z,
//				m_width / 2.0f, m_height / 2.0f, z,
//				m_width / 2.0f, -m_height / 2.0f, z
//		};
//
//		byte[] indices = new byte[]{//first triangle uses points 0, 1 and 2, second uses 2, 3 and 0; DOESNT CHANGE BETWEEN DIFF TEXTURES
//			0, 1, 2,
//			2, 3, 0
//		};
//
//		float[] tcs = new float[] {//tcs --> texturecoordinates; DOESNT CHANGE BETWEEN DIFF TEXTURES
//			u0, v1,
//				u0, v0,
//				u1, v0,
//				u1, v1
//		};
//
//		medwices, tcs);
//	}
//}
//
//void Font::CreateHash() {
//	std::string charList = 
//		"\n !\"#$%&'()*+,-."
//		"/0123456789:;<=>"
//		"?@ABCDEFGHIJKLMN"
//		"OPQRSTUVWXYZ[\\]^"
//		"_`abcdefghijklmn"
//		"opqrstuvwxyz{|}~";
//	for (int i = 0; i < charList.length(); i++) 
//	{
//		m_letters.emplace(charList.at(i), i);
//	}
//}
//
//private void setString(String text, float x, float y, boolean centered) {
//	message = new VertexArray[text.length()];
//	for (int i = 0; i < text.length(); i++) {
//		message[i] = mesh[charToCode(text.charAt(i))];
//	}
//	if (centered) {
//		baseX = x - text.length() * m_width / 2 / 2;//the spacing is too much, so halve the width again to remove the spacing
//	}
//	else {
//		baseX = x;
//	}
//	baseY = y;
//}
//
//
///*
//public void setString(String text, float x, float y){
//setString(text, x, y, false);
//}
//*/
//
//public void drawString(String text, float x, float y, boolean centered) {
//	setString(text, x, y, centered);
//	position.z = 3.0f;
//	render();
//}
//
//public void drawString(String text, float x, float y, float z, boolean centered) {
//	position.z = z;
//	setString(text, x, y, centered);
//	render();
//}
//
//public void drawString(String text, float x, float y) {
//	drawString(text, x, y, false);
//}
//
//private int charToCode(char c) {
//	return hash.get(c);
//}
//
//public void update() {
//}
//
//public void render() {
//	Shader.FONT.enable();
//	position.x = baseX;
//	position.y = baseY;
//	for (VertexArray letter : message) {
//		//Shader.FONT.setUniformMat4f("ml_matrix", Matrix4f.translate(position));
//		Shader.FONT.setUniformMat4f("vw_matrix", Matrix4f.translate(position));
//		texture.bind();
//		position.x += (m_width / 2);//the spacing is too much, so halve the width again to remove the spacing
//		if (letter == mesh[0]) {
//			position.y -= height;
//			position.x = baseX;
//		}
//		else
//			letter.render();
//	}
//
//	Shader.FONT.disable();
//}
//
//
