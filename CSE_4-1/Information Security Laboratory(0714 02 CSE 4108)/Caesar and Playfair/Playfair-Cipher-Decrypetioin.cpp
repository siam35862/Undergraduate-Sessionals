#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

string playfairEncrypt(string &keyword, string &ciphertext)
{
    string temp_keyword, temp_ciphertext;
    for (char character : keyword)
    {
        if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z'))
            temp_keyword.push_back(character);
    }
    for (char character : ciphertext)
    {
        if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z'))
            temp_ciphertext.push_back(character);
    }
    keyword = temp_keyword;
    ciphertext = temp_ciphertext;

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

    int i;

    string message;
    for (i = 0; i < ciphertext.size(); i += 2)
    {

        if (ciphertext[i] > 'Z')
            ciphertext[i] ^= 32;
        if (ciphertext[i + 1] > 'Z')
            ciphertext[i + 1] ^= 32;

        if (mp[ciphertext[i]].first == mp[ciphertext[i + 1]].first)
        {
            r = mp[ciphertext[i]].first;
            int c1 = mp[ciphertext[i]].second;
            int c2 = mp[ciphertext[i + 1]].second;
            c1 = (c1 - 1 + 5) % 5;
            c2 = (c2 - 1 + 5) % 5;
            message.push_back(square_matrix[r][c1]);
            message.push_back(square_matrix[r][c2]);
        }
        else if (mp[ciphertext[i]].second == mp[ciphertext[i + 1]].second)
        {
            c = mp[ciphertext[i]].second;
            int r1 = mp[ciphertext[i]].first;
            int r2 = mp[ciphertext[i + 1]].first;
            r1 = (r1 - 1 + 5) % 5;
            r2 = (r2 - 1 + 5) % 5;
            message.push_back(square_matrix[r1][c]);
            message.push_back(square_matrix[r2][c]);
        }
        else
        {
            int r1 = mp[ciphertext[i]].first;
            int r2 = mp[ciphertext[i + 1]].first;
            int c1 = mp[ciphertext[i]].second;
            int c2 = mp[ciphertext[i + 1]].second;

            message.push_back(square_matrix[r1][c2]);
            message.push_back(square_matrix[r2][c1]);
        }
    }
    cout << "Message: ";
    for (i = 0; i < message.size(); i += 2)
    {
        if (message[i] < 'a')
            message[i] ^= 32;
        if (message[i + 1] < 'a')
            message[i + 1] ^= 32;
        cout << message[i] << message[i + 1] << " ";
    }
    cout << endl;

   

    return message;
}

int main()
{
    cout << endl
         << "Playfair Cipher Decryption:-\n";
    string keyword;
    cout << "Enter the Keyword: ";
    getline(cin, keyword);
    string ciphertext;
    cout << "Enter the ciphertext: ";
    getline(cin, ciphertext);

    string plaintext = playfairEncrypt(keyword, ciphertext);

    cout << "Original Plaintext: "<<plaintext<<endl;

   
    

    return 0;
}