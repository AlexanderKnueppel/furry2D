/*
* ****************************************
*
* This file is part of Furry2D, a simple gameframework for 2D desktop games.
*
* Copyright (c) 2015 Furry2D. All rights reserved.
*
* For the full copyright and license information, please view the LICENSE.txt
* file that was distributed with this source code.
*
* \Author Alexander Knueppel
*
* ****************************************
*/

#include <furry2d/furry2d.h>

FURRY_NS_BEGIN

GLFWApplication* GLFWApplication::instance_ = nullptr;

GLFWApplication::GLFWApplication(const Args &args, const Config &config, std::string name) : Application(name) {
	config_ = config;
	instance_ = this;
}
GLFWApplication::GLFWApplication(const Args &args, std::string name) : Application(name) {
	config_ = Config();
	instance_ = this;
}
GLFWApplication::GLFWApplication(std::string name) : Application(name) {
	config_ = Config();
	instance_ = this;
}

bool GLFWApplication::initializeDependencies() {
	if (!glfwInit()) {
		gLogFatal << "Can't initialize GLFW";
		return false;
	}

	if (!tryCreateContext(config_)) {// preprocess config? Otherwise the parameter is useless...
		gLogFatal << "Context creation failed!";
		return false;
	}

	gLog << "Context creation successful";
	return true;
}

bool GLFWApplication::tryCreateContext(const Config &config) {
	int major = 0, minor = 0;
	switch (config.version()) {
	case Config::OpenGLVersion::OpenGL_3_3_Core:
		major = minor = 3;
		break;
	case Config::OpenGLVersion::OpenGL_4_0_Core:
		major = 4; minor = 0;
		break;
	case Config::OpenGLVersion::OpenGL_4_1_Core:
		major = 4; minor = 1;
		break;
	case Config::OpenGLVersion::OpenGL_4_2_Core:
		major = 4; minor = 2;
		break;
	case Config::OpenGLVersion::OpenGL_4_3_Core:
		major = 4; minor = 3;
		break;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*no support for multiple monitors at the moment*/
	window_ = glfwCreateWindow(config.width(), config.height(), config.title().c_str(), NULL, NULL);
	glfwMakeContextCurrent(window_);

	if (gl3wInit()) {
		gLogError << "Context can not be created (gl3w)";
		return false;
	}
	if (!gl3wIsSupported(major, minor)) {
		gLogError << "OpenGL "<< major << "." << minor <<" not supported";
		return false;
	}

	glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

	/* Input */
	glfwSetKeyCallback(window_, GLFWApplication::glfwKeyEvent);
	glfwSetMouseButtonCallback(window_, GLFWApplication::glfwMouseEvent);
	glfwSetCursorPosCallback(window_, GLFWApplication::glfwMouseMoveEvent);
	glfwSetScrollCallback(window_, GLFWApplication::glfwMouseScrollEvent);

	glfwMakeContextCurrent(window_);
	return true;
}

void GLFWApplication::shutdownDependencies() {
	glfwTerminate();
}
void GLFWApplication::update() {
	static Timer timer(true);
	double elapsed = timer.elapsed().count() / 1000.0;

	update(elapsed);
	render(elapsed);

	swapBuffers();
}

void GLFWApplication::swapBuffers() {
	glfwSwapBuffers(window_);
	glfwPollEvents();
}

/**/
void GLFWApplication::clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFWApplication::glfwKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
	KeyboardEvent e(static_cast<KeyboardEvent::Key_US>(key), 
					static_cast<InputEvent::Modifiers>(mods));
	switch (action) {
	case GLFW_PRESS:
		instance_->keyPressEvent(e);
		break;
	case GLFW_RELEASE:
		instance_->keyReleaseEvent(e);
		break;
	default:
		instance_->keyPressEvent(e);
	}
}

void GLFWApplication::glfwMouseEvent(GLFWwindow *window, int button, int action, int mods) {
	MouseEvent e(static_cast<MouseEvent::Button>(button),
		static_cast<InputEvent::Modifiers>(mods));
	switch (action) {
	case GLFW_PRESS:
		instance_->mousePressEvent(e);
		break;
	case GLFW_RELEASE:
		instance_->mouseReleaseEvent(e);
		break;
	default:
		instance_->mousePressEvent(e);
	}
}
void GLFWApplication::glfwMouseMoveEvent(GLFWwindow *window, double x, double y) { //position
	instance_->mouseMoveEvent(x, y);
}
void GLFWApplication::glfwMouseScrollEvent(GLFWwindow *window, double x, double y) { //offset
	instance_->scrollEvent(x, y);
}

FURRY_NS_END