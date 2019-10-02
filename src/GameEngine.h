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
	*	�G���W���̏���������
	*
	*	@param windowSize	�E�C���h�E�T�C�Y
	*	@param title		�^�C�g����
	*/
	bool Init(glm::vec2 windowsize, std::string title);

	/**
	*	�G���W���̋N������
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

	glm::vec2 windowSize;				///< �E�C���h�E�T�C�Y
	VertexArrayObject backBufferVao;	///< �o�b�N�o�b�t�@��VAO
};