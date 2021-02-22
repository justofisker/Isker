#pragma once

#define SINGLETON(x) \
public: \
    static x& Get() \
    { \
        static x instance; \
        return instance; \
    } \
private: \
    x() {} \
public: \
    x(x const&) = delete; \
    void operator=(x const&) = delete;
