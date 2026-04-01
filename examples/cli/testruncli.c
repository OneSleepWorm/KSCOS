//--locale=english --no-multibyte-chars

#include "osconnect.h"
#include "KSCbasicdrawN.h"
#include "CLI.h"

int main(){
    kinitscreen(240,320,NULL,0,0);
    kconnect_init();
    kconnect_receive_init();
    kprintf("Hello world!\n");
    while(1){
            run_cli_root(kfgetc());
    }
}
