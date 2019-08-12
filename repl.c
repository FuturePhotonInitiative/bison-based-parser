#include "parser.h"
#include <stdio.h>
#include <string.h>

int main() {

    for (;;) {
        printf("> ");
        char from_user[256];
        fgets(from_user, sizeof(from_user), stdin);
        command ret_val = parseCommand(from_user);
        printf("\n");
        printf("command: %s{\n", from_user);
        printf("command code: %d\n", ret_val.command_code);
        printf("command args: [");
        for (int i = 0; i < ret_val.args_len; i++) {
            if (ret_val.command_code == COMMAND_PEK_IIC_WRITE && i == 3) {
                printf("\"%s\"", ret_val.args + 3);
                break;
            }
            else {
                printf("%d", ret_val.args[i]);
            }

            if (i != (ret_val.args_len - 1)) {
                printf(",");
            }
        }
        printf("]\n");
        printf("}\n");
        // for (int i = 0; i < ret_val.args_len; i++) {
        //     printf("%d\n", ret_val.args[i]);
        // }    
    }
    
}
