#include "GameEngine.h"

/**
*	���_�f�[�^�\����
*/
struct Vertex {

	glm::vec3 position;	///< ���_���W
	glm::vec2 texcoord;	///< �e�N�X�`�����W
	glm::vec4 color;	///< ���_�F
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

	//�V�X�e���̍Ő[���̏�����
	if (window.Init(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), title.c_str())) {
		return false;
	}
	
	//�o�b�N�o�b�t�@�p ibo,vbo �쐬
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
