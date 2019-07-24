/*****************************************************************************
 * file: command_caller.c
 *
 * author: Mark Nash
 * date created: 9 July 2019
 * date last modified: 23 July 2019
 *
 *****************************************************************************/

#include "command_caller.h"
#include "vcu108.h"
#include "qsfp_iic.h"
#include "qsfp_gpio.h"
#include "pek.h"

char *USE_PTCS = "Please use PIC Test Control Software to complete this task\r\n";

// the Bison parser returns structs with integers determining the types of some arguments.
// in this case, the index of the map indicates the location of the string to put into the specified files.
char *unmap_gpio[] = {
    "",
    "MODSEL",
    "RESET",
    "MODPRS",
    "INT",
    "LPMODE",
    "ALL"
};

char *unmap_vcu108_port[] = {
    "",
    "LED",
    "BUTTON",
    "SWITCH",
    "ALL"
};

char *unmap_cfp_port[] = {
    "",
    "prg_cntl1",
    "prg_cntl2",
    "prg_cntl3",
    "tx_dis",
    "mod_lopwr",
    "mod_rstn"
};

char *unmap_data_type[] = {
    "",
    "CHAR",
    "HEX",
}

void call_command(command cmd) {
    int status;
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
        case COMMAND_QSFP_GPIO_READ: {
        	char *port = unmap_gpio[cmd.args[0]];
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
        }
        case COMMAND_QSFP_GPIO_SET: {
        	char *port = unmap_gpio[cmd.args[0]];
			status = qsfp_gpio_set(port);
			if (status == XST_SUCCESS)
				xil_printf("%s has been set\r\n", port);
			else
				xil_printf("An error occurred while setting port: %s\r\n", port);
			break;
        }
        case COMMAND_QSFP_GPIO_CLEAR: {
        	char *port = unmap_gpio[cmd.args[0]];
            status = qsfp_gpio_clear(port);
			if (status == XST_SUCCESS)
				xil_printf("%s has been cleared\r\n", port);
			else
				xil_printf("An error occurred while clearing port: %s\r\n", port);
            break;
        }
        case COMMAND_QSFP_GPIO_TOGGLE: {
        	char *port = unmap_gpio[cmd.args[0]];
            status = qsfp_gpio_toggle(port);
            if (status == XST_SUCCESS)
				xil_printf("%s has been toggled\r\n", port);
			else
				xil_printf("An error occurred while toggling port: %s\r\n", port);
            break;
        }
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
        case COMMAND_VCU108_GPIO_READ: {
            char *port = unmap_vcu108_port[cmd.args[0]];
            status = vcu108_gpio_read(port, cmd.args[1]);
            if (cmd.args[0] == VCU108_PORT_ALL) {
                if (status == XST_FAILURE) {
                    xil_printf("An error occurred while reading all ports\r\n");
                }
            } else {
                // for any port that is not 'all': print what was read
                xil_printf("%s[%d] = %d\r\n", port, cmd.args[1], status);
            }
            break;
        }
        case COMMAND_VCU108_GPIO_SET:
            vcu108_gpio_set(cmd.args[0]);
            xil_printf("LED[%d] = 1\r\n", cmd.args[0]);
            break;
        case COMMAND_VCU108_GPIO_CLEAR:
            vcu108_gpio_clear(cmd.args[0]);
			xil_printf("LED[%d] = 0\r\n", cmd.args[0]);
            break;
        case COMMAND_VCU108_GPIO_TOGGLE:
            vcu108_gpio_toggle(cmd.args[0]);
            xil_printf("LED[%d] has been toggled\r\n", cmd.args[0]);
            break;
        case COMMAND_VCU108_GPIO_DEBUG:
            vcu108_gpio_debug();
            break;
        case COMMAND_PEK_GPIO_READ: {
            // code copied from the EE's command caller
            u8 data[31];
            status = pek_gpio_read(cmd.args[0], cmd.args[1], data);
            switch (status) {
                case XST_SUCCESS:
                    xil_printf("The read value of Port %d, Pin %d = %d\r\n", cmd.args[0], cmd.args[1], data[0]);
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
        }
        case COMMAND_PEK_GPIO_SET:
            status = pek_gpio_set(cmd.args[0], cmd.args[1]);
            switch (status) {
				case XST_SUCCESS:
					xil_printf("The pin has been set\r\n");
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
					xil_printf("Invalid status received from pek_gpio_set()\r\n");
			}
            break;
        case COMMAND_PEK_GPIO_CLEAR:
            status = pek_gpio_clear(cmd.args[0], cmd.args[1]);
            switch (status) {
				case XST_SUCCESS:
					xil_printf("The pin has been cleared\r\n");
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
					xil_printf("Invalid status received from pek_gpio_clear()\r\n");
			}
            break;
        case COMMAND_PEK_GPIO_TOGGLE: {
            // code copied from the EE's command caller
            u8 data[31];
            status = pek_gpio_toggle(cmd.args[0], cmd.args[1], data);
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
        }
        case COMMAND_PEK_GPIO_DEBUG:
            pek_gpio_debug();
            break;
        case COMMAND_PEK_IIC_READ:
            qsfp_iic_read(cmd.args[0], cmd.args[1], cmd.args[2]);
            break;
        case COMMAND_PEK_IIC_WRITE: {
            qsfp_iic_write(cmd.args[0], cmd.args[1], unmap_data_type[cmd.args[2]], &cmd.args[3]);
            xil_printf("\r\nNew Data: %s\r\n", &cmd.args[3]);
            break;
        }
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
            xil_printf("ERROR\r\n");
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
                case ERROR_START_ADDR_NOT_LESS_THAN_END:
                    xil_printf("the end index when reading needs to be greater than the start index\r\n");
                    break;
                case ERROR_PEK_WRITE_DATA_TOO_LONG:
                    xil_printf("your input data is too large. Its length must be less than 255 minus the start address")
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
                    xil_printf(cmd.args[1]);
                    xil_printf("\r\n");
                    break;
                default:
                    xil_printf("you produced an error that was caught by the parser, but not by the command caller. This should be fixed\r\n");
                    break;
            }
            break;
    }
}
