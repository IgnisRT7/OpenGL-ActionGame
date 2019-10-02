#include "GameEngine.h"

/**
*	頂点データ構造体
*/
struct Vertex {

	glm::vec3 position;	///< 頂点座標
	glm::vec2 texcoord;	///< テクスチャ座標
	glm::vec4 color;	///< 頂点色
};

Vertex vertices[] = {

	{ { -0.5, -0.5, 0 },{ 0, 1 },{ 1, 0, 0, 1} },
	{ { 0.5, -0.5, 0 },{ 1, 1 },{ 0, 1, 0, 1} },
	{ { 0.5, 0.5, 0 },{ 1, 0 },{ 0, 0, 1, 1} },
	{ { -0.5, 0.5, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },
};

GLuint indices[] = {
	0,1,2,2,3,0
};

bool GameEngine::Init(glm::vec2 windowSize,std::string title){

	GLSystem::Window& window = GLSystem::Window::Instance();

	//システムの最深部の初期化
	if (window.Init(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), title.c_str())) {
		return false;
	}
	
	//バックバッファ用 ibo,vbo 作成
	BufferObject vbo, ibo;

	vbo.Init("VertexBuffer", GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	ibo.Init("IndexBuffer", GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
	backBufferVao.Init(vbo.GetId(), ibo.GetId());

	if (!vbo.isValid() || !ibo.isValid()) {
		return false;
	}

	if (backBufferVao.Bind()) {
		backBufferVao.VertexAttribPointer(0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
		backBufferVao.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texcoord)));
		backBufferVao.VertexAttribPointer(2, 4, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));
		backBufferVao.UnBind(true);
	}
	else {
		return false;
	}





	return true;
}
