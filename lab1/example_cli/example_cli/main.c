#include <libicli/cli.h>
#include <libicli/sample_commands.h>
#include <stdio.h>

/**
 * @brief Custom command for testing
 * @param argc Argument count
 * @param argv Array of argument strings
 * @param context User provided context
 * @param error_code Pointer to store error code if not NULL
 * @return 0 on success, non-zero on error
 */
static int hello_execute(int argc, char **argv, void *context, icli_error_code *error_code)
{
    printf("Hello, world!\n");

    if (error_code)
    {
        *error_code = ICLI_SUCCESS;
    }
    return 0;
}

int main()
{
    icli_error_code error_code;

    /* Create CLI */
    icli_t *cli = icli_create("> ", "exit", NULL, &error_code);
    if (cli == NULL)
    {
        fprintf(stderr, "Failed to create CLI: %s\n", icli_error_to_string(error_code));
        return 1;
    }

    /* Create help command */
    icli_command_t *help_cmd = icli_create_help_command(&error_code);
    if (help_cmd == NULL)
    {
        fprintf(stderr, "Failed to create help command: %s\n", icli_error_to_string(error_code));
        icli_destroy(cli);
        return 1;
    }

    error_code = icli_register_command(cli, help_cmd, &error_code);
    if (error_code != ICLI_SUCCESS)
    {
        fprintf(stderr, "Failed to register help command: %s\n", icli_error_to_string(error_code));
        icli_command_destroy(help_cmd);
        icli_destroy(cli);
        return 1;
    }

    /* Create and register echo command */
    icli_command_t *echo_cmd = icli_create_echo_command(&error_code);
    if (echo_cmd == NULL)
    {
        fprintf(stderr, "Failed to create echo command: %s\n", icli_error_to_string(error_code));
        icli_destroy(cli);
        return 1;
    }

    error_code = icli_register_command(cli, echo_cmd, &error_code);
    if (error_code != ICLI_SUCCESS)
    {
        fprintf(stderr, "Failed to register echo command: %s\n", icli_error_to_string(error_code));
        icli_command_destroy(echo_cmd);
        icli_destroy(cli);
        return 1;
    }

    /* Create and register version command */
    icli_command_t *version_cmd = icli_create_version_command("1.0.0", &error_code);
    if (version_cmd == NULL)
    {
        fprintf(stderr, "Failed to create version command: %s\n", icli_error_to_string(error_code));
        icli_destroy(cli);
        return 1;
    }

    error_code = icli_register_command(cli, version_cmd, &error_code);
    if (error_code != ICLI_SUCCESS)
    {
        fprintf(stderr, "Failed to register version command: %s\n", icli_error_to_string(error_code));
        icli_command_destroy(version_cmd);
        icli_destroy(cli);
        return 1;
    }

    /* Create and register custom hello command */
    icli_command_t *hello_cmd = icli_command_create(
        "hello",
        "Display hello message",
        hello_execute,
        &error_code);
    if (hello_cmd == NULL)
    {
        fprintf(stderr, "Failed to create hello command: %s\n", icli_error_to_string(error_code));
        icli_destroy(cli);
        return 1;
    }

    error_code = icli_register_command(cli, hello_cmd, &error_code);
    if (error_code != ICLI_SUCCESS)
    {
        fprintf(stderr, "Failed to register hello command: %s\n", icli_error_to_string(error_code));
        icli_command_destroy(hello_cmd);
        icli_destroy(cli);
        return 1;
    }

    /* Run the CLI */
    printf("Interactive CLI. Type 'help' for available commands, 'exit' to quit.\n");
    error_code = icli_run(cli, &error_code);
    if (error_code != ICLI_SUCCESS)
    {
        fprintf(stderr, "CLI encountered an error: %s\n", icli_error_to_string(error_code));
    }

    /* Cleanup */
    icli_destroy(cli);

    return 0;
}