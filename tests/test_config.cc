#include "server/log/log.h"
#include "server/config/config.h"
#include <yaml-cpp/yaml.h>

server::ConfigVar<int>::ptr g_int_value_config =
    server::Config::Lookup("system.port", (int)8080, "system.port");

server::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config =
    server::Config::Lookup("system.int_vec", std::vector<int>{1, 2}, "system int vec");

server::ConfigVar<std::list<int>>::ptr g_int_list_value_config =
    server::Config::Lookup("system.int_list", std::list<int>{1, 2}, "system int list");

server::ConfigVar<std::set<int>>::ptr g_int_set_value_config =
    server::Config::Lookup("system.int_set", std::set<int>{1, 2}, "system int set");

server::ConfigVar<std::unordered_set<int>>::ptr g_int_uset_value_config =
    server::Config::Lookup("system.int_uset", std::unordered_set<int>{1, 2}, "system int uset");

server::ConfigVar<std::map<std::string, int>>::ptr g_str_int_map_value_config =
    server::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k", 2}}, "system str int map");

server::ConfigVar<std::unordered_map<std::string, int>>::ptr g_str_int_umap_value_config =
    server::Config::Lookup("system.str_int_umap", std::unordered_map<std::string, int>{{"k", 2}}, "system str int map");

void test_config()
{
    INFO(SIG_LOG_ROOT()) << " val " << g_int_value_config->getValue();
    INFO(SIG_LOG_ROOT()) << " val " << g_int_value_config->toString();
    // INFO(SIG_LOG_ROOT()) << " val " << g_int_vec_value_config->getValue();
    INFO(SIG_LOG_ROOT()) << " val " << g_int_vec_value_config->toString();
    // INFO(SIG_LOG_ROOT()) << " val " << g_int_list_value_config->getValue();
    INFO(SIG_LOG_ROOT()) << " val " << g_int_list_value_config->toString();
    // INFO(SIG_LOG_ROOT()) << " val " << g_int_set_value_config->getValue();
    INFO(SIG_LOG_ROOT()) << " val " << g_int_set_value_config->toString();
    // INFO(SIG_LOG_ROOT()) << " val " << g_int_uset_value_config->getValue();
    INFO(SIG_LOG_ROOT()) << " val " << g_int_uset_value_config->toString();
    // INFO(SIG_LOG_ROOT()) << " val " << g_str_int_map_value_config->getValue();
    INFO(SIG_LOG_ROOT()) << " val " << g_str_int_map_value_config->toString();
    // INFO(SIG_LOG_ROOT()) << " val " << g_str_int_umap_value_config->getValue();
    INFO(SIG_LOG_ROOT()) << " val " << g_str_int_umap_value_config->toString();

#define CLO(l)                                  \
    for (auto v : l)                            \
    {                                           \
        INFO(SIG_LOG_ROOT()) << " value " << v; \
    }

#define MAP(m)               \
    for (auto v : m)         \
    {                        \
        INFO(SIG_LOG_ROOT()) \
            << " key "       \
            << v.first       \
            << " value "     \
            << v.second;     \
    }

    CLO(g_int_vec_value_config->getValue());
    CLO(g_int_list_value_config->getValue());
    CLO(g_int_set_value_config->getValue());
    CLO(g_int_uset_value_config->getValue());
    MAP(g_str_int_map_value_config->getValue());
    MAP(g_str_int_umap_value_config->getValue());
}

/**
 * 打印 yaml 格式数据
 */
void print_yaml(const YAML::Node &node, int level)
{
    if (node.IsScalar())
    {
        INFO(SIG_LOG_ROOT())
            << std::string(level * 4, ' ')
            << node.Scalar()
            << " - " << node.Type()
            << " - " << level
            << " scalar";
    }
    else if (node.IsNull())
    {
        INFO(SIG_LOG_ROOT())
            << std::string(level * 4, ' ')
            << "NULL - " << node.Type()
            << " - " << level
            << " null";
    }
    else if (node.IsMap())
    {
        for (auto it = node.begin();
             it != node.end(); ++it)
        {
            INFO(SIG_LOG_ROOT())
                << std::string(level * 4, ' ')
                << it->first << " - "
                << it->second.Type() << " - " << level
                << " map";
            print_yaml(it->second, level + 1);
        }
    }
    else if (node.IsSequence())
    {
        for (size_t i = 0; i < node.size(); ++i)
        {
            INFO(SIG_LOG_ROOT())
                << std::string(level * 4, ' ')
                << i << " - " << node[i].Type()
                << " - " << level
                << " sequence ";
            print_yaml(node[i], level + 1);
        }
    }
}

void mult_config()
{
}

void test_yml()
{
    YAML::Node root = YAML::LoadFile("/opt/rc/project/linux-server/conf/log.yml");
    // print_yaml(root, 0);
    server::Config::LoadFromYaml(root);
}

int main()
{
    // test_config();
    test_yml();
    // mult_config();

    // server::ConfigVar<int>::ptr p = server::Config::Lookup("server.port", (int)8080, "");
    // p->getValue();
    return 0;
}