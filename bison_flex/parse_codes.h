#ifndef PARSER_H
#define PARSER_H

#define COMMAND_INVALID 255
#define COMMAND_CFP_GPIO_READ 0
#define COMMAND_CFP_GPIO_SET 1
#define COMMAND_CFP_GPIO_CLEAR 2
#define COMMAND_CFP_GPIO_TOGGLE 3
#define COMMAND_CFP_GPIO_DEBUG 4
#define COMMAND_CFP_MDIO_READ 5
#define COMMAND_CFP_MDIO_WRITE 6
#define COMMAND_CFP_MDIO_DEBUG 7
#define COMMAND_QSFP_GPIO_READ 8
#define COMMAND_QSFP_GPIO_SET 9
#define COMMAND_QSFP_GPIO_CLEAR 10
#define COMMAND_QSFP_GPIO_TOGGLE 11
#define COMMAND_QSFP_GPIO_DEBUG 12
#define COMMAND_QSFP_IIC_READ 13
#define COMMAND_QSFP_IIC_DEBUG 14
#define COMMAND_QSFP_BERT 15
#define COMMAND_QSFP_EYESCAN 16
#define COMMAND_VCU108_GPIO_READ 17
#define COMMAND_VCU108_GPIO_SET 18
#define COMMAND_VCU108_GPIO_CLEAR 19
#define COMMAND_VCU108_GPIO_TOGGLE 20
#define COMMAND_VCU108_GPIO_DEBUG 21
#define COMMAND_PEK_GPIO_READ 22
#define COMMAND_PEK_GPIO_SET 23
#define COMMAND_PEK_GPIO_CLEAR 24
#define COMMAND_PEK_GPIO_TOGGLE 25
#define COMMAND_PEK_GPIO_DEBUG 26
#define COMMAND_PEK_IIC_READ 27
#define COMMAND_PEK_IIC_WRITE 28
#define COMMAND_PEK_IIC_DEBUG 29
#define COMMAND_PEK_BERT 30
#define COMMAND_PEK_EYESCAN 31
#define COMMAND_PEK_OPTOCOUPLER 32

#define ERROR_INVALID_GPIO_PORT 0
#define ERROR_INVALID_GPIO_PIN 1
#define ERROR_INVALID_QSFP_IIC_READ_PAGE 2
#define ERROR_START_ADDR_NOT_LESS_THAN_END 3
#define ERROR_PEK_WRITE_DATA_TOO_LONG 4
#define ERROR_INVALID_PEK_IIC_WRITE_PAGE 5
#define ERROR_INVALID_PEK_GPIO_PORT 6
#define ERROR_INVALID_PEK_GPIO_PIN 7
#define ERROR_INVALID_PEK_IIC_READ_PAGE 8
#define ERROR_INVALID_CFP_MDIO_ADDRESS 9
#define ERROR_OTHER 255

#include "parse_types.h"

command parseCommand(char* commandString);

#endif