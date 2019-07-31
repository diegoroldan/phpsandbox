# Arithmetic & Logic Gate Algorithm Evaluator

## Algorithm

The algorithm uses the Shunting-Yard algorithm and applies the Reverse Polish
Notation to traverse through an array of tokens, essentially a parsed 
mathematical expression (string).

 - Queue holds operands, evaluation of expression results, and operators
         including logical operators.

 - Stack holds the logical operators.

```php
<ALGORITHM>

WHILE TOKENS

  IF OPERATOR
    WHILE OPERATOR WITH GREATER PRECEDENCE
      POP OPERATOR FROM STACK
      EVALUATE EXPRESSION
      PUSH EXPRESSION RESULT TO QUEUE
    PUSH CURRENT OPERATOR TO STACK

  ELSE IF LEFT BRACKET
    PUSH TO STACK

  ELSE IF RIGHT BRACKET
    WHILE NOT LEFT BRACKET AT TOP OF STACK
      POP OPERATOR FROM STACK
      EVALUATE EXPRESSION
      PUSH EXPRESSION RESULT TO QUEUE
    POP LEFT BRACKET FROM STACK

  ELSE IF OPERAND
      PUSH TO QUEUE

WHILE OPERATORS IN STACK
  EVALUATE EXPRESSION
  PUSH RESULT TO QUEUE

</ALGORITHM>
```

## References

 - [The Shunting Yard Algorithm](https://rosettacode.org/wiki/Parsing/Shunting-yard_algorithm) developed by Edsger Dijkstra
 - [Reverse Polish Notation (RPN)](http://rosettacode.org/wiki/Parsing/RPN_calculator_algorithm)
 
## Examples

In this example the $result variable will have an array with the mathematical 
expression submitted for evaluation and the result of the mathematical 
expression. 

```php
require_once('algae/src/MathExpression.inc');

$args = new stdClass();
$args->debug = true;
$args->math_expression = '((1 + 2) = ( 2 + 1))';

$math_exp = new \PARCE\ALGAE\MathExpression($args);
$result = $math_exp->evaluate();
```

## Upcoming features

 - Less strict syntax for arithmetic operands.