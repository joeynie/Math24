#ifndef SOLVE_HPP
#define SOLVE_HPP

#include <vector>
#include "logger.hpp"

class Solver {
private:
    Logger& logger = Logger::getInstance();
    float (*func[6])(float,float) = {
        [](float a, float b){return a+b;},
        [](float a, float b){return a-b;},
        [](float a, float b){return a*b;},
        [](float a, float b){return a/b;},
        [](float a, float b){return b-a;},
        [](float a, float b){return b/a;},
    };
    std::string (*func_str[6])(std::string,std::string) = {
        [](std::string a, std::string b){return a + "+" + b;},
        [](std::string a, std::string b){return a + "-" + b;},
        [](std::string a, std::string b){return a + "*" + b;},
        [](std::string a, std::string b){return a + "/" + b;},
        [](std::string a, std::string b){return b + "-" + a;},
        [](std::string a, std::string b){return b + "/" + a;},
    };

    std::vector<std::string> expressions;//得到该数的表达式
    std::vector<std::string> results;
    void getBestExpression();
public:
    // Solver();
    bool solve(std::vector<float>& numbers);
    void solve(std::vector<float>& numbers, std::vector<std::string>& expression);
    std::string getResult();
    const float goal = 24;
};


#endif // SOLVE_HPP