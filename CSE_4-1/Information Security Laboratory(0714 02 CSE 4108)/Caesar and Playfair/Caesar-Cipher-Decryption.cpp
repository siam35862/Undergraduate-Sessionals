#include <iostream>
#include <string>
using namespace std;

string caesarDecrypt(string &caesar_cipher)
{
    string plaintext;

    for (char cipher_character : caesar_cipher)
    {
        char character;
        if (cipher_character < 'A' || cipher_character > 'z' || (cipher_character > 'Z' && cipher_character < 'a'))
            character = cipher_character;
        else if (cipher_character < 'a')
        {
            character = (((cipher_character - 'A'- 3+26) % 26) + 'A')^32;
        }
        else
        {
            character = (((cipher_character - 'a'- 3+26) % 26) + 'a')^32;
        }
        
        plaintext.push_back(character);
    }
    return plaintext;
}

int main()
{
    cout<<endl<<"Caesar Cipher Decryption:-\n";
    string caesar_cipher;
    cout << "Enter the Caesar Cipher: ";
    getline(cin, caesar_cipher);

    string plaintext = caesarDecrypt(caesar_cipher);

    cout << "Decrypted Plaintext: " << plaintext << endl;

    return 0;
}