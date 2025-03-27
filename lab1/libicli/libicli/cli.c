#include <libicli/cli.h>
#include <libicli/utils.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * @struct command_node_t
 * @brief Node in the command linked list
 */
typedef struct command_node_t {
    icli_command_t* command;
    struct command_node_t* next;
} command_node_t;

/**
 * @struct icli_t
 * @brief Structure representing the CLI
 */
struct icli_t {
    char* prompt;
    char* exit_command;
    void* context;
    command_node_t* commands;
    int command_count;
};

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
) {
    if (prompt == NULL || exit_command == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return NULL;
    }

    icli_t* cli = (icli_t*)malloc(sizeof(icli_t));
    if (cli == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return NULL;
    }

    cli->prompt = icli_utils_strdup_safe(prompt, error_code);
    if (cli->prompt == NULL) {
        free(cli);
        return NULL;
    }

    cli->exit_command = icli_utils_strdup_safe(exit_command, error_code);
    if (cli->exit_command == NULL) {
        free(cli->prompt);
        free(cli);
        return NULL;
    }

    cli->context = context;
    cli->commands = NULL;
    cli->command_count = 0;

    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return cli;
}

/**
 * @brief Destroy a CLI instance and free its resources
 * @param cli CLI to destroy
 */
void icli_destroy(icli_t* cli) {
    if (cli == NULL) {
        return;
    }

    free(cli->prompt);
    free(cli->exit_command);

    /* Free all commands */
    command_node_t* current = cli->commands;
    while (current != NULL) {
        command_node_t* next = current->next;
        icli_command_destroy(current->command);
        free(current);
        current = next;
    }

    free(cli);
}

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
) {
    if (cli == NULL || command == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return ICLI_ERROR_NULL_POINTER;
    }

    /* Check if command already exists */
    command_node_t* current = cli->commands;
    while (current != NULL) {
        if (strcmp(current->command->name, command->name) == 0) {
            if (error_code) {
                *error_code = ICLI_ERROR_COMMAND_EXISTS;
            }
            return ICLI_ERROR_COMMAND_EXISTS;
        }
        current = current->next;
    }

    /* Create new command node */
    command_node_t* node = (command_node_t*)malloc(sizeof(command_node_t));
    if (node == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return ICLI_ERROR_MEMORY_ALLOCATION;
    }

    node->command = command;
    node->next = cli->commands;
    cli->commands = node;
    cli->command_count++;

    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return ICLI_SUCCESS;
}

/**
 * @brief Find a command by name
 * @param cli CLI instance
 * @param name Command name
 * @return Command or NULL if not found
 */
static icli_command_t* find_command(icli_t* cli, const char* name) {
    command_node_t* current = cli->commands;
    while (current != NULL) {
        if (strcmp(current->command->name, name) == 0) {
            return current->command;
        }
        current = current->next;
    }
    return NULL;
}

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
) {
    if (cli == NULL || command_line == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return 0;
    }

    /* Split command line into tokens */
    int argc;
    char** argv = icli_utils_split_string(command_line, &argc, error_code);
    if (argc == 0 || argv == NULL) {
        return 0;
    }

    int result = 0;

    /* Check if it's the exit command */
    if (strcmp(argv[0], cli->exit_command) == 0) {
        result = 1;
    } else {
        /* Find and execute command */
        icli_command_t* command = find_command(cli, argv[0]);
        if (command == NULL) {
            if (error_code) {
                *error_code = ICLI_ERROR_COMMAND_NOT_FOUND;
            }
            printf("Command not found: %s\n", argv[0]);
        } else {
            icli_error_code cmd_error = ICLI_SUCCESS;
            /* Pass the CLI instance as the context for all commands
             * This allows commands like 'help' to access the CLI structure */
            int cmd_result = command->execute(argc, argv, cli, &cmd_error);
            if (cmd_result != 0) {
                printf("Command failed: %s\n", icli_error_to_string(cmd_error));
                if (error_code) {
                    *error_code = cmd_error;
                }
            }
        }
    }

    /* Free argument array */
    icli_utils_free_string_array(argv, argc);

    return result;
}

/**
 * @brief Run the CLI loop
 * @param cli CLI instance
 * @param error_code Pointer to store error code if not NULL
 * @return ICLI_SUCCESS on success, error code otherwise
 */
icli_error_code icli_run(icli_t* cli, icli_error_code* error_code) {
    if (cli == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return ICLI_ERROR_NULL_POINTER;
    }

    char input_buffer[1024];
    int should_exit = 0;

    while (!should_exit) {
        printf("%s ", cli->prompt);
        fflush(stdout);

        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            if (feof(stdin)) {
                /* End of file - exit gracefully */
                break;
            } else {
                if (error_code) {
                    *error_code = ICLI_ERROR_IO;
                }
                return ICLI_ERROR_IO;
            }
        }

        /* Remove trailing newline */
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }

        /* Skip empty lines */
        if (input_buffer[0] == '\0') {
            continue;
        }

        icli_error_code cmd_error = ICLI_SUCCESS;
        should_exit = icli_process_command(cli, input_buffer, &cmd_error);
        if (cmd_error != ICLI_SUCCESS && error_code) {
            *error_code = cmd_error;
        }
    }

    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return ICLI_SUCCESS;
}

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
) {
    if (cli == NULL || name == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return NULL;
    }

    icli_command_t* command = find_command(cli, name);
    if (command == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_COMMAND_NOT_FOUND;
        }
        return NULL;
    }

    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return command;
}

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
) {
    if (cli == NULL || count == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return NULL;
    }

    *count = cli->command_count;
    if (cli->command_count == 0) {
        if (error_code) {
            *error_code = ICLI_SUCCESS;
        }
        return NULL;
    }

    icli_command_t** commands = (icli_command_t**)malloc(
        cli->command_count * sizeof(icli_command_t*)
    );
    if (commands == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return NULL;
    }

    int i = 0;
    command_node_t* current = cli->commands;
    while (current != NULL) {
        commands[i++] = current->command;
        current = current->next;
    }

    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return commands;
}

/**
 * @brief Get the user context
 * @param cli CLI instance
 * @param error_code Pointer to store error code if not NULL
 * @return User context or NULL if not set
 */
void* icli_get_context(icli_t* cli, icli_error_code* error_code) {
    if (cli == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return NULL;
    }

    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return cli->context;
}