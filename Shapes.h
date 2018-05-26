#ifndef OOP_SHAPES_H
#define OOP_SHAPES_H


#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include "Base.h"
#include "Container.h"



std::string to_fixed_string(const double& d){
    static char buf[255];
    snprintf(buf,255,"%.2lf",d);
    return buf;
}


class Shape    : public Printable{
public:
    Shape() {
        ++shape_counter;
    }

    virtual ~Shape() {
        --shape_counter;
    }

    static std::size_t GetCount(){
        return shape_counter;
    };

    virtual std::string get_info() const = 0 ;
protected:

private:
    static std::size_t shape_counter;
};
std::size_t Shape::shape_counter = 0;

class Point    : public Shape, public Named{
public:
    Point(double xx, double yy) : Named("Point"),x(xx),y(yy) { };
    Point(const Point& p)       : Named("Point"),x(p.x),y(p.y) { };

    virtual ~Point() = default;

    double distance(const Point& p) const{
        return sqrt( (x-p.x)*(x-p.x) + (y-p.y)*(y-p.y) );
    }
    bool on_line(const Point& p) const{
        return x == p.x || y == p.y;
    }
    double get_x() const {
        return x;
    }
    double get_y() const {
        return y;
    }
    virtual std::string get_info() const{
        return unified_string(Named::get_info(),generate_info());
    }
protected:
    double x,y;
private:
    std::string generate_info() const{
        std::string result("{ ");
        result += to_fixed_string(x);
        result += " , ";
        result += to_fixed_string(y);
        return result += " }";
    }
};
class Circle   : public Shape, public Named{
public:
    Circle(double xx, double yy, double rr) : Named("Circle"),center(xx,yy),r(rr) { };

    virtual ~Circle() = default;

    virtual std::string get_info() const{
        return unified_string(Named::get_info(),generate_info());
    }
protected:
    Point center;
    double r;
private:
    std::string generate_info() const{

        std::string result("{ ");
        result += center.get_info();
        result += " , ";
        result += "Radius : ";
        result += to_fixed_string(r);
        result += " }";

        result += " [ ";
        result += "Area = ";
        result += to_fixed_string(0);
        result += " , ";
        result += "Lenght = ";
        result += to_fixed_string(2.0 * PI * r);
        result += " ]";

        return result;
    }
};

class Polyline : public Shape , public Named{
public:
    Polyline(std::string name = "Polyline") : Named(std::move(name)) { }
    virtual ~Polyline() = default;

    void AddPoint( Point const & point ){
        points.push_back(point);
    }
    virtual std::string get_info() const{
        return unified_string(Named::get_info(),generate_info());
    }
    virtual double length()  const{
        double result = 0;
        if(points.size() > 1)
            for(std::size_t iter = 0; (iter + 1) < points.size();++iter)
                result += points[iter].distance(points[iter+1]);
        return result;
    }
protected:
    Container<Point> points;
private:
    std::string generate_info() const{
        if(!points.size())
            return "Empty";

        std::string result("{ ");
        std::size_t iter = 0;
        for(; (iter + 1) < points.size();++iter){
            result += points[iter].get_info();
            result += " , ";
        }
        if(iter < points.size())
            result += points[iter].get_info();
        result += " }";

        result += " [ ";
        result += "Lenght = ";
        result += to_fixed_string(length());
        result += " ]";

        return result;
    }
};
class Polygon  : public Polyline {
public:
    Polygon(const std::vector<Point>& ps,std::string name = "Polygon") : Polyline(name) {
        if(ps.size() < 3)
            throw(std::logic_error(name + " constructor :not have enough points"));
        //Надо сделать проверку на несовпадение вершин : сортировка и последовательное сравнение пар

        for(std::size_t iter = 0; iter < ps.size();++iter)
            AddPoint(ps[iter]);
    }
    virtual ~Polygon() = default;


    virtual std::string get_info() const{
        return unified_string(Named::get_info(),generate_info());
    }
    virtual double length() const{
        double result =  Polyline::length();
        if(points.size())
            result += points.front().distance(points.back());
        return result;
    }
protected:

private:
    std::string generate_info() const{
        if(!points.size())
            return "Empty";

        std::string result("{ ");
        std::size_t iter = 0;
        for(; iter + 1 < points.size();++iter){
            result += points[iter].get_info();
            result += " , ";
        }
        if(iter < points.size())
            result += points[iter].get_info();
        result += " }";

        result += " [ ";
        result += "Perimeter = ";
        result += to_fixed_string(length());
        result += " ]";

        return result;
    }
};

class Rect     : public Shape , public Named{
public:
    Rect(const Point& a,const Point& b,std::string name = "Rect") : Named(name),diag_points{a,b} {
        if(a.on_line(b))
            throw(std::logic_error(name + " constructor : points on line"));
    }

    virtual ~Rect() = default;

    virtual std::string get_info() const{
        return unified_string(Named::get_info(),generate_info());
    }
    virtual double Perimeter() const{
        double dim_1 = fabs(diag_points[0].get_x() - diag_points[1].get_x());
        double dim_2 = fabs(diag_points[0].get_y() - diag_points[1].get_y());

        return 2*(dim_1 + dim_2);
    }
    virtual double Area() const{
        double dim_1 = fabs(diag_points[0].get_x() - diag_points[1].get_x());
        double dim_2 = fabs(diag_points[0].get_y() - diag_points[1].get_y());

        return dim_1*dim_2;
    }
protected:
    Point diag_points[2];
private:
    std::string generate_info() const{
        std::string result("{ ");
        result += diag_points[0].get_info();
        result += " , ";
        result += diag_points[1].get_info();
        result += " }";

        result += " [ ";
        result += "Perimeter = ";
        result += to_fixed_string(Perimeter());
        result += " , ";
        result += "Area = ";
        result += to_fixed_string(Area());
        result += " ]";

        return result;
    }
};
class Square   : public Rect {
public:
    Square(const Point& a,const Point& b,std::string name = "Square") : Rect(a,b,name) {
        double dim_1 = fabs(diag_points[0].get_x() - diag_points[1].get_x());
        double dim_2 = fabs(diag_points[0].get_y() - diag_points[1].get_y());
        if(dim_1 != dim_2)
            throw(std::logic_error(name + " constructor : parties are not equal"));
    }

    virtual ~Square() = default;

    virtual std::string get_info() const{
        return unified_string(Named::get_info(),generate_info());
    }
    virtual double Perimeter() const{
        double dim_1 = fabs(diag_points[0].get_x() - diag_points[1].get_x());

        return 4*(dim_1);
    }
    virtual double Area() const{
        double dim_2 = fabs(diag_points[0].get_y() - diag_points[1].get_y());

        return dim_2*dim_2;
    }
protected:

private:
    std::string generate_info() const{
        std::string result("{ ");
        result += diag_points[0].get_info();
        result += " , ";
        result += diag_points[1].get_info();
        result += " }";

        result += " [ ";
        result += "Perimeter = ";
        result += to_fixed_string(Perimeter());
        result += " , ";
        result += "Area = ";
        result += to_fixed_string(Area());
        result += " ]";

        return result;
    }
};





#endif //OOP_SHAPES_H
