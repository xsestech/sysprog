#pragma once

#include <libicli/error.h>

/**
 * @file utils.h
 * @brief Utility functions for libicli
 */

/**
 * @brief Split a string into tokens
 * @param input Input string to split
 * @param argc Pointer to store the number of tokens
 * @param error_code Pointer to store error code if not NULL
 * @return Array of token strings or NULL on error (must be freed by caller)
 */
char** icli_utils_split_string(const char* input, int* argc, icli_error_code* error_code);

/**
 * @brief Free an array of strings
 * @param array The array to free
 * @param count Number of elements in the array
 */
void icli_utils_free_string_array(char** array, int count);

/**
 * @brief Safely duplicate a string with error handling
 * @param str String to duplicate
 * @param error_code Pointer to store error code if not NULL
 * @return New string or NULL on error (must be freed by caller)
 */
char* icli_utils_strdup_safe(const char* str, icli_error_code* error_code);