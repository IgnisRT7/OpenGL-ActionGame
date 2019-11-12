#include "UniformBuffer.h"
#include "DebugLogger.h"

namespace UniformBuffer {

	bool UniformBuffer::UniformBlock::MapBuffer() {


		return false;
	}

	UniformBlockPtr Create(GLsizeiptr size, GLuint bindingPoint, const char* name){

		DebugLogger::LogBuffer& inst = DebugLogger::LogBuffer::Instance();
		inst.Log("Creating UniformBuffer. name: " );

		struct Impl : UniformBlock {};
		auto p = std::make_shared<UniformBlock>();

		if (!p) {
			return {};
		}

		GLuint ubo;
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			return {};
		}

		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return p;
	}

}