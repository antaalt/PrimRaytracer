#include "Application.h"

#include "Utils/LogTest.h"
#include "Utils\Type.h"


int main(int argc, char *argv[])
{
	Application::Application app(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	app.run();

	system("pause");
	return 0;
}