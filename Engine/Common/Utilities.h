/*
* Commons utilities used in this engine
*/

#pragma once
#include "DataTypes.h"
#include "OgreMatrix4.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreVector4.h"
#include "OgreQuaternion.h"
#include "MGDAssert.h"
#include "OgreColourValue.h"

typedef Ogre::Matrix4		Matrix44;
typedef Ogre::Matrix3		Matrix33;
typedef Ogre::Vector2		Vec2;
typedef Ogre::Vector3		Vec3;
typedef Ogre::Vector4		Vec4;
typedef Ogre::Quaternion	Quaternion;
typedef Ogre::ColourValue	Color;

static void ConvertDecToHex(uint32 i_uiDec, char* o_Hex)
{
	if(!o_Hex)
	{
		MGD_ASSERT(0);
		return;
	}

	int c=10,c2=0,c3=0;  //counter variables
	const char letter[6]={65,66,67,68,69,70};
	const char number[10]={48,49,50,51,52,53,54,55,56,57};	
	while (i_uiDec>=1)
	{
		c=10;c2=0;
		if ((i_uiDec%16)<10)
		{
			while (c2!=i_uiDec%16)
			{
				c2++;
			}
			o_Hex[c3]=number[c2];
		}

		c2=0;
		if ((i_uiDec%16)>=10)
		{
			while (i_uiDec%16!=c)
			{
				c2++; // to get actual letter
				c++;  // to help get place of letter[c] in letter[6] array
			}
			o_Hex[c3]=letter[c2];
		}
		i_uiDec/=16;
		c3++;  // going on to next position in the array for the output of hex conversion
	}
	while (c3>=0)
	{
		// Array needs to be displayed backwards
		if (c3<=0)
			break;
		c3--;
		o_Hex[c3];
	}
}