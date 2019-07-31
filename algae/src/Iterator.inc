<?php

namespace PARCE\ALGAE;

include dirname(__FILE__) . '/Evaluator.inc';

/**
 * Class Iterator
 *
 * This class provides the methods to traverse an array or list of tokens that
 * represent a parsed version of a string.
 *
 * Based on the shunting-yard algorithm it provides the methods for parsing
 * mathematical expressions and it has been extended to also handled logical
 * gate expressions.
 *
 * Algorithm has also been extended to evaluate expression and provide an output
 * in Reverse Polish Notation (RPN).
 */
class Iterator extends Evaluator {

  private $queue;  // holds all variables, as well operators as they get popped
                   // from the stack.
  private $stack;  // holds all the operators during evaluation.

  public function __construct() {
    $this->queue = array();
    $this->stack = array();
  }

  /**
   * Checks whether a token is an operator.
   *
   * @param $token
   *   a token from the list of tokens passed to be traverse.
   * @return bool
   *   true if the token is an operator, false otherwise.
   */
  private function isOperator($token) {
    $operators = parent::getOperators();
    return array_key_exists($token, $operators);
  }

  /**
   * Checks whether a token is a left bracket.
   *
   * @param $token
   *   a token from the list of tokens passed to be traverse.
   * @return bool
   *   true if the token is a left bracket, false otherwise.
   */
  private function isLeftBracket($token) {
    if($token == '(') return TRUE;
    return FALSE;
  }

  /**
   * Checks whether a token is a right bracket.
   *
   * @param $token
   *   a token from the list of tokens passed to be traverse.
   * @return bool
   *   true if the token is a right bracket, false otherwise.
   */
  private function isRightBracket($token) {
    if($token == ')') return TRUE;
    return FALSE;
  }

  /**
   * Checks whether a token is an operand.
   *
   * @param $token
   *   a token from the list of tokens passed to be traverse.
   * @return bool
   *   true if the token is an operand, false otherwise.
   */
  private function isOperand($token) {

    if($this->isLeftBracket($token) || $this->isRightBracket($token) || $this->isOperator($token)) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Checks whether the last operator in the stack has greater precedence than
   * the parameter operator.
   *
   * @param $operator
   *   an operator to compare its precedence against top stack operator.
   * @return bool
   *   true if the operator in the stack has greater precedence, false otherwise.
   */
  private function operatorHasGreaterPrecedence($operator) {

    // check if the stack is empty
    if($this->isStackEmpty()) {
      return FALSE;
    }
    // get last operator in stack
    $stack_last = end($this->stack);
    // skip if the last token in the stack is a left bracket.
    if(!$this->isLeftBracket($stack_last)) {
      // set a hierarchy of operator precedence
      $precedence = parent::getOperators();
      // determine if last operator in $stack is of greater precedence than $token
      if($precedence[$stack_last]['weight'] > $precedence[$operator]['weight']) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Checks whether the stack is empty.
   *
   * @return bool
   *   true if the stack is empty, false otherwise.
   */
  private function isStackEmpty() {
    return empty($this->stack);
  }

  /**
   * Checks if the last operator in the stack matches the parameter operator.
   *
   * @param $operator
   *   an operator to check against the last operator in the stack.
   * @return bool
   *   true if the operator in the stack matches the parameter operator.
   */
  private function isStackTopOperator($operator) {
    // check if the last operator in the stack matches the $operator
    $stack_last = end($this->stack);
    // return True or False
    if($stack_last == $operator) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Traverses through a list of tokens. Based on the Shunting-Yard Algorithm
   * and Reverse Polish Notation.
   *
   * @param $arg
   *   an array of information with all the data needed to perform an evaluation
   *   on a mathematical expression.
   * @return array
   *   an array of information after the mathematical expression has been
   *   evaluated.
   * @throws \Exception
   *   an exception to handle a possible infinite loop error. the error is
   *   thrown early in the process to avoid any looping issues, however it does
   *   not mean that it is infinite.
   */
  public function traverse($arg) {

    $tokens = $arg->tokens;

    $token_index = 0;

    while($token_index < sizeof($tokens)) {

      // safe mode
      if($token_index > sizeof($tokens)) {

        $json_msg[] = array(
          'error_msg' => 'Infinite loop error occurred while iterating through the mathematical expression.',
          'error' => TRUE,
          'type' => 'DEBUG',
        );

        throw new \Exception(json_encode($json_msg));
      }

      $current_token = $tokens[$token_index];

      // if OPERATOR
      if($this->isOperator($current_token)) {
        // while this operator has greater precedence
        while($this->operatorHasGreaterPrecedence($current_token)) {
          // pop operator from $stack
          $operator = array_pop($this->stack);
          // evaluate expression
          $eval_result = parent::evaluateExpression($operator, $this->queue);
          // push output to queue
          array_push($this->queue, $eval_result);
        }
        // push current operator to $operator_stack
        array_push($this->stack, $current_token);
      }

      // if LEFT bracket
      elseif($this->isLeftBracket($current_token)) {
        // push bracket to $stack
        array_push($this->stack, $current_token);
      }


      // if RIGHT bracket
      elseif($this->isRightBracket($current_token)) {
        // while not left bracket at top of $stack
        while(!$this->isStackTopOperator('(')) {
          // pop operator from $stack
          $operator = array_pop($this->stack);
          // evaluate expression
          $eval_result = parent::evaluateExpression($operator, $this->queue);
          // push output to queue
          array_push($this->queue, $eval_result);
        }
        // pop left bracket from stack, and discard it
        array_pop($this->stack);
      }


      // if variable, or operand, e.g. null, '', integer
      elseif($this->isOperand($current_token)) {
        // push to queue
        array_push($this->queue, $current_token);
      }

      ++$token_index;
    }

    // while operators on stack
    while(!$this->isStackEmpty()) {
      // pop operator from stack
      $operator = array_pop($this->stack);
      // evaluate expression
      $eval_result = parent::evaluateExpression($operator, $this->queue);
      // push output to queue
      array_push($this->queue, $eval_result);
    }

    return array(
      'math_expression' => $arg->math_expression_sanitized,
      'evaluation'  => ($this->queue[0]) ? TRUE : FALSE,
    );
  }

}