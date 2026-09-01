#include <iostream>
#include <string>
#include <stack>

class InfixToPostfix {
public:
    static std::string convertToPostfix(const std::string& expression) {
        std::stack<char> stack;
        const std::string opening = "({[";
        const std::string closing = ")}]";
        const std::string operator1 = "+-*/";
        const std::string operator2 = "-+/*";
        std::string postFix = "";

        for (char c : expression) {
            if (opening.find(c) != std::string::npos) {
                stack.push(c);
            }
            else if (operator1.find(c) != std::string::npos) {
                while (!stack.empty() && opening.find(stack.top()) == std::string::npos && 
                       (operator1.find(c) <= operator1.find(stack.top()) || 
                        operator2.find(c) <= operator2.find(stack.top()))) {
                    postFix += stack.top();
                    stack.pop();
                }
                stack.push(c);
            }
            else if (closing.find(c) != std::string::npos) {
                while (closing.find(c) != opening.find(stack.top())) {
                    postFix += stack.top();
                    stack.pop();
                }
                stack.pop(); // Remove matching opening bracket
            }
            else {
                postFix += c;
            }
        }

        while (!stack.empty()) {
            postFix += stack.top();
            stack.pop();
        }

        return postFix;
    }
};

int main() {
    std::string expression = "abcd+++";
    std::string result = InfixToPostfix::convertToPostfix(expression);
    std::cout << "PostFix: " << result << std::endl;
    return 0;
}