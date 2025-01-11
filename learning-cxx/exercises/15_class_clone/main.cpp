#include "../exercise.h"

// READ: 复制构造函数 <https://zh.cppreference.com/w/cpp/language/copy_constructor>
// READ: 函数定义（显式弃置）<https://zh.cppreference.com/w/cpp/language/function>


class DynFibonacci {
    size_t *cache;
    int cached;
    int capacity;

public:
    // 实现动态设置容量的构造器
    DynFibonacci(int capacity) : cache(new size_t[capacity]), cached(2), capacity(capacity) {
        cache[0] = 0;
        cache[1] = 1;
    }

    // 实现复制构造器
    DynFibonacci(const DynFibonacci &other) : cache(new size_t[other.capacity]), cached(other.cached), capacity(other.capacity) {
        for (int i = 0; i < cached; ++i) {
            cache[i] = other.cache[i];
        }
    }

    // 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }

    // 实现正确的缓存优化斐波那契计算
    size_t get(int i) {
        if (i < 0) {
            ASSERT(false, "Index cannot be negative");
        }
        // 如果需要计算的索引超出当前容量，扩展容量
        if (i >= capacity) {
            // 简单扩展策略，例如翻倍
            int new_capacity = capacity * 2;
            size_t* new_cache = new size_t[new_capacity];
            for (int j = 0; j < cached; ++j) {
                new_cache[j] = cache[j];
            }
            delete[] cache;
            cache = new_cache;
            capacity = new_capacity;
        }
        // 扩展缓存
        while (i >= cached) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
            ++cached;
        }
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    // NOTICE: 名字相同参数也相同，但 const 修饰不同的方法是一对重载方法，可以同时存在
    //         本质上，方法是隐藏了 this 参数的函数
    //         const 修饰作用在 this 上，因此它们实际上参数不同
    size_t get(int i) const {
        if (i <= cached) {
            return cache[i];
        }
        ASSERT(false, "i out of range");
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    DynFibonacci const fib_ = fib;
    ASSERT(fib_.get(10) == fib.get(10), "Object cloned");
    return 0;
}
