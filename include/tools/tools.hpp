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
};

#endif
