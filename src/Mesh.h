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
	*	�X�^�e�B�b�N���b�V������N���X
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
	*	�X�^�e�B�b�N���b�V���Ǘ��o�b�t�@
	*/
	class Buffer {
		friend class StaticMesh;
	public:

		/**
		*	�쐬����
		*/
		static StaticMeshPtr Create();

	private:

		Buffer() = default;
		~Buffer() = default;
		Buffer(const Buffer&) = delete;
		const Buffer& operator=(const Buffer&) = delete;

	private:

		BufferObject vbo, ibo;	///< ���b�V�����_�f�[�^�i�[�p
		GLintptr iboOffset;		///< �ǉ��\�ȃC���f�b�N�X�o�b�t�@�̃������I�t�Z�b�g
		GLintptr vboOffset;		///< �ǉ��\�Ȓ��_�o�b�t�@�̃������I�t�Z�b�g

		std::map<std::string, StaticMeshPtr_s> meshBuffer;	///< ���b�V���f�[�^�̃o�b�t�@
	};



}