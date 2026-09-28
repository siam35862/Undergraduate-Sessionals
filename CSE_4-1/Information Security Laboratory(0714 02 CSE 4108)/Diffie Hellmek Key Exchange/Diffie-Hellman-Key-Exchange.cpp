#include <iostream>
#include <cstdint>

using namespace std;
#define int long long

int bin_exp(int a, int b, int mod)
{
    if (b == 0)
        return 1;

    int val = bin_exp(a, b / 2, mod);

    if (b % 2 == 0)
        return (1LL * val * val) % mod;
    else
        return (((1LL * val * val) % mod) * a) % mod;
}
int32_t main()
{
    cout << "Diffie Hellman Key Exchange:\n";
    int q, a, xa, xb;
    cout << "Enter the value of q, a, XA, and XB respectively: ";
    cin >> q >> a >> xa >> xb;
    int ya, yb, ka, kb;
    cout << "Global Public Elements:\n";
    cout << "\tPrime Number(q): " << q << endl;
    cout << "\tPrimitive Root(a): " << a << endl;
    cout << "Private Key:\n";
    cout << "\t" << "XA: " << xa << endl;
    cout << "\tXB: " << xb << endl;
    cout << "Public Key:\n";
    cout << "\tYA: " << (ya = bin_exp(a, xa, q)) << endl;
    cout << "\tYB: " << (yb = bin_exp(a, xb, q)) << endl;
    cout << "Secrekt Key:\n";
    cout << "\tKA: " << (ka = bin_exp(yb, xa, q)) << endl;
    cout << "\tKB: " << (kb = bin_exp(ya, xb, q)) << endl;
    cout << "The secret key of both user are " << (ka == kb ? "matched." : "not matched.") << endl;
    return 0;
}