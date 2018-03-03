#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "Texture.h"

#include <string>
#include <math.h>

namespace raycaster
{
    
    template <typename T>
    struct vec2
    {
    public:
        T x, y;
        
    public:
        vec2() : x(0), y(0) {}
        
        vec2(T _n) : x(_n), y(_n) {}
        
        vec2(T _x, T _y) : x(_x), y(_y) {}
        
        template<typename Y>
        vec2(const vec2<Y> &o) : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)) {}
        
        bool operator==(const vec2<T> &o) const
        {
            return x == o.x && y == o.y;
        }
        
        vec2<T> operator+(const vec2 &o) const
        {
            return vec2(o.x+x, o.y+y);
        }
        vec2<T>& operator+=(const vec2 &o)
        {
            this->x += o.x;
            this->y += o.y;
            return *this;
        }
        vec2<T> operator-(const vec2 &o) const
        {
            return vec2(x-o.x, y-o.y);
        }
        vec2<T>& operator-=(const vec2 &o)
        {
            x -= o.x;
            y -= o.y;
            return *this;
        }
        vec2<T> operator*(T scalar) const
        {
            return vec2(x*scalar, y*scalar);
        }
        vec2<T> operator/(T scalar) const
        {
            return vec2(x/scalar, y/scalar);
        }
        
        vec2<T> multPerCoord(const vec2 &o) const
        {
            return vec2(x*o.x, y*o.y);
        }
        T dot(vec2 o) const
        {
            return x*o.x + y*o.y;
        }
        T cross(vec2 o) const
        {
            return x * o.y - y * o.x;
        }
        float len() const
        {
            return sqrtf(x*x + y*y);
        }
        float sqrLen() const
        {
            return x*x + y*y;
        }
        vec2<float> norm() const
        {
            float l = len();
            return {x/l, y/l};
        }
        
        std::string toStr() const
        {
            return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
        }
    };
    
    
    template <class T>
    bool inB(T v, T a, T b)
    {
        return a <= v && v < b;
    }
    
    float getFraction(float n);
    vec2<float> getFraction(vec2<float> v);
    int getOctant(float angle);
    int modSgn(int x);
    vec2<int> getDeltaBrick(float angle);
    float convertAngle(float angle);
}

#endif
