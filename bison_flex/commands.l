%option noyywrap 
%option nomain
%option nounput
%option noinput
%{
#include "parse_types.h"
#include "parse_codes.h"

#ifndef LOCAL
#include "parser.h"
#else
#include "parser_test.h"
#endif

#include <stdlib.h>
#include <math.h>

#undef YY_BUF_SIZE
#define YY_BUF_SIZE 512
#define min(x, y) (x < y)? x : y;
%}

%%

cfp     { return CFP_CMD; }

qsfp	{ return QSFP_CMD; }

pek	    { return PEK_CMD; }

vcu108  { return VCU108_CMD; }

all	    { return ALL_PORT; }

bert    { return BERT; }

clear   { return CLEAR; }

debug	{ return DEBUG; }

eyescan	{ return EYESCAN; }

gpio	{ return GPIO; }

mdio    { return MDIO; }

iic		{ return IIC; }

read	{ return READ; }

set	    { return SET; }

toggle	{ return TOGGLE; }

write	{ return WRITE; }

char    { return CHAR; }

hex     { return HEX; }

int	        { yylval.command_code = GPIO_PORT_INT; return GPIO_PORT; }

lpmode	    { yylval.command_code = GPIO_PORT_LPMODE; return GPIO_PORT; }

modprs	    { yylval.command_code = GPIO_PORT_MODPRS; return GPIO_PORT; }

modsel	    { yylval.command_code = GPIO_PORT_MODSEL; return GPIO_PORT; }

reset	    { yylval.command_code = GPIO_PORT_RESET; return GPIO_PORT; }

leds        { yylval.command_code = VCU108_PORT_LEDS; return VCU108_PORT; }

buttons     { yylval.command_code = VCU108_PORT_BUTTONS; return VCU108_PORT; }

switches    { yylval.command_code = VCU108_PORT_SWITCHES; return VCU108_PORT; }

prg_cntl1   { yylval.command_code = CFP_PORT_PRG_CNTL1; return CFP_PORT; }

prg_cntl2   { yylval.command_code = CFP_PORT_PRG_CNTL2; return CFP_PORT; }

prg_cntl3   { yylval.command_code = CFP_PORT_PRG_CNTL3; return CFP_PORT; }

tx_dis      { yylval.command_code = CFP_PORT_TX_DIS; return CFP_PORT; }

mod_lopwr   { yylval.command_code = CFP_PORT_MOD_LOPWR; return CFP_PORT; }

mod_rstn    { yylval.command_code = CFP_PORT_MOD_RSTN; return CFP_PORT; }

[0-9]+ { 
    long parsed_num = strtol(yytext, NULL, 10);
    if (parsed_num >= 0 && parsed_num < 256) {
        yylval.num = parsed_num;
        return NUMBER;
    }
}

\"[0-9a-fA-F]{2}([ ][0-9a-fA-F]{2})*\" {
    strncpy(yylval.str, yytext + 1, strlen(yytext) - 2);
    yylval.str[strlen(yytext) - 2] = '\0';
    return HEX_STR;
}

\"[^\"]+\" {
    strncpy(yylval.str, yytext + 1, strlen(yytext) - 2);
    yylval.str[strlen(yytext) - 2] = '\0';
    return STR;
}

0x[0-9a-fA-F]{4} {
    strncpy(yylval.str, yytext, 6);
    return TWO_BYTES;
}

[ \r\t] ;/*Ignore whitespace

this rule is not really useful. Also, it may be error prone.
[^ \t\n]* {
    strncpy(yylval.str, yytext, MAX_ARGS);
    yylval.str[MAX_ARGS-1] = '\0';
    return STR;
}
*/

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
