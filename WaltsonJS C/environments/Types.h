/**
 * Types module created for identifying
 * lexical items.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_TYPES__
#define __MY_TYPES__

/**************************************/
/*             Bad Types              */
/**************************************/

extern char *BAD_NUMBER;
extern char *UNKNOWN_CHAR;

/**************************************/
/*              Unaries               */
/**************************************/

extern char *ID;
extern char *TRUEVAL;
extern char *FALSEVAL;
extern char *INTEGER;
extern char *REAL;
extern char *STRING;
extern char *OBJECT;
extern char *ARRAY;
extern char *ENDofINPUT;

/**************************************/
/*              Symbols               */
/**************************************/

extern char *COMMA;
extern char *COLON;
extern char *MINUS;
extern char *OBRACE;
extern char *CBRACE;
extern char *OBRACKET;
extern char *CBRACKET;

/**************************************/
/*           Parsing Types            */
/**************************************/

extern char *PROGRAM;
extern char *ATTRLIST;
extern char *MIXINLIST;

/**************************************/
/*         Environment Types          */
/**************************************/

extern char *ENV;
extern char *TABLE;
extern char *IDLIST;
extern char *VALLIST;

#endif
