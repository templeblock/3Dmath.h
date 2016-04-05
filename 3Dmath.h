/*
 * 3Dmath.h: basic 3D math utilities in C                          v0.01
 * Steven Skomski (skomski.org)
 * 
 * DESCRIPTION
 * You should probably not use this for production work, and instead use
 * something more developed and optimized, as this is just an 
 * independent study project of mine. I hope to make it more useful and
 * full-featured in the future, but for now it's only really useful for 
 * simple demonstrations.
 * 
 * CONVENTIONS
 * Matrices are in row-major order, so if they are to be used with
 * OpenGL, calls to glUniformMatrix4fv and others should specify that
 * matrices be transposed.
 * 
 * LICENSE
 * This software is in the public domain. Where that dedication is not
 * recognized, you are granted a perpetual, irrevocable license to copy,
 * distribute, and modify this file as you see fit. No warranty is
 * implied, so use this at your own risk.
 * 
 */

#ifndef SE_3DMATH_IMPLEMENTATION
#define SE_3DMATH_IMPLEMENTATION

#include <math.h>       // trig fcns
#include <memory.h>     // memcpy

#ifdef __cplusplus
extern "C" {
#endif

#ifndef seFloat
#ifdef SE_OPENGL
typedef GLfloat seFloat;
#else
typedef float seFloat;
#endif
#endif

/* Constants */
#define SE_PI 3.14159265358979323846f

/* Macros */
#define SE_DEG2RAD(n) (n*(SE_PI/180.0f))
#define SE_RAD2DEG(n) (n*(180.0f/SE_PI))
#define SE_SQUARED(n) (n*n)


/** STRUCTURES / TYPES ************************************************/

typedef struct {
    seFloat e[16];
} seMat4;

typedef struct {
    seFloat e[9];
} seMat3;

typedef struct {
    seFloat x, y, z;
} seVec3;


/** PROTOTYPES ********************************************************/

/* 3D Vectors */
seVec3 seV3Assign(seFloat x, seFloat y, seFloat z);
seFloat seV3Length(seVec3 v);
seFloat seV3Dot(seVec3 v1, seVec3 v2);
seVec3 seV3Cross(seVec3 v1, seVec3 v2);
seVec3 seV3Normalize(seVec3 v);
seVec3 seV3Scale(seVec3 v, const seFloat len);
seVec3 seV3Add(seVec3 v1, seVec3 v2);
seVec3 seV3Subtract(seVec3 v1, seVec3 v2);
seVec3 seV3MultiplyM3(seMat3 m, seVec3 v);

/* 4x4 Matrices */
seMat4 seM4Fill(seFloat n);
seMat4 seM4Multiply(seMat4 m1, seMat4 m2);
seMat4 seM4Perspective(seFloat angle, seFloat ratio, seFloat near, seFloat far);
seMat4 seM4LookAt(seVec3 eye, seVec3 center, seVec3 up);
seMat4 seM4Identity();
seMat4 seM4Scale(seFloat x, seFloat y, seFloat z);
seMat4 seM4Translate(seFloat x, seFloat y, seFloat z);
seMat4 seM4Rotate(seFloat x, seFloat y, seFloat z);
seMat4 seM4RotateEuler(seVec3 v);
seMat4 seM4RotateAA(seVec3 v, const seFloat t);


/** IMPLEMENTATION ****************************************************/

/* 3D Vectors */

/* 
 * seV3Assign:
 * Returns a vector with component values specified by the parameters.
 * 
 */
seVec3 seV3Assign(seFloat x, seFloat y, seFloat z)
{
    seVec3 out;
    out.x = x;
    out.y = y;
    out.z = z;

    return out;
}

/* 
 * seV3Length:
 * Returns the length of a 3D vector.
 * 
 */
seFloat seV3Length(seVec3 v)
{
    return sqrtf(SE_SQUARED(v.x) + SE_SQUARED(v.y) + SE_SQUARED(v.z));
}

/* 
 * seV3Dot:
 * Returns the dot product of two 3D vectors.
 * 
 */
seFloat seV3Dot(seVec3 v1, seVec3 v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

/* 
 * seV3Cross:
 * Returns the cross product of two 3D vectors.
 * 
 */
seVec3 seV3Cross(seVec3 v1, seVec3 v2)
{
    seVec3 out;
    out.x = (v1.y * v2.z) - (v1.z * v2.y);
    out.y = (v1.z * v2.x) - (v1.x * v2.z);
    out.z = (v1.x * v2.y) - (v1.y * v2.x);

    return out;
}

/* 
 * seV3Normalize:
 * Returns a normalized version of the specified 3D vector.
 * 
 */
seVec3 seV3Normalize(seVec3 v)
{
    seFloat len = seV3Length(v);

    seVec3 out;
    out.x = v.x / len;
    out.y = v.y / len;
    out.z = v.z / len;

    return out;
}

/* 
 * seV3Scale:
 * Returns the specified vector scaled by the specified factor.
 * 
 */
seVec3 seV3Scale(seVec3 v, const seFloat len)
{
    v = seV3Normalize(v);
    v.x *= len;
    v.y *= len;
    v.z *= len;

    return v;
}

/* 
 * seV3Add:
 * Returns the sum of two 3D vectors.
 * 
 */
seVec3 seV3Add(seVec3 v1, seVec3 v2)
{
    seVec3 out;
    out.x = v1.x + v2.x;
    out.y = v1.y + v2.y;
    out.z = v1.z + v2.z;

    return out;
}

/* 
 * seV3Subtract:
 * Returns the difference of two 3D vectors.
 * 
 */
seVec3 seV3Subtract(seVec3 v1, seVec3 v2)
{
    seVec3 out;
    out.x = v1.x - v2.x;
    out.y = v1.y - v2.y;
    out.z = v1.z - v2.z;

    return out;
}


/* 
 * seV3MultiplyM3:
 * Returns the product of a specified 3x3 matrix and 3D vector.
 * 
 */
seVec3 seV3MultiplyM3(seMat3 m, seVec3 v)
{
    seVec3 out;
    out.x = (m.e[0] * v.x) + (m.e[1] * v.y) + (m.e[2] * v.z);
    out.y = (m.e[3] * v.x) + (m.e[4] * v.y) + (m.e[5] * v.z);
    out.z = (m.e[6] * v.x) + (m.e[7] * v.y) + (m.e[8] * v.z);

    return out;
}

/* 4x4 Matrices */

/* 
 * seM4Fill:
 * Constructs and returns a 4x4 matrix populated with a given number.
 * 
 */
seMat4 seM4Fill(seFloat n)
{
    seMat4 out;
    memset(&out.e, n, sizeof(out.e));
    return out;
}

/* 
 * seM4Multiply:
 * Returns the result of AxB, where A and B are 4x4 matrices.
 * 
 */
seMat4 seM4Multiply(seMat4 A, seMat4 B)
{
    seMat4 out;
    out.e[0]  = (A.e[0]  * B.e[0]) + (A.e[1]  * B.e[4]) + (A.e[2]  * B.e[8])  + (A.e[3]  * B.e[12]);
    out.e[1]  = (A.e[0]  * B.e[1]) + (A.e[1]  * B.e[5]) + (A.e[2]  * B.e[9])  + (A.e[3]  * B.e[13]);
    out.e[2]  = (A.e[0]  * B.e[2]) + (A.e[1]  * B.e[6]) + (A.e[2]  * B.e[10]) + (A.e[3]  * B.e[14]);
    out.e[3]  = (A.e[0]  * B.e[3]) + (A.e[1]  * B.e[7]) + (A.e[2]  * B.e[11]) + (A.e[3]  * B.e[15]);
    out.e[4]  = (A.e[4]  * B.e[0]) + (A.e[5]  * B.e[4]) + (A.e[6]  * B.e[8])  + (A.e[7]  * B.e[12]);
    out.e[5]  = (A.e[4]  * B.e[1]) + (A.e[5]  * B.e[5]) + (A.e[6]  * B.e[9])  + (A.e[7]  * B.e[13]);
    out.e[6]  = (A.e[4]  * B.e[2]) + (A.e[5]  * B.e[6]) + (A.e[6]  * B.e[10]) + (A.e[7]  * B.e[14]);
    out.e[7]  = (A.e[4]  * B.e[3]) + (A.e[5]  * B.e[7]) + (A.e[6]  * B.e[11]) + (A.e[7]  * B.e[15]);
    out.e[8]  = (A.e[8]  * B.e[0]) + (A.e[9]  * B.e[4]) + (A.e[10] * B.e[8])  + (A.e[11] * B.e[12]);
    out.e[9]  = (A.e[8]  * B.e[1]) + (A.e[9]  * B.e[5]) + (A.e[10] * B.e[9])  + (A.e[11] * B.e[13]);
    out.e[10] = (A.e[8]  * B.e[2]) + (A.e[9]  * B.e[6]) + (A.e[10] * B.e[10]) + (A.e[11] * B.e[14]);
    out.e[11] = (A.e[8]  * B.e[3]) + (A.e[9]  * B.e[7]) + (A.e[10] * B.e[11]) + (A.e[11] * B.e[15]);
    out.e[12] = (A.e[12] * B.e[0]) + (A.e[13] * B.e[4]) + (A.e[14] * B.e[8])  + (A.e[15] * B.e[12]);
    out.e[13] = (A.e[12] * B.e[1]) + (A.e[13] * B.e[5]) + (A.e[14] * B.e[9])  + (A.e[15] * B.e[13]);
    out.e[14] = (A.e[12] * B.e[2]) + (A.e[13] * B.e[6]) + (A.e[14] * B.e[10]) + (A.e[15] * B.e[14]);
    out.e[15] = (A.e[12] * B.e[3]) + (A.e[13] * B.e[7]) + (A.e[14] * B.e[11]) + (A.e[15] * B.e[15]);

    return out;
}

/* 
 * seM4Perspective:
 * Constructs and returns a clip-space transformation matrix.
 * 
 */
seMat4 seM4Perspective(seFloat angle, seFloat ratio, seFloat near, seFloat far)
{
    seFloat ct = 1.0f / tan(angle / 2.0f);
    seMat4 out = seM4Fill(0);

    out.e[0]  =  ct / ratio;
    out.e[5]  =  ct;
    out.e[10] = (far + near) / (near - far);
    out.e[11] = (2 * far * near) / (near - far);
    out.e[14] = -1;

    return out;
}

/* 
 * seM4LookAt:
 * Constructs and returns a view-space transformation matrix.
 * 
 */
seMat4 seM4LookAt(seVec3 eye, seVec3 center, seVec3 up)
{
    seVec3 f = seV3Normalize(seV3Subtract(center, eye));    // front
    seVec3 s = seV3Normalize(seV3Cross(f, up));             // side
    seVec3 u = seV3Cross(s, f);                             // up

    seMat4 out;
    out.e[0]  =  s.x;
    out.e[4]  =  u.x;
    out.e[8]  = -f.x;
    out.e[12] =  0;
    out.e[1]  =  s.y;
    out.e[5]  =  u.y;
    out.e[9]  = -f.y;
    out.e[13] =  0;
    out.e[2]  =  s.z;
    out.e[6]  =  u.z;
    out.e[10] = -f.z;
    out.e[14] =  0;
    out.e[3]  = -seV3Dot(s, eye);
    out.e[7]  = -seV3Dot(u, eye);
    out.e[11] =  seV3Dot(f, eye);
    out.e[15] =  1;

    return out;
}

/* 
 * seM4Identity:
 * Returns a 4x4 identity matrix.
 * 
 */
seMat4 seM4Identity()
{
    seMat4 out = seM4Fill(0);
    out.e[0]  = 1;
    out.e[5]  = 1;
    out.e[10] = 1;
    out.e[15] = 1;

    return out;
}

/* 
 * seM4Scale:
 * Constructs and returns a scaling transformation matrix.
 * 
 */
seMat4 seM4Scale(seFloat x, seFloat y, seFloat z)
{    
    seMat4 out = seM4Fill(0);
    out.e[0]  = x;
    out.e[5]  = y;
    out.e[10] = z;
    out.e[15] = 1.0;

    return out;
}

/* 
 * seM4Translate:
 * Constructs and returns a translation transformation matrix.
 * 
 */
seMat4 seM4Translate(seFloat x, seFloat y, seFloat z)
{
    seMat4 out = seM4Fill(0);
    out.e[3]  = x;
    out.e[7]  = y;
    out.e[11] = z;
    out.e[0]  = 1;
    out.e[5]  = 1;
    out.e[10] = 1;
    out.e[15] = 1;

    return out;
}

/* 
 * seM4RotateEuler:
 * Constructs and returns a rotation transformation matrix using
 * Euler angles.
 * 
 */
seMat4 seM4RotateEuler(seFloat x, seFloat y, seFloat z)
{
    seFloat cx = cos(SE_DEG2RAD(x));
    seFloat sx = sin(SE_DEG2RAD(x));
    seFloat cy = cos(SE_DEG2RAD(y));
    seFloat sy = sin(SE_DEG2RAD(y));
    seFloat cz = cos(SE_DEG2RAD(z));
    seFloat sz = sin(SE_DEG2RAD(z));

    seMat4 out;
    out.e[0]  =  cy * cz;
    out.e[1]  = -cy * sz;
    out.e[2]  =  sy;
    out.e[4]  =  sx * sy * cz + cx * sz;
    out.e[5]  = -sx * sy * sz + cx * cz;
    out.e[6]  = -sx * cy;
    out.e[8]  = -cx * sy * cz + sx * sz;
    out.e[9]  =  cx * sy * sz + sx * cz;
    out.e[10] =  cx * cy;
    out.e[3]  = 0;
    out.e[7]  = 0;
    out.e[11] = 0;
    out.e[12] = 0;
    out.e[13] = 0;
    out.e[14] = 0;
    out.e[15] = 1;

    return out;
}

/* 
 * seM4RotateEulerV3:
 * Constructs and returns a rotation transformation matrix using
 * Euler angles stored in a 3D vector.
 * 
 */
seMat4 seM4RotateEulerV3(seVec3 v)
{
    return seM4Rotate(v.x, v.y, v.z);
}

/* 
 * seM4RotateAA:
 * Constructs and returns a rotation transformation matrix using
 * an axis specified by a 3D vector and an angle by which to rotate
 * around the specified axis.
 * 
 */
seMat4 seM4RotateAA(seVec3 v, const seFloat t)
{
    v = seV3Normalize(v);

    seFloat c = cos(t * 0.01745329251f);
    seFloat s = sin(t * 0.01745329251f);

    seMat4 out;
    out.e[0]  =  c + v.x * v.x * (1 - c);
    out.e[1]  = -v.z * s + v.x * v.y * (1 - c);
    out.e[2]  =  v.y * s + v.x * v.z * (1 - c);
    out.e[4]  =  v.z * s + v.y * v.x * (1 - c);
    out.e[5]  =  c + v.y * v.y * (1 - c);
    out.e[6]  = -v.x * s + v.y * v.z * (1 - c);
    out.e[8]  = -v.y * s + v.z * v.x * (1 - c);
    out.e[9]  =  v.x * s + v.z * v.y * (1 - c);
    out.e[10] =  c + v.z * v.z * (1 - c);
    out.e[3]  = 0;
    out.e[7]  = 0;
    out.e[11] = 0;
    out.e[12] = 0;
    out.e[13] = 0;
    out.e[14] = 0;
    out.e[15] = 1;

    return out;
}

#ifdef __cplusplus
}
#endif
#endif
