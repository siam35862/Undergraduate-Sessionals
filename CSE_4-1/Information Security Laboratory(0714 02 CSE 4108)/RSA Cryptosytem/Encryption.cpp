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
    ll e,n;
    cout<<endl;
    cout<<"RSA Cryptosystem Encryption: \n";
    cout<<"Enter the public key(e,n): ";
    cin>>e>>n;

    ll m;
    cout<<"Enter the message: ";
    cin>>m;

    ll e_m=bin_exp(m,e,n);

    cout<<"Encrypted Message: "<<e_m<<endl;
    
   


}