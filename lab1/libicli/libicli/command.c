#include <libicli/command.h>
#include <stdlib.h>
#include <string.h>

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
) {
    if (name == NULL || execute == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_NULL_POINTER;
        }
        return NULL;
    }

    icli_command_t* command = (icli_command_t*)malloc(sizeof(icli_command_t));
    if (command == NULL) {
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return NULL;
    }

    command->name = strdup(name);
    if (command->name == NULL) {
        free(command);
        if (error_code) {
            *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
        }
        return NULL;
    }

    if (description != NULL) {
        command->description = strdup(description);
        if (command->description == NULL) {
            free(command->name);
            free(command);
            if (error_code) {
                *error_code = ICLI_ERROR_MEMORY_ALLOCATION;
            }
            return NULL;
        }
    } else {
        command->description = NULL;
    }

    command->execute = execute;

    if (error_code) {
        *error_code = ICLI_SUCCESS;
    }
    return command;
}

/**
 * @brief Destroy a command and free its resources
 * @param command Command to destroy
 */
void icli_command_destroy(icli_command_t* command) {
    if (command == NULL) {
        return;
    }

    if (command->name != NULL) {
        free(command->name);
    }

    if (command->description != NULL) {
        free(command->description);
    }

    free(command);
}