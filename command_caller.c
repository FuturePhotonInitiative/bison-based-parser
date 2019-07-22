/*****************************************************************************
 * file: command_caller.c
 *
 * author: Mark Nash
 * date created: 9 July 2019
 * date last modified: 22 July 2019
 *
 *****************************************************************************/

#include "command_caller.h"
#include "vcu108.h"
#include "qsfp_iic.h"
#include "qsfp_gpio.h"
#include "pek.h"

int status;

void call_command(command cmd) {
    switch (cmd.command_code) {
        case COMMAND_CFP_GPIO_READ:
            // cfp_gpio_read(unmap_cfp_port[cmd.args[0]]);
            // break;
        case COMMAND_CFP_GPIO_SET:
            // cfp_gpio_set(unmap_cfp_port[cmd.args[0]]);
            // break;
        case COMMAND_CFP_GPIO_CLEAR:
            // cfp_gpio_clear(unmap_cfp_port[cmd.args[0]]);
            // break;
        case COMMAND_CFP_GPIO_TOGGLE:
            // cfp_gpio_toggle(unmap_cfp_port[cmd.args[0]]);
            // break;
        case COMMAND_CFP_GPIO_DEBUG:
            // cfp_gpio_debug();
            // break;
            xil_printf("Command not yet implemented\r\n");
            break;
        case COMMAND_QSFP_GPIO_READ:
            char *port = unmap_gpio[cmd.args[0]]
            status = qsfp_gpio_read(port);
            if (status != XST_INVALID_PARAM) {
                if (cmd.args[0] == GPIO_PORT_ALL) {
                    xil_printf("All ports have been printed\r\n");
                } else {
                    // for any port that is not 'all': print what was read
                    xil_printf("%s = %d\r\n", port, status);
                }
            } else {
                xil_printf("An unknown error has occurred with port %s\r\n", port);
            }
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
        case COMMAND_QSFP_GPIO_DEBUG:
            qsfp_gpio_debug();
            break;
        case COMMAND_QSFP_IIC_READ:
            qsfp_iic_read(cmd.args[0], cmd.args[1], cmd.args[2]);
            break;
        case COMMAND_QSFP_IIC_DEBUG:
            qsfp_iic_debug();
            break;
        case COMMAND_QSFP_BERT:
            xil_printf(USE_PTCS);
            break;
        case COMMAND_QSFP_EYESCAN:
            xil_printf(USE_PTCS);
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
        case COMMAND_VCU108_GPIO_DEBUG:
            vcu108_debug();
            break;
        case COMMAND_PEK_GPIO_READ:
            // code copied from the EE's command caller
            u8 data[31];
            status = pek_gpio_read(cmd.args[0], cmd.args[1], data);
            switch (status) {
                case XST_SUCCESS:
                    xil_printf("The read value of Port %d, Pin %d = %d\r\n", port, pin, data[0]);
                    xil_printf("Data: [");
                    for (int i = 0; i <= 30; i++) {
                        xil_printf("%d ", data[i]);
                    }
                    xil_printf("]\r\n");
                    break;
                case XST_SEND_ERROR:
                    xil_printf("Data transmission failure\r\n");
                    break;
                case XST_INVALID_PARAM:
                    xil_printf("Specified Port & Pin are invalid for that command\r\n");
                    break;
                case XST_NO_FEATURE:
                    xil_printf("Invalid command\r\n");
                    break;
                case XST_RECV_ERROR:
                    xil_printf("Data receiving failure\r\n");
                    break;
                case XST_FAILURE:
                    xil_printf("Invalid status received from PEK\r\n");
                    break;
                default:
                    xil_printf("Invalid status received from pek_gpio_read()\r\n");
            }
            break;
        case COMMAND_PEK_GPIO_SET:
            pek_set_port(cmd.args[0], cmd.args[1]);
            break;
        case COMMAND_PEK_GPIO_CLEAR:
            pek_gpio_clear(cmd.args[0], cmd.args[1]);
            break;
        case COMMAND_PEK_GPIO_TOGGLE:
            // code copied from the EE's command caller
            u8 data[31];
            status = pek_gpio_toggle(cmd.args[0], cmd.args[1]);
            switch (status) {
				case XST_SUCCESS:
					xil_printf("The pin has been toggled\r\n");
					xil_printf("The previous value was: %d\r\n", data[0]);
					xil_printf("The new value is now  : %d\r\n", data[1]);
					xil_printf("Data: [");
					for (int i = 0; i <= 30; i++) {
						xil_printf("%d ", data[i]);
					}
					xil_printf("]\r\n");
				    break;
				case XST_SEND_ERROR:
					xil_printf("Data transmission failure\r\n");
				    break;
				case XST_INVALID_PARAM:
					xil_printf("Specified Port & Pin are invalid for that command\r\n");
				    break;
				case XST_NO_FEATURE:
					xil_printf("Invalid command\r\n");
				    break;
				case XST_RECV_ERROR:
					xil_printf("Data receiving failure\r\n");
				    break;
				case XST_FAILURE:
					xil_printf("Invalid status received from PEK\r\n");
				    break;
				default:
					xil_printf("Invalid status received from pek_gpio_toggle()\r\n");
			}
            break;
        case COMMAND_PEK_GPIO_DEBUG:
            pek_gpio_debug();
            break;
        case COMMAND_PEK_IIC_READ:
            qsfp_iic_read(cmd.args[0], cmd.args[1], cmd.args[2]);
            break;
        case COMMAND_PEK_IIC_WRITE:
            unsigned char data_start_index = 6;
            if (cmd.args[2] == 'c') {
                // the data type is "char" not "hex", which is one more character
                data_start_index += 1;
            }
            qsfp_iic_write(cmd.args[0], cmd.args[1], &(cmd.args + 2), &(cmd.args + data_start_index));
            break;
        case COMMAND_PEK_IIC_DEBUG:
            pek_iic_debug();
            break;
        case COMMAND_PEK_BERT:
            xil_printf(USE_PTCS);
            break;
        case COMMAND_PEK_EYESCAN:
            xil_printf(USE_PTCS);
            break;
        case COMMAND_PEK_OPTOCOUPLER:
            // pek_optocoupler(cmd.args[0], cmd.args[1]);
            xil_printf("Command not yet implemented\r\n");
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
                case ERROR_INVALID_PEK_GPIO_PORT:
                    xil_printf("the pek port cannot be used with that command\r\n");
                    break;
                case ERROR_INVALID_PEK_GPIO_PIN:
                    xil_printf("the pek pin cannot be used with that port and command\r\n");
                    break;
                case ERROR_INVALID_PEK_IIC_READ_PAGE:
                    xil_printf("the pek read page is out of bounds\r\n");
                    break;
                case ERROR_OTHER:
                    xil_printf("the structure of the command is invalid\r\n");
                    break;
                default:
                    xil_printf("you produced an error that was caught by the parser, but not by the command caller. This should be fixed\r\n");
                    break;
            }
            break;
    }
}
