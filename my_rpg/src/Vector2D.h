#pragma once

#include <cmath>

namespace SDL2
{
class Vector2D
{
public:
    Vector2D(){}
    Vector2D(double x,double y):m_x(x),m_y(y){}
    ~Vector2D(){}

    inline double getX()const{ return m_x; }
    inline double getY()const{ return m_y; }

    inline void setX(double x){ m_x = x; }
    inline void setY(double y){ m_y = y; }

    double length()
    {
        return std::sqrt(m_x * m_x + m_y * m_y);
    }

    Vector2D operator+(const Vector2D &v)
    {
        return Vector2D(m_x + v.m_x,m_y + v.m_y);
    }

    Vector2D operator-(const Vector2D &v)
    {
        return Vector2D(m_x - v.m_x,m_y - v.m_y);
    }

    Vector2D& operator+=(const Vector2D &v)
    {
        m_x += v.m_x;
        m_y += v.m_y;

        return *this;
    }

    Vector2D operator-=(const Vector2D &v)
    {
        m_x -= v.m_x;
        m_y -= v.m_y;

        return *this;
    }

    Vector2D operator*(double v)
    {
        return Vector2D(m_x * v,m_y *v);
    }

    Vector2D& operator*=(double v)
    {
        m_x *= v;
        m_y *= v;
        return *this;
    }

    Vector2D operator/(double v)
    {
        return Vector2D(m_x / v,m_y / v);
    }

    Vector2D operator/=(double v)
    {
        m_x /= v;
        m_y /= v;
        return *this;
    }

    void normalize()
    {
        double l = length();
        if(l > 0){
            (*this) *= 1/l;
        }
    }
private:
    double m_x;
    double m_y;
};
}