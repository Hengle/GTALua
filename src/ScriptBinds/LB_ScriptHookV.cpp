// =================================================================================
// Includes 
// =================================================================================
#include "Includes.h"
#include "GTALua.h"
#include "lua/Lua.h"
#include "ScriptEngine/ScriptEngine.h"
#include "ScriptBinds.h"
#include "Memory/Memory.h"
#include "thirdparty/ScriptHookV/ScriptHookV.h"

// =================================================================================
// Sleep 
// =================================================================================
void LB_ThreadSleep(UINT s)
{
#ifndef GTA_LUA_TEST_EXE
	ScriptHook::ThreadWait(s);
#endif
}

// =================================================================================
// Init Native
// =================================================================================
void LB_InitNative(Natives::NativeReg* pNative)
{
	ScriptHook::InitNative(pNative->hHash);
}

vector<ScriptBinds::ScriptThread::LuaScriptThread*> vScriptThreadQueue;
void Lua_StartThread()
{
	printf("Lua_StartThread\n");
	ScriptBinds::ScriptThread::LuaScriptThread* pScriptThread = vScriptThreadQueue.at(0);
	vScriptThreadQueue.erase(vScriptThreadQueue.begin());

	pScriptThread->Run();
}

void LB_CreateThread(ScriptBinds::ScriptThread::LuaScriptThread* pThread)
{
	vScriptThreadQueue.push_back(pThread);
	ScriptHook::RegisterScript(GetModuleHandle("GTALua.dll"), Lua_StartThread);
}

// =================================================================================
// Bind 
// =================================================================================
void ScriptBinds::ScriptHookBind::Bind()
{
	// TODO: Add ability to register own threads
	luabind::module(lua->State(), "scripthookv")
	[
		luabind::def("CreateThread", LB_CreateThread),
		luabind::def("ThreadSleep", LB_ThreadSleep),
		luabind::def("InitNative", LB_InitNative),
		luabind::def("NativePushInt", ScriptHook::PushValue<int>),
		luabind::def("NativePushFloat", ScriptHook::PushValue<float>),
		luabind::def("NativePushVector", ScriptHook::PushValue<rage::CVector>),
		luabind::def("NativePushBool", ScriptHook::PushValue<bool>),
		luabind::def("NativeCall_GetInt", ScriptHook::Call<int>),
		luabind::def("NativeCall_GetFloat", ScriptHook::Call<float>),
		luabind::def("NativeCall_GetVector", ScriptHook::Call<rage::CVector>),
		luabind::def("NativeCall_GetBool", ScriptHook::Call<bool>)
	];
}