#pragma once

#include <string>
#include <cstddef>
#include <vector>

struct State {
    size_t ref_count = 1;

    std::vector<std::string> value;
};

class COWVector {
public:
    COWVector() {
        DeleteState();
        state_ = new State;
    }

    ~COWVector() {
        DeleteState();
    }

    COWVector(const COWVector& other) {
        if (this != &other) {
            DeleteState();
            state_ = other.state_;
            ++state_->ref_count;
        }
    }
    COWVector& operator=(const COWVector& other) {
        if (this != &other) {
            DeleteState();
            state_ = other.state_;
            ++state_->ref_count;
        }
        return *this;
    }

    size_t Size() const {
        return state_->value.size();
    }

    void Resize(size_t size) {
        if (size != state_->value.size()) {
            CopyState();
            state_->value.resize(size);
        }
    }

    const std::string& Get(size_t at) const {
        return state_->value[at];
    }
    const std::string& Back() const {
        return state_->value.back();
    }

    void PushBack(const std::string& value) {
        CopyState();
        state_->value.push_back(value);
    }

    void Set(size_t at, const std::string& value) {
        CopyState();
        state_->value[at] = value;
    }

private:
    void DeleteState() {
        if (state_ && --state_->ref_count == 0) {
            delete state_;
        }
    }

    void CopyState() {
        if (state_->ref_count > 1) {
            State* old = state_;
            --state_->ref_count;
            state_ = new State;
            state_->value.resize(old->value.size());

            for (size_t index = 0; index < state_->value.size(); ++index) {
                state_->value[index] = old->value[index];
            }
        }
    }

    State* state_ = nullptr;
};
