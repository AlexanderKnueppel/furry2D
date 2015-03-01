#ifndef __FURRY_CORE_GLFWAPPLICATION_H__
#define __FURRY_CORE_GLFWAPPLICATION_H__  

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

struct GLFWwindow;
#include <memory>
#include <vector>
#include <map>

FURRY_NS_BEGIN

class FURRY_API GLFWApplication : public Application {
private:
	GLFWwindow* window_;
public:
	struct Args {
		int &argc;
		char **argv;
	};
	/* Input */
	class InputEvent;
	class KeyboardEvent;
	class MouseEvent;
	//class MouseMoveEvent;
	//class MouseScrollEvent;
	
	/* Configuration */
	class Config {
	public:
		enum class OpenGLVersion : std::uint8_t {
			OpenGL_3_3_Core = 0,
			OpenGL_4_0_Core,
			OpenGL_4_1_Core,
			OpenGL_4_2_Core,
			OpenGL_4_3_Core,
			VersionCount
		};
		enum WindowFlag {
			Resizable = 1 << 0,
			Fullscreen = 1 << 1,
			Maximized = 1 << 2,
			Minimized = 1 << 3
		};
		typedef std::uint32_t WindowFlags;

		Config() { //Default
			setTitle("No title");
			setSize(800, 600);
			setVersion(OpenGLVersion::OpenGL_3_3_Core);
			setWindowFlags(WindowFlag::Resizable);
		}

		Config& setTitle(std::string title) {
			title_ = std::move(title);
			return *this;
		}
		std::string title() const {
			return title_;
		}

		Config& setSize(const std::uint16_t &width, const std::uint16_t &height) {
			width_ = width; height_ = height;
			return *this;
		}
		std::uint16_t width() const { return width_; }
		std::uint16_t height() const { return height_; }

		Config& setVersion(OpenGLVersion version) {
			opengl_version_ = version;
			return *this;
		}
		OpenGLVersion version() const {
			return opengl_version_;
		}

		Config& setWindowFlags(WindowFlags flags) {
			flags_ = flags;
			return *this;
		}
		WindowFlags flags() const {
			return flags_;
		}
	private:
		std::string   title_;
		OpenGLVersion opengl_version_;
		WindowFlags   flags_;
		std::uint16_t width_, height_;
	};

	void setConfig(Config &config) {
		config_ = config;
	}
	const Config &config() {
		return config_;
	}

	explicit GLFWApplication(const Args &args, const Config &config, std::string name);
	explicit GLFWApplication(const Args &args, std::string name);
	explicit GLFWApplication(std::string name);
	
	/** No copying and moving allowed*/
	GLFWApplication(const GLFWApplication&) = delete;
	GLFWApplication(GLFWApplication&&) = delete;
	GLFWApplication& operator=(const GLFWApplication&) = delete;
	GLFWApplication& operator=(GLFWApplication&&) = delete;

	//virtual bool initialize() = 0;
	//virtual void shutdown() = 0;
	
	virtual void render(double elapsedTime) = 0;
	virtual void update(double elapsedTime) = 0;

	bool initializeDependencies();
	void shutdownDependencies();

	bool tryCreateContext(const Config &config);

	void update();
	void swapBuffers();
protected:
	/* Input */
	virtual void keyPressEvent(KeyboardEvent& ev) {}
	virtual void keyReleaseEvent(KeyboardEvent& ev) {}

	virtual void mousePressEvent(MouseEvent& ev) {}
	virtual void mouseReleaseEvent(MouseEvent& ev) {}
	virtual void mouseMoveEvent(double xpos, double ypos) {}
	virtual void scrollEvent(double xoffset, double yoffset) {}

	Config config_;

	//void setMousePosition();
	//void showCursor(bool);

	// some opnegl-specific methods
	void clear(float r, float g, float b, float a);
private:
	static GLFWApplication* instance_;
	static void glfwKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void glfwMouseEvent(GLFWwindow *window, int button, int action, int mods);
	static void glfwMouseMoveEvent(GLFWwindow *window, double x, double y);
	static void glfwMouseScrollEvent(GLFWwindow *window, double x, double y);
};

class GLFWApplication::InputEvent {
public:
	InputEvent(const InputEvent&) = delete;
	InputEvent(InputEvent&&) = delete;
	InputEvent& operator=(const InputEvent&) = delete;
	InputEvent& operator=(InputEvent&&) = delete;

	enum class Modifiers : std::uint8_t {
		Shift = GLFW_MOD_SHIFT,
		Ctrl = GLFW_MOD_CONTROL,
		Alt = GLFW_MOD_ALT,
		Super = GLFW_MOD_SUPER
	};
protected:
	explicit InputEvent() {}
};

class GLFWApplication::KeyboardEvent : GLFWApplication::InputEvent {
	friend class GLFWApplication;
public:
	enum class Key_US : std::int16_t {
		Unknown = GLFW_KEY_UNKNOWN,
		Space = GLFW_KEY_SPACE,
		Apostrophe = GLFW_KEY_APOSTROPHE,
		Comma = GLFW_KEY_COMMA,
		Minus = GLFW_KEY_MINUS,
		Period = GLFW_KEY_PERIOD,
		Slash = GLFW_KEY_SLASH, 
		Zero = GLFW_KEY_0,
		One = GLFW_KEY_1,
		Two = GLFW_KEY_2,
		Three = GLFW_KEY_3,
		Four = GLFW_KEY_4,
		Five = GLFW_KEY_5,
		Six = GLFW_KEY_6 ,
		Seven = GLFW_KEY_7,
		Eight = GLFW_KEY_8,
		Nine = GLFW_KEY_9,
		Semicolon = GLFW_KEY_SEMICOLON,
		Equal = GLFW_KEY_EQUAL,
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O, 
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R =	GLFW_KEY_R,
		S =	GLFW_KEY_S,
		T =	GLFW_KEY_T,
		U =	GLFW_KEY_U,
		V =	GLFW_KEY_V,
		W =	GLFW_KEY_W,
		X =	GLFW_KEY_X,
		Y =	GLFW_KEY_Y,
		Z =	GLFW_KEY_Z,
		LeftBracket = GLFW_KEY_LEFT_BRACKET,
		BackSlash =	GLFW_KEY_BACKSLASH,
		RightBracket = GLFW_KEY_RIGHT_BRACKET,
		GraveAccent = GLFW_KEY_GRAVE_ACCENT,
		World1 = GLFW_KEY_WORLD_1,
		World2 = GLFW_KEY_WORLD_2,
		Escape = GLFW_KEY_ESCAPE,
		Enter = GLFW_KEY_ENTER,
		Tab = GLFW_KEY_TAB,
		Backspace =	GLFW_KEY_BACKSPACE,
		Insert = GLFW_KEY_INSERT,
		Delete = GLFW_KEY_DELETE,
		Right =	GLFW_KEY_RIGHT,
		Left = GLFW_KEY_LEFT,
		Down = GLFW_KEY_DOWN,
		Up = GLFW_KEY_UP,
		PageUp = GLFW_KEY_PAGE_UP,
		PageDown = GLFW_KEY_PAGE_DOWN,
		Home = GLFW_KEY_HOME,
		End = GLFW_KEY_END,
		CapsLock = GLFW_KEY_CAPS_LOCK,
		ScrollLock = GLFW_KEY_SCROLL_LOCK,
		NumLock = GLFW_KEY_NUM_LOCK,
		Print = GLFW_KEY_PRINT_SCREEN,
		Pause = GLFW_KEY_PAUSE,
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		 //= 	GLFW_KEY_F13,
		 //= 	GLFW_KEY_F14,
		 //= 	GLFW_KEY_F15,
		 //= 	GLFW_KEY_F16,
		 //= 	GLFW_KEY_F17,
		 //= 	GLFW_KEY_F18,
		 //= 	GLFW_KEY_F19,
		 //= 	GLFW_KEY_F20,
		 //= 	GLFW_KEY_F21,
		 //= 	GLFW_KEY_F22,
		 //= 	GLFW_KEY_F23,
		 //= 	GLFW_KEY_F24,
		 //= 	GLFW_KEY_F25,
		NumZero = GLFW_KEY_KP_0,
		NumOne = GLFW_KEY_KP_1,
		NumTwo = GLFW_KEY_KP_2,
		NumThree = GLFW_KEY_KP_3,
		NumFour = GLFW_KEY_KP_4,
		NumFive = GLFW_KEY_KP_5,
		NumSix = GLFW_KEY_KP_6,
		NumSeven = GLFW_KEY_KP_7,
		NumEight = GLFW_KEY_KP_8,
		NumNine = GLFW_KEY_KP_9,
		Decimal = GLFW_KEY_KP_DECIMAL,
		Divide = GLFW_KEY_KP_DIVIDE,
		Multiply = GLFW_KEY_KP_MULTIPLY,
		Subtract = GLFW_KEY_KP_SUBTRACT,
		Add = GLFW_KEY_KP_ADD,
		NumEnter = GLFW_KEY_KP_ENTER,
		NumEqual = GLFW_KEY_KP_EQUAL,
		LeftShift = GLFW_KEY_LEFT_SHIFT,
		LeftCtrl = GLFW_KEY_LEFT_CONTROL,
		LeftAlt = GLFW_KEY_LEFT_ALT,
		LeftSuper = GLFW_KEY_LEFT_SUPER,
		RightShift = GLFW_KEY_RIGHT_SHIFT,
		RightCtrl = GLFW_KEY_RIGHT_CONTROL,
		RightAlt = 	GLFW_KEY_RIGHT_ALT,
		RightSuper = GLFW_KEY_RIGHT_SUPER,
		Menu = GLFW_KEY_MENU ,
		Last = 	Menu
	};

	typedef Key_US Key;

	Key key() const { 
		return keys_;  
	}
	Modifiers modifiers() const {
		return modifiers_;
	}
private:
	KeyboardEvent(Key key, Modifiers modifiers) : keys_(key), modifiers_(modifiers) {}
	Key keys_;
	Modifiers modifiers_;
};

class GLFWApplication::MouseEvent : GLFWApplication::InputEvent{
	friend class GLFWApplication;
public:
	enum class Button : std::int16_t {
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Right = GLFW_MOUSE_BUTTON_RIGHT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		Button1 = GLFW_MOUSE_BUTTON_1,
		Button2 = GLFW_MOUSE_BUTTON_2,
		Button3 = GLFW_MOUSE_BUTTON_3,
		Button4 = GLFW_MOUSE_BUTTON_4,
		Button5 = GLFW_MOUSE_BUTTON_5,
		Button6 = GLFW_MOUSE_BUTTON_6,
		Button7 = GLFW_MOUSE_BUTTON_7,
		Button8 = GLFW_MOUSE_BUTTON_8,
		Last = GLFW_MOUSE_BUTTON_LAST
	};

	Button btn() const {
		return button_;
	}
	Modifiers modifiers() const {
		return modifiers_;
	}
private:
	MouseEvent(Button btn, Modifiers modifiers) : button_(btn), modifiers_(modifiers) {}
	Button button_;
	Modifiers modifiers_;
};

FURRY_NS_END

#endif
