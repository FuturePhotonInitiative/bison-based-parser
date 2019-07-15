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

#include "main.h"

/**
 * This function simply prints the prompt character to the command line
 */
void prompt() {
	print("\r\n>> ");
}

/**
 *	This function is used to prompt the user and get a command from
 *	the user input on the command line. It returns the command entered
 *	as a char*. (not working) In the case of the up or down arrow keys
 *	the function will instead go through previous commands and
 *	return whichever command is displayed when the user presses <enter>
 *	The left and right arrow keys will allow for the user to in-line
 *	edit the current command.
 */
void getUserInput(char *cmd, int rows, int cols, char cmdBuf[rows][cols], int *cmdBufLen, int *cmdIdx) {
	char *c = '\0';
	int idx = 0;
	int len = 0;
	prompt();
	while((*c = inbyte()) != ENTER) {
		//handle backspace
		if(*c == BS) {
			if(idx > 0) {
				idx--;
				len--;
				*(cmd + idx) = '\0';
				xil_printf("%c", *c);
				xil_printf(" ");
				xil_printf("%c", *c);
			}
		}
		else if(*c == 27) {
			//inbyte();
			char tmp = inbyte();
			switch(tmp) {
				//up
				//clear line and reprint what's at cmdIdx, cmdIdx must be < 49
				case 'A':
					//xil_printf("len = %d\r\n", len);
					if(*cmdIdx < 49 && strcmp(cmdBuf[*cmdIdx], "'\0'") != 0) {

						for(int i = len; i > 0; i--) {
							xil_printf("%c", BS);
							xil_printf("%c", SPACE);
							xil_printf("%c", BS);
						}
						xil_printf("%s", cmdBuf[*cmdIdx]);
						cmd = cmdBuf[*cmdIdx];
						len = strlen(cmdBuf[*cmdIdx]);
						idx = len;
						(*cmdIdx)++;
					}
					break;
				//down
				//clear line and enter what's at cmdIdx - 1, cmdIdx must be > 0
				case 'B':
					if(*cmdIdx > 0) {
						if(*cmdIdx == *cmdBufLen) {
							(*cmdIdx)--;
						}
						(*cmdIdx)--;
						for(int i = len; i > 0; i--) {
							xil_printf("%c", BS);
							xil_printf("%c", SPACE);
							xil_printf("%c", BS);
						}
						cmd = cmdBuf[*cmdIdx];
						xil_printf("%s", cmdBuf[*cmdIdx]);
						len = strlen(cmdBuf[*cmdIdx]);
						idx = len;
					}
					else if(*cmdIdx == 0) {
						for(int i = len; i > 0; i--) {
							xil_printf("%c", BS);
							xil_printf("%c", SPACE);
							xil_printf("%c", BS);
						}
						cmd = '\0';
						len = 0;
						idx = 0;
					}
					break;
				//right
				case 'C':
					//need to print everything again from idx to len
					if(idx < len) {
						idx++;
						xil_printf("%c", cmd[idx-1]);
					}
					//xil_printf("%c", "$1C");
					break;
				//left - need to reprint letters after the current idx
				case 'D':
					if(idx > 0) {
						idx--;
						xil_printf("%c", BS);
					}
					break;
			}
		}
		//if the idx != len, need to print new letter and reprint rest of the string
		else {
			if(idx != len) {
				//move everything over 1 idx to the right and then print
				//need to go from len to idx
				for(int i = len; i > idx; i--) {
					cmd[i] = cmd[i-1];
				}
				cmd[idx] = *c;
				for(int i = idx; i <= len; i++) {
					xil_printf("%c", cmd[i]);
				}
				for(int i = len; i > idx; i--) {
					xil_printf("%c", BS);
				}
			}
			else {
				*(cmd + idx) = *c;
				xil_printf("%c", *c);
			}
			idx++;
			len++;
		}
	}
	*(cmd+len) = '\0';
	strcpy(cmdBuf[*cmdBufLen], cmd);
	(*cmdBufLen)++;
	xil_printf("\r\nYou entered: %s\r\n", cmd);
}


/**
 * Main controlling thread of the program.
 */

int main() {
	init_platform();
	xil_printf("Welcome to the PIC Emulation Test Bed (PETB)");
	char cmdBuffer[50][50];
	for(int i = 0; i < 50; i++) {
		cmdBuffer[i][0] = '\0';
	}
	int bufLen = 0;
	int *cmdBufLen = &bufLen;

	for(;;) {
		char str_input[50];
		int cmdIdxVal = 0;
		int *cmdIdx = &cmdIdxVal;

		getUserInput(str_input, 50, 50, cmdBuffer, cmdBufLen, cmdIdx);

        call_command(parseCommand(str_input));
    }

	xil_printf("\r\nThank you for using the PIC Emulation Test Bed. Goodbye!\r\n");
	cleanup_platform();
	return 0;
}
