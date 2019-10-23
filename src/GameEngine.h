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

	/**
	*	�`�揈��
	*/
	void Render();

private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	const GameEngine& operator=(const GameEngine&) = delete;

private:

	glm::vec2 windowSize;				///< �E�C���h�E�T�C�Y
	VertexArrayObject backBufferVao;	///< �o�b�N�o�b�t�@��VAO

	Shader::ProgramPtr progOffBuffer,	///< �I�t�X�N���[���o�b�t�@�`��p�V�F�[�_
		progBackRender,					///< �o�b�N�o�b�t�@�`��p�V�F�[�_
		progFontRenderer;					///< �t�H���g�`��p�V�F�[�_

	OffscreenBuffer offBuffer;			///< �I�t�X�N���[���o�b�t�@

	Texture::Image2DPtr sampleTexture;	///< �T���v���p�e�N�X�`��(�f�o�b�O�p)
};