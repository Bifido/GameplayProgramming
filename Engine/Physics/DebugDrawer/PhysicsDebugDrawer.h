#pragma once
#include "LinearMath\btIDebugDraw.h"
#include "BulletDynamics\Dynamics\btDynamicsWorld.h"
#include "OgreSceneNode.h"
#include "DynamicLines.h"
#include "Box2D\Common\b2Draw.h"

class PhysicsDebugDrawer : public btIDebugDraw
{
public:
	PhysicsDebugDrawer(btDynamicsWorld *world);
	~PhysicsDebugDrawer();

	virtual void setDebugMode(int i_iIsEnabled)
	{
		m_bIsInDebug = (i_iIsEnabled == 0) ? false : true;

		if (!m_bIsInDebug)
			m_pLineDrawer->clear();
	}

	//0 for off, anything else for on.
	virtual int	getDebugMode() const
	{
		return m_bIsInDebug;
	}
	
	virtual void step();
	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
	virtual void reportErrorWarning(const char* i_szWarningString);
	virtual void draw3dText(const btVector3& location,const char* textString);
	
private:
	Ogre::SceneNode*	m_pNode;
	btDynamicsWorld*	m_pWorld;
	DynamicLines*		m_pLineDrawer;
	bool				m_bIsInDebug;
};



// BOX2D
struct b2AABB;

class DebugDraw : public b2Draw
{
public:
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	void DrawTransform(const b2Transform& xf);

	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

	void DrawString(int x, int y, const char* string, ...); 

	void DrawString(const b2Vec2& p, const char* string, ...);

	void DrawAABB(b2AABB* aabb, const b2Color& color);
};
