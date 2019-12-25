#include "UniformBuffer.h"
#include "DebugLogger.h"

namespace UniformBuffer {

	UniformBufferObjectPtr Buffer::Create(GLsizeiptr size, GLuint bindingPoint, const char* name) {

		struct Impl : UniformBufferObject { Impl() {} ~Impl() {} };
		auto p = std::make_shared<UniformBufferObject>();
		if (!p) {
			return{};
		}

		//Securing size considering ubo alignment
		GLint offsetAlignment = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetAlignment);
		size = ((size + offsetAlignment - 1) / offsetAlignment) * offsetAlignment;

		//create ubo
		glGenBuffers(1, &p->uboId);
		glBindBuffer(GL_UNIFORM_BUFFER, p->uboId);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, p->uboId);

		if (glGetError()) {
			return {};
		}

		p->size = size;
		p->bindingPoint = bindingPoint;
		p->name = name;

		return p;
	}

	UniformBufferObject::~UniformBufferObject() {
		
		if (uboId) {
			glDeleteBuffers(1, &uboId);
		}
	}

	void UniformBufferObject::BindBufferRange(GLintptr offset, GLintptr size) {

		glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, uboId, offset, size);
	}

	bool UniformBufferObject::BufferSubData(const GLvoid* data, GLintptr offset, GLsizeiptr size) {

		if (offset + size > this->size) {
			return false;	// size over.
		}

		if (offset == 0 && size == 0) {
			size = this->size;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		return glGetError() == GL_NO_ERROR();
	}

}