/**
*	@file GameEngine.cpp
*/
#include "GameEngine.h"
#include "DebugLogger.h"
#include "RenderingPart.h"
#include "Resource.h"

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

		if (!offBuffer.Init(window.Width(), window.Height(), GL_RGBA)) {
			throw("offscreen buffer creation failed!!");
		}
	}
	catch (const char* errStr) {

		log.Log("Engine Initialization failed!!",DebugLogger::LogType::Error);
		log.Log((std::string("error log : ") + errStr).c_str(),DebugLogger::LogType::Error);

		return false;
	}
	
	return true;
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

	float aspect = 800.0f / 600.0f;
	glm::mat4 matView = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 matProj = glm::perspective(45.0f, aspect, 1.0f, 500.0f);
	glm::mat4 matVP = matProj * matView;
	matVP = glm::identity<glm::mat4>();

	//オフスクリーンバッファに描画
	glBindFramebuffer(GL_FRAMEBUFFER, offBuffer.GetFrameBuffer());

	glClearColor(0.1f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	auto prog_s = progOffBuffer.lock();
	prog_s->UseProgram();

	//サンプルテクスチャバインドのコード
	prog_s->BindTexture(GL_TEXTURE0, sampleTexture.lock()->Id());
	prog_s->SetViewProjectionMatrix(matVP);
	if (backBufferVao.Bind()) {

		glDrawElements(
			GL_TRIANGLES, Rendering::parts[0].size,
			GL_UNSIGNED_INT, Rendering::parts[0].offset);

		backBufferVao.UnBind();
	}
	
	//バックバッファに描画
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 0, 1);
	
	auto progBack_s = progBackRender.lock();
	progBack_s->UseProgram();

	//オフスクリーンバッファのテクスチャをバインド
	progBack_s->BindTexture(GL_TEXTURE0, offBuffer.GetTexture());
	progBack_s->SetViewProjectionMatrix(glm::identity<glm::mat4>());
	if (backBufferVao.Bind()) {

		glDrawElements(
			GL_TRIANGLES, Rendering::parts[0].size,
			GL_UNSIGNED_INT, Rendering::parts[0].offset);

		backBufferVao.UnBind();
	}

	glUseProgram(0);

	window.SwapBuffers();
}

GameEngine::~GameEngine() {

	DebugLogger::LogBuffer::Instance().Log("Fainalized GameEngine");
}
