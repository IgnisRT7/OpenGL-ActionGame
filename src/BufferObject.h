/**
*	@file BufferObject.h
*/
#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>

class BufferObject {
public:

	BufferObject() = default;
	~BufferObject();
	BufferObject(const BufferObject&) = delete;
	const BufferObject& operator=(const BufferObject&) = delete;

	/**
	*	����������
	*
	*	@param name		�o�b�t�@��
	*	@param target	�^�[�Q�b�g(GL_ARRAY_BUFFER,GL_ELEMENT_ARRAY_BUFFER...)
	*	@param size		�o�b�t�@�T�C�Y
	*	@param data		�o�b�t�@�f�[�^�i�[��(Default = nullptr)
	*	@param usage	�o�b�t�@�̎g�p���@(Default = GL_STATIC_DRAW)
	*
	*	@retval true	����������
	*	@retval false	���������s
	*/
	bool Init(const char* name,GLenum target, GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);

	/**
	*	�I�u�W�F�N�gID�̎擾
	*
	*	@return �I�u�W�F�N�gID
	*/
	GLuint GetId() const { return id; }

	/**
	*	���̃o�b�t�@���L�����ۂ��𒲂ׂ�
	*
	*	@return true	�L��
	*	@return false	����
	*/
	bool isValid() const { return id != 0; }

private:

	GLuint id = 0;
	std::string name;
	GLenum target = GL_NONE;
	GLenum usage = GL_NONE;
	GLsizeiptr size = 0;
};

class VertexArrayObject {
public:

	VertexArrayObject() = default;
	~VertexArrayObject();
	VertexArrayObject(const VertexArrayObject&) = delete;
	const VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	/**
	*	����������
	*
	*	@param vbo	�o�C���h����VBO
	*	@param ibo	�o�C���h����IBO
	*
	*	@retval true	����������
	*	@retval false	���������s
	*/
	bool Init(GLuint vboId, GLuint iboId);

	/**
	*	�o�C���h����
	*
	*	@retval true	�o�C���h����
	*	@retval false	�o�C���h���s
	*/
	bool Bind();

	/**
	*	�A���o�C���h����
	*
	*	@param vaoiboUnBind	vbo��ibo���A���o�C���h���邩�ǂ���(Default = false)
	*/
	void UnBind(bool vaoiboUnBind = false);

	/**
	*	�A�g���r���[�g�̐ݒ菈��
	*
	*	@param index	�C���f�b�N�X�ԍ�
	*	@param size		�G�������g��
	*	@param type		�G�������g�̌^
	*	@param stride	1���_�f�[�^�̃T�C�Y(�\���̃T�C�Y)
	*	@param offset	���̃G�������g�̃I�t�Z�b�g
	*	@param normalize ���K���t���O(Default = GL_FALSE)
	*/
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLsizei stride, GLvoid* offset, GLboolean normalized = GL_FALSE);

	/**
	*	�I�u�W�F�N�gID�̎擾
	*
	*	@return �I�u�W�F�N�gID
	*/
	GLuint GetId() const { return id; }

	/**
	*	���̃o�b�t�@���L�����ۂ��𒲂ׂ�
	*
	*	@retval true	�L��
	*	@retval false	����
	*/
	bool isValid() const { return id != 0; }

private:

	GLuint id = 0;
};