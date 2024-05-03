#include <iostream>

template<typename T>
class MyUniquePtr {
private:
    T* ptr;

public:

    explicit MyUniquePtr(T* p = nullptr) : ptr(p) {}

    ~MyUniquePtr() {
        delete ptr;
    }

    MyUniquePtr(const MyUniquePtr&) = delete;
    MyUniquePtr& operator=(const MyUniquePtr&) = delete;

    MyUniquePtr(MyUniquePtr&& moving) noexcept {
        ptr = moving.ptr;
        moving.ptr = nullptr;
    }

    MyUniquePtr& operator=(MyUniquePtr&& moving) noexcept {
        if (this != &moving) {
            delete ptr;
            ptr = moving.ptr;
            moving.ptr = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }

    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T* p = nullptr) {
        T* old = ptr;
        ptr = p;
        delete old;
    }
};

#include <iostream>

template<typename T>
class MySharedPtr {
private:
    T* ptr;
    int* count;

public:
    explicit MySharedPtr(T* p = nullptr) : ptr(p), count(new int(1)) {
        if (!p) *count = 0;
    }

    MySharedPtr(const MySharedPtr& other) : ptr(other.ptr), count(other.count) {
        (*count)++;
    }

    MySharedPtr& operator=(const MySharedPtr& other) {
        if (this == &other) return *this;

        if (--(*count) == 0) {
            delete ptr;
            delete count;
        }

        ptr = other.ptr;
        count = other.count;
        (*count)++;
        return *this;
    }

    ~MySharedPtr() {
        if (--(*count) == 0) {
            delete ptr;
            delete count;
        }
    }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
};

