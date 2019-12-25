/**
*	@file Uniform.h
*/
#pragma once

#include "Shader.h"

#include <memory>
#include <GL/glew.h>
#include <unordered_map>

namespace UniformBuffer {

	class UniformBufferObject;
	using UniformBufferObjectPtr_s = std::shared_ptr<UniformBufferObject>;
	using UniformBufferObjectPtr = std::weak_ptr<UniformBufferObject>;

	/**
	*	ユニファーブバッファクラス
	*
	*	シェーダとのバインディングはしていないので外部で設定する必要があります
	*
	*/
	class UniformBufferObject {
		friend class Buffer;
	public:

		UniformBufferObject() = default;
		~UniformBufferObject();
		UniformBufferObject(const UniformBufferObject&) = delete;
		const UniformBufferObject& operator=(const UniformBufferObject&) = delete;

		/**
		*	転送位置の設定処理
		*
		*	@param program	適用するプログラム
		*	@param name		割り当て先
		*
		*	@retval true	成功
		*	@retval false	失敗
		*/
		void BindBufferRange(GLintptr offset, GLintptr size);

		/**
		*	バッファにデータを転送する
		*
		*	@param data		転送するデータ
		*	@param offset	転送するデータ位置オフセット
		*	@param size		転送するデータサイズ
		*/
		bool BufferSubData(const GLvoid* data, GLintptr offset = 0, GLsizeiptr size = 0);

		/**
		*	バッファサイズの取得
		*
		*	@return バッファサイズ
		*/
		GLsizeiptr Size() const { return size; }

		/**
		*	バインディングポイントの取得
		*
		*	@return バインディングポイント
		*/
		GLuint BingingPoint()const { return bindingPoint; }

	private:

		GLuint uboId;			
		GLsizeiptr size;
		GLuint bindingPoint;
		const char* name;
	
	};




	class Buffer {
	public:
		
		/**
		*	ユニファームバッファの作成処理
		*
		*	@param size		確保するバッファサイズ
		*	@param bindingPoint	割り当てるバインディングポイント
		*	@param name		バッファ名*
		*
		*	@return ユニファームバッファのリソース
		*/
		static UniformBufferObjectPtr Create(GLsizeiptr size, GLuint bindingPoint, const char* name);

	private:

		std::unordered_map<std::string, UniformBufferObjectPtr_s> uboMap;
	};
}