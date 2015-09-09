////////////////////////////////////////////////////////////////////////////
//
//  This file is part of OWEngine source code.
//  Copyright (C) 2006 Robert Beckebans <trebor_7@users.sourceforge.net>
//  Copyright (C) 2013 V.
//  Copyright (C) 2015 Dusan Jocic <dusanjocic@msn.com>
// 
//  OWEngine source code is free software; you can redistribute it
//  and/or modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//  
//  OWEngine source code is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA,
//  or simply visit <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------------
//  File name:   lua_entity.c
//  Version:     v1.00
//  Created:     
//  Compilers:   Visual Studio
//  Description: Entity library for Lua
// -------------------------------------------------------------------------
//  History: 
//
////////////////////////////////////////////////////////////////////////////

#ifdef G_ENABLE_LUA_SCRIPTING

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "g_lua.h"
#include "../g_local.h"
#include "../classes/BaseEntity.h"
#include <api/coreAPI.h>
#include <api/serverAPI.h>
#include <shared/array.h>
#include <shared/colorTable.h>

static int entity_Target(lua_State * L)
{
	/*lua_Entity     *lent;
	lua_Entity     *target;
	edict_s      *t = NULL;

	target = (lua_Entity*)lua_newuserdata(L, sizeof(lua_Entity));
	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	lent = lua_getentity(L, 1);


	if(!lent->e)
	{
		g_core->Print("entity_Target: invalid entity!\n");
		return 0;
	}
	if(!lent->e->target)
	{
		g_core->Print("entity_Target: no target!\n");
		return 0;
	}

	t = G_PickTarget(lent->e->target);
	if(!t)
	{
		G_Printf("entity_Target: Couldn't find target %s\n", lent->e->target);
		return 0;
	}

	target->e = t;*/

	return 1;


}

static int entity_Find(lua_State * L)
{
	char           *s;
	lua_Entity     *lent;
	int             i;
	edict_s      *t;

	lent = (lua_Entity*)lua_newuserdata(L, sizeof(lua_Entity));

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	s = (char *)luaL_checkstring(L, 1);

	lent->e = NULL;

	for(i = 0; i < level.num_entities; i++)
	{
		// Here we use tent to point to potential targets
		t = &g_entities[i];

		if(!t->ent)
			continue;

		if(Q_stricmp(t->ent->getTargetName(), s) == 0)
		{
			lent->e = t;
			break;
		}

	}

	if(!lent->e)
	{
		g_core->Print("entity_Find: entity '%s' not found!\n", s);
		return 0;
	}

	return 1;
}


static int entity_Teleport(lua_State * L)
{
	lua_Entity     *lent;
	lua_Entity     *target;


	lent = lua_getentity(L, 1);
	target = lua_getentity(L, 2);

	if(!lent->e)
	{
		g_core->Print("entity_Teleport: invalid entity!\n");
		return 0;
	}
	if(!target->e)
	{
		g_core->Print("entity_Teleport: invalid target!\n");
		return 0;
	}

	// TODO
	//lent->e->ent->teleportTo(target->e->ent);

	//if(lent->e->client)
	//	TeleportPlayer(lent->e, target->e->s.origin, target->e->s.angles);
	//else
	//	TeleportEntity(lent->e, target->e->s.origin, target->e->s.angles);
	return 1;
}



static int entity_IsRocket(lua_State * L)
{
	lua_Entity     *lent;
	bool        rocket = false;

	lent = lua_getentity(L, 1);

//	if(lent->e->classname == "rocket")
//		rocket = true;

	lua_pushboolean(L, rocket);

	return 1;
}
static int entity_IsGrenade(lua_State * L)
{
	lua_Entity     *lent;
	bool        grenade = false;

	lent = lua_getentity(L, 1);

//	if(lent->e->classname == "grenade")
//		grenade = true;

	lua_pushboolean(L, grenade);

	return 1;
}


static int entity_Spawn(lua_State * L)
{
	lua_Entity     *lent;

	lent = (lua_Entity*)lua_newuserdata(L, sizeof(lua_Entity));

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	const char *s = (char *)luaL_checkstring(L, 1);

	BaseEntity *be = G_SpawnClass(s);
	lent->e = be->getEdict();

	return 1;
}

static int entity_GetNumber(lua_State * L)
{
	lua_Entity     *lent;

	lent = lua_getentity(L, 1);
	lua_pushnumber(L, lent->e - g_entities);
//  lua_pushnumber(L, lent->e->s.number);

	return 1;
}

static int entity_IsClient(lua_State * L)
{
	lua_Entity     *lent;

	lent = lua_getentity(L, 1);
//	lua_pushboolean(L, lent->e->client != NULL);

	return 1;
}

static int entity_GetClientName(lua_State * L)
{
	lua_Entity     *lent;

	lent = lua_getentity(L, 1);
//	lua_pushstring(L, lent->e->client->pers.netname);

	return 1;
}

static int entity_Print(lua_State * L)
{
	lua_Entity     *lent;
	int             i;
	char            buf[MAX_STRING_CHARS];
	int             n = lua_gettop(L);	// number of arguments

	lent = lua_getentity(L, 1);
	//if(!lent->e->client)
	//	return luaL_error(L, "`Print' must be used with a client entity");

	memset(buf, 0, sizeof(buf));

	lua_getglobal(L, "tostring");
	for(i = 2; i <= n; i++)
	{
		const char     *s;

		lua_pushvalue(L, -1);	// function to be called
		lua_pushvalue(L, i);	// value to print
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);	// get result

		if(s == NULL)
			return luaL_error(L, "`tostring' must return a string to `print'");

		Q_strcat(buf, sizeof(buf), s);

		lua_pop(L, 1);			// pop result
	}

	g_server->SendServerCommand(lent->e - g_entities, va("print \"%s\n\"", buf));

	return 0;
}

static int entity_CenterPrint(lua_State * L)
{
	lua_Entity     *lent;
	int             i;
	char            buf[MAX_STRING_CHARS];
	int             n = lua_gettop(L);	// number of arguments

	lent = lua_getentity(L, 1);
//	if(!lent->e->client)
	///	return luaL_error(L, "`CenterPrint' must be used with a client entity");

	memset(buf, 0, sizeof(buf));

	lua_getglobal(L, "tostring");
	for(i = 2; i <= n; i++)
	{
		const char     *s;

		lua_pushvalue(L, -1);	// function to be called
		lua_pushvalue(L, i);	// value to print
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);	// get result

		if(s == NULL)
			return luaL_error(L, "`tostring' must return a string to `print'");

		Q_strcat(buf, sizeof(buf), s);

		lua_pop(L, 1);			// pop result
	}

	g_server->SendServerCommand(lent->e - g_entities, va("cp \"" S_COLOR_WHITE "%s\n\"", buf));

	return 0;
}

static int entity_GetClassName(lua_State * L)
{
//	lua_Entity     *lent;

//	lent = lua_getentity(L, 1);
//	lua_pushstring(L, lent->e->classname);

	return 1;
}

static int entity_SetClassName(lua_State * L)
{
//	lua_Entity     *lent;

//  char           *classname;

	//lent = lua_getentity(L, 1);
	//lent->e->classname = (char *)luaL_checkstring(L, 2);

//  lent->e->classname = classname;

	return 1;
}

static int entity_GetTargetName(lua_State * L)
{
	lua_Entity     *lent;

	lent = lua_getentity(L, 1);
//	lua_pushstring(L, lent->e->name);

	return 1;
}

static int entity_Rotate(lua_State * L)
{
	lua_Entity     *lent;
	vec_t          *vec;

	lent = lua_getentity(L, 1);
	vec = lua_getvector(L, 2);

	//lent->e->s.apos.trType = TR_LINEAR;
	//lent->e->s.apos.trDelta[0] = vec[0];
	//lent->e->s.apos.trDelta[1] = vec[1];
	//lent->e->s.apos.trDelta[2] = vec[2];

	return 1;
}

static int entity_GC(lua_State * L)
{
//  G_Printf("Lua says bye to entity = %p\n", lua_getentity(L));

	return 0;
}

static int entity_ToString(lua_State * L)
{
	lua_Entity     *lent;
	edict_s      *gent;
	char            buf[MAX_STRING_CHARS];

	lent = lua_getentity(L, 1);
	gent = lent->e;
	Com_sprintf(buf, sizeof(buf), "entity: class=%s name=%s id=%i pointer=%p\n", gent->ent->getClassName(), gent->ent->getTargetName(), gent - g_entities,
				gent);
	lua_pushstring(L, buf);

	return 1;
}

static int entity_SetRenderModel(lua_State * L)
{
	lua_Entity     *lent;

	lent = lua_getentity(L, 1);
	const char *modelName = (char *)luaL_checkstring(L, 2);

	lent->e->ent->setRenderModel(modelName);

	return 1;
}
// Custom Lua stack argument->reference function
// This function should always be called last! (Since it pops the lua stack)
static int luaM_toref (lua_State *L, int i) 
{
    int ref = -1;

    // convert the function pointer to a string so we can use it as index
    char buf[10] = {0};
    char * index = itoa ( (int)lua_topointer ( L, i ), buf, 16 );

    // get the callback table we made in CLuaMain::InitVM (at location 1)
    lua_getref ( L, 1 );
    lua_getfield ( L, -1, index );
    ref = lua_tonumber ( L, -1 );
    lua_pop ( L, 1 );
    lua_pop ( L, 1 );

    // if it wasn't added yet, add it to the callback table and the registry
    // else, get the reference from the table
    if ( !ref ) {
        // add a new reference (and get the id)
        lua_settop ( L, i );
        ref = lua_ref ( L, 1 );

        // and add it to the callback table
        lua_getref ( L, 1 );
        lua_pushstring ( L, index );
        lua_pushnumber ( L, ref );
        lua_settable ( L, -3 );
        lua_pop ( L, 1 );
    }
    return ref;
}
void G_RunLuaFunctionByRef(lua_State *L, int ref, const char *sig, ...);
static int entity_AddEventHandler(lua_State * L)
{
	lua_Entity     *lent;

	lent = lua_getentity(L, 1);
	const char *eventName = (char *)luaL_checkstring(L, 2);
	int checkArgType = lua_type ( L, 3 );
    if ( checkArgType != LUA_TFUNCTION ) {
		g_core->RedWarning("entity_AddEventHandler: event handler must be a function pointer\n");
		return 0;
    }
	int func = luaM_toref(L,3);

	if(lent->e->ent->addLuaEventHandler(L,eventName,func)) {

		return 0;
	}


	return 1;
}




static int entity_SetOrigin(lua_State * L)
{
	lua_Entity     *lent;
	vec_t          *vec;

	lent = lua_getentity(L, 1);
	vec = lua_getvector(L, 2);

	lent->e->ent->setOrigin(vec);

	return 1;
}
static const luaL_reg entity_ctor[] = {
	{"Spawn", entity_Spawn},

	{"Find", entity_Find},		//find an entity by name e.g ent = entity.Find("myentity");
	{"Target", entity_Target},	//find an entitys target, e.g target = entity.Target(ent);

	{NULL, NULL}
};

static const luaL_reg entity_meta[] = {
	{"__gc", entity_GC},
	{"__tostring", entity_ToString},
	{"GetNumber", entity_GetNumber},
	{"IsClient", entity_IsClient},
	{"GetClientName", entity_GetClientName},
	{"Print", entity_Print},
	{"CenterPrint", entity_CenterPrint},
	{"GetClassName", entity_GetClassName},
	{"SetClassName", entity_SetClassName},
	{"GetTargetName", entity_GetTargetName},
	{"Rotate", entity_Rotate},

	{"IsRocket", entity_IsRocket},
	{"IsGrenade", entity_IsGrenade},
	{"Teleport", entity_Teleport},

	{"SetOrigin", entity_SetOrigin},
	{"SetRenderModel", entity_SetRenderModel},
	{"AddEventHandler", entity_AddEventHandler},

	{NULL, NULL}
};

int luaopen_entity(lua_State * L)
{
	luaL_newmetatable(L, "game.entity");

	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);		// pushes the metatable
	lua_settable(L, -3);		// metatable.__index = metatable

	luaL_register(L, NULL, entity_meta);
	luaL_register(L, "entity", entity_ctor);

	return 1;
}

void lua_pushentity(lua_State * L, edict_s * ent)
{
	lua_Entity     *lent;

	lent = (lua_Entity*)lua_newuserdata(L, sizeof(lua_Entity));

	luaL_getmetatable(L, "game.entity");
	lua_setmetatable(L, -2);

	lent->e = ent;
}

lua_Entity     *lua_getentity(lua_State * L, int argNum)
{
	void           *ud;

	ud = luaL_checkudata(L, argNum, "game.entity");
	luaL_argcheck(L, ud != NULL, argNum, "`entity' expected");
	return (lua_Entity *) ud;
}

#endif
