#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "add-concatanation-operator.h"
#include "infix-To-Postfix.h"
using namespace std;

struct NFA
{

    int start;
    int end;

    vector<vector<pair<int, char>>> graph;

    NFA()
    {
        graph.resize(1000);
    }
    NFA(int s, int e, char ch) : NFA()
    {

        start = s;
        end = e;
        graph[s].push_back({e, ch});
    }
    NFA(int s, int e) : NFA()
    {

        start = s;
        end = e;
    }
    void merge(NFA nfa1, NFA nfa2)
    {
        for (int i = 0; i <= nfa1.end; i++)
        {
            for (auto it : nfa1.graph[i])
            {
                graph[i].push_back(it);
            }
        }
        for (int i = 0; i <= nfa2.end; i++)
        {
            for (auto it : nfa2.graph[i])
            {
                graph[i].push_back(it);
            }
        }
    }
    void merge(NFA nfa)
    {
        for (int i = 0; i <= nfa.end; i++)
        {
            for (auto it : nfa.graph[i])
            {
                graph[i].push_back(it);
            }
        }
    }
    void kleeneStar(NFA nfa)
    {
        merge(nfa);
        graph[start].push_back({nfa.start, '~'});
        graph[start].push_back({end, '~'});
        graph[nfa.end].push_back({nfa.start, '~'});
        graph[nfa.end].push_back({end, '~'});
    }
    void positiveClosure(NFA nfa)
    {
        merge(nfa);
        graph[start].push_back({nfa.start, '~'});
        graph[nfa.end].push_back({nfa.start, '~'});
        graph[nfa.end].push_back({end, '~'});
    }
    void optional(NFA nfa)
    {
        merge(nfa);
        graph[start].push_back({nfa.start, '~'});
        graph[start].push_back({end, '~'});
        graph[nfa.end].push_back({end, '~'});
    }
    void concatanate(NFA nfa1, NFA nfa2)
    {
        start = nfa1.start;
        end = nfa2.end;
        merge(nfa1, nfa2);
        graph[nfa1.end].push_back({nfa2.start, '~'});
    }
    void OR(NFA nfa1, NFA nfa2)
    {
        merge(nfa1, nfa2);
        graph[start].push_back({nfa1.start, '~'});
        graph[start].push_back({nfa2.start, '~'});
        graph[nfa1.end].push_back({end, '~'});
        graph[nfa2.end].push_back({end, '~'});
    }
};

void kleeneStar(stack<NFA> &nfas, int &state)
{
    NFA nfa;
    if (nfas.size() > 0)
    {
        nfa = nfas.top();
        nfas.pop();
    }
    else
        return;
    NFA new_nfa = NFA(state, state + 1);
    state += 2;
    new_nfa.kleeneStar(nfa);

    nfas.push(new_nfa);
}
void positiveClosure(stack<NFA> &nfas, int &state)
{
    NFA nfa;
    if (nfas.size() > 0)
    {
        nfa = nfas.top();
        nfas.pop();
    }
    else
        return;
    NFA new_nfa = NFA(state, state + 1);
    state += 2;
    new_nfa.positiveClosure(nfa);
    nfas.push(new_nfa);
}
void optional(stack<NFA> &nfas, int &state)
{
    NFA nfa;
    if (nfas.size() > 0)
    {
        nfa = nfas.top();
        nfas.pop();
    }
    else
        return;
    NFA new_nfa = NFA(state, state + 1);
    state += 2;
    new_nfa.optional(nfa);
    nfas.push(new_nfa);
}
void concatanate(stack<NFA> &nfas, int &state)
{

    NFA nfa1, nfa2;
    if (nfas.size() > 1)
    {
        nfa2 = nfas.top();
        nfas.pop();
        nfa1 = nfas.top();
        nfas.pop();
    }
    else
        return;
    NFA new_nfa = NFA();
    new_nfa.concatanate(nfa1, nfa2);
    nfas.push(new_nfa);
}
void OR(stack<NFA> &nfas, int &state)
{
    NFA nfa1, nfa2;
    if (nfas.size() > 1)
    {
        nfa2 = nfas.top();
        nfas.pop();
        nfa1 = nfas.top();
        nfas.pop();
    }
    else
        return;
    NFA new_nfa = NFA(state, state + 1);
    state += 2;
    new_nfa.OR(nfa1, nfa2);
    nfas.push(new_nfa);
}
NFA postfix_expression_to_nfa(string &expression)
{

    stack<NFA> nfas;

    int state = 0;
    for (auto ch : expression)
    {
        if (ch == '*')
        {
            kleeneStar(nfas, state);
        }
        else if (ch == '+')
        {
            positiveClosure(nfas, state);
        }
        else if (ch == '?')
        {
            optional(nfas, state);
        }
        else if (ch == '.')
        {
            concatanate(nfas, state);
        }
        else if (ch == '|')
        {
            OR(nfas, state);
        }
        else
        {

            NFA new_nfa = NFA(state, state + 1, ch);
            nfas.push(new_nfa);
            state += 2;
        }
    }
    if (nfas.size() == 0)
        return NFA();
    else
        return nfas.top();
}
