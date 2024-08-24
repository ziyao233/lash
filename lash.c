// SPDX-License-Identifier: MIT
/*
 *	lash
 *	lash.c
 *	Copyright (c) 2024 Yao Zi <ziyao at disroot dot org>
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glob.h>
#include <sys/wait.h>
#include <unistd.h>

#include <lua.h>
#include <lauxlib.h>

static int
raw_glob(lua_State *l)
{
	const char *pattern = luaL_checkstring(l, 1);
	glob_t matches;
	int ret = glob(pattern, 0, NULL, &matches);

	lua_createtable(l, matches.gl_pathc ,0);

	if (ret < 0)
		return 1;

	for (size_t i = 0; i < matches.gl_pathc; i++) {
		lua_pushstring(l, matches.gl_pathv[i]);
		lua_rawseti(l, -2, i + 1);
	}

	return 1;
}

static int
raw_exec(lua_State *l)
{
	int n = lua_gettop(l);
	const char *path = luaL_checkstring(l, 1);
	const char **argv = malloc(sizeof(char *) * (n + 1));
	if (!argv)
		luaL_error(l, "Out of Memory");

	for (int i = 0; i < n; i++) {
		argv[i] = lua_tostring(l, i + 1);
		if (!argv[i]) {
			free(argv);
			luaL_typeerror(l, i + 1, "string");
		}
	}
	argv[n] = NULL;

	int pid = fork();
	if (pid > 0) {
		int status, ret;

		do
			ret = waitpid(pid, &status, 0);
		while (ret < 0 && errno == EINTR);

		lua_pushboolean(l, !(!WIFEXITED(status) ||
				     WEXITSTATUS(status)));
	} else if (pid == 0) {
		if (execvp(path, (char * const *)argv)) {
			fprintf(stderr, "Cannot execute %s: %s\n",
				        path, strerror(errno));
			exit(-1);
		}
		// never reaches here
	} else {
		lua_pushboolean(l, 0);
	}

	free(argv);
	return 1;
}

static const luaL_Reg gLashFunc[] = {
	{ "glob", raw_glob },
	{ "exec", raw_exec },
	{ NULL, NULL },
};

int
luaopen_lash_raw(lua_State *l)
{
	luaL_newlib(l, gLashFunc);
	return 1;
}
