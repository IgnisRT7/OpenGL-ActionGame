/**
*	@file BufferObject.cpp
*/

#include "BufferObject.h"

BufferObject::~BufferObject(){

	if (id) {
		glDeleteBuffers(1, &id);
	}
}

bool BufferObject::Init(const char* name, GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {

	std::cout << "[Info]: '" << name << "' is initializing...";

	if (id != 0) {
		std::cout << "\n[Warning]: '" << name << "'is Already initialized!!" << std::endl;
	}

	glGenBuffers(1, &id);
	glBindBuffer(target,id);
	if (data != nullptr) {
		glBufferData(target, size, data, usage);
	}

	//unbind
	glBindBuffer(target, 0);

	if (auto err = glGetError() != GL_NO_ERROR) {
		std::cout << "\n[Error]: errorcode : " << std::hex << err << std::endl;
		return false;
	}

	this->name = name;
	this->target = target;
	this->size = size;
	this->usage = usage;

	std::cout << "completed." << std::endl;

	return true;
}

VertexArrayObject::~VertexArrayObject(){

	if (id) {
		glDeleteVertexArrays(1, &id);
	}
}

bool VertexArrayObject::Init(GLuint vboId, GLuint iboId){

	std::cout << "[Info]: VAO is initializing...";

	if (id != 0) {
		std::cout << "\n[Warning]: VAO is Already initialized!!" << std::endl;
	}
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	//unbind
	glBindVertexArray(0);

	if (auto err = glGetError() != GL_NO_ERROR) {

		std::cout << "\n[Error]: errorcode : " << std::hex << err << std::endl;
		return false;
	}

	return false;
}

bool VertexArrayObject::Bind(){

	glBindVertexArray(id);
	return glGetError() != GL_NO_ERROR;
}

void VertexArrayObject::UnBind(bool vaoiboUnBind){

	glBindVertexArray(0);
	if (vaoiboUnBind) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void VertexArrayObject::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLsizei stride, GLvoid* offset, GLboolean normalized){

	glVertexAttribPointer(index, size, type, normalized, stride, offset);
}
