#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	command retVal;
	char* commandStr;

	for(;;)
	{
		commandStr = malloc(120);
		size_t len = 120;
		getline(&commandStr, &len, stdin);
		retVal = parseCommand(commandStr);
		printf("%hhx:", retVal.command_code);
		for(int i = 0; i < retVal.args_len; i++)
		{
			printf(" %hhx,", retVal.args[i]);
		}
		printf("\n");
		free(commandStr);
	}

}
