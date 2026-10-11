#include <iostream>
#include <string>
using namespace std;
// ~ considered as epsilon
string addConcatanation(string &expression)
{
    string new_expression;
    for (int i = 0; i < expression.size(); i++)
    {
        if (i == 0)
            new_expression.push_back(expression[i]);
        else
        {
            if (((expression[i - 1] >= '0' && expression[i - 1] <= '9') ||
                 (expression[i - 1] >= 'A' && expression[i - 1] <= 'Z') || 
                 (expression[i - 1] >= 'a' && expression[i - 1] <= 'z') || 
                 expression[i - 1] == '*' || expression[i - 1] == '+' || 
                 expression[i - 1] == ')' || expression[i - 1] == '~'||
                 expression[i-1]=='?') &&
                ((expression[i] >= '0' && expression[i] <= '9') || 
                (expression[i] >= 'A' && expression[i] <= 'Z') || 
                (expression[i] >= 'a' && expression[i] <= 'z') || 
                expression[i] == '~' || expression[i] == '('))
            {
                new_expression.push_back('.');
                new_expression.push_back(expression[i]);
            }
            else
                new_expression.push_back(expression[i]);
        }
    }
    return new_expression;
}