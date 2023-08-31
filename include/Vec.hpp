#pragma once


#include <glm/glm.hpp>





// // Overloaded operator for multiplying Vec3f by a float
// Vec3f operator*(const Vec3f& vec, float scalar) {
//     return Vec3f(vec.x * scalar, vec.y * scalar, vec.z * scalar);
// }

// // Overloaded operator for multiplying a float by Vec3f
// Vec3f operator*(float scalar, const Vec3f& vec) {
//     return  Vec3f(vec.x * scalar, vec.y * scalar, vec.z * scalar); // Reuse the previous operator for consistency
// }


// // Overloaded operator for dividing Vec3f by a float
// Vec3f operator/(const Vec3f& vec, float scalar) {
//     return Vec3f(vec.x / scalar, vec.y / scalar, vec.z / scalar);
// }

class Vec3f {
public:
    float x, y, z;
    Vec3f() : x(0), y(0), z(0) {}
    Vec3f(float xx) : x(xx), y(xx), z(xx) {}
    Vec3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    Vec3f operator * (const float &r) const { return Vec3f(x * r, y * r, z * r); }
    Vec3f operator / (const float &r) const { return Vec3f(x / r, y / r, z / r); }

    float length() {return std::sqrt(x * x + y * y + z * z);}
    Vec3f normalized() {
        float n = std::sqrt(x * x + y * y + z * z);
        return Vec3f(x / n, y / n, z / n);
    }

    // Vec3f normalize(const Vec3f &v) {
    //     float n = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    //     return Vec3f(v.x / n, v.y / n, v.z / n);
    // }

    // float length(const Vec3f &v) {return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);}

    Vec3f operator * (const Vec3f &v) const { return Vec3f(x * v.x, y * v.y, z * v.z); }
    Vec3f operator - (const Vec3f &v) const { return Vec3f(x - v.x, y - v.y, z - v.z); }
    Vec3f operator + (const Vec3f &v) const { return Vec3f(x + v.x, y + v.y, z + v.z); }
    Vec3f operator - () const { return Vec3f(-x, -y, -z); }
    Vec3f& operator += (const Vec3f &v) { x += v.x, y += v.y, z += v.z; return *this; }
    friend Vec3f operator * (const float &r, const Vec3f &v)
    { return Vec3f(v.x * r, v.y * r, v.z * r); }
    friend std::ostream & operator << (std::ostream &os, const Vec3f &v)
    { return os << v.x << ", " << v.y << ", " << v.z; }
    double       operator[](int index) const;
    double&      operator[](int index);


    static Vec3f Min(const Vec3f &p1, const Vec3f &p2) {
        return Vec3f(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
                       std::min(p1.z, p2.z));
    }

    static Vec3f Max(const Vec3f &p1, const Vec3f &p2) {
        return Vec3f(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
                       std::max(p1.z, p2.z));
    }
};
inline double Vec3f::operator[](int index) const {
    return (&x)[index];
}

inline float dotProduct(const Vec3f &a, const Vec3f &b)
{ return a.x * b.x + a.y * b.y + a.z * b.z; }

inline Vec3f crossProduct(const Vec3f &a, const Vec3f &b)
{
    return Vec3f(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
    );
}



//typedef Vec3f Vec3f;





