#include "../exercise.h"
#include <memory>

// 阅读资料:
// std::shared_ptr 文档: https://zh.cppreference.com/w/cpp/memory/shared_ptr
// std::weak_ptr 文档: https://zh.cppreference.com/w/cpp/memory/weak_ptr

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    // 创建一个指向整数10的shared_ptr
    auto shared = std::make_shared<int>(10);
    
    // 创建一个包含三个shared_ptr的数组，这三个shared_ptr都指向shared
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    // 创建一个weak_ptr，观察shared的引用计数
    std::weak_ptr<int> observer = shared;
    // 此时，shared有四个引用：shared + ptrs[0] + ptrs[1] + ptrs[2]
    ASSERT(observer.use_count() == 4, "");

    // 重置ptrs[0]，减少一个引用
    ptrs[0].reset();
    // 现在，shared有三个引用：shared + ptrs[1] + ptrs[2]
    ASSERT(observer.use_count() == 3, "");

    // 将ptrs[1]设为nullptr，减少一个引用
    ptrs[1] = nullptr;
    // 现在，shared有两个引用：shared + ptrs[2]
    ASSERT(observer.use_count() == 2, "");

    // 将ptrs[2]指向一个新的shared_ptr，指向一个与shared相同值的新整数
    ptrs[2] = std::make_shared<int>(*shared);
    // 现在，shared只有一个引用：shared
    ASSERT(observer.use_count() == 1, "");

    // 将ptrs[0]重新指向shared，引用计数加1
    ptrs[0] = shared;
    // 引用计数变为2：shared + ptrs[0]
    
    // 将ptrs[1]重新指向shared，引用计数加1
    ptrs[1] = shared;
    // 引用计数变为3：shared + ptrs[0] + ptrs[1]
    
    // 将ptrs[2]移动shared，shared指向ptrs[2]，原ptrs[2]指向与shared相同的新整数
    ptrs[2] = std::move(shared);
    // 此时，shared已被移动，引用计数为3：ptrs[0] + ptrs[1] + ptrs[2]
    ASSERT(observer.use_count() == 3, "");

    // 移动ptrs[0]，使其不再指向shared
    std::ignore = std::move(ptrs[0]);
    
    // 移动ptrs[1]，先移动自身，实际上没有变化
    ptrs[1] = std::move(ptrs[1]);
    
    // 移动ptrs[2]到ptrs[1]，ptrs[1]指向shared，引用计数保持不变
    ptrs[1] = std::move(ptrs[2]);
    // 此时，只有ptrs[1]指向shared，所以引用计数为1
    ASSERT(observer.use_count() == 2, "");

    // 使用observer.lock()获取shared_ptr，引用计数加1
    shared = observer.lock();
    // 引用计数变为2：ptrs[1] + shared
    ASSERT(observer.use_count() == 3, "");

    // 将shared设为nullptr，引用计数减1
    shared = nullptr;
    ASSERT(observer.use_count() == 2, "");
    // 将数组中的ptrs全部设为nullptr，引用计数归零
    for (auto &ptr : ptrs) ptr = nullptr;
    // 引用计数为0
    ASSERT(observer.use_count() == 0, "");

    // 再次使用observer.lock()，因为对象已被销毁，shared为空
    shared = observer.lock();
    // 引用计数为0
    ASSERT(observer.use_count() == 0, "");

    return 0;
}
