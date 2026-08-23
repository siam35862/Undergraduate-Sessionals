#include <iostream>
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
    a%=mod;

    ll val = bin_exp(a, b / 2, mod);
    val = (ll)(((__int128_t)val * val) % mod);
    if (b % 2 == 0)
        return val;
    else
        return (ll)(((__int128_t)a * val) % mod);
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
    d = (x % phi_of_n + phi_of_n) % phi_of_n;
}

void encrption(ll e, ll n, ll m, ll &e_m)
{
    e_m = bin_exp(m, e, n);
}

void decryption(ll d, ll n, ll e_m, ll &d_m)
{
    d_m = bin_exp(e_m, d, n);
}
int main()
{
    ll d, e, n;
    key_generation(d, e, n);
    cout << endl;
    cout << "RSA Cryptosystem: \n";
    cout << "Private Key: (" << d << ", " << n << ")" << endl;
    cout << "Public Key: (" << e << ", " << n << ")" << endl;
    ll m;
    cout << "Enter the Message: ";
    cin >> m;
    ll e_m, d_m;
    encrption(e, n, m, e_m);
    decryption(d, n, e_m, d_m);

    cout << "Encrypted Message: " << e_m << endl;
    cout << "Decrypted Message:" << d_m << endl;
}