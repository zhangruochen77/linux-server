#include "server/thread/thread.h"
#include "server/log/log.h"
#include <string>

void fun1() {
    for (int i = 0; i < 1000; i++) {
        server::Thread t([](){
            INFO(SIG_LOG_ROOT()) << " get thread id = " << server::Thread::GetPid() << " thread name " << server::Thread::GetName();
        }, std::string("thread:") + std::to_string(i));
    }
}

int main() {
    fun1();

    sleep(1);
    return 0;
}
