#include <Platform/PC/Window.h>
#include <stdexcept>

namespace Stardust::Platform::PC {
	Window::Window(int w, int h, std::string t, bool f, bool v) : width(w), height(h), title(t), fullScreen(f), vsync(v)
	{
		if (!glfwInit()) {
			throw std::runtime_error("ERROR! Could not initialize GLFW.");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //MAC OS
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); //MAC OS

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