/*
 * Задание №3.
 * Составить и отладить программу:
 * - создать Container фигур, наполнить его в цикле конкретными фигурами (не менее 20 штук) случайного типа с помощью фабрики фигур.
 * - вывести в std::cout общее кол-во фигур с помощью Shape::GetCount();
 * - напечатать в std::cout сведения обо всех фигурах;
 * - очистить память (удалить все фигуры);
 * - вывести в std::cout общее кол-во фигур с помощью Shape::GetCount() – если оказалось не 0, разобраться, почему, и исправить;
 * - завершить работу программы.
 *
*/


#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <chrono>
#include <deque>
#include <cassert>


#include "Base.h"
#include "Shapes.h"
#include "Shape_factory.h"


int main(){
    auto& Factory = Random_shape_factory::get_instance();
    Container<std::unique_ptr<Shape>> Shapes;
    UnaryF_const<std::unique_ptr<Shape>> print_info = [](const std::unique_ptr<Shape>& sp) { std::cout << sp->get_info() << '\n';};

    for(auto iter = 0; iter < 21;++iter)
        Shapes.push_back(Factory.create_random_shape());
    std::cout << Shape::GetCount() << '\n';

    Shapes.for_each(print_info);
    Shapes.clear();

    std::cout << Shape::GetCount() << '\n';
    return  0;
}