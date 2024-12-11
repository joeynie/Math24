#include "gui.hpp"

void LogPanel::init()
{
	begin();
	log_display = new Fl_Text_Display(10,10,width-20,height-60);
	log_buffer = new Fl_Text_Buffer();
	log_display->buffer(log_buffer);
	log_display->textsize(12);
	log_display->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
	log_display->textfont(FL_COURIER);
	end();
}

void LogPanel::loadLog()
{
	//清空显示区
	log_buffer->remove(0, log_buffer->length());
	std::fstream log_file;
	log_file.open("../app.log", std::fstream::in);
	// 将app.log文件内容读入log_buffer
	if (log_file.is_open())
	{
		std::string line;
		while (std::getline(log_file, line))
		{
			log_buffer->append(line.c_str());
			log_buffer->append("\n");
		}
		log_file.close();
	}

}
void LogPanel::show()
{
	loadLog();
	Fl_Group::show();
}


