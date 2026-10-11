
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

    ifstream in("regular-expression.txt");
    if (!in)
    {
        cout << "File: regular-expression.txt does not exist.";
    }
    else
    {
        stringstream buffer;
        buffer << in.rdbuf();
        regular_expression = buffer.str();
        in.close();
    }
    ofstream out("nfa.txt");
    ofstream out2("nfa.in");

    regular_expression = addConcatanation(regular_expression);
    out << "Regular Expression: " << regular_expression << endl;

    string postfix_expression;
    infixToPostfix(regular_expression, postfix_expression);
    out << "Postfix Expression: " << postfix_expression << endl;

    NFA nfa = postfix_expression_to_nfa(postfix_expression);
    out << endl;

    out << "NFA Graph:\n";
    out << nfa.start << " " << nfa.end << endl;

    out2 << nfa.start << " " << nfa.end << endl
         << endl;
    for (int i = 0; i <= nfa.end; i++)
    {
        for (auto &[u, w] : nfa.graph[i])
        {

            out << i << "  " << w << "  " << u << endl;
            out2 << i << " " << w << " " << u << endl;
        }
    }
    out2.close();
    out << "\nNFA Table\n";
    out << "Start: " << nfa.start << ", Final: " << nfa.end << endl;
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
        out << "    " << e;
    }
    out << endl;
    for (int i = 0; i <= nfa.end; i++)
    {
        out << i << "   ";
        for (auto e : edges)
        {
            if (table[{i, e}].size() == 0)
            {
                out << "-    ";
            }
            else
            {
                for (auto it : table[{i, e}])
                    out << it << ",";
                out << "   ";
            }
        }
        out << endl;
    }

    out << "Note: ~ used for epsilon.";
    out.close();
    cout << endl
         << "Successfully finished the program.\n";
}