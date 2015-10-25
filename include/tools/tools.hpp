#ifndef _TOOLS_HPP
#define _TOOLS_HPP

#include "structs.hpp"

class Tools
{
public:
    static inline float distance(Point start, Point end)
    {
        float dx = start.x - end.x;
        float dy = end.y - end.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    static inline float manhattenDistance(Point start, Point end)
    {
		float x = (float)(std::fabs((float)(start.x - end.x)));
		float y = (float)(std::fabs((float)(start.y - end.y)));
		return x + y;
    }
    
    static inline double perpDot(const Point& a, const Point& b) { return (a.y * b.x) - (a.x * b.y); }

    static inline bool getIntersection(Ray ray, Segment* segment, Point& intersect)
    {
        Point a(ray.end - ray.start);
        Point b(segment->p2 - segment->p1);

        double f = perpDot(a, b);
        if(!f)
            return false;

        Point c(segment->p2 - ray.end);
        double aa = perpDot(a,c);
        double bb = perpDot(b,c);

        if(f < 0)
        {
            if(aa > 0 || aa < f) return false;
            if(bb > 0 || bb < f) return false;
        }
        else
        {
            if(aa < 0 || aa > f) return false;
            if(bb < 0 || bb > f) return false;
        }

        double out = 1.0 - (aa / f);
        intersect = ((segment->p2 - segment->p1) * out) + segment->p1;
        return true;
    }

    static inline bool getIntersection2(Ray ray, Segment* segment, Point& intersect)
    {
        Point r1 = ray.start;
        Point r2 = ray.end;
        Point p1 = segment->p1;
        Point p2 = segment->p2; 

        float d = (r1.x - r2.x) * (p1.y - p2.y) - (r1.y - r2.y) * (p1.x - p2.x);

        if(d == 0)
            return false;

        float pre  = (r1.x * r2.y - r1.y * r2.x);
        float post = (p1.x * p2.y - p1.y * p2.x);
        float x = ( pre * (p1.x - p2.x) - (r1.x - r2.x) * post ) / d;
        float y = ( pre * (p1.y - p2.y) - (r1.y - r2.y) * post ) / d;

        if( x < std::min(r1.x, r2.x) || x > std::max(r1.x, r2.x) ) return false;
        if( x < std::min(p1.x, p2.x) || x > std::max(p1.x, p2.x) ) return false;
        if( y < std::min(r1.y, r2.y) || y > std::max(r1.y, r2.y) ) return false;
        if( y < std::min(p1.y, p2.y) || y > std::max(p1.y, p2.y) ) return false;

        intersect.x = (int)x;
        intersect.y = (int)y;

        return true;
    }

    static inline float deg2rad(float degrees)
    {
        return (degrees * M_PI) / 180;
    }

    static inline float rad2deg(float rad)
    {
        return (rad * 180.f) / M_PI;
    }

    static inline float normalizeRad(float a)
    {
        float x = std::fmod(a, 2*M_PI);
        if(x < 0)
            x += 2*M_PI;
        return x;
    }
    
    static inline bool degreesBetween(float _angle, float _min, float _max)
    {
        float angle = std::fmod(360 + _angle, 360);
        float min = std::fmod(3600000 + _min, 360);
        float max = std::fmod(3600000 + _max, 360);

        if(min < max) return (min <= angle && angle <= max);

        return (min <= angle || angle <= max);
    }

    // Need this to be radians to stop the buggy shit...
    static inline bool radiansBetween2(float _angle, float _min, float _max)
    {
        return false;
    }
    
    static inline bool radiansBetween(float _angle, float _min, float _max)
    {
        float angle = std::fmod(360 + (rad2deg(_angle)), 360);
        float min = std::fmod(360000 + rad2deg(_min), 360);
        float max = std::fmod(360000 + rad2deg(_max), 360);

        if(min < max) return (min <= angle && angle <= max);

        return (min <= angle || angle <= max);
    }

    static inline bool pointInTriangle(Point t1, Point t2, Point t3, Point p)
    {
        float d = ((t2.y-t3.y)*(t1.x-t3.x) + (t3.x-t2.x)*(t1.y-t3.y));

        float a = ((t2.y-t3.y)*(p.x-t3.x) + (t3.x-t2.x)*(p.y-t3.y)) / d;
        float b = ((t3.y-t1.y)*(p.x-t3.x) + (t1.x-t3.x)*(p.y-t3.y)) / d;
        float c = 1 - a - b;
        
        return 0 <= a && a <= 1 && 0 <= b && b <= 1 && 0 <= c && c <= 1;
    }
};

#endif
