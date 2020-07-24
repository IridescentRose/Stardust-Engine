#include <Platform/PC/Window.h>
#include <stdexcept>
#include <iostream>

namespace Stardust::Platform::PC {
	void error_callback(int error, const char* str){
		std::cout << "ERROR: CODE " << error << " REASON: " << str << std::endl;
	}

	Window::Window(int w, int h, std::string t, bool f, bool v) : width(w), height(h), title(t), fullScreen(f), vsync(v)
	{
		m_Window = nullptr;
		
		glfwSetErrorCallback(error_callback);
		
		if (!glfwInit()) {
			throw std::runtime_error("ERROR! Could not initialize GLFW.");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //MAC OS
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); //MAC OS

		//No need for MSAA, I think retina handles this
#else
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Others
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Others
		
		glfwWindowHint(GLFW_SAMPLES, 4); //4x MSAA
#endif
		//Create Window
		if (fullScreen) {
			const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, vidmode->refreshRate);

			height = vidmode->height;
			width = vidmode->width;

			m_Window = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		else {
			m_Window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		}

		if(m_Window == nullptr){
			throw std::runtime_error("Failed to initialize a window!");
		}

		glfwMakeContextCurrent(m_Window);
		glfwShowWindow(m_Window);

		setVsync(v);
	}

	Window::~Window()
	{
		//Callbacks
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::update()
	{
		glfwPollEvents();
	}
	void Window::draw()
	{
		glfwSwapBuffers(m_Window);
	}
	Window* g_Window;
}
