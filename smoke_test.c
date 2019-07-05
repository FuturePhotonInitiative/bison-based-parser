#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

/* This is the version before the EEs want it to be changed */
char *TEST_COMMANDS[] = {
    "petb gpio read 0 modsel",
    "petb gpio set 0 modsel",
    "petb gpio clear 0 modsel",
    "petb gpio toggle 0 modsel",
    "pek gpio read 0 0",
    "pek gpio set 0 0",
    "pek gpio clear 0 0",
    "pek gpio toggle 0 0",
    "pek gpio write 0 0 0",
    "pek gpio list",
    "adc read 0",
    "dac write",
    "spixfer string",
    "spilb string",
    "i2cwrite 0 0",
    "i2cread 0 0",
    "petb bert 0",
    "petb eyescan 0 0 64 1 0 1",
    "debug iic",
    "debug gpio",
    "cfp gpio read 0"
};

int main() {
	command retVal;

    FILE *fp;
    fp = fopen("file2.txt", "w");

	for (int i = 0; i < sizeof(TEST_COMMANDS); i++) {
		char *commandStr = TEST_COMMANDS[i];
		retVal = parseCommand(commandStr);
        fprintf(fp, "%c Command: %s\n", retVal.command_code == i ? '*' : 'X', TEST_COMMANDS[i]);
        fprintf(fp, "{\n\tcommand_code: %d\n\t", retVal.command_code);
        for(int i = 0; i < retVal.args_len - 1; i++) {
			fprintf(fp, "%hhx, ", retVal.args[i]);
		}
        fprintf(fp, "%hhx\n}\n", retVal.args[retVal.args_len - 1]);
        fflush(fp);
	}

    fflush(fp);
    fclose(fp);

}