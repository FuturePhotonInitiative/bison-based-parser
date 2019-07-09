/*****************************************************************************
 * file: command_caller.c
 *
 * author: Mark Nash
 * date: 9 July 2019
 *
 *****************************************************************************/
#include "command_caller.h"
#include "petb_interface.h"
#include "pek_interface.h"
#include "qsfp_iic.h"
#include "xil_printf.h"
#include "vcu108_gpio.h"

void call_command(command cmd) {
    switch (cmd.command_code) {
        case COMMAND_CFP_GPIO_READ:
            // cfp_gpio_read(unmap_cfp_port[cmd.args[0]]);
            break;
        case COMMAND_CFP_GPIO_SET:
            // cfp_gpio_set(unmap_cfp_port[cmd.args[0]]);
            break;
        case COMMAND_CFP_GPIO_CLEAR:
            // cfp_gpio_clear(unmap_cfp_port[cmd.args[0]]);
            break;
        case COMMAND_CFP_GPIO_TOGGLE:
            // cfp_gpio_toggle(unmap_cfp_port[cmd.args[0]]);
            break;
        case COMMAND_CFP_DEBUG:
            // cfp_debug();
            break;
        case COMMAND_QSFP_GPIO_READ:
            qsfp_gpio_read(unmap_gpio[cmd.args[0]]);
            break;
        case COMMAND_QSFP_GPIO_SET:
            qsfp_gpio_set(unmap_gpio[cmd.args[0]]);
            break;
        case COMMAND_QSFP_GPIO_CLEAR:
            qsfp_gpio_clear(unmap_gpio[cmd.args[0]]);
            break;
        case COMMAND_QSFP_GPIO_TOGGLE:
            qsfp_gpio_toggle(unmap_gpio[cmd.args[0]]);
            break;
        case COMMAND_QSFP_IIC_READ:
            qsfp_iicreadtest(cmd.args[0], cmd.args[1], cmd.args[2]);
            break;
        case COMMAND_QSFP_BERT:
            xil_printf(USE_PTCS);
            break;
        case COMMAND_QSFP_EYESCAN:
            xil_printf(USE_PTCS);
            break;
        case COMMAND_QSFP_DEBUG:
            hello_iic();
            break;
        case COMMAND_VCU108_DEBUG:
            vcu108_debug();
            break;
        case COMMAND_VCU108_GPIO_READ:
            vcu108_gpio_read(unmap_vcu108_port[cmd.args[0]], cmd.args[1]);
            break;
        case COMMAND_VCU108_GPIO_SET:
            vcu108_gpio_set(cmd.args[0]);
            break;
        case COMMAND_VCU108_GPIO_CLEAR:
            vcu108_gpio_clear(cmd.args[0]);
            break;
        case COMMAND_VCU108_GPIO_TOGGLE:
            vcu108_gpio_toggle(cmd.args[0]);
            break;


        // wat
        case COMMAND_PEK_GPIO_READ:
            break;
        case COMMAND_PEK_GPIO_SET:
            break;
        case COMMAND_PEK_GPIO_CLEAR:
            break;
        case COMMAND_PEK_GPIO_TOGGLE:
            break;



        case COMMAND_PEK_IIC_WRITE:
            unsigned char data_start_index = 6;
            if (args[2] == 'c') {
                // the data type is "char" not "hex", which is one more character
                data_start_index += 1;
            }
            qsfp_iicwritetest(args[0], args[1], args[2], args[data_start_index]);
            break;
        case COMMAND_PEK_IIC_READ:
            qsfp_iicreadtest(args[0], args[1], args[2]);
            break;
        case COMMAND_PEK_BERT:
            xil_printf(USE_PTCS);
            break;
        case COMMAND_PEK_EYESCAN:
            xil_printf(USE_PTCS);
            break;
        case COMMAND_INVALID:
            switch (cmd.args[0]) {
                case ERROR_INVALID_GPIO_PORT:
                    xil_printf("the GPIO port cannot be used with that command\r\n");
                    break;
                case ERROR_INVALID_GPIO_PIN:
                    xil_printf("the GPIO pin cannot be used with that command\r\n");
                    break;
                case ERROR_INVALID_QSFP_IIC_READ_PAGE:
                    xil_printf("the read page value is invalid\r\n");
                    break;
                case ERROR_PEK_WRITE_DATA_TOO_LONG:
                    break;
                case ERROR_INVALID_PEK_IIC_WRITE_PAGE:
                    xil_printf("the pek write page is out of bounds\r\n");
                    break;
                case ERROR_INVALID_PEK_PORT:
                    xil_printf("the pek port cannot be used with that command\r\n");
                    break;
                case ERROR_INVALID_PEK_IIC_READ_PAGE:
                    xil_printf("the pek read page is out of bounds\r\n");
                    break;
                case ERROR_OTHER:
                    xil_printf("the structure of the command is invalid\r\n");
                default:
                    xil_printf("you produced an error that was caught by the parser, but not by the command caller. This should be fixed\r\n");
                    break;
            }
            break;
    }
}