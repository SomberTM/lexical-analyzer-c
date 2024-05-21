gcc -g -o lexer-debug lexer.c analyzer.c main.c
gdb ./lexer-debug
rm -rf ./lexer-debug
