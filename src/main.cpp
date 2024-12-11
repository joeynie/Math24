#include "gui.hpp"
#include "logger.hpp"

int main(int argc, char** argv)
{
    Logger& logger = Logger::getInstance();
    logger.setLogFile("app.log");
    logger.log("Starting Math24");


	MyWindow* menu_window = new MyWindow(500, 800, "Math 24");
	menu_window->init();
	menu_window->show();


	return Fl::run();
}