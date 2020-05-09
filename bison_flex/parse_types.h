#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#define MAX_ARGS 50
#define MAX_COMMAND_ARGS 150

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

enum cfp_ports {
    ZERO_NOT_USED,
    CFP_PORT_PRG_CNTL1,
    CFP_PORT_PRG_CNTL2,
    CFP_PORT_PRG_CNTL3,
    CFP_PORT_TX_DIS,
    CFP_PORT_MOD_LOPWR,
    CFP_PORT_MOD_RSTN,
    CFP_PORT_PRG_ALRM1,
    CFP_PORT_PRG_ALRM2,
    CFP_PORT_PRG_ALRM3,
    CFP_PORT_RX_LOS,
    CFP_PORT_MOD_ABS,
    CFP_PORT_ALL
};

#define WRITE_DATA_TYPE_CHAR 1
#define WRITE_DATA_TYPE_HEX 2

typedef struct command {
	unsigned char command_code;
	unsigned char args_len;
	unsigned char args[MAX_COMMAND_ARGS];
} command;

#endif
