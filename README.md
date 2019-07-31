# PHP Sandbox
A list of PHP Sandbox projects.

## ALGAE
The implementation uses the Shunting-Yard algorithm and applies the Reverse 
Polish Notation (RPN) to traverse through an array of tokens, essentially to 
parsed mathematical expressions (string). The evaluation of these expressions
return a boolean value of true or false. 

e.g. mathematical expression to determine if it evaluates to true or false.

 - ```(9 = 3 OR 1 = 4) OR (1 = 2)```
 > evaluates to _false_
 - ```((1 + 2) = ( 2 + 1))```
 > evaluates to _true_