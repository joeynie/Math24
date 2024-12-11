#include "gui.hpp"


void Mode0::init()
{
	begin();
	input_nums = new Fl_Input(width/2,height/2, 200, 30, "Enter numbers separated by space:");
	input_nums->textsize(20);

	output_result = new Fl_Output(width/2,height/2+30, 200, 30, "Result:");
	output_result->textfont(FL_HELVETICA_BOLD);
	output_result->textsize(20);
	button_solve = new Fl_Button(width/2,height/2+70, 200, 30, "Solve");
	button_solve->callback(Mode0::solve_callback, this);
	// log_display = new Fl_Text_Display(10, 10, width-20, height/2-20, "Log");
	// log_display->textsize(12);
	// log_display->textfont(FL_COURIER);

	end();
}

void Mode0::solve_callback(Fl_Widget* widget, void* data){
	Mode0* window = (Mode0*)data;
	window->output_result->value("Solving...");
	std::string input_str = window->input_nums->value();
	std::vector<float> nums;
	std::stringstream ss(input_str);
	std::string str;
	while(getline(ss, str, ' ')){
		try{
			float num = std::stof(str);
			nums.push_back(num);
		}
		catch(const std::exception& e){
			window->logger.log(std::string(e.what()), LogLevel::ERROR);
		}
	}
	if(nums.size() != 4){
		window->output_result->value("Error: Please enter 4 numbers separated by space.");
		window->logger.log("Error: Please enter 4 numbers separated by space.", LogLevel::ERROR);
		return;
	}
	window->solver.solve(nums);
	std::string result = window->solver.getResult();
	window->output_result->value(result.c_str());
	// auto buf = new Fl_Text_Buffer();
	// window->log_display->buffer(buf);
	// 追加文本
	// buf->append(result.c_str());
}

void MyWindow::init()
{
	logger.log("Math 24 init");
	color(fl_rgb_color(255, 255, 255));
	 Fl::scheme("");
	current_mode = -1;
	begin();
	modes.emplace_back(new Mode0(0,0,width,height));
	modes.emplace_back(new Mode1(0,0,width,height));

	menu = new Fl_Menu_Window(0, 0, width, height);
	menu->color(fl_rgb_color(255, 255, 255));
	box_logo = new Fl_Box(width/2-100, height/4, 200, 80, "Math 24");
	box_logo->box(FL_PLASTIC_UP_BOX);
	box_logo->color(fl_rgb_color(255, 255, 255));
	box_logo->labelsize(40);
	box_logo->labelfont(FL_COURIER);
	choose_mode.emplace_back(new Fl_Button(width/2-50, height/2+0*50, 100, 30, "Mode 0"));
	choose_mode.emplace_back(new Fl_Button(width/2-50, height/2+1*50, 100, 30, "Mode 1"));
	choose_mode[0]->callback(MyWindow::choose_mode_callback, this);	
	choose_mode[1]->callback(MyWindow::choose_mode_callback, this);

	menu->end();

	button_return_menu = new Fl_Button(10, height-50, 60, 30, "Return");
	button_return_menu->callback(MyWindow::return_menu_callback, this);
	button_return_menu->hide();
	end();
}
void MyWindow::choose_mode_callback(Fl_Widget* widget, void* data)
{
	MyWindow* window = (MyWindow*)data;
	std::string name = ((Fl_Button*)widget)->label();
	int index = std::stoi(name.substr(name.length()-1));
	window->menu->hide();
	
	window->modes[index]->show();
	window->button_return_menu->show();
	window->current_mode = index;
}
void MyWindow::return_menu_callback(Fl_Widget* widget, void* data)
{
	MyWindow* window = (MyWindow*)data;
	window->menu->show();
	window->modes[window->current_mode]->hide();
}

