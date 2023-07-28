#include "server/thread/thread.h"
#include "server/log/log.h"
#include <string>

void fun1() {
    for (int i = 0; i < 10; i++) {
        server::Thread t([](){
            INFO(SIG_LOG_ROOT()) << " get thread id = " << server::Thread::GetPid() << " thread name " << server::Thread::GetName();
        }, std::string("thread:" + i));
    }
}

int main() {
    fun1();

    return 0;
}
