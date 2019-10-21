/**
*	@file	main.cpp
*	@author	Takuya Yokoyama
*/

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "GameEngine.h"
#include "GLSystem.h"
#include "BufferObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Font.h"
#include "OffscreenBuffer.h"

/**
*	頂点データ構造体
*
struct Vertex {

	glm::vec3 position;	///< 頂点座標
	glm::vec2 texcoord;	///< テクスチャ座標
	glm::vec4 color;	///< 頂点色
};

static int GBufferCount = 4;

Vertex vertices[] = {

	//全面描画用データ
	{ { -0.5, -0.5, 0 },{ 0, 1 },{ 1, 0, 0, 1} },
	{ { 0.5, -0.5, 0 },{ 1, 1 },{ 0, 1, 0, 1} },
	{ { 0.5, 0.5, 0 },{ 1, 0 },{ 0, 0, 1, 1} },
	{ { -0.5, 0.5, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },
	
	//オフスクリーンバッファ同時描画用データ(メイン)
	{ { -1.0, -1.0, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
	{ {  0.5, -1.0, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
	{ {  0.5,  0.5, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
	{ { -1.0,  0.5, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },

	//オフスクリーンバッファ同時描画用データ(サブ)
	{ { -1.0, -1.0, 0 }, { 0, 1 }, { 1, 1, 1, 1 } },
	{ { -0.5, -1.0, 0 }, { 1, 1 }, { 1, 1, 1, 1 } },
	{ { -0.5, -0.5, 0 }, { 1, 0 }, { 1, 1, 1, 1 } },
	{ { -1.0, -0.5, 0 }, { 0, 0 }, { 1, 1, 1, 1 } },

};

GLuint indices[] = {

	//全面描画用データ
	0,1,2,2,3,0,

	4,5,6,6,7,4,

	8,9,10,10,11,8,
};*/


int main(){

	GameEngine& engine = GameEngine::Instance();;

	if (engine.Init(glm::vec2(2048, 1060), "title")){
		return -1;
	}

	engine.Run();

	return -1;

	/*
	GLSystem::Window& window = GLSystem::Window::Instance();
	if (!window.Init(1000,800,"OpenGLActionGame")) {
		return -1;
	}

	//最終出力用バッファ初期化・設定

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
	else {
		return -1;
	}

	// フォント初期化

	auto fontPtr= Font::Buffer::Instance().CreateFontFromFile("res/font/Font.fnt");
	if (!fontPtr.lock()->IsValid()) {
		return -1;
	}
	
	Font::Renderer fontRenderer;
	fontRenderer.Init(1000, glm::ivec2(1000, 800));

	// テスト出力用テクスチャ

	Texture::Image2DPtr texture = Texture::Buffer::Instance().LoadFromFile("res/texture/sampleTex.dds");
	if (texture.expired()) {
		return -1;
	}

	// シェーダプログラム作成

	Shader::ProgramPtr prog = Shader::Buffer::Create("res/shader/Default.vert", "res/shader/Default.frag");
	Shader::ProgramPtr progBack = Shader::Buffer::Create("res/shader/FinalRender.vert", "res/shader/FinalRender.frag");
	Shader::ProgramPtr fontProg = Shader::Buffer::Create("res/shader/FontRenderer.vert", "res/shader/FontRenderer.frag");
	if (prog.expired() || fontProg.expired() || prog.expired()) {
		return -1;
	}
	auto err = glGetError();
	OffscreenBuffer offScreen;
	offScreen.Init(window.Width(), window.Height(),GL_RGBA);

	err = glGetError();

	//fontPtr.lock()->Shader(fontProg);
	

	float aspect = 800.0f / 600.0f;
	glm::mat4 matView = glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 matProj = glm::perspective(45.0f, aspect, 1.0f, 500.0f);
	glm::mat4 matVP = matProj * matView;

	auto error = glGetError();

	//main loop
	while (!window.ShouldClose()) {

		//オフスクリーンバッファに描画
		glBindFramebuffer(GL_FRAMEBUFFER,offScreen.GetFrameBuffer());

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		auto prog_s = prog.lock();
		prog_s->UseProgram();
		prog_s->BindTexture(GL_TEXTURE0, texture.lock()->Id());
		prog_s->SetViewProjectionMatrix(matVP);
		if (vao.Bind()) {

			glDrawElements(
				GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
				GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

			vao.UnBind();
		}

		//バックバッファに描画
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		auto progBack_s = progBack.lock();
		progBack_s->UseProgram();
		progBack_s->BindTexture(GL_TEXTURE0, offScreen.GetTexture());
		if (vao.Bind()) {

			glDrawElements(
				GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
				GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

			vao.UnBind();
		}
	
		glUseProgram(0);

		window.SwapBuffers();
	}*/
}



