/*****************************************************************************
 * file: command_caller.h
 *
 * author: Mark Nash
 * date created: 9 July 2019
 * date last modified: 22 July 2019
 *
 *****************************************************************************/

#ifndef COMMAND_CALLER_H
#define COMMAND_CALLER_H

#include "parser.h"

char *USE_PTCS = "Please use PIC Test Control Software to complete this task\r\n";

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

void call_command(command cmd);

#endif