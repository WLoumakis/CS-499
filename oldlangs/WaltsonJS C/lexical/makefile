# Makefile for the Lexer, Lexeme, and Types modules.
# 
# Created by Walt Loumakis

OBJS = Lexer.o Lexeme.o Types.o
OBJFLS = -Wall -Wextra -std=c99 -g -c
TARGS = tester
TARFL = -Wall -Wextra -std=c99 -g

# Makes all files
all: $(TARGS)

# Makes the tester executable
tester: $(OBJS) Tester.c
	gcc $(TARFL) $(OBJS) Tester.c -o tester

# Makes the Lexer object file
Lexer.o: Lexer.c Lexer.h
	gcc $(OBJFLS) Lexer.c

# Makes the Lexeme object file
Lexeme.o: Lexeme.c Lexeme.h
	gcc $(OBJFLS) Lexeme.c

# Makes the Types object file
Types.o: Types.c Types.h
	gcc $(OBJFLS) Types.c

# Cleans the object files from the directory
clean:
	rm -f $(OBJS) $(TARGS)
