/**
 * 配置框架
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <algorithm>

namespace server
{
    /**
     * 抽象类 变量类型基础类
     */
    class ConfigBarBase
    {
    public:
        /**
         * 构造函数 名称 描述初始化
         */
        ConfigBarBase(const std::string &name, const std::string &description);
        ~ConfigBarBase();

        const std::string &getName() const { return m_name; }
        const std::string &getDescription() const { return m_description; }

        virtual std::string toString() = 0;

    protected:
        std::string m_name;        // 变量名称
        std::string m_description; // 变量描述

    private:
    };
}

#endif // !__CONFIG_H__