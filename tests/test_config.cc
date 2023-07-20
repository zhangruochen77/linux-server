#include "server/log/log.h"
#include "server/config/config.h"

server::ConfigVar<int>::ptr g_int_value_config =
    server::Config::LookUp("system.port", (int)8080, "system.port");


void test_config() {
    INFO(SIG_LOG_ROOT())
    << " val " << g_int_value_config->toString();
}

int main()
{
    test_config();
    return 0;
}