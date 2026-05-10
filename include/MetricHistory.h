#include <deque>
#include <iostream>
#pragma once


template <typename T>
class MetricHistory{
private:
    size_t maxSize;
    std::deque<T> history;
public:
    MetricHistory(size_t maxSize) : maxSize(maxSize) {}

    void add(const T& value) {
        if (history.size() == maxSize)
            history.pop_front();
        history.push_back(value);
    }

    const std::deque<T>& getHistory() const {
        return history;
    }

    size_t size() const {
        return history.size();
    }

    T latest() const {
        if (history.empty())
            throw std::runtime_error("History is empty");
        return history.back();
    }
};