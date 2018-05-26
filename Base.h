#ifndef OOP_BASE_H
#define OOP_BASE_H

/*Задание №2.
 *   Создать абстрактный класс Printable – базовый класс для всех сущностей, информацию о которых можно представить (распечатать) в виде строки (std::string).
 *   Создать класс Named – наследник Printable – базовый класс для всех сущностей, которые имеют имя. Строка с именем без default-значения
 *   передаётся в единственный конструктор.
 *   Создать абстрактный класс Shape – наследник Printable – базовый класс для фигур на декартовой плоскости.
 *
 *   Спроектировать и реализовать иерархию классов конкретных именованных (т.е. наследников Named) фигур:
 *   Point - точка, +
 *   Circle - окружность, +
 *   Rect - прямоугольник со сторонами, параллельными OX и OY (с помощью двух точек), +
 *   Square - прямоугольник с одинаковыми сторонами,
 *   Polyline - ломаная; должна быть реализована с помощью Container< Point >, наполняться с помощью метода AddPoint( Point const & point ),
 *   Polygon - произвольный многоугольник.
 *
 *   С помощью статического метода Shape::GetCount() реализовать возможность узнать, сколько существует экземпляров Shape (т.е. любых фигур) в данный момент.
 *   У каждой фигуры в качестве текстовой информации должна печататься содержательная информация, например, у точки есть координаты, у окружности ещё есть радиус и площадь,
 *   у ломаной - длина и т.п.
 *   Необходимо реализовать также и вывод информации о каждом объекте в стандартный поток вывода std::ostream с помощью оператора
 *   std::ostream & operator << ( std::ostream & ioStream, ... );
 *
 *   Реализовать фабрику фигур, которая умеет создавать конкретную фигуру заданного типа со случайными параметрами.
*/


#define PI 3.1415

std::string unified_string(std::string name, const std::string &info) {
    name += " \t: ";
    return name += info;
}

class Printable
{
public:
    virtual std::string get_info()  const =  0;
    virtual ~Printable() = default;
protected:
private:
};


/////////////////////////////////////////////////////////////////////////////////

class Named : protected Printable{
public:
    Named(std::string init_string) : name(init_string) { }
    Named() = delete;
    Named(Named&) = delete;

    virtual std::string get_info() const{
        return name;
    }
    virtual ~Named() { }
    friend std::ostream& operator<<(std::ostream& os, const Named& dt);//not need
protected:

private:
    std::string name;
};
std::ostream& operator<<(std::ostream& os, const Named& dt)
{
    os << dt.get_info();
    return os;
}




#endif //OOP_BASE_H
