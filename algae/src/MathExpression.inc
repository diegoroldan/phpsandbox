<?php

namespace PARCE\ALGAE;

include dirname(__FILE__) . '/Iterator.inc';

/**
 * Class MathExpression
 *
 * This class allows to perform a mathematical expression evaluation by
 * instantiating it and invoking the evaluate() method. This object will
 * sanitize and tokenize a JSON object passed as an argument to the evaluate()
 * method followed by instantiating an Iterator object where the the tokens will
 * be traverse and the math expression and logical operation will be evaluated.
 */
class MathExpression {

  private static $args;

  /**
   * @param $args
   *   can be a string representing a json object or a stdClass object with all
   *   the required arguments.
   *     - math_expression: a string with arithmetic and logic gate expressions
   *         to evaluate.
   */
  public function __construct($args) {

    self::$args = $args;
  }

  /**
   * Method accessible publicly to allow to perform arithmetic & logic
   * expression evaluations.
   *
   * @return array|string
   */
  public function evaluate() {

    $arg = self::$args;

    try {

      // sanitize the mathematical expression.
      $arg->math_expression_sanitized = self::sanitizeExpression($arg->math_expression);
      // tokenize the decoded json object.
      $arg->tokens = self::tokenize($arg->math_expression_sanitized);
      // instantiate the iterator.
      $eval = new Iterator();
      // invoke the iterator's traverse method to evaluate the expression.
      $result = $eval->traverse($arg);

    } catch(\Exception $e) {
      // present the error to the caller in json format.
      return self::jsonDecode($e->getMessage());
    }

    return $result;
  }

  /**
   * A string in JSON format containing all the required information to perform
   * an evaluation.
   *
   * @param $str
   *   a php object representation of a decoded json string.
   * @return String
   *   a sanitized string in json format.
   */
  private function sanitizeExpression($str) {

    // Sanitized the expression from the object.
    $str_sanitized = str_replace(array('(', ')'), array(' ( ', ' ) '), $str);
    $str_sanitized = trim($str_sanitized);

    return $str_sanitized;
  }

  /**
   * json_decode wrapper.
   *
   * @param $json
   *   the json object to be decoded into its php object representation.
   * @return bool|mixed
   *   if it is a valid json object then it returns the php object representation,
   *   otherwise it returns FALSE.
   */
  private function jsonDecode($json) {

    try {
      if ($php = $this->isJSON($json)) {
        // decode the json object to a php object.
        return $php;
      }
    } catch(\Exception $e) {}

    return FALSE;
  }

  /**
   * Checks to make sure that the json string has a valid format.
   *
   * @param $json
   *   a json string to be validated.
   * @return mixed
   *   a php object representation of the json object, otherwise throws an
   *   exception.
   * @throws \Exception
   *   if the json object does not have a valid formatting.
   */
  private function isJSON($json) {

    $php = json_decode($json);

    if(is_string($json) && (is_object($php) || is_array($php))&& (json_last_error() == JSON_ERROR_NONE)) {
      return $php;
    }

    $json_msg[] = array(
      'error_msg' => t('The JSON object has an invalid format.'),
      'error' => TRUE,
      'type'  => 'DEBUG',
    );

    throw new \Exception(json_encode($json_msg));
  }

  /**
   * Splits a string into tokens.
   *
   * @param $string
   *   the string to be tokenize
   * @return array
   *   a list of tokens representing the string parameter.
   */
  private function tokenize($string) {
    // split the string into tokens.
    return preg_split('/[\s]+/', $string);
  }
}



