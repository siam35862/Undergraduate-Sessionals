
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include "add-concatanation-operator.h"
#include "infix-To-Postfix.h"
using namespace std;
int main()
{
    string regular_expression;

    ifstream file("regular-expression.txt");
    if (!file)
    {
        cout << "File: regular-expression.txt does not exist.";
    }
    else
    {
        stringstream buffer;
        buffer << file.rdbuf();
        regular_expression = buffer.str();
        file.close();
    }
    ofstream file2("postfix.txt");

    regular_expression = addConcatanation(regular_expression);
    file2 << "Regular Expression: " << regular_expression << endl;

    string postfix_expression;
    infixToPostfix(regular_expression, postfix_expression);
    file2 << "Postfix Expression: " << postfix_expression << endl;

   
    cout << endl
         << "Successfully finished the program.\n";
}