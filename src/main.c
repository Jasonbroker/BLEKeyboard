
#include "print.h"

int main(int argc, char const *argv[])
{
    log_init();
    
    printf("init done");
    dprintf("init debug done");

    for (;;) {
        NRF_LOG_PROCESS();
    }
}
