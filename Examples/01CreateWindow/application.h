#ifndef __EXAMPLE_APP_01_H__
#define __EXAMPLE_APP_01_H__

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

class Example01Application : public furry2d::GLFWApplication {
public:
	Example01Application() : oldx(0), oldy(0), furry2d::GLFWApplication("GLFW Test") {
	}

	bool initialize() {
		this->engine_->updateSystem(this, true, false);
		return true;
	}

	void update(double elapsed) {
	}

	void render(double elapsed) {
		clear(1.f, 1.f, 0.f, 1.f);
	}

	void shutdown() {
	}

protected:
	void keyPressEvent(KeyboardEvent& ev) {
		std::cout << "Key pressed: " << (int)ev.key() << std::endl;
	}
	void keyReleaseEvent(KeyboardEvent& ev) {
		std::cout << "Key released:  " << (int)ev.key() << std::endl;
	}
	void mousePressEvent(MouseEvent& ev) {
		std::cout << "Btn pressed: " << (int)ev.btn() << std::endl;
	}
	void mouseReleaseEvent(MouseEvent& ev) {
		std::cout << "Btn released:  " << (int)ev.btn() << std::endl;
	}
	void mouseMoveEvent(double x, double y) {
		std::cout << "Mouse offset x/y: " << x-oldx << " | " << y-oldy << std::endl;
		oldx = x;
		oldy = y;
	}
	void scrollEvent(double x, double y) {
		std::cout << "scrollOffset x/y: " << x << " | " << y << std::endl;
	}
private:
	double oldx, oldy;
};

#endif