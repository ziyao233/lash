// SPDX-License-Identifier: MIT
/*
 *	lash
 *	lash.c
 *	Copyright (c) 2024 Yao Zi <ziyao at disroot dot org>
 */

#include <stdio.h>

#include <glob.h>

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

static const luaL_Reg gLashFunc[] = {
	{ "glob", raw_glob },
	{ },
};

int
luaopen_lash_raw(lua_State *l)
{
	luaL_newlib(l, gLashFunc);
	return 1;
}
;
