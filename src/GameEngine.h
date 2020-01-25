/**
*	@file GameEngine.h
*/

#pragma once
#include "GLSystem.h"
#include "BufferObject.h"
#include "OffscreenBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <string>
#include <functional>

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
	*	�`��X�e�[�g�̏���������
	*/
	void InitRenderState();

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

	glm::vec2 windowSize;				///< �E�C���h�E�T�C�Y(�}�X�^�[�̉𑜓x)
	VertexArrayObject backBufferVao;	///< �o�b�N�o�b�t�@��VAO

	Shader::ProgramPtr progOffBuffer,	///< �I�t�X�N���[���o�b�t�@�`��p�V�F�[�_
		progBackRender,					///< �o�b�N�o�b�t�@�`��p�V�F�[�_
		progFontRenderer;					///< �t�H���g�`��p�V�F�[�_

	OffscreenBufferPtr offBuffer;			///< �I�t�X�N���[���o�b�t�@

	Mesh::Buffer meshBuffer;
	Mesh::FilePtr planeTest;			///< ���ʃ|���S�����b�V��(�f�o�b�O�p)

	Texture::Image2DPtr sampleTexture;	///< �T���v���p�e�N�X�`��(�f�o�b�O�p)

	Shader::ProgramPtr progTest;

	BufferObject vboTest, iboTest;
	VertexArrayObject vaoTest;

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};
	const std::vector<Vertex> vertices = {
	{ {-1,-1, 0 }, { 0, 0 }, { 0, 0, 1 }},	//����
	{ { 1,-1, 0 }, { 1, 0 }, { 0, 0, 1 }},	//�E��
	{ { 1, 1, 0 }, { 1, 1 }, { 0, 0, 1 }},	//�E��
	{ {-1, 1, 0 }, { 0, 1 }, { 0, 0, 1 }}, };	//����
	const std::vector<GLubyte> indices = { 0,1,2,2,3,0 };

};