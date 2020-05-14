param(
    [switch]$d=$false
)

if(-not $d)
{
    C:\Program` Files` `(x86`)\BisonFlex\win_flex.exe -L --header-file=lexer.h -o lexer.c commands.l
    C:\Program` Files` `(x86`)\BisonFlex\win_bison.exe --no-lines --defines=parser.h -o parser.c parser.y
}
else
{
    C:\Program` Files` `(x86`)\BisonFlex\win_flex.exe --debug --header-file=lexer_test.h -o lexer_test.c commands.l
    C:\Program` Files` `(x86`)\BisonFlex\win_bison.exe --no-lines --verbose --debug --defines=parser_test.h -o parser_test.c parser.y
}
