/**
*	@file OffscreenBuffer.h
*/
#include "Texture.h"
#include <GL/glew.h>

#pragma once

class OffscreenBuffer;
using OffscreenBufferPtr = std::shared_ptr<OffscreenBuffer>;

class OffscreenBuffer {
public:

	/**
	*	�쐬����
	*/
	static OffscreenBufferPtr Create(int width, int height, GLenum format);

	/**
	*	����������
	*
	*	@param width	��
	*	@param height	����
	*	@param iformat	�g�p����t�H�[�}�b�g
	*/
	bool Init(int width, int height, GLenum iformat);

	/**
	*	�e�N�X�`��ID�̎擾
	*
	*	@return �e�N�X�`����ID
	*/
	GLuint GetTexture() const { return offTexture.lock()->Id(); }

	/**
	*	�I�t�X�N���[���o�b�t�@�̎擾
	*
	*	@return �I�t�X�N���[���o�b�t�@��ID
	*/
	GLuint GetFrameBuffer() const { return offScreenID; }
	
	~OffscreenBuffer();

private:

	OffscreenBuffer() = default;

	OffscreenBuffer(const OffscreenBuffer&) = delete;
	const OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;

private:

	Texture::Image2DPtr offTexture;	///< �I�t�X�N���[�������_�����O�p�e�N�X�`��
	GLuint offScreenID = -1;				///< �I�t�X�N���[���o�b�t�@�pID
	GLuint offDepthID = -1;					///< �f�v�X�o�b�t�@�pID


};

