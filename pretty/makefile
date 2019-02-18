# Makefile for the Lexer, Lexeme, Types, Parser and Pretty modules.
# 
# Created by Walt Loumakis

OBJS = Lexer.o Lexeme.o Types.o Parser.o Pretty.o
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

# Makes the Parser object file
Parser.o: Parser.c Parser.h
	gcc $(OBJFLS) Parser.c

# Makes the Pretty object file
Pretty.o: Pretty.c Pretty.h
	gcc $(OBJFLS) Pretty.c

# Cleans the object files from the directory
clean:
	rm -f $(OBJS) $(TARGS)
