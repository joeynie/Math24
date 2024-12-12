#include "solve.hpp"
#include <algorithm>
#include <iostream>

bool Solver::solve(std::vector<float>& nums)
{
    logger.log("Solving...");
    expressions.clear();
    results.clear();
    for(auto& num : nums) expressions.emplace_back(std::to_string((int)num));
    start_time = std::chrono::high_resolution_clock::now();
    solve(nums,expressions);
    getBestExpression();
    logger.log("Solved: " + getResult());
    get_time();
    return !results.empty();
}
void Solver::get_time()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::cout << "Time used: " << std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count() << " ns" << std::endl;
}
//dfs
void Solver::solve(std::vector<float>& nums,std::vector<std::string>& expression)
{
    int n = nums.size();
    for(int i=0; i<n; i++)
    {
        for(int j=i+1; j<n; j++)
        {
            float a = nums[i], b = nums[j];
            std::string exp_a = expression[i], exp_b = expression[j];
            std::vector<float> next;
            std::vector<std::string> next_exp;
            for(int k=0;k<n;k++){
                if(k!=i && k!=j) next.emplace_back(nums[k]),next_exp.emplace_back(expression[k]);
            }
            for(int k=0;k<6;k++){
                if(std::abs(b) < 1e-6 && k==3) continue; // 0不作除数
                if(std::abs(a) < 1e-6 && k==5) continue; // 0不作除数
                next.emplace_back(func[k](a,b));
                next_exp.emplace_back('('+func_str[k](exp_a,exp_b)+')');
                if(next.size() == 1){
                    if(std::abs(next[0] - goal) < 1e-6){
                        results.emplace_back(next_exp[0]);
                        // get_time();
                    }
                }
                if(next.size() > 1) solve(next,next_exp);
                next.pop_back();
                next_exp.pop_back();
            }
        }
    }
}

void Solver::getBestExpression()
{
    std::vector<std::pair<std::string,int>> results_compare;
    for(auto& result : results) {
        std::pair tmp(result,0);
        for(auto& c: tmp.first) if(c=='+' || c=='*') tmp.second++;
        results_compare.emplace_back(tmp);
    }
    std::sort(results_compare.begin(),results_compare.end(),
        [](const std::pair<std::string,int>& a,const std::pair<std::string,int>& b){
        return a.second > b.second;
    });
    results.clear();
    for(auto& result : results_compare) 
        results.emplace_back(result.first);
}

std::string Solver::getResult()
{
    if(results.empty()) return "No solution.";
    else return results[0];
}