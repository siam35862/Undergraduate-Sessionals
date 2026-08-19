#include <string>
#include <algorithm>
using namespace std;
void removeComments(string &source_program)
{
    string clean_source_program;
    bool flag = true;
    int count = 0;
    for (int i = 0; i < source_program.size(); i++)
    {
        char character = source_program[i];
        char pre_character = ' ';
        if (i > 0)
            pre_character = source_program[i - 1];
        if (character == '"')
        {
            if (flag)
            {
                if (pre_character != '\'')
                    flag = !flag;
            }
            else
            {

                if (pre_character == '\\')
                {
                    if (count % 2 == 0)
                        flag = !flag;
                }
                else
                    flag = !flag;
            }
        }
        if (flag && pre_character == '/' && character == '/')
        {
            clean_source_program.pop_back();

            for (i; i < source_program.size(); i++)
                if (source_program[i] == '\n')
                    break;
        }
        else if (flag && pre_character == '/' && character == '*')
        {
            clean_source_program.pop_back();

            for (i=i+2; i < source_program.size(); i++)
                if (source_program[i] == '/'&&source_program[i-1]=='*')
                    break;
        }
        else
            clean_source_program += character;
        if (character == '\\')
            count++;
        else
            count = 0;
    }
    source_program = clean_source_program;
}
void removeSpaceNewline(string &source_program)
{
    string clean_source_program;
    bool flag = true;
    bool flag2 = true;
    int count = 0;
    for (int i = 0; i < source_program.size(); i++)
    {
        char character = source_program[i];
        char pre_character;
        if (i > 0)
            pre_character = source_program[i - 1];
        if (flag)
        {
            if (flag2 && character == '#')
            {
                if (clean_source_program.size() > 0 && clean_source_program.back() != '\n')
                    clean_source_program += '\n';

                for (i; i < source_program.size(); i++)
                {

                    clean_source_program += source_program[i];
                    if (source_program[i] == '\n')
                    {
                        break;
                    }
                }
                continue;
            }
            if (character == '"')
            {
                if (flag2)
                {
                    if (pre_character != '\'')
                        flag2 = !flag2;
                }
                else
                {

                    if (pre_character == '\\')
                    {
                        if (count % 2 == 0)
                            flag2 = !flag2;
                    }
                    else
                        flag2 = !flag2;
                }
            }
            if (character == ' ' || character == '\n')
            {
                continue;
            }
            else if (character == ';' || character == ')' || character == '}' || character == '(' || character == '{'||character==',')
            {
                flag = true;
            }
            else
            {
                flag = false;
            }

            clean_source_program += character;
        }
        else
        {
            if (flag2 && character == '#')
            {

                for (i; i < source_program.size(); i++)
                {

                    clean_source_program += source_program[i];
                    if (source_program[i] == '\n')
                    {
                        flag = true;
                        break;
                    }
                }
                continue;
            }
            if (character == '"')
            {
                if (flag2)
                {
                    if (pre_character != '\'')
                        flag2 = !flag2;
                }
                else
                {

                    if (pre_character == '\\')
                    {
                        if (count % 2 == 0)
                            flag2 = !flag2;
                    }
                    else
                        flag2 = !flag2;
                }
            }
            if (flag2 && (character == ';' || character == ')' || character == '}' || character == '(' || character == '{'||character==',')||character==' ')
            {
                flag = true;
            }
            if (character == '\n')
            {
                flag = true;
            }

            clean_source_program += character;
        }
        if (character == '\\')
            count++;
        else
            count = 0;
    }
    source_program = clean_source_program;
}
void removeTrailingSpaceNewline(string &source_program)
{
    removeComments(source_program);
    removeSpaceNewline(source_program);
}
