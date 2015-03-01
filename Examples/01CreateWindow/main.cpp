#include <furry2d/furry2d.h>
#include "application.h"

#ifdef _DEBUG
#	pragma comment(lib, "../../bin/Debug/furry2dD.lib")
#else
#	pragma comment(lib, "../../bin/Release/furry2d.lib")
#endif

int main(int argc, char** argv) {
	std::cout << "Start Example" << std::endl;
	furry2d::Engine engine;

	engine.setApplication(new Example01Application());

	engine.run();

	return 0;
}
