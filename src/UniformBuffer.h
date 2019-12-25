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
	*	���j�t�@�[�u�o�b�t�@�N���X
	*
	*	�V�F�[�_�Ƃ̃o�C���f�B���O�͂��Ă��Ȃ��̂ŊO���Őݒ肷��K�v������܂�
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
		*	�]���ʒu�̐ݒ菈��
		*
		*	@param program	�K�p����v���O����
		*	@param name		���蓖�Đ�
		*
		*	@retval true	����
		*	@retval false	���s
		*/
		void BindBufferRange(GLintptr offset, GLintptr size);

		/**
		*	�o�b�t�@�Ƀf�[�^��]������
		*
		*	@param data		�]������f�[�^
		*	@param offset	�]������f�[�^�ʒu�I�t�Z�b�g
		*	@param size		�]������f�[�^�T�C�Y
		*/
		bool BufferSubData(const GLvoid* data, GLintptr offset = 0, GLsizeiptr size = 0);

		/**
		*	�o�b�t�@�T�C�Y�̎擾
		*
		*	@return �o�b�t�@�T�C�Y
		*/
		GLsizeiptr Size() const { return size; }

		/**
		*	�o�C���f�B���O�|�C���g�̎擾
		*
		*	@return �o�C���f�B���O�|�C���g
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
		*	���j�t�@�[���o�b�t�@�̍쐬����
		*
		*	@param size		�m�ۂ���o�b�t�@�T�C�Y
		*	@param bindingPoint	���蓖�Ă�o�C���f�B���O�|�C���g
		*	@param name		�o�b�t�@��*
		*
		*	@return ���j�t�@�[���o�b�t�@�̃��\�[�X
		*/
		static UniformBufferObjectPtr Create(GLsizeiptr size, GLuint bindingPoint, const char* name);

	private:

		std::unordered_map<std::string, UniformBufferObjectPtr_s> uboMap;
	};
}