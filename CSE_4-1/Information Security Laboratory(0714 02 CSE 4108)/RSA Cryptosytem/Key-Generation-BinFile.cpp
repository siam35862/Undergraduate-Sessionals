#include <iostream>
#include <fstream>
#include <random>

using namespace std;
#define ll long long

ll gcd(ll a, ll b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
}

ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

int main()
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
    ll d = (x % phi_of_n + phi_of_n) % phi_of_n;

    // Save Public Key (e, n) to binary file
    ofstream pubFile("public_key.bin", ios::binary);
    if (!pubFile) {
        cerr << "Error writing to public_key.bin\n";
        return 1;
    }
    pubFile.write(reinterpret_cast<const char*>(&e), sizeof(e));
    pubFile.write(reinterpret_cast<const char*>(&n), sizeof(n));
    pubFile.close();

    // Save Private Key (d, n) to binary file
    ofstream privFile("private_key.bin", ios::binary);
    if (!privFile) {
        cerr << "Error writing to private_key.bin\n";
        return 1;
    }
    privFile.write(reinterpret_cast<const char*>(&d), sizeof(d));
    privFile.write(reinterpret_cast<const char*>(&n), sizeof(n));
    privFile.close();

    cout << "\nRSA Cryptosystem Key Generation: \n";
    cout << "Public Key  (e, n): (" << e << ", " << n << ") -> Saved to 'public_key.bin'\n";
    cout << "Private Key (d, n): (" << d << ", " << n << ") -> Saved to 'private_key.bin'\n";

    return 0;
}