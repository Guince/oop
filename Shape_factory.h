#ifndef OOP_SHAPE_FACTORY_H
#define OOP_SHAPE_FACTORY_H


#include <memory>
#include <vector>
#include <ctime>
#include "Shapes.h"

class Random_shape_factory{
public:
    static Random_shape_factory& get_instance(){
        static Random_shape_factory instance;
        return instance;
    }

    virtual std::unique_ptr<Shape> create_Point(){
        auto nw_Point = new Point(get_rnd_value(),get_rnd_value());
        return std::unique_ptr<Shape>(dynamic_cast<Shape*>(nw_Point));
    }
    virtual std::unique_ptr<Shape> create_Circle(){
        auto nw_Circle = new Circle(get_rnd_value(),get_rnd_value(),get_rnd_value());
        return std::unique_ptr<Shape>(dynamic_cast<Shape*>(nw_Circle));
    }
    virtual std::unique_ptr<Shape> create_Polyline(){
        auto nw_Polyline = new Polyline();
        auto count = get_rnd_value() % 5 ;
        for(auto iter = 0;iter < count;++iter)
            nw_Polyline->AddPoint(Point(get_rnd_value(),get_rnd_value()));
        return std::unique_ptr<Shape>(dynamic_cast<Shape*>(nw_Polyline));
    }
    virtual std::unique_ptr<Shape> create_Polygon(){
        std::vector<Point> points;

        auto count = get_rnd_value() % 5 + 3;
        double rnd_shift_1 = get_rnd_value() % 10 + 1,rnd_shift_2 = get_rnd_value() % 10 + 1;

        if(count)
            points.push_back(Point(get_rnd_value(),get_rnd_value()));

        for(auto iter = 1;iter < count;++iter)
            points.push_back(Point(points.back().get_x() + rnd_shift_1,points.back().get_y() + rnd_shift_2));


        auto nw_Polygon = new Polygon(points);
        return std::unique_ptr<Shape>(dynamic_cast<Shape*>(nw_Polygon));
    }
    virtual std::unique_ptr<Shape> create_Rect(){
        Point p_1(get_rnd_value(),get_rnd_value());
        Point p_2(p_1.get_x() + get_rnd_value() % 10 + 1,p_1.get_y() + get_rnd_value() % 10 + 1);
        auto nw_Rect = new Rect(p_1,p_2);
        return std::unique_ptr<Shape>(dynamic_cast<Shape*>(nw_Rect));
    }
    virtual std::unique_ptr<Shape> create_Square(){
        Point p_1(get_rnd_value(),get_rnd_value());
        double square_leng = get_rnd_value() + 1;
        Point p_2(p_1.get_x() + square_leng,p_1.get_y() + square_leng);
        auto nw_Square = new Square(p_1,p_2);
        return std::unique_ptr<Shape>(dynamic_cast<Shape*>(nw_Square));
    }

    std::unique_ptr<Shape> create_random_shape(){
        auto num = get_rnd_value() % 6;
        switch(num){
            case 0 :
                return create_Point();
            case 1 :
                return create_Circle();
            case 2 :
                return create_Polyline();
            case 3 :
                return create_Polygon();
            case 4 :
                return create_Rect();
            default:
                return create_Square();
        }
    }

    virtual ~Random_shape_factory() = default;
private:
    Random_shape_factory() = default;
    Random_shape_factory(Random_shape_factory&) = delete;
    Random_shape_factory& operator=(const Random_shape_factory&) = delete;

    int get_rnd_value(){
        static auto init_val = (int)std::time(nullptr);
        std::srand((unsigned int)init_val);
        init_val = std::rand();
        return init_val % 100;
    }
};


#endif //OOP_SHAPE_FACTORY_H
