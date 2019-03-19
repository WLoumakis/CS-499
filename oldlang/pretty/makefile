# Makefile for the Lexer, Lexeme, Types, Parser and Pretty modules.
# 
# Created by Walt Loumakis

OBJS = Lexer.o Lexeme.o Types.o Parser.o
OBJFLS = -Wall -Wextra -std=c99 -g -c
TARGS = pp
TARFL = -Wall -Wextra -std=c99 -g

# Makes all files
all: $(TARGS)

# Makes the pp executable
pp: $(OBJS) Pretty.c
	gcc $(TARFL) $(OBJS) Pretty.c -o pp

# Makes the Lexer object file
Lexer.o: Lexer.c Lexer.h
	gcc $(OBJFLS) Lexer.c

# Makes the Lexeme object file
Lexeme.o: Lexeme.c Lexeme.h
	gcc $(OBJFLS) Lexeme.c

# Makes the Types object file
Types.o: Types.c Types.h
	gcc $(OBJFLS) Types.c

# Makes the Parser object file
Parser.o: Parser.c Parser.h
	gcc $(OBJFLS) Parser.c

# Cleans the object files from the directory
clean:
	rm -f $(OBJS) $(TARGS)
