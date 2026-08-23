#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>

using namespace std;
#define ll long long

ll gcd(ll a, ll b)
{
    return (b == 0) ? a : gcd(b, a % b);
}

ll extgcd(ll a, ll b, ll &x, ll &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

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
        return ((ll)(((__int128_t)a * val) % mod)) % mod;
}

void generate_and_save_keys()
{
    random_device rand;
    mt19937_64 random_generator(rand());
    uniform_int_distribution<ll> range(1e8, 2e9);

    ll p = 1999999943, q = 1999999973;
    ll n = p * q;
    ll phi_of_n = (p - 1) * (q - 1);

    ll e = range(random_generator);
    while (gcd(e, phi_of_n) != 1)
    {
        e = range(random_generator);
    }

    ll x, y;
    extgcd(e, phi_of_n, x, y);
    ll factor = abs(x / phi_of_n);
    ll d = (x + (factor + 1) * phi_of_n) % phi_of_n;

    // Save Public Key (e, n)
    ofstream pubFile("public_key.bin", ios::binary);
    if (!pubFile)
    {
        cerr << "Error writing to public_key.bin\n";
        return;
    }
    pubFile.write(reinterpret_cast<const char*>(&e), sizeof(e));
    pubFile.write(reinterpret_cast<const char*>(&n), sizeof(n));
    pubFile.close();

    // Save Private Key (d, n)
    ofstream privFile("private_key.bin", ios::binary);
    if (!privFile)
    {
        cerr << "Error writing to private_key.bin\n";
        return;
    }
    privFile.write(reinterpret_cast<const char*>(&d), sizeof(d));
    privFile.write(reinterpret_cast<const char*>(&n), sizeof(n));
    privFile.close();

    cout << "[1] Keys generated and saved to 'public_key.bin' and 'private_key.bin'\n";
    cout << "    Public Key  (e, n): (" << e << ", " << n << ")\n";
    cout << "    Private Key (d, n): (" << d << ", " << n << ")\n\n";
}

void encrypt_file()
{
    // Read Public Key from binary file
    ifstream pubFile("public_key.bin", ios::binary);
    if (!pubFile)
    {
        cerr << "Error: 'public_key.bin' not found.\n";
        return;
    }
    ll e, n;
    pubFile.read(reinterpret_cast<char*>(&e), sizeof(e));
    pubFile.read(reinterpret_cast<char*>(&n), sizeof(n));
    pubFile.close();

    ifstream inFile("input.txt");
    if (!inFile)
    {
        cerr << "Error: 'input.txt' not found.\n";
        return;
    }
    string message((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Write ciphertext to binary file
    ofstream outFile("encrypted.bin", ios::binary);
    if (!outFile)
    {
        cerr << "Error creating 'encrypted.bin'\n";
        return;
    }

    for (char c : message)
    {
        ll m = static_cast<unsigned char>(c);
        ll cipher = bin_exp(m, e, n);
        outFile.write(reinterpret_cast<const char*>(&cipher), sizeof(cipher));
    }
    outFile.close();

    cout << "[2] 'input.txt' encrypted using public key -> saved to 'encrypted.bin'\n\n";
}

void decrypt_file()
{
    // Read Private Key from binary file
    ifstream privFile("private_key.bin", ios::binary);
    if (!privFile)
    {
        cerr << "Error: 'private_key.bin' not found.\n";
        return;
    }
    ll d, n;
    privFile.read(reinterpret_cast<char*>(&d), sizeof(d));
    privFile.read(reinterpret_cast<char*>(&n), sizeof(n));
    privFile.close();

    // Read ciphertext from binary file
    ifstream cipherFile("encrypted.bin", ios::binary);
    if (!cipherFile)
    {
        cerr << "Error: 'encrypted.bin' not found.\n";
        return;
    }

    string decrypted_msg = "";
    ll cipher;
    while (cipherFile.read(reinterpret_cast<char*>(&cipher), sizeof(cipher)))
    {
        ll m = bin_exp(cipher, d, n);
        decrypted_msg += static_cast<char>(m);
    }
    cipherFile.close();

    ofstream outFile("decrypted.txt");
    if (!outFile)
    {
        cerr << "Error creating 'decrypted.txt'\n";
        return;
    }

    outFile << decrypted_msg;
    outFile.close();

    cout << "[3] 'encrypted.bin' decrypted using private key -> saved to 'decrypted.txt'\n\n";
}

int main()
{
    generate_and_save_keys();
    encrypt_file();
    decrypt_file();

    cout << "Workflow completed successfully.\n";
    return 0;
}