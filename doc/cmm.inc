program              :        (statements functions)+ ;

statements           :        statement* ;
functions            :        function* ;

statement            :        if_statement | while_statement | read_statement | write_statement | assign_statement | declare_statement | return_statement | SEMICOLON ;
function             :        FUNC declare_keyword ID LPAREN (declare_keyword ID (COMMA declare_keyword ID)*)? RPAREN LBRACE statements RBRACE ;

if_statement         :        IF LPAREN condition RPAREN LBRACE statements RBRACE (ELSE LBRACE statements RBRACE)? ;
while_statement      :        WHILE LPAREN condition RPAREN LBRACE statements RBRACE ;
read_statement       :        READ LPAREN ID RPAREN SEMICOLON ;
write_statement      :        WRITE LPAREN expression RPAREN SEMICOLON ;
assign_statement     :        (ID | ID array) ASSIGN expression SEMICOLON ;
declare_statement    :        declare_keyword ID(COMMA ID)* SEMICOLON ;
return_statement     :        RETURN (ID)? SEMICOLON ;

declare_keyword      :        INT | INT array | REAL | REAL array | VOID ;
condition            :        expression comparison_op expression ;
expression           :        term (add_op term)* ;
term                 :        factor (mul_op factor)* ;
comparison_op        :        LT | LTE | GT | GTE | EQUAL | NEQUAL ;
add_op               :        PLUS | MINUS ;
mul_op               :        TIMES | DIVIDE ;
factor               :        REAL_LITERAL | INTEGER_LITERAL | ID | LPAREN expression RPAREN | ID array ;
array                :        LBRACKET (INTEGER_LITERAL | ID) RBRACKET ;

FUNC                 :        'func' ;
RETURN               :        'return' ;
READ                 :        'read' ;
WRITE                :        'write' ;
WHILE                :        'while' ;
IF                   :        'if' ;
ELSE                 :        'else' ;
INT                  :        'int' ;
REAL                 :        'real' ;
VOID                 :        'void' ;
SEMICOLON            :        ';' ;
LPAREN               :        '(' ;
RPAREN               :        ')' ;
LBRACKET             :        '[' ;
RBRACKET             :        ']' ;
LBRACE               :        '{' ;
RBRACE               :        '}' ;
COMMA                :        ',' ;
ASSIGN               :        '=' ;
PLUS                 :        '+' ;
MINUS                :        '-' ;
TIMES                :        '*' ;
DIVIDE               :        '/' ;
LT                   :        '<' ;
LTE                  :        '<=' ;
GT                   :        '>' ;
GTE                  :        '>=' ;
EQUAL                :        '==' ;
NEQUAL               :        '<>' ;

LINE_COMMENT         :        '//';
BLOCK_LEFT_COMMENT   :        '/*';
BLOCK_RIGHT_COMMENT  :        '*/';

LETTER               :        ('a'..'z'|'A'..'Z') ;
DIGIT                :        ('0'..'9') ;
ID                   :        LETTER ((LETTER | DIGIT | '_')*(LETTER | DIGIT))? ;
INTEGER_LITERAL      :        ('1'..'9')('0'..'9')*|'0' ;
REAL_LITERAL         :        (INTEGER_LITERAL)('.'(INTEGER_LITERAL)+)? ;

NEWLINE              :        '\r'? '\n' ;
WS                   :        (' '|'\t')+ {skip();};
