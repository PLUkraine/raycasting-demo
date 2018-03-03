#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

#include "spdlog/spdlog.h"

#include "RaycasterEngine.h"

namespace raycaster
{
    template<>
    bool vec2<float>::operator==(const vec2<float> &o) const
    {
        const static float EPS = 1e-8;
        return fabsf(x - o.x) < EPS && fabsf(y - o.y) < EPS;
    }
    
    float getFraction(float n)
    {
        float _res = n - int(n);
        return (_res >= 0.f) ? _res : (1.f + _res);
    }
    vec2<float> getFraction(vec2<float> v)
    {
        return vec2<float>(getFraction(v.x), getFraction(v.y));
    }
    int getOctant(float angle)
    {
        const static double M_PI_3_4 = M_PI_2 + M_PI_4;
        if (inB<float>(angle, -M_PI, -M_PI_3_4))
            return 5;
        if (inB<float>(angle, -M_PI_3_4, -M_PI_2))
            return 6;
        if (inB<float>(angle, -M_PI_2, -M_PI_4))
            return 7;
        if (inB<float>(angle, -M_PI_4, 0.))
            return 8;
        if (inB<float>(angle, 0., M_PI_4))
            return 1;
        if (inB<float>(angle, M_PI_4, M_PI_2))
            return 2;
        if (inB<float>(angle, M_PI_2, M_PI_3_4))
            return 3;
        if (inB<float>(angle, M_PI_3_4, M_PI))
            return 4;
        return -1;
    }
    
    int modSgn(int x)
    {
        return x > 0 ? 1 : 0;
    }
    vec2<int> getDeltaBrick(float angle)
    {
        angle = convertAngle(angle);
        
        if (angle < M_PI / 2.f)
        {
            return vec2<int>(1, 1);
        }
        if (angle < M_PI)
        {
            return vec2<int>(-1, 1);
        }
        if (angle < 3.f*M_PI / 2.f)
        {
            return vec2<int>(-1, -1);
        }
        
        return vec2<int>(1, -1);
    }
    float convertAngle(float angle)
    {
        const float D_PI = M_PI + M_PI;
        
        while (angle < 0.f)
        {
            angle += D_PI;
        }
        while (angle >= D_PI)
        {
            angle -= D_PI;
        }
        return angle;
    }
}
