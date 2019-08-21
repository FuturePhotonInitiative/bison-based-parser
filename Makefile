# repl:repl.c y.tab_test.c lex.yy_test.c y.tab_test.h lex.yy_test.h
# 	gcc -DLOCAL -Wall -Wextra -Wpedantic -o repl repl.c y.tab_test.c lex.yy_test.c

smoke_test:smoke_test.c y.tab_test.c lex.yy_test.c y.tab_test.h lex.yy_test.h
	gcc -DLOCAL -Wall -Wextra -Wpedantic -o smoke_test smoke_test.c y.tab_test.c lex.yy_test.c

y.tab_test.c: parser.y
	bison --verbose --debug --defines=y.tab_test.h -o y.tab_test.c parser.y

lex.yy_test.c: commands.l y.tab_test.h
	flex --debug --header-file=lex.yy_test.h -o lex.yy_test.c commands.l


y.tab_test.h: parser.y
	bison --verbose --debug --defines=y.tab.h -o y.tab_test.c parser.y

lex.yy_test.h: commands.l y.tab_test.h
	flex --debug --header-file=lex.yy_test.h -o lex.yy_test.c commands.l

remote: y.tab.c lex.yy.c y.tab.h lex.yy.h parser.h parse_types.h
	zip transfer y.tab.c lex.yy.c y.tab.h lex.yy.h parser.h parse_types.h

y.tab.c: parser.y
	bison --defines=y.tab.h -o y.tab.c parser.y

lex.yy.c: commands.l y.tab.h
	flex --header-file=lex.yy.h commands.l

y.tab.h: parser.y
	bison --defines=y.tab.h -o y.tab.c parser.y

lex.yy.h: commands.l y.tab.h
	flex --header-file=lex.yy.h commands.l

clean:
	rm lex.yy.h y.tab.h lex.yy.c y.tab.c lex.yy_test.h y.tab_test.h lex.yy_test.c y.tab_test.c localbuild transfer.zip
