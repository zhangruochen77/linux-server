#include <iostream>
#include "log.h"

int main() {
    server::LogFormmtter fmt("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
}