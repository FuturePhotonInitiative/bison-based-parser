/*****************************************************************************
 * file: command_caller.h
 * description: header file for the command_caller file
 * version: 0.01
 *
 * author: Sean Wisnewski
 * company: Precision Optical Transceivers
 * date: 18 March 2019
 *****************************************************************************/

#ifndef COMMAND_CALLER_H
#define COMMAND_CALLER_H

#include "parser.h"

char *USE_PTCS = "Please use PIC Test Control Software to complete this task\r\n"

char *unmap_gpio[] = {
    "",
    "modsel",
    "reset",
    "modprs",
    "int",
    "lpmode",
    "all"
};

char *unmap_vcu108_port[] = {
    "",
    "leds",
    "buttons",
    "switches",
    "all"
}

char *unmap_cfp_port[] = {
    "",
    "prg_cntl1",
    "prg_cntl2",
    "prg_cntl3",
    "tx_dis",
    "mod_lopwr",
    "mod_rstn"
}

void call_command(command cmd);

#endif