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

    static void ListAllMember(const std::string &prefix,
                              const YAML::Node &node,
                              std::list<std::pair<std::string, const YAML::Node>> &output)
    {
        if (prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789") != std::string::npos)
        {
            ERROR(SIG_LOG_ROOT()) << "Config invalid name: " << prefix << " : " << node;
            return;
        }
        output.push_back(std::make_pair(prefix, node));
        if (node.IsMap())
        {
            for (auto it = node.begin();
                 it != node.end(); ++it)
            {
                ListAllMember(prefix.empty() ? it->first.Scalar()
                                             : prefix + "." + it->first.Scalar(),
                              it->second, output);
            }
        }
    }
    
    void Config::LoadFromYaml(const YAML::Node &root)
    {
        std::list<std::pair<std::string, const YAML::Node>> all_nodes;
        ListAllMember("", root, all_nodes);

        for (auto &i : all_nodes)
        {
            std::string key = i.first;
            if (key.empty())
            {
                continue;
            }

            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            ConfigVarBase::ptr var = LookupBase(key);

            if (var)
            {
                if (i.second.IsScalar())
                {
                    var->fromString(i.second.Scalar());
                }
                else
                {
                    std::stringstream ss;
                    ss << i.second;
                    var->fromString(ss.str());
                }
            }
        }
    }

    static std::map<std::string, uint64_t> s_file2modifytime;

    void Config::LoadFromConfDir(const std::string &path, bool force)
    {
        // std::string absoulte_path = sylar::EnvMgr::GetInstance()->getAbsolutePath(path);
        std::vector<std::string> files;
        // FSUtil::ListAllFile(files, absoulte_path, ".yml");
        files.push_back(path);

        for (auto &i : files)
        {
            {
                struct stat st;
                lstat(i.c_str(), &st);
                // sylar::Mutex::Lock lock(s_mutex);
                if (!force && s_file2modifytime[i] == (uint64_t)st.st_mtime)
                {
                    continue;
                }
                s_file2modifytime[i] = st.st_mtime;
            }
            try
            {
                YAML::Node root = YAML::LoadFile(i);
                LoadFromYaml(root);
                INFO(SIG_LOG_ROOT())
                    << "LoadConfFile file="
                    << i << " ok";
            }
            catch (...)
            {
                INFO(SIG_LOG_ROOT())
                    << "LoadConfFile file="
                    << i << " failed";
            }
        }
    }

    ConfigVarBase::ptr Config::LookupBase(const std::string &name)
    {
        // RWMutexType::ReadLock lock(GetMutex());
        auto it = GetDatas().find(name);
        return it == GetDatas().end() ? nullptr : it->second;
    }

    void Config::Visit(std::function<void(ConfigVarBase::ptr)> cb)
    {
        // RWMutexType::ReadLock lock(GetMutex());
        ConfigVarMap &m = GetDatas();
        for (auto it = m.begin();
             it != m.end(); ++it)
        {
            cb(it->second);
        }
    }

    
}