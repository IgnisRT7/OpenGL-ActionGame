/**
*	@file OffscreenBuffer.cpp
*/
#include "OffscreenBuffer.h"
#include "Texture.h"
#include "DebugLogger.h"
#include <GL/glew.h>
#include <iostream>

OffscreenBuffer::~OffscreenBuffer(){

	//DebugLogger::Log("Offscreenbuffer finalized");

}

OffscreenBufferPtr OffscreenBuffer::Create(int width, int height, GLenum iformat){

	DebugLogger::Log("OffscreenBuffer::Create() ", DebugLogger::Infomation, false);

	struct Impl : OffscreenBuffer{};
	auto p = std::make_shared<Impl>();
	if (!p) {

		DebugLogger::Log("creation faild!", DebugLogger::Error);
		return {};
	}

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

	p->offTexture = Texture::Buffer::Create(width, height, iformat, texFormat, nullptr);

	if (iformat == GL_DEPTH_COMPONENT) {

		//深度バッファ描画時はシェーダ内で切り捨て処理を行うので
		//フレームバッファのみで問題ない
		glGenFramebuffers(1, &p->offScreenID);
		glBindFramebuffer(GL_FRAMEBUFFER, p->offScreenID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, p->offTexture.lock()->Id(), 0);
		glDrawBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {

		//深度バッファの作成
		glGenRenderbuffers(1, &p->offDepthID);
		glBindRenderbuffer(GL_RENDERBUFFER, p->offDepthID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//フレームバッファの作成 
		glGenFramebuffers(1, &p->offScreenID);
		glBindFramebuffer(GL_FRAMEBUFFER, p->offScreenID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, p->offDepthID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, p->offTexture.lock()->Id(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		DebugLogger::Log("Framebuffer generate faild!", DebugLogger::Error);
	}
	
	DebugLogger::Log("completed.");
	return p;
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

		//深度バッファ描画時はシェーダ内で切り捨て処理を行うので
		//フレームバッファのみで問題ない
		glGenFramebuffers(1, &offScreenID);
		glBindFramebuffer(GL_FRAMEBUFFER, offScreenID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, offTexture.lock()->Id(), 0);
		glDrawBuffer(GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else {

		//深度バッファの作成
		glGenRenderbuffers(1, &offDepthID);
		glBindRenderbuffer(GL_RENDERBUFFER, offDepthID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 

		//フレームバッファの作成 
		glGenFramebuffers(1, &offScreenID);
		glBindFramebuffer(GL_FRAMEBUFFER, offScreenID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, offDepthID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offTexture.lock()->Id(),0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "[Error]: Framebuffer generate faild!" << std::endl;
	}

	return true;
}

