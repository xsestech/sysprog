#include <libicli/utils.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Split a string into tokens
 * @param input Input string to split
 * @param argc Pointer to store the number of tokens
 * @param error_code Pointer to store error code if not NULL
 * @return Array of token strings or NULL on error (must be freed by caller)
 */
char** icli_utils_split_string(const char* input, int* argc, icli_error_code* error_code) {
    if (input == NULL || argc == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return NULL;
    }

    *argc = 0;

    /* Count the number of tokens */
    int count = 0;
    int in_token = 0;
    for (const char* p = input; *p != '\0'; p++) {
        if (isspace((unsigned char)*p)) {
            in_token = 0;
        } else if (!in_token) {
            in_token = 1;
            count++;
        }
    }

    if (count == 0) {
        *argc = 0;
        if (error_code) {
            *error_code = ICLI_SUCCESS;
        }
        return NULL;
    }

    /* Allocate the array of token pointers */
    char** tokens = (char**)malloc(count * sizeof(char*));
    if (tokens == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return NULL;
    }

    /* Extract the tokens */
    int token_idx = 0;
    in_token = 0;
    const char* token_start = input;

    for (const char* p = input; ; p++) {
        if (*p == '\0' || isspace((unsigned char)*p)) {
            if (in_token) {
                size_t token_len = p - token_start;
                tokens[token_idx] = (char*)malloc(token_len + 1);
                if (tokens[token_idx] == NULL) {
                    /* Free all allocated tokens */
                    for (int i = 0; i < token_idx; i++) {
                        free(tokens[i]);
                    }
                    free(tokens);
                    if (error_code) {
                        *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
                    }
                    return NULL;
                }
                memcpy(tokens[token_idx], token_start, token_len);
                tokens[token_idx][token_len] = '\0';
                token_idx++;
                in_token = 0;
            }
            if (*p == '\0') {
                break;
            }
        } else if (!in_token) {
            token_start = p;
            in_token = 1;
        }
    }

    *argc = count;
    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return tokens;
}

/**
 * @brief Free an array of strings
 * @param array The array to free
 * @param count Number of elements in the array
 */
void icli_utils_free_string_array(char** array, int count) {
    if (array == NULL) {
        return;
    }

    for (int i = 0; i < count; i++) {
        if (array[i] != NULL) {
            free(array[i]);
        }
    }

    free(array);
}

/**
 * @brief Safely duplicate a string with error handling
 * @param str String to duplicate
 * @param error_code Pointer to store error code if not NULL
 * @return New string or NULL on error (must be freed by caller)
 */
char* icli_utils_strdup_safe(const char* str, icli_error_code* error_code) {
    if (str == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return NULL;
    }

    char* new_str = strdup(str);
    if (new_str == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return NULL;
    }

    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return new_str;
}