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

		std::cout << "[info]: GLSystem is Initializing...";

		if (isInitialized) {
			std::cout << "\n[warning]: system is already initialized!!" << std::endl;
		}

		if (!glfwInit()) {
			std::cout << "\n[error]: " << __FILE__ << "->" << __func__ << "glfwInit error!!" << std::endl;
			return false;
		}


		window = glfwCreateWindow(width, height, title, nullptr, nullptr);

		if (!window) {
			std::cout << "\n[error]: " << __FILE__ << "->" << __func__ << "glfwCreateWindow error!!" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);

		if (glewInit()) {
			std::cout << "\n[erorr]: " << __FILE__ << "->" << __func__ << "glewInit error!!" << std::endl;
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
	}



}