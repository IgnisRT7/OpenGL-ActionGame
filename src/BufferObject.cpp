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

	auto& inst = DebugLogger::LogBuffer::Instance();

 	DebugLogger::LogBuffer::Log("BufferObject::init");
	inst.Log((std::string("name: ") + std::string(name)).c_str());
		
	if (id != 0) {
		inst.Log("this buffer is Allready initialized!!", DebugLogger::LogType::Warning);
		return true;
	}

	glGenBuffers(1, &id);
	glBindBuffer(target,id);
	if (data != nullptr) {

		glBufferData(target, size, data, usage);
		if (auto err = glGetError()) {
			std::cout << err << std::endl;
		}
	}

	//unbind
	glBindBuffer(target, 0);

	if (auto err = glGetError() != GL_NO_ERROR) {
		inst.Log("bufferobject generation failed! error is: ", DebugLogger::LogType::Error);
	//	inst.Log(reinterpret_cast<const char*>(gluGetString(err)),DebugLogger::LogType::Error);

		return false;
	}

	this->name = name;
	this->target = target;
	this->size = size;
	this->usage = usage;

	inst.Log("completed");

	return true;
}

bool BufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data){

	auto& inst = DebugLogger::LogBuffer::Instance();

	if (offset + size > this->size) {

		inst.Log("Insufficient destination buffer size",DebugLogger::Warning);
		return false;
	}

	glBindBuffer(target, id);
	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, 0);

	GLenum error = glGetError();

	if (error != GL_NO_ERROR) {
		inst.Log("Data transfer failed!!",DebugLogger::Error);
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

	auto& inst = DebugLogger::LogBuffer::Instance();
	inst.Log("VAO is initializing...");

	if (id != 0) {
		inst.Log("VAO is Already initialized!!",DebugLogger::Warning);
	}
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	//unbind
	glBindVertexArray(0);

	if (auto err = glGetError() != GL_NO_ERROR) {

		inst.Log("Error!",DebugLogger::Error);
		return false;
	}

	inst.Log("completed.");

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

	std::cout << "[Info]: Finalized VAO" << std::endl;

	if (id) {
		glDeleteVertexArrays(1, &id);
	}
}
