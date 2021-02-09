grammar Sq;

query: field_tree_list EOF;
field_tree_list: field_tree (field_tree)*;
field_tree: dot_expression (brace_expression)?;
brace_expression: '{' field_tree_list '}';
dot_expression: field_call ('.' field_call)*;
field_call: ID parameter_list? list_filter?;
parameter_list: '(' parameter? (',' parameter)* ')';
parameter: (named_parameter | primitive_value);
named_parameter: ID '=' primitive_value;
primitive_value: (INTEGER | DQ_STR | boolean);
boolean: (BOOLEAN_TRUE | BOOLEAN_FALSE);
list_filter: (list_element_access | simple_list_slice | list_slice);
list_element_access: '[' INTEGER ']';
simple_list_slice: '[' list_slice_start? ':' list_slice_stop? ']';
list_slice: '[' list_slice_start? ':' list_slice_stop? ':' list_slice_step? ']';
list_slice_start: INTEGER;
list_slice_stop: INTEGER;
list_slice_step: INTEGER;

BOOLEAN_TRUE: 'true';
BOOLEAN_FALSE: 'false';
ID: [a-zA-Z_]+;
WS: [ \t\r\n]+ -> skip;
DQ_STR: '"' (~'"' | '\\"' )* '"';
INTEGER: '-'?[0-9]+;
