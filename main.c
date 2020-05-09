/*****************************************************************************
 * file: menu.c
 * description: Top-level functionality for the PETB. Based off of Xilinx IPI.
 * version: 0.03
 *
 * author: Irfan Punekar
 * company: Precision Optical Transceivers
 * date: 02 October 2018
 *
 *****************************************************************************/

#include "xparameters.h"	/* defines XPAR values */
#include "xuartns550_l.h"

#include <xil_printf.h>
#include <xil_types.h>

#include "command_caller/command_caller.h"
#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN_COMMAND 50

/**
 * Main controlling thread of the program. It all starts here
 *
 * Get the command from the user, no more than 49 bytes, and execute the command
 * A command ends when a newline character is read or 49 bytes is reached
 */
int main() {
	init_platform();

	for(;;) {
		char command[MAX_LEN_COMMAND];
		command[MAX_LEN_COMMAND - 1] = '\0';

		char read;
		for (int i = 0; i < MAX_LEN_COMMAND - 1; i++) {
		    read = inbyte();
            command[i] = read;
		    if (read == '\n') {
		        command[i + 1] = '\0';
		        break;
		    }
		}

        call_command(parseCommand(command));
    }

	cleanup_platform();
	return 0;
}
