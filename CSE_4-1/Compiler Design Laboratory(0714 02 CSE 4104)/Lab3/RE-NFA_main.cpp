
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include "Regular-Exrepression-To-NFA.h"
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
    ofstream file2("nfa.txt");

    regular_expression = addConcatanation(regular_expression);
    file2 << "Regular Expression: " << regular_expression << endl;

    string postfix_expression;
    infixToPostfix(regular_expression, postfix_expression);
    file2 << "Postfix Expression: " << postfix_expression << endl;

    NFA nfa = postfix_expression_to_nfa(postfix_expression);
    file2 << endl;
    file2 << "NFA Graph/Table\n";
    file2 << "Start: " << nfa.start << ", Final: " << nfa.end << endl;
    set<char> st;
    for (auto it : postfix_expression)
    {
        if (isalnum(it))
            st.insert(it);
    }
    vector<char> edges;
    for (auto e : st)
        edges.push_back(e);
    edges.push_back('~');

    map<pair<int, char>, vector<int>> table;

    for (int i = 0; i <= nfa.end; i++)
    {
        for (auto &[u, w] : nfa.graph[i])
        {
            table[{i, w}].push_back(u);
        }
    }
    for (auto e : edges)
    {
        file2 << "    " << e;
    }
    file2 << endl;
    for (int i = 0; i <= nfa.end; i++)
    {
        file2 << i << "   ";
        for (auto e : edges)
        {
            if (table[{i, e}].size() == 0)
            {
                file2 << "-    ";
            }
            else
            {
                for (auto it : table[{i, e}])
                    file2 << it << ",";
                file2<<"   ";
            }
        }
        file2 << endl;
    }

    file2 << "Note: ~ used for epsilon.";
    file2.close();
    cout << endl
         << "Successfully finished the program.\n";
}