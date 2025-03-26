#ifndef AST_H_
#define AST_H_

#define MAX_STACK 100

typedef struct NODE
{
    char *value;            // holds the string value
    struct NODE **children; // dynamic array of pointers to child nodes
    int child_count;        // current num of child nodes
    int capacity;           // max capacity of the child array

} NODE;

typedef enum
{
    SYMBOL_TERMINAL,
    SYMBOL_NONTERMINAL
} SymbolType;

typedef struct
{
    SymbolType type;
    int value;
} Symbol;

typedef struct
{
    Symbol array[MAX_STACK]; // array to hold elements of the stack
    int top;                 // index of the top element
} STACK;

typedef enum
{
    TERM_DEF,       // "def"
    TERM_FED,       // "fed"
    TERM_INT,       // "int"
    TERM_DOUBLE,    // "double"
    TERM_IF,        // "if"
    TERM_THEN,      // "then"
    TERM_ELSE,      // "else"
    TERM_FI,        // "fi"
    TERM_WHILE,     // "while"
    TERM_DO,        // "do"
    TERM_OD,        // "od"
    TERM_PRINT,     // "print"
    TERM_RETURN,    // "return"
    TERM_PLUS,      // "+"
    TERM_MINUS,     // "-"
    TERM_MULT,      // "*"
    TERM_DIV,       // "/"
    TERM_MOD,       // "%"
    TERM_OR,        // "or"
    TERM_AND,       // "and"
    TERM_NOT,       // "not"
    TERM_LPAREN,    // "("
    TERM_RPAREN,    // ")"
    TERM_COMMA,     // ","
    TERM_SEMICOLON, // ";"
    TERM_LBRACKET,  // "["
    TERM_RBRACKET,  // "]"
    TERM_LT,        // "<"
    TERM_GT,        // ">"
    TERM_EQEQ,      // "=="
    TERM_LE,        // "<="
    TERM_GE,        // ">="
    TERM_NE,        // "<>"
    TERM_ASSIGN,    // "="
    TERM_DOT,       // "."
    TERM_ID,        // identifier (a–z,...)
    TERM_NUMBER,    // numeric literal
    TERM_EOF,       // end-of-input marker
    NUM_TERMINALS
} Terminal;

/* Nonterminals from the transformed grammar.
   (Some productions were refactored to remove ambiguity in <factor>.) */
typedef enum
{
    NT_PROGRAM,       // <program> → <fdecls_list> <decl_list> <stmt_seq> .
    NT_FDECLS_LIST,   // <fdecls_list> → <fdec> ; <fdecls_list> | ε
    NT_FDEC,          // <fdec> → def <type> <id> ( <params> ) <decl_list> <stmt_seq> fed
    NT_PARAMS,        // <params> → <type> <id> <param_list> | ε
    NT_PARAM_LIST,    // <param_list> → , <type> <id> <param_list> | ε
    NT_DECL_LIST,     // <decl_list> → <decl> ; <decl_list> | ε
    NT_DECL,          // <decl> → <type> <varlist>
    NT_TYPE,          // <type> → int | double
    NT_VARLIST,       // <varlist> → <id> <varlist_tail>
    NT_VARLIST_TAIL,  // <varlist_tail> → , <id> <varlist_tail> | ε
    NT_STMT_SEQ,      // <stmt_seq> → <statement> <stmt_seq_tail>
    NT_STMT_SEQ_TAIL, // <stmt_seq_tail> → ; <statement> <stmt_seq_tail> | ε
    NT_STATEMENT,     // <statement> → <id> <var_tail> = <expr>
                      //             | if <bexpr> then <stmt_seq> <if_tail>
                      //             | while <bexpr> do <stmt_seq> od
                      //             | print <expr>
                      //             | return <expr>
                      //             | ε
    NT_IF_TAIL,       // <if_tail> → fi | else <stmt_seq> fi
    NT_EXPR,          // <expr> → <term> <expr_prime>
    NT_EXPR_PRIME,    // <expr_prime> → + <term> <expr_prime> | - <term> <expr_prime> | ε
    NT_TERM,          // <term> → <factor> <term_prime>
    NT_TERM_PRIME,    // <term_prime> → * <factor> <term_prime>
                      //               | / <factor> <term_prime>
                      //               | % <factor> <term_prime>
                      //               | ε
    NT_FACTOR,        // <factor> → <id> <factor_tail> | <number> | ( <expr> )
    NT_FACTOR_TAIL,   // <factor_tail> → ( <exprseq> ) | <var_tail>
    NT_EXPRSEQ,       // <exprseq> → <expr> <exprseq_tail> | ε
    NT_EXPRSEQ_TAIL,  // <exprseq_tail> → , <expr> <exprseq_tail> | ε
    NT_BEXPR,         // <bexpr> → <bterm> <bexpr_prime>
    NT_BEXPR_PRIME,   // <bexpr_prime> → or <bterm> <bexpr_prime> | ε
    NT_BTERM,         // <bterm> → <bfactor> <bterm_prime>
    NT_BTERM_PRIME,   // <bterm_prime> → and <bfactor> <bterm_prime> | ε
    NT_BFACTOR,       // <bfactor> → ( <bfactor_inner> ) | not <bfactor>
    NT_BFACTOR_INNER, // <bfactor_inner> → <expr> <comp> <expr>
    NT_COMP,          // <comp> → < | > | == | <= | >= | <>
    NT_VAR_TAIL,      // <var_tail> → [ <expr> ] | ε
    NUM_NONTERMINALS
} Nonterminal;

/* Structure representing an entry in the LL(1) parsing table.
   The production is represented as a string (for simplicity). */
typedef struct
{
    const char *production;
} ParseTableEntry;

#endif
