/**
*	@file	main.cpp
*	@author	Takuya Yokoyama
*/

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "GLSystem.h"
#include "BufferObject.h"
#include "Shader.h"
#include "Texture.h"

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


int main(){

	GLSystem::Window& window = GLSystem::Window::Instance();
	if (!window.Init(1000,800,"OpenGLActionGame")) {
		return -1;
	}

	auto i = glGetError();

	BufferObject vbo, ibo;
	VertexArrayObject vao;
	vbo.Init("VertexBuffer", GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	ibo.Init("IndexBuffer", GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
	vao.Init(vbo.GetId(), ibo.GetId());

	if (!vbo.isValid() || !ibo.isValid() || !vao.isValid()) {
		return -1;
	}

	if (vao.Bind()) {
		vao.VertexAttribPointer(0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
		vao.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texcoord)));
		vao.VertexAttribPointer(2, 4, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));
		vao.UnBind(true);
	}

	TexturePtr texture = Texture::LoadFromFile("res/texture/sampleTex.dds");
	if (!texture) {
		return -1;
	}

	Shader::ProgramPtr prog = Shader::Program::Create("res/shader/Default.vert", "res/shader/Default.frag");
	if (!prog->isValid()) {
		return -1;
	}

	//main loop
	while (!window.ShouldClose()) {

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		prog->UseProgram();
		prog->BindTexture(GL_TEXTURE0, texture->Id(), GL_TEXTURE_2D);
		if (vao.Bind()) {

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
			glDrawElements(
				GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
				GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

			vao.UnBind();
		}

		window.SwapBuffers();
	}
}



