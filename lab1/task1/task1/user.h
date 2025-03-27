#ifndef TASK1_USER_H
#define TASK1_USER_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_LOGIN_LENGTH 6
#define MAX_USERS 100

typedef struct
{
    char login[MAX_LOGIN_LENGTH + 1];
    uint32_t pin;
    uint32_t request_limit;
    uint32_t current_requests;
} user_t;

typedef struct
{
    user_t users[MAX_USERS];
    size_t user_count;
} user_manager_t;

/**
 * @brief Initialize user manager
 * @param manager Pointer to user manager structure
 * @return 0 on success, non-zero on error
 */
int user_manager_init(user_manager_t *manager);

/**
 * @brief Register new user
 * @param manager Pointer to user manager structure
 * @param login User login (max 6 chars)
 * @param pin User PIN code
 * @return 0 on success, non-zero on error
 */
int user_manager_register(user_manager_t *manager, const char *login, uint32_t pin);

/**
 * @brief Authenticate user
 * @param manager Pointer to user manager structure
 * @param login User login
 * @param pin User PIN code
 * @return Pointer to user on success, NULL on error
 */
user_t *user_manager_auth(user_manager_t *manager, const char *login, uint32_t pin);

/**
 * @brief Set request limit for user
 * @param manager Pointer to user manager structure
 * @param username Target username
 * @param limit New request limit
 * @return 0 on success, non-zero on error
 */
int user_manager_set_limit(user_manager_t *manager, const char *username, uint32_t limit);

/**
 * @brief Check if user can make more requests
 * @param user Pointer to user structure
 * @return true if user can make more requests, false otherwise
 */
bool user_can_make_request(user_t *user);

/**
 * @brief Increment user's request counter
 * @param user Pointer to user structure
 */
void user_increment_requests(user_t *user);

#endif // TASK1_USER_H