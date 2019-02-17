/**
 * Types module for labelling Lexical items.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_TYPES__
#define __MY_TYPES__

/**************************************************************************/
/*                              Bad Lexemes                               */
/**************************************************************************/

extern char *BAD_TYPE;

/**************************************************************************/
/*                                Literals                                */
/**************************************************************************/

extern char *TRUEVAL;
extern char *FALSEVAL;
extern char *STRING;
extern char *NUMBER;
extern char *ENDofINPUT;

/**************************************************************************/
/*                                Symbols                                 */
/**************************************************************************/

extern char *COLON;
extern char *OBRACKET;
extern char *CBRACKET;
extern char *COMMA;
extern char *OBRACE;
extern char *CBRACE;

/**************************************************************************/
/*                               Workspaces                               */
/**************************************************************************/

extern char *WORKSPACE;
extern char *NAME;
extern char *LANGUAGE;
extern char *LEARNING_OPT_OUT;
extern char *SYSTEM_SETTINGS;
extern char *TOOLING;
extern char *STORE_GENERIC_RESPONSES;
extern char *DISAMBIGUATION;
extern char *PROMPT;
extern char *NONE_OF_THE_ABOVE_PROMPT;
extern char *ENABLED;
extern char *SENSITIVITY;
extern char *HUMAN_AGENT_ASSIST;

/**************************************************************************/
/*                                Intents                                 */
/**************************************************************************/

extern char *INTENTS;
extern char *INTENT;
extern char *EXAMPLES;
extern char *MENTIONS;

/**************************************************************************/
/*                                Entities                                */
/**************************************************************************/

extern char *ENTITIES;
extern char *ENTITY;
extern char *VALUES;
extern char *VALUE;
extern char *SYNONYMS;
extern char *SYNONYM;
extern char *PATTERNS;
extern char *PATTERN;
extern char *VALUE_TYPE;

/**************************************************************************/
/*                             Decision Tree                              */
/**************************************************************************/

extern char *TREE;

/******************/
/*     Types      */
/******************/

extern char *TYPE;

/******************/
/*     Title      */
/******************/

extern char *TITLE;

/******************/
/*     Output     */
/******************/

extern char *OUTPUT;

///////////////////////
//       Text        //
///////////////////////

extern char *TEXT;
extern char *SELECTION_POLICY;
extern char *DELIMETER;

///////////////////////
//       Pause       //
///////////////////////

extern char *PAUSE;
extern char *TIME;
extern char *TYPING;

///////////////////////
//       Image       //
///////////////////////

extern char *IMAGE;
extern char *SOURCE;

///////////////////////
//      Option       //
///////////////////////

extern char *OPTION;
extern char *PREFERENCE;
extern char *LABEL;

///////////////////////
// Connect to Agent  //
///////////////////////

extern char *CONNECT_TO_AGENT;
extern char *MESSAGE_TO_HUMAN_AGENT;

/******************/
/*    Metadata    */
/******************/

extern char *METADATA;
extern char *FALLBACK;

/******************/
/*   Conditions   */
/******************/

extern char *CONDITIONS;

/******************/
/*  Dialog Node   */
/******************/

extern char *DIALOG_NODE;

/******************/
/*  Prev Sibling  */
/******************/

extern char *PREVIOUS_SIBLING;

/******************/
/*  Description   */
/******************/

extern char *DESCRIPTION;

/******************/
/*     Parent     */
/******************/

extern char *PARENT;

/******************/
/*    Context     */
/******************/

extern char *CONTEXT;

/******************/
/*   Next Step    */
/******************/

extern char *NEXT_STEP;
extern char *BEHAVIOR;
extern char *SELECTOR;

/******************/
/*    Actions     */
/******************/

extern char *ACTIONS;
extern char *ACTION_TYPE;
extern char *PARAMETERS;
extern char *RESULT_VARIABLE;
extern char *CREDENTIALS;

/******************/
/*   Event Name   */
/******************/

extern char *EVENT_NAME;

/******************/
/*    Variable    */
/******************/

extern char *VARIABLE;

/******************/
/*   Digress In   */
/******************/

extern char *DIGRESS_IN;

/******************/
/*  Digress Out   */
/******************/

extern char *DIGRESS_OUT;

/******************/
/* Dgrs Out Slots */
/******************/

extern char *DIGRESS_OUT_SLOTS;

/******************/
/*   User Label   */
/******************/

extern char *USER_LABEL;

#endif
