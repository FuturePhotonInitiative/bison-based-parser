param(
    [switch]$d=$false
)

if(-not $d)
{
    C:\Program` Files` `(x86`)\BisonFlex\win_flex.exe -L --header-file=lex.yy.h commands.l
    C:\Program` Files` `(x86`)\BisonFlex\win_bison --no-lines --defines=y.tab.h -o y.tab.c parser.y
}
else
{
    C:\Program` Files` `(x86`)\BisonFlex\win_flex.exe --debug --header-file=lex.yy.h -o lex.yy.c commands.l
    C:\Program` Files` `(x86`)\BisonFlex\win_bison.exe --no-lines --verbose --debug --defines=y.tab.h -o y.tab.c parser.y
}
