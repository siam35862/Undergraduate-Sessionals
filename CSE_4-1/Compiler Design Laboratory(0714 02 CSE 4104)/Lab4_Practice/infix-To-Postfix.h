#include <iostream>
#include <string>
#include <stack>

using namespace std;

void infixToPostfix(string &infix, string &postfix)
{
    stack<char> stack;
    string binary_oprator;
    binary_oprator = "|.";
    
    for (char c : infix)
    {
        if (c == '(')
        {
            stack.push(c);
        }
        else if (binary_oprator.find(c) != std::string::npos)
        {
            while (!stack.empty() && stack.top() != '(' && binary_oprator.find(c) <= binary_oprator.find(stack.top()))
            {
                postfix += stack.top();
                stack.pop();
            }
            stack.push(c);
        }
        else if (c == ')')
        {
            while (!stack.empty()&&stack.top() != '(')
            {
                postfix += stack.top();
                stack.pop();
            }
            stack.pop();
        }
        else
            postfix += c;
      
    }
   
    while (!stack.empty())
    {
        postfix += stack.top();
        stack.pop();
    }
}