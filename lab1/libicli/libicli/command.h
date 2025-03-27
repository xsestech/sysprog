#pragma once

#include <libicli/error.h>

/**
 * @file command.h
 * @brief Command structure and handling for libicli
 */

/**
 * @struct icli_command_t
 * @brief Structure representing a command in the CLI
 */
typedef struct icli_command_t {
 char* name;                 /**< Command name */
 char* description;          /**< Command description */

 /**
  * @brief Function pointer for command execution
  * @param argc Argument count
  * @param argv Array of argument strings
  * @param context User provided context
  * @param error_code Pointer to store error code if not NULL
  * @return 0 on success, non-zero on error
  */
 int (*execute)(int argc, char** argv, void* context, icli_error_code* error_code);
} icli_command_t;

/**
 * @brief Create a new command
 * @param name Command name
 * @param description Command description
 * @param execute Execution function
 * @param error_code Pointer to store error code if not NULL
 * @return Newly created command or NULL on error
 */
icli_command_t* icli_command_create(
    const char* name,
    const char* description,
    int (*execute)(int argc, char** argv, void* context, icli_error_code* error_code),
    icli_error_code* error_code
);

/**
 * @brief Destroy a command and free its resources
 * @param command Command to destroy
 */
void icli_command_destroy(icli_command_t* command);