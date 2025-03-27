#ifndef TASK1_APP_STATE_H
#define TASK1_APP_STATE_H

#include "user.h"

typedef struct
{
    user_manager_t user_manager;
    user_t *current_user;
} app_state_t;

#endif // TASK1_APP_STATE_H