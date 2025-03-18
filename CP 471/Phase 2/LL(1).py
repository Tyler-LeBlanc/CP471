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
        "char"
    ],
    "digit": ["num"],
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
    "fname": {"char"},
    "decs": {"int", "double"},
    "decs_": {";","ε"},
    "decl": {"int", "double"},
    "type": {"int", "double"},
    "varlist": {"char"},
    "varlist_":{",","ε"},
    "st_seq": {"char", "if", "while", "print", "return", ";","ε"},
    "statement": {"char", "if", "while", "print", "return", "ε"},
    "st_seq_": {"else",  "ε"},
    "expr": {"(", "char", "num" },
    "expr_": {"+", "-","ε"},
    "term": {"(","char", "num"},
    "term_": {"*","/","%","ε"},
    "exprseq": {"(","char", "num"},
    "exprseq_": {",","ε"},
    "bexpr": {"(","char", "num","not"},
    "bexpr_": {"or", "ε"},
    "bterm": {"(", "not", "char", "num"},
    "bterm_": {"and", "ε"},
    "bfactor": {"(","not","char", "num"},
    "comp": {"<",">","==","<=",">=", "<>"},
    "var": {"char"},
    "id": {"char"},
    "id_": {"char", "num", "ε"},
    "number": {"num"},
    "Double": {"num"},
    "integer": {"num"},
    "integer_": {"num", "ε"},
    "factor": {"char", "num","("},
    "letter": {"char"},
    "digit": {"num",},
    "digits_":{"num", "ε"},
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
    "decs": {".",";", "fed", "if", "while", "print", "return","char"},
    "decs_": {".", ";", "fed", "if", "while", "print", "return","char"},
    "decl": {".", ";", "fed", "if", "while", "print", "return","char"},
    "type": {"char"},
    "varlist": {".", ";", "fed", "if", "while", "print", "return","char"},
    "varlist_": {".", ";", "fed", "if", "while", "print", "return","char"},
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
    "comp": {"(","char", "num"},
    "var": {".",";",")","fed",",","int","double","=","if", "then","while","print","return","*","%","/","char", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "]" ,"char",},
    "id": {".",";",")", "(","fed",",","int","double","=","if", "then","while","print","return","*","%","/","char", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "[", "]" ,"char",},
    "id_": {".",";",")", "(","fed",",","int","double","=","if", "then","while","print","return","*","%","/","char", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "[", "]" ,"char",},
    "number": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "Double": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "integer": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "integer_": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "factor": {".", ";", ")","fed",",","then","fi","do","od","+", "-", "*","/","%","or", "and", "<>", "<", ">", "<=", ">=","]"},
    "letter": {".",";",")", "(","fed",",","int","double","=","if", "then","while","print","return","*","%","/","char", "num", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "[", "]" ,"char",},
    "digit": {".",";",")", "(","fed",",","int","double","=","if", "then","while","print","return","*","%","/","char", "num", "fi", "do", "od", "else", "+", "-", "*","/","%", "or", "and", "<>", "<", ">", "<=", ">=", "[", "]" ,"char",},
    "digits_":{},
    "exponent": {},
    "sign": {"num"}

}


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

# Display the LL(1) Parsing Table

headers = ["Non-Terminal"] + list(terminals)
rows = [[nt] + [ll1_table[nt][t] if ll1_table[nt][t] else "-" for t in terminals] for nt in ll1_table]
print(tabulate(rows, headers, tablefmt="grid"))

#headers = ["Non-Terminal"] + list(terminals)
with open("ll(1).txt", "w", encoding="utf-8") as file:
    fileString = tabulate(rows, headers, tablefmt="grid")
    file.write(fileString)
