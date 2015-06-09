#pragma once
#include "LinearMath\btMotionState.h"
#include "../Common/DataTypes.h"
#include "Box2D/Dynamics/b2Body.h"
#include "../Common/Utilities.h"

class MotionState
{
public:
	MotionState(ObjectId i_oComponentName, ObjectId i_oOwnerId);
	virtual ~MotionState();

	ObjectId m_oComponentName;
	ObjectId m_oOwnerId;

};

class BulletMotionState : public MotionState, public btMotionState {
public:
	BulletMotionState(ObjectId i_oComponentName, ObjectId i_oOwnerId);
	virtual ~BulletMotionState();

	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);

};

class Box2DMotionState : public MotionState
{
public:
	Box2DMotionState(ObjectId i_oComponentName, ObjectId i_oOwnerId);
	virtual ~Box2DMotionState();

	virtual void getWorldTransform( b2Body& o_pBody ) const;
	virtual void setWorldTransform(const Vec2& o_oPos, real o_fAngle);
};
