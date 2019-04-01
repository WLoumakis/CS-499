/**
 * Lexer module created to read through a file
 * and return lexical items as it comes across them.
 * 
 * Created by Walt Loumakis
 */

#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Lexer.h"
#include "Types.h"
#include "Lexeme.h"

#define null NULL

/**************************************/
/*     Private Method Prototypes      */
/**************************************/

static Lexeme *lexString();
static Lexeme *lexNumber();
static Lexeme *lexVar();

static char readChar();
static void putBack(char ch);
static void skipWhiteSpace();

/**************************************/
/*              Globals               */
/**************************************/

FILE *fp;
int line;

/**************************************/
/*           Public Methods           */
/**************************************/

void initLexer(char *filename) {
	fp = fopen(filename, "r");
	assert(fp != null);
	line = 1;
}

void closeLexer() {
	fclose(fp);
}

Lexeme *lex() {
	skipWhiteSpace();

	char ch = readChar();

	if (ch == EOF) return newLexeme(ENDofINPUT, UNDEFINED, UNDEFINED, UNDEFINED, line);

	switch(ch) {
		case ':': return newLexeme(COLON, UNDEFINED, UNDEFINED, UNDEFINED, line);
		case '[': return newLexeme(OBRACKET, UNDEFINED, UNDEFINED, UNDEFINED, line);
		case ']': return newLexeme(CBRACKET, UNDEFINED, UNDEFINED, UNDEFINED, line);
		case ',': return newLexeme(COMMA, UNDEFINED, UNDEFINED, UNDEFINED, line);
		case '{': return newLexeme(OBRACE, UNDEFINED, UNDEFINED, UNDEFINED, line);
		case '}': return newLexeme(CBRACE, UNDEFINED, UNDEFINED, UNDEFINED, line);
		case '-': return newLexeme(MINUS, UNDEFINED, UNDEFINED, UNDEFINED, line);
		case '\"':
			putBack(ch);
			return lexString();
		default:
			if (isdigit(ch) || ch == '.') {
				putBack(ch);
				return lexNumber();
			}
			else if (isalpha(ch) || ch == '_') {
				putBack(ch);
				return lexVar();
			}
			else {
				char *temp = (char *)malloc(sizeof(char) + 1);
				temp[0] = ch;
				temp[1] = 0;
				return newLexeme(UNKNOWN_CHAR, UNDEFINED, UNDEFINED, temp, line);
			}
	}
}

/**************************************/
/*          Private Methods           */
/**************************************/

static Lexeme *lexString() {
	int ch = -1;
	int last = -1;
	int length = 64;
	char *buffer = (char *)malloc(sizeof(char) * length + 1);
	int index = 0;

	// This gets the first quote
	ch = readChar();
	
	// Despite IBM currently stating that strings cannot have the \" character,
	// I am implementing a method that allows for \" characters for two reasons:
	//		1.	IBM may choose to implement allowing the \" character later on, and
	//		2.	Even if IBM does not choose to do so, the Watson methods called that take strings
	//			as input will return an error if there is a \" character anyway.
	last = ch;
	while ((ch = readChar()) != EOF && (ch != '\"' || (ch == '\"' && last == '\\'))) {
		if (index == length) {
			length *= 2;
			buffer = (char *)realloc(buffer, sizeof(char) * length + 1);
		}
		buffer[index++] = ch;
        buffer[index] = 0;
        last = ch;
	}
	return newLexeme(STRING, UNDEFINED, UNDEFINED, buffer, line);
}

static Lexeme *lexNumber() {
	int ch;
	int length = 32;
	int index = 0;
	int real = 0;
	char *buffer = (char *)malloc(sizeof(char) * length + 1);
	while ((ch = readChar()) != EOF && (isdigit(ch) || ch == '.')) {
		if (index == length)
			return newLexeme(BAD_NUMBER, UNDEFINED, UNDEFINED, UNDEFINED, line);
		if (ch == '.' && real == 1)
			return newLexeme(BAD_NUMBER, UNDEFINED, UNDEFINED, UNDEFINED, line);
		if (ch == '.')
			real = 1;
		buffer[index++] = ch;
		buffer[index] = 0;
	}
	putBack(ch);
	if (real) return newLexeme(REAL, UNDEFINED, atof(buffer), UNDEFINED, line);
	return newLexeme(INTEGER, atoi(buffer), UNDEFINED, UNDEFINED, line);
}

static Lexeme *lexVar() {
	int ch;
	int length = 64;
	char *buffer = (char *)malloc(sizeof(char) * length + 1);
	int index = 0;

	while ((ch = readChar()) != EOF && (isalpha(ch) || ch == '_')) {
		if (index == length) {
			length *= 2;
			buffer = (char *)realloc(buffer, sizeof(char) * length + 1);
		}
		buffer[index++] = ch;
		buffer[index] = 0;
	}
	putBack(ch);
	return newLexeme(ID, UNDEFINED, UNDEFINED, buffer, line);
}

static char readChar() {
	char ch = fgetc(fp);
	if (ch == '\n') ++line;
	return ch;
}

static void putBack(char ch) {
	if (ch == '\n') --line;
	ungetc(ch, fp);
}

static void skipWhiteSpace() {
	char ch;

	while ((ch = readChar()) != EOF && isspace(ch));

	putBack(ch);
}
