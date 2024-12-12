#include "gui.hpp"
#include "calculator.hpp"
//平行世界 孪生模式
void Mode3::init()
{
	begin();
	image = new Fl_PNG_Image("../images/cute.png");
	box_image = new Fl_Box(10,10,image->w(),image->h());
	box_image->box(FL_NO_BOX);
	box_image->image(image);

	countup = new CountUp(w()/2-100,50,200,50);

	button_start = new MyButton(w()/2-80,120,120,40,"Start");
	button_start->callback(Mode3::start_callback,this);

	button_reset = new MyButton(w()-80,120,50,40,"Reset");
	button_reset->callback(Mode3::reset_callback,this);

	button_swap = new MyButton(w()-80,170,50,40,"Swap");
	button_swap->callback(Mode3::swap_callback,this);

	// nums = new Fl_Output(w()/2-100,h()/2, 200, 30, "Numbers");
	// nums->textsize(20);
	for(int i = 0; i < 4; i++){
		boxs_question.emplace_back(new MyButton(80+i%2*w()/3, 150 + i/2 * 150, 170, 170,"",2,i));
		boxs_question[i]->callback(Mode3::select_number_callback, this);
	}
	for(int i = 0; i < 4; i++){
		boxs_question.emplace_back(new MyButton(80+i%2*w()/3, 500 + i/2 * 150, 170, 170,"",2,i+4));
		boxs_question[i+4]->callback(Mode3::select_number_callback, this);
	}
	for(int i = 0; i < 4; i++){
		boxs_operator.emplace_back(new MyButton(w()/4-30+i*w()/6, 450, 60, 60,operators[i].c_str(),2,i));
		boxs_operator[i]->callback(Mode3::select_operator_callback, this);
	}
	// button_random = new MyButton(w()/2-100,h()/2+90, 200, 30, "Check answer");
	// button_random->callback(Mode3::input_answer_callback, this);

	button_next = new MyButton(w()/2+150,h()/2+60, 50, 50, "",1);
	button_next->callback(Mode3::random_callback, this);
	button_next->hide();

	output_result = new Fl_Output(w()/2-70,20, 150, 30, "Result:");
	output_result->textfont(FL_HELVETICA_BOLD);
	output_result->box(FL_NO_BOX);
	output_result->textsize(20);
	end();
}
void Mode3::show (){
	Fl_Group::show();
	countup->set_timer(false);
	reset();
}
void Mode3::start_callback(Fl_Widget* widget, void* data){
	Mode3* mode = (Mode3*)data;
	mode->reset();
	mode->countup->set_timer(true);
	random_callback(widget, data);
}
void Mode3::reset()
{
	round = 1;
	question.clear();
	question2.clear();
	// nums->value("");
	for(auto box : boxs_question) box->clear_label();
	// input_nums->value("");
	output_result->value("");
	merge.reset();

}

void Mode3::random_callback(Fl_Widget* widget, void* data){
	Mode3* window = (Mode3*)data;
	window->reset();
	// int question_set[8] = {7,10,7,5,  8,12,4,5};
	for(int i=0; i<4; i++){
		int num = std::rand()%13+1;
		window->question.push_back(num);
		// else  window->question2.push_back(num);
		window->boxs_question[i]->set_number(num);
	}
	window->solver.solve(window->question);
	window->answer_str = window->solver.getResult();
	// 确保可解
	if (window->answer_str == "No solution.") {random_callback(widget, data); return;}
	// question中数字在answer_str中的位置
	int tmp_question2[4]={-1,-1,-1,-1};
	for(int i=0,pos=0; i<4; i++){
		int start = window->answer_str.find_first_of("0123456789", pos);
		if(start==std::string::npos) break;
		int end = start+1;
		while(end<window->answer_str.size() && isdigit(window->answer_str[end])) end++;
		int num = std::stoi(window->answer_str.substr(start, end-start));
		for(int j=0; j<4; j++)
			if(tmp_question2[j]==-1 && num==window->question[j]) {tmp_question2[j] = i;break;}
		pos = end;
	}
	// 生成question2
	while(true){
		window->answer_str2 = window->answer_str;
		window->question2.clear();
		for(int i=0; i<4; i++){
			int num = std::rand()%13+1;
			window->question2.push_back(num);
		}
		// qustion2中数字替换answer_str中question1的数字
		for(int i=0,pos=0; i<4; i++){
			int start = window->answer_str2.find_first_of("0123456789", pos);
			if(start==std::string::npos) break;
			int end = start+1;
			while(end<window->answer_str2.size() && isdigit(window->answer_str2[end])) end++;
			window->answer_str2.replace(start, end-start, std::to_string((int)window->question2[i]));
			pos = start+std::to_string((int)window->question2[i]).size();
		}
		// std::cout << "question2: " << window->answer_str2 << std::endl;
		if(std::abs(calculate(window->answer_str2 ) - window->solver.goal) < 1e-6) break;
	}
	for(int i=0; i<4; i++){
		// question2 是 answer_str2中的顺序， 要变成显示框的顺序，即匹配question
		window->boxs_question[i+4]->set_number(window->question2[tmp_question2[i]]);
	}
	window->solver.solve(window->question2);
	window->answer_str2 = window->solver.getResult();
	if (window->answer_str2 == "No solution.") {random_callback(widget, data); return;}
	// Start countdown
	window->button_next->hide();
}
void Mode3::select_number_callback(Fl_Widget* widget, void* data){
	Mode3* window = (Mode3*)data;
	int index = ((MyButton*)widget)->index;
	index = index%4;
	std::cout << "Selected number: " << index << std::endl;
	int num = window->boxs_question[index]->number;
	int num2 = window->boxs_question[index+4]->number;
	if(window->merge.id1==-1) {
		window->merge.id1 = index;
		window->merge.num1 = num;
		window->merge.num3 = num2;
	}
	else if (window->merge.op==-1 && window->is_swap == false){
		window->merge.id1 = index;
		window->merge.num1 = num;
		window->merge.num3 = num2;
	}
	else if(window->merge.id2==-1 && window->merge.id1!=index) {
		window->merge.id2 = index;
		window->merge.num2 = num;
		window->merge.num4 = num2;
		if(window->merge.op!=-1){
			window->cal_merge();
		}
	}
}

void Mode3::select_operator_callback(Fl_Widget* widget, void* data){
	Mode3* window = (Mode3*)data;
	int op = ((MyButton*)widget)->index;
	// std::cout << "Selected operator: " << op << std::endl;
	window->merge.op = op;
	if(window->merge.id1!=-1 && window->merge.id2!=-1){
		window->cal_merge();
	}
}

void Mode3::reset_callback(Fl_Widget *widget, void *data){
	Mode3* window = (Mode3*)data;
	window->output_result->value("");
	for(int i=0; i<4; i++){
		window->boxs_question[i]->set_number(window->question[i]);
		window->boxs_question[i+4]->set_number(window->question2[i]);
	}
	window->merge.reset();
}

void Mode3::swap_callback(Fl_Widget *widget, void *data){
	Mode3* window = (Mode3*)data;
	window->is_swap = !window->is_swap;
	if(window->is_swap){
		window->button_swap->labelsize(24);
	}
	else {
		window->button_swap->labelsize(20);
	}
	if(window->merge.id1!=-1 && window->merge.id2!=-1){
		window->boxs_question[window->merge.id1]->set_number(window->merge.num2);
		window->boxs_question[window->merge.id2]->set_number(window->merge.num1);
	}
	window->merge.reset();
}