#include "gui.hpp"
#include "calculator.hpp"

void Mode1::init()
{
	std::srand(std::time(nullptr));
	begin();
	nums = new Fl_Output(width/2-100,height/2, 200, 30, "Numbers");
	nums->textsize(20);

	output_result = new Fl_Output(width/2-100,height/2+30, 200, 30, "Result:");
	output_result->textfont(FL_HELVETICA_BOLD);
	output_result->textsize(20);
	button_random = new Fl_Button(width/2-100,height/2+70, 200, 30, "Next task");
	button_random->callback(Mode1::random_callback, this);

	input_nums = new Fl_Input(width/2-100,height/2+110, 200, 30, "Your answer:");
	input_nums->textsize(20);
	input_nums->callback(Mode1::input_answer_callback, this);
	input_nums->when(FL_WHEN_ENTER_KEY);

	box_countdown = new Fl_Box(width/2-50,50, 100, 50, "30s");
	box_countdown->box(FL_BORDER_FRAME);
	box_countdown->labelsize(30);
	box_countdown->align(FL_ALIGN_CENTER);
	box_countdown->color(FL_WHITE);
	end();
}

void Mode1::random_callback(Fl_Widget* widget, void* data){
	Mode1* window = (Mode1*)data;
	std::vector<float> nums;
	for(int i=0; i<4; i++){
		int num = std::rand()%13+1;
		nums.push_back(num);
	}
	window->solver.solve(nums);
	std::string nums_str = "";
	for(int i=0; i<4; i++) nums_str += std::to_string((int)nums[i]) + " ";
	window->nums->value(nums_str.c_str());
	
	std::string result = window->solver.getResult();
	window->output_result->value(result.c_str());

	// Start countdown
	Fl::remove_timeout(Mode1::countdown_callback, window);
	Fl::add_timeout(3.0, Mode1::countdown_callback, window);
	window->countdown_time = window->MAX_COUNTDOWN_TIME;
	window->time_str = std::to_string(window->countdown_time)+"s";
	window->box_countdown->label(window->time_str.c_str());
}

void Mode1::input_answer_callback(Fl_Widget* widget, void* data){
	Mode1* window = (Mode1*)data;
	std::string input = window->input_nums->value();
	double result = calculate(input);
	window->logger.log(std::to_string(result));
	if(std::abs(result - window->solver.goal)<1e-6){
		window->output_result->value("Correct!");
	}
	else{
		window->output_result->value("Incorrect,please try again.");
	}
}

void Mode1::countdown_callback(void* data){
	Mode1* window = (Mode1*)data;
	window->countdown_time--;
	window->time_str = std::to_string(window->countdown_time)+"s";
	window->box_countdown->label(window->time_str.c_str());
	if(window->countdown_time>0){
		Fl::add_timeout(1.0, Mode1::countdown_callback, window);
	}
	else{
		window->box_countdown->label("Time's up!");
		window->input_nums->value("");
	}

}