#pragma once

#include "LuaPlus.h"
#include "../Common/Utilities.h"
#include "../Manager/LuaManager.h"

namespace LuaUtilities
{

	inline void ConvertVec3ToLuaObject(const Vec3& vec, LuaPlus::LuaObject& outLuaObject)
	{
		outLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());
		outLuaObject.SetNumber("x", vec.x);
		outLuaObject.SetNumber("y", vec.y);
		outLuaObject.SetNumber("z", vec.z);
	};

	inline void ConvertVec2ToLuaObject(const Vec2& vec, LuaPlus::LuaObject& outLuaObject)
	{
		outLuaObject.AssignNewTable(LuaManager::GetSingleton().GetLuaState());
		outLuaObject.SetNumber("x", vec.x);
		outLuaObject.SetNumber("y", vec.y);
	};


	inline void ConvertLuaObjectToVec3(const LuaPlus::LuaObject& i_oLuaObject, Vec3& outVec3)
	{
		LuaPlus::LuaObject temp;

		// x
		temp = i_oLuaObject.Get("x");
		//if (temp.IsNumber())
			outVec3.x = temp.GetFloat();

		// y
		temp = i_oLuaObject.Get("y");
		//if (temp.IsNumber())
			outVec3.y = temp.GetFloat();

		// z
		temp = i_oLuaObject.Get("z");
		//if (temp.IsNumber())
			outVec3.z = temp.GetFloat();
	};

	inline void ConvertLuaObjectToColor(const LuaPlus::LuaObject& i_oLuaObject, Ogre::ColourValue& outColor)
	{
		LuaPlus::LuaObject temp;

		// r
		temp = i_oLuaObject.Get("r");
		//if (temp.IsNumber())
		outColor.r = temp.GetFloat();

		// g
		temp = i_oLuaObject.Get("g");
		//if (temp.IsNumber())
		outColor.r = temp.GetFloat();

		// b
		temp = i_oLuaObject.Get("b");
		//if (temp.IsNumber())
		outColor.r = temp.GetFloat();

		// alpha
		temp = i_oLuaObject.Get("a");
		//if (temp.IsNumber())
		outColor.a = temp.GetFloat();
	};
}
