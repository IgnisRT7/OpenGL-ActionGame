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
	*	@desc	�e�N�X�`��ID�̎擾
	*
	*	@return	�e�N�X�`��ID
	*/
	GLuint Id() const { return textureId; }

	/**
	*	�e�N�X�`���̕��̎擾
	*
	*	@return �e�N�X�`���̕��@
	*/
	GLint Width() const { return width; }

	/**
	*	�e�N�X�`���̍����̎擾
	*
	*	@return �e�N�X�`���̍���
	*/
	GLint Height() const { return height; }

	/**
	*	�e�N�X�`�����̎擾
	*
	*	@return �e�N�X�`����
	*/
	std::string Name() const { return name; }

public:

	GLuint textureId;	///< �e�N�X�`��ID
	GLint width = -1;	///< �e�N�X�`���̕�
	GLint height = -1;	///< �e�N�X�`���̍���

	std::string name;	///< �e�N�X�`����1
};

using TexturePtr = std::shared_ptr<Texture>;




#endif