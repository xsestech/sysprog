#include <libicli/sample_commands.h>
#include <libicli/cli.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure holding version command data
 */
typedef struct {
    char* version_string;
} version_command_data_t;

/**
 * @brief Help command implementation
 * @param argc Argument count
 * @param argv Array of argument strings
 * @param context User provided context (should be icli_t*)
 * @param error_code Pointer to store error code if not NULL
 * @return 0 on success, non-zero on error
 */
static int help_execute(int argc, char** argv, void* context, icli_error_code* error_code) {
    icli_t* cli = (icli_t*)context;
    if (cli == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return 1;
    }

    int command_count;
    icli_command_t** commands = icli_get_commands(cli, &command_count, error_code);
    if (commands == NULL && command_count > 0) {
        return 1;
    }

    printf("Available commands:\n");
    for (int i = 0; i < command_count; i++) {
        if (commands[i]->description) {
            printf("  %-15s - %s\n", commands[i]->name, commands[i]->description);
        } else {
            printf("  %s\n", commands[i]->name);
        }
    }

    free(commands);
    
    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return 0;
}

/**
 * @brief Echo command implementation
 * @param argc Argument count
 * @param argv Array of argument strings
 * @param context User provided context
 * @param error_code Pointer to store error code if not NULL
 * @return 0 on success, non-zero on error
 */
static int echo_execute(int argc, char** argv, void* context, icli_error_code* error_code) {
    if (argc < 2) {
        printf("Usage: echo <text>\n");
        if (error_code) {
            *error_code = ICLI_ERROR_INVALID_ARGS;
        }
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return 0;
}

/**
 * @brief Version command implementation
 * @param argc Argument count
 * @param argv Array of argument strings
 * @param context User provided context
 * @param error_code Pointer to store error code if not NULL
 * @return 0 on success, non-zero on error
 */
static int version_execute(int argc, char** argv, void* context, icli_error_code* error_code) {
    version_command_data_t* data = (version_command_data_t*)context;
    if (data == NULL || data->version_string == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return 1;
    }

    printf("Version: %s\n", data->version_string);
    
    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return 0;
}

/**
 * @brief Version command destructor
 * @param context Context to destroy
 */
static void version_destructor(void* context) {
    if (context) {
        version_command_data_t* data = (version_command_data_t*)context;
        free(data->version_string);
        free(data);
    }
}

/**
 * @brief Create a help command that displays available commands
 * @param error_code Pointer to store error code if not NULL
 * @return Help command or NULL on error
 */
icli_command_t* icli_create_help_command(icli_error_code* error_code) {
    return icli_command_create(
        "help", 
        "Display help information", 
        help_execute, 
        error_code
    );
}

/**
 * @brief Create an echo command that prints its arguments
 * @param error_code Pointer to store error code if not NULL
 * @return Echo command or NULL on error
 */
icli_command_t* icli_create_echo_command(icli_error_code* error_code) {
    return icli_command_create(
        "echo", 
        "Echo the provided text", 
        echo_execute, 
        error_code
    );
}

/**
 * @brief Create a version command that displays version information
 * @param version_str Version string to display
 * @param error_code Pointer to store error code if not NULL
 * @return Version command or NULL on error
 */
icli_command_t* icli_create_version_command(const char* version_str, icli_error_code* error_code) {
    if (version_str == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return NULL;
    }

    version_command_data_t* data = (version_command_data_t*)malloc(sizeof(version_command_data_t));
    if (data == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return NULL;
    }

    data->version_string = strdup(version_str);
    if (data->version_string == NULL) {
        free(data);
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return NULL;
    }

    icli_command_t* command = icli_command_create(
        "version", 
        "Display version information", 
        version_execute, 
        error_code
    );

    if (command == NULL) {
        free(data->version_string);
        free(data);
        return NULL;
    }

    return command;
}