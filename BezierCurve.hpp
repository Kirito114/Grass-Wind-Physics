#include <deque>
#include <cmath>
#include <iostream>
#include "point3.h"

class BezierCurve
{
private:
    std::deque<point3> controlPoints;
public:
    BezierCurve(std::deque<point3> _controlPoints);
    ~BezierCurve();
    void getPoints(unsigned int precision,point3 * points) const;
};

float getBernsteinPolynom(unsigned int i,unsigned int n,float t);
int fact(int n);
