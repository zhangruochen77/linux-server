#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

namespace server
{
    /**
     * 不可拷贝对象
     */
    class NonCopyAble
    {
    public:
        NonCopyAble() = default;
        ~NonCopyAble() = default;
        NonCopyAble(const NonCopyAble &) = delete;
        NonCopyAble &operator=(const NonCopyAble &) = delete;
    };
}

#endif