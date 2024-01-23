#pragma once

#include <cstddef>
#include <iterator>

struct BaseNode {
    BaseNode* next = nullptr;
    BaseNode* prev = nullptr;
};

template <class T>
class List {
public:
    struct ListNode : BaseNode {
        T value_;

        ListNode() = default;

        ListNode(const T& value) {
            value_ = value;
        }

        ListNode(T&& value) : value_(std::move(value)) {
        }
    };

    class Iterator {
        friend List;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator() = default;
        Iterator(BaseNode* ptr) {
            m_ptr_ = ptr;
        }

        Iterator& operator++() {
            m_ptr_ = m_ptr_->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp(m_ptr_);
            m_ptr_ = m_ptr_->next;
            return tmp;
        }

        Iterator& operator--() {
            m_ptr_ = m_ptr_->prev;
            return *this;
        }
        Iterator operator--(int) {
            Iterator tmp(m_ptr_);
            m_ptr_ = m_ptr_->prev;
            return tmp;
        }

        reference operator*() const {
            return static_cast<ListNode*>(m_ptr_)->value_;
        }
        pointer operator->() const {
            return &static_cast<ListNode*>(m_ptr_)->value_;
        }

        bool operator==(const Iterator& rhs) const {
            return m_ptr_ == rhs.m_ptr_;
        }
        bool operator!=(const Iterator& rhs) const {
            return m_ptr_ != rhs.m_ptr_;
        }

    private:
        BaseNode* m_ptr_ = nullptr;
    };

    void DeleteList() {
        while (first_ && first_->next != last_) {
            ListNode* tmp_list = static_cast<ListNode*>(first_->next);
            first_->next->next->prev = first_;
            first_->next = first_->next->next;

            tmp_list->next = tmp_list->prev = nullptr;
            delete tmp_list;
        }

        size_ = 0;

        if (first_ == nullptr) {
            first_ = new BaseNode;
            last_ = first_;
            first_->next = first_;
            first_->prev = first_;
        }
    }

    List() {
        DeleteList();
    }

    List(const List& rhs) {
        DeleteList();

        BaseNode *rhs_first = rhs.first_->next, *rhs_last = rhs.last_;

        while (rhs_first != rhs_last) {

            ListNode* new_node = new ListNode{static_cast<ListNode*>(rhs_first)->value_};
            new_node->prev = last_->prev;
            new_node->next = last_;
            last_->prev->next = new_node;
            last_->prev = new_node;

            rhs_first = rhs_first->next;
        }

        size_ = rhs.size_;
    }

    List(List&& rhs) {
        if (first_ != rhs.first_) {
            DeleteList();

            if (first_ != nullptr) {
                delete first_;
            }
            first_ = std::move(rhs.first_);
            last_ = std::move(rhs.last_);
            rhs.first_ = rhs.last_ = new BaseNode;
            rhs.first_->next = rhs.first_;
            rhs.first_->prev = rhs.first_;

            size_ = std::move(rhs.size_);
            rhs.size_ = 0;
        }
    }

    ~List() {
        DeleteList();

        delete first_;
    }

    List& operator=(const List& rhs) {
        if (first_ != rhs.first_) {
            DeleteList();

            BaseNode *rhs_first = rhs.first_->next, *rhs_last = rhs.last_;

            while (rhs_first != rhs_last) {

                ListNode* new_node = new ListNode{static_cast<ListNode*>(rhs_first)->value_};
                new_node->prev = last_->prev;
                new_node->next = last_;
                last_->prev->next = new_node;
                last_->prev = new_node;

                rhs_first = rhs_first->next;
            }

            size_ = rhs.size_;
        }
        return *this;
    }

    List& operator=(List&& rhs) {
        if (first_ != rhs.first_) {
            DeleteList();

            if (first_ != nullptr) {
                delete first_;
            }
            first_ = std::move(rhs.first_);
            last_ = std::move(rhs.last_);
            rhs.first_ = rhs.last_ = new BaseNode;
            rhs.first_->next = rhs.first_;
            rhs.first_->prev = rhs.first_;

            size_ = std::move(rhs.size_);
            rhs.size_ = 0;
        }
        return *this;
    }

    bool IsEmpty() const {
        return !size_;
    }
    size_t Size() const {
        return size_;
    }

    void PushBack(const T& value) {
        ++size_;

        ListNode* new_node = new ListNode{value};
        new_node->prev = last_->prev;
        new_node->next = last_;
        last_->prev->next = new_node;
        last_->prev = new_node;
    }
    void PushBack(T&& value) {
        ++size_;

        ListNode* new_node = new ListNode{std::forward<T>(value)};
        new_node->prev = last_->prev;
        new_node->next = last_;
        last_->prev->next = new_node;
        last_->prev = new_node;
    }

    void PushFront(const T& value) {
        ++size_;

        ListNode* new_node = new ListNode{value};
        new_node->prev = first_;
        new_node->next = first_->next;
        first_->next->prev = new_node;
        first_->next = new_node;
    }
    void PushFront(T&& value) {
        ++size_;

        ListNode* new_node = new ListNode{std::forward<T>(value)};
        new_node->prev = first_;
        new_node->next = first_->next;
        first_->next->prev = new_node;
        first_->next = new_node;
    }

    T& Front() {
        return static_cast<ListNode*>(first_->next)->value_;
    }
    const T& Front() const {
        return static_cast<ListNode*>(first_->next)->value_;
    }
    T& Back() {
        return static_cast<ListNode*>(last_->prev)->value_;
    }
    const T& Back() const {
        return static_cast<ListNode*>(last_->prev)->value_;
    }

    void PopBack() {
        --size_;

        ListNode* current = static_cast<ListNode*>(last_->prev);
        last_->prev->prev->next = last_;
        last_->prev = last_->prev->prev;

        delete current;
    }
    void PopFront() {
        --size_;

        ListNode* current = static_cast<ListNode*>(first_->next);
        first_->next->next->prev = first_;
        first_->next = first_->next->next;

        delete current;
    }

    void Erase(Iterator iter) {
        --size_;
        ListNode* current = static_cast<ListNode*>(iter.m_ptr_);

        current->prev->next = current->next;
        current->next->prev = current->prev;
        delete current;
    }

    Iterator Begin() {
        return Iterator(first_->next);
    }
    Iterator End() {
        return Iterator(last_);
    }

private:
    BaseNode* first_ = nullptr;
    BaseNode* last_ = nullptr;
    size_t size_ = 0;
};

template <class T>
List<T>::Iterator begin(List<T>& list) {
    return list.Begin();
}

template <class T>
List<T>::Iterator end(List<T>& list) {
    return list.End();
}
