#pragma once

#include <libicli/error.h>
#include <libicli/command.h>

/**
 * @file cli.h
 * @brief Main CLI interface for libicli
 */

/**
 * @struct icli_t
 * @brief Structure representing the CLI
 */
typedef struct icli_t icli_t;

/**
 * @brief Create a new CLI instance
 * @param prompt The prompt string to display
 * @param exit_command The command to exit the CLI (e.g., "exit")
 * @param context User provided context passed to commands
 * @param error_code Pointer to store error code if not NULL
 * @return Newly created CLI or NULL on error
 */
icli_t* icli_create(
    const char* prompt,
    const char* exit_command,
    void* context,
    icli_error_code* error_code
);

/**
 * @brief Destroy a CLI instance and free its resources
 * @param cli CLI to destroy
 */
void icli_destroy(icli_t* cli);

/**
 * @brief Register a command with the CLI
 * @param cli CLI instance
 * @param command Command to register
 * @param error_code Pointer to store error code if not NULL
 * @return ICLI_SUCCESS on success, error code otherwise
 */
icli_error_code icli_register_command(
    icli_t* cli,
    icli_command_t* command,
    icli_error_code* error_code
);

/**
 * @brief Run the CLI loop
 * @param cli CLI instance
 * @param error_code Pointer to store error code if not NULL
 * @return ICLI_SUCCESS on success, error code otherwise
 */
icli_error_code icli_run(icli_t* cli, icli_error_code* error_code);

/**
 * @brief Update the context of a command
 * @param command Command to update
 * @param context New context
 */
void icli_command_set_context(icli_command_t* command, void* context);

/**
 * @brief Process a single command
 * @param cli CLI instance
 * @param command_line Command line to process
 * @param error_code Pointer to store error code if not NULL
 * @return 0 to continue, 1 to exit
 */
int icli_process_command(
    icli_t* cli,
    const char* command_line,
    icli_error_code* error_code
);

/**
 * @brief Get registered command by name
 * @param cli CLI instance
 * @param name Command name
 * @param error_code Pointer to store error code if not NULL
 * @return Command or NULL if not found
 */
icli_command_t* icli_get_command(
    icli_t* cli,
    const char* name,
    icli_error_code* error_code
);

/**
 * @brief Get all registered commands
 * @param cli CLI instance
 * @param count Pointer to store the number of commands
 * @param error_code Pointer to store error code if not NULL
 * @return Array of command pointers or NULL on error
 */
icli_command_t** icli_get_commands(
    icli_t* cli,
    int* count,
    icli_error_code* error_code
);

/**
 * @brief Get the user context
 * @param cli CLI instance
 * @param error_code Pointer to store error code if not NULL
 * @return User context or NULL if not set
 */
void* icli_get_context(icli_t* cli, icli_error_code* error_code);