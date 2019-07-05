#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#define MAX_ARGS 50

#define GPIO_PORT_MODSEL 1
#define GPIO_PORT_RESET 2
#define GPIO_PORT_LPMODE 3
#define GPIO_PORT_MODPRS 4
#define GPIO_PORT_INT 5
#define GPIO_PORT_ALL 6

#define VCU108_PORT_LEDS 1
#define VCU108_PORT_BUTTONS 2
#define VCU108_PORT_SWITCHES 3
#define VCU108_PORT_ALL 4

#define CFP_PORT_PRG_CNTL1 1
#define CFP_PORT_PRG_CNTL2 2
#define CFP_PORT_PRG_CNTL3 3
#define CFP_PORT_TX_DIS 4
#define CFP_PORT_MOD_LOPWR 5
#define CFP_PORT_MOD_RSTN 6
#define CFP_PORT_ALL 7

typedef struct command {
	unsigned char command_code;
	unsigned char args_len;
	unsigned char args[MAX_ARGS];
} command;

#endif
