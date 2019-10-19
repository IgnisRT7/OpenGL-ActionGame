/**
*	@file OffscreenBuffer.cpp
*/
#include "OffscreenBuffer.h"
#include "Texture.h"
#include <GL/glew.h>
#include <iostream>

OffscreenBuffer::~OffscreenBuffer()
{
}

bool OffscreenBuffer::Init(int width, int height, GLenum iformat){

	//GLuint offBuffer,frameBuffer,depthBuffer;

	GLenum texFormat = GL_RGBA;
	switch (iformat) {
	default:
		texFormat = GL_RGBA;
		break;

	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		texFormat = GL_DEPTH_COMPONENT;
	}

	offTexture = Texture::Buffer::Create(width, height, iformat, texFormat, nullptr);

	if (iformat == GL_DEPTH_COMPONENT) {

		//�[�x�o�b�t�@�`�掞�̓V�F�[�_���Ő؂�̂ď������s���̂�
		//�t���[���o�b�t�@�݂̂Ŗ��Ȃ�
		glGenFramebuffers(1, &offScreenID);
		glBindFramebuffer(GL_FRAMEBUFFER, offScreenID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, offTexture->Id(), 0);
		glDrawBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {

		//�[�x�o�b�t�@�̍쐬
		glGenRenderbuffers(1, &offDepthID);
		glBindRenderbuffer(GL_RENDERBUFFER, offDepthID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 

		//�t���[���o�b�t�@�̍쐬 
		glGenFramebuffers(1, &offScreenID);
		glBindFramebuffer(GL_FRAMEBUFFER, offScreenID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, offDepthID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offTexture->Id(),0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[Error]: Framebuffer generate faild!" << std::endl;
	}

	return true;
}

