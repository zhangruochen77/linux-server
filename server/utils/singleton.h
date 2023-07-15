/**
 * 单例类
 */

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

namespace server
{
    /**
     * @brief 目标定义生产一个单例类
     */
    template <class T>
    class Singleton
    {
    public:
        /**
         * @brief 静态开辟一个单例对象
         */
        static T* getInstance()
        {
            static T instance;
            return &instance;
        }
    };
}

#endif