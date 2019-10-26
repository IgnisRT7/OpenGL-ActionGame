/**
*	@file GameEngine.cpp
*/
#include "GameEngine.h"
#include "DebugLogger.h"


/**
*	頂点データ構造体
*/
struct Vertex {

	glm::vec3 position;	///< 頂点座標
	glm::vec2 texcoord;	///< テクスチャ座標
	glm::vec4 color;	///< 頂点色
};

const Vertex vertices[] = {

	//ウインドウ全体の矩形
	{ { -1.0, -1.0, 0 },{ 0, 1 },{ 1, 0, 0, 1} },		// left - down
	{ { 1.0, -1.0, 0 },{ 1, 1 },{ 0, 1, 0, 1} },		// right - down
	{ { 1.0, 1.0, 0 },{ 1, 0 },{ 0, 0, 1, 1} },			// right - up
	{ { -1.0, 1.0, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },	// left - up

	//メイン用の矩形
	{ { -1.0, -1.0, 0 },{ 0, 1 },{ 1, 0, 0, 1} },
	{ { 0.33, -1.0, 0 },{ 1, 1 },{ 0, 1, 0, 1} },
	{ { 0.33, 0.33, 0 },{ 1, 0 },{ 0, 0, 1, 1} },
	{ { -1.0, 0.33, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },

	//サブ用の矩形
	{ { -1.0, 0.33, 0 },{ 0, 1 },{ 1, 0, 0, 1} },
	{ { 0.66, -1.0, 0 },{ 1, 1 },{ 0, 1, 0, 1} },
	{ { 1.0, 1.0, 0 },{ 1, 0 },{ 0, 0, 1, 1} },
	{ { -1.0, 1.0, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },
};

const GLuint indices[] = {
	0,1,2,2,3,0,
	2,3,4,4,2,3,
	5,6,7,7,5,6,
};

/**
*	部分描画データ
*/
struct RenderingPart {
	GLsizei size;	///< 描画するインデックス数
	GLvoid* offset;	///< 描画開始インデックスのバイトオフセット
};

/**
*	RenderingPartを作成する
*
*	@param size 描画するインデックス数
*	@param offset 描画開始インデックスのオフセット(インデックス単位)
*
*	@return 作成した部分描画オブジェクト
*/
constexpr RenderingPart MakeRenderingPart(GLsizei size, GLsizei offset) {
	return { size,reinterpret_cast<GLvoid*>(sizeof(offset) * sizeof(GLuint)) };
}

/**
*	部分描画データリスト
*/
static const RenderingPart renderingParts[] = {
	MakeRenderingPart(6,0),
	MakeRenderingPart(6,6),
};

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

		vbo.Init("VertexBuffer", GL_ARRAY_BUFFER, sizeof(vertices), vertices);
		ibo.Init("IndexBuffer", GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
		backBufferVao.Init(vbo.GetId(), ibo.GetId());

		if (!vbo.isValid() || !ibo.isValid()) {
			throw("Main vbo & ibo creation failed!!");
		}

		if (backBufferVao.Bind()) {
			backBufferVao.VertexAttribPointer(0, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
			backBufferVao.VertexAttribPointer(1, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texcoord)));
			backBufferVao.VertexAttribPointer(2, 4, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));
			backBufferVao.UnBind(true);
		}
		else {
			throw("Main vao creation failed!!");
		}

		/// -- ここからデバッグ用コード --
		sampleTexture = Texture::Buffer::Instance().LoadFromFile("res/texture/sampleTex.dds");
		if (sampleTexture.expired()) {
			throw("SampleTexture creation failed!!");
		}

		progBackRender = Shader::Buffer::Create("res/shader/FinalRender.vert", "res/shader/FinalRender.frag");
		progOffBuffer = Shader::Buffer::Create("res/shader/Default.vert", "res/shader/Default.frag");
		progFontRenderer = Shader::Buffer::Create("res/shader/FontRenderer.vert", "res/shader/FontRenderer.frag");

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

		//各ノード更新処理

		//各コリジョンデータ更新処理

		//各行列データ転送処理

		Render();
	}
}

void GameEngine::Render(){

	GLSystem::Window& window = GLSystem::Window::Instance();

	float aspect = 800.0f / 600.0f;
	glm::mat4 matView = glm::lookAt(glm::vec3(0, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 matProj = glm::perspective(45.0f, aspect, 1.0f, 500.0f);
	glm::mat4 matVP = matProj * matView;

	//オフスクリーンバッファに描画
	glBindFramebuffer(GL_FRAMEBUFFER, offBuffer.GetFrameBuffer());

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	auto prog_s = progOffBuffer.lock();
	prog_s->UseProgram();
	prog_s->BindTexture(GL_TEXTURE0, sampleTexture.lock()->Id());
	prog_s->SetViewProjectionMatrix(matVP);
	if (backBufferVao.Bind()) {

		glDrawElements(
			GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
			GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

		backBufferVao.UnBind();
	}

	//バックバッファに描画
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 1, 1, 1);
	
	auto progBack_s = progBackRender.lock();
	progBack_s->UseProgram();
	progBack_s->BindTexture(GL_TEXTURE0, offBuffer.GetTexture());
	progBack_s->SetViewProjectionMatrix(glm::identity<glm::mat4>());
	if (backBufferVao.Bind()) {

		glDrawElements(
			GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
			GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));

		backBufferVao.UnBind();
	}

	glUseProgram(0);

	window.SwapBuffers();
}

GameEngine::~GameEngine() {

	DebugLogger::LogBuffer::Instance().Log("Fainalized GameEngine");
}
