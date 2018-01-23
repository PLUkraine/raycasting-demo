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
        
        vec2<T> multPerCoord(const vec2 &o)
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
    // TODO make_vec2 function - perfect forwarding is used here, look it up
    
    // interface for sampling in normalized coordinates
    class Sampler
    {
    public:
        virtual vec2<float> sample(vec2<float> normCoord, vec2<float> dimentions)=0;
    };
    
    class RepeatingSampler : Sampler
    {
    public:
        vec2<float> sample(vec2<float> normCoord, vec2<float> dimentions);
    };
    
    
	void drawLinef(core::Texture *image, int x0, int y0, double angle);
	void drawLine(core::Texture *image, int x0, int y0, int x1, int y1);
	int getOctant(double angle);
    inline float getFraction(float n)
    {
        float _res = n - int(n);
        return (_res >= 0.f) ? _res : (1.f + _res);
    }
    inline vec2<float> getFraction(vec2<float> v)
    {
        return vec2<float>(getFraction(v.x), getFraction(v.y));
    }
}

#endif
