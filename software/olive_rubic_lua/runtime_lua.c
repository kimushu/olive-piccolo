/*
 * runtime_lua.c
 *
 *  Created on: 2017/11/11
 *      Author: shuta
 */

#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "lauxlib.h"
#include "lualib.h"
#include "rubic_agent.h"

/**
 * @func runner
 * @brief Lua runner for Rubic agent
 */
static int runner(const char *data, int flags, void *agent_context)
{
    lua_State *L;
    int result;

    // Open stdio streams
    stdin = fdopen(STDIN_FILENO, "rb");
    stdout = fdopen(STDOUT_FILENO, "ab");
    stderr = fdopen(STDERR_FILENO, "ab");

    // Initialize Lua state
    L = luaL_newstate();
    if (!L) {
        result = -ENOMEM;
        fprintf(stderr, "Cannot allocate Lua state\n");
        goto cleanup;
    }
    /* [  ] */
    luaL_openlibs(L);

    // Compile source (Do not run here)
    if (flags & RUBIC_AGENT_RUNNER_FLAG_SOURCE) {
        result = luaL_loadstring(L, data);
    } else {
        result = luaL_loadfile(L, data);
    }
    if (result != 0) {
        /* [ error ] */
        fprintf(stderr, "Compilation error (%d): %s\n", result, lua_tostring(L, -1));
        lua_pop(L, 1);
        goto cleanup;
    }
    /* [ func ] */

    // Notify initialization complete
    result = rubic_agent_runner_notify_init(agent_context);
    if (result != 0) {
        goto cleanup;
    }

    printf("TESTTEST\n");
    sleep(1);

    // Start execution
    result = lua_pcall(L, 0, 0, 0);
    if (result != 0) {
        /* [ error ] */
        fprintf(stderr, "Runtime error (%d): %s\n", result, lua_tostring(L, -1));
        lua_pop(L, 1);
        goto cleanup;
    }

cleanup:
    if (L) {
        lua_close(L);
    }
    fclose(stdin);
    stdin = NULL;
    fclose(stdout);
    stdout = NULL;
    fclose(stderr);
    stderr = NULL;
    return result;
}

/**
 * @func runtime_lua_init
 * @brief Initialize runtime for Lua
 */
int runtime_lua_init(void)
{
    return rubic_agent_register_runtime("lua", LUA_RELEASE + 4, runner);
}
