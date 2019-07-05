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

al	    |
all	    { return ALL_PORT; }

b       |
be      |
ber     |
bert    { return BERT; }

c	    |
cl     	|
cle	    |
clea	|
clear   { return CLEAR; }

debug	{ return DEBUG; }

e		|
ey		|
eye		|
eyes	|
eyesc	|
eyesca	|
eyescan	{ return EYESCAN; }

g	    |
gp  	|
gpi	    |
gpio	{ return GPIO; }

ii	    |
iic		{ return IIC; }

in	    |
int	    { yylval.command_code = GPIO_PORT_INT; return GPIO_PORT; }

lp	    |
lpm	    |
lpmo	|
lpmod	|
lpmode	{ yylval.command_code = GPIO_PORT_LPMODE; return GPIO_PORT; }

modp	|
modpr	|
modprs	{ yylval.command_code = GPIO_PORT_MODPRS; return GPIO_PORT; }

mods	|
modse	|
modsel	{ yylval.command_code = GPIO_PORT_MODSEL; return GPIO_PORT; }

pek	    { yylval.command_code = 5; return PEK_CMD; }

qsfp	{ yylval.command_code = 6; return QSFP_CMD; }

rea	    |
read	{ return READ; }

res	    |
rese	|
reset	{ yylval.command_code = GPIO_PORT_RESET; return GPIO_PORT; }

se	    |
set	    { return SET; }

t	    |
to	    |
tog	    |
togg	|
toggl	|
toggle	{ return TOGGLE; }

w	    |
wr	    |
wri	    |
writ	|
write	{ return WRITE; }

cfp     { yylval.command_code = 9; return CFP_CMD; }

vcu108  { yylval.command_code = 10; return VCU108_CMD; }

leds    { yylval.command_code = VCU108_PORT_LEDS; return VCU108_PORT; }

buttons { yylval.command_code = VCU108_PORT_BUTTONS; return VCU108_PORT; }

switches    { yylval.command_code = VCU108_PORT_SWITCHES; return VCU108_PORT; }

prg_cntl1   { yylval.command_code = CFP_PORT_PRG_CNTL1; return CFP_PORT; }

prg_cntl2   { yylval.command_code = CFP_PORT_PRG_CNTL2; return CFP_PORT; }

prg_cntl3   { yylval.command_code = CFP_PORT_PRG_CNTL3; return CFP_PORT; }

tx_dis  { yylval.command_code = CFP_PORT_TX_DIS; return CFP_PORT; }

mod_lopwr   { yylval.command_code = CFP_PORT_MOD_LOPWR; return CFP_PORT; }

mod_rstn    { yylval.command_code = CFP_PORT_MOD_RSTN; return CFP_PORT; }

hex     { return HEX; }

char    { return CHAR; }

(0x)?[0-9a-fA-F]* { 
    long tmp = strtol(yytext, NULL, 0);
    if (tmp >= 0 && tmp < 256) {
        yylval.num = tmp;
        return NUMBER;
    }
}

[\'\"][^\'\"]+[\'\"] {
    strncpy(yylval.str, yytext, MAX_ARGS);
    yylval.str[MAX_ARGS - 1] = '\0';
    return STR;
}

[ \r\t] ;/*Ignore whitespace*/

[^ \t\n]* { strncpy(yylval.str, yytext, MAX_ARGS); yylval.str[MAX_ARGS-1] = '\0'; return STR; }

\n {return 0; /*EOF*/}

%%

YY_BUFFER_STATE currentString;

void _setupLexInput(char* inputBuffer) {
        yy_delete_buffer(YY_CURRENT_BUFFER);
        currentString = yy_scan_string(inputBuffer);
        BEGIN(INITIAL);
}

void _cleanupLex() {
        yy_delete_buffer(currentString);
}

