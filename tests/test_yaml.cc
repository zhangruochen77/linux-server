#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
    YAML::Load("../conf/log.yml");
    return 0;
}