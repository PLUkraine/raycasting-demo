#include "Window.h"

namespace core
{
	Window::Window()
		: m_window(nullptr)
	{
	}

	Window::~Window()
	{
		dispose();
	}

	void Window::createGlContext(int width, int height)
	{
		// TODO create window
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_RESIZABLE, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		m_window = glfwCreateWindow(width, height, "Default Title", nullptr, nullptr);
		if (!m_window) return;

		glfwMakeContextCurrent(m_window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;
		
		int w, h;
		glfwGetFramebufferSize(m_window, &w, &h);
		glViewport(0, 0, w, h);
	}

	void Window::update()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Window::dispose()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
	}
}
