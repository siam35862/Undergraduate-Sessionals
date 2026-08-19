#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "CutTrailingSpaceAndNewline.h"
using namespace std;

bool valid_keyword(string &str)
{
    if (str[0] == '_' || (str[0] >= 'A' && str[0] <= 'Z') || (str[0] >= 'a' && str[0] <= 'z'))
    {
        for (auto ch : str)
        {
            if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '_') || ch == '$')
                continue;
            else
                return false;
        }
        return true;
    }
    else
        return false;
}

bool valid_number(string &str)
{
    for (auto ch : str)
    {
        if ((ch >= '0' && ch <= '9'))
            continue;
        else
            return false;
    }
    return true;
}

int main()
{
    int n;
    string source_program;
    string file_name = "input.c";
    if (ifstream file(file_name); file)
    {
        // Alternatives
        //  stringstream buffer;
        //  buffer << file.rdbuf();
        //  source_program = buffer.str();
        //  file.close();

        source_program = {istreambuf_iterator<char>{file}, {}};
    }
    else
        cout << "File does not Exist.";

    removeTrailingSpaceNewline(source_program);

    string output_file_name = "output.c";

    ofstream file(output_file_name);
    file << source_program;
    file.close();
    vector<string> reserved_Keyword = {"void", "int", "float","char","for","if","do","else","while","double","long","return","malloc","calloc","register","free","delete"};
    map<string, int> reserved_Keywords;
    for (auto str : reserved_Keyword)
        reserved_Keywords[str]++;
    vector<string> keywords;
    vector<int> identifiers;

    string new_word = "";
    vector<string> words;
    map<string, int> marked;
    int flag = 1;
    for (int i = 0; i < source_program.size(); i++)
    {
        char ch = source_program[i];
        if (ch == '"')
        {
            flag = !flag;
            continue;
        }
        if (!flag)
        {
            continue;
        }
        if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ',' || ch == ';' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == ' ' || ch == '\n')
        {

            if (new_word.size() > 0 && marked[new_word] == 0)
            {

                marked[new_word] = 1;
                words.push_back(new_word);
                new_word.clear();
            }
            else if (new_word.size() > 0 && marked[new_word] == 1)
                new_word.clear();
        }
        else
            new_word += ch;
    }
    // cout << "Words\n";
    // for (auto str : words)
    //     cout << str << " ";
    // cout << endl;
    string output_file_name2 = "output2.txt";

    ofstream file2(output_file_name2);
    file2 << "Symbol Table\n";
    file2 << "Symbol No:      Symbol:          Type:" << endl;
    int no = 1;
    vector<string> invalid_identifier;
    for (auto word : words)
    {
        if (reserved_Keywords[word] == 1)
        {
            file2 << no << "                " << word << "            " << "Keyword" << endl;
            no++;
        }
        else
        {
            if (valid_keyword(word))
            {
                file2 << no << "                 " << word << "              " << "Identifier" << endl;
                no++;
            }
            else
            {
                if (valid_number(word))
                {
                    file2 << no << "                 " << word << "              " << "Immediate Value" << endl;
                    no++;
                }
                else
                    invalid_identifier.push_back(word);
            }
        }
    }
    file2 << "Invalid Identifiers:\n";
    for (auto invalid : invalid_identifier)
    {
        file2 << invalid << endl;
    }
    file2.close();

    cout << "\nSuccessfully Finished the Program.\n";
    return 0;
}