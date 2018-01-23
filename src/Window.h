#ifndef WINDOW_H
#define WINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace core
{
	class Window
	{
	private:
		GLFWwindow *m_window;

	public:
		Window();
		~Window();

		void createGlContext(int width, int height);
		void update();
		void dispose();
		inline int mustClose() { return glfwWindowShouldClose(m_window); }
		inline int getKeyState(int key) { return glfwGetKey(m_window, key); }
	};
}

#endif