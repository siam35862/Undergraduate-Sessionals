#include <iostream>

using namespace std;
#define ll long long
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

int main()
{
    ll d,n;
    cout<<endl;
    cout<<"RSA Cryptosystem Decryption: \n";
    cout<<"Enter the private key(d,n): ";
    cin>>d>>n;

    ll e_m;
    cout<<"Enter the encrypted message: ";
    cin>>e_m;

    ll m=bin_exp(e_m,d,n);

    cout<<"Decrypted Original Message: "<<m<<endl;


}