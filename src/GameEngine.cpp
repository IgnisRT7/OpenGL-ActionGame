/**
*	@file GameEngine.cpp
*/
#include "GameEngine.h"
#include "DebugLogger.h"
#include "RenderingPart.h"
#include "Resource.h"
#include "glm/gtc/matrix_transform.hpp"

GameEngine& GameEngine::Instance(){

	static GameEngine instance;
	return instance;
}

bool GameEngine::Init(glm::vec2 windowSize,std::string title){

	GLSystem::Window& window = GLSystem::Window::Instance();

	auto& log = DebugLogger::LogBuffer::Instance();

	log.Log("initializing GameEngine...");

	try {

		//システムの最深部の初期化
		if (!window.Init(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), title.c_str())) {
			throw("GLsystem initialization failed!!");
		}
		this->windowSize = windowSize;

		//バックバッファ用 ibo,vbo 作成
		BufferObject vbo, ibo;

		vbo.Init("VertexBuffer", GL_ARRAY_BUFFER, sizeof(Rendering::vertices), Rendering::vertices);
		ibo.Init("IndexBuffer", GL_ELEMENT_ARRAY_BUFFER, sizeof(Rendering::indices), Rendering::indices);
		backBufferVao.Init(vbo.GetId(), ibo.GetId());

		if (!vbo.isValid() || !ibo.isValid()) {
			throw("Main vbo & ibo creation failed!!");
		}

		if (backBufferVao.Bind()) {
			backBufferVao.VertexAttribPointer(0, 3, GL_FLOAT, sizeof(Rendering::Vertex), reinterpret_cast<GLvoid*>(offsetof(Rendering::Vertex, position)));
			backBufferVao.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(Rendering::Vertex), reinterpret_cast<GLvoid*>(offsetof(Rendering::Vertex, texcoord)));
			backBufferVao.VertexAttribPointer(2, 4, GL_FLOAT, sizeof(Rendering::Vertex), reinterpret_cast<GLvoid*>(offsetof(Rendering::Vertex, color)));
			backBufferVao.UnBind(true);
		}
		else {
			throw("Main vao creation failed!!");
		}

		/// -- ここからデバッグ用コード --
		sampleTexture = Texture::Buffer::Instance().LoadFromFile(TextureFile::testDebugTexture);
		if (sampleTexture.expired()) {
			throw("SampleTexture creation failed!!");
		}

		progBackRender = Shader::Buffer::Create(ShaderFile::finalRender_vert, ShaderFile::finalRender_frag);
		progOffBuffer = Shader::Buffer::Create(ShaderFile::default_vert, ShaderFile::default_frag);
		progFontRenderer = Shader::Buffer::Create(ShaderFile::fontRenderer_vert, ShaderFile::fontRenderer_frag);

		if (progBackRender.expired() || progOffBuffer.expired() || progFontRenderer.expired()) {
			throw("shader compile failed!!");
		}

		offBuffer = OffscreenBuffer::Create(window.Width(), window.Height(), GL_RGBA);
		if (!offBuffer) {
			throw("offscreen buffer creation failed!!");
		}

		meshBuffer.Init(1'000'000 * sizeof(Mesh::Vertex), 1'000'000 * sizeof(GLushort));
		planeTest = meshBuffer.AddPlane("ParticlePlane");
		meshBuffer.LoadMesh(MeshFile::treeModel);

		//デバッグ用頂点データの初期化
		progStaticMesh = Shader::Buffer::Create(ShaderFile::staticMesh_vert, ShaderFile::staticMesh_frag);
		vboTest.Init("testVBO", GL_ARRAY_BUFFER, 10000 * sizeof(Vertex), reinterpret_cast<const GLvoid*>(this->vertices.data()), GL_STATIC_DRAW);
		iboTest.Init("testIBO", GL_ELEMENT_ARRAY_BUFFER, 10000 * sizeof(GLushort), reinterpret_cast<const GLvoid*>(this->indices.data()), GL_STATIC_DRAW);
		vaoTest.Init(vboTest.GetId(), iboTest.GetId());
		if (vaoTest.Bind()) {
			vaoTest.VertexAttribPointer(0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
			vaoTest.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texCoord)));
			vaoTest.VertexAttribPointer(2, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
			vaoTest.UnBind(true);
		}
	
		//meshBuffer.LoadMesh(MeshFile::treeModel);
		fontRenderer.Init(1000, windowSize);
		fontNormal = Font::Buffer::Instance().CreateFontFromFile("res/Font/Font.fnt");
		fontNormal.lock()->Shader(Shader::Buffer::Create("res/shader/FontRenderer.vert", "res/shader/FontRenderer.frag"));
	}
	catch (const char* errStr) {

		log.Log("Engine Initialization failed!!",DebugLogger::LogType::Error);
		log.Log((std::string("error log : ") + errStr).c_str(),DebugLogger::LogType::Error);

		return false;
	}

	log.ClearInfo();
	
	return true;
}

void GameEngine::InitRenderState() {



}

void GameEngine::Run(){

	GLSystem::Window& window = GLSystem::Window::Instance();

	while (!window.ShouldClose()) {

		Input::Update();

		//各ノード更新処理
		//Update();

		//各コリジョンデータ更新処理

		//各行列データ転送処理
		
		Render();

	}
}

void GameEngine::Render(){

	GLSystem::Window& window = GLSystem::Window::Instance();

	static float timer = 0;
	timer += 1 / 60.0f;

	float aspect = windowSize.x / windowSize.y;
	glm::mat4 matView = glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 matProj = glm::perspective(45.0f, aspect, 1.0f, 1000.0f);
	glm::mat4 matVP = matProj * matView;
//	matVP = glm::identity<glm::mat4>();

	glm::mat4 matModel = glm::identity<glm::mat4>();
	matModel = glm::rotate(glm::mat4(), timer * 0.01f, glm::vec3(0, 1, 0));
	matModel = glm::identity<glm::mat4>();
	matModel = glm::scale(glm::mat4(1), glm::vec3(1));

	//オフスクリーンバッファに描画
	glBindFramebuffer(GL_FRAMEBUFFER, offBuffer->GetFrameBuffer());
	//const auto texMa0in = offBuffer->GetTexture();
	glViewport(0, 0, offBuffer->GetOffscreenWidth(), offBuffer->GetOffscreenHeight());
	glClearColor(0.5f, 0.6f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);

#if false

	auto prog_s = progOffBuffer.lock();
	prog_s->UseProgram();

	//サンプルテクスチャバインドのコード
	prog_s->BindTexture(GL_TEXTURE0, sampleTexture.lock()->Id());
	prog_s->SetViewProjectionMatrix(matVP);
	prog_s->SetModelMatrix(matModel);
	if (backBufferVao.Bind()) {

		glDrawElements(
			GL_TRIANGLES, Rendering::parts[0].size,
			GL_UNSIGNED_INT, Rendering::parts[0].offset);

		backBufferVao.UnBind();
	}


#else

	meshBuffer.SetViewProjectionMatrix(matVP);
	Mesh::Draw(meshBuffer.GetFile("Cube"), glm::identity<glm::mat4>());
	Mesh::Draw(meshBuffer.GetFile("red_pine_tree"), glm::identity<glm::mat4>());

#endif
	//Mesh::Draw(this->planeTest, glm::rotate(glm::mat4(), timer * 0.3f, glm::vec3(0, 1, 0)));
	
	//バックバッファに描画
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowSize.x, windowSize.y);

	glDisable(GL_DEPTH_TEST);
	
	auto progBack_s = progBackRender.lock();
	progBack_s->UseProgram();

	//オフスクリーンバッファのテクスチャをバインド
	progBack_s->BindTexture(GL_TEXTURE0, offBuffer->GetTexture());
	progBack_s->SetViewProjectionMatrix(glm::identity<glm::mat4>());
	if (backBufferVao.Bind()) {

		glDrawElements(
			GL_TRIANGLES, Rendering::parts[1].size,
			GL_UNSIGNED_INT, Rendering::parts[1].offset);

		backBufferVao.UnBind();
	}

	glUseProgram(0);

	fontRenderer.MapBuffer();
	fontRenderer.AddString(glm::vec3(0), L"デバッグ用テキストfkajehrea", fontNormal);
	fontRenderer.UnMapBuffer();
	fontRenderer.Draw();

	window.SwapBuffers();
}

GameEngine::~GameEngine() {

//	DebugLogger::LogBuffer::Instance().Log("Fainalized GameEngine");
}
