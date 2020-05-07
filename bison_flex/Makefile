repl:../repl.c parser_test.c lexer_test.c parser_test.h lexer_test.h
	gcc -DLOCAL -Wall -Wextra -Wpedantic -o repl repl.c parser_test.c lexer_test.c

# smoke_test:smoke_test.c parser_test.c lexer_test.c parser_test.h lexer_test.h
# 	gcc -DLOCAL -Wall -Wextra -Wpedantic -o smoke_test smoke_test.c parser_test.c lexer_test.c

parser_test.c: parser.y
	bison --verbose --debug --defines=parser_test.h -o parser_test.c parser.y

lexer_test.c: commands.l parser_test.h
	flex --debug --header-file=lexer_test.h -o lexer_test.c commands.l


parser_test.h: parser.y
	bison --verbose --debug --defines=parser.h -o parser_test.c parser.y

lexer_test.h: commands.l parser_test.h
	flex --debug --header-file=lexer_test.h -o lexer_test.c commands.l

remote: parser.c lexer.c parse_codes.h lexer.h parse_codes.h parse_types.h
	zip transfer parser.c lexer.c parser.h lexer.h parser.h parse_types.h

parser.c: parser.y
	bison --defines=parser.h -o parser.c parser.y

lexer.c: commands.l parse_codes.h
	flex --header-file=lexer.h commands.l

parser.h: parser.y
	bison --defines=parser.h -o parser.c parser.y

lexer.h: commands.l parse_codes.h
	flex --header-file=lexer.h commands.l

clean:
	rm lexer.h parser.h lexer.c parser.c lexer_test.h parser_test.h lexer_test.c parser_test.c localbuild transfer.zip
