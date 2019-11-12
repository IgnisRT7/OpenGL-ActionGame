/**
*	@file Uniform.h
*/
#pragma once
#include <memory>
#include <GL/glew.h>

namespace UniformBuffer {

	class UniformBlock {
	public:



		bool MapBuffer();
		void UnMapBuffer();

	private:

		GLuint ubo;
	};

	using UniformBlockPtr = std::shared_ptr<UniformBlock>;

	/**
	*	���j�t�@�[���o�b�t�@�쐬����
	*/
	UniformBlockPtr Create(GLsizeiptr size, GLuint bindingPoint, const char* name);

}
