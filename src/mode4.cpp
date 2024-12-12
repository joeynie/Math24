#include "gui.hpp"
#include "calculator.hpp"
//平行世界 孪生模式
void Mode4::init()
{
	begin();
	image = new Fl_PNG_Image("../images/cute.png");
	box_image = new Fl_Box(10,10,image->w(),image->h());
	box_image->box(FL_NO_BOX);
	box_image->image(image);

	countup = new CountUp(w()/2-100,50,200,50);

	button_start = new MyButton(w()/2-80,120,120,40,"Start");
	button_start->callback(Mode4::start_callback,this);

	button_reset = new MyButton(w()-80,120,50,40,"Reset");
	button_reset->callback(Mode4::reset_callback,this);

	button_swap = new MyButton(w()-80,170,50,40,"Swap");
	button_swap->callback(Mode4::swap_callback,this);

	button_revert = new MyButton(w()-80,220,50,40,"Revert");
	button_revert->callback(Mode4::revert_callback,this);

	// nums = new Fl_Output(w()/2-100,h()/2, 200, 30, "Numbers");
	// nums->textsize(20);
	for(int i = 0; i < 4; i++){
		boxs_question.emplace_back(new MyButton(80+i%2*w()/3, 150 + i/2 * 150, 170, 170,"",2,i));
		boxs_question[i]->callback(Mode4::select_number_callback, this);
	}
	for(int i = 0; i < 4; i++){
		boxs_operator.emplace_back(new MyButton(w()/4-30+i*w()/6, 450, 60, 60,operators[i].c_str(),2,i));
		boxs_operator[i]->callback(Mode4::select_operator_callback, this);
	}
	// button_random = new MyButton(w()/2-100,h()/2+90, 200, 30, "Check answer");
	// button_random->callback(Mode4::input_answer_callback, this);

	button_next = new MyButton(w()/2+150,h()/2+60, 50, 50, "",1);
	button_next->callback(Mode4::random_callback, this);
	button_next->hide();

	output_result = new Fl_Output(w()/2-70,20, 150, 30, "Result:");
	output_result->textfont(FL_HELVETICA_BOLD);
	output_result->box(FL_NO_BOX);
	output_result->textsize(20);
	end();
}
void Mode4::show (){
	Fl_Group::show();
	countup->set_timer(false);
	reset();
}
void Mode4::start_callback(Fl_Widget* widget, void* data){
	Mode4* mode = (Mode4*)data;
	mode->reset();
	mode->countup->set_timer(true);
	random_callback(widget, data);
}
void Mode4::reset()
{
	round = 1;
	question.clear();
	// nums->value("");
	for(auto box : boxs_question) {
		box->clear_label();
		box->is_used = false;
	}
	// input_nums->value("");
	output_result->value("");
	merge.reset();
	history_boxs.clear();
}

void Mode4::random_callback(Fl_Widget* widget, void* data){
	Mode4* window = (Mode4*)data;
	window->reset();
	// int question_set[8] = {7,10,7,5,  8,12,4,5};
	window->protects.clear();
	for(int i=0; i<4; i++){
		int num = std::rand()%13+1;
		window->question.push_back(num);
		// else  window->question2.push_back(num);
		window->boxs_question[i]->set_number(num);
		int ran = std::rand()%10;
		window->protects.emplace_back( ran<2 );
		window->boxs_question[i]->set_protect(window->protects[i]);
	}
	window->solver.solve(window->question);
	window->answer_str = window->solver.getResult();
	// 确保可解
	if (window->answer_str == "No solution.") {random_callback(widget, data); return;}
	// Start countdown
	window->button_next->hide();
}
void Mode4::select_number_callback(Fl_Widget* widget, void* data){
	Mode4* window = (Mode4*)data;
	if(((MyButton*)widget)->is_used) return;
	int index = ((MyButton*)widget)->index;
	index = index%4;
	std::cout << "Selected number: " << index << std::endl;
	int num = window->boxs_question[index]->number;
	if(window->merge.id1==-1) {
		window->merge.id1 = index;
		window->merge.num1 = num;
	}
	else if (window->merge.op==-1 && window->is_swap == false){
		window->merge.id1 = index;
		window->merge.num1 = num;
	}
	else if(window->merge.id2==-1 && window->merge.id1!=index) {
		window->merge.id2 = index;
		window->merge.num2 = num;
		if(window->merge.op!=-1){
			window->cal_merge();
		}
	}
}

void Mode4::select_operator_callback(Fl_Widget* widget, void* data){
	Mode4* window = (Mode4*)data;
	int op = ((MyButton*)widget)->index;
	// std::cout << "Selected operator: " << op << std::endl;
	window->merge.op = op;
	if(window->merge.id1!=-1 && window->merge.id2!=-1){
		window->cal_merge();
	}
}

void Mode4::cal_merge(){
	record();
	float ans = merge.merge(0);
	// std::cout << "ans: " << ans <<"  id2  "<<merge.id2 << std::endl;
	boxs_question[merge.id2]->set_number(ans); 
	boxs_question[merge.id2]->is_used = false;
	boxs_question[merge.id1]->clear_label();	
	boxs_question[merge.id1]->is_used = true;
	merge.id1 = merge.id2;
	merge.num1 = ans;								
	merge.id2 = -1;
	merge.num2 = -1;
	merge.op = -1;
	merge.merge_cnt ++;
	int left_cnt = 0;
	for(auto box : boxs_question) if(!box->is_used) left_cnt++;
	if(left_cnt == 1){
		if(std::abs(ans - solver.goal)<1e-6 ){
			output_result->value("Correct!");
			logger.log(" Correct!");
			button_next->show();
		}
		else{
			output_result->value("Incorrect.");
			logger.log("Incorrect.");
		}
	}
}

void Mode4::reset_callback(Fl_Widget *widget, void *data){
	Mode4* window = (Mode4*)data;
	window->output_result->value("");
	for(int i=0; i<4; i++){
		window->boxs_question[i]->set_number(window->question[i]);
		window->boxs_question[i]->is_used = false;
	}
	window->merge.reset();
}

void Mode4::swap_callback(Fl_Widget *widget, void *data){
	Mode4* window = (Mode4*)data;
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

void Mode4::revert_callback(Fl_Widget *widget, void *data){
	Mode4* window = (Mode4*)data;
	if(window->history_boxs.size() > 0){
		window->revert_to(window->history_boxs.size()-1);
	}
}