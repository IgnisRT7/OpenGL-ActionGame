/**
*	@file GameEngine.h
*/

#pragma once
#include "GLSystem.h"
#include "BufferObject.h"
#include "OffscreenBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <string>

class GameEngine {
public:

	static GameEngine& Instance();

	/**
	*	エンジンの初期化処理
	*
	*	@param windowSize	ウインドウサイズ
	*	@param title		タイトル名
	*/
	bool Init(glm::vec2 windowsize, std::string title);

	/**
	*	エンジンの起動処理
	*/
	void Run();

	void Update();

	void CollisionUpdate();

	/**
	*	描画処理
	*/
	void Render();

private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	const GameEngine& operator=(const GameEngine&) = delete;

private:

	glm::vec2 windowSize;				///< ウインドウサイズ
	VertexArrayObject backBufferVao;	///< バックバッファのVAO

	Shader::ProgramPtr progOffBuffer,	///< オフスクリーンバッファ描画用シェーダ
		progBackRender,					///< バックバッファ描画用シェーダ
		progFontRenderer;					///< フォント描画用シェーダ

	OffscreenBuffer offBuffer;			///< オフスクリーンバッファ

	Texture::Image2DPtr sampleTexture;	///< サンプル用テクスチャ(デバッグ用)
};