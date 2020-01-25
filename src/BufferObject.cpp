/**
*	@file BufferObject.cpp
*/

#include "BufferObject.h"
#include "DebugLogger.h"

BufferObject::~BufferObject(){

	std::cout << "[Info]: BufferObject '" << name << "' has been destroyed";
	Destroy();
}

bool BufferObject::Init(const char* name, GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {

	std::string logstr("BufferObject::init() name:");
	logstr.insert(logstr.size(), name);
	DebugLogger::Log(logstr.c_str(), DebugLogger::Infomation, false);
		
	if (id != 0) {
		DebugLogger::Log("this buffer is Allready initialized!!", DebugLogger::LogType::Warning);
		return true;
	}

	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, size, data, usage);
	if (auto err = glGetError()) {
		DebugLogger::Log("BufferSubData faild!", DebugLogger::Error);
		return false;
	}

	//unbind
	glBindBuffer(target, 0);

	this->name = name;
	this->target = target;
	this->size = size;
	this->usage = usage;

	DebugLogger::Log("completed");

	return true;
}

bool BufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data){

	if (offset + size > this->size) {

		DebugLogger::Log("Insufficient destination buffer size",DebugLogger::Warning);
		return false;
	}

	glBindBuffer(target, id);
	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, 0);

	GLenum error = glGetError();

	if (error != GL_NO_ERROR) {
		DebugLogger::Log("Data transfer failed!!",DebugLogger::Error);
		return false;
	}
	return true;
}

void BufferObject::Destroy(){

	if (id) {
		glDeleteBuffers(1, &id);
	}
}

VertexArrayObject::~VertexArrayObject(){

	if (id) {
		glDeleteVertexArrays(1, &id);
	}
}

bool VertexArrayObject::Init(GLuint vboId, GLuint iboId){

	DebugLogger::Log("VertexArrayObject::Init() ", DebugLogger::Infomation, false);

	if (id != 0) {
		DebugLogger::Log("VAO is Already initialized!!",DebugLogger::Warning);
	}
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	//unbind
	glBindVertexArray(0);

	if (auto err = glGetError() != GL_NO_ERROR) {

		DebugLogger::Log("Error!",DebugLogger::Error);
		return false;
	}

	DebugLogger::Log("completed.");

	return true;
}

bool VertexArrayObject::Bind(){

	glBindVertexArray(id);
	return glGetError() == GL_NO_ERROR;
}

void VertexArrayObject::UnBind(bool vaoiboUnBind){

	glBindVertexArray(0);

	if (vaoiboUnBind) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void VertexArrayObject::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLsizei stride, GLvoid* offset, GLboolean normalized){

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, offset);
}

void VertexArrayObject::Destroy(){

	DebugLogger::Log("Destroy VAO");

	if (id) {
		glDeleteVertexArrays(1, &id);
	}
}
