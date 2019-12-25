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
	*	作成処理
	*/
	static OffscreenBufferPtr Create(int width, int height, GLenum format);

	/**
	*	初期化処理
	*
	*	@param width	幅
	*	@param height	高さ
	*	@param iformat	使用するフォーマット
	*/
	bool Init(int width, int height, GLenum iformat);

	/**
	*	テクスチャIDの取得
	*
	*	@return テクスチャのID
	*/
	GLuint GetTexture() const { return offTexture.lock()->Id(); }

	/**
	*	オフスクリーンバッファの取得
	*
	*	@return オフスクリーンバッファのID
	*/
	GLuint GetFrameBuffer() const { return offScreenID; }
	
	~OffscreenBuffer();

private:

	OffscreenBuffer() = default;

	OffscreenBuffer(const OffscreenBuffer&) = delete;
	const OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;

private:

	Texture::Image2DPtr offTexture;	///< オフスクリーンレンダリング用テクスチャ
	GLuint offScreenID = -1;				///< オフスクリーンバッファ用ID
	GLuint offDepthID = -1;					///< デプスバッファ用ID


};

