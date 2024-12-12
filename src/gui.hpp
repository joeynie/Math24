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
#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_JPEG_Image.H"

static Fl_Color color_bg=fl_rgb_color(244,220,188);
static Fl_Color color_button=fl_rgb_color(255,211,130);

static std::string get_string(int num){
	if(num <= 10)return std::to_string(num);
	else switch(num){
		case 11: return "J";
		case 12: return "Q";
		case 13: return "K";
		default: return "";
	}
}
class MyButton : public Fl_Button
{
public:
	MyButton(int x, int y, int w, int h, const char* label="", int type=0) : Fl_Button(x, y, w, h, label)
	{	
		switch(type) {
			case 0: {
				labelsize(h/2);
				box(FL_NO_BOX);
				labelcolor(FL_WHITE);
				align(FL_ALIGN_INSIDE | FL_ALIGN_IMAGE_BACKDROP);
				Fl_PNG_Image* original_image = new Fl_PNG_Image("../images/button.png");
				Fl_Image* scaled_image = original_image->copy(w*1.5, h*1.5);
				image(scaled_image); 
			} break;
			case 1: {
				box(FL_NO_BOX);
				labelcolor(FL_WHITE);
				align(FL_ALIGN_INSIDE | FL_ALIGN_IMAGE_BACKDROP);
				Fl_PNG_Image* original_image = new Fl_PNG_Image("../images/continue.png");
				Fl_Image* scaled_image = original_image->copy(w, h);
				image(scaled_image); 
			} break;
			case 2: {
				labelsize(42);
				labelfont(FL_HELVETICA_BOLD);
				box(FL_NO_BOX);
				align(FL_ALIGN_CENTER | FL_ALIGN_IMAGE_BACKDROP);
				Fl_PNG_Image* original_image = new Fl_PNG_Image("../images/frame.png");
				Fl_Image* scaled_image = original_image->copy(170, 170);
				image(scaled_image);
			}
			default: break;
		}
	}
    int number;
	std::string number_str;
	void set_number(int num){
		number=num;
		update_label();
	}
	void update_label() {
		number_str=get_string(number);
		label(number_str.c_str());
		redraw();
	}
};
class Mode0: public Fl_Group
{
public:
	Mode0(int x, int y, int w, int h) : Fl_Group(x, y, w, h)
	{
		init();
		hide();
	}
	void init();

	static void solve_callback(Fl_Widget* widget, void* data);

private:
	Fl_Image* image;
	Fl_Box* box_image;
	Fl_Input* input_nums;
	Fl_Button* button_solve;
	Fl_Output* output_result;
	Fl_Text_Display* log_display;

	Solver solver;
	Logger& logger=Logger::getInstance();
};

class CountDown : public Fl_Widget
{
public:
	CountDown(int x, int y, int w, int h, int MAX_TIME) : Fl_Widget(x, y, w, h),x(x),y(y){
		time_left=MAX_TIME;
		max_time=MAX_TIME;
	}
	void draw()
	{
		fl_push_clip(x,y,w(),h());
		fl_color(color_bg);
		fl_rectf(x,y,w(),h());
		if(time_left > 0) {
			(time_left > 12.0f)? fl_color(FL_BLACK) : fl_color(FL_RED);
			(time_left > 8.0f)? fl_font(FL_HELVETICA, 24) : fl_font(FL_HELVETICA, 35);
			std::string time_left_str=std::to_string(time_left);
			time_left_str = time_left_str.substr(0,time_left_str.find(".")+2);
			fl_draw(time_left_str.c_str(), x, y, w(), h(), FL_ALIGN_CENTER);
		}
		else {
			fl_color(FL_RED);
			fl_font(FL_HELVETICA, 24);
			fl_draw("Time's up!", x, y, w(), h(), FL_ALIGN_CENTER);
		}
		fl_pop_clip();
	}
	void set_timer(bool count_down){
		is_running = count_down;
		if(count_down) {
			time_left=max_time;
			is_running=true;
			Fl::add_timeout(0.1, countdown_callback, this);
		}
	}
	float get_time_left() const {
		return time_left;
	}
private:
	int x,y;
	float time_left,max_time;
	bool is_running=false;
	static void countdown_callback(void* data) {
		CountDown* countdown=static_cast<CountDown*>(data);
		if(countdown->is_running) {
			countdown->time_left-=0.1;
			if(countdown->time_left<=0) {
				countdown->time_left=0;
				countdown->is_running=false;
				Fl::remove_timeout(countdown_callback, data);
			}
			else Fl::repeat_timeout(0.1, countdown_callback, data);
			Fl::redraw();
		}
	}
};

class Mode1: public Fl_Group
{
public:
	Mode1(int x, int y, int w, int h) : Fl_Group(x, y, w, h)
	{
		init();
		hide();
	}
	void show();
	void init();
	void reset();
	static void random_callback(Fl_Widget* widget, void* data);
	static void input_answer_callback(Fl_Widget* widget, void* data);
	// static void countdown_callback(void* data);
	static void show_answer_callback(void* data);
private:
	Fl_Image* image;
	Fl_Box* box_image;
	std::vector<MyButton*> boxs_question;
	std::vector<float> question;
	Fl_Button* button_random;
	Fl_Input* input_nums;
	Fl_Button* button_next;
	std::string answer_str;
	Fl_Output* output_result;

	Solver solver;
	Logger& logger=Logger::getInstance();

	const int MAX_COUNTDOWN_TIME=30;
	CountDown *countdown;
};

class CountUp : public Fl_Widget
{
public:
	CountUp(int x, int y, int w, int h) : Fl_Widget(x, y, w, h){
		time_left=0;
	}
	void draw()
	{
		fl_push_clip(x(),y(),w(),h());
		fl_color(color_bg);
		fl_rectf(x(),y(),w(),h());
		fl_color(FL_BLACK) ;
		fl_font(FL_HELVETICA, 24);
		std::string time_left_str=std::to_string(time_left);
		fl_draw(time_left_str.c_str(), x(), y(), w(), h(), FL_ALIGN_CENTER);
		fl_pop_clip();
	}
	void set_timer(bool count){
		is_running = count;
		if(is_running) {
			time_left = 0;
			is_running=true;
			Fl::add_timeout(1, count_callback, this);
		}
		else Fl::remove_timeout(count_callback, this);
	}
	float get_time_left() const {
		return time_left;
	}
private:
	int time_left;
	bool is_running=false;
	static void count_callback(void* data) {
		CountUp* countdown=static_cast<CountUp*>(data);
		if(countdown->is_running) {
			countdown->time_left+=1;
			Fl::repeat_timeout(1, count_callback, data);
			Fl::redraw();
		}
	}
};

class Mode2: public Fl_Group
{
public:
	Mode2(int x, int y, int w, int h) : Fl_Group(x, y, w, h)
	{
		init();
		hide();
	}
	void init();
	void reset();
	static void start_callback(Fl_Widget* widget, void* data);	
	static void random_callback(Fl_Widget* widget, void* data);
	static void input_answer_callback(Fl_Widget* widget, void* data);
	static void show_answer_callback(void* data);

private:
	Fl_Image* image;
	Fl_Box* box_image;
	// Fl_Output* nums;
	std::vector<MyButton*> boxs_question;
	std::vector<float> question;
	Fl_Button* button_random;
	Fl_Input* input_nums;
	Fl_Button* button_next;
	std::string answer_str;
	Fl_Output* output_result;

	Solver solver;
	Logger& logger=Logger::getInstance();

	int round , total_time;
	const int MAX_ROUND=10;
	Fl_Button* button_start;
	CountUp *countup;
};

class LogPanel: public Fl_Group
{
public:
	LogPanel(int x, int y, int w, int h) : Fl_Group(x, y, w, h)
	{
		init();
		hide();
	}
	void init();
	void show();

private:
	Fl_Text_Display* log_display;
	Logger& logger=Logger::getInstance();
	Fl_Text_Buffer* log_buffer ;
	void loadLog();
};

class MyWindow : public Fl_Window
{
public:
	MyWindow(int w, int h, const char* title) : Fl_Window(w, h, title)
	{}
	void init();
	static void choose_mode_callback(Fl_Widget* widget, void* data);
	static void return_menu_callback(Fl_Widget* widget, void* data);
	static void exit_callback(Fl_Widget* widget, void* data);
private:
	int current_mode;
	Fl_Group* menu;
	std::vector<Fl_Group*> modes;
    std::vector<Fl_Button*> choose_mode;
    static std::unordered_map<std::string,int> button_names;
	Fl_Button* button_return_menu, *button_exit;
	Fl_Box* box_logo;
	Fl_Image* image;
	Fl_Box* box_image;
	

	Logger& logger=Logger::getInstance();
};

#endif // GUI_HPP