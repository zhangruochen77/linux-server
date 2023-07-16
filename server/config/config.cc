#include "config.h"

namespace server
{
    ConfigBarBase::ConfigBarBase(const std::string &name, const std::string &description)
        : m_name(name), m_description(description)
    {
        // 设置名称自动转化存储为小写方式
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    }

    ConfigBarBase::~ConfigBarBase()
    {
    }
}