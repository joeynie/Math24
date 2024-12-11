#include "gui.hpp"
#include "calculator.hpp"
//限时
void Mode1::init()
{
	std::srand(std::time(nullptr));
	begin();
	image = new Fl_PNG_Image("../images/scratch.png");
	box_image = new Fl_Box(10,10,image->w(),image->h());
	box_image->box(FL_NO_BOX);
	box_image->image(image);

	nums = new Fl_Output(w()/2-100,h()/2, 200, 30, "Numbers");
	nums->textsize(20);
	button_random = new MyButton(w()/2-100,h()/2+70, 200, 30, "Check answer");
	button_random->callback(Mode1::input_answer_callback, this);

	input_nums = new Fl_Input(w()/2-100,h()/2+30, 200, 30, "Your answer:");
	input_nums->textsize(20);
	input_nums->callback(Mode1::input_answer_callback, this);
	input_nums->when(FL_WHEN_ENTER_KEY);
	button_next = new MyButton(w()/2+110,h()/2+20, 50, 50, "",1);
	button_next->callback(Mode1::random_callback, this);

	countdown = new CountDown(w()/2-100,50,200,50,MAX_COUNTDOWN_TIME);

	output_result = new Fl_Output(w()/2-100,h()/2+110, 200, 30, "Result:");
	output_result->textfont(FL_HELVETICA_BOLD);
	output_result->textsize(20);
	end();
}
void Mode1::show (){
	Fl_Group::show();
	countdown->set_timer(false);
	reset();
}
void Mode1::reset()
{
	question.clear();
	nums->value("");
	input_nums->value("");
	output_result->value("");

}

void Mode1::random_callback(Fl_Widget* widget, void* data){
	Mode1* window = (Mode1*)data;
	window->reset();
	for(int i=0; i<4; i++){
		int num = std::rand()%13+1;
		window->question.push_back(num);
	}
	window->solver.solve(window->question);
	//显示题目
	std::string nums_str = "";
	for(int i=0; i<4; i++) nums_str += get_string((int)window->question[i]) + " ";
	window->nums->value(nums_str.c_str());
	
	window->answer_str = window->solver.getResult();
	// 确保可解
	if (window->answer_str == "No solution.") {random_callback(widget, data); return;}
	// 30s后显示答案
	Fl::remove_timeout(show_answer_callback, window);
	Fl::add_timeout(window->MAX_COUNTDOWN_TIME, Mode1::show_answer_callback, window);
	// window->output_result->value(window->answer_str.c_str());

	// Start countdown
	window->countdown->set_timer(true);
	window->button_next->hide();
}
void Mode1::show_answer_callback(void* data){
	Mode1* window = (Mode1*)data;
	window->output_result->value(window->answer_str.c_str());
	window->countdown->set_timer(false);
	window->button_next->show();
}
void Mode1::input_answer_callback(Fl_Widget* widget, void* data){
	Mode1* window = (Mode1*)data;
	std::string input = window->input_nums->value();
	// 检查用了给定的4个数字
	int count[21]= {0};
	for(int i=0; i<4; i++) count[(int)window->question[i]]--;
	for(int i=0;i<input.size();i++){
		switch (input[i]) {
			case 'J': count[11]++; break;
			case 'Q': count[12]++; break;
			case 'K': count[13]++; break;
			default: break;
		}
		if(input[i]>='0' && input[i]<='9'){
			if(i+1 < input.size() && input[i]=='1' && input[i+1]>='0' && input[i+1]<='9') {
				int num = std::stoi(input.substr(i,2));
				count[num]++;
				i++;
			}
			else count[input[i]-'0']++;
		}
	}
	for(int i=1; i<=13; i++) 
		if(count[i]!=0) {
			window->output_result->value("Invalid input.");
			window->logger.log(input + "Invalid input.");
			return;
		}
	// 检查是否等于24
	double result = calculate(input);
	// window->logger.log(std::to_string(result));
	if(std::abs(result - window->solver.goal)<1e-6){
		window->output_result->value("Correct!");
		window->logger.log(input + " Correct!");
		window->countdown->set_timer(false);
		window->button_next->show();
	}
	else{
		window->output_result->value("Incorrect.");
		window->logger.log(input + "Incorrect.");
	}
}

// void Mode1::countdown_callback(void* data){
// 	Mode1* window = (Mode1*)data;
// 	window->countdown_time--;
// 	window->time_str = std::to_string(window->countdown_time)+"s";
// 	window->box_countdown->label(window->time_str.c_str());
// 	if(window->countdown_time>0){
// 		Fl::add_timeout(1.0, Mode1::countdown_callback, window);
// 	}
// 	else{
// 		window->box_countdown->label("Time's up!");
// 		window->input_nums->value("");
// 	}

// }