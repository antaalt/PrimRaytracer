#include "Application.h"
/*int preview()
{
	app::Application application(480, 360);
	application.run();

	system("pause");
	return 0;
}

#include "WhittedTracer.h"
#include "PinholeCamera.h"
int saveToFile(std::string path)
{
	unsigned int width = 480;
	unsigned int height = 360;
	app::tracer::Renderer renderer(width, height);
	bool initState = renderer.init();
	renderer.loadScene("", app::tracer::Acceleration::NO_ACCEL);
	renderer.setCamera(new app::tracer::PinholeCamera(width, height));
	renderer.setTracer(new app::tracer::WhittedTracer());
	bool renderState = renderer.render();
	return 0;
}*/

int main(int argc, char *argv[])
{
	app::Application application(480, 360);
	application.run();
	
	system("pause");
	return 0;
}