/**
*	@file GLSystem.cpp
*/
#include "GLSystem.h"
#include "DebugLogger.h"
#include <iostream>

namespace GLSystem {

	void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam) {

		auto& inst = DebugLogger::LogBuffer::Instance();
		inst.Log("receive gl message");
		inst.Log(message, type == GL_DEBUG_TYPE_ERROR ? DebugLogger::LogType::Error : DebugLogger::LogType::Infomation);
	}
	
	Window& GLSystem::Window::Instance(){

		static Window window;
		return window;
	}

	bool Window::Init(int width, int height, const char* title){

		auto& inst = DebugLogger::LogBuffer::Instance();

		this->width = width;
		this->height = height;

		inst.Log("GLSystem is Initializing...");

		try {

			if (isInitialized) {
				throw("system is already initialized!!");
			}

			if (!glfwInit()) {
				throw("GLFW initialization faild!!");
			}
			glfwInitialized = true;

			window = glfwCreateWindow(width, height, title, nullptr, nullptr);

			// During init, enable debug output
			glEnable(GL_DEBUG_OUTPUT);
			
			glDebugMessageCallback(MessageCallback, 0);

			if (!window) {
				throw("GLFW window creation failed!!");
			}

			glfwMakeContextCurrent(window);

			if (glewInit()) {
				throw("GLEW initialization failed!!");
			}
		}
		catch (const char* errStr) {

			inst.Log("GLSystem initialization Faild", DebugLogger::LogType::Error);
			inst.Log("error code: ", DebugLogger::LogType::Error);
			return false;
		}
		inst.Log("completed");
	
		return true;
	}

	bool Window::ShouldClose(){

		return glfwWindowShouldClose(window);
	}

	void Window::SwapBuffers() {

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Window::~Window(){

		DebugLogger::LogBuffer::Instance().Log("Fainalized GLsystem");
		if (glfwInitialized) {
			glfwTerminate();
		}
	}



}