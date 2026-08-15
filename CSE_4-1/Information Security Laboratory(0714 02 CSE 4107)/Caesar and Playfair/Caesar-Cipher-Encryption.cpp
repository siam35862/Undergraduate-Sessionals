#include <iostream>
#include <string>
using namespace std;

string caesarEncrypt(string &plaintext)
{
    string caesar_cipher;

    for (char character : plaintext)
    {
        char cipher_character;
        if (character < 'A' || character > 'z' || (character > 'Z' && character < 'a'))
            cipher_character = character;
        else if (character < 'a')
        {
            cipher_character = (((character - 'A' + 3) % 26) + 'A')^32;
        }
        else
        {
            cipher_character = (((character - 'a' + 3) % 26) + 'a')^32;
        }
        
        caesar_cipher.push_back(cipher_character);
    }
    return caesar_cipher;
}

int main()
{
    cout<<endl<<"Caesar Cipher Encryption:-\n";
    string plaintext;
    cout << "Enter the Plaintext: ";
    getline(cin, plaintext);

    string caesar_cipher = caesarEncrypt(plaintext);

    cout << "Encrypted Caesar Cipher: " << caesar_cipher << endl;

    return 0;
}