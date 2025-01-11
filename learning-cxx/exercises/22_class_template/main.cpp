#include "../exercise.h"
#include <cstring> // 包含 memcpy 的头文件
#include <stdexcept>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i]; // 复制 shape
            size *= shape[i];     // 计算 size
        }
        data = new T[size];       // 分配内存
        std::memcpy(data, data_, size * sizeof(T)); // 复制数据
    }

    ~Tensor4D() {
        delete[] data;
    }

    // 禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 实现单向广播的加法
    Tensor4D &operator+=(Tensor4D const &others) {
        // 检查形状是否兼容
        for (int i = 0; i < 4; ++i) {
            if (shape[i] != others.shape[i] && others.shape[i] != 1) {
                throw std::invalid_argument("Incompatible shapes for broadcasting.");
            }
        }

        // 计算每个维度的步长
        unsigned int stride_this[4] = {1, 1, 1, 1};
        unsigned int stride_others[4] = {1, 1, 1, 1};
        for (int i = 2; i >= 0; --i) {
            stride_this[i] = stride_this[i + 1] * shape[i + 1];
            stride_others[i] = stride_others[i + 1] * others.shape[i + 1];
        }

        // 遍历每个元素并相加
        for (unsigned int i = 0; i < shape[0]; ++i) {
            for (unsigned int j = 0; j < shape[1]; ++j) {
                for (unsigned int k = 0; k < shape[2]; ++k) {
                    for (unsigned int l = 0; l < shape[3]; ++l) {
                        // 计算 this 的索引
                        unsigned int idx_this = i * stride_this[0] + j * stride_this[1] + k * stride_this[2] + l * stride_this[3];
                        // 计算 others 的索引（考虑广播）
                        unsigned int idx_others = (i % others.shape[0]) * stride_others[0] +
                                                 (j % others.shape[1]) * stride_others[1] +
                                                 (k % others.shape[2]) * stride_others[2] +
                                                 (l % others.shape[3]) * stride_others[3];
                        // 相加
                        data[idx_this] += others.data[idx_others];
                    }
                }
            }
        }

        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        Tensor4D<int> t0(shape, data); // 直接初始化
        Tensor4D<int> t1(shape, data); // 直接初始化
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        Tensor4D<float> t0(s0, d0); // 直接初始化
        Tensor4D<float> t1(s1, d1); // 直接初始化
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        Tensor4D<double> t0(s0, d0); // 直接初始化
        Tensor4D<double> t1(s1, d1); // 直接初始化
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}