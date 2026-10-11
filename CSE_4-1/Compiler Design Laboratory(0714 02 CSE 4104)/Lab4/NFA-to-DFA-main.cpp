
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include "NFA-to-DFA.h"
using namespace std;
int main()
{
    ofstream out("dfa.txt");

    int type;

    cout << "Type 0 for RE to NFA to DFA or type any key for NFA to DFA: ";
    cin >> type;
    NFA nfa;
    if (type)
    {
        string nfa_filename = "nfa.in";
        nfa = build_nfa(nfa_filename);
    }
    else
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

        regular_expression = addConcatanation(regular_expression);
        out << "Regular Expression: " << regular_expression << endl;

        string postfix_expression;
        infixToPostfix(regular_expression, postfix_expression);
        out << "Postfix Expression: " << postfix_expression << endl;

        nfa = postfix_expression_to_nfa(postfix_expression);
    }
    out << "NFA Graph:\n";
    out << "Start: " << nfa.start << ", End: " << nfa.end << endl
        << endl;
    for (int i = 0; i <= nfa.end; i++)
    {
        for (auto &[u, w] : nfa.graph[i])
        {

            out << i << "  " << w << "  " << u << endl;
        }
    }
    cout << endl;

    out << endl
        << "DFA: " << endl;
    DFA dfa = nfa_to_dfa(nfa);

    out << "The states are: " << endl;
    for (auto state : dfa.states)
    {
        out << state.state_name << " = { ";
        for (auto it : state.state)
            out << it << ", ";
        out << "}\n";
    }
    out << "Start: " << dfa.start.state_name << endl;
    out << "Final: " << dfa.end.state_name << endl;

    for (auto it : dfa.graph)
    {
        for (auto p : it.second)
        {
            out << it.first.state_name << " " << p.first << " " << p.second.state_name << endl;
        }
    }
    cout << endl
         << "Successfully finished the program.\n";
}