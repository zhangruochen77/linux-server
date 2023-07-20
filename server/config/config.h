/**
 * 配置框架
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <memory>
#include <typeinfo>
#include <algorithm>
#include <unordered_map>
#include <boost/lexical_cast.hpp>

#include "../log/log.h"

namespace server
{
    /**
     * 抽象类 变量类型基础类
     */
    class ConfigVarBase
    {
    public:
        typedef std::shared_ptr<ConfigVarBase> ptr;
        /**
         * 构造函数 名称 描述初始化
         */
        ConfigVarBase(const std::string &name, const std::string &description);
        ~ConfigVarBase();

        const std::string &getName() const { return m_name; }
        const std::string &getDescription() const { return m_description; }

        /**
         * 转换字符串
         */
        virtual std::string toString() = 0;

        /**
         * 从字符串初始化值
         */
        virtual bool fromString(const std::string &val) = 0;

        /**
         * 返回配置参数值的类型名称
         */
        virtual std::string getTypeName() const = 0;

    protected:
        std::string m_name;        // 变量名称
        std::string m_description; // 变量描述

    private:
    };

    /**
     * 定义从 from 类型到 target 类型的基本转换
     */
    template <class F, class T>
    class LexicalCast
    {
    public:
        /**
         * 普通类型转换伪函数
         */
        T operator()(const F &f)
        {
            return boost::lexical_cast<T>(f);
        }
    };

    /**
     * 配置变量实现类
     * T 实际变量的类型 对该类型进行转换
     * FromStr 从字符串转换为 T 类型的仿函数
     * ToStr 从 T 类型转换为字符串的仿函数
     */
    template <class T, class FromStr = LexicalCast<std::string, T>, class ToStr = LexicalCast<T, std::string>>
    class ConfigVar : public ConfigVarBase
    {
    public:
        typedef std::shared_ptr<ConfigVar> prt;
        /**
         * 初始化构造函数 对变量名称以及变量描述，变量值进行初始化
         */
        ConfigVar(const std::string &name, const T &val, const std::string &description = "")
            : ConfigVarBase(name, description), m_val(val) {}
        ~ConfigVar() {}

        /**
         * 获取变量字符串 yaml 格式
         */
        std::string toString() override
        {
            try
            {
                return ToStr()(m_val);
            }
            catch (const std::exception &e)
            {
                ERROR(SIG_LOG_ROOT())
                    << "ConfigVar::toString exception: "
                    << e.what()
                    << " convert: "
                    << typeid(this->m_val).name()
                    << "to string name "
                    << m_name;
            }

            return "";
        }

        /**
         * 从字符串初始化值
         */
        bool fromString(const std::string &val) override
        {
            try
            {
                this->setValue(FromStr()(val));
                return true;
            }
            catch (const std::exception &e)
            {
                ERROR(SIG_LOG_ROOT())
                    << "ConfigVar::fromString exception: "
                    << e.what()
                    << " convert:"
                    << val
                    << " to "
                    << typeid(this->m_val).name();
            }

            return false;
        }

        /**
         * 返回配置参数值的类型名称
         */
        std::string getTypeName() const override
        {
            return typeid(this->m_val).name();
        }

        void setValue(const T &val) { this->m_val = val; };
        const T getValue() const { return this->m_val; };

    private:
        T m_val; // 存放的变量值
    };

    /**
     * 配置中心类
     */
    class Config
    {
    public:
        typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

        /**
         * 在配置中心寻找值 如果值存在 那么进行转换返回
         * 如果值不存在 那么就将默认值添加
         * 但是值存在 但是类型不匹配 转换失败 那么返回 nullptr
         * 添加时存在name不满足匹配要求 那么抛出非法异常
         */
        template <class T>
        static typename ConfigVar<T>::ptr
        LookUp(const std::string &name,
               const T &default_value,
               const std::string &description = "")
        {
            auto it = GetDatas().find(name);
            if (GetDatas().end() != it)
            {
                // 存在情况 类型转换返回
                auto temp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
                if (temp)
                {
                    INFO(SIG_LOG_ROOT())
                        << "Config::LookUp get data: "
                        << name
                        << "exists";

                    return temp;
                }

                ERROR(SIG_LOG_ROOT())
                    << "Config::LookUp get data:"
                    << name
                    << "exists but type not "
                    << typeid(T).name()
                    << " really type is "
                    << it->second->getTypeName();

                return nullptr;
            }

            // 第一个参数检查
            if (std::string::npos !=
                name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789"))
            {
                ERROR(SIG_LOG_ROOT())
                    << "Config::LookUp exception: invalid name: "
                    << name;

                throw std::invalid_argument(name);
            }

            // 添加默认值并返回
            typename ConfigVar<T>::ptr p = std::make_shared<ConfigVar<T>>(name, default_value, description);
            GetDatas()[name] = p;
            return p;
        }

        /**
         * 在配置中心寻找值 如果值存在 那么进行转换返回
         * 如果值不存在 那么返回 nullptr
         */
        template <class T>
        static typename ConfigVar<T>::ptr
        LookUp(const std::string &name)
        {
            auto it = GetDatas().find(name);
            if (GetDatas().end() == it)
            {
                return nullptr;
            }

            return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
        }

    private:
        static ConfigVarMap &GetDatas()
        {
            static ConfigVarMap s_datas;
            return s_datas;
        }
    };
}

#endif // !__CONFIG_H__