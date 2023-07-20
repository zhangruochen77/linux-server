#include "config.h"

namespace server
{
    ConfigVarBase::ConfigVarBase(const std::string &name, const std::string &description)
        : m_name(name), m_description(description)
    {
        // 设置名称自动转化存储为小写方式
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    }

    ConfigVarBase::~ConfigVarBase() {}
}