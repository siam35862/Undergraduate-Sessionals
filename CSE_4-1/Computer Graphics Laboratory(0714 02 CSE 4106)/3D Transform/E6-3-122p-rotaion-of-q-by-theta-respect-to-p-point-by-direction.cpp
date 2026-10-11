#include <bits/stdc++.h>
using namespace std;

vector<vector<double>> mat_mul(vector<vector<double>> A, vector<vector<double>> B)
{

    vector<vector<double>> M(B.size(), vector<double>(B[0].size()));
    ;

    int i, j;
    for (i = 0; i < B.size(); i++)
    {
        for (j = 0; j < B[0].size(); j++)
        {
            M[i][j] = 0;
            for (int k = 0; k < B.size(); k++)
            {
                M[i][j] += A[i][k] * B[i][j];
            }
        }
    }
    return M;
}
vector<vector<double>> translate(vector<double> &p, int sign, vector<vector<double>> &q)
{
    vector<vector<double>> Tp(4, vector<double>(4, 0));
    int i, j;
    for (i = 0; i < 4; i++)
    {
        Tp[i][i] = 1;
    }
    for (i = 0; i < 3; i++)
        Tp[3][i] = sign * p[i];

    return mat_mul(Tp, q);
}
//align and rotation and reverse align implement 
int32_t main()
{
    vector<double> p(3);
    vector<double> v(3);
    vector<vector<double>> q(4, vector<double>(1, 1));
    double theta;
    cout << "Enter the P point: ";
    cin >> p[0] >> p[1] >> p[2];
    cout << "Enter the V vector: ";
    cin >> v[0] >> v[1] >> v[2];
    cout << "Enter the Q point: ";
    cin >> q[0][0] >> q[1][0] >> q[2][0];
    cout << "Enter the theta in degree: ";
    cin >> theta;

    return 0;
}