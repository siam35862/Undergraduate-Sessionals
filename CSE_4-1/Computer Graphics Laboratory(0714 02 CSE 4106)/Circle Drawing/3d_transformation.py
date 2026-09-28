"""
Rotation about an arbitrary axis L, specified by a direction vector V and a
point P on the axis (Problem 6.3).

Method (matches the textbook derivation exactly):
    1. T_{-P}      : translate P to the origin
    2. A_V         : rotate so that V aligns with the z-axis K
    3. R_{theta,K} : rotate by theta about K (now easy, since K = z-axis)
    4. A_V^{-1}    : undo the alignment rotation
    5. T_{P}       : undo the translation

    R_{theta,L} = T_P . A_V^{-1} . R_{theta,K} . A_V . T_{-P}

All matrices are 4x4 homogeneous transforms, using column vectors
(p' = M @ p), which is the standard convention in most graphics texts.

A_V itself is built from two elementary rotations (the standard
Rogers/Adams construction referenced as "Prob. 6.2"):
    - Rx(alpha): rotates V into the x-z plane
    - Ry(beta):  rotates the result onto the z-axis
    A_V = Ry(beta) . Rx(alpha)
"""

import numpy as np


# ---------------------------------------------------------------------
# Elementary 4x4 homogeneous transforms
# ---------------------------------------------------------------------

def translation(t):
    """Homogeneous translation matrix for vector t = (tx, ty, tz)."""
    tx, ty, tz = t
    M = np.eye(4)
    M[0, 3] = tx
    M[1, 3] = ty
    M[2, 3] = tz
    return M


def rotation_x(angle):
    """Homogeneous rotation matrix about the x-axis (radians)."""
    c, s = np.cos(angle), np.sin(angle)
    M = np.eye(4)
    M[1, 1], M[1, 2] = c, -s
    M[2, 1], M[2, 2] = s, c
    return M


def rotation_y(angle):
    """Homogeneous rotation matrix about the y-axis (radians)."""
    c, s = np.cos(angle), np.sin(angle)
    M = np.eye(4)
    M[0, 0], M[0, 2] = c, s
    M[2, 0], M[2, 2] = -s, c
    return M


def rotation_z(angle):
    """Homogeneous rotation matrix about the z-axis (radians)."""
    c, s = np.cos(angle), np.sin(angle)
    M = np.eye(4)
    M[0, 0], M[0, 1] = c, -s
    M[1, 0], M[1, 1] = s, c
    return M


# ---------------------------------------------------------------------
# A_V : the alignment transform that rotates direction V onto +z (K)
# ---------------------------------------------------------------------

def alignment_matrix(V):
    """
    Build A_V such that A_V @ [Vx, Vy, Vz, 0]^T = [0, 0, 1, 0]^T
    (V need not be pre-normalized; it is normalized internally).

    Construction:
        d      = sqrt(b^2 + c^2)                     (b, c = V's y, z comps)
        Rx(alpha): cos(alpha) = c/d, sin(alpha) = b/d   -> brings V into x-z plane
        Ry(beta):  cos(beta)  = d,   sin(beta)  = a     -> brings V onto z-axis
        A_V = Ry(beta) @ Rx(alpha)

    Special case: if V is already parallel to z (a = b = 0, d = 0),
    A_V is the identity (or 180 degree flip if V points along -z).
    """
    V = np.asarray(V, dtype=float)
    V = V / np.linalg.norm(V)
    a, b, c = V

    d = np.hypot(b, c)  # sqrt(b^2 + c^2)

    if np.isclose(d, 0.0):
        # V already along +z or -z
        if c >= 0:
            return np.eye(4)
        else:
            return rotation_x(np.pi)  # flip 180 degrees

    cos_alpha, sin_alpha = c / d, b / d
    alpha = np.arctan2(sin_alpha, cos_alpha)

    # After Rx(alpha), V becomes (a, 0, d). Ry(beta) must send this to (0,0,1):
    #   x' = a*cos(beta) + d*sin(beta) = 0  and  z' = -a*sin(beta) + d*cos(beta) = 1
    # which is solved by cos(beta) = d, sin(beta) = -a.
    cos_beta, sin_beta = d, -a
    beta = np.arctan2(sin_beta, cos_beta)

    Rx = rotation_x(alpha)
    Ry = rotation_y(beta)
    return Ry @ Rx


# ---------------------------------------------------------------------
# The full Problem 6.3 transform
# ---------------------------------------------------------------------

def rotate_about_axis(theta_deg, V, P):
    """
    Build R_{theta,L}: rotation by theta_deg (degrees) about the axis L
    through point P with direction V.

    Returns the 4x4 homogeneous matrix
        R = T_P @ A_V_inv @ Rz(theta) @ A_V @ T_negP
    """
    theta = np.radians(theta_deg)

    T_negP = translation(-np.asarray(P, dtype=float))
    T_P = translation(np.asarray(P, dtype=float))

    A_V = alignment_matrix(V)
    A_V_inv = A_V.T  # rotation matrices are orthogonal: inverse = transpose

    R_theta_K = rotation_z(theta)  # rotate about K (the z-axis) once aligned

    R = T_P @ A_V_inv @ R_theta_K @ A_V @ T_negP
    return R


def rotate_about_axis_verbose(theta_deg, V, P, point):
    """
    Same computation as rotate_about_axis(), but prints the matrix AND the
    running point after every one of the 5 steps, so each stage of the
    textbook recipe can be inspected individually.
    """
    theta = np.radians(theta_deg)
    P = np.asarray(P, dtype=float)
    V = np.asarray(V, dtype=float)
    pt = np.array([*point, 1.0])  # homogeneous point, updated after each step

    def show(step_no, label, M, pt):
        pt_new = M @ pt
        print(f"--- Step {step_no}: {label} ---")
        print("Input point  :", np.round(pt[:3], 6))
        print("Matrix:\n", np.round(M, 4))
        print("Output point :", np.round(pt_new[:3], 6))
        print()
        return pt_new

    # Step 1: T_{-P}  -- translate P to the origin
    T_negP = translation(-P)
    pt = show(1, "T_-P  (translate P to origin)", T_negP, pt)

    # Step 2: A_V  -- align V with K (the z-axis)
    A_V = alignment_matrix(V)
    pt = show(2, "A_V   (align V with z-axis K)", A_V, pt)

    # Step 3: R_{theta,K}  -- rotate by theta about K
    R_theta_K = rotation_z(theta)
    pt = show(3, f"R_theta,K (rotate {theta_deg} deg about K)", R_theta_K, pt)

    # Step 4: A_V^{-1}  -- undo the alignment
    A_V_inv = A_V.T
    pt = show(4, "A_V^-1 (undo alignment)", A_V_inv, pt)

    # Step 5: T_P  -- undo the translation
    T_P = translation(P)
    pt = show(5, "T_P   (translate back)", T_P, pt)

    print("Final rotated point:", np.round(pt[:3], 6))
    return pt[:3]


def apply_transform(M, point):
    """Apply a 4x4 homogeneous transform to a 3D point."""
    p = np.array([*point, 1.0])
    p_out = M @ p
    return p_out[:3]


# ---------------------------------------------------------------------
# Independent cross-check: Rodrigues' rotation formula about axis L
# ---------------------------------------------------------------------

def rotate_about_axis_rodrigues(theta_deg, V, P, point):
    """
    Directly rotate `point` about the axis through P with direction V,
    using Rodrigues' formula. Used only to verify rotate_about_axis().
    """
    theta = np.radians(theta_deg)
    V = np.asarray(V, dtype=float)
    V = V / np.linalg.norm(V)
    P = np.asarray(P, dtype=float)
    point = np.asarray(point, dtype=float)

    r = point - P  # vector from axis point to the point being rotated
    r_rot = (
        r * np.cos(theta)
        + np.cross(V, r) * np.sin(theta)
        + V * np.dot(V, r) * (1 - np.cos(theta))
    )
    return P + r_rot


# ---------------------------------------------------------------------
# Demo / self-test using Fig. 6-6's data: theta = 45 degrees
# ---------------------------------------------------------------------

if __name__ == "__main__":
    # Example axis: direction V and a point P on the axis
    V = np.array([1.0, 1.0, 1.0])   # arbitrary axis direction
    P = np.array([2.0, 1.0, 0.0])   # arbitrary point on the axis
    theta_deg = 45.0

    # Point to rotate (deliberately NOT on the axis, so rotation is visible)
    Q = np.array([5.0, 2.0, 1.0])

    print("=" * 60)
    print("STEP-BY-STEP (prints matrix + point after each stage)")
    print("=" * 60)
    Q_rotated_verbose = rotate_about_axis_verbose(theta_deg, V, P, Q)

    print("=" * 60)
    print("ONE-SHOT (single composed matrix, for comparison)")
    print("=" * 60)
    R = rotate_about_axis(theta_deg, V, P)
    Q_rotated_matrix = apply_transform(R, Q)
    print("Composed transform R_theta,L =\n", np.round(R, 4))
    print("Q rotated (composed matrix) =", np.round(Q_rotated_matrix, 6))

    # Cross-check with Rodrigues' formula
    Q_rotated_rodrigues = rotate_about_axis_rodrigues(theta_deg, V, P, Q)
    print("Q rotated (Rodrigues check) =", np.round(Q_rotated_rodrigues, 6))
    print(
        "\nAll three methods match:",
        np.allclose(Q_rotated_verbose, Q_rotated_matrix)
        and np.allclose(Q_rotated_matrix, Q_rotated_rodrigues),
    )