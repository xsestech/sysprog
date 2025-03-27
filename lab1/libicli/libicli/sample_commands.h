#pragma once

#include <libicli/command.h>

/**
 * @file sample_commands.h
 * @brief Sample commands for the CLI
 */

/**
 * @brief Create a help command that displays available commands
 * @param error_code Pointer to store error code if not NULL
 * @return Help command or NULL on error
 */
icli_command_t* icli_create_help_command(icli_error_code* error_code);

/**
 * @brief Create an echo command that prints its arguments
 * @param error_code Pointer to store error code if not NULL
 * @return Echo command or NULL on error
 */
icli_command_t* icli_create_echo_command(icli_error_code* error_code);

/**
 * @brief Create a version command that displays version information
 * @param version_str Version string to display
 * @param error_code Pointer to store error code if not NULL
 * @return Version command or NULL on error
 */
icli_command_t* icli_create_version_command(const char* version_str, icli_error_code* error_code);