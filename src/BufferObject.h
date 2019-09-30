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
	*	初期化処理
	*
	*	@param name		バッファ名
	*	@param target	ターゲット(GL_ARRAY_BUFFER,GL_ELEMENT_ARRAY_BUFFER...)
	*	@param size		バッファサイズ
	*	@param data		バッファデータ格納元(Default = nullptr)
	*	@param usage	バッファの使用方法(Default = GL_STATIC_DRAW)
	*
	*	@retval true	初期化成功
	*	@retval false	初期化失敗
	*/
	bool Init(const char* name,GLenum target, GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);

	/**
	*	バッファにデータを転送する
	*
	*	@param offset	転送開始位置(バイト単位)
	*	@param size		転送するバイト数
	*	@param data		転送するデータへのポインタ
	*
	*	@retval true	転送成功
	*	@retval false	転送失敗
	*/
	bool BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) {

		if (offset + size > this->size) {
			std::cout << "[Warning]: Insufficient destination buffer size" << std::endl;
			return false;
		}

		glBindBuffer(target, id);
		glBufferSubData(target, offset, size, data);
		glBindBuffer(target, 0);

		const GLenum error = glGetError();

		if (error != GL_NO_ERROR) {
			std::cout << "[Error]: Data transfer failed!!" << std::endl;
			return false;
		}
		return true;
	}


	/**
	*	オブジェクトIDの取得
	*
	*	@return オブジェクトID
	*/
	GLuint GetId() const { return id; }

	/**
	*	このバッファが有効か否かを調べる
	*
	*	@return true	有効
	*	@return false	無効
	*/
	bool isValid() const { return id != 0; }

	/**
	*	バッファサイズの取得
	*
	*	@return バッファサイズ
	*/	
	size_t Size() const { return static_cast<size_t>(size); }

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
	*	初期化処理
	*
	*	@param vbo	バインドするVBO
	*	@param ibo	バインドするIBO
	*
	*	@retval true	初期化成功
	*	@retval false	初期化失敗
	*/
	bool Init(GLuint vboId, GLuint iboId);

	/**
	*	バインド処理
	*
	*	@retval true	バインド成功
	*	@retval false	バインド失敗
	*/
	bool Bind();

	/**
	*	アンバインド処理
	*
	*	@param vaoiboUnBind	vboとiboをアンバインドするかどうか(Default = false)
	*/
	void UnBind(bool vaoiboUnBind = false);

	/**
	*	アトリビュートの設定処理
	*
	*	@param index	インデックス番号
	*	@param size		エレメント数
	*	@param type		エレメントの型
	*	@param stride	1頂点データのサイズ(構造体サイズ)
	*	@param offset	このエレメントのオフセット
	*	@param normalize 正規化フラグ(Default = GL_FALSE)
	*/
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLsizei stride, GLvoid* offset, GLboolean normalized = GL_FALSE);

	/**
	*	オブジェクトIDの取得
	*
	*	@return オブジェクトID
	*/
	GLuint GetId() const { return id; }

	/**
	*	このバッファが有効か否かを調べる
	*
	*	@retval true	有効
	*	@retval false	無効
	*/
	bool isValid() const { return id != 0; }

private:

	GLuint id = 0;
};