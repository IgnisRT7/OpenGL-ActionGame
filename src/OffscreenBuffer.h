/**
*	@file OffscreenBuffer.h
*/
#include "Texture.h"
#include <GL/glew.h>

#pragma once

class OffscreenBuffer {
public:

	OffscreenBuffer() = default;
	~OffscreenBuffer();
	OffscreenBuffer(const OffscreenBuffer&) = delete;
	const OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;

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
	GLuint GetTexture() const { return offTexture->Id(); }

	/**
	*	�I�t�X�N���[���o�b�t�@�̎擾
	*
	*	@return �I�t�X�N���[���o�b�t�@��ID
	*/
	GLuint GetFrameBuffer() const { return offScreenID; }
	

private:

	std::shared_ptr<Texture::Image2D> offTexture;	///< �I�t�X�N���[�������_�����O�p�e�N�X�`��
	GLuint offScreenID;				///< �I�t�X�N���[���o�b�t�@�pID
	GLuint offDepthID;				///< �f�v�X�o�b�t�@�pID


};