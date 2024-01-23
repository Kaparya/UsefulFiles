#pragma once

#include <iterator>
#include <cstddef>

class Vector {
public:
    Vector() = default;
    Vector(size_t size) {
        delete[] data_;
        data_ = new int[size];
        capacity_ = size;
        size_ = size;
        for (size_t index = 0; index < size_; ++index) {
            data_[index] = 0;
        }
    }
    Vector(std::initializer_list<int> data) {
        delete[] data_;
        data_ = new int[data.size()];
        capacity_ = data.size();
        size_ = data.size();
        for (size_t index = 0; index < data.size(); ++index) {
            data_[index] = *(data.begin() + index);
        }
    }
    Vector(const Vector& rhs) {
        delete[] data_;
        if (rhs.Size() > 0) {
            data_ = new int[rhs.Size()];
        }
        capacity_ = rhs.Size();
        size_ = rhs.Size();
        for (size_t index = 0; index < rhs.Size(); ++index) {
            data_[index] = rhs[index];
        }
    }

    Vector(Vector&& rhs) {
        data_ = rhs.data_;
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        rhs.data_ = nullptr;
        rhs.size_ = 0;
        rhs.capacity_ = 0;
    }

    ~Vector() {
        if (data_ != nullptr) {
            delete[] data_;
        }
        capacity_ = 0;
    }

    void Swap(Vector& new_one) {
        Vector tmp(new_one);
        new_one = Vector(*this);
        *this = Vector(tmp);
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(const int& value) {
        if (size_ < capacity_) {
            data_[size_++] = value;
            return;
        }
        if (capacity_ == 0) {
            data_ = new int[1];
            data_[size_++] = value;
            ++capacity_;
            return;
        }

        capacity_ *= 2;
        int* new_data = new int[capacity_];
        for (size_t index = 0; index < size_; ++index) {
            new_data[index] = data_[index];
        }
        delete[] data_;
        data_ = new_data;
        data_[size_++] = value;
    }

    void PopBack() {
        --size_;
    }

    void Clear() {
        size_ = 0;
    }

    void Reserve(size_t reserve_capacity) {
        if (reserve_capacity <= capacity_) {
            return;
        }

        int* new_data = new int[reserve_capacity];
        for (size_t index = 0; index < size_; ++index) {
            new_data[index] = data_[index];
        }
        delete[] data_;
        capacity_ = reserve_capacity;
        data_ = new_data;
    }

    Vector& operator=(const Vector& rhs) {
        if (rhs.data_ != this->data_) {
            delete[] data_;
            data_ = new int[rhs.Size()];
            capacity_ = rhs.Size();
            size_ = rhs.Size();
            for (size_t index = 0; index < rhs.Size(); ++index) {
                data_[index] = rhs[index];
            }
        }
        return *this;
    }

    Vector& operator=(Vector&& moved) {
        if (&moved != this) {
            delete[] data_;
            data_ = moved.data_;
            capacity_ = moved.capacity_;
            size_ = moved.size_;
            moved.data_ = nullptr;
            moved.size_ = 0;
            moved.capacity_ = 0;
        }
        return *this;
    }

    int& operator[](size_t index) {
        return data_[index];
    }
    int operator[](size_t index) const {
        return data_[index];
    }

    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = int;
        using difference_type = ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        Iterator() = default;
        Iterator(pointer ptr) : m_ptr_(ptr){};

        value_type& operator[](difference_type index) const {
            return *(m_ptr_ + index);
        }

        reference operator*() const {
            return *m_ptr_;
        }
        pointer operator->() {
            return m_ptr_;
        }

        Iterator& operator++() {
            ++m_ptr_;
            return *this;
        }
        Iterator& operator--() {
            --m_ptr_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        Iterator operator--(int) {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        Iterator& operator+=(difference_type n) {
            m_ptr_ += n;
            return *this;
        }
        Iterator& operator-=(difference_type n) {
            m_ptr_ -= n;
            return *this;
        }

        difference_type operator-(const Iterator& rhs) {
            return m_ptr_ - rhs.m_ptr_;
        }
        friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
            return lhs.m_ptr_ - rhs.m_ptr_;
        }

        Iterator operator+(difference_type rhs) const {
            return Iterator(m_ptr_ + rhs);
        }
        Iterator operator-(difference_type rhs) const {
            return Iterator(m_ptr_ - rhs);
        }

        friend Iterator operator+(difference_type lhs, const Iterator& rhs) {
            return Iterator(lhs + rhs.m_ptr_);
        }
        friend Iterator operator-(difference_type lhs, const Iterator& rhs) {
            return Iterator(-lhs + rhs.m_ptr_);
        }

        auto operator<=>(const Iterator&) const = default;

    private:
        pointer m_ptr_;
    };

    Iterator begin() {
        return Iterator(data_);
    }
    Iterator end() {
        return Iterator(data_ + size_);
    }

private:
    int* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;
};