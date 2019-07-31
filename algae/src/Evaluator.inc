<?php

/**
 * @file
 * Evaluation class for mathematical and logical expressions.
 */

namespace PARCE\ALGAE;

/**
 * Class Evaluator
 *
 * This class provides all the methods to evaluate mathematical expressions as
 * well as evaluate logic gate expressions.
 */
abstract class Evaluator {

  public function __construct()  {}

  /**
   * Gets an expression's term value depending on its Type, e.g. a string,
   * booleans, date, numbers, etc.
   *
   * @param $term
   *   the token to have evaluated.
   * @param $which
   *   which term in the expression we are retrieving the value for, e.g. left
   *   or right.
   * @return bool|int|string|array
   */
  public function getTermValue($term, $which) {

    if(is_array($term)) { // multi-select
      return $term;
    }

    if(is_bool($term)) {
      return $term;
    }

    if(is_numeric($term)) {
      return $term;
    }

    // The following conditions forces the right term of the expression to
    // evaluate to the empty whether is null or empty. This masks
    // join_logic_full right operands.
    if($which == 'right' && ($term == 'null')) {
      return '';
    }

    if($term == 'null') {
      return $term;
    }

    if($term == '\'\'') {
      return '';
    }

    // this assumes this value to be a string
    return $term;
  }

  /**
   * Defines all operators allowed in the iterator with their corresponding
   * precedence.
   *
   * @param bool $array_flip
   *   if TRUE then it returns a list of operators with where the value is set
   *   to the operator symbol.
   * @return array
   *   a list of operators and their precedence value, key values respectively
   *   unless $array_flip is True.
   */
  public function getOperators() {
    // the weight represents the precedence.
    $operators = array(
      '*'   => array('weight' => 10, 'method' => 'multiply'),        // arithmetic
      '/'   => array('weight' => 10, 'method' => 'divide'),          // arithmetic
      '+'   => array('weight' => 8,  'method' => 'add'),             // arithmetic
      '-'   => array('weight' => 8,  'method' => 'subtract'),        // arithmetic
      '='   => array('weight' => 4,  'method' => 'equal'),           // comparison
      '!='  => array('weight' => 4,  'method' => 'notEqual'),        // comparison
      '<'   => array('weight' => 4,  'method' => 'lessThan'),        // comparison
      '<='  => array('weight' => 4,  'method' => 'lessThanOrEqual'), // comparison
      '>'   => array('weight' => 4,  'method' => 'moreThan'),        // comparison
      '>='  => array('weight' => 4,  'method' => 'moreThanOrEqual'), // comparison
      'AND' => array('weight' => 2,  'method' => 'logicalAND'),      // logical
      'OR'  => array('weight' => 2,  'method' => 'logicalOR'),       // logical
    );

    return $operators;
  }

  /**
   * Evaluates a mathematical expression.
   *
   * @param $operator
   *   the operator to be used to evaluate the expression.
   * @param $queue
   *   a list of variables and operators as set by the ALGEAIterator.
   * @return mixed
   * @throws \Exception
   */
  public function evaluateExpression($operator, &$queue) {
    // pop the first operand from queue
    $operandRight = array_pop($queue);
    // pop the second operand from queue
    $operandLeft = array_pop($queue);
    // resolved right operand value
    $operandRightValue = $this->getTermValue($operandRight, 'right');
    // resolved left operand value
    $operandLeftValue = $this->getTermValue($operandLeft, 'left');

    // get a list of all the allowed operators
    $operators = $this->getOperators();
    // invoke the proper operator method and passed the operands as arguments.
    if(isset($operators[$operator])) {
      $method = $operators[$operator]['method'];
      return call_user_func(__NAMESPACE__ . "\Evaluator::$method", $operandLeftValue, $operandRightValue);
    }

    $json_msg[] = array(
      'error_msg' => t('The operator !operator is not allowed', array(
        '!operator' => $operators[$operator])
      ),
      'error' => TRUE,
      'type' => 'DEBUG',
    );

    throw new \Exception(json_encode($json_msg));
  }

  /**
   * Performs an addition.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function add($leftOperand, $rightOperand) {
    return $leftOperand + $rightOperand;
  }

  /**
   * Performs a subtraction.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function subtract($leftOperand, $rightOperand) {
    return $leftOperand - $rightOperand;
  }

  /**
   * Performs a multiplication.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function multiply($leftOperand, $rightOperand) {
    return $leftOperand * $rightOperand;
  }

  /**
   * Performs a division.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function divide($leftOperand, $rightOperand) {
    return $leftOperand / $rightOperand;
  }

  /**
   * Performs an equal comparison.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function equal($leftOperand, $rightOperand) {

    // special case for multi-select
    if(is_array($leftOperand)) {
      return in_array($rightOperand, $leftOperand);
    }

    return $leftOperand == $rightOperand;
  }

  /**
   * Performs a not equal comparison.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function notEqual($leftOperand, $rightOperand) {
    return $leftOperand != $rightOperand;
  }

  /**
   * Performs a less than comparison.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function lessThan($leftOperand, $rightOperand) {
    return $leftOperand < $rightOperand;
  }

  /**
   * Performs a more than comparison.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function moreThan($leftOperand, $rightOperand) {
    return $leftOperand > $rightOperand;
  }

  /**
   * Performs a less than or equal.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function lessThanOrEqual($leftOperand, $rightOperand) {
    return ($leftOperand <= $rightOperand);
  }

  /**
   * Performs a more than or equal.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function moreThanOrEqual($leftOperand, $rightOperand) {
    return ($leftOperand >= $rightOperand);
  }

  /**
   * Performs a logical AND.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function logicalAND($leftOperand, $rightOperand) {
    return $leftOperand AND $rightOperand;
  }

  /**
   * Performs a logical OR.
   *
   * @param $leftOperand
   *   the left operand or term of the expression to be calculated.
   * @param $rightOperand
   *   the right operand or term of the expression to be calculated.
   * @return int
   *   the result of the math expression.
   */
  private function logicalOR($leftOperand, $rightOperand) {
    return $leftOperand OR $rightOperand;
  }

}
