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
#include "Lexeme.h"
#include "Types.h"

#define null NULL

static void skipWhiteSpace();
static char readChar();

static Lexeme *lexString();
static Lexeme *lexKeyword();
static Lexeme *matchedKeyword(char *str);

FILE *fp;
int line;

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
		case '\"':
			ungetc(ch, fp);
			return lexString();
		default:
			ungetc(ch, fp);
			return lexKeyword();
	}
}

static Lexeme *lexString() {
	int ch = -1;
	int last = -1;
	int length = 64;
	char *buffer = (char *)malloc(sizeof(char) * length + 1);
	int index = 0;

	// This gets the first quote
	ch = readChar();
	buffer[index++] = ch;
	buffer[index] = 0;

	last = ch;
	ch = readChar();
	// Despite IBM currently stating that strings cannot have the \" character,
	// I am implementing a method that allows for \" characters for two reasons:
	//		1.	IBM may choose to implement allowing the \" character later on, and
	//		2.	Even if IBM does not choose to do so, the Watson methods called that take strings
	//			as input will return an error if there is a \" character anyway.
	while (!feof(fp) && (ch != '\"' || (ch == '\"' && last == '\\'))) {
		if (index == length) {
			length *= 2;
			buffer = realloc(buffer, sizeof(char) * length + 1);
		}
		buffer[index++] = ch;
        buffer[index] = 0;
        last = ch;
        ch = readChar(fp);
	}
	if (index == length) {
		++length;
        buffer = realloc(buffer, sizeof(char) * length + 1);
    }
	buffer[index++] = '\"';
    buffer[index] = 0;
	return newLexeme(STRING, UNDEFINED, UNDEFINED, buffer, line);
}

static Lexeme *lexKeyword() {
	int ch;
	int length = 64;
	char *buffer = (char *)malloc(sizeof(char) * length + 1);
	int index = 0;

	ch = readChar();
	while (!feof(fp) && (isalpha(ch) || ch == '_')) {
		if (index == length) {
			length *=2;
			buffer = realloc(buffer, sizeof(char) * length + 1);
		}
		buffer[index++] = ch;
		ch = readChar();
	}
	if (ch == '\n') line--;
	ungetc(ch, fp);
	return matchedKeyword(buffer);
}

static Lexeme *matchedKeyword(char *str) {
	// Literals
	if (strcmp(str, TRUEVAL) == 0) { free(str); return newLexeme(TRUEVAL, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, FALSEVAL) == 0) { free(str); return newLexeme(FALSEVAL, UNDEFINED, UNDEFINED, UNDEFINED, line); }

	// Workspaces
	else if (strcmp(str, WORKSPACE) == 0) { free(str); return newLexeme(WORKSPACE, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, NAME) == 0) { free(str); return newLexeme(NAME, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, LANGUAGE) == 0) { free(str); return newLexeme(LANGUAGE, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, LEARNING_OPT_OUT) == 0) { free(str); return newLexeme(LEARNING_OPT_OUT, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, SYSTEM_SETTINGS) == 0) { free(str); return newLexeme(SYSTEM_SETTINGS, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, TOOLING) == 0) { free(str); return newLexeme(TOOLING, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, STORE_GENERIC_RESPONSES) == 0) { free(str); return newLexeme(STORE_GENERIC_RESPONSES, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, PROMPT) == 0) { free(str); return newLexeme(PROMPT, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, NONE_OF_THE_ABOVE_PROMPT) == 0) { free(str); return newLexeme(NONE_OF_THE_ABOVE_PROMPT, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, ENABLED) == 0) { free(str); return newLexeme(ENABLED, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, SENSITIVITY) == 0) { free(str); return newLexeme(SENSITIVITY, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	
	// Intents
	else if (strcmp(str, INTENTS) == 0) { free(str); return newLexeme(INTENTS, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, INTENT) == 0) { free(str); return newLexeme(INTENT, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, EXAMPLES) == 0) { free(str); return newLexeme(EXAMPLES, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, MENTIONS) == 0) { free(str); return newLexeme(MENTIONS, UNDEFINED, UNDEFINED, UNDEFINED, line); }

	// Entities
	else if (strcmp(str, ENTITIES) == 0) { free(str); return newLexeme(ENTITIES, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, ENTITY) == 0) { free(str); return newLexeme(ENTITY, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, VALUES) == 0) { free(str); return newLexeme(VALUES, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, VALUE) == 0) { free(str); return newLexeme(VALUE, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, SYNONYMS) == 0) { free(str); return newLexeme(SYNONYMS, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, SYNONYM) == 0) { free(str); return newLexeme(SYNONYM, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, PATTERNS) == 0) { free(str); return newLexeme(PATTERNS, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, PATTERN) == 0) { free(str); return newLexeme(PATTERN, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, VALUE_TYPE) == 0) { free(str); return newLexeme(VALUE_TYPE, UNDEFINED, UNDEFINED, UNDEFINED, line); }

	// Decision Tree
	else if (strcmp(str, TREE) == 0) { free(str); return newLexeme(TREE, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Types
	else if (strcmp(str, TYPE) == 0) { free(str); return newLexeme(TYPE, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Title
	else if (strcmp(str, TITLE) == 0) { free(str); return newLexeme(TITLE, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Output
	else if (strcmp(str, OUTPUT) == 0) { free(str); return newLexeme(OUTPUT, UNDEFINED, UNDEFINED, UNDEFINED, line); }

			// Text
	else if (strcmp(str, TEXT) == 0) { free(str); return newLexeme(TEXT, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, SELECTION_POLICY) == 0) { free(str); return newLexeme(SELECTION_POLICY, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, DELIMETER) == 0) { free(str); return newLexeme(DELIMETER, UNDEFINED, UNDEFINED, UNDEFINED, line); }

			// Pause
	else if (strcmp(str, PAUSE) == 0) { free(str); return newLexeme(PAUSE, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, TIME) == 0) { free(str); return newLexeme(TIME, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, TYPING) == 0) { free(str); return newLexeme(TYPING, UNDEFINED, UNDEFINED, UNDEFINED, line); }

			// Image
	else if (strcmp(str, IMAGE) == 0) { free(str); return newLexeme(IMAGE, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, SOURCE) == 0) { free(str); return newLexeme(SOURCE, UNDEFINED, UNDEFINED, UNDEFINED, line); }

			// Option
	else if (strcmp(str, OPTION) == 0) { free(str); return newLexeme(OPTION, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, PREFERENCE) == 0) { free(str); return newLexeme(PREFERENCE, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, LABEL) == 0) { free(str); return newLexeme(LABEL, UNDEFINED, UNDEFINED, UNDEFINED, line); }

			// Connect to Agent
	else if (strcmp(str, CONNECT_TO_AGENT) == 0) { free(str); return newLexeme(CONNECT_TO_AGENT, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, MESSAGE_TO_HUMAN_AGENT) == 0) { free(str); return newLexeme(MESSAGE_TO_HUMAN_AGENT, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Metadata
	else if (strcmp(str, METADATA) == 0) { free(str); return newLexeme(METADATA, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, FALLBACK) == 0) { free(str); return newLexeme(FALLBACK, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Conditions
	else if (strcmp(str, CONDITIONS) == 0) { free(str); return newLexeme(CONDITIONS, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Dialog Node
	else if (strcmp(str, DIALOG_NODE) == 0) { free(str); return newLexeme(DIALOG_NODE, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Previous Sibling
	else if (strcmp(str, PREVIOUS_SIBLING) == 0) { free(str); return newLexeme(PREVIOUS_SIBLING, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Description
	else if (strcmp(str, DESCRIPTION) == 0) { free(str); return newLexeme(DESCRIPTION, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Parent
	else if (strcmp(str, PARENT) == 0) { free(str); return newLexeme(PARENT, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Context
	else if (strcmp(str, CONTEXT) == 0) { free(str); return newLexeme(CONTEXT, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Next Step
	else if (strcmp(str, NEXT_STEP) == 0) { free(str); return newLexeme(NEXT_STEP, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, BEHAVIOR) == 0) { free(str); return newLexeme(BEHAVIOR, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, SELECTOR) == 0) { free(str); return newLexeme(SELECTOR, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Actions
	else if (strcmp(str, ACTIONS) == 0) { free(str); return newLexeme(ACTIONS, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, ACTION_TYPE) == 0) { free(str); return newLexeme(ACTION_TYPE, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, PARAMETERS) == 0) { free(str); return newLexeme(PARAMETERS, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, RESULT_VARIABLE) == 0) { free(str); return newLexeme(RESULT_VARIABLE, UNDEFINED, UNDEFINED, UNDEFINED, line); }
	else if (strcmp(str, CREDENTIALS) == 0) { free(str); return newLexeme(CREDENTIALS, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Event Name
	else if (strcmp(str, EVENT_NAME) == 0) { free(str); return newLexeme(EVENT_NAME, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Variable
	else if (strcmp(str, VARIABLE) == 0) { free(str); return newLexeme(VARIABLE, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Digress In
	else if (strcmp(str, DIGRESS_IN) == 0) { free(str); return newLexeme(DIGRESS_IN, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Digress Out
	else if (strcmp(str, DIGRESS_OUT) == 0) { free(str); return newLexeme(DIGRESS_OUT, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// Digress Out Slots
	else if (strcmp(str, DIGRESS_OUT_SLOTS) == 0) { free(str); return newLexeme(DIGRESS_OUT_SLOTS, UNDEFINED, UNDEFINED, UNDEFINED, line); }

		// User Label
	else if (strcmp(str, USER_LABEL) == 0) { free(str); return newLexeme(USER_LABEL, UNDEFINED, UNDEFINED, UNDEFINED, line); }

	// Couldn't find the keyword, so its a bad type.
	else return newLexeme(BAD_TYPE, UNDEFINED, UNDEFINED, str, line);
}

static char readChar() {
	char ch = fgetc(fp);
	if (ch == '\n') ++line;
	return ch;
}

static void skipWhiteSpace() {
	char ch;

	while ((ch = readChar()) != EOF && isspace(ch));

	if (ch != EOF) ungetc(ch, fp);
}
