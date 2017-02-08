#include "BezierCurve.hpp"

BezierCurve::BezierCurve(std::deque<point3> _controlPoints) : controlPoints(_controlPoints)
{}

BezierCurve::~BezierCurve()
{}

void BezierCurve::getPoints(unsigned int precision,point3 * points) const
{
    //point3 * points = new point3[precision];
    float t;
    int j;
    for(unsigned int i=0;i<precision;i++)
    {
        t=i/float(precision-1);
        j=0;
            for(std::deque<point3>::const_iterator it=controlPoints.begin();it != controlPoints.end();it++)
            {
                points[i].x+=getBernsteinPolynom(j,controlPoints.size()-1,t)*it->x;
                points[i].y+=getBernsteinPolynom(j,controlPoints.size()-1,t)*it->y;
                points[i].z+=getBernsteinPolynom(j,controlPoints.size()-1,t)*it->z;
                j++;
            }
    }
    //return points;
}

float getBernsteinPolynom(unsigned int i,unsigned int n,float t)
{
    float polynom;
        polynom=(fact(n)/(fact(i)*fact(n-i)))*pow(t,i)*pow(1-t,n-i);

    return polynom;
}

int fact(int n)
{
    if(n==0)
        return 1;
    else
        return n*fact(n-1);
}
