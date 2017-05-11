#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <stdio.h>

#if LUA_VERSION_NUM < 502
# ifndef luaL_newlib
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
# endif
#endif

#define pushfstring(buf, sz, e, fmt, ...) do{if(sz-e > 0) e+=snprintf(buf+e, sz-e, fmt, ##__VA_ARGS__);}while(0)

#define MAX_BUF_SZ (1024 * 1024)
#define MAX_BUF_SZ_MIN (8 * 1024)
#define MAX_FRAME_SZ 128

#define TOP_MAX 15
#define BOTTOM_MAX 10

#if LUA_VERSION_NUM >= 502
static int get_params(lua_State *L,
		      char nparams, char isvararg,
		      lua_Debug *ar,
		      char *buff,
		      size_t buff_sz)
{
	int i = 0;
	const char *name;
	size_t e = 0;
	while ((name = lua_getlocal(L, ar, i++)) != NULL) {
		e += snprintf(buff + e, buff_sz -e, "%s, ", name);
		lua_pop(L, 1);
	}
	if (e > 0)
		e -= 2;
	buff[e] = '\0';
	if (isvararg) {
		e += snprintf(buff + e, buff_sz - e, ", ...");
	}
	return e;
}
#endif

static size_t dump_lua_traceback(lua_State *L,
				 char *buf, size_t sz,
				 int is_show_var,
				 int top_max, int bottom_max)
{
	size_t e = 0;
	int level = 1;
	int firstpart = 1;
	lua_Debug ar;
	int i = 0;

	while (lua_getstack(L, level++, &ar)) {
		if (level > top_max && firstpart) {
			if (!lua_getstack(L, level+bottom_max, &ar))
				level--;
			else {
				pushfstring(buf, sz, e, "%s", "\n...");
				while (lua_getstack(L, level+bottom_max, &ar))
					level++;
			}
			firstpart = 0;
			continue;
		}
		pushfstring(buf, sz, e, "\n");
		lua_getinfo(L, "Snl", &ar);
		pushfstring(buf, sz, e, "[%d]%s:", level - 2, ar.short_src);
		if (ar.currentline > 0)
			pushfstring(buf, sz, e, "%d:", ar.currentline);
		if (*ar.namewhat != '\0') {
			char parambuf[256] = {0};
#if LUA_VERSION_NUM >= 502
			get_params(L, ar.nparams, ar.isvararg, parambuf, sizeof(parambuf));
#endif
			pushfstring(buf, sz, e, " in function %s(%s)", ar.name, parambuf);
		} else {
			if (*ar.what == 'm')
				pushfstring(buf, sz, e, " in main chunk");
			else if (*ar.what == 'C' || *ar.what == 't')
				pushfstring(buf, sz, e, " ?");
			else
				pushfstring(buf, sz, e, " in function <%s:%d>",
					    ar.short_src, ar.linedefined);
		}

		if (!lua_checkstack(L, 1)) /* to call lua_getlocal*/
			continue;

		i = 1;
		while(is_show_var) {
			const void *pointer;
			int type;
			lua_Debug arf;
			const char *name = lua_getlocal(L, &ar, i++);
			const char *typename;
			if (name == NULL)
				break;
			type = lua_type(L, -1);
			typename = lua_typename(L, type);
			pushfstring(buf, sz, e, "\n\t%s(%s) : ", name, typename);
			switch(type) {
			case LUA_TFUNCTION:
				pointer = lua_topointer(L, -1);
				lua_getinfo(L, ">Snl", &arf);
				if (*arf.what == 'C' || *arf.what == 't')
					pushfstring(buf, sz, e, "%p %s@C",
						    pointer,
						    (arf.name != NULL ? arf.name : "defined"));
				else
					pushfstring(buf, sz, e, "%p %s@%s:%d",
						    pointer,
						    (arf.name != NULL ? arf.name : "defined"),
						    arf.short_src, arf.linedefined);
				break;
			case LUA_TBOOLEAN:
				pushfstring(buf, sz, e, "%s", lua_toboolean(L, 1) ? "true" : "false");
				lua_pop(L, 1);
				break;
			case LUA_TNIL:
				pushfstring(buf, sz, e, "nil");
				lua_pop(L, 1);
				break;
			case LUA_TNUMBER:
			case LUA_TSTRING:
				pushfstring(buf, sz, e, "%s", lua_tostring(L, -1));
				lua_pop(L, 1);
				break;
			default:
				pushfstring(buf, sz, e, "%p", lua_topointer(L, -1));
				lua_pop(L, 1);
				break;
			}
		}
	}
	pushfstring(buf, sz, e, "\n");
	return e;
}

static int lua__traceback(lua_State *L)
{
	int is_show_var;
	int top_max, bottom_max;
	size_t buff_sz;
	char *buff = NULL;
	size_t len = 0;
	lua_State *LD = L;
	if (lua_isthread(L, 1)) {
		LD = lua_tothread(L, 1);
		lua_remove(L, 1);
	}
	is_show_var = lua_isnoneornil(L, 1) ? 1 : lua_toboolean(L, 1);
	buff_sz = luaL_optinteger(L, 2, MAX_BUF_SZ);
	if (buff_sz < MAX_BUF_SZ_MIN)
		buff_sz = MAX_BUF_SZ_MIN;
	top_max = luaL_optinteger(L, 3, TOP_MAX);
	bottom_max = luaL_optinteger(L, 4, BOTTOM_MAX);
	buff = malloc(buff_sz);
	if (buff == NULL)
		return 0;
	len = dump_lua_traceback(LD, buff, buff_sz, is_show_var, top_max, bottom_max);
	lua_pushlstring(L, buff, len);
	free(buff);
	return 1;
}

int luaopen_ltrace(lua_State* L)
{
	luaL_Reg lfuncs[] = {
		{"traceback", lua__traceback},
		{NULL, NULL},
	};
	luaL_newlib(L, lfuncs);
	return 1;
}

