/**
*	@file GLSystem.cpp
*/
#include "GLSystem.h"
#include <iostream>

namespace GLSystem {
	
	Window& GLSystem::Window::Instance(){

		static Window window;
		return window;
	}

	bool Window::Init(int width, int height, const char* title){

		this->width = width;
		this->height = height;

		std::cout << "[info]: GLSystem is Initializing...";

		try {

			if (isInitialized) {
				std::cout << "\n[warning]: system is already initialized!!" << std::endl;
				return true;
			}

			if (!glfwInit()) {
				throw("GLFW initialization faild!!");
			}
			glfwInitialized = true;

			window = glfwCreateWindow(width, height, title, nullptr, nullptr);

			if (!window) {
				throw("GLFW window creation failed!!");
			}

			glfwMakeContextCurrent(window);

			if (glewInit()) {
				throw("GLEW initialization failed!!");
			}
		}
		catch (const char* errStr) {

			std::cout << "[Error]: GLSystem initialization Faild" << std::endl;
			std::cout << "         " << errStr;
			return false;
		}

		std::cout << "completed." << std::endl;
	
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

		std::cout << "[Info]: Fainalized GLsystem" << std::endl;
		if (glfwInitialized) {
			glfwTerminate();
		}
	}



}