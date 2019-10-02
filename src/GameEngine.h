/**
*	@file GameEngine.h
*/

#pragma once
#include "GLSystem.h"
#include "BufferObject.h"
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

	void Render();

private:

	GameEngine() = default;
	~GameEngine() {}
	GameEngine(const GameEngine&) = delete;
	const GameEngine& operator=(const GameEngine&) = delete;

private:

	glm::vec2 windowSize;				///< ウインドウサイズ
	VertexArrayObject backBufferVao;	///< バックバッファのVAO
};