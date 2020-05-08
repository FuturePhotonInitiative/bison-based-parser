%{
#include "parse_types.h"
#include "parse_codes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef LOCAL
#include "lexer_test.h"
int yydebug = 1;
#else
#include "lexer.h"
#endif

void _setupLexInput(char*);
void _cleanupLex();
void invalidateCommand(command *, unsigned char);
int yyerror(command*, char const *);
%}

%define parse.error verbose

%parse-param {command *command_val}

%token CFP_CMD
%token QSFP_CMD
%token VCU108_CMD
%token PEK_CMD
%token ALL_PORT
%token GPIO
%token MDIO
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
%token <str> STR
%token <str> HEX_STR
%token <str> TWO_BYTES
%token BERT
%token EYESCAN
%token IIC
%token CHAR
%token HEX

%type <cmd> command
%type <cmd> cfp_command
%type <cmd> qsfp_command
%type <cmd> vcu108_command
%type <cmd> pek_command

%union {
    char command_code;
    unsigned char num;
    char str[MAX_ARGS];
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
    CFP_CMD GPIO DEBUG {
        $$.command_code = COMMAND_CFP_GPIO_DEBUG;
        $$.args_len = 0;
    } |
    CFP_CMD MDIO READ NUMBER {
        if ($4 > 31) {
            invalidateCommand(&$$, ERROR_INVALID_CFP_MDIO_ADDRESS);
        } else {
            $$.command_code = COMMAND_CFP_MDIO_READ;
            $$.args_len = 1;
            $$.args[0] = $4;
        }
    } |
    CFP_CMD MDIO WRITE NUMBER TWO_BYTES {
        if ($4 > 31) {
            invalidateCommand(&$$, ERROR_INVALID_CFP_MDIO_ADDRESS);
        } else {
            $$.command_code = COMMAND_CFP_MDIO_WRITE;
            $$.args_len = 1 + strlen($5);
            $$.args[0] = $4;
            strncpy((char *)$$.args + 1, $5, strlen($5));
        }
    } |
    CFP_CMD MDIO DEBUG {
        $$.command_code = COMMAND_CFP_MDIO_DEBUG;
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
        switch ($4) {
            case GPIO_PORT_MODSEL:
            case GPIO_PORT_RESET:
            case GPIO_PORT_LPMODE:
                $$.command_code = COMMAND_QSFP_GPIO_SET;
                $$.args_len = 1;
                $$.args[0] = $4;
                break;
            default:
                invalidateCommand(&$$, ERROR_INVALID_GPIO_PORT);
        }
    } |
    QSFP_CMD GPIO SET ALL_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_SET;
        $$.args_len = 1;
        $$.args[0] = GPIO_PORT_ALL;
    } |
    QSFP_CMD GPIO CLEAR GPIO_PORT {
        switch ($4) {
            case GPIO_PORT_MODSEL:
            case GPIO_PORT_RESET:
            case GPIO_PORT_LPMODE:
                $$.command_code = COMMAND_QSFP_GPIO_CLEAR;
                $$.args_len = 1;
                $$.args[0] = $4;
                break;
            default:
                invalidateCommand(&$$, ERROR_INVALID_GPIO_PORT);
        }
    } |
    QSFP_CMD GPIO CLEAR ALL_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_CLEAR;
        $$.args_len = 1;
        $$.args[0] = GPIO_PORT_ALL;
    } |
    QSFP_CMD GPIO TOGGLE GPIO_PORT {
        switch ($4) {
            case GPIO_PORT_MODSEL:
            case GPIO_PORT_RESET:
            case GPIO_PORT_LPMODE:
                $$.command_code = COMMAND_QSFP_GPIO_TOGGLE;
                $$.args_len = 1;
                $$.args[0] = $4;
                break;
            default:
                invalidateCommand(&$$, ERROR_INVALID_GPIO_PORT);
        }
    } |
    QSFP_CMD GPIO TOGGLE ALL_PORT {
        $$.command_code = COMMAND_QSFP_GPIO_TOGGLE;
        $$.args_len = 1;
        $$.args[0] = GPIO_PORT_ALL;
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
        if ($4 > 3) {
            invalidateCommand(&$$, ERROR_INVALID_QSFP_IIC_READ_PAGE);
        }
        else if ($5 >= $6) {
            invalidateCommand(&$$, ERROR_START_ADDR_NOT_LESS_THAN_END);
        } else {
            $$.command_code = COMMAND_QSFP_IIC_READ;
            $$.args_len = 3;
            $$.args[0] = $4;
            $$.args[1] = $5;
            $$.args[2] = $6;
        }
    } |
    QSFP_CMD IIC DEBUG {
        $$.command_code = COMMAND_QSFP_IIC_DEBUG;
        $$.args_len = 0;
    } |
    QSFP_CMD GPIO DEBUG {
	    $$.command_code = COMMAND_QSFP_GPIO_DEBUG;
		$$.args_len = 0;
	};


vcu108_command:
    VCU108_CMD GPIO READ VCU108_PORT NUMBER {
        int failure = 0;
        switch ($4) {
            case VCU108_PORT_LEDS:
                if ($5 > 7) {
                    invalidateCommand(&$$, ERROR_INVALID_GPIO_PIN);
                    failure = 1;
                }
                break;
            case VCU108_PORT_BUTTONS:
                if ($5 > 4) {
                    invalidateCommand(&$$, ERROR_INVALID_GPIO_PIN);
                    failure = 1;
                }
                break;
            case VCU108_PORT_SWITCHES:
                if ($5 > 3) {
                    invalidateCommand(&$$, ERROR_INVALID_GPIO_PIN);
                    failure = 1;
                }
                break;
            default:
                break;
        }
        if (!failure) {
            $$.command_code = COMMAND_VCU108_GPIO_READ;
            $$.args_len = 2;
            $$.args[0] = $4;
            $$.args[1] = $5;
        }
    } |
    VCU108_CMD GPIO READ ALL_PORT {
        $$.command_code = COMMAND_VCU108_GPIO_READ;
        $$.args_len = 1;
        $$.args[0] = VCU108_PORT_ALL;
    } |
    VCU108_CMD GPIO SET NUMBER {
        if ($4 > 7) {
            invalidateCommand(&$$, ERROR_INVALID_GPIO_PIN);
        } else {
            $$.command_code = COMMAND_VCU108_GPIO_SET;
            $$.args_len = 1;
            $$.args[0] = $4;
        }
    } |
    VCU108_CMD GPIO CLEAR NUMBER {
        if ($4 > 7) {
            invalidateCommand(&$$, ERROR_INVALID_GPIO_PIN);
        } else {
            $$.command_code = COMMAND_VCU108_GPIO_CLEAR;
            $$.args_len = 1;
            $$.args[0] = $4;
        }
    } |
    VCU108_CMD GPIO TOGGLE NUMBER {
        if ($4 > 7) {
            invalidateCommand(&$$, ERROR_INVALID_GPIO_PIN);
        } else {
            $$.command_code = COMMAND_VCU108_GPIO_TOGGLE;
            $$.args_len = 1;
            $$.args[0] = $4;
        }
    } |
    VCU108_CMD GPIO DEBUG {
		$$.command_code = COMMAND_VCU108_GPIO_DEBUG;
		$$.args_len = 0;
	};


pek_command: 
    PEK_CMD IIC WRITE NUMBER NUMBER HEX HEX_STR {
        if ($4 > 3) {
            invalidateCommand(&$$, ERROR_INVALID_PEK_IIC_WRITE_PAGE);
        } else {
            int data_length = (strlen($7) + 1) / 3;
            if (data_length < (255 - $5)) {
                $$.command_code = COMMAND_PEK_IIC_WRITE;
                $$.args_len = 3;
                $$.args[0] = $4;
                $$.args[1] = $5;
                $$.args[2] = WRITE_DATA_TYPE_HEX;

                sprintf((char *)($$.args + $$.args_len), "%s", $7);
                $$.args_len += strlen($7) + 1;
            }
            else {
                invalidateCommand(&$$, ERROR_PEK_WRITE_DATA_TOO_LONG);
            }
        }
    } |
    PEK_CMD IIC WRITE NUMBER NUMBER CHAR STR {
        if ($4 > 3) {
            invalidateCommand(&$$, ERROR_INVALID_PEK_IIC_WRITE_PAGE);
        } else {
            int data_length = strlen($7);
            if (data_length < (255 - $5)) {
                $$.command_code = COMMAND_PEK_IIC_WRITE;
                $$.args_len = 3;
                $$.args[0] = $4;
                $$.args[1] = $5;
                $$.args[2] = WRITE_DATA_TYPE_CHAR;

                sprintf((char *)($$.args + $$.args_len), "%s", $7);
                $$.args_len += strlen($7) + 1;
            }
            else {
                invalidateCommand(&$$, ERROR_PEK_WRITE_DATA_TOO_LONG);
            }
        }
    } |
    PEK_CMD IIC READ NUMBER NUMBER NUMBER {
        if ($4 > 3) {
            invalidateCommand(&$$, ERROR_INVALID_PEK_IIC_READ_PAGE);
        } else if ($5 >= $6) {
            invalidateCommand(&$$, ERROR_START_ADDR_NOT_LESS_THAN_END);
        } else {
            $$.command_code = COMMAND_PEK_IIC_READ;
            $$.args_len = 3;
            $$.args[0] = $4;
            $$.args[1] = $5;
            $$.args[2] = $6;
        }
    } |
    PEK_CMD GPIO READ NUMBER NUMBER {
        unsigned char success = 0;
        switch ($4) {
            case 1:
                switch ($5) {
                    case 0:
                    case 1:
                    case 4:
                        success = 1;
                        break;
                }
                break;
            case 2:
                switch ($5) {
                    case 0:
                    case 1:
                    case 3:
                    case 4:
                    case 5:
                        success = 1;
                        break;
                }
                break;
            case 3:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                        success = 1;
                        break;
                }
                break;
            case 5:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        success = 1;
                        break;
                }
                break;
            case 6:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        success = 1;
                        break;
                }
                break;
            default:
                invalidateCommand(&$$, ERROR_INVALID_PEK_GPIO_PORT);
        }
        if (success) {
            $$.command_code = COMMAND_PEK_GPIO_READ;
            $$.args_len = 2;
            $$.args[0] = $4;
            $$.args[1] = $5;
        } else {
            invalidateCommand(&$$, ERROR_INVALID_PEK_GPIO_PIN);
        }
    } |
    PEK_CMD GPIO SET NUMBER NUMBER {
        unsigned char success = 0;
        switch ($4) {
            case 1:
                switch ($5) {
                    case 0:
                        success = 1;
                        break;
                }
                break;
            case 2:
                switch ($5) {
                    case 0:
                    case 1:
                    case 5:
                        success = 1;
                        break;
                }
                break;
            case 5:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        success = 1;
                        break;
                }
                break;
            case 6:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        success = 1;
                        break;
                }
                break;
            default:
                invalidateCommand(&$$, ERROR_INVALID_PEK_GPIO_PORT);
        }
        if (success) {
            $$.command_code = COMMAND_PEK_GPIO_SET;
            $$.args_len = 2;
            $$.args[0] = $4;
            $$.args[1] = $5;
        } else {
            invalidateCommand(&$$, ERROR_INVALID_PEK_GPIO_PIN);
        }
    } |
    PEK_CMD GPIO CLEAR NUMBER NUMBER {
        unsigned char success = 0;
        switch ($4) {
            case 1:
                switch ($5) {
                    case 0:
                        success = 1;
                        break;
                }
                break;
            case 2:
                switch ($5) {
                    case 0:
                    case 1:
                    case 5:
                        success = 1;
                        break;
                }
                break;
            case 5:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        success = 1;
                        break;
                }
                break;
            case 6:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        success = 1;
                        break;
                }
                break;
            default:
                invalidateCommand(&$$, ERROR_INVALID_PEK_GPIO_PORT);
        }
        if (success) {
            $$.command_code = COMMAND_PEK_GPIO_CLEAR;
            $$.args_len = 2;
            $$.args[0] = $4;
            $$.args[1] = $5;
        } else {
            invalidateCommand(&$$, ERROR_INVALID_PEK_GPIO_PIN);
        }
    } |
    PEK_CMD GPIO TOGGLE NUMBER NUMBER {
        unsigned char success = 0;
        switch ($4) {
            case 1:
                switch ($5) {
                    case 0:
                        success = 1;
                        break;
                }
                break;
            case 2:
                switch ($5) {
                    case 0:
                    case 1:
                    case 5:
                        success = 1;
                        break;
                }
                break;
            case 5:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        success = 1;
                        break;
                }
                break;
            case 6:
                switch ($5) {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        success = 1;
                        break;
                }
                break;
            default:
                invalidateCommand(&$$, ERROR_INVALID_PEK_GPIO_PORT);
        }
        if (success) {
            $$.command_code = COMMAND_PEK_GPIO_TOGGLE;
            $$.args_len = 2;
            $$.args[0] = $4;
            $$.args[1] = $5;
        } else {
            invalidateCommand(&$$, ERROR_INVALID_PEK_GPIO_PIN);
        }
    } |
    PEK_CMD BERT {
        $$.command_code = COMMAND_PEK_BERT;
        $$.args_len = 0;
    } |
    PEK_CMD EYESCAN {
        $$.command_code = COMMAND_PEK_EYESCAN;
        $$.args_len = 0;
    } |
    PEK_CMD GPIO DEBUG {
        $$.command_code = COMMAND_PEK_GPIO_DEBUG;
        $$.args_len = 0;
    } |
    PEK_CMD IIC DEBUG {
        $$.command_code = COMMAND_PEK_IIC_DEBUG;
        $$.args_len = 0;
    };

%%

/**
 * A helper function for invalidating a command.
 * 
 * @param val the command struct to populate
 * @param error_code the error code to populate the struct with so the user knows why the command failed
*/
void invalidateCommand(command *val, unsigned char error_code) {
    val->command_code = COMMAND_INVALID;
    val->args_len = 1;
    val->args[0] = error_code;
}

/**
 * I believe this function is for if there is no rule for the given set of tokens
 * The structure of an error command struct will be the the command code as COMMAND_INVALID,
 * the first argument being the error code defined in parse_codes.h, and the
 other
 arguments
 * being characters of the msg
 * 
 * @param val the command struct to populate
 * @param msg the error message since we defined parse.error verbose
*/
int yyerror(command *val, char const *msg) {
    val->command_code = COMMAND_INVALID;
    val->args_len = 1 + strlen(msg);
    val->args[0] = ERROR_OTHER;
    strncpy((char *)val->args + 1, msg, strlen(msg));
    val->args[strlen(msg) + 1] = '\0';
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
