#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libicli/cli.h>
#include <libicli/sample_commands.h>
#include "user.h"
#include "app_state.h"

#define MAX_INPUT_LENGTH 256
#define MAX_ARGS 4

int time_execute(int argc, char **argv, void *context, icli_error_code *error_code)
{
    icli_t *cli = (icli_t *)context;
    app_state_t *state = (app_state_t *)icli_get_context(cli, error_code);
    if (!state || !state->current_user)
    {
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    if (!user_can_make_request(state->current_user))
    {
        printf("You have reached your request limit\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    printf("%02d:%02d:%02d\n", tm->tm_hour, tm->tm_min, tm->tm_sec);

    user_increment_requests(state->current_user);
    if (error_code)
        *error_code = ICLI_SUCCESS;
    return 0;
}

int date_execute(int argc, char **argv, void *context, icli_error_code *error_code)
{
    icli_t *cli = (icli_t *)context;
    app_state_t *state = (app_state_t *)icli_get_context(cli, error_code);
    if (!state || !state->current_user)
    {
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    if (!user_can_make_request(state->current_user))
    {
        printf("You have reached your request limit\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    printf("%02d.%02d.%d\n", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

    user_increment_requests(state->current_user);
    if (error_code)
        *error_code = ICLI_SUCCESS;
    return 0;
}

int howmuch_execute(int argc, char **argv, void *context, icli_error_code *error_code)
{
    icli_t *cli = (icli_t *)context;
    app_state_t *state = (app_state_t *)icli_get_context(cli, error_code);
    if (!state || !state->current_user)
    {
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    if (!user_can_make_request(state->current_user))
    {
        printf("You have reached your request limit\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    if (argc != 3) // command + date + flag
    {
        printf("Usage: howmuch <date> <flag>\n");
        printf("Example: howmuch 23.03.2025 -s\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_ARGS;
        return 1;
    }

    struct tm tm = {0};
    if (strptime(argv[1], "%d.%m.%Y", &tm) == NULL)
    {
        printf("Invalid date format. Use DD.MM.YYYY\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_ARGS;
        return 1;
    }

    time_t target = mktime(&tm);
    time_t now = time(NULL);
    double diff = difftime(now, target);

    if (strcmp(argv[2], "-s") == 0)
    {
        printf("%.0f seconds\n", diff);
    }
    else if (strcmp(argv[2], "-m") == 0)
    {
        printf("%.0f minutes\n", diff / 60);
    }
    else if (strcmp(argv[2], "-h") == 0)
    {
        printf("%.0f hours\n", diff / 3600);
    }
    else if (strcmp(argv[2], "-y") == 0)
    {
        printf("%.0f years\n", diff / (365 * 24 * 3600));
    }
    else
    {
        printf("Invalid flag. Use -s, -m, -h, or -y\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_ARGS;
        return 1;
    }

    user_increment_requests(state->current_user);
    if (error_code)
        *error_code = ICLI_SUCCESS;
    return 0;
}

int sanctions_execute(int argc, char **argv, void *context, icli_error_code *error_code)
{
    icli_t *cli = (icli_t *)context;
    app_state_t *state = (app_state_t *)icli_get_context(cli, error_code);
    if (!state || !state->current_user)
    {
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    if (!user_can_make_request(state->current_user))
    {
        printf("You have reached your request limit\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    if (argc != 3)
    {
        printf("Usage: sanctions <username> <limit>\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_ARGS;
        return 1;
    }

    uint32_t limit = atoi(argv[2]);
    uint32_t confirmation;
    printf("Enter confirmation code (12345): ");
    if (scanf("%u", &confirmation) != 1 || confirmation != 12345)
    {
        printf("Invalid confirmation code\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_ARGS;
        return 1;
    }

    if (user_manager_set_limit(&state->user_manager, argv[1], limit) != 0)
    {
        printf("Failed to set sanctions\n");
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }

    printf("Sanctions set successfully\n");
    user_increment_requests(state->current_user);
    if (error_code)
        *error_code = ICLI_SUCCESS;
    return 0;
}

int logout_execute(int argc, char **argv, void *context, icli_error_code *error_code)
{
    icli_t *cli = (icli_t *)context;
    app_state_t *state = (app_state_t *)icli_get_context(cli, error_code);
    if (!state)
    {
        if (error_code)
            *error_code = ICLI_ERROR_INVALID_COMMAND;
        return 1;
    }
    state->current_user = NULL;
    printf("Logged out\n");
    if (error_code)
        *error_code = ICLI_SUCCESS;
    return 0;
}

void auth_menu(app_state_t *state)
{
    char login[MAX_LOGIN_LENGTH + 1];
    uint32_t pin;
    char choice;

    while (1)
    {
        printf("\n1. Login\n2. Register\n3. Exit\nChoice: ");
        scanf(" %c", &choice);
        getchar(); // Clear newline

        if (choice == '3')
        {
            exit(0);
        }

        printf("Login (max 6 chars): ");
        scanf("%6s", login);
        printf("PIN (0-100000): ");
        scanf("%u", &pin);

        if (choice == '1')
        {
            state->current_user = user_manager_auth(&state->user_manager, login, pin);
            if (state->current_user)
            {
                printf("Login successful\n");
                return;
            }
            else
            {
                printf("Invalid credentials. Please register first if you haven't already.\n");
            }
        }
        else if (choice == '2')
        {
            if (user_manager_register(&state->user_manager, login, pin) == 0)
            {
                printf("Registration successful. You can now login.\n");
            }
            else
            {
                printf("Registration failed. The login might already be taken or invalid.\n");
            }
        }
        else
        {
            printf("Invalid choice. Please select 1, 2, or 3.\n");
        }
    }
}

int main(void)
{
    app_state_t state = {0};
    if (user_manager_init(&state.user_manager) != 0)
    {
        fprintf(stderr, "Failed to initialize user manager\n");
        return 1;
    }

    icli_error_code error_code;
    icli_t *cli = icli_create("> ", "exit", &state, &error_code);
    if (!cli)
    {
        fprintf(stderr, "Failed to create CLI\n");
        return 1;
    }

    // Create and register commands
    icli_command_t *help_cmd = icli_create_help_command(&error_code);
    if (!help_cmd)
    {
        fprintf(stderr, "Failed to create help command\n");
        icli_destroy(cli);
        return 1;
    }

    icli_command_t *time_cmd = icli_command_create("time", "Show current time", time_execute, &error_code);
    icli_command_t *date_cmd = icli_command_create("date", "Show current date", date_execute, &error_code);
    icli_command_t *howmuch_cmd = icli_command_create("howmuch", "Calculate time difference", howmuch_execute, &error_code);
    icli_command_t *sanctions_cmd = icli_command_create("sanctions", "Set user request limit", sanctions_execute, &error_code);
    icli_command_t *logout_cmd = icli_command_create("logout", "Logout from current user", logout_execute, &error_code);

    if (!time_cmd || !date_cmd || !howmuch_cmd || !sanctions_cmd || !logout_cmd)
    {
        fprintf(stderr, "Failed to create commands\n");
        icli_destroy(cli);
        return 1;
    }

    // Add commands to CLI
    if (icli_register_command(cli, help_cmd, &error_code) != 0 ||
        icli_register_command(cli, time_cmd, &error_code) != 0 ||
        icli_register_command(cli, date_cmd, &error_code) != 0 ||
        icli_register_command(cli, howmuch_cmd, &error_code) != 0 ||
        icli_register_command(cli, sanctions_cmd, &error_code) != 0 ||
        icli_register_command(cli, logout_cmd, &error_code) != 0)
    {
        fprintf(stderr, "Failed to register commands\n");
        icli_destroy(cli);
        return 1;
    }

    char input[256];
    while (1)
    {
        if (!state.current_user)
        {
            auth_menu(&state);
        }

        printf("%s> ", state.current_user->login);
        if (!fgets(input, sizeof(input), stdin))
        {
            break;
        }

        // Remove trailing newline
        input[strcspn(input, "\n")] = 0;

        // Execute command
        int result = icli_process_command(cli, input, &error_code);
        if (result == 1)
        {
            break; // Exit command was executed
        }
        if (error_code != ICLI_SUCCESS)
        {
            switch (error_code)
            {
            case ICLI_ERROR_INVALID_COMMAND:
                printf("Invalid command\n");
                break;
            case ICLI_ERROR_INVALID_ARGS:
                printf("Invalid arguments\n");
                break;
            case ICLI_ERROR_COMMAND_NOT_FOUND:
                printf("Command not found\n");
                break;
            default:
                printf("Command execution failed: %s\n", icli_error_to_string(error_code));
                break;
            }
        }
    }

    // Cleanup
    icli_destroy(cli);
    return 0;
}