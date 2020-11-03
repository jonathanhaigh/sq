grammar Sq;

field_tree_list: field_tree (field_tree)*;
field_tree: dot_expression (brace_expression)?;
brace_expression: '{' field_tree_list '}';
dot_expression: field_call ('.' field_call)*;
field_call: ID parameter_list?;
parameter_list: '(' DQ_STR? (',' DQ_STR)* ')';

ID: [a-z_]+;
WS: [ \t\r\n]+ -> skip;
DQ_STR: '"' (~'"' | '\\"' )* '"';
