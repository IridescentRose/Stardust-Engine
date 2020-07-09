#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Stardust::Platform::PC {
	class Window {
	public:
		Window(int width, int height, std::string title, bool fullscreen, bool vsync);
		~Window();

		void update();
		void draw();

		inline int getWidth() {
			return width;
		}

		inline int getHeight() {
			return height;
		}

		inline int getWindowWidth() {
			int w, h;
			glfwGetWindowSize(m_Window, &w, &h);
			return w;
		}

		inline int getWindowHeight() {
			int w, h;
			glfwGetWindowSize(m_Window, &w, &h);
			return h;
		}

		inline std::string getTitle() {
			return title;
		}

		inline void setTitle(std::string t) {
			title = t;
			glfwSetWindowTitle(m_Window, title.c_str());
		}

		inline bool shouldClose() {
			return glfwWindowShouldClose(m_Window);
		}

		inline void setFullScreen(bool fs) {
			fullScreen = fs;

			const GLFWvidmode* vid = glfwGetVideoMode(glfwGetPrimaryMonitor());
			if (!fullScreen) {
				glfwSetWindowMonitor(m_Window, NULL, 0, 0, width, height, 0);

				glfwSetWindowSize(m_Window, width, height);
				glfwHideWindow(m_Window);
				glfwSetWindowPos(
					m_Window, vid->width / 2 - width / 2, vid->height / 2 - height / 2);
				glfwShowWindow(m_Window);
			}
			else {
				glfwSetWindowMonitor(
					m_Window, glfwGetPrimaryMonitor(), 0, 0, vid->width, vid->height, vid->refreshRate);
			}
		}

		inline void setVsync(bool v) {
			vsync = v;
			if (vsync) {
				glfwSwapInterval(1);
			}
			else {
				glfwSwapInterval(0);
			}
		}

	private:
		int width, height;
		std::string title;
		bool fullScreen;
		bool vsync;

		GLFWwindow* m_Window;
	};

	extern Window* g_Window;
}