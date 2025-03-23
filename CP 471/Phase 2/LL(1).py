from tabulate import tabulate

# Example Grammar



productions = {
    "program": ["fdecls decs st_seq ."],
    "fdecls": [ "fdec fdecls_"],
    "fdecls_": ["; fdec fdecls_", "ε"],
    "fdec": ["def type fname ( params ) decs st_seq fed"],
    "params": ["type var params_"],
    "params_": [", type var params_","ε"],
    "fname": ["id"],
    "decs": ["decl decs_"],
    "decs_": ["; decl decs_", "ε"],
    "decl": ["type varlist"],
    "type": ["int", "double"],
    "varlist": ["var , varlist_"],
    "varlist_":[", var varlist_", "ε"],
    "st_seq": ["statement", "statement ; st_seq"],
    "statement": [
        "var = expr",
        "if bexpr then st_seq st_seq_ fi",
        "while bexpr do st_seq od",
        "print expr",
        "return expr",
        "ε"
    ],
    "st_seq_": ["else st_seq", "ε"],
    "expr": ["term expr_"],
    "expr_": ["+ term expr_", "- term expr_", "ε"],
    "term": ["factor term_"],
    "term_": ["* factor term_", "% factor term_", "/ factor term_", "ε"],
    "exprseq": ["expr , exprseq_"],
    "exprseq_": [", expr exprseq_","ε"],
    "bexpr": ["bterm bexpr_"],
    "bexpr_": ["or bfactor bterm_", "ε"],
    "bterm": ["bfactor bterm_"],
    "bterm_": ["and bfactor bterm_", "ε"],
    "bfactor": ["( bexpr )", "not bfactor", "expr comp expr"],
    "comp": ["<", ">", "==", "<=", ">=", "<>"],
    "var": ["id", "id [ expr ]"],
    "id": ["letter id_"],
    "id_": ["letter id_", "digit id_", "ε"],
    "number": ["integer", "Double"],
    "Double": ["integer . integer"],
    "integer": ["digit integer_"],
    "integer_": ["digit integer_", "ε"],
    "factor": ["var", "number", "( expr )", "fname ( exprseq )"],
    "letter": [
        "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"
    ],
    "digit": ["1","2","3","4","5","6","7","8","9","0"],
    "digits_": ["digit", "digits_"],
    "exponent": ["e sign digit digits_" , "E sign digit digits_" , "ε"],
    "sign": ["+", "-", "ε"],
}

first_sets = {
    "program": {"def"},
    "fdecls": {"def"},
    "fdecls_": {";","ε"},
    "fdec": {"def"},
    "params": {"int", "double"},
    "params_": {",","ε"},
    "fname": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "decs": {"int", "double"},
    "decs_": {";","ε"},
    "decl": {"int", "double"},
    "type": {"int", "double"},
    "varlist": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "varlist_":{",","ε"},
    "st_seq": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "if", "while", "print", "return", ";","ε"},
    "statement": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "if", "while", "print", "return", "ε"},
    "st_seq_": {"else",  "ε"},
    "expr": {"(", "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0" },
    "expr_": {"+", "-","ε"},
    "term": {"(","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0"},
    "term_": {"*","/","%","ε"},
    "exprseq": {"(","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0"},
    "exprseq_": {",","ε"},
    "bexpr": {"(","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0","not"},
    "bexpr_": {"or", "ε"},
    "bterm": {"(", "not", "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0"},
    "bterm_": {"and", "ε"},
    "bfactor": {"(","not","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0"},
    "comp": {"<",">","==","<=",">=", "<>"},
    "var": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "id": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "id_": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0", "ε"},
    "number": {"1","2","3","4","5","6","7","8","9","0"},
    "Double": {"1","2","3","4","5","6","7","8","9","0"},
    "integer": {"1","2","3","4","5","6","7","8","9","0"},
    "integer_": {"1","2","3","4","5","6","7","8","9","0", "ε"},
    "factor": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0","("},
    "letter": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "digit": {"1","2","3","4","5","6","7","8","9","0",},
    "digits_":{"1","2","3","4","5","6","7","8","9","0", "ε"},
    "exponent":{"e","E","ε"},
    "sign":{"ε","+","-"}
}

follow_sets = {
    "program": {"$"},
    "fdecls": {"int","double"},
    "fdecls_": {"int","double"},
    "fdec": {";", "int","double"},
    "params": {")"},
    "params_": {")"},
    "fname": {"("},
    "decs": {".",";", "fed", "if", "while", "print", "return","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "decs_": {".", ";", "fed", "if", "while", "print", "return","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "decl": {".", ";", "fed", "if", "while", "print", "return","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "type": {"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "varlist": {".", ";", "fed", "if", "while", "print", "return","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "varlist_": {".", ";", "fed", "if", "while", "print", "return","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"},
    "st_seq": {".","fed", "fi", "od", "else"},
    "statement": {".", ";","fed","fi","od","else"},
    "st_seq_": {"fi"},
    "expr": {".", ";", ")","fed",",","then","fi","do","od","else","and","or","<",">","==","<=",">=", "<>", "]"},
    "expr_": {".", ";", ")","fed",",","then","fi","do","od","else","and","or","<",">","==","<=",">=", "<>", "]"},
    "term": {".", ";", ")","fed",",","then","fi","do","od","else","and","or","<",">","==","<=",">=", "<>", "]", "+", "-"},
    "term_": {".", ";", ")","fed",",","then","fi","do","od","else","and","or","<",">","==","<=",">=", "<>", "]", "+", "-"},
    "exprseq": {")"},
    "exprseq_": {")"},
    "bexpr": {")", "then", "do"},
    "bexpr_": {")", "then", "do"},
    "bterm": {")", "then", "do", "or"},
    "bterm_": {")", "then", "do", "or"},
    "bfactor": {")", "then", "do", "and","or"},
    "comp": {"(","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0"},
    "var": {".",";",")","fed",",","int","double","=","if", "then","while","print","return","*","%","/","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "]" ,"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",},
    "id": {".",";",")", "(","fed",",","int","double","=","if", "then","while","print","return","*","%","/","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "[", "]" ,"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",},
    "id_": {".",";",")", "(","fed",",","int","double","=","if", "then","while","print","return","*","%","/","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "[", "]" ,"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",},
    "number": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "Double": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "integer": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "integer_": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "factor": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "letter": {".",";",")", "(","fed",",","int","double","=","if", "then","while","print","return","*","%","/","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "[", "]" ,"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",},
    "digit": {".",";",")", "(","fed",",","int","double","=","if", "then","while","print","return","*","%","/","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z", "1","2","3","4","5","6","7","8","9","0", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "[", "]" ,"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",},
    "digits_":{},
    "exponent": {},
    "sign": {"1","2","3","4","5","6","7","8","9","0"}

}

from tabulate import tabulate

# Terminals (Gather from First and Follow sets)
terminals = set()
for first in first_sets.values():
    for symbol in first:
        if symbol != "ε":
            terminals.add(symbol)
for follow in follow_sets.values():
    for symbol in follow:
        terminals.add(symbol)
terminals.add("$")  # End of input symbol

# Initialize LL(1) Parsing Table
ll1_table = {non_terminal: {t: "" for t in terminals} for non_terminal in productions.keys()}

# Fill Table Using First and Follow Sets
for non_terminal, rules in productions.items():
    for rule in rules:
        first_of_rule = set()
        if rule == "ε":
            first_of_rule = {"ε"}
        else:
            for symbol in rule:
                first_of_rule |= first_sets.get(symbol, {symbol}) - {"ε"}
                if "ε" not in first_sets.get(symbol, {symbol}):
                    break
            else:
                first_of_rule.add("ε")

        # Populate Table
        for terminal in first_of_rule - {"ε"}:
            ll1_table[non_terminal][terminal] = rule
        
        # If epsilon is in First(A), add to Follow(A)
        if "ε" in first_of_rule:
            for terminal in follow_sets[non_terminal]:
                ll1_table[non_terminal][terminal] = rule

# Generate C-compatible array of structs
c_code = """#include <search.h>
#include <stdio.h>

#define MAX_RULES %d

typedef struct {
    char *non_terminal;
    char *terminal;
    char *production;
} ParsingRule;

ParsingRule parsing_table[MAX_RULES] = {
""" % (sum(1 for nt in ll1_table for t in ll1_table[nt] if ll1_table[nt][t]))

for non_terminal, terminals_map in ll1_table.items():
    for terminal, production in terminals_map.items():
        if production:  # Only add entries with actual rules
            c_code += f'    {{"{non_terminal}", "{terminal}", "{production}"}},\n'

c_code += """};

int main() {
    printf("LL(1) Parsing Table Loaded. Use hash map lookup to access rules.\\n");
    return 0;
}
"""

# Write the C-compatible table to a file
with open("ll1_table.c", "w", encoding="utf-8") as file:
    file.write(c_code)

print("C code for LL(1) table generated in ll1_table.c")

