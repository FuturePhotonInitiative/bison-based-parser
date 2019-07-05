// Test file for a parser. Run main with no arguments.

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// @return if the the test had the same outcome as the assertion
bool test_int_args(char *cmd_str, unsigned char expected_cmd_code, unsigned char *args, unsigned char len_args, FILE *fp, bool assert_fail) {
    fprintf(fp, "command: %s\n", cmd_str);
    command ret_val = parseCommand(cmd_str);
    bool failure = false;
    if (ret_val.command_code != expected_cmd_code) {
        fprintf(fp, "--command codes dont match--%s\n", ret_val.command_code == COMMAND_INVALID ? " the command is invalid" : "");
        fprintf(fp, "  parsed code: %d\nexpected code: %d\n", ret_val.command_code, expected_cmd_code);
        failure = true;
    } else {
        fprintf(fp, "command codes match\n");

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

    fprintf(fp, "%s\n", failure ? "FAILURE" : "SUCCESS");

    fprintf(fp, "\n");

    return !(failure == assert_fail);
}

int main() {

    FILE *fp;
    fp = fopen("new_commands.txt", "w");

    unsigned char args1[1];
    unsigned char args2[2];

    int fail_count = 0;

    args1[0] = CFP_PORT_PRG_CNTL1;
    fail_count += test_int_args("cfp gpio read prg_cntl1", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp);

    args1[0] = CFP_PORT_PRG_CNTL2;
    fail_count += test_int_args("cfp gpio read prg_cntl2", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp);

    args1[0] = CFP_PORT_PRG_CNTL3;
    fail_count += test_int_args("cfp gpio read prg_cntl3", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp);

    args1[0] = CFP_PORT_TX_DIS;
    fail_count += test_int_args("cfp gpio read tx_dis", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp);

    args1[0] = CFP_PORT_MOD_LOPWR;
    fail_count += test_int_args("cfp gpio read mod_lopwr", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_MOD_RSTN;
    // test_int_args("cfp gpio read mod_rstn", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_ALL;
    // test_int_args("cfp gpio read all", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL1;
    // test_int_args("cfp gpio set prg_cntl1", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL2;
    // test_int_args("cfp gpio set prg_cntl2", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL3;
    // test_int_args("cfp gpio set prg_cntl3", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_TX_DIS;
    // test_int_args("cfp gpio set tx_dis", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_MOD_LOPWR;
    // test_int_args("cfp gpio set mod_lopwr", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_MOD_RSTN;
    // test_int_args("cfp gpio set mod_rstn", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL1;
    // test_int_args("cfp gpio clear prg_cntl1", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL2;
    // test_int_args("cfp gpio clear prg_cntl2", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL3;
    // test_int_args("cfp gpio clear prg_cntl3", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_TX_DIS;
    // test_int_args("cfp gpio clear tx_dis", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_MOD_LOPWR;
    // test_int_args("cfp gpio clear mod_lopwr", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_MOD_RSTN;
    // test_int_args("cfp gpio clear mod_rstn", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL1;
    // test_int_args("cfp gpio toggle prg_cntl1", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL2;
    // test_int_args("cfp gpio toggle prg_cntl2", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_PRG_CNTL3;
    // test_int_args("cfp gpio toggle prg_cntl3", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_TX_DIS;
    // test_int_args("cfp gpio toggle tx_dis", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_MOD_LOPWR;
    // test_int_args("cfp gpio toggle mod_lopwr", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = CFP_PORT_MOD_RSTN;
    // test_int_args("cfp gpio toggle mod_rstn", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // test_int_args("cfp debug", COMMAND_CFP_DEBUG, 0, 0, fp);

    // args1[0] = GPIO_PORT_MODSEL;
    // test_int_args("qsfp gpio read modsel", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_RESET;
    // test_int_args("qsfp gpio read reset", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_LPMODE;
    // test_int_args("qsfp gpio read lpmode", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_MODPRS;
    // test_int_args("qsfp gpio read modprs", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_INT;
    // test_int_args("qsfp gpio read int", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_ALL;
    // test_int_args("qsfp gpio read all", COMMAND_QSFP_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_MODSEL;
    // test_int_args("qsfp gpio set modsel", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_RESET;
    // test_int_args("qsfp gpio set reset", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_LPMODE;
    // test_int_args("qsfp gpio set lpmode", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_MODSEL;
    // test_int_args("qsfp gpio clear modsel", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_RESET;
    // test_int_args("qsfp gpio clear reset", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_LPMODE;
    // test_int_args("qsfp gpio clear lpmode", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_MODSEL;
    // test_int_args("qsfp gpio toggle modsel", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_RESET;
    // test_int_args("qsfp gpio toggle reset", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_LPMODE;
    // test_int_args("qsfp gpio toggle lpmode", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args3[0] = 0;
    // args3[1] = 0;
    // args3[2] = 0;
    // test_int_args("qsfp iic read 0 0 0", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp);

    // args3[0] = 3;
    // args3[1] = 255;
    // args3[2] = 255;
    // test_int_args("qsfp iic read 3 255 255", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp);

    // args3[0] = 2;
    // args3[1] = 200;
    // args3[2] = 154;
    // test_int_args("qsfp iic read 2 200 154", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp);

    // test_int_args("qsfp bert", COMMAND_QSFP_BERT, 0, 0, fp);

    // test_int_args("qsfp eyescan", COMMAND_QSFP_EYESCAN, 0, 0, fp);

    // test_int_args("qsfp debug", COMMAND_QSFP_DEBUG, 0, 0, fp);

    // test_int_args("vcu108 debug", COMMAND_VCU108_DEBUG, 0, 0, fp);

    // /* max and min values for each */
    // args2[0] = VCU108_PORT_LEDS;
    // args2[1] = 0;
    // test_int_args("vcu108 gpio read leds 0", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // args2[0] = VCU108_PORT_LEDS;
    // args2[1] = 7;
    // test_int_args("vcu108 gpio read leds 7", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // args2[0] = VCU108_PORT_BUTTONS;
    // args2[1] = 0;
    // test_int_args("vcu108 gpio read buttons 0", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // args2[0] = VCU108_PORT_BUTTONS;
    // args2[1] = 4;
    // test_int_args("vcu108 gpio read buttons 4", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // args2[0] = VCU108_PORT_SWITCHES;
    // args2[1] = 0;
    // test_int_args("vcu108 gpio read switches 0", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // args2[0] = VCU108_PORT_SWITCHES;
    // args2[1] = 3;
    // test_int_args("vcu108 gpio read switches 3", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // args1[0] = VCU108_PORT_ALL;
    // test_int_args("vcu108 gpio read all", COMMAND_VCU108_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = 0;
    // test_int_args("vcu108 gpio set 0", COMMAND_VCU108_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = 7;
    // test_int_args("vcu108 gpio set 7", COMMAND_VCU108_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = 0;
    // test_int_args("vcu108 gpio clear 0", COMMAND_VCU108_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = 7;
    // test_int_args("vcu108 gpio clear 7", COMMAND_VCU108_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = 0;
    // test_int_args("vcu108 gpio toggle 0", COMMAND_VCU108_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = 7;
    // test_int_args("vcu108 gpio toggle 7", COMMAND_VCU108_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_MODSEL;
    // test_int_args("pek gpio read modsel", COMMAND_PEK_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_RESET;
    // test_int_args("pek gpio read reset", COMMAND_PEK_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_LPMODE;
    // test_int_args("pek gpio read lpmode", COMMAND_PEK_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_MODPRS;
    // test_int_args("pek gpio read modprs", COMMAND_PEK_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_INT;
    // test_int_args("pek gpio read int", COMMAND_PEK_GPIO_READ, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_ALL;
    // test_int_args("pek gpio read all", COMMAND_PEK_GPIO_READ, args1, sizeof(args1), fp);


    // args1[0] = GPIO_PORT_MODSEL;
    // test_int_args("pek gpio set modsel", COMMAND_PEK_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_RESET;
    // test_int_args("pek gpio set reset", COMMAND_PEK_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_LPMODE;
    // test_int_args("pek gpio set lpmode", COMMAND_PEK_GPIO_SET, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_MODSEL;
    // test_int_args("pek gpio clear modsel", COMMAND_PEK_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_RESET;
    // test_int_args("pek gpio clear reset", COMMAND_PEK_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_LPMODE;
    // test_int_args("pek gpio clear lpmode", COMMAND_PEK_GPIO_CLEAR, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_MODSEL;
    // test_int_args("pek gpio toggle modsel", COMMAND_PEK_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_RESET;
    // test_int_args("pek gpio toggle reset", COMMAND_PEK_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // args1[0] = GPIO_PORT_LPMODE;
    // test_int_args("pek gpio toggle lpmode", COMMAND_PEK_GPIO_TOGGLE, args1, sizeof(args1), fp);



    // // no modprs for toggle
    // args1[0] = GPIO_PORT_MODPRS;
    // test_int_args("pek gpio toggle modprs", COMMAND_PEK_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // // no int for toggle
    // args1[0] = GPIO_PORT_INT;
    // test_int_args("pek gpio toggle int", COMMAND_PEK_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // // no all for toggle
    // args1[0] = GPIO_PORT_ALL;
    // test_int_args("pek gpio toggle all", COMMAND_PEK_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // // no modprs for clear
    // args1[0] = GPIO_PORT_MODPRS;
    // test_int_args("pek gpio clear modprs", COMMAND_PEK_GPIO_CLEAR, args1, sizeof(args1), fp);

    // // no int for clear
    // args1[0] = GPIO_PORT_INT;
    // test_int_args("pek gpio clear int", COMMAND_PEK_GPIO_CLEAR, args1, sizeof(args1), fp);

    // // no all for clear
    // args1[0] = GPIO_PORT_ALL;
    // test_int_args("pek gpio clear all", COMMAND_PEK_GPIO_CLEAR, args1, sizeof(args1), fp);

    // // no modprs for set
    // args1[0] = GPIO_PORT_MODPRS;
    // test_int_args("pek gpio set modprs", COMMAND_PEK_GPIO_SET, args1, sizeof(args1), fp);

    // // no int for set
    // args1[0] = GPIO_PORT_INT;
    // test_int_args("pek gpio set int", COMMAND_PEK_GPIO_SET, args1, sizeof(args1), fp);

    // // no all for set
    // args1[0] = GPIO_PORT_ALL;
    // test_int_args("pek gpio set all", COMMAND_PEK_GPIO_SET, args1, sizeof(args1), fp);

    // // 7 is max for vcu108 gpio toggle
    // args1[0] = 8;
    // test_int_args("vcu108 gpio toggle 8", COMMAND_VCU108_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // // 7 is max for vcu108 gpio clear
    // args1[0] = 8;
    // test_int_args("vcu108 gpio clear 8", COMMAND_VCU108_GPIO_CLEAR, args1, sizeof(args1), fp);

    // // 7 is max for vcu108 gpio set
    // args1[0] = 8;
    // test_int_args("vcu108 gpio set 8", COMMAND_VCU108_GPIO_SET, args1, sizeof(args1), fp);

    // // 3 is max for switches
    // args2[0] = VCU108_PORT_SWITCHES;
    // args2[1] = 4;
    // test_int_args("vcu108 gpio read switches 4", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // // 4 is max for buttons
    // args2[0] = VCU108_PORT_BUTTONS;
    // args2[1] = 5;
    // test_int_args("vcu108 gpio read buttons 5", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // // 7 is max for leds
    // args2[0] = VCU108_PORT_LEDS;
    // args2[1] = 8;
    // test_int_args("vcu108 gpio read leds 8", COMMAND_VCU108_GPIO_READ, args2, sizeof(args2), fp);

    // // all should take zero arguments
    // test_int_args("vcu108 gpio read all 0", COMMAND_VCU108_GPIO_READ, args1, sizeof(args2), fp);

    // // should not take any arguments
    // test_int_args("vcu108 debug 0", COMMAND_VCU108_DEBUG, 0, 0, fp);

    // should not take any arguments
    // test_int_args("qsfp debug 0", COMMAND_QSFP_DEBUG, 0, 0, fp);

    // should not take any arguments
    // test_int_args("qsfp eyescan 0", COMMAND_QSFP_EYESCAN, 0, 0, fp);

    // // should not take any arguments
    // test_int_args("qsfp bert 0", COMMAND_QSFP_BERT, 0, 0, fp);

    // // 3 is the limit for arg0
    // test_int_args("qsfp iic read 4 255 255", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp);

    // // 256 will parse to a string instead of an int which sufficiently makes it fail

    // test_int_args("qsfp iic read 4 255 256", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp);

    // // negative numbers will be parsed as strings
    // test_int_args("qsfp iic read -3 255 255", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp);

    // // cannot toggle modprs
    // args1[0] = GPIO_PORT_MODPRS;
    // test_int_args("qsfp gpio toggle modprs", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // // cannot toggle int
    // args1[0] = GPIO_PORT_INT;
    // test_int_args("qsfp gpio toggle int", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // // cannot toggle all
    // args1[0] = GPIO_PORT_ALL;
    // test_int_args("qsfp gpio toggle all", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp);

    // // cannot clear modprs
    // args1[0] = GPIO_PORT_MODPRS;
    // test_int_args("qsfp gpio clear modprs", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // // cannot clear int
    // args1[0] = GPIO_PORT_INT;
    // test_int_args("qsfp gpio clear int", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // // cannot clear all
    // args1[0] = GPIO_PORT_ALL;
    // test_int_args("qsfp gpio clear all", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // // cannot set modprs
    // args1[0] = GPIO_PORT_MODPRS;
    // test_int_args("qsfp gpio set modprs", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp);

    // // cannot set int
    // args1[0] = GPIO_PORT_INT;
    // test_int_args("qsfp gpio set int", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp);

    // // cannot set all
    // args1[0] = GPIO_PORT_ALL;
    // test_int_args("qsfp gpio set all", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp);

    // // cannot toggle all
    // args1[0] = CFP_PORT_ALL;
    // test_int_args("cfp gpio toggle all", COMMAND_CFP_GPIO_TOGGLE, args1, sizeof(args1), fp);
    
    // // cannot clear all
    // args1[0] = CFP_PORT_ALL;
    // test_int_args("cfp gpio clear all", COMMAND_CFP_GPIO_CLEAR, args1, sizeof(args1), fp);

    // // cannot set all
    // args1[0] = CFP_PORT_ALL;
    // test_int_args("cfp gpio set all", COMMAND_CFP_GPIO_SET, args1, sizeof(args1), fp);

    // // misspelled command test
    // args1[0] = CFP_PORT_TX_DIS;
    // test_int_args("cfp gpio read tx_dip", COMMAND_CFP_GPIO_READ, args1, sizeof(args1), fp);

    fflush(fp);
    fclose(fp);
}
