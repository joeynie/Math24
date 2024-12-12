#include "gui.hpp"
#include "calculator.hpp"
//竞速10组
void Mode2::init()
{
	begin();
	image = new Fl_PNG_Image("../images/cute.png");
	box_image = new Fl_Box(10,10,image->w(),image->h());
	box_image->box(FL_NO_BOX);
	box_image->image(image);

	countup = new CountUp(w()/2-100,50,200,50);

	button_start = new MyButton(w()/2-80,120,160,40,"Start");
	button_start->callback(Mode2::start_callback,this);

	// nums = new Fl_Output(w()/2-100,h()/2, 200, 30, "Numbers");
	// nums->textsize(20);
	for(int i = 0; i < 4; i++){
		boxs_question.emplace_back(new MyButton(80+i%2*w()/3, 150 + i/2 * 150, 170, 170,"",2));
	}
	button_random = new MyButton(w()/2-100,h()/2+70, 200, 30, "Check answer");
	button_random->callback(Mode2::input_answer_callback, this);

	input_nums = new Fl_Input(w()/2-100,h()/2+30, 200, 30, "Your answer:");
	input_nums->textsize(20);
	input_nums->callback(Mode2::input_answer_callback, this);
	input_nums->when(FL_WHEN_ENTER_KEY);
	button_next = new MyButton(w()/2+110,h()/2+20, 50, 50, "",1);
	button_next->callback(Mode2::random_callback, this);
	button_next->hide();


	output_result = new Fl_Output(w()/2-100,h()/2+110, 200, 30, "Result:");
	output_result->textfont(FL_HELVETICA_BOLD);
	output_result->textsize(20);
	end();
}
void Mode2::start_callback(Fl_Widget* widget, void* data){
	Mode2* mode = (Mode2*)data;
	mode->reset();
	mode->countup->set_timer(true);
	random_callback(widget, data);
}
void Mode2::reset()
{
	round = 1;
	question.clear();
	// nums->value("");
	for(auto box : boxs_question) box->label("");
	input_nums->value("");
	output_result->value("");

}

void Mode2::random_callback(Fl_Widget* widget, void* data){
	Mode2* window = (Mode2*)data;
	window->reset();
	for(int i=0; i<4; i++){
		int num = std::rand()%13+1;
		window->question.push_back(num);
		window->boxs_question[i]->set_number(num);
	}
	window->solver.solve(window->question);
	//显示题目
	// std::string nums_str = "";
	// for(int i=0; i<4; i++) nums_str += get_string((int)window->question[i]) + " ";
	// window->nums->value(nums_str.c_str());
	
	window->answer_str = window->solver.getResult();
	// 确保可解
	if (window->answer_str == "No solution.") {random_callback(widget, data); return;}

	// Start countdown
	window->button_next->hide();
}
void Mode2::input_answer_callback(Fl_Widget* widget, void* data){
	Mode2* window = (Mode2*)data;
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
		// window->countdown->set_timer(false);
		window->button_next->show();
		window->round++;
		std::cout << "Round " << window->round << std::endl;
		if(window->round>=window->MAX_ROUND) {
			window->countup->set_timer(false);
			window->button_start->label("Restart");
			window->button_start->redraw();
		}
		random_callback(widget, data);
	}
	else{
		window->output_result->value("Incorrect.");
		window->logger.log(input + "Incorrect.");
	}
}