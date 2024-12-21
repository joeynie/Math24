#include "gui.hpp"


void MyWindow::init()
{
	logger.log("Math 24 init");
	color(color_bg);
	 Fl::scheme("");
	current_mode = -1;
	begin();

	modes.emplace_back(new Mode0(0,0,w(),h()));
	modes.emplace_back(new Mode1(0,0,w(),h()));
	modes.emplace_back(new Mode2(0,0,w(),h()));
	modes.emplace_back(new Mode3(0,0,w(),h()));
	modes.emplace_back(new Mode4(0,0,w(),h()));
	modes.emplace_back(new LogPanel(0,0,w(),h()));

	menu = new Fl_Menu_Window(0, 0, w(), h()-50);
	menu->color(color_bg);
	box_logo = new Fl_Box(w()/2-100, h()/2-100, 200, 80, "Math 24");
	box_logo->box(FL_NO_BOX);
	box_logo->color(color_bg);
	box_logo->labelsize(40);
	box_logo->labelfont(FL_COURIER);

	image = new Fl_PNG_Image("../images/logo.png");
	box_image = new Fl_Box(10,10,image->w(),image->h());
	box_image->box(FL_NO_BOX);
	box_image->image(image);

	int ww = 150, hh = 40;
	choose_mode.emplace_back(new MyButton(w()/2-ww/2, h()/2+0*50, ww, hh, "人机解题"));
	choose_mode.emplace_back(new MyButton(w()/2-ww/2, h()/2+1*50, ww, hh, "限时挑战"));
	choose_mode.emplace_back(new MyButton(w()/2-ww/2, h()/2+2*50, ww, hh, "竞速挑战"));
	choose_mode.emplace_back(new MyButton(w()/2-ww/2, h()/2+3*50, ww, hh, "平行世界"));
	choose_mode.emplace_back(new MyButton(w()/2-ww/2, h()/2+4*50, ww, hh, "时间回溯"));
	choose_mode.emplace_back(new MyButton(w()/2-ww/2, h()/2+5*50, ww, hh, "日志"));
	for(auto mode : choose_mode)	mode->callback(MyWindow::choose_mode_callback, this);	

	menu->end();

	button_return_menu = new MyButton(10, h()-50, 60, 30, "Return");
	button_return_menu->callback(MyWindow::return_menu_callback, this);
	button_return_menu->hide();
	button_exit = new MyButton(10, h()-50, 60, 30, "Exit");
	button_exit->callback(MyWindow::exit_callback, this);
	button_exit->show();
	end();
}
std::unordered_map<std::string, int> MyWindow::button_names = {
	{"人机解题",0},
	{"限时挑战",1},
	{"竞速挑战",2},
	{"平行世界",3},
	{"时间回溯",4},
	{"日志",5}
};
void MyWindow::choose_mode_callback(Fl_Widget* widget, void* data)
{
	MyWindow* window = (MyWindow*)data;
	window->menu->hide();
	std::string name = ((Fl_Button*)widget)->label();
	int index = button_names.find(name)->second;

	window->modes[index]->show();
	window->button_return_menu->show();
	window->button_exit->hide();
	window->current_mode = index;
}
void MyWindow::return_menu_callback(Fl_Widget* widget, void* data)
{
	MyWindow* window = (MyWindow*)data;
	window->menu->show();
	window->modes[window->current_mode]->hide();
	window->button_return_menu->hide();
	window->button_exit->show();
}
void MyWindow::exit_callback(Fl_Widget *widget, void *data)
{
	exit(0);
}
