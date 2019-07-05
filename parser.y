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

%token PETB_CMD 
%token PEK_CMD 
%token DAC_CMD 
%token ADC_CMD 
%token I2CREAD_CMD 
%token I2CWRITE_CMD 
%token SPILB_CMD 
%token SPIXFER_CMD 
%token DEBUG_CMD
%token GPIO 
%token <command_code> GPIO_SUB 
%token <command_code> SUB_CMD 
%token <command_code> ADDR 
%token <command_code> PETB_PIN 
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
%type <cmd> petb_command
%type <cmd> pek_command
%type <cmd> adc_command
%type <cmd> dac_command
%type <cmd> spixfer_command
%type <cmd> spilb_command
%type <cmd> i2cwrite_command
%type <cmd> i2cread_command
%type <cmd> debug_command

%union{
    char command_code;
    char num;
    char str [MAX_ARGS];
    command cmd;
}
%%

command: petb_command { *command_val = $1; }|
         pek_command { *command_val = $1; }|
         adc_command { *command_val = $1; }|
         dac_command { *command_val = $1; }|
         spixfer_command { *command_val = $1; }|
         spilb_command { *command_val = $1; }|
         i2cwrite_command { *command_val = $1; }|
         i2cread_command { *command_val = $1; } |
		 debug_command { *command_val = $1; }
         ;

petb_command: PETB_CMD GPIO READ NUMBER PETB_PIN
            {
                $$.command_code = COMMAND_PETB_GPIO_READ;
                $$.args_len = 2;
                $$.args[0] = $4;
                $$.args[1] = $5;
                if ($4 != 0)
                {
                        $$.command_code = COMMAND_INVALID;
                        $$.args_len = 1;
                        $$.args[0] = ERROR_INVALID_PETB_PORT;
                }
            } |
              PETB_CMD GPIO SET NUMBER PETB_PIN
            {
                $$.command_code = COMMAND_PETB_GPIO_SET;
                $$.args_len = 2;
                $$.args[0] = $4;
                $$.args[1] = $5;
                if ($4 != 0)
                {
                        $$.command_code = COMMAND_INVALID;
                        $$.args_len = 1;
                        $$.args[0] = ERROR_INVALID_PETB_PORT;
                }
            } |
              PETB_CMD GPIO CLEAR NUMBER PETB_PIN
            {
                $$.command_code = COMMAND_PETB_GPIO_CLEAR;
                $$.args_len = 2;
                $$.args[0] = $4;
                $$.args[1] = $5;
                if ($4 != 0)
                {
                        $$.command_code = COMMAND_INVALID;
                        $$.args_len = 1;
                        $$.args[0] = ERROR_INVALID_PETB_PORT;
                }
            } |
              PETB_CMD GPIO TOGGLE NUMBER PETB_PIN
            {
                $$.command_code = COMMAND_PETB_GPIO_TOGGLE;
                $$.args_len = 2;
                $$.args[0] = $4;
                $$.args[1] = $5;
                if ($4 != 0)
                {
                        $$.command_code = COMMAND_INVALID;
                        $$.args_len = 1;
                        $$.args[0] = ERROR_INVALID_PETB_PORT;
                }
            } |
			/*petb eyescan vrange prescale vmax hmax drp stepsize */
			PETB_CMD EYESCAN NUMBER NUMBER NUMBER STR NUMBER NUMBER
			{
				$$.command_code = COMMAND_PETB_EYESCAN;
				$$.args_len = 6;
				/* vrange */
				if( $3 < 0 || $3 > 3 )
				{
					$$.command_code = COMMAND_INVALID;
					$$.args_len = 1;
					$$.args[0] = ERROR_INVALID_EYESCAN_VRANGE;
				}
				else
				{
					((unsigned short *)($$.args))[0] = $3;
				}
				/* prescale */
				if( $4 < 0 || $4 > 29 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_PRESCALE;
				}
				else
				{
					((unsigned short *)($$.args))[1] = $4;
				}
				/* vmax */
				if( $5 < 64 || $5 > 127 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_VMAX;
				}
				else
				{
					((unsigned short *)($$.args))[2] = $5;
				}
				/* hmax */
				errno = 0;
				unsigned short tmp = strtol($6, NULL, 0);
				if(errno != 0 || tmp < 256 || tmp > 512 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_HMAX;
				}
				else
				{
					((unsigned short *)($$.args))[3] = tmp;
				}
				/* drp */
				if( $7 < 0 || $7 > 3 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_DRP;
				}
				else
				{
					((unsigned short *)($$.args))[4] = $7;
				}
				/* stepsize */
				if( $8 != 1 && $8 != 2 && $8 != 4 && $8 != 8 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_STEPSIZE;
				}
				else
				{
					((unsigned short *)($$.args))[5] = $8;
				}
				
			} |
			/*petb eyescan vrange prescale vmax hmax drp stepsize */
			PETB_CMD EYESCAN NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER
			{
				$$.command_code = COMMAND_PETB_EYESCAN;
				$$.args_len = 6;
				/* vrange */
				if( $3 < 0 || $3 > 3 )
				{
					$$.command_code = COMMAND_INVALID;
					$$.args_len = 1;
					$$.args[0] = ERROR_INVALID_EYESCAN_VRANGE;
				}
				else
				{
					((unsigned short *)($$.args))[0] = $3;
				}
				/* prescale */
				if( $4 < 0 || $4 > 29 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_PRESCALE;
				}
				else
				{
					((unsigned short *)($$.args))[1] = $4;
				}
				/* vmax */
				if( $5 < 64 || $5 > 127 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_VMAX;
				}
				else
				{
					((unsigned short *)($$.args))[2] = $5;
				}
				/* hmax */
				if((unsigned char)($6) < 1)
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_HMAX;
				}
				else
				{
					((unsigned short *)($$.args))[3] = (unsigned char)($6);
				}
				/* drp */
				if( $7 < 0 || $7 > 3 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_DRP;
				}
				else
				{
					((unsigned short *)($$.args))[4] = $7;
				}
				/* stepsize */
				if( $8 != 1 && $8 != 2 && $8 != 4 && $8 != 8 )
				{
					if( $$.command_code != COMMAND_INVALID )
					{
						$$.command_code = COMMAND_INVALID;
						$$.args_len = 0;
					}
					$$.args[$$.args_len++] = ERROR_INVALID_EYESCAN_STEPSIZE;
				}
				else
				{
					((unsigned short *)($$.args))[5] = $8;
				}
				
			} |
			PETB_CMD BERT NUMBER
			{
				$$.command_code = COMMAND_PETB_BERT;
				$$.args_len = 1;
				if($3 < 0 || $3 > 7)
				{
					$$.command_code = COMMAND_INVALID;
					$$.args[0] = ERROR_INVALID_BERT_PATTERN;
				}
				else
				{
					$$.args[0] = $3;
				}
			}
            ;

pek_command: PEK_CMD GPIO READ NUMBER NUMBER
           {
                $$.command_code = COMMAND_PEK_GPIO_READ;
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
           } |
             PEK_CMD GPIO WRITE NUMBER NUMBER NUMBER
           {
                $$.command_code = COMMAND_PEK_GPIO_TOGGLE;
                $$.args_len = 3;
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
                if($6 != 0 && $6 != 1)
                {
                        $$.command_code = COMMAND_INVALID;
                        if($$.args_len != 3)
                        {
                                $$.args_len = $$.args_len + 1;
                                $$.args[$$.args_len - 1] = ERROR_INVALID_GPIO_VALUE;
                        }
                        else
                        {
                                $$.args_len = 1;
                                $$.args[0] = ERROR_INVALID_GPIO_VALUE;
                        }
                }
                else
                {
                        $$.args[2] = $6;
                }
           } |
             PEK_CMD GPIO LIST
           {
                $$.command_code = COMMAND_PEK_GPIO_LIST;
                $$.args_len = 0;
           }
        ;

adc_command: ADC_CMD READ NUMBER
        {
                /* TODO make sure these are all hex */
                /* Validate that the user has provided the correct channel */
                if($3 != 0 && $3 != 0x0e && $3 != 0x10 && $3 != 0x13 && $3 != 0x16 && $3 != 0x17)
                {
                        $$.command_code = COMMAND_INVALID;
                }
                else
                {
                        $$.command_code = COMMAND_ADC_READ;
                        $$.args_len = 1;
                        $$.args[0] = $3;
                        $$.args[1] = -1;
                }
        }
        ;

dac_command: DAC_CMD WRITE 
        {/* TODO get the specs for the write arguments */
                $$.command_code = COMMAND_DAC_WRITE;
        }
        ;

/* TODO write a lex rule for str, and remember the rules about length matching */
spixfer_command: SPIXFER_CMD STR
        {
                $$.command_code = COMMAND_SPIXFER;
                int i = 0;
                while($2[i] != '\0' && i < MAX_ARGS - 1)
                {
                        $$.args[i] = $2[i];
                        i++;
                }
                $$.args[i] = '\0';
                $$.args_len = strlen($$.args);
        } |
        SPIXFER_CMD NUMBER
        {
                $$.command_code = COMMAND_SPIXFER;
                int i = 0;
                char tmp[MAX_ARGS - 1];
                /*TODO this will cause strings originally entered as hex to be transfered as dec*/
                sprintf(tmp, "%d", $2);
                while(tmp[i] != '\0' && i < MAX_ARGS - 1)
                {
                        $$.args[i] = tmp[i];
                        i++;
                }
                $$.args[i] = '\0';
                $$.args_len = strlen($$.args);
        }
        ;

spilb_command: SPILB_CMD STR
        {
                $$.command_code = COMMAND_SPILB;
                int i = 0;
                while($2[i] != '\0')
                {
                        $$.args[i] = $2[i];
                        i++;
                }
                $$.args[i] = '\0';
                $$.args_len = strlen($$.args);
        } |
        SPILB_CMD NUMBER
        {
                $$.command_code = COMMAND_SPILB;
                int i = 0;
                char tmp[MAX_ARGS - 1];
                /*TODO this will cause strings originally entered as hex to be transfered as dec*/
                sprintf(tmp, "%d", $2);
                while(tmp[i] != '\0' && i < MAX_ARGS - 1)
                {
                        $$.args[i] = tmp[i];
                        i++;
                }
                $$.args[i] = '\0';
                $$.args_len = strlen($$.args);
        }
        ;

/* TODO */
i2cwrite_command: I2CWRITE_CMD NUMBER NUMBER
        {
                $$.command_code = COMMAND_I2CWRITE;
                /* Don't check the addresses because NUMBER is a byte, and the valid address space is the full range of byte */
                $$.args_len = 2;
                $$.args[0] = $2;
                $$.args[1] = $3;
        }
        ;

/* TODO */
i2cread_command: I2CREAD_CMD NUMBER NUMBER
        {
                $$.command_code = COMMAND_I2CREAD;
                /* Don't check the addresses because NUMBER is a byte, and the valid address space is the full range of byte */
                $$.args_len = 2;
                $$.args[0] = $2;
                $$.args[1] = $3;
        }
        ;

debug_command: DEBUG_CMD PETB_CMD IIC
		{
			$$.command_code = COMMAND_DEBUG_IIC;
			$$.args_len = 0;
			$$.args[0] = 0;
		} |
		DEBUG_CMD PETB_CMD GPIO
		{
			$$.command_code = COMMAND_DEBUG_GPIO;
			$$.args_len = 0;
			$$.args[0] = 0;
		}
		;

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

