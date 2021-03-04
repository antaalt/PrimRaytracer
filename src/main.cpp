#include "Viewer.h"

int main(int argc, char* argv[])
{
	viewer::Viewer app;

	aka::Config cfg{};
	cfg.width = 800;
	cfg.height = 600;
	cfg.name = "Game";
	cfg.app = &app;

	aka::Application::run(cfg);

	return 0;
}
