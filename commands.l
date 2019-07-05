%option noyywrap 
%option nomain
%{
#include "parse_types.h"
#include "parser.h"

#ifndef LOCAL
#include "y.tab.h"
#else
#include "y.tab_test.h"
#endif

#include <stdlib.h>
#include <math.h>

#undef YY_BUF_SIZE
#define YY_BUF_SIZE 512
#define min(x, y) (x < y)? x : y;
%}

%%

ad	|
adc	 { yylval.command_code = 1; return ADC_CMD; }

al	|
all	 { yylval.command_code = ALL_PIN; return PETB_PIN; }

b    |
be   |
ber  |
bert { return BERT; }

c	|
cl	|
cle	|
clea	|
clear	 { yylval.command_code = GPIO_CLEAR; return CLEAR; }

d
da	|
dac	 { yylval.command_code = 2; return DAC_CMD; }

debug	{ return DEBUG_CMD; }

e		|
ey		|
eye		|
eyes	|
eyesc	|
eyesca	|
eyescan	{ return EYESCAN; }

g	|
gp	|
gpi	|
gpio	 { return GPIO; }

ii	|
iic		{ return IIC; }

i2cr	|
i2cre	|
i2crea	|
i2cread	 { yylval.command_code = 3; return I2CREAD_CMD; }

i2cw	|
i2cwr	|
i2cwri	|
i2cwrit	|
i2cwrite	 { yylval.command_code = 4; return I2CWRITE_CMD; }

in	|
int	 { yylval.command_code = INT_PIN; return PETB_PIN; }

li	|
lis	|
list	 { yylval.command_code = GPIO_LIST; return LIST; }

lp	|
lpm	|
lpmo	|
lpmod	|
lpmode	 { yylval.command_code = LPMODE_PIN; return PETB_PIN; }

modp	|
modpr	|
modprs	 { yylval.command_code = MODPRS_PIN; return PETB_PIN; }

mods	|
modse	|
modsel	 { yylval.command_code = MODSEL_PIN; return PETB_PIN; }

pek	 { yylval.command_code = 5; return PEK_CMD; }

pet	|
petb	 { yylval.command_code = 6; return PETB_CMD; }

rea	|
read	 { yylval.command_code = GPIO_READ; return READ; }

res	|
rese	|
reset	 { yylval.command_code = RESET_PIN; return PETB_PIN; }

se	|
set	 { yylval.command_code = GPIO_SET; return SET; }

spil	|
spilb	 { yylval.command_code = 7; return SPILB_CMD; }

spix	|
spixf	|
spixfe	|
spixfer	 { yylval.command_code = 8; return SPIXFER_CMD; }

t	|
to	|
tog	|
togg	|
toggl	|
toggle	 { yylval.command_code = GPIO_TOGGLE; return TOGGLE; }

w	|
wr	|
wri	|
writ	|
write	 { yylval.command_code = GPIO_WRITE; return WRITE; }

(0x)?[0-9a-fA-F]*       { 
                        long tmp = strtol(yytext, NULL, 0);
                        if(tmp >= 0 && tmp < 256)
                        {
                                yylval.num = tmp;
                                return NUMBER;
                        }
                        strncpy(yylval.str, yytext, MAX_ARGS);
                        yylval.str[MAX_ARGS-1] = '\0';
                        return STR; 
                        } 
[ \r\t] ;/*Ignore whitespace*/

[^ \t\n]* { strncpy(yylval.str, yytext, MAX_ARGS); yylval.str[MAX_ARGS-1] = '\0'; return STR; }

\n {return 0; /*EOF*/}

%%

YY_BUFFER_STATE currentString;

void _setupLexInput(char* inputBuffer)
{
        yy_delete_buffer(YY_CURRENT_BUFFER);
        currentString = yy_scan_string(inputBuffer);
        BEGIN(INITIAL);
}

void _cleanupLex()
{
        yy_delete_buffer(currentString);
}

