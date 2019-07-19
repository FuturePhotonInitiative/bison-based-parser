// Test file for a parser. Run main with no arguments.

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// how many tests failed to match the assertion
int fail_assertion_count = 0;

// @return true if the test did not have the same outcome as the assertion
bool test_int_args(char *cmd_str, unsigned char expected_cmd_code, unsigned char *args, unsigned char len_args, FILE *fp, bool assert_success) {
    fprintf(fp, "command: %s\n", cmd_str);
    command ret_val = parseCommand(cmd_str);
    bool failure = false;
    if (ret_val.command_code != expected_cmd_code) {
        fprintf(fp, "--command codes dont match--%s\n", ret_val.command_code == COMMAND_INVALID ? " the command is invalid" : "");
        fprintf(fp, "  parsed code: %d\nexpected code: %d\n", ret_val.command_code, expected_cmd_code);
        failure = true;
    } else {
        fprintf(fp, "command codes match: %d\n", ret_val.command_code);

        if (ret_val.args_len != len_args) {
            fprintf(fp, "--size of args do not match--\n");
            fprintf(fp, "len of parsed args: %d\n", ret_val.args_len);
            fprintf(fp, "len expected args : %d\n", len_args);
            failure = true;
        } else {
            fprintf(fp, "argument lengths match\n");
        }

        for (int i = 0; i < ret_val.args_len || i < len_args; i++) {
            if (i < ret_val.args_len && i < len_args) {
                if (ret_val.args[i] != args[i]) {
                    fprintf(fp, "--arg[%d] does not match--\n", i);
                    failure = true;
                }
            }
        }

        fprintf(fp, "  parsed args: {");
        if (ret_val.args_len != 0) {
            for (int i = 0; i < ret_val.args_len - 1; i++) {
                fprintf(fp, "%d ", ret_val.args[i]);
            }
            fprintf(fp, "%d", ret_val.args[ret_val.args_len - 1]);
        }
        fprintf(fp, "}\n");

        fprintf(fp, "expected args: {");
        if (len_args != 0) {
            for (int i = 0; i < len_args - 1; i++) {
                fprintf(fp, "%d ", args[i]);
            }
            fprintf(fp, "%d", args[len_args - 1]);
        }
        fprintf(fp, "}\n");
    }

    fprintf(fp, "%s\n\n", failure ? "FAILURE" : "SUCCESS");
    if (failure == assert_success) {
        fail_assertion_count += 1;
        fprintf(fp, "Error code: %d\n", ret_val.args[0]);
        fprintf(fp, "assertion failed\n");
        char message1[] = "FAILED instead of SUCCEEDED";
        char message2[] = "SUCCEEDED instead of FAILED";
        fprintf(fp, "%s %s\n\n", cmd_str, failure ? message1 : message2);
    }
    fflush(fp);
}

void test_pek_iic_write(char *cmd_str, unsigned char ar0, unsigned char ar1, char *ar2, char *ar3, FILE *fp, bool assert_success) {
    fprintf(fp, "command: %s\n", cmd_str);
    command ret_val = parseCommand(cmd_str);
    bool failure = false;
    if (ret_val.command_code != COMMAND_PEK_IIC_WRITE) {
        fprintf(fp, "--command codes dont match--%s\n", ret_val.command_code == COMMAND_INVALID ? " the command is invalid" : "");
        fprintf(fp, "  parsed code: %d\nexpected code: %d\n", ret_val.command_code, COMMAND_PEK_IIC_WRITE);
        failure = true;
    } else {
        fprintf(fp, "command codes match\n");
        unsigned char ret_ar0 = ret_val.args[0];
        unsigned char ret_ar1 = ret_val.args[1];
        char *ret_ar2 = ret_val.args + 2;
        char *ret_ar3 = ret_ar2 + strlen(ret_ar2) + 1;
        if (ar0 != ret_ar0 || ar1 != ret_ar1 || strncmp(ar2, ret_ar2, strlen(ar2)) != 0 || strncmp(ar3, ret_ar3, strlen(ar3) != 0)) {
            fprintf(fp, "--args do not match--\n");
            failure = true;
            fprintf(fp, "  parsed args {%d %d %s %s}\n", ret_ar0, ret_ar1, ret_ar2, ret_ar3);
            fprintf(fp, "expected args {%d %d %s %s}\n", ar0, ar1, ar2, ar3);
        }
    }

    fprintf(fp, "%s\n\n", failure ? "FAILURE" : "SUCCESS");
    if (failure == assert_success) {
        fail_assertion_count += 1;
        fprintf(fp, "Error code: %d\n", ret_val.args[0]);
        fprintf(fp, "assertion failed\n");
        char message1[] = "FAILED instead of SUCCEEDED";
        char message2[] = "SUCCEEDED instead of FAILED";
        fprintf(fp, "%s %s\n\n", cmd_str, failure ? message1 : message2);
    }
    fflush(fp);
}

void test_pek_gpio_read(unsigned char port, unsigned char pin, FILE *fp, bool success) {
    unsigned char args2[2];
    args2[0] = port;
    args2[1] = pin;
    char command_str[50];
    sprintf(command_str, "pek gpio read %d %d", port, pin);
    test_int_args(command_str, COMMAND_PEK_GPIO_READ, args2, sizeof(args2), fp, success);
}

void test_pek_gpio_sct(
        char *base_command_str, 
        char *command_str, 
        unsigned char port, 
        unsigned char pin,
        unsigned char command, 
        FILE *fp, 
        bool success) {
    unsigned char args2[2];
    args2[0] = port;
    args2[1] = pin;
    sprintf(command_str, "%s %d %d", base_command_str, port, pin);
    test_int_args(command_str, command, args2, sizeof(args2), fp, success);
}

void test_all_pek_gpio_set_clear_toggle(char *sct, FILE *fp) {
    char base_command_str[20];
    sprintf(base_command_str, "pek gpio %s", sct);
    unsigned char args2[2];
    unsigned char command;
    switch(sct[0]) {
        case 's':
            command = COMMAND_PEK_GPIO_SET;
            break;
        case 'c':
            command = COMMAND_PEK_GPIO_CLEAR;
            break;
        case 't':
            command = COMMAND_PEK_GPIO_TOGGLE;
            break;
    }
    char command_str[50];

    test_pek_gpio_sct(base_command_str, command_str, 0, 0, command, fp, false);
    test_pek_gpio_sct(base_command_str, command_str, 0, 1, command, fp, false);

    test_pek_gpio_sct(base_command_str, command_str, 1, 0, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 1, 1, command, fp, false);

    test_pek_gpio_sct(base_command_str, command_str, 2, 0, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 2, 1, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 2, 2, command, fp, false);
    test_pek_gpio_sct(base_command_str, command_str, 2, 3, command, fp, false);
    test_pek_gpio_sct(base_command_str, command_str, 2, 4, command, fp, false);
    test_pek_gpio_sct(base_command_str, command_str, 2, 5, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 2, 6, command, fp, false);

    test_pek_gpio_sct(base_command_str, command_str, 3, 0, command, fp, false);
    test_pek_gpio_sct(base_command_str, command_str, 3, 1, command, fp, false);

    test_pek_gpio_sct(base_command_str, command_str, 4, 0, command, fp, false);
    test_pek_gpio_sct(base_command_str, command_str, 4, 1, command, fp, false);

    test_pek_gpio_sct(base_command_str, command_str, 5, 0, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 5, 1, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 5, 2, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 5, 3, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 5, 4, command, fp, false);

    test_pek_gpio_sct(base_command_str, command_str, 6, 0, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 6, 1, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 6, 2, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 6, 3, command, fp, true);
    test_pek_gpio_sct(base_command_str, command_str, 6, 4, command, fp, false);
}

int main() {

    FILE *fp = fopen("new_commands.txt", "w");

    unsigned char args1[1];
    unsigned char args2[2];
    unsigned char args3[3];

    //****** cfp gpio read ******//
    // normal usages
    args1[0] = CFP_PORT_PRG_CNTL1;
    test_int_args("cfp gpio read prg_cntl1", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_PRG_CNTL2;
    test_int_args("cfp gpio read prg_cntl2", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_PRG_CNTL3;
    test_int_args("cfp gpio read prg_cntl3", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_TX_DIS;
    test_int_args("cfp gpio read tx_dis", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_MOD_LOPWR;
    test_int_args("cfp gpio read mod_lopwr", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_MOD_RSTN;
    test_int_args("cfp gpio read mod_rstn", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_ALL;
    test_int_args("cfp gpio read all", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, true);
    // misspelled command test
    args1[0] = CFP_PORT_TX_DIS;
    test_int_args("cfp gpio read tx_dip", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, false);
    // no arguments
    args1[0] = CFP_PORT_TX_DIS;
    test_int_args("cfp gpio read", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, false);
    // two arguments
    args1[0] = CFP_PORT_TX_DIS;
    test_int_args("cfp gpio read tx_dis tx_dis", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp, false);


    //****** cfp gpio set ******//
    // normal usages
    args1[0] = CFP_PORT_PRG_CNTL1;
    test_int_args("cfp gpio set prg_cntl1", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_PRG_CNTL2;
    test_int_args("cfp gpio set prg_cntl2", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_PRG_CNTL3;
    test_int_args("cfp gpio set prg_cntl3", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_TX_DIS;
    test_int_args("cfp gpio set tx_dis", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_MOD_LOPWR;
    test_int_args("cfp gpio set mod_lopwr", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_MOD_RSTN;
    test_int_args("cfp gpio set mod_rstn", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp, true);
    // cannot set all
    args1[0] = CFP_PORT_ALL;
    test_int_args("cfp gpio set all", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp, false);


    //****** cfp gpio clear ******//
    // normal usages
    args1[0] = CFP_PORT_PRG_CNTL1;
    test_int_args("cfp gpio clear prg_cntl1", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_PRG_CNTL2;
    test_int_args("cfp gpio clear prg_cntl2", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_PRG_CNTL3;
    test_int_args("cfp gpio clear prg_cntl3", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_TX_DIS;
    test_int_args("cfp gpio clear tx_dis", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_MOD_LOPWR;
    test_int_args("cfp gpio clear mod_lopwr", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_MOD_RSTN;
    test_int_args("cfp gpio clear mod_rstn", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    // cannot clear all
    args1[0] = CFP_PORT_ALL;
    test_int_args("cfp gpio clear all", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp, false);


    //****** cfp gpio toggle ******//
    // normal usages
    args1[0] = CFP_PORT_PRG_CNTL1;
    test_int_args("cfp gpio toggle prg_cntl1", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_PRG_CNTL2;
    test_int_args("cfp gpio toggle prg_cntl2", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_PRG_CNTL3;
    test_int_args("cfp gpio toggle prg_cntl3", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_TX_DIS;
    test_int_args("cfp gpio toggle tx_dis", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_MOD_LOPWR;
    test_int_args("cfp gpio toggle mod_lopwr", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    args1[0] = CFP_PORT_MOD_RSTN;
    test_int_args("cfp gpio toggle mod_rstn", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    // cannot toggle all
    args1[0] = CFP_PORT_ALL;
    test_int_args("cfp gpio toggle all", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp, false);


    //****** cfp gpio debug ******//
    // normal usages
    test_int_args("cfp gpio debug", COMMAND_CFP_GPIO_DEBUG, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("cfp gpio debug 0", COMMAND_CFP_GPIO_DEBUG, args1, sizeof(args1), fp, false);


    //****** qsfp gpio read ******//
    // normal usages
    args1[0] = GPIO_PORT_MODSEL;
    test_int_args("qsfp gpio read modsel", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_RESET;
    test_int_args("qsfp gpio read reset", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_LPMODE;
    test_int_args("qsfp gpio read lpmode", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_MODPRS;
    test_int_args("qsfp gpio read modprs", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_INT;
    test_int_args("qsfp gpio read int", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_ALL;
    test_int_args("qsfp gpio read all", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp, true);


    //****** qsfp gpio set ******//
    // normal usages
    args1[0] = GPIO_PORT_MODSEL;
    test_int_args("qsfp gpio set modsel", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_RESET;
    test_int_args("qsfp gpio set reset", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_LPMODE;
    test_int_args("qsfp gpio set lpmode", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp, true);
    // cannot set modprs
    args1[0] = GPIO_PORT_MODPRS;
    test_int_args("qsfp gpio set modprs", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp, false);
    // cannot set int
    args1[0] = GPIO_PORT_INT;
    test_int_args("qsfp gpio set int", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp, false);
    // cannot set all
    args1[0] = GPIO_PORT_ALL;
    test_int_args("qsfp gpio set all", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp, false);


    //****** qsfp gpio clear ******//
    // normal usages
    args1[0] = GPIO_PORT_MODSEL;
    test_int_args("qsfp gpio clear modsel", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_RESET;
    test_int_args("qsfp gpio clear reset", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_LPMODE;
    test_int_args("qsfp gpio clear lpmode", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    // cannot clear modprs
    args1[0] = GPIO_PORT_MODPRS;
    test_int_args("qsfp gpio clear modprs", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp, false);
    // cannot clear int
    args1[0] = GPIO_PORT_INT;
    test_int_args("qsfp gpio clear int", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp, false);
    // cannot clear all
    args1[0] = GPIO_PORT_ALL;
    test_int_args("qsfp gpio clear all", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp, false);


    //****** qsfp gpio toggle ******//
    // normal usages
    args1[0] = GPIO_PORT_MODSEL;
    test_int_args("qsfp gpio toggle modsel", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_RESET;
    test_int_args("qsfp gpio toggle reset", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    args1[0] = GPIO_PORT_LPMODE;
    test_int_args("qsfp gpio toggle lpmode", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    // cannot toggle modprs
    args1[0] = GPIO_PORT_MODPRS;
    test_int_args("qsfp gpio toggle modprs", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp, false);
    // cannot toggle int
    args1[0] = GPIO_PORT_INT;
    test_int_args("qsfp gpio toggle int", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp, false);
    // cannot toggle all
    args1[0] = GPIO_PORT_ALL;
    test_int_args("qsfp gpio toggle all", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp, false);


    //****** qsfp iic read ******//
    // normal usages
    args3[0] = 0;
    args3[1] = 0;
    args3[2] = 0;
    test_int_args("qsfp iic read 0 0 0", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 3;
    args3[1] = 255;
    args3[2] = 255;
    test_int_args("qsfp iic read 3 255 255", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 2;
    args3[1] = 200;
    args3[2] = 154;
    test_int_args("qsfp iic read 2 200 154", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 4;
    args3[1] = 255;
    args3[2] = 255;
    // 3 is the limit for arg0
    test_int_args("qsfp iic read 4 255 255", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, false);
    // 256 will parse to a string instead of an int which sufficiently makes it fail
    test_int_args("qsfp iic read 4 255 256", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, false);
    // negative numbers will be parsed as strings
    test_int_args("qsfp iic read -3 255 255", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, false);


    //****** qsfp iic debug ******//
    // normal usages
    test_int_args("qsfp iic debug", COMMAND_QSFP_IIC_DEBUG, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("qsfp iic debug 0", COMMAND_QSFP_IIC_DEBUG, args1, sizeof(args1), fp, false);


    //****** qsfp bert ******//
    // normal usages
    test_int_args("qsfp bert", COMMAND_QSFP_BERT, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("qsfp bert 0", COMMAND_QSFP_BERT, args1, sizeof(args1), fp, false);


    //****** qsfp eyescan ******//
    // normal usages
    test_int_args("qsfp eyescan", COMMAND_QSFP_EYESCAN, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("qsfp eyescan 0", COMMAND_QSFP_EYESCAN, args1, sizeof(args1), fp, false);


    //****** qsfp gpio debug ******//
    // normal usages
    test_int_args("qsfp gpio debug", COMMAND_QSFP_GPIO_DEBUG, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("qsfp gpio debug 0", COMMAND_QSFP_GPIO_DEBUG, args1, sizeof(args1), fp, false);


    //****** vcu108 gpio debug ******//
    test_int_args("vcu108 gpio debug", COMMAND_VCU108_GPIO_DEBUG, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("vcu108 gpio debug 0", COMMAND_VCU108_GPIO_DEBUG, args1, sizeof(args1), fp, false);


    //****** vcu108 gpio read ******//
    // max and min values for each
    args2[0] = VCU108_PORT_LEDS;
    args2[1] = 0;
    test_int_args("vcu108 gpio read leds 0", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, true);
    args2[0] = VCU108_PORT_LEDS;
    args2[1] = 7;
    test_int_args("vcu108 gpio read leds 7", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, true);
    args2[0] = VCU108_PORT_BUTTONS;
    args2[1] = 0;
    test_int_args("vcu108 gpio read buttons 0", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, true);
    args2[0] = VCU108_PORT_BUTTONS;
    args2[1] = 4;
    test_int_args("vcu108 gpio read buttons 4", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, true);
    args2[0] = VCU108_PORT_SWITCHES;
    args2[1] = 0;
    test_int_args("vcu108 gpio read switches 0", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, true);
    args2[0] = VCU108_PORT_SWITCHES;
    args2[1] = 3;
    test_int_args("vcu108 gpio read switches 3", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, true);
    args1[0] = VCU108_PORT_ALL;
    test_int_args("vcu108 gpio read all", COMMAND_VCU108_GPIO_READ, args1, sizeof(args1), fp, true);
    // 7 is max for leds
    args2[0] = VCU108_PORT_LEDS;
    args2[1] = 8;
    test_int_args("vcu108 gpio read leds 8", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, false);
    // 4 is max for buttons
    args2[0] = VCU108_PORT_BUTTONS;
    args2[1] = 5;
    test_int_args("vcu108 gpio read buttons 5", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, false);
    // 3 is max for switches
    args2[0] = VCU108_PORT_SWITCHES;
    args2[1] = 4;
    test_int_args("vcu108 gpio read switches 4", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp, false);
    // all should take zero arguments
    args1[0] = 0;
    test_int_args("vcu108 gpio read all 0", COMMAND_VCU108_GPIO_READ, args1, sizeof(args1), fp, false);


    //****** vcu108 gpio set ******//
    // max and min values
    args1[0] = 0;
    test_int_args("vcu108 gpio set 0", COMMAND_VCU108_GPIO_SET, args1, sizeof(args1), fp, true);
    args1[0] = 7;
    test_int_args("vcu108 gpio set 7", COMMAND_VCU108_GPIO_SET, args1, sizeof(args1), fp, true);
    // 7 is max for vcu108 gpio set
    args1[0] = 8;
    test_int_args("vcu108 gpio set 8", COMMAND_VCU108_GPIO_SET, args1, sizeof(args1), fp, false);


    //****** vcu108 gpio clear ******//
    // max and min values
    args1[0] = 0;
    test_int_args("vcu108 gpio clear 0", COMMAND_VCU108_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    args1[0] = 7;
    test_int_args("vcu108 gpio clear 7", COMMAND_VCU108_GPIO_CLEAR, args1, sizeof(args1), fp, true);
    // 7 is max for vcu108 gpio clear
    args1[0] = 8;
    test_int_args("vcu108 gpio clear 8", COMMAND_VCU108_GPIO_CLEAR, args1, sizeof(args1), fp, false);


    //****** vcu108 gpio toggle ******//
    // max and min values
    args1[0] = 0;
    test_int_args("vcu108 gpio toggle 0", COMMAND_VCU108_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    args1[0] = 7;
    test_int_args("vcu108 gpio toggle 7", COMMAND_VCU108_GPIO_TOGGLE, args1, sizeof(args1), fp, true);
    // 7 is max for vcu108 gpio toggle
    args1[0] = 8;
    test_int_args("vcu108 gpio toggle 8", COMMAND_VCU108_GPIO_TOGGLE, args1, sizeof(args1), fp, false);


    //****** pek gpio read ******//
    test_pek_gpio_read(0, 0, fp, false);
    test_pek_gpio_read(0, 1, fp, false);

    test_pek_gpio_read(1, 0, fp, true);
    test_pek_gpio_read(1, 1, fp, true);
    test_pek_gpio_read(1, 2, fp, false);
    test_pek_gpio_read(1, 3, fp, false);
    test_pek_gpio_read(1, 4, fp, true);
    test_pek_gpio_read(1, 5, fp, false);

    test_pek_gpio_read(2, 0, fp, true);
    test_pek_gpio_read(2, 1, fp, true);
    test_pek_gpio_read(2, 2, fp, false);
    test_pek_gpio_read(2, 3, fp, true);
    test_pek_gpio_read(2, 4, fp, true);
    test_pek_gpio_read(2, 5, fp, true);
    test_pek_gpio_read(2, 6, fp, false);

    test_pek_gpio_read(3, 0, fp, true);
    test_pek_gpio_read(3, 1, fp, true);
    test_pek_gpio_read(3, 2, fp, true);
    test_pek_gpio_read(3, 3, fp, true);
    test_pek_gpio_read(3, 4, fp, true);
    test_pek_gpio_read(3, 5, fp, true);
    test_pek_gpio_read(3, 6, fp, true);
    test_pek_gpio_read(3, 7, fp, true);
    test_pek_gpio_read(3, 8, fp, false);

    test_pek_gpio_read(4, 0, fp, false);
    test_pek_gpio_read(4, 1, fp, false);

    test_pek_gpio_read(5, 0, fp, true);
    test_pek_gpio_read(5, 1, fp, true);
    test_pek_gpio_read(5, 2, fp, true);
    test_pek_gpio_read(5, 3, fp, true);
    test_pek_gpio_read(5, 4, fp, true);
    test_pek_gpio_read(5, 5, fp, false);

    test_pek_gpio_read(6, 0, fp, true);
    test_pek_gpio_read(6, 1, fp, true);
    test_pek_gpio_read(6, 2, fp, true);
    test_pek_gpio_read(6, 3, fp, true);
    test_pek_gpio_read(6, 4, fp, false);


    //****** pek gpio set ******//
    test_all_pek_gpio_set_clear_toggle("set", fp);


    //****** pek gpio clear ******//
    test_all_pek_gpio_set_clear_toggle("clear", fp);


    //****** pek gpio toggle ******//
    test_all_pek_gpio_set_clear_toggle("toggle", fp);


    /****** pek iic write ******/
    // normal usages, min, max
    unsigned char ar0 = 0;
    unsigned char ar1 = 0;
    char *ar2 = "hex";
    char *ar3 = "\"0\"";
    char curr_str[80];
    sprintf(curr_str, "pek iic write %d %d %s %s", ar0, ar1, ar2, ar3);
    test_pek_iic_write(curr_str, ar0, ar1, ar2, ar3, fp, true);
    ar0 = 3;
    ar1 = 255;
    ar2 = "hex";
    ar3 = "\'0xFF835D\'";
    sprintf(curr_str, "pek iic write %d %d %s %s", ar0, ar1, ar2, ar3);
    test_pek_iic_write(curr_str, ar0, ar1, ar2, ar3, fp, true);
    ar0 = 3;
    ar1 = 255;
    ar2 = "char";
    ar3 = "\"hello, how are you\"";
    sprintf(curr_str, "pek iic write %d %d %s %s", ar0, ar1, ar2, ar3);
    test_pek_iic_write(curr_str, ar0, ar1, ar2, ar3, fp, true);
    // 3 is the max for the first argument
    ar0 = 4;
    ar1 = 255;
    ar2 = "hex";
    ar3 = "\"0\"";
    sprintf(curr_str, "pek iic write %d %d %s %s", ar0, ar1, ar2, ar3);
    test_pek_iic_write(curr_str, ar0, ar1, ar2, ar3, fp, false);


    //****** pek iic read ******//
    // normal usages, min, max
    args3[0] = 0;
    args3[1] = 0;
    args3[2] = 0;
    test_int_args("pek iic read 0 0 0", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 3;
    args3[1] = 255;
    args3[2] = 255;
    test_int_args("pek iic read 3 255 255", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 2;
    args3[1] = 200;
    args3[2] = 154;
    test_int_args("pek iic read 2 200 154", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, true);
    // 3 is max for the 0th arg
    args3[0] = 4;
    args3[1] = 255;
    args3[2] = 255;
    test_int_args("pek iic read 4 255 255", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, false);


    //****** pek bert ******//
    // normal usages
    test_int_args("pek bert", COMMAND_PEK_BERT, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("pek bert 0", COMMAND_PEK_BERT, args1, sizeof(args1), fp, false);


    //****** pek eyescan ******//
    // normal usages
    test_int_args("pek eyescan", COMMAND_PEK_EYESCAN, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("pek eyescan 0", COMMAND_PEK_EYESCAN, args1, sizeof(args1), fp, false);


    //****** pek gpio debug ******//
    // normal usage
    test_int_args("pek gpio debug", COMMAND_PEK_GPIO_DEBUG, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("pek gpio debug 0", COMMAND_PEK_GPIO_DEBUG, args1, sizeof(args1), fp, false);


    //****** pek iic debug ******//
    // normal usage
    test_int_args("pek iic debug", COMMAND_PEK_IIC_DEBUG, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("pek iic debug 0", COMMAND_PEK_IIC_DEBUG, args1, sizeof(args1), fp, false);


    fprintf(fp, "\n---Failed assertion count: %d---\n", fail_assertion_count);

    fflush(fp);
    fclose(fp);
}
