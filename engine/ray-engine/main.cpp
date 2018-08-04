#include "Application.h"
int main(int argc, char *argv[])
{
	app::Application application(480, 360);
	application.run();
	
	system("pause");
	return 0;
}