/**
*	@file Mesh.h
*/
#pragma once

#include "BufferObject.h"

#include <memory>
#include <map>
#include <string>
#include <glm/glm.hpp>

namespace Mesh {

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};

	struct Material {
		glm::vec4 baseColor;
	};

	/**
	*	スタティックメッシュ制御クラス
	*/
	class StaticMesh {
	public:

		StaticMesh() = default;
		virtual ~StaticMesh() = default;

	private:

		

	};

	using StaticMeshPtr = std::weak_ptr<StaticMesh>;
	using StaticMeshPtr_s = std::shared_ptr<StaticMesh>;

	/**
	*	スタティックメッシュ管理バッファ
	*/
	class Buffer {
		friend class StaticMesh;
	public:

		/**
		*	作成処理
		*/
		static StaticMeshPtr Create();

	private:

		Buffer() = default;
		~Buffer() = default;
		Buffer(const Buffer&) = delete;
		const Buffer& operator=(const Buffer&) = delete;

	private:

		BufferObject vbo, ibo;	///< メッシュ頂点データ格納用
		GLintptr iboOffset;		///< 追加可能なインデックスバッファのメモリオフセット
		GLintptr vboOffset;		///< 追加可能な頂点バッファのメモリオフセット

		std::map<std::string, StaticMeshPtr_s> meshBuffer;	///< メッシュデータのバッファ
	};



}