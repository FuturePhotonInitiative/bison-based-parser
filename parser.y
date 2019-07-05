%{
#include "parse_types.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef LOCAL
#include "lex.yy_test.h"
int yydebug = 1;
#else
#include <xil_printf.h>
#include "lex.yy.h"
#endif

void _setupLexInput(char*);
void _cleanupLex();
int yyerror(command*, char const *);
%}

%parse-param {command *command_val}

%token QSFP_CMD
%token PEK_CMD 
%token DAC_CMD 
%token ADC_CMD 
%token I2CREAD_CMD 
%token I2CWRITE_CMD 
%token SPIXFER_CMD 
%token CFP_CMD
%token DEBUG_CMD
%token VCU108_CMD
%token ALL_PORT
%token GPIO 
%token <command_code> GPIO_SUB 
%token <command_code> SUB_CMD 
%token <command_code> ADDR 
%token <command_code> GPIO_PORT
%token <command_code> VCU108_READ_PORT 
%token <num> NUMBER 
%token <command_code> READ 
%token <command_code> WRITE
%token SET
%token CLEAR
%token TOGGLE
%token LIST
%token <str> STR
%token BERT
%token EYESCAN
%token IIC

%type <cmd> command
%type <cmd> cfp_command
%type <cmd> qsfp_command
%type <cmd> vcu108_command
%type <cmd> pek_command
%type <cmd> debug_command

%union {
    char command_code;
    char num;
    char str [MAX_ARGS];
    command cmd;
}

%%

command: 
    cfp_command { *command_val = $1; } |
    qsfp_command { *command_val = $1; } |
    vcu108_command { *command_val = $1; } |
    pek_command { *command_val = $1; } |
	debug_command { *command_val = $1; };

cfp_command: 
    CFP_CMD GPIO READ NUMBER {
        $$.command_code = COMMAND_CFP_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = $4;
    };

qsfp_command: 
    QSFP_CMD GPIO READ GPIO_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = $4;
    } |
    QSFP_CMD GPIO READ ALL_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = GPIO_PORT_ALL;
    } |
    QSFP_CMD GPIO SET GPIO_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_SET;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_GPIO_VALUE
        }
    } |
    QSFP_CMD GPIO CLEAR GPIO_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_CLEAR;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_GPIO_VALUE
        }
    } |
    QSFP_CMD GPIO TOGGLE GPIO_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_TOGGLE;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_GPIO_VALUE
        }
    } |
    QSFP_CMD EYESCAN {
        $$.command_code = COMMAND_QSFP_EYESCAN;
        $$.args_len = 0;	
    } |
    QSFP_CMD BERT {
        $$.command_code = COMMAND_QSFP_BERT;
        $$.args_len = 0;
    } |
    QSFP_CMD IIC READ NUMBER NUMBER NUMBER {
        $$.command_code = COMMAND_QSFP_IIC_READ;
        $$.args_len = 3;
        $$.args[0] = $4;
        $$.args[1] = $5;
        $$.args[2] = $6;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_QSFP_IIC_READ_PAGE;
        }
        else if ($5 > 255) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_QSFP_IIC_READ_START_ADDRESS;
        }
        else if ($6 > 255) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_QSFP_IIC_READ_END_ADDRESS;
        }
    };

vcu108_command:
    VCU108_CMD GPIO READ VCU108_READ_PORT NUMBER {
        $$.command_code = COMMAND_VCU108_GPIO_READ;
        $$.args_len = 2;
        $$.args[0] = $4;
        $$.args[1] = $5;
        switch ($4) {
            case VCU108_PORT_LEDS:
                if ($5 > 7) {
                    $$.command_code = COMMAND_INVALID;
                    $$.args_len = 1;
                    $$.args[0] = ERROR_INVALID_GPIO_VALUE;
                }
                break;
            case VCU108_PORT_BUTTONS:
                if ($5 > 4) {
                    $$.command_code = COMMAND_INVALID;
                    $$.args_len = 1;
                    $$.args[0] = ERROR_INVALID_GPIO_VALUE;
                }
                break;
            case VCU108_PORT_SWITCHES:
                if ($5 > 3) {
                    $$.command_code = COMMAND_INVALID;
                    $$.args_len = 1;
                    $$.args[0] = ERROR_INVALID_GPIO_VALUE;
                }
                break;
            default:
                break;
        }
    } |
    VCU108_CMD GPIO READ ALL_PORT {
        $$.command_code = COMMAND_VCU108_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = VCU108_PORT_ALL;
    } |
    VCU108_CMD GPIO SET NUMBER {
        $$.command_code = COMMAND_VCU108_GPIO_SET;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 7) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_GPIO_VALUE;
        }
    } |
    VCU108_CMD GPIO CLEAR NUMBER {
        $$.command_code = COMMAND_VCU108_GPIO_CLEAR;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 7) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_GPIO_VALUE;
        }
    } |
    VCU108_CMD GPIO TOGGLE NUMBER {
        $$.command_code = COMMAND_VCU108_GPIO_TOGGLE;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 7) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_GPIO_VALUE;
        }
    };

pek_command: 
    PEK_CMD IIC WRITE NUMBER NUMBER STR STR {
        $$.command_code = COMMAND_PEK_IIC_WRITE;
        $$.args_len = 2;
        $$.args[0] = $4;
        $$.args[1] = $5;

        // copy datatype string
        int i = 0;
        for (; $6[i] != '\0'; i++) {
            $$.args[$$.args_len] = $6[i];
            $$.args_len++;
        }
        $$.args[$$.args_len] = '\0';
        $$.args_len += 1;
        int value_start = $$.args_len;

        // copy hex or string string
        int i = 0;
        for (; $7[i] != '\0'; i++) {
            $$.args[$$.args_len] = $7[i];
            $$.args_len++;
        }
        $$.args[$$.args_len] = '\0';
        $$.args_len += 1;

        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_IIC_WRITE_PAGE;
        } 
        else if ($5 > 255) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_START_ADDRESS;
        } 
        else if ((strncmp(args + 2, "char", 4) != 0) && (strncmp(args + 2, "str", 3) != 0)) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_IIC_DATA_TYPE;
        }
        else if (!(strlen(args[value_start]) < (255 - $5))) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_PEK_WRITE_DATA_TOO_LONG;
        }
    } |

    // This is the exact same as the above except the 1 byte number is converted back into a string
    PEK_CMD IIC WRITE NUMBER NUMBER STR NUMBER {
        $$.command_code = COMMAND_PEK_IIC_WRITE;
        $$.args_len = 2;
        $$.args[0] = $4;
        $$.args[1] = $5;

        // copy datatype string
        int i = 0;
        for (; $6[i] != '\0'; i++) {
            $$.args[$$.args_len] = $6[i];
            $$.args_len++;
        }
        $$.args[$$.args_len] = '\0';
        $$.args_len += 1;
        int value_start = $$.args_len;

        // ********** v changed
        char hex[3];
        sprintf(hex, "%x", $7);
        // copy hex
        int i = 0;
        for (; hex[i] != '\0'; i++) {
            $$.args[$$.args_len] = hex[i];
        // ********** ^ changed
            $$.args_len++;
        }
        $$.args[$$.args_len] = '\0';
        $$.args_len += 1;

        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_IIC_WRITE_PAGE;
        } 
        else if ($5 > 255) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_START_ADDRESS;
        } 
        else if ((strncmp(args + 2, "char", 4) != 0) && (strncmp(args + 2, "str", 3) != 0)) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_IIC_DATA_TYPE;
        }
        else if (!(strlen(args[value_start]) < (255 - $5))) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_PEK_WRITE_DATA_TOO_LONG;
        }
    } |
    PEK_CMD IIC READ NUMBER NUMBER NUMBER {
        $$.command_code = COMMAND_PEK_IIC_READ;
        $$.args_len = 3;
        $$.args[0] = $4;
        $$.args[1] = $5;
        $$.args[2] = $6;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_IIC_READ_PAGE;
        }
        else if ($5 > 255) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_IIC_READ_START_ADDRESS;
        }
        else if ($6 > 255) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_IIC_READ_END_ADDRESS;
        }
    } |

    PEK_CMD GPIO READ GPIO_PORT {
        $$.command_code = COMMAND_PEK_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = $4;
    } |







             PEK_CMD GPIO SET NUMBER NUMBER
           {
                $$.command_code = COMMAND_PEK_GPIO_SET;
                $$.args_len = 2;
                if($4 < 0 || $4 > 3)
                {
                        $$.command_code = COMMAND_INVALID;
                        $$.args_len = 1;
                        $$.args[0] = ERROR_INVALID_PEK_PORT;
                }
                else
                {
                        $$.args[0] = $4; 
                }
                if($5 < 0 || $5 > 7)
                {
                        $$.command_code = COMMAND_INVALID;
                        if($$.args_len == 1)
                        {
                                $$.args_len = 2;
                                $$.args[1] = ERROR_INVALID_PEK_PIN; 
                        } 
                        else
                        {
                                $$.args_len = 1;
                                $$.args[0] = ERROR_INVALID_PEK_PIN;
                        }
                }
                else
                {
                        $$.args[1] = $5;
                }
           } |
             PEK_CMD GPIO CLEAR NUMBER NUMBER
           {
                $$.command_code = COMMAND_PEK_GPIO_CLEAR;
                $$.args_len = 2;
                if($4 < 0 || $4 > 3)
                {
                        $$.command_code = COMMAND_INVALID;
                        $$.args_len = 1;
                        $$.args[0] = ERROR_INVALID_PEK_PORT;
                }
                else
                {
                        $$.args[0] = $4; 
                }
                if($5 < 0 || $5 > 7)
                {
                        $$.command_code = COMMAND_INVALID;
                        if($$.args_len == 1)
                        {
                                $$.args_len = 2;
                                $$.args[1] = ERROR_INVALID_PEK_PIN; 
                        } 
                        else
                        {
                                $$.args_len = 1;
                                $$.args[0] = ERROR_INVALID_PEK_PIN;
                        }
                }
                else
                {
                        $$.args[1] = $5;
                }
           } |
             PEK_CMD GPIO TOGGLE NUMBER NUMBER
           {
                $$.command_code = COMMAND_PEK_GPIO_TOGGLE;
                $$.args_len = 2;
                if($4 < 0 || $4 > 3)
                {
                        $$.command_code = COMMAND_INVALID;
                        $$.args_len = 1;
                        $$.args[0] = ERROR_INVALID_PEK_PORT;
                }
                else
                {
                        $$.args[0] = $4; 
                }
                if($5 < 0 || $5 > 7)
                {
                        $$.command_code = COMMAND_INVALID;
                        if($$.args_len == 1)
                        {
                                $$.args_len = 2;
                                $$.args[1] = ERROR_INVALID_PEK_PIN; 
                        } 
                        else
                        {
                                $$.args_len = 1;
                                $$.args[0] = ERROR_INVALID_PEK_PIN;
                        }
                }
                else
                {
                        $$.args[1] = $5;
                }
           } 
        ;

debug_command: 
    DEBUG_CMD VCU108_CMD {
		$$.command_code = COMMAND_DEBUG_VCU108;
		$$.args_len = 0;
	} |
	DEBUG_CMD QSFP_CMD {
	    $$.command_code = COMMAND_DEBUG_QSFP;
		$$.args_len = 0;
	};

%%

int yyerror(command* val, char const * msg)
{
        (*val).command_code = -1;
        (*val).args_len = 1;
        (*val).args[0] = ERROR_OTHER;
        return -1;
}

command parseCommand(char* commandStr)
{
        command command_val;
        _setupLexInput(commandStr);
        int yyparseReturn = yyparse(&command_val);
		_cleanupLex();
        if(yyparseReturn == 0)
        {
                return command_val;
        }
        command_val.command_code = -1;
        return command_val;
}

