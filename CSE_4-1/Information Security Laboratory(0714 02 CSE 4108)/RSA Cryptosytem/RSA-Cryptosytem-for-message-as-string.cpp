#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;
#define ll long long

ll gcd(ll a, ll b)
{
    if (b == 0)
        return a;
    else
        return gcd(b, a % b);
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

void key_generation(ll &d, ll &e, ll &n)
{
    random_device rand;
    mt19937_64 random_generator(rand());
    uniform_int_distribution<ll> range(1e8, 2e9);

    ll p = 1999999943, q = 1999999973;
    n = p * q;
    ll phi_of_n = (p - 1) * (q - 1);

    e = range(random_generator);
    while (gcd(e, phi_of_n) != 1)
    {
        e = range(random_generator);
    }

    ll x, y;
    extgcd(e, phi_of_n, x, y);
    ll factor = abs(x / phi_of_n);
    d = (x + (factor + 1) * phi_of_n) % phi_of_n;
}

// Encrypts each character's ASCII value
vector<ll> encryption(ll e, ll n, const string &msg)
{
    vector<ll> encrypted_msg;
    for (char c : msg)
    {
        ll m = static_cast<ll>(c);
        encrypted_msg.push_back(bin_exp(m, e, n));
    }
    return encrypted_msg;
}

// Decrypts each block back to ASCII and reconstructs the string
string decryption(ll d, ll n, const vector<ll> &e_m)
{
    string decrypted_msg = "";
    for (ll cipher : e_m)
    {
        ll m = bin_exp(cipher, d, n);
        decrypted_msg += static_cast<char>(m);
    }
    return decrypted_msg;
}

int main()
{
    ll d, e, n;
    key_generation(d, e, n);

    cout << "\nRSA Cryptosystem: \n";
    cout << "Private Key: (" << d << ", " << n << ")\n";
    cout << "Public Key: (" << e << ", " << n << ")\n\n";

    string msg;
    cout << "Enter the Message: ";
    getline(cin, msg); // Use getline to capture spaces in text

    // Encrypt
    vector<ll> e_m = encryption(e, n, msg);

    cout << "\nEncrypted Message (Ciphertext Array):\n";
    for (ll val : e_m)
    {
        cout << val << " ";
    }
    cout << endl;

    // Decrypt
    string d_m = decryption(d, n, e_m);
    cout << "\nDecrypted Message:\n" << d_m << endl;

    return 0;
}