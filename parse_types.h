#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#define MAX_ARGS 50

#define GPIO_CLEAR 1
#define GPIO_LIST 2
#define GPIO_READ 3
#define GPIO_SET 4
#define GPIO_TOGGLE 5
#define GPIO_WRITE 6

#define GPIO_PORT_MODSEL 1
#define GPIO_PORT_RESET 2
#define GPIO_PORT_MODPRS 3
#define GPIO_PORT_INT 4
#define GPIO_PORT_ALL 5

#define VCU108_PORT_LEDS 1
#define VCU108_PORT_BUTTONS 2
#define VCU108_PORT_SWITCHES 3
#define VCU108_PORT_ALL 4

typedef struct command {
	unsigned char command_code;
	unsigned char args_len;
	unsigned char args[MAX_ARGS];
} command;

#endif
