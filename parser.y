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

%token CFP_CMD
%token QSFP_CMD
%token VCU108_CMD
%token PEK_CMD  
%token ALL_PORT
%token GPIO 
%token <command_code> GPIO_PORT
%token <command_code> VCU108_PORT 
%token <command_code> CFP_PORT
%token <num> NUMBER 
%token READ 
%token WRITE
%token DEBUG
%token SET
%token CLEAR
%token TOGGLE
%token LIST
%token <str> STR
%token BERT
%token EYESCAN
%token IIC
%token HEX
%token CHAR

%type <cmd> command
%type <cmd> cfp_command
%type <cmd> qsfp_command
%type <cmd> vcu108_command
%type <cmd> pek_command

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
    pek_command { *command_val = $1; };

cfp_command: 
    CFP_CMD GPIO READ CFP_PORT {
        $$.command_code = COMMAND_CFP_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = $4;
    } |
    CFP_CMD GPIO READ ALL_PORT {
        $$.command_code = COMMAND_CFP_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = CFP_PORT_ALL;
    } |
    CFP_CMD GPIO SET CFP_PORT {
        $$.command_code = COMMAND_CFP_GPIO_SET;
        $$.args_len = 1;
        $$.args[0] = $4;
    } |
    CFP_CMD GPIO CLEAR CFP_PORT {
        $$.command_code = COMMAND_CFP_GPIO_CLEAR;
        $$.args_len = 1;
        $$.args[0] = $4;
    } |
    CFP_CMD GPIO TOGGLE CFP_PORT {
        $$.command_code = COMMAND_CFP_GPIO_TOGGLE;
        $$.args_len = 1;
        $$.args[0] = $4;
    } |
    CFP_CMD DEBUG {
        $$.command_code = COMMAND_CFP_DEBUG;
        $$.args_len = 0;
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
            $$.args[0] = ERROR_INVALID_GPIO_VALUE;
        }
    } |
    QSFP_CMD GPIO CLEAR GPIO_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_CLEAR;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_GPIO_VALUE;
        }
    } |
    QSFP_CMD GPIO TOGGLE GPIO_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_TOGGLE;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_GPIO_VALUE;
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
    } |
    QSFP_CMD DEBUG {
	    $$.command_code = COMMAND_QSFP_DEBUG;
		$$.args_len = 0;
	};


vcu108_command:
    VCU108_CMD GPIO READ VCU108_PORT NUMBER {
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
    } |
    VCU108_CMD DEBUG {
		$$.command_code = COMMAND_VCU108_DEBUG;
		$$.args_len = 0;
	};


pek_command: 
    PEK_CMD IIC WRITE NUMBER NUMBER HEX STR {
        $$.command_code = COMMAND_PEK_IIC_WRITE;
        $$.args_len = 2;
        $$.args[0] = $4;
        $$.args[1] = $5;

        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_IIC_WRITE_PAGE;
        }
        else {
            sprintf($$.args + $$.args_len, "%s", "hex");
            $$.args_len += 4;
            sprintf($$.args + $$.args_len, "%s", $7);
            $$.args_len += strlen($7) + 1;
        }
    } |
    PEK_CMD IIC WRITE NUMBER NUMBER CHAR STR {
        $$.command_code = COMMAND_PEK_IIC_WRITE;
        $$.args_len = 2;
        $$.args[0] = $4;
        $$.args[1] = $5;

        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_IIC_WRITE_PAGE;
        }
        else {
            sprintf($$.args + $$.args_len, "%s", "char");
            $$.args_len += 5;
            sprintf($$.args + $$.args_len, "%s", $7);
            $$.args_len += strlen($7) + 1;
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
    PEK_CMD GPIO READ ALL_PORT {
        $$.command_code = COMMAND_PEK_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = GPIO_PORT_ALL;
    } |
    PEK_CMD GPIO SET GPIO_PORT {
        $$.command_code = COMMAND_PEK_GPIO_SET;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_PORT;
        }
    } |
    PEK_CMD GPIO CLEAR GPIO_PORT {
        $$.command_code = COMMAND_PEK_GPIO_CLEAR;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_PORT;
        }
    } |
    PEK_CMD GPIO TOGGLE GPIO_PORT {
        $$.command_code = COMMAND_PEK_GPIO_TOGGLE;
        $$.args_len = 1;
        $$.args[0] = $4;
        if ($4 > 3) {
            $$.command_code = COMMAND_INVALID;
            $$.args_len = 1;
            $$.args[0] = ERROR_INVALID_PEK_PORT;
        }
    } |
    PEK_CMD BERT {
        $$.command_code = COMMAND_PEK_BERT;
        $$.args_len = 0;
    } |
    PEK_CMD EYESCAN {
        $$.command_code = COMMAND_PEK_EYESCAN;
        $$.args_len = 0;
    };

%%

int yyerror(command* val, char const * msg) {
    (*val).command_code = -1;
    (*val).args_len = 1;
    (*val).args[0] = ERROR_OTHER;
    return -1;
}

command parseCommand(char* commandStr) {
    command command_val;
    _setupLexInput(commandStr);
    int yyparseReturn = yyparse(&command_val);
    _cleanupLex();
    if (yyparseReturn == 0) {
        return command_val;
    }
    command_val.command_code = -1;
    return command_val;
}
