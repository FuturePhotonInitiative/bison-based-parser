#include "bison_flex/parse_codes.h"
#include <stdio.h>

/**
 * A repl to test a command entered and show the parsing debug print output
 * Press ctrl + C to exit
 */
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
            /* print the string of a pek iic write command */
            if (ret_val.command_code == COMMAND_PEK_IIC_WRITE && i == 3) {
                printf("\"%s\"", ret_val.args + 3);
                break;
            }

            /* print the string of an error message */
            if (i == 0 && ret_val.args[0] == 255) {
                printf("%d,", ret_val.args[0]);
                putchar('\"');
                printf("%s", (char *)ret_val.args + 1);
                putchar('\"');
                break;
            }

            printf("%d", ret_val.args[i]);

            if (i != (ret_val.args_len - 1)) {
                printf(",");
            }
        }
        printf("]\n");
        printf("}\n");  
    }
    
}
