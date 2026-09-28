/*
 * Rotation about an arbitrary axis L, specified by a direction vector V and
 * a point P on the axis (Problem 6.3).
 *
 * Method:
 *   1. T_{-P}      : translate P to the origin
 *   2. A_V         : rotate so that V aligns with the z-axis K
 *   3. R_{theta,K} : rotate by theta about K (now easy, since K = z-axis)
 *   4. A_V^{-1}    : undo the alignment rotation
 *   5. T_{P}       : undo the translation
 *
 *   R_{theta,L} = T_P * A_V^{-1} * R_{theta,K} * A_V * T_{-P}
 *
 * All matrices are 4x4 homogeneous transforms, column-vector convention
 * (p' = M * p). Build with: g++ -std=c++17 -O2 rotation_about_arbitrary_axis.cpp -o rotate
 */

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>

// ---------------------------------------------------------------------
// Small 4x4 matrix / 3D vector helpers
// ---------------------------------------------------------------------

using Mat4 = std::array<std::array<double, 4>, 4>;
using Vec3 = std::array<double, 3>;

Mat4 identity() {
    Mat4 M{};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            M[i][j] = (i == j) ? 1.0 : 0.0;
    return M;
}

Mat4 matmul(const Mat4 &A, const Mat4 &B) {
    Mat4 C{};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 4; ++k) sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    return C;
}

// Apply a 4x4 homogeneous transform to a 3D point.
Vec3 apply_transform(const Mat4 &M, const Vec3 &p) {
    double x = p[0], y = p[1], z = p[2], w = 1.0;
    return {M[0][0] * x + M[0][1] * y + M[0][2] * z + M[0][3] * w,
            M[1][0] * x + M[1][1] * y + M[1][2] * z + M[1][3] * w,
            M[2][0] * x + M[2][1] * y + M[2][2] * z + M[2][3] * w};
}

Mat4 transpose(const Mat4 &M) {
    Mat4 T{};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) T[i][j] = M[j][i];
    return T;
}

Vec3 normalize(const Vec3 &v) {
    double n = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return {v[0] / n, v[1] / n, v[2] / n};
}

Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]};
}

double dot(const Vec3 &a, const Vec3 &b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

Vec3 sub(const Vec3 &a, const Vec3 &b) { return {a[0] - b[0], a[1] - b[1], a[2] - b[2]}; }
Vec3 add(const Vec3 &a, const Vec3 &b) { return {a[0] + b[0], a[1] + b[1], a[2] + b[2]}; }
Vec3 scale(const Vec3 &a, double s) { return {a[0] * s, a[1] * s, a[2] * s}; }

void print_mat(const Mat4 &M) {
    std::cout << std::fixed << std::setprecision(4);
    for (int i = 0; i < 4; ++i) {
        std::cout << "  [";
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(8) << M[i][j];
            if (j < 3) std::cout << ",";
        }
        std::cout << " ]\n";
    }
}

void print_vec(const std::string &label, const Vec3 &v) {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << label << " (" << v[0] << ", " << v[1] << ", " << v[2] << ")\n";
}

// ---------------------------------------------------------------------
// Elementary homogeneous transforms
// ---------------------------------------------------------------------

Mat4 translation(const Vec3 &t) {
    Mat4 M = identity();
    M[0][3] = t[0];
    M[1][3] = t[1];
    M[2][3] = t[2];
    return M;
}

Mat4 rotation_x(double angle) {
    Mat4 M = identity();
    double c = std::cos(angle), s = std::sin(angle);
    M[1][1] = c;  M[1][2] = -s;
    M[2][1] = s;  M[2][2] = c;
    return M;
}

Mat4 rotation_y(double angle) {
    Mat4 M = identity();
    double c = std::cos(angle), s = std::sin(angle);
    M[0][0] = c;  M[0][2] = s;
    M[2][0] = -s; M[2][2] = c;
    return M;
}

Mat4 rotation_z(double angle) {
    Mat4 M = identity();
    double c = std::cos(angle), s = std::sin(angle);
    M[0][0] = c;  M[0][1] = -s;
    M[1][0] = s;  M[1][1] = c;
    return M;
}

// ---------------------------------------------------------------------
// A_V : rotates direction V onto +z (K)
// ---------------------------------------------------------------------

Mat4 alignment_matrix(const Vec3 &V_in) {
    Vec3 V = normalize(V_in);
    double a = V[0], b = V[1], c = V[2];

    double d = std::sqrt(b * b + c * c);

    if (d < 1e-12) {
        // V already along +z or -z
        return (c >= 0.0) ? identity() : rotation_x(M_PI);
    }

    double cos_alpha = c / d, sin_alpha = b / d;
    double alpha = std::atan2(sin_alpha, cos_alpha);

    // After Rx(alpha), V becomes (a, 0, d). Ry(beta) must send this to (0,0,1):
    //   x' = a*cos(beta) + d*sin(beta) = 0
    //   z' = -a*sin(beta) + d*cos(beta) = 1
    // solved by cos(beta) = d, sin(beta) = -a.
    double cos_beta = d, sin_beta = -a;
    double beta = std::atan2(sin_beta, cos_beta);

    Mat4 Rx = rotation_x(alpha);
    Mat4 Ry = rotation_y(beta);
    return matmul(Ry, Rx);
}

// ---------------------------------------------------------------------
// The full Problem 6.3 transform (single composed matrix)
// ---------------------------------------------------------------------

Mat4 rotate_about_axis(double theta_deg, const Vec3 &V, const Vec3 &P) {
    double theta = theta_deg * M_PI / 180.0;

    Vec3 negP = {-P[0], -P[1], -P[2]};
    Mat4 T_negP = translation(negP);
    Mat4 T_P = translation(P);

    Mat4 A_V = alignment_matrix(V);
    Mat4 A_V_inv = transpose(A_V);  // rotation matrices are orthogonal

    Mat4 R_theta_K = rotation_z(theta);

    Mat4 R = matmul(T_P, matmul(A_V_inv, matmul(R_theta_K, matmul(A_V, T_negP))));
    return R;
}

// ---------------------------------------------------------------------
// Step-by-step version: prints the matrix AND running point at each stage
// ---------------------------------------------------------------------

Vec3 rotate_about_axis_verbose(double theta_deg, const Vec3 &V, const Vec3 &P,
                                const Vec3 &point) {
    double theta = theta_deg * M_PI / 180.0;
    Vec3 pt = point;

    auto show = [](int step_no, const std::string &label, const Mat4 &M, Vec3 &pt) {
        std::cout << "--- Step " << step_no << ": " << label << " ---\n";
        print_vec("Input point  :", pt);
        std::cout << "Matrix:\n";
        print_mat(M);
        Vec3 out = apply_transform(M, pt);
        print_vec("Output point :", out);
        std::cout << "\n";
        pt = out;
    };

    Vec3 negP = {-P[0], -P[1], -P[2]};
    Mat4 T_negP = translation(negP);
    show(1, "T_-P  (translate P to origin)", T_negP, pt);

    Mat4 A_V = alignment_matrix(V);
    show(2, "A_V   (align V with z-axis K)", A_V, pt);

    Mat4 R_theta_K = rotation_z(theta);
    show(3, "R_theta,K (rotate " + std::to_string(theta_deg) + " deg about K)",
         R_theta_K, pt);

    Mat4 A_V_inv = transpose(A_V);
    show(4, "A_V^-1 (undo alignment)", A_V_inv, pt);

    Mat4 T_P = translation(P);
    show(5, "T_P   (translate back)", T_P, pt);

    std::cout << "Final rotated point: ";
    print_vec("", pt);
    return pt;
}

// ---------------------------------------------------------------------
// Independent cross-check: Rodrigues' rotation formula about axis L
// ---------------------------------------------------------------------

Vec3 rotate_about_axis_rodrigues(double theta_deg, const Vec3 &V_in,
                                  const Vec3 &P, const Vec3 &point) {
    double theta = theta_deg * M_PI / 180.0;
    Vec3 V = normalize(V_in);

    Vec3 r = sub(point, P);
    Vec3 term1 = scale(r, std::cos(theta));
    Vec3 term2 = scale(cross(V, r), std::sin(theta));
    Vec3 term3 = scale(V, dot(V, r) * (1 - std::cos(theta)));

    Vec3 r_rot = add(add(term1, term2), term3);
    return add(P, r_rot);
}

// ---------------------------------------------------------------------
// Demo: theta = 45 degrees, same numbers as the Python version
// ---------------------------------------------------------------------

int main() {
    Vec3 V = {1.0, 1.0, 1.0};   // arbitrary axis direction
    Vec3 P = {2.0, 1.0, 0.0};   // arbitrary point on the axis
    double theta_deg = 45.0;
    Vec3 Q = {5.0, 2.0, 1.0};   // point to rotate (not on the axis)

    std::cout << "============================================================\n";
    std::cout << "STEP-BY-STEP (prints matrix + point after each stage)\n";
    std::cout << "============================================================\n";
    Vec3 Q_verbose = rotate_about_axis_verbose(theta_deg, V, P, Q);

    std::cout << "============================================================\n";
    std::cout << "ONE-SHOT (single composed matrix, for comparison)\n";
    std::cout << "============================================================\n";
    Mat4 R = rotate_about_axis(theta_deg, V, P);
    Vec3 Q_matrix = apply_transform(R, Q);
    std::cout << "Composed transform R_theta,L =\n";
    print_mat(R);
    print_vec("Q rotated (composed matrix) =", Q_matrix);

    Vec3 Q_rodrigues = rotate_about_axis_rodrigues(theta_deg, V, P, Q);
    print_vec("Q rotated (Rodrigues check) =", Q_rodrigues);

    auto close = [](const Vec3 &a, const Vec3 &b) {
        for (int i = 0; i < 3; ++i)
            if (std::abs(a[i] - b[i]) > 1e-6) return false;
        return true;
    };

    bool all_match = close(Q_verbose, Q_matrix) && close(Q_matrix, Q_rodrigues);
    std::cout << "\nAll three methods match: " << (all_match ? "true" : "false") << "\n";

    return 0;
}
