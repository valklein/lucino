/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 1 "query_parser.y"

#include "lucene.h"
#include "lcn_search.h"


/* autogenerated funktions, do not touch */
void
lcn_query_parser_(void*, int, lcn_query_token_t *, lcn_query_info_t* info );

void *
lcn_query_parser_Alloc(void *(*malloc_proc)(size_t));

void
lcn_query_parser_Trace( FILE *stream, char *z);

void lcn_query_parser_Free(
    void *p,                    /* The parser to be deleted        */
    void (*free_proc)(void*)    /* Function used to reclaim memory */
);

/* end of autogenerated functions */


void
lcn_query_parser_parse( void* p,
                        int token_id,
                        lcn_query_token_t *t,
                        lcn_query_info_t* info )
{
    lcn_query_parser_( p, token_id, t, info );
}


void *
lcn_query_parser_alloc(void *(*malloc_proc)(size_t))
{
    return lcn_query_parser_Alloc( malloc_proc );
}

void
lcn_query_parser_trace( FILE *stream, char *z)
{
#ifndef NDEBUG
    lcn_query_parser_Trace( stream, z);
#endif
}

void lcn_query_parser_free( void *p, void (*free_proc)(void*))
{
    lcn_query_parser_Free( p, free_proc );
}

struct lcn_query_with_clause_t
{
    lcn_query_t *query;
    int clause;
};

static lcn_query_t*
query_list_to_query( lcn_list_t* query_list, apr_pool_t *pool )
{
    unsigned int i;
    lcn_query_t *bq;

    (void) lcn_boolean_query_create( &bq, pool );

    for( i = 0; i < lcn_list_size( query_list ); i++ )
    {
        struct lcn_query_with_clause_t* t = lcn_list_get( query_list, i );
        (void) lcn_boolean_query_add( bq, t->query, t->clause );
    }

    return bq;
}

#line 85 "query_parser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    lcn_query_parser_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is lcn_query_parser_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    lcn_query_parser_ARG_SDECL     A static variable declaration for the %extra_argument
**    lcn_query_parser_ARG_PDECL     A parameter declaration for the %extra_argument
**    lcn_query_parser_ARG_STORE     Code to store %extra_argument into yypParser
**    lcn_query_parser_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 22
#define YYACTIONTYPE unsigned char
#define lcn_query_parser_TOKENTYPE lcn_query_token_t *
typedef union {
  lcn_query_parser_TOKENTYPE yy0;
  char        * yy9;
  lcn_list_t  * yy12;
  lcn_query_t * yy36;
  int yy43;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define lcn_query_parser_ARG_SDECL  lcn_query_info_t* info ;
#define lcn_query_parser_ARG_PDECL , lcn_query_info_t* info 
#define lcn_query_parser_ARG_FETCH  lcn_query_info_t* info  = yypParser->info 
#define lcn_query_parser_ARG_STORE yypParser->info  = info 
#define YYNSTATE 28
#define YYNRULE 17
#define YYERRORSYMBOL 10
#define YYERRSYMDT yy43
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* Next are that tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     1,   17,   27,    9,   16,   20,   19,   18,   26,   46,
 /*    10 */    17,   14,   15,   28,   20,   19,   18,   26,   17,   44,
 /*    20 */    16,    3,   20,   19,   18,   23,    5,   17,    7,   12,
 /*    30 */    40,   20,   19,   18,   22,   17,   40,   40,   40,   20,
 /*    40 */    19,   18,   25,   24,    2,    2,   40,   11,   13,   13,
 /*    50 */     4,   10,   21,   40,    2,   40,    6,   40,   13,   40,
 /*    60 */    40,    8,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    11,   12,   13,    3,    1,   16,   17,   18,   19,   20,
 /*    10 */    12,   13,    9,    0,   16,   17,   18,   19,   12,    0,
 /*    20 */     1,    5,   16,   17,   18,   19,   14,   12,    3,    7,
 /*    30 */    21,   16,   17,   18,   19,   12,   21,   21,   21,   16,
 /*    40 */    17,   18,   19,    2,    4,    4,   21,    3,    8,    8,
 /*    50 */     6,    7,    2,   21,    4,   21,    1,   21,    8,   21,
 /*    60 */    21,    6,
};
#define YY_SHIFT_USE_DFLT (-1)
static const signed char yy_shift_ofst[] = {
 /*     0 */    41,   13,   16,   44,    0,   55,   25,   -1,   -1,   -1,
 /*    10 */    -1,   22,   -1,   41,    3,   -1,   50,   -1,   -1,   -1,
 /*    20 */    -1,   40,   -1,   -1,   40,   -1,   -1,   19,
};
#define YY_REDUCE_USE_DFLT (-12)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -11,  -12,  -12,  -12,   12,  -12,  -12,  -12,  -12,  -12,
 /*    10 */   -12,  -12,  -12,   -2,  -12,  -12,    6,  -12,  -12,  -12,
 /*    20 */   -12,   15,  -12,  -12,   23,  -12,  -12,  -12,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    45,   45,   45,   45,   45,   45,   45,   29,   31,   30,
 /*    10 */    32,   34,   33,   45,   45,   35,   45,   36,   37,   38,
 /*    20 */    39,   45,   42,   43,   45,   40,   41,   45,
};
#define YY_SZ_ACTTAB (sizeof(yy_action)/sizeof(yy_action[0]))

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammer, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  int stateno;       /* The state-number */
  int major;         /* The major token value.  This is the code
                     ** number for the token at this stack level */
  YYMINORTYPE minor; /* The user-supplied minor token value.  This
                     ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
  int yyerrcnt;                 /* Shifts left before out of the error */
  lcn_query_parser_ARG_SDECL                /* A place to hold %extra_argument */
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void lcn_query_parser_Trace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "SPACE",         "CLAUSE",        "TERM_TEXT",   
  "FIELD_PREFIX",  "SEP",           "QUOT_MARK",     "ASTERISK",    
  "OPEN_BR",       "CLOSE_BR",      "error",         "expr",        
  "bool_query",    "query_list",    "term_list",     "term",        
  "term_query",    "prefix_query",  "phrase_query",  "some_query",  
  "query",       
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "query ::= expr",
 /*   1 */ "term_list ::= term_list SPACE TERM_TEXT",
 /*   2 */ "term_list ::= TERM_TEXT",
 /*   3 */ "phrase_query ::= FIELD_PREFIX SEP QUOT_MARK term_list QUOT_MARK",
 /*   4 */ "prefix_query ::= FIELD_PREFIX SEP ASTERISK",
 /*   5 */ "prefix_query ::= FIELD_PREFIX SEP TERM_TEXT ASTERISK",
 /*   6 */ "term_query ::= FIELD_PREFIX SEP TERM_TEXT",
 /*   7 */ "bool_query ::= OPEN_BR query_list CLOSE_BR",
 /*   8 */ "some_query ::= bool_query",
 /*   9 */ "some_query ::= phrase_query",
 /*  10 */ "some_query ::= prefix_query",
 /*  11 */ "some_query ::= term_query",
 /*  12 */ "query_list ::= CLAUSE some_query",
 /*  13 */ "query_list ::= some_query",
 /*  14 */ "query_list ::= query_list SPACE CLAUSE some_query",
 /*  15 */ "query_list ::= query_list SPACE some_query",
 /*  16 */ "expr ::= query_list",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *lcn_query_parser_TokenName(int tokenType){
#ifndef NDEBUG
  if( tokenType>0 && tokenType<(sizeof(yyTokenName)/sizeof(yyTokenName[0])) ){
    return yyTokenName[tokenType];
  }else{
    return "Unknown";
  }
#else
  return "";
#endif
}

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to lcn_query_parser_ and lcn_query_parser_Free.
*/
void *lcn_query_parser_Alloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(YYCODETYPE yymajor, YYMINORTYPE *yypminor){
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor( yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from lcn_query_parser_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void lcn_query_parser_Free(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
  (*freeProc)((void*)pParser);
}

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  /* if( pParser->yyidx<0 ) return YY_NO_ACTION;  */
  i = yy_shift_ofst[stateno];
  if( i==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
    int iFallback;            /* Fallback token */
    if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
           && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
           yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
      }
#endif
      return yy_find_shift_action(pParser, iFallback);
    }
#endif
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  i = yy_reduce_ofst[stateno];
  if( i==YY_REDUCE_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer ot the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
  if( yypParser->yyidx>=YYSTACKDEPTH ){
     lcn_query_parser_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     lcn_query_parser_ARG_STORE; /* Suppress warning about unused %extra_argument var */
     return;
  }
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 20, 1 },
  { 14, 3 },
  { 14, 1 },
  { 18, 5 },
  { 17, 3 },
  { 17, 4 },
  { 16, 3 },
  { 12, 3 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 19, 1 },
  { 13, 2 },
  { 13, 1 },
  { 13, 4 },
  { 13, 3 },
  { 11, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  lcn_query_parser_ARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<sizeof(yyRuleName)/sizeof(yyRuleName[0]) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0:
#line 100 "query_parser.y"
{
    info->query = yymsp[0].minor.yy36;
}
#line 643 "query_parser.c"
        break;
      case 1:
#line 104 "query_parser.y"
{
    (void) lcn_list_add( yymsp[-2].minor.yy12, apr_pstrdup( info->pool, yymsp[0].minor.yy0->text) );
    yygotominor.yy12 = yymsp[-2].minor.yy12;
}
#line 651 "query_parser.c"
        break;
      case 2:
#line 109 "query_parser.y"
{
    (void) lcn_list_create( &(yygotominor.yy12), 10, info->pool );
    (void) lcn_list_add( yygotominor.yy12, apr_pstrdup( info->pool, yymsp[0].minor.yy0->text ) );
}
#line 659 "query_parser.c"
        break;
      case 3:
#line 114 "query_parser.y"
{
    lcn_list_t *term_list;
    unsigned int i;

    (void) lcn_multi_phrase_query_create( &(yygotominor.yy36), info->pool );
    (void) lcn_list_create( &term_list, 10, info->pool );

    for( i = 0; i < lcn_list_size( yymsp[-1].minor.yy12 ); i++ )
    {
        lcn_term_t *term;
        lcn_term_create( &term,
                         yymsp[-4].minor.yy0->text,
                         (char*) lcn_list_get( yymsp[-1].minor.yy12, i ),
                         LCN_TERM_TEXT_COPY,
                         info->pool );
        (void) lcn_multi_phrase_query_add_term( yygotominor.yy36, term );
    }
}
#line 681 "query_parser.c"
        break;
      case 4:
#line 133 "query_parser.y"
{
    lcn_term_t *term;

    (void) lcn_term_create( &term, yymsp[-2].minor.yy0->text, "", LCN_TERM_TEXT_COPY, info->pool );
    (void) lcn_prefix_query_create( &(yygotominor.yy36), term, info->pool );
}
#line 691 "query_parser.c"
        break;
      case 5:
#line 140 "query_parser.y"
{
    lcn_term_t *term;

    (void) lcn_term_create( &term, yymsp[-3].minor.yy0->text, yymsp[-1].minor.yy0->text, LCN_TERM_TEXT_COPY, info->pool );
    (void) lcn_prefix_query_create( &(yygotominor.yy36), term, info->pool );
}
#line 701 "query_parser.c"
        break;
      case 6:
#line 147 "query_parser.y"
{
    (void) lcn_term_query_create_by_chars( &(yygotominor.yy36), yymsp[-2].minor.yy0->text, yymsp[0].minor.yy0->text, info->pool );
}
#line 708 "query_parser.c"
        break;
      case 7:
#line 151 "query_parser.y"
{
    lcn_query_t *q = query_list_to_query( yymsp[-1].minor.yy12, info->pool );
    yygotominor.yy36 = q;
}
#line 716 "query_parser.c"
        break;
      case 8:
      case 9:
      case 10:
      case 11:
#line 156 "query_parser.y"
{ yygotominor.yy36 = yymsp[0].minor.yy36; }
#line 724 "query_parser.c"
        break;
      case 12:
#line 162 "query_parser.y"
{
    struct lcn_query_with_clause_t* q = apr_pcalloc( info->pool, sizeof(struct lcn_query_with_clause_t));
    (void) lcn_list_create( &(yygotominor.yy12), 10, info->pool );
    q->query = yymsp[0].minor.yy36;
    q->clause = (*(yymsp[-1].minor.yy0->text) == '-' ? LCN_BOOLEAN_CLAUSE_MUST_NOT : LCN_BOOLEAN_CLAUSE_MUST);
    (void) lcn_list_add( yygotominor.yy12, q );
}
#line 735 "query_parser.c"
        break;
      case 13:
#line 170 "query_parser.y"
{
    struct lcn_query_with_clause_t* q = apr_pcalloc( info->pool, sizeof(struct lcn_query_with_clause_t));
    (void) lcn_list_create( &(yygotominor.yy12), 10, info->pool );
    q->query = yymsp[0].minor.yy36;
    q->clause = LCN_BOOLEAN_CLAUSE_SHOULD;
    (void) lcn_list_add( yygotominor.yy12, q );
}
#line 746 "query_parser.c"
        break;
      case 14:
#line 178 "query_parser.y"
{
    struct lcn_query_with_clause_t* q = apr_pcalloc( info->pool, sizeof(struct lcn_query_with_clause_t));
    q->query = yymsp[0].minor.yy36;
    q->clause = (*(yymsp[-1].minor.yy0->text) == '-' ? LCN_BOOLEAN_CLAUSE_MUST_NOT : LCN_BOOLEAN_CLAUSE_MUST);
    (void) lcn_list_add( yymsp[-3].minor.yy12, q );
    yygotominor.yy12 = yymsp[-3].minor.yy12;
}
#line 757 "query_parser.c"
        break;
      case 15:
#line 186 "query_parser.y"
{
    struct lcn_query_with_clause_t* q = apr_pcalloc( info->pool, sizeof(struct lcn_query_with_clause_t));
    q->query = yymsp[0].minor.yy36;
    q->clause = LCN_BOOLEAN_CLAUSE_SHOULD;
    (void) lcn_list_add( yymsp[-2].minor.yy12, q );
    yygotominor.yy12 = yymsp[-2].minor.yy12;
}
#line 768 "query_parser.c"
        break;
      case 16:
#line 194 "query_parser.y"
{
    yygotominor.yy36 = query_list_to_query( yymsp[0].minor.yy12, info->pool );
}
#line 775 "query_parser.c"
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yypParser,yygoto);
  if( yyact < YYNSTATE ){
    yy_shift(yypParser,yyact,yygoto,&yygotominor);
  }else if( yyact == YYNSTATE + YYNRULE + 1 ){
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  lcn_query_parser_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 78 "query_parser.y"

    info->status = LCN_ERR_QUERY_PARSER_SYNTAX_ERROR;
#line 808 "query_parser.c"
  lcn_query_parser_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  lcn_query_parser_ARG_FETCH;
#define TOKEN (yyminor.yy0)
  lcn_query_parser_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  lcn_query_parser_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  lcn_query_parser_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "lcn_query_parser_Alloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void lcn_query_parser_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  lcn_query_parser_TOKENTYPE yyminor       /* The value for the token */
  lcn_query_parser_ARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
    if( yymajor==0 ) return;
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  lcn_query_parser_ARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,yymajor);
    if( yyact<YYNSTATE ){
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      if( yyendofinput && yypParser->yyidx>=0 ){
        yymajor = 0;
      }else{
        yymajor = YYNOCODE;
      }
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else if( yyact == YY_ERROR_ACTION ){
      int yymx;
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_shift_action(yypParser,YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }else{
      yy_accept(yypParser);
      yymajor = YYNOCODE;
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}