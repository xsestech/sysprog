#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <task1/user.h>

int user_manager_init(user_manager_t *manager)
{
    if (!manager)
    {
        return -1;
    }
    manager->user_count = 0;
    return 0;
}

static bool is_valid_login(const char *login)
{
    if (!login || strlen(login) > MAX_LOGIN_LENGTH)
    {
        return false;
    }
    for (size_t i = 0; login[i]; i++)
    {
        if (!isalnum(login[i]))
        {
            return false;
        }
    }
    return true;
}

static bool is_valid_pin(uint32_t pin)
{
    return pin <= 100000;
}

int user_manager_register(user_manager_t *manager, const char *login, uint32_t pin)
{
    if (!manager || !login || !is_valid_login(login) || !is_valid_pin(pin))
    {
        return -1;
    }

    if (manager->user_count >= MAX_USERS)
    {
        return -1;
    }

    // Check if user already exists
    for (size_t i = 0; i < manager->user_count; i++)
    {
        if (strcmp(manager->users[i].login, login) == 0)
        {
            return -1;
        }
    }

    user_t *new_user = &manager->users[manager->user_count++];
    strncpy(new_user->login, login, MAX_LOGIN_LENGTH);
    new_user->login[MAX_LOGIN_LENGTH] = '\0';
    new_user->pin = pin;
    new_user->request_limit = 0; // No limit by default
    new_user->current_requests = 0;

    return 0;
}

user_t *user_manager_auth(user_manager_t *manager, const char *login, uint32_t pin)
{
    if (!manager || !login)
    {
        return NULL;
    }

    for (size_t i = 0; i < manager->user_count; i++)
    {
        if (strcmp(manager->users[i].login, login) == 0 &&
            manager->users[i].pin == pin)
        {
            return &manager->users[i];
        }
    }
    return NULL;
}

int user_manager_set_limit(user_manager_t *manager, const char *username, uint32_t limit)
{
    if (!manager || !username)
    {
        return -1;
    }

    for (size_t i = 0; i < manager->user_count; i++)
    {
        if (strcmp(manager->users[i].login, username) == 0)
        {
            manager->users[i].request_limit = limit;
            manager->users[i].current_requests = 0;
            return 0;
        }
    }
    return -1;
}

bool user_can_make_request(user_t *user)
{
    if (!user)
    {
        return false;
    }
    return user->request_limit == 0 || user->current_requests < user->request_limit;
}

void user_increment_requests(user_t *user)
{
    if (!user || user->request_limit == 0)
    {
        return;
    }
    user->current_requests++;
}