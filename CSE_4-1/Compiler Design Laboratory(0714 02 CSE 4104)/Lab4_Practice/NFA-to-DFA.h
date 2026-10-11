#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include "Regular-Exrepression-To-NFA.h"

struct DFA_State
{
    vector<int> state;
    char state_name;
    DFA_State()
    {
    }
    DFA_State(char ch) : state_name(ch)
    {
    }
    void add(int nfa_state)
    {
        state.push_back(nfa_state);
    }
    bool match(DFA_State dfa_state)
    {
        if (dfa_state.state.size() != state.size())
            return 0;
        else
        {
            for (int i = 0; i < state.size(); i++)
            {
                if (dfa_state.state[i] != state[i])
                    return 0;
            }
            return 1;
        }
    }
    bool operator<(const DFA_State &other) const
    {

        return tie(state, state_name) < tie(other.state, other.state_name);
    }
};
struct DFA
{
    DFA_State start, end;
    map<DFA_State, map<char, DFA_State>> graph;
    vector<DFA_State> states;
    DFA()
    {
    }
    DFA(DFA_State start)
    {
        this->start = start;
        add_state(start);
    }

    DFA_State add_state(DFA_State dfa_state)
    {
        for (auto state : states)
        {
            if (state.match(dfa_state))
                return state;
        }
        states.push_back(dfa_state);
        return states.back();
    }
    void add_edge(DFA_State state1, DFA_State state2, char symbol)
    {
        graph[state1][symbol] = state2;
    }
    void set_end(DFA_State end)
    {
        this->end = end;
    }
};
void find_closure(NFA nfa, int state, set<int> &closures, int *visited)
{
    if (visited[state])
        return;
    visited[state] = 1;
    for (auto p : nfa.graph[state])
    {
        if (p.second == '~')
        {
            closures.insert(p.first);
            find_closure(nfa, p.first, closures, visited);
        }
    }
}
void find_closure(NFA nfa, set<int> &states, set<int> &closures)
{
    int visited[nfa.end + 1] = {};
    for (auto it : states)
    {
        closures.insert(it);
        find_closure(nfa, it, closures, visited);
    }
}
void find_terminals(NFA nfa, vector<char> &terminals)
{
    set<char> terminal;
    for (auto &it : nfa.graph)
    {
        for (auto p : it)
        {
            if (p.second != '~')
                terminal.insert(p.second);
        }
    }
    for (auto it : terminal)
    {
        terminals.push_back(it);
    }
}
DFA nfa_to_dfa(NFA nfa)
{
    int nfa_start = nfa.start;
    set<int> states;
    states.insert(nfa_start);
    set<int> current_closure;
    find_closure(nfa, states, current_closure);

    char state_name = 'A';
    DFA_State current_dfa_state(state_name++);
    for (auto it : current_closure)
    {
        current_dfa_state.add(it);
    }
    DFA dfa(current_dfa_state);
    vector<char> terminals;
    find_terminals(nfa, terminals);
    int ind = 0;

    while (ind < dfa.states.size())
    {
        current_dfa_state = dfa.states[ind];
        ind++;
        for (auto terminal : terminals)
        {
            set<int> new_closure;
            set<int> moved_state;
            for (auto state : current_dfa_state.state)
            {
                for (auto p : nfa.graph[state])
                {
                    if (p.second == terminal)
                    {
                        moved_state.insert(p.first);
                    }
                }
            }
            find_closure(nfa, moved_state, new_closure);
            if (new_closure.size() > 0)
            {
                DFA_State new_dfa_state(state_name++);
                for (auto it : new_closure)
                {
                    new_dfa_state.add(it);
                }

                DFA_State dfa_state = dfa.add_state(new_dfa_state);
                if (dfa_state.state_name != new_dfa_state.state_name)
                {
                    state_name--;
                }
                dfa.add_edge(current_dfa_state, dfa_state, terminal);
            }
        }
    }
    dfa.set_end(current_dfa_state);
    return dfa;
}