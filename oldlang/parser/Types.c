/**
 * Types module for labelling Lexical items.
 * 
 * Created by Walt Loumakis
 */

/**************************************************************************/
/*                              Bad Lexemes                               */
/**************************************************************************/

char *BAD_TYPE = "BAD_TYPE";
char *BAD_NUMBER = "BAD_NUMBER";
char *BAD_BOOL = "BAD_BOOL";

/**************************************************************************/
/*                                Literals                                */
/**************************************************************************/

char *TRUEVAL = "true";
char *FALSEVAL = "false";
char *STRING = "STRING";
char *NUMBER = "NUMBER";
char *ENDofINPUT = "ENDofINPUT";

/**************************************************************************/
/*                                Symbols                                 */
/**************************************************************************/

char *COLON = "COLON";
char *OBRACKET = "OBRACKET";
char *CBRACKET = "CBRACKET";
char *COMMA = "COMMA";
char *OBRACE = "OBRACE";
char *CBRACE = "CBRACE";

/**************************************************************************/
/*                               Workspaces                               */
/**************************************************************************/

char *WORKSPACE = "workspace";
char *NAME = "name";
char *LANGUAGE = "language";
char *COUNTEREXAMPLES = "counterexamples";
char *LEARNING_OPT_OUT = "learning_opt_out";
char *SYSTEM_SETTINGS = "system_settings";
char *TOOLING = "tooling";
char *STORE_GENERIC_RESPONSES = "store_generic_responses";
char *DISAMBIGUATION = "disambiguation";
char *PROMPT = "prompt";
char *NONE_OF_THE_ABOVE_PROMPT = "none_of_the_above_prompt";
char *ENABLED = "enabled";
char *SENSITIVITY = "sensitivity";
char *HUMAN_AGENT_ASSIST = "human_agent_assist";

/**************************************************************************/
/*                                Intents                                 */
/**************************************************************************/

char *INTENTS = "intents";
char *INTENT = "intent";
char *EXAMPLES = "examples";
char *MENTIONS = "mentions";

/**************************************************************************/
/*                                Entities                                */
/**************************************************************************/

char *ENTITIES = "entities";
char *ENTITY = "entity";
char *VALUES = "values";
char *VALUE = "value";
char *SYNONYMS = "synonyms";
char *SYNONYM = "synonym";
char *PATTERNS = "patterns";
char *PATTERN = "pattern";
char *VALUE_TYPE = "value_type";
char *FUZZY_MATCH = "fuzzy_match";

/**************************************************************************/
/*                             Decision Tree                              */
/**************************************************************************/

char *TREE = "tree";

/******************/
/*     Types      */
/******************/

char *TYPE = "type";

/******************/
/*     Title      */
/******************/

char *TITLE = "title";

/******************/
/*    Children    */
/******************/

char *CHILDREN = "children";

/******************/
/*     Output     */
/******************/

char *OUTPUT = "output";

///////////////////////
//       Text        //
///////////////////////

char *TEXT = "text";
char *SELECTION_POLICY = "selection_policy";
char *DELIMETER = "delimeter";

///////////////////////
//       Pause       //
///////////////////////

char *PAUSE = "pause";
char *TIME = "time";
char *TYPING = "typing";

///////////////////////
//       Image       //
///////////////////////

char *IMAGE = "image";
char *SOURCE = "source";

///////////////////////
//      Option       //
///////////////////////

char *OPTION = "option";
char *PREFERENCE = "preference";
char *LABEL = "label";

///////////////////////
// Connect to Agent  //
///////////////////////

char *CONNECT_TO_AGENT = "connect_to_agent";
char *MESSAGE_TO_HUMAN_AGENT = "message_to_human_agent";

/******************/
/*    Metadata    */
/******************/

char *METADATA = "metadata";
char *FALLBACK = "fallback";

/******************/
/*   Conditions   */
/******************/

char *CONDITIONS = "conditions";

/******************/
/*  Dialog Node   */
/******************/

char *DIALOG_NODE = "dialog_node";

/******************/
/*  Prev Sibling  */
/******************/

char *PREVIOUS_SIBLING = "previous_sibling";

/******************/
/*  Description   */
/******************/

char *DESCRIPTION = "description";

/******************/
/*     Parent     */
/******************/

char *PARENT = "parent";

/******************/
/*    Context     */
/******************/

char *CONTEXT = "context";

/******************/
/*   Next Step    */
/******************/

char *NEXT_STEP = "next_step";
char *BEHAVIOR = "behavior";
char *SELECTOR = "selector";

/******************/
/*    Actions     */
/******************/

char *ACTIONS = "actions";
char *ACTION_TYPE = "action_type";
char *PARAMETERS = "parameters";
char *RESULT_VARIABLE = "result_variable";
char *CREDENTIALS = "credentials";

/******************/
/*   Event Name   */
/******************/

char *EVENT_NAME = "event_name";

/******************/
/*    Variable    */
/******************/

char *VARIABLE = "variable";

/******************/
/*   Digress In   */
/******************/

char *DIGRESS_IN = "digress_in";

/******************/
/*  Digress Out   */
/******************/

char *DIGRESS_OUT = "digress_out";

/******************/
/* Dgrs Out Slots */
/******************/

char *DIGRESS_OUT_SLOTS = "digress_out_slots";

/******************/
/*   User Label   */
/******************/

char *USER_LABEL = "user_label";

/**************************************************************************/
/*                             Parsing Types                              */
/**************************************************************************/

char *WORKSPACES = "WORKSPACES";
char *WORKSPACE_OBJ = "WORKSPACE_OBJ";
char *WORKSPACE_ATTR = "WORKSPACE_ATTR";
char *INTENT_NAME = "INTENT_NAME";
char *USER_EXAMPLES = "USER_EXAMPLES";
char *EXAMPLE_ATTR = "EXAMPLE_ATTR";
char *MENTIONS_ATTR = "MENTIONS_ATTR";
char *ENTITY_ATTR = "ENTITY_ATTR";
char *ENTITY_NAME = "ENTITY_NAME";
char *ENTITY_VALS = "ENTITY_VALS";
char *ENTITY_VAL_ATTR = "ENTITY_VAL_ATTR";
char *ENTITY_VAL_NAME = "ENTITY_VAL_NAME";
char *ENTITY_VAL_TYPE = "ENTITY_VAL_TYPE";
char *NODE_LIST = "NODE_LIST";
char *NODE_ATTR = "NODE_ATTR";
char *OUTPUT_VALS = "OUTPUT_VALS";
char *TEXT_VAL_ATTR = "TEXT_VAL_ATTR";
char *STRING_LIST = "STRING_LIST";
char *PAUSE_ATTR = "PAUSE_ATTR";
char *IMAGE_ATTR = "IMAGE_ATTR";
char *OPTION_ATTR = "OPTION_ATTR";
char *OPTIONS_OBJS = "OPTIONS_OBJS";
char *OPTIONS_OBJ_ATTR = "OPTIONS_OBJ_ATTR";
char *METADATA_ATTR = "METADATA_ATTR";
char *CONTEXT_OBJ_ATTR = "CONTEXT_OBJ_ATTR";
char *NEXT_STEP_ATTR = "NEXT_STEP_ATTR";
char *ACTIONS_ATTR = "ACTIONS_ATTR";
char *PARAMETERS_ATTR = "PARAMETERS_ATTR";
