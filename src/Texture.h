/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <GL/glew.h>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <vector>

class Texture {
private:

	Texture() = default;
	Texture(const Texture&) = delete;
	const Texture operator=(const Texture&) = delete;
	~Texture() = default;

	/**
	*	@desc	テクスチャIDの取得
	*
	*	@return	テクスチャID
	*/
	GLuint Id() const { return textureId; }

	/**
	*	テクスチャの幅の取得
	*
	*	@return テクスチャの幅　
	*/
	GLint Width() const { return width; }

	/**
	*	テクスチャの高さの取得
	*
	*	@return テクスチャの高さ
	*/
	GLint Height() const { return height; }

	/**
	*	テクスチャ名の取得
	*
	*	@return テクスチャ名
	*/
	std::string Name() const { return name; }

public:

	GLuint textureId;	///< テクスチャID
	GLint width = -1;	///< テクスチャの幅
	GLint height = -1;	///< テクスチャの高さ

	std::string name;	///< テクスチャ名1
};

using TexturePtr = std::shared_ptr<Texture>;




#endif