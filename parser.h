#ifndef PARSER_H
#define PARSER_H

#define COMMAND_INVALID 255
#define COMMAND_CFP_GPIO_READ 0
#define COMMAND_CFP_GPIO_SET 1
#define COMMAND_CFP_GPIO_CLEAR 2
#define COMMAND_CFP_GPIO_TOGGLE 3
#define COMMAND_CFP_GPIO_DEBUG 4
#define COMMAND_QSFP_GPIO_READ 5
#define COMMAND_QSFP_GPIO_SET 6
#define COMMAND_QSFP_GPIO_CLEAR 7
#define COMMAND_QSFP_GPIO_TOGGLE 8
#define COMMAND_QSFP_GPIO_DEBUG 9
#define COMMAND_QSFP_IIC_READ 10
#define COMMAND_QSFP_IIC_DEBUG 11
#define COMMAND_QSFP_BERT 12
#define COMMAND_QSFP_EYESCAN 13
#define COMMAND_VCU108_GPIO_READ 14
#define COMMAND_VCU108_GPIO_SET 15
#define COMMAND_VCU108_GPIO_CLEAR 16
#define COMMAND_VCU108_GPIO_TOGGLE 17
#define COMMAND_VCU108_GPIO_DEBUG 18
#define COMMAND_PEK_GPIO_READ 19
#define COMMAND_PEK_GPIO_SET 20
#define COMMAND_PEK_GPIO_CLEAR 21
#define COMMAND_PEK_GPIO_TOGGLE 22
#define COMMAND_PEK_GPIO_DEBUG 23
#define COMMAND_PEK_IIC_READ 24
#define COMMAND_PEK_IIC_WRITE 25
#define COMMAND_PEK_IIC_DEBUG 26
#define COMMAND_PEK_BERT 27
#define COMMAND_PEK_EYESCAN 28
#define COMMAND_PEK_OPTOCOUPLER 29

#define ERROR_INVALID_GPIO_PORT 0
#define ERROR_INVALID_GPIO_PIN 1
#define ERROR_INVALID_QSFP_IIC_READ_PAGE 2
#define ERROR_START_ADDR_NOT_LESS_THAN_END 3
#define ERROR_PEK_WRITE_DATA_TOO_LONG 4
#define ERROR_INVALID_PEK_IIC_WRITE_PAGE 5
#define ERROR_INVALID_PEK_GPIO_PORT 6
#define ERROR_INVALID_PEK_GPIO_PIN 7
#define ERROR_INVALID_PEK_IIC_READ_PAGE 8
#define ERROR_OTHER 255

#include "parse_types.h"

command parseCommand(char* commandString);

#endif
