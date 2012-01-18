#include <algorithm>
#include <climits>
#include <limits>
#include <cmath>
#include "geometry.h"
#include "interval.h"
#include "adaptive.h"

namespace
{
    bool intersect_axis_rectangles(point const &A, point const &B, point const &C, point const &D)
    {
        return !( (C.x > B.x) || (D.x < A.x) || (C.y > B.y) || (D.y < A.y) );
    }
    bool check_bounding_boxes2(point const &A, point const &B, point const &C, point const &D)
    {  
        using std::min;
        using std::max;
        return intersect_axis_rectangles
        (
            point(min(A.x, B.x), min(A.y, B.y)),
            point(max(A.x, B.x), max(A.y, B.y)),
            point(min(C.x, D.x), min(C.y, D.y)),
            point(max(C.x, D.x), max(C.y, D.y))
        );
    }

    bool check_bounding_boxes(point const &A, point const &B, point const &C, point const &D)
    {
        return !(((A.x < C.x) && (A.x < D.x) && (D.x < C.x) && (D.x < D.x)) ||
                 ((C.x < A.x) && (C.x < D.x) && (D.x < A.x) && (D.x < D.x)) ||
                 ((A.y < C.y) && (A.y < D.y) && (D.y < C.y) && (D.y < D.y)) ||
                 ((C.y < A.y) && (C.y < D.y) && (D.y < A.y) && (D.y < D.y)));
    }
}

int epsilon_left_turn(const point& a, const point& b, const point& c)
{
    double res = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    static double eps1 = 8 * std::numeric_limits<double>::epsilon();
    double eps = eps1 * std::abs((b.x - a.x) * (c.y - a.y)) + std::abs((b.y - a.y) * (c.x - a.x));
    if (res > eps)
    {
        return 1;
    }
    if (-res > eps)
    {
        return -1;
    }
    return 0;
}

int left_turn(point const &a, point const &b, point const &c)
{
    int res;
    res = epsilon_left_turn(a, b, c);
    if (res != 0)
    {
        return res;
    }
    res = interval_left_turn(a, b, c);
    if (res != 0)
    {
        return res;
    }
    res = adaptive_left_turn(a, b, c);
    return res;
}


bool segments_intersects(point const &A, point const &B, point const &C, point const &D)
{
    if (!check_bounding_boxes(A, B, C, D))
    {
        return false;
    }

    if (1 == left_turn(A, B, C) * left_turn(A, B, D))
    {
        return false;
    }
    if (1 ==  left_turn(C, D, A) * left_turn(C, D, B))
    {
        return false;
    }
    return true;
}