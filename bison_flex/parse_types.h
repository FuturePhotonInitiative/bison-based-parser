#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

#define MAX_ARGS 50
#define MAX_COMMAND_ARGS 150

enum gpio_ports {
    ZERO_IS_NOT_USED,
    GPIO_PORT_MODSEL,
    GPIO_PORT_RESET,
    GPIO_PORT_LPMODE,
    GPIO_PORT_MODPRS,
    GPIO_PORT_INT,
    GPIO_PORT_ALL
};

enum vcu108_ports {
    ZERO_SLOT_NOT_USED,
    VCU108_PORT_LEDS,
    VCU108_PORT_BUTTONS,
    VCU108_PORT_SWITCHES,
    VCU108_PORT_ALL
};

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
