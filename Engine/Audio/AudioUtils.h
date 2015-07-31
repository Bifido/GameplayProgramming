#pragma once
#include "../Common/Utilities.h"
#include "AL\al.h"

namespace AudioUtils
{
	inline void ConvertVec3ToOpenALobj(const Vec3& vec, ALfloat* outALObject)
	{
		outALObject[0] = vec.x;
		outALObject[1] = vec.y;
		outALObject[2] = vec.z;
	}

	inline void ConvertVec3ToOpenALobj(const Vec3& vec, ALfloat* outALObject, int startIndex)
	{
		outALObject[startIndex + 0] = vec.x;
		outALObject[startIndex + 1] = vec.y;
		outALObject[startIndex + 2] = vec.z;
	}

	inline void ConvertQuatToOpenALobj(const Quaternion& quat, ALfloat* outALObject)
	{
		ConvertVec3ToOpenALobj(quat.xAxis(), outALObject, 0);
		ConvertVec3ToOpenALobj(quat.yAxis(), outALObject, 3);
	}

	inline void FillOpenALfloat3(float value, ALfloat* outALObject)
	{
		outALObject[0] = value;
		outALObject[1] = value;
		outALObject[2] = value;
	}

	inline void FillOpenALfloat6(float value, ALfloat* outALObject)
	{
		outALObject[0] = value;
		outALObject[1] = value;
		outALObject[2] = value;
		outALObject[3] = value;
		outALObject[4] = value;
		outALObject[5] = value;
	}

	const ALfloat ZeroALfloatVec3[] = { 0.0f, 0.0f, 0.0f };
	const ALfloat ZeroALfloatVec6[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

	inline const std::string GetALErrorString(ALenum error)
	{
		switch (error)
		{
		case AL_NO_ERROR:
			return std::string("AL_NO_ERROR");
			break;
		case AL_INVALID_NAME:
			return std::string("AL_INVALID_NAME");
			break;
		case AL_INVALID_ENUM:
			return std::string("AL_INVALID_ENUM");
			break;
		case AL_INVALID_VALUE:
			return std::string("AL_INVALID_VALUE");
			break;
		case AL_INVALID_OPERATION:
			return std::string("AL_INVALID_OPERATION");
			break;
		case AL_OUT_OF_MEMORY:
			return std::string("AL_OUT_OF_MEMORY");
			break;
		default:
			return std::string("AL_UNKNOW_ERROR: %d",error);
			break;
		}
	}
}