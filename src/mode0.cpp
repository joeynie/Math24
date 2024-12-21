#include "gui.hpp"
#include "calculator.hpp"

//玩家出题
void Mode0::init()
{
	begin();
	image = new Fl_PNG_Image("../images/nap.png");
	box_image = new Fl_Box(10,10,image->w(),image->h());
	box_image->box(FL_NO_BOX);
	box_image->image(image);

	input_nums = new Fl_Input(w()/2-100,h()/2-100, 200, 50, "4 numbers:");
	input_nums->textsize(30);

	output_result = new Fl_Output(w()/2-100,h()/2-40, 200, 50, "Result:");
	output_result->textfont(FL_HELVETICA_BOLD);
	output_result->textsize(24);
	button_solve = new MyButton(w()/2-100,h()/2+20, 200, 50, "Solve");
	button_solve->callback(Mode0::solve_callback, this);

	button_load_file = new MyButton(w()/2-100,h()/2+100, 200, 50, "Load File");
	button_load_file->callback(Mode0::load_file_callback, this);

	end();
}

void Mode0::solve_callback(Fl_Widget* widget, void* data){
	Mode0* window = (Mode0*)data;
	window->output_result->value("Solving...");
	std::string input_str = window->input_nums->value();
	std::vector<float> nums;
	std::stringstream ss(input_str);
	std::string str;
	while(ss >> str){
		try{
			float num ;
			if (str == "J")num = 11;
			else if (str == "Q")num = 12;
			else if (str == "K")num = 13;
			else if (str == "A")num = 1;
			else num = std::stof(str);
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
}

void Mode0::load_file_callback(Fl_Widget* widget, void* data){
	Mode0* window = (Mode0*)data;
	Fl_File_Chooser fc("../","txt(*.txt)", Fl_File_Chooser::SINGLE, "*.txt");
	fc.show();
	while(fc.shown()){
		Fl::wait();
	}
	if(fc.count() == 1){
		std::string file_path = fc.value(1);
		window->logger.log("Loading file: " + file_path, LogLevel::INFO);
		std::ifstream file(file_path);
		if(file.is_open()){
			std::ofstream output_file(file_path.replace(file_path.find(".txt"), 4, "_result.txt"));
			if(!output_file.is_open()){
				window->logger.log("Error: Failed to open output file.", LogLevel::ERROR);
				return;
			}
			std::string line;
			while(getline(file, line)){
				window->input_nums->value(line.c_str());
				window->solve_callback(nullptr, data);
				if(std::string(window->output_result->value()).compare("No solution.") == 0)
					output_file << "-\t" << line << std::endl;
				else
					output_file << "+\t" << line << std::endl;
			}
			file.close();
			output_file.close();
		}
		else{
			window->logger.log("Error: Failed to open file.", LogLevel::ERROR);
		}
	}
}