/*
* Задание №1.
* Спроектировать и реализовать шаблонный класс Container со следующими возможностями:
* - добавить новый элемент в начало/конец; +
* - удалить первый/последний элемент; +
* - получить значение первого/последнего элемента; +
* - перебрать все элементы (не обязательно делать итератор); +
* - узнать кол-во элементов; +
* - проверить на пустоту; +
* - очистить. +
*
* Класс должен быть эффективным и универсальным (должен подходить для хранения экземпляров любых типов и использования где угодно).
* Для тех, кто в танке: не надо реализовывать Container с помощью какого-либо готового класса контейнера.
*/


#ifndef OOP_CONTAINER_H
#define OOP_CONTAINER_H

#include <functional>

template <typename T>
using UnaryF       = std::function<void(T&)>;
template <typename T>
using UnaryF_const = std::function<void(const T&)>;


template <typename T>
class dynamic_ring_buffer{
public:
    dynamic_ring_buffer(std::size_t init_size = 16) : ring_buffer(nullptr),full_size(init_size),head_indx(0),tail_indx(0) {
        ring_buffer = (T*)malloc(sizeof(T) * full_size);
        if(ring_buffer == NULL)
            throw(std::bad_alloc());
    }
    ~dynamic_ring_buffer() {
        clear();
        free(ring_buffer);
    }

    template <typename N>
    void push_back(N&& n){
        constructor_call(ring_buffer[tail_indx],n);

        tail_indx = inc_value(tail_indx);
        if(tail_indx == head_indx)
            expansion();
    }
    template <typename N>
    void push_front(N&& n){
        head_indx = dec_value(head_indx);
        constructor_call(ring_buffer[head_indx],n);
        if(head_indx == tail_indx)
            expansion();
    }

    void pop_back(){
        if(head_indx == tail_indx)
            throw(std::logic_error("Empty Container\n"));

        destructor_call(ring_buffer[head_indx]);
        head_indx = inc_value(head_indx);

        if(contraction7())
            contraction();
    }
    void pop_front(){
        if(head_indx == tail_indx)
            throw(std::logic_error("Empty Container\n"));

        tail_indx = dec_value(tail_indx);
        destructor_call(ring_buffer[tail_indx]);

        if(contraction7())
            contraction();
    }

    T& operator[](std::size_t idx){
        if(idx >= size())
            throw(std::range_error("Out of border\n"));
        idx = (head_indx + idx) % full_size;
        return ring_buffer[idx];
    }
    const T& operator[](std::size_t idx) const{
        if(idx >= size())
            throw(std::range_error("Out of border\n"));
        idx = (head_indx + idx) % full_size;
        return ring_buffer[idx];
    }

    T& back(){
        return operator[](size()-1);
    }
    const T& back() const{
        return operator[](size()-1);
    }

    T& front(){
        return operator[](0);
    }
    const T& front() const{
        return operator[](0);
    }


    void for_each(UnaryF<T>& F){
        for(auto iter = head_indx; iter < (head_indx > tail_indx) * full_size;++iter)
            F(ring_buffer[iter]);
        for(auto iter = (tail_indx >= head_indx) * head_indx;iter < tail_indx;++iter)
            F(ring_buffer[iter]);
    }
    void for_each(UnaryF_const<T> & F) const {
        for(auto iter = head_indx; iter < (head_indx > tail_indx) * full_size;++iter)
            F(ring_buffer[iter]);
        for(auto iter = (tail_indx >= head_indx) * head_indx;iter < tail_indx;++iter)
            F(ring_buffer[iter]);
    }

    std::size_t size() const {
        return  (head_indx > tail_indx) * (full_size - head_indx) + tail_indx - head_indx * (tail_indx >= head_indx);
        //not fast,but beauty
/*        std::size_t ret_val_1 = 0;
        UnaryF_const counter = [&ret_val_1](T& t) { ++ret_val_1; };
        for_each(counter);

        return ret_val_1;*/
    }
    void clear(){
        UnaryF<T> destructor = destructor_call;
        for_each(destructor);
        head_indx = tail_indx;
    }

private:
    bool contraction7() const{
        auto cur_size = size();
        auto free_space = full_size - cur_size;
        return (full_size > 8) && free_space >= cur_size*3;
    }
    void contraction(){
        auto nw_full_size = size();
        auto nw_ring_buffer = (T*)malloc(sizeof(T) * nw_full_size);
        if(!ring_buffer)
            return;

        std::size_t counter = 0;
        UnaryF<T> mover = [&counter,&nw_ring_buffer](T& t) { nw_ring_buffer[counter++] = std::move(t); destructor_call(t); };
        for_each(mover);

        free(ring_buffer);
        ring_buffer = std::exchange(nw_ring_buffer, nullptr);

        full_size = nw_full_size;
        head_indx = 0;
        tail_indx = counter;
    }
    void expansion(){
        auto nw_full_size = 2 * full_size;

        auto nw_ring_buffer = (T*)malloc(sizeof(T) * nw_full_size);
        if(!ring_buffer)
            throw(std::bad_alloc());

        std::size_t counter = 0;
        UnaryF<T> mover = [&counter,&nw_ring_buffer](T& t) { constructor_call(nw_ring_buffer[counter++],t); destructor_call(t); };

        for(auto iter = head_indx; iter < full_size;++iter)
            mover(ring_buffer[iter]);

        for(auto iter = (head_indx ? 0 : tail_indx);iter < tail_indx;++iter)
            mover(ring_buffer[iter]);

        free(ring_buffer);
        ring_buffer = std::exchange(nw_ring_buffer, nullptr);

        full_size = nw_full_size;
        head_indx = 0;
        tail_indx = counter;
    }


    std::size_t dec_value(std::size_t v) const {
        auto nw_v = v + full_size - 1;
        return nw_v % full_size;
    }
    std::size_t inc_value(std::size_t v) const {
        auto nw_v = v + 1;
        return nw_v % full_size;
    }

    static void destructor_call(T& t){
        t.~T();
    }
    template <typename N>
    static void constructor_call(T& t,N&& v){
        new(&t)T(std::move(v));
    }

    T* ring_buffer;
    std::size_t   full_size;
    std::size_t   head_indx,tail_indx;//[)
};

template <typename T>
using Container = dynamic_ring_buffer<T>;

#endif //OOP_CONTAINER_H
