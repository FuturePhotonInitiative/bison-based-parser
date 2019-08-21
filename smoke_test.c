// Test file for a parser. Run main with no arguments.

#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// how many tests failed to match the assertion
int fail_assertion_count = 0;

/**
 * Prints if the command was valid or not. If the command was parsed to a validity not expected
 * then print the error code, the inputted command, and it it either failed instead of succeeded or succeeded instead of failed.
 * 
 * @param ret_val the command structure that was returned from parsing
 * @param failed if the command failed to be parsed or not
 * @param should_succeed if the command should have succeeded or not
 * @param cmd_str the command string that was input
 * @param fp the file pointer to log debug messages to
 */
void print_and_record_result(command ret_val, bool failed, bool should_succeed, char *cmd_str, FILE *fp) {
    fprintf(fp, "%s\n\n", failed ? "FAILURE" : "SUCCESS");
    if (failed == should_succeed) {
        fail_assertion_count += 1;
        fprintf(fp, "Error code: %d\n", ret_val.args[0]);
        fprintf(fp, "assertion failed\n");
        char message1[] = "FAILED instead of SUCCEEDED";
        char message2[] = "SUCCEEDED instead of FAILED";
        fprintf(fp, "%s %s\n\n", cmd_str, failed ? message1 : message2);
    }
}

/**
 * Tests a command that has all its arguments as unsigned characters. This will: 
 * - run the command string through the parser,
 * - test if the struct's command code matches. If it does not, print the expected and actual command code.
 * - If it does, check the arguments for validity and the correct length. In any case, print what the actual and expected values are.
 * - record if the test did not meet expectations
 * 
 * @param cmd_str the command string to run through the parser
 * @param expected_cmd_code the command code the command should parse to
 * @param args an array that the command struct's arguments should be
 * @param len_args the length in bytes that the argument array should be
 * @param fp the file pointer to log debug messages to
 * @param assert_success if the parsing should succeed or not
 */
void test_int_args(char *cmd_str, unsigned char expected_cmd_code, unsigned char *args, unsigned char len_args, FILE *fp, bool assert_success) {
    fprintf(fp, "command: %s\n", cmd_str);
    command ret_val = parseCommand(cmd_str);
    bool failure = false;
    if (ret_val.command_code != expected_cmd_code) {
        fprintf(fp, "--command codes dont match--%s\n", ret_val.command_code == COMMAND_INVALID ? (char *)(ret_val.args + 1) : "");
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

    print_and_record_result(ret_val, failure, assert_success, cmd_str, fp);
    fflush(fp);
}

/**
 * Tests a pek iic write command for its validity. One of these commands is a string, so test_int_args will not work.
 * It does everything that test_int_args does except it pays special attention to the string argument.
 * 
 * @param cmd_str the command string to run through the parser
 * @param args an array that the command struct's args should be less the string on the end
 * @param string the string that the command struct's string argument should be exactly
 * @param fp the file pointer to log debug messages to
 * @param assert_success if the parsing should succeed or not
 */ 
void test_pek_iic_write(char *cmd_str, unsigned char *args, char *string, FILE *fp, bool assert_success) {
    fprintf(fp, "command: %s\n", cmd_str);
    command ret_val = parseCommand(cmd_str);
    bool failure = false;
    if (ret_val.command_code != COMMAND_PEK_IIC_WRITE) {
        fprintf(fp, "--command codes dont match--%s\n", 
                (ret_val.command_code == COMMAND_INVALID && ret_val.args[0] == ERROR_OTHER) ? (char *)(ret_val.args + 1) : "");
        fprintf(fp, "  parsed code: %d\nexpected code: %d\n", ret_val.command_code, COMMAND_PEK_IIC_WRITE);
        failure = true;
    } else {
        fprintf(fp, "command codes match\n");
        if (args[0] != ret_val.args[0] || args[1] != ret_val.args[1] || args[2] != ret_val.args[2] || strncmp(string, (const char *)&ret_val.args[3], strlen(string)) != 0) {
            fprintf(fp, "--args do not match--\n");
            fprintf(fp, "  parsed args {%d %d %d %s}\n", ret_val.args[0], ret_val.args[1], ret_val.args[2], &ret_val.args[3]);
            fprintf(fp, "expected args {%d %d %d %s}\n", args[0], args[1], args[2], string);
            failure = true;
        }
    }

    print_and_record_result(ret_val, failure, assert_success, cmd_str, fp);
    fflush(fp);
}

/**
 * Tests a cfp mdio write command for its validity. One of these commands is a string, so test_int_args will not work.
 * It does everything that test_int_args does except it pays special attention to the string argument.
 * 
 * @param cmd_str the command string to run through the parser
 * @param args an array that the command struct's args should be less the string on the end
 * @param string the string that the command struct's string argument should be exactly
 * @param fp the file pointer to log debug messages to
 * @param assert_success if the parsing should succeed or not
 */ 
void test_mdio_write(char *cmd_str, unsigned char *args, char *string, FILE *fp, bool assert_success) {
    fprintf(fp, "command: %s\n", cmd_str);
    command ret_val = parseCommand(cmd_str);
    bool failure = false;
    if (ret_val.command_code != COMMAND_CFP_MDIO_WRITE) {
        fprintf(fp, "--command codes dont match--%s\n", ret_val.command_code == COMMAND_INVALID ? (char *)(ret_val.args + 1) : "");
        fprintf(fp, "  parsed code: %d\nexpected code: %d\n", ret_val.command_code, COMMAND_CFP_MDIO_WRITE);
        failure = true;
    }
    else {
        fprintf(fp, "command codes match\n");
        if (args[0] != ret_val.args[0] || strncmp(string, (const char *)ret_val.args + 1, strlen(string) != 0)) {
            fprintf(fp, "--args do not match--\n");
            fprintf(fp, "  parsed args {%d %s}\n", ret_val.args[0], ret_val.args + 1);
            fprintf(fp, "expected args {%d %s}\n", args[0], string);
            failure = true;
        }
    }

    print_and_record_result(ret_val, failure, assert_success, cmd_str, fp);
    fflush(fp);
}

/**
 * A helper function for running a pek gpio read test.
 * 
 * @param port the port argument that the input command string should contain
 * @param pin the pin argument that theinput command string should contain
 * @param fp the file pointer to log debug messages to
 * @param success if the command should succeed or not
 */ 
void test_pek_gpio_read(unsigned char port, unsigned char pin, FILE *fp, bool success) {
    unsigned char args2[2];
    args2[0] = port;
    args2[1] = pin;
    char command_str[50];
    sprintf(command_str, "pek gpio read %d %d", port, pin);
    test_int_args(command_str, COMMAND_PEK_GPIO_READ, args2, sizeof(args2), fp, success);
}

/**
 * A helper function for running a pek gpio (set, clear or toggle) test
 * 
 * @param base_command_str "pek gpio set", "pek gpio clear" or "pek gpio toggle"
 * @param command_str an array that one can put a command string into
 * @param port the port to put as an arg in the command string
 * @param pin the pin to put as an arg in the command string
 * @param command_code the command code to test
 * @param fp the file pointer to log debug messages to
 * @param success if the command should succeed or not
 */ 
void test_pek_gpio_sct(
        char *base_command_str, 
        char *command_str, 
        unsigned char port, 
        unsigned char pin,
        unsigned char command_code, 
        FILE *fp, 
        bool success) {
    unsigned char args2[2];
    args2[0] = port;
    args2[1] = pin;
    sprintf(command_str, "%s %d %d", base_command_str, port, pin);
    test_int_args(command_str, command_code, args2, sizeof(args2), fp, success);
}

/**
 * This function tests all the pek gpio (set, clear, or toggle) commands. There are an easilly enumerable amount of cases that it
 * should succeed or fail in, so this tests all of them
 * 
 * @param sct the string (either "set", "clear", or "toggle")
 * @param fp the file pointer to log debug messages to
 */ 
void test_all_pek_gpio_set_clear_toggle(char *sct, FILE *fp) {
    char base_command_str[20];
    sprintf(base_command_str, "pek gpio %s", sct);
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

/**
 * Runs all the test cases that we care to test for the frontend of the Bison parser.
 * The status of every command tested is logged to the file (currently "new_commands.txt" in the same directory as the exe)
 */ 
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


    //****** cfp mdio read ******//
    // normal usages
    args1[0] = 0;
    test_int_args("cfp mdio read 0", COMMAND_CFP_MDIO_READ, args1, sizeof(args1), fp, true);
    args1[0] = 31;
    test_int_args("cfp mdio read 31", COMMAND_CFP_MDIO_READ, args1, sizeof(args1), fp, true);
    // to large number
    args1[0] = 32;
    test_int_args("cfp mdio read 32", COMMAND_CFP_MDIO_READ, args1, sizeof(args1), fp, false);
    // must take an arg
    test_int_args("cfp mdio read", COMMAND_CFP_MDIO_READ, args1, sizeof(args1), fp, false);
    // must not take more than one arg
    test_int_args("cfp mdio read 0 0", COMMAND_CFP_MDIO_READ, args1, sizeof(args1), fp, false);


    //****** cfp mdio write ******//
    char command_string[50];
    char two_byte_number[7];
    // normal usages
    args1[0] = 0;
    strncpy(two_byte_number, "0x0000", 6);
    sprintf(command_string, "cfp mdio write %d %s", args1[0], two_byte_number);
    test_mdio_write(command_string, args1, two_byte_number, fp, true);
    args1[0] = 31;
    strncpy(two_byte_number, "0x0000", 6);
    sprintf(command_string, "cfp mdio write %d %s", args1[0], two_byte_number);
    test_mdio_write(command_string, args1, two_byte_number, fp, true);
    args1[0] = 0;
    strncpy(two_byte_number, "0xFFFF", 6);
    sprintf(command_string, "cfp mdio write %d %s", args1[0], two_byte_number);
    test_mdio_write(command_string, args1, two_byte_number, fp, true);
    args1[0] = 31;
    strncpy(two_byte_number, "0xFFFF", 6);
    sprintf(command_string, "cfp mdio write %d %s", args1[0], two_byte_number);
    test_mdio_write(command_string, args1, two_byte_number, fp, true);
    args1[0] = 23;
    strncpy(two_byte_number, "0x8f9a", 6);
    sprintf(command_string, "cfp mdio write %d %s", args1[0], two_byte_number);
    test_mdio_write(command_string, args1, two_byte_number, fp, true);
    // more than two bytes
    args1[0] = 0;
    char number_too_long[8];
    strncpy(two_byte_number, "0xFFFF0", 7);
    sprintf(command_string, "cfp mdio write %d %s", args1[0], number_too_long);
    test_mdio_write(command_string, args1, number_too_long, fp, false);
    // less than two bytes
    args1[0] = 0;
    strncpy(two_byte_number, "0xFFF", 6);
    sprintf(command_string, "cfp mdio write %d %s", args1[0], two_byte_number);
    test_mdio_write(command_string, args1, two_byte_number, fp, false);
    // more than 32 is invalid
    args1[0] = 32;
    strncpy(two_byte_number, "0x8f9a", 6);
    sprintf(command_string, "cfp mdio write %d %s", args1[0], two_byte_number);
    test_mdio_write(command_string, args1, two_byte_number, fp, false);


    //****** cfp mdio write ******//
    // normal usages
    test_int_args("cfp mdio debug", COMMAND_CFP_MDIO_DEBUG, 0, 0, fp, true);
    // should not take any arguments
    args1[0] = 0;
    test_int_args("cfp mdio debug 0", COMMAND_CFP_MDIO_DEBUG, args1, sizeof(args1), fp, false);


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
    args1[0] = GPIO_PORT_ALL;
    test_int_args("qsfp gpio set all", COMMAND_QSFP_GPIO_SET, args1, sizeof(args1), fp, true);


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
    args1[0] = GPIO_PORT_ALL;
    test_int_args("qsfp gpio clear all", COMMAND_QSFP_GPIO_CLEAR, args1, sizeof(args1), fp, true);


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
    args1[0] = GPIO_PORT_ALL;
    test_int_args("qsfp gpio toggle all", COMMAND_QSFP_GPIO_TOGGLE, args1, sizeof(args1), fp, true);


    //****** qsfp iic read ******//
    // normal usages
    args3[0] = 0;
    args3[1] = 0;
    args3[2] = 1;
    test_int_args("qsfp iic read 0 0 1", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 3;
    args3[1] = 254;
    args3[2] = 255;
    test_int_args("qsfp iic read 3 254 255", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 2;
    args3[1] = 154;
    args3[2] = 200;
    test_int_args("qsfp iic read 2 154 200", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, true);
    // start > end
    args3[0] = 2;
    args3[1] = 200;
    args3[2] = 154;
    test_int_args("qsfp iic read 2 200 154", COMMAND_QSFP_IIC_READ, args3, sizeof(args3), fp, false);
    // 3 is the limit for arg0
    args3[0] = 4;
    args3[1] = 255;
    args3[2] = 255;
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
    char curr_str[80];
    char *str = malloc(50);
    // normal usages, min, max for hex
    strncpy(str, "00", 3);
    args3[0] = 0;
    args3[1] = 0;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, true);

    strncpy(str, "00", 3);
    args3[0] = 3;
    args3[1] = 253;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, true);

    strncpy(str, "00 6a 7C", 9);
    args3[0] = 1;
    args3[1] = 201;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, true);

    // page cannot be greater than 3
    strncpy(str, "00", 3);
    args3[0] = 4;
    args3[1] = 0;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);

    // len !< (255 - start)
    strncpy(str, "00", 3);
    args3[0] = 0;
    args3[1] = 254;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);

    // len !< (255 - start)
    strncpy(str, "00 6a 7C", 9);
    args3[0] = 0;
    args3[1] = 254;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);

    // invalid hex
    strncpy(str, "00 6a 7C 8G", 12);
    args3[0] = 0;
    args3[1] = 254;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);

    // invalid hex
    strncpy(str, "00 6a 7C 8D ", 13);
    args3[0] = 0;
    args3[1] = 254;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);

    // invalid hex
    strncpy(str, "006a7C8D", 9);
    args3[0] = 0;
    args3[1] = 254;
    args3[2] = WRITE_DATA_TYPE_HEX;
    sprintf(curr_str, "pek iic write %d %d hex \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);

    // normal usages, min, max for char
    strncpy(str, "h", 2);
    args3[0] = 0;
    args3[1] = 0;
    args3[2] = WRITE_DATA_TYPE_CHAR;
    sprintf(curr_str, "pek iic write %d %d char \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, true);

    strncpy(str, "h", 2);
    args3[0] = 3;
    args3[1] = 253;
    args3[2] = WRITE_DATA_TYPE_CHAR;
    sprintf(curr_str, "pek iic write %d %d char \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, true);

    strncpy(str, "Hello, World!", 14);
    args3[0] = 2;
    args3[1] = 201;
    args3[2] = WRITE_DATA_TYPE_CHAR;
    sprintf(curr_str, "pek iic write %d %d char \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, true);

    // page cannot be greater than 3
    strncpy(str, "h", 2);
    args3[0] = 4;
    args3[1] = 0;
    args3[2] = WRITE_DATA_TYPE_CHAR;
    sprintf(curr_str, "pek iic write %d %d char \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);

    // len !< (255 - start)
    strncpy(str, "h", 2);
    args3[0] = 0;
    args3[1] = 254;
    args3[2] = WRITE_DATA_TYPE_CHAR;
    sprintf(curr_str, "pek iic write %d %d char \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);

    // len !< (255 - start)
    strncpy(str, "hel", 4);
    args3[0] = 0;
    args3[1] = 254;
    args3[2] = WRITE_DATA_TYPE_CHAR;
    sprintf(curr_str, "pek iic write %d %d char \"%s\"", args3[0], args3[1], str);
    test_pek_iic_write(curr_str, args3, str, fp, false);


    //****** pek iic read ******//
    // normal usages, min, max
    // normal usages
    args3[0] = 0;
    args3[1] = 0;
    args3[2] = 1;
    test_int_args("pek iic read 0 0 1", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 3;
    args3[1] = 254;
    args3[2] = 255;
    test_int_args("pek iic read 3 254 255", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, true);
    args3[0] = 2;
    args3[1] = 154;
    args3[2] = 200;
    test_int_args("pek iic read 2 154 200", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, true);
    // start > end
    args3[0] = 2;
    args3[1] = 200;
    args3[2] = 154;
    test_int_args("pek iic read 2 200 154", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, false);
    // 3 is the limit for arg0
    args3[0] = 4;
    args3[1] = 255;
    args3[2] = 255;
    test_int_args("pek iic read 4 255 255", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, false);
    // 256 will parse to a string instead of an int which sufficiently makes it fail
    test_int_args("pek iic read 3 255 256", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, false);
    // negative numbers will be parsed as strings
    test_int_args("pek iic read -3 255 255", COMMAND_PEK_IIC_READ, args3, sizeof(args3), fp, false);


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


    test_int_args("pek debug", COMMAND_PEK_GPIO_DEBUG, 0, 0, fp, false);


    fprintf(fp, "\n---Failed assertion count: %d---\n", fail_assertion_count);

    fflush(fp);
    fclose(fp);
}
