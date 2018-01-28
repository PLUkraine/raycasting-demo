#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

#include "spdlog/spdlog.h"

#include "RaycasterEngine.h"

namespace raycaster
{

	int getOctant(int dx, int dy)
	{
		bool dxLarger = abs(dx) > abs(dy);

		if (dx > 0 && dy > 0)
			return dxLarger ? 1 : 2;
		else if (dx < 0 && dy > 0)
			return dxLarger ? 4 : 3;
		else if (dx < 0 && dy < 0)
			return dxLarger ? 5 : 6;
		else
			return dxLarger ? 8 : 7;
	}

	bool inB(int v, int b)
	{
		return 0 <= v && v < b;
	}

	int getOctant(double angle)
	{
		const static double M_PI_3_4 = M_PI_2 + M_PI_4;
		if (inB(angle, -M_PI, -M_PI_3_4))
			return 5;
		if (inB(angle, -M_PI_3_4, -M_PI_2))
			return 6;
		if (inB(angle, -M_PI_2, -M_PI_4))
			return 7;
		if (inB(angle, -M_PI_4, 0.))
			return 8;
		if (inB(angle, 0., M_PI_4))
			return 1;
		if (inB(angle, M_PI_4, M_PI_2))
			return 2;
		if (inB(angle, M_PI_2, M_PI_3_4))
			return 3;
		if (inB(angle, M_PI_3_4, M_PI))
			return 4;
		
		return -1;
	}

	// forward means from real coordinates to algorithm's
	// !forward == from algo's to real
	void swapCoords(int &x, int &y, int octant, bool forward)
	{
		switch (octant)
		{
		case 1:
			break;
		case 2:
			std::swap(x, y);
			break;
		case 3:
			if (forward) x = -x;
			std::swap(x, y);
			if (!forward) x = -x;
			break;
		case 4:
			x = -x;
			break;
		case 5:
			x = -x;
			y = -y;
			break;
		case 6:
			x = -x;
			y = -y;
			std::swap(x, y);
			break;
		case 7:
			if (forward) y = -y;
			std::swap(x, y);
			if (!forward) y = -y;
			break;
		case 8:
			y = -y;
			break;
		}
	}
	float getSlope(double angle)
	{
		float answer = static_cast<float>(tan(angle));
		if (answer < 0) answer = -answer;
		if (answer > 1.f) answer = 1.f / answer;
		return answer;
	}

	

	void drawLinef(core::Texture *image, int x0, int y0, double angle)
	{
		int octant = getOctant(angle);
		float slope = getSlope(angle);

		int x = x0;
		int y = y0;
		
		float error = 0;
		while (inB(x, image->width()) && inB(y, image->height()))
		{
			image->getData(x, y, core::Texture::RED) = 0xff;
			swapCoords(x, y, octant, true);
			error += slope;
			++x;
			if (error > 0.5f)
			{
				error -= 1.f;
				++y;
			}
			swapCoords(x, y, octant, false);
		}
	}

	void drawLine(core::Texture * image, int x0, int y0, int x1, int y1)
	{
		// TODO fix
		int dx = x1 - x0;
		int dy = y1 - y0;
		int octant = getOctant(dx, dy);
		float slope = getSlope(atan2(dy, dx));
		
		int x = x0;
		int y = y0;

		int accX = 0;
		float error = 0;
		int fin = std::max(abs(dx), abs(dy));
		while (accX < fin)
		{
			if (inB(x, image->width()) && inB(y, image->height()))
				image->getData(x, y, core::Texture::BLUE) = 0xff;
			swapCoords(x, y, octant, true);
			error += slope;
			++x; ++accX;
			if (error > 0.5f)
			{
				error -= 1.f;
				++y;
			}
			swapCoords(x, y, octant, false);
		}
	}

    //real deal
    template<>
    bool vec2<float>::operator==(const vec2<float> &o) const
    {
        const static float EPS = 1e-8;
        return fabsf(x - o.x) < EPS && fabsf(y - o.y) < EPS;
    }
    
    vec2<float> RepeatingSampler::sample(vec2<float> normCoord, vec2<float> dimentions)
    {
        vec2<float> coord = getFraction(normCoord);
        return coord.multPerCoord(dimentions);
    }
    
    
    
}
