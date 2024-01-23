#pragma once

#include <string>

struct Block {
    int strong = 0;
    int weak = 0;
};

class WeakPtr;

class SharedPtr {
    friend WeakPtr;

public:
    SharedPtr() = default;
    SharedPtr(std::string* value) {
        if (counter_ && counter_->strong == 0) {
            delete value_;
            value_ = nullptr;
            if (counter_->weak == 0) {
                delete counter_;
                counter_ = nullptr;
            }
        }
        value_ = value;
        counter_ = new Block{1, 0};
    }
    ~SharedPtr() {
        if (!counter_) {
            return;
        }
        if (counter_ && --counter_->strong == 0) {
            delete value_;
            value_ = nullptr;
            if (counter_->weak == 0) {
                delete counter_;
                counter_ = nullptr;
            }
        }
    }
    SharedPtr(const WeakPtr& ptr);
    SharedPtr(const SharedPtr& other) {
        value_ = other.value_;
        counter_ = other.counter_;
        if (counter_) {
            ++counter_->strong;
        }
    }
    SharedPtr(SharedPtr&& other) {
        value_ = other.value_;
        counter_ = other.counter_;
        other.value_ = nullptr;
        other.counter_ = nullptr;
    }
    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other) {
            return *this;
        }
        if (counter_) {
            --counter_->strong;
        }
        value_ = other.value_;
        counter_ = other.counter_;
        if (counter_) {
            ++counter_->strong;
        }
        return *this;
    }
    SharedPtr& operator=(SharedPtr&& other) {
        if (this == &other) {
            return *this;
        }
        if (counter_) {
            --counter_->strong;
        }
        value_ = other.value_;
        counter_ = other.counter_;
        other.value_ = nullptr;
        other.counter_ = nullptr;
        return *this;
    }

    std::string& operator*() const {
        return *value_;
    }
    std::string* operator->() const {
        return value_;
    }
    std::string* Get() {
        return value_;
    }

    void Reset(std::string* other) {
        if (counter_) {
            --counter_->strong;
        }
        value_ = other;
        counter_ = new Block{1, 0};
    }

private:
    std::string* value_ = nullptr;
    Block* counter_ = nullptr;
};

class WeakPtr {
public:
    WeakPtr() = default;
    WeakPtr(const SharedPtr& ptr) {
        value_ = ptr.value_;
        counter_ = ptr.counter_;
        if (counter_) {
            ++counter_->weak;
        }
    }
    WeakPtr(const WeakPtr& ptr) {
        value_ = ptr.value_;
        counter_ = ptr.counter_;
        if (counter_) {
            ++counter_->weak;
        }
    }
    WeakPtr(WeakPtr&& ptr) {
        value_ = ptr.value_;
        counter_ = ptr.counter_;
        ptr.value_ = nullptr;
        ptr.counter_ = nullptr;
    }
    ~WeakPtr() {
        if (counter_) {
            --counter_->weak;
        }
    }

    WeakPtr& operator=(const WeakPtr& other) {
        if (this == &other) {
            return *this;
        }
        if (counter_) {
            --counter_->weak;
        }
        value_ = other.value_;
        counter_ = other.counter_;
        if (counter_) {
            ++counter_->weak;
        }
        return *this;
    }
    WeakPtr& operator=(WeakPtr&& other) {
        if (this == &other) {
            return *this;
        }
        if (counter_) {
            --counter_->weak;
        }
        value_ = other.value_;
        counter_ = other.counter_;
        other.value_ = nullptr;
        other.counter_ = nullptr;
        return *this;
    }

    std::string& operator*() const {
        return *value_;
    }
    std::string* operator->() const {
        return value_;
    }
    std::string* Get() {
        return value_;
    }

    bool IsExpired() const {
        if (counter_) {
            return counter_->strong == 0;
        }
        return true;
    }

    SharedPtr Lock() const {
        SharedPtr rez;
        if (!IsExpired()) {
            rez.value_ = value_;
            rez.counter_ = counter_;
            if (counter_) {
                ++counter_->strong;
            }
        }
        return rez;
    }

private:
    std::string* value_ = nullptr;
    Block* counter_ = nullptr;
};

SharedPtr::SharedPtr(const WeakPtr& ptr) {
    SharedPtr help = ptr.Lock();
    value_ = help.value_;
    counter_ = help.counter_;
}
