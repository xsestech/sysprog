#pragma once

/**
 * @file error.h
 * @brief Error handling for the libicli library
 */

/**
 * @enum icli_error_code
 * @brief Error codes used throughout the libicli library
 */
typedef enum {
  ICLI_SUCCESS = 0,         /**< Operation completed successfully */
  ICLI_ERROR_NULL_POINTER,  /**< Null pointer was provided */
  ICLI_ERROR_INVALID_ARGS,  /**< Invalid arguments were provided */
  ICLI_ERROR_COMMAND_NOT_FOUND, /**< Command was not found */
  ICLI_ERROR_MEMORY_ALLOCATION, /**< Memory allocation failed */
  ICLI_ERROR_COMMAND_EXISTS,    /**< Command already exists */
  ICLI_ERROR_INVALID_COMMAND,   /**< Invalid command format */
  ICLI_ERROR_IO,               /**< I/O error */
  ICLI_ERROR_UNKNOWN           /**< Unknown error */
} icli_error_code;

/**
 * @brief Get error message for a specific error code
 * @param code The error code
 * @return String representation of the error
 */
const char* icli_error_to_string(icli_error_code code);