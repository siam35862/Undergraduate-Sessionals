#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

string playfairEncrypt(string &keyword, string &plaintext)
{
    string temp_keyword, temp_plaintext;
    for (char character : keyword)
    {
        if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z'))
            temp_keyword.push_back(character);
    }
    for (char character : plaintext)
    {
        if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z'))
            temp_plaintext.push_back(character);
    }

    string caeser_cipher;

    vector<vector<char>> square_matrix(5, vector<char>(5));
    int r = 0, c = 0;
    map<char, pair<int, int>> mp;

    for (char character : keyword)
    {
        if (character > 'Z')
            character ^= 32;
        if (mp.find(character) == mp.end())
        {
            square_matrix[r][c] = character;

            if (character == 'I' || character == 'J')
            {
                mp['I'] = {r, c};
                mp['J'] = {r, c};
            }
            else
                mp[character] = {r, c};
            c++;
            if (c == 5)
            {
                r++;
                c = 0;
            }
        }
    }

    for (char character = 'A'; character <= 'Z'; character += 1)
    {

        if (mp.find(character) == mp.end())
        {
            square_matrix[r][c] = character;
            if (character == 'I' || character == 'J')
            {
                square_matrix[r][c] = 'I';
                mp['I'] = {r, c};
                mp['J'] = {r, c};
            }
            else
                mp[character] = {r, c};
            c++;
            if (c == 5)
            {
                r++;
                c = 0;
            }
        }
    }

    "The square matrix from the given keyword: \n";
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
            if (square_matrix[i][j] == 'I' || square_matrix[i][j] == 'J')
                cout << "IJ ";
            else
                cout << square_matrix[i][j] << ' ';
        cout << endl;
    }

    string message;
    int i;
    for (i = 1; i < plaintext.size(); i++)
    {
        if (plaintext[i] < 'a')
            plaintext[i] ^= 32;
        message.push_back(plaintext[i - 1]);
        if (plaintext[i] == plaintext[i - 1])
        {
            if (plaintext[i] == 'x')
            {
                message.push_back('q');
            }
            else
                message.push_back('x');
        }
        else
        {
            message.push_back(plaintext[i]);
            i++;
        }
    }
    if (i == plaintext.size())
    {
        message.push_back(plaintext[i - 1]);
        if (plaintext[i - 1] == 'x')
        {
            message.push_back('q');
        }
        else
            message.push_back('x');
    }

    cout << "Message: ";
    for (i = 0; i < message.size(); i += 2)
    {
        cout << message[i] << message[i + 1] << " ";
    }
    cout << endl;
    for (i = 0; i < message.size(); i += 2)
    {
        if (mp[(message[i] ^ 32)].first == mp[(message[i + 1] ^ 32)].first)
        {
            r = mp[(message[i] ^ 32)].first;
            int c1 = mp[(message[i] ^ 32)].second;
            int c2 = mp[(message[i + 1] ^ 32)].second;
            c1 = (c1 + 1) % 5;
            c2 = (c2 + 1) % 5;
            caeser_cipher.push_back(square_matrix[r][c1]);
            caeser_cipher.push_back(square_matrix[r][c2]);
        }
        else if (mp[(message[i] ^ 32)].second == mp[(message[i + 1] ^ 32)].second)
        {
            c = mp[(message[i] ^ 32)].second;
            int r1 = mp[(message[i] ^ 32)].first;
            int r2 = mp[(message[i + 1] ^ 32)].first;
            r1 = (r1 + 1) % 5;
            r2 = (r2 + 1) % 5;
            caeser_cipher.push_back(square_matrix[r1][c]);
            caeser_cipher.push_back(square_matrix[r2][c]);
        }
        else
        {
            int r1 = mp[(message[i] ^ 32)].first;
            int r2 = mp[(message[i + 1] ^ 32)].first;
            int c1 = mp[(message[i] ^ 32)].second;
            int c2 = mp[(message[i + 1] ^ 32)].second;

            caeser_cipher.push_back(square_matrix[r1][c2]);
            caeser_cipher.push_back(square_matrix[r2][c1]);
        }
    }
    return caeser_cipher;
}

int main()
{
    cout << endl
         << "Playfair Cipher Encryption:-\n";
    string keyword;
    cout << "Enter the Keyword: ";
    getline(cin, keyword);
    string plaintext;
    cout << "Enter the Plaintext: ";
    getline(cin, plaintext);

    string playfair_cipher = playfairEncrypt(keyword, plaintext);

    cout << "Ciphertext: ";

    for (int i = 0; i < playfair_cipher.size(); i += 2)
    {
        cout << playfair_cipher[i] << playfair_cipher[i + 1] << " ";
    }
    cout << endl;

    return 0;
}