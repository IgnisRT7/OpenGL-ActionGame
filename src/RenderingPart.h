/**
*	@file RenderingPart.h
*/
#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace Rendering {

	/**
	*	���_�f�[�^�\����
	*/
	struct Vertex {

		glm::vec3 position;	///< ���_���W
		glm::vec2 texcoord;	///< �e�N�X�`�����W
		glm::vec4 color;	///< ���_�F
	};

	const Vertex vertices[] = {

		//�E�C���h�E�S�̂̋�`
		{ { -1.0, -1.0, 0 },{ 0, 1 },{ 1, 0, 0, 1} },		// left - down
		{ { 1.0, -1.0, 0 },{ 1, 1 },{ 0, 1, 0, 1} },		// right - down
		{ { 1.0, 1.0, 0 },{ 1, 0 },{ 0, 0, 1, 1} },			// right - up
		{ { -1.0, 1.0, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },	// left - up

		//���C���p�̋�`
		{ { -1.0, -1.0, 0 },{ 0, 1 },{ 1, 0, 0, 1} },
		{ { 0.33, -1.0, 0 },{ 1, 1 },{ 0, 1, 0, 1} },
		{ { 0.33, 0.33, 0 },{ 1, 0 },{ 0, 0, 1, 1} },
		{ { -1.0, 0.33, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },

		//�T�u�p�̋�`
		{ { -1.0, 0.33, 0 },{ 0, 1 },{ 1, 0, 0, 1} },
		{ { -0.35, 0.33, 0 },{ 1, 1 },{ 0, 1, 0, 1} },
		{ { -0.35, 1.0, 0 },{ 1, 0 },{ 0, 0, 1, 1} },
		{ { -1.0, 1.0, 0 },{ 0, 0 },{ 0.8, 0.8, 0.8, 1} },
	};

	const GLuint indices[] = {
	0,1,2,2,3,0,
	4,5,6,6,7,4,
	8,9,10,10,11,8,
	};

	/**
	*	�`�悷���`�̎��
	*/
	enum RenderPartType {
		FULL_WINDOW = 0,		//�t���`���
		MAIN_PART,
		SUB_WORLDPOSITION,
		SUB_WORLDNORMAL,
		SUB_,
	
	};



	/**
	*	�����`��f�[�^
	*/
	struct RenderingPart {
		GLsizei size;	///< �`�悷��C���f�b�N�X��
		GLvoid* offset;	///< �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
	};

	/**
	*	RenderingPart���쐬����
	*
	*	@param size �`�悷��C���f�b�N�X��
	*	@param offset �`��J�n�C���f�b�N�X�̃I�t�Z�b�g(�C���f�b�N�X�P��)
	*
	*	@return �쐬���������`��I�u�W�F�N�g
	*/
	RenderingPart MakePart(GLsizei size, GLsizei offset) {
		return { size, reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)) };
	}

	RenderingPart parts[] = {
		MakePart(6, 0),
		MakePart(6, 6),
		MakePart(6, 12),
	};
}