#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#define MAX_ARGS 50

#define GPIO_CLEAR 1
#define GPIO_LIST 2
#define GPIO_READ 3
#define GPIO_SET 4
#define GPIO_TOGGLE 5
#define GPIO_WRITE 6

#define ALL_PIN 1
#define INT_PIN 2
#define LPMODE_PIN 3
#define MODPRS_PIN 4
#define MODSEL_PIN 5
#define RESET_PIN 6

typedef struct command{
	unsigned char command_code;
	unsigned char args_len;
	unsigned char args[MAX_ARGS];
}command;

#endif
