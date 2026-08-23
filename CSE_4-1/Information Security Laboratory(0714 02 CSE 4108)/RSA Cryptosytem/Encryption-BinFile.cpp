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
    // 1. Read Public Key (e, n) from public_key.bin
    ifstream pubFile("public_key.bin", ios::binary);
    if (!pubFile)
    {
        cerr << "Error: 'public_key.bin' not found.\n";
        return 1;
    }

    ll e, n;
    pubFile.read(reinterpret_cast<char*>(&e), sizeof(e));
    pubFile.read(reinterpret_cast<char*>(&n), sizeof(n));
    pubFile.close();

    cout << "\nRSA Cryptosystem Encryption:\n";
    cout << "Loaded Public Key (e, n): (" << e << ", " << n << ")\n";

    // 2. Read message string from message.txt
    ifstream msgFile("message.txt");
    if (!msgFile)
    {
        cerr << "Error: 'message.txt' not found.\n";
        return 1;
    }

    string message((istreambuf_iterator<char>(msgFile)), istreambuf_iterator<char>());
    msgFile.close();

    // 3. Encrypt string and write binary output to encrypted.bin
    ofstream outFile("encrypted.bin", ios::binary);
    if (!outFile)
    {
        cerr << "Error creating 'encrypted.bin'\n";
        return 1;
    }

    for (char c : message)
    {
        ll m = static_cast<unsigned char>(c);
        ll cipher = bin_exp(m, e, n);
        outFile.write(reinterpret_cast<const char*>(&cipher), sizeof(cipher));
    }
    outFile.close();

    cout << "Message from 'message.txt' encrypted successfully -> Saved to 'encrypted.bin'\n";

    return 0;
}