#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#define ll long long

ll bin_exp(ll a, ll b, ll mod)
{
    if (b == 0)
        return 1;
    a %= mod;

    ll val = bin_exp(a, b / 2, mod);
    val = (ll)(((__int128_t)val * val) % mod);
    if (b % 2 == 0)
        return val;
    else
        return (ll)(((__int128_t)a * val) % mod);
}

int main()
{
    // 1. Read Private Key (d, n) from private_key.bin
    ifstream privFile("private_key.bin", ios::binary);
    if (!privFile)
    {
        cerr << "Error: 'private_key.bin' not found.\n";
        return 1;
    }

    ll d, n;
    privFile.read(reinterpret_cast<char*>(&d), sizeof(d));
    privFile.read(reinterpret_cast<char*>(&n), sizeof(n));
    privFile.close();

    cout << "\nRSA Cryptosystem Decryption:\n";
    cout << "Loaded Private Key (d, n): (" << d << ", " << n << ")\n";

    // 2. Read encrypted binary data from encrypted.bin
    ifstream cipherFile("encrypted.bin", ios::binary);
    if (!cipherFile)
    {
        cerr << "Error: 'encrypted.bin' not found.\n";
        return 1;
    }

    string decrypted_msg = "";
    ll cipher;

    // Read sizeof(ll) bytes sequentially until EOF
    while (cipherFile.read(reinterpret_cast<char*>(&cipher), sizeof(cipher)))
    {
        ll m = bin_exp(cipher, d, n);
        decrypted_msg += static_cast<char>(m);
    }
    cipherFile.close();

    // 3. Save original text to decrypted.txt
    ofstream outFile("decrypted.txt");
    if (!outFile)
    {
        cerr << "Error creating 'decrypted.txt'\n";
        return 1;
    }

    outFile << decrypted_msg;
    outFile.close();

    cout << "Decrypted Message: " << decrypted_msg << "\n";
    cout << "Saved successfully to 'decrypted.txt'\n";

    return 0;
}