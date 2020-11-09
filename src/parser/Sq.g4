grammar Sq;

field_tree_list: field_tree (field_tree)*;
field_tree: dot_expression (brace_expression)?;
brace_expression: '{' field_tree_list '}';
dot_expression: field_call ('.' field_call)*;
field_call: ID parameter_list?;
parameter_list: '(' primitive_value? (',' primitive_value)* ')';
primitive_value: (INTEGER | DQ_STR);

ID: [a-z_]+;
WS: [ \t\r\n]+ -> skip;
DQ_STR: '"' (~'"' | '\\"' )* '"';
INTEGER: '-'?[0-9]+;
