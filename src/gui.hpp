#ifndef GUI_HPP
#define GUI_HPP

#include <iostream>
#include <string>
#include "FL/Fl.H"
#include "FL/Fl_Menu_Item.H"
#include "FL/Fl_Menu_Window.H"
#include "FL/Fl_Text_Display.H"
#include "FL/Fl_Timer.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include "FL/Fl_Input.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Output.H"
#include "logger.hpp"
#include "solve.hpp"
#include <sstream>
#include <vector>

class Mode0: public Fl_Group
{
public:
	Mode0(int x, int y, int w, int h) : Fl_Group(x, y, w, h),height(h),width(w)
	{
		init();
		hide();
	}
	void init();

	static void solve_callback(Fl_Widget* widget, void* data);

private:
	int height,width;
	Fl_Image* image;
	Fl_Input* input_nums;
	Fl_Button* button_solve;
	Fl_Output* output_result;
	Fl_Text_Display* log_display;

	Solver solver;
	Logger& logger=Logger::getInstance();
};

class Mode1: public Fl_Group
{
public:
	Mode1(int x, int y, int w, int h) : Fl_Group(x, y, w, h),height(h),width(w)
	{
		init();
		hide();
	}
	void init();

	static void random_callback(Fl_Widget* widget, void* data);
	static void input_answer_callback(Fl_Widget* widget, void* data);
	static void countdown_callback(void* data);
private:
	int height,width;
	Fl_Image* image;
	Fl_Output* nums;
	Fl_Button* button_random;
	Fl_Input* input_nums;
	Fl_Output* output_result;

	Solver solver;
	Logger& logger=Logger::getInstance();

	Fl_Box* box_countdown;
	int countdown_time;
	const int MAX_COUNTDOWN_TIME=30;
	std::string time_str;
	
	

};

class Mode2: public Fl_Group
{
public:
	Mode2(int x, int y, int w, int h) : Fl_Group(x, y, w, h),height(h),width(w)
	{
		init();
		hide();
	}
	void init();

private:
	int height,width;
	Fl_Image* image;
	Fl_Input* input_nums;
	Fl_Button* button_solve;
	Fl_Output* output_result;
	Fl_Text_Display* log_display;

	Solver solver;
	Logger& logger=Logger::getInstance();
};

class LogPanel: public Fl_Group
{
public:
	LogPanel(int x, int y, int w, int h) : Fl_Group(x, y, w, h),height(h),width(w)
	{
		init();
		hide();
	}
	void init();
	void show();

private:
	int height,width;
	Fl_Text_Display* log_display;
	Logger& logger=Logger::getInstance();
	Fl_Text_Buffer* log_buffer ;
	void loadLog();
};

class MyWindow : public Fl_Window
{
public:
	MyWindow(int w, int h, const char* title) : Fl_Window(w, h, title),height(h),width(w)
	{}
	void init();
	static void choose_mode_callback(Fl_Widget* widget, void* data);
	static void return_menu_callback(Fl_Widget* widget, void* data);
	static void exit_callback(Fl_Widget* widget, void* data);
private:
	int height,width,current_mode;
	Fl_Group* menu;
	std::vector<Fl_Group*> modes;
    std::vector<Fl_Button*> choose_mode;
    std::vector<std::string> button_names; 
	Fl_Button* button_return_menu, *button_exit;
	Fl_Box* box_logo;

	Logger& logger=Logger::getInstance();
};

#endif // GUI_HPP