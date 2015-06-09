#include "PhysicsDebugDrawer.h"
#include "OgreResourceGroupManager.h"
#include "OgreMaterialManager.h"
#include "..\PhysicsUtilities.h"
#include "../../LOG/LOGManager.h"
#include "../../Graphics/RendererFactory.h"
#include <gl/gl.h>
#include "Box2D/Collision/b2Collision.h"


PhysicsDebugDrawer::PhysicsDebugDrawer(btDynamicsWorld *i_pWorld)
	: m_pNode()
	, m_pWorld(i_pWorld)
	, m_bIsInDebug(false)
{
	Ogre::SceneManager* pScene = RendererFactory::GetRendererPtr()->GetScene();
	MGD_ASSERT(pScene);
	if(pScene)
	{
		m_pNode = pScene->getRootSceneNode();
	}

	m_pLineDrawer = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
	m_pNode->attachObject(m_pLineDrawer);

	if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists("BtOgre"))
		Ogre::ResourceGroupManager::getSingleton().createResourceGroup("BtOgre");
	if (!Ogre::MaterialManager::getSingleton().resourceExists("BtOgre/DebugLines"))
	{
		Ogre::MaterialPtr pMat = Ogre::MaterialManager::getSingleton().create("BtOgre/DebugLines", "BtOgre");
		pMat->setReceiveShadows(false);
		pMat->setSelfIllumination(1,1,1);
	}

	m_pLineDrawer->setMaterial("BtOgre/DebugLines");
}


PhysicsDebugDrawer::~PhysicsDebugDrawer(void)
{
	Ogre::MaterialManager::getSingleton().remove("BtOgre/DebugLines");
	Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("BtOgre");
	delete m_pLineDrawer;
}

void PhysicsDebugDrawer::step()
{
	if (m_bIsInDebug)
	{
		m_pWorld->debugDrawWorld();
		m_pLineDrawer->update();
		m_pNode->needUpdate();
		m_pLineDrawer->clear();
	}
	else
	{
		m_pLineDrawer->clear();
		m_pLineDrawer->update();
		m_pNode->needUpdate();
	}
}

void PhysicsDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	Vec3 oFrom;
	Vec3 oTo;
	Physics::btVector3ToVec3(from,oFrom);
	Physics::btVector3ToVec3(to,oTo);
	m_pLineDrawer->addPoint(oFrom);
	m_pLineDrawer->addPoint(oTo);
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{

	Vec3 oPointOnB;
	Vec3 oNormalOnB;
	Physics::btVector3ToVec3(PointOnB,oPointOnB);
	Physics::btVector3ToVec3(normalOnB,oNormalOnB);
	m_pLineDrawer->addPoint(oPointOnB);
	m_pLineDrawer->addPoint(oPointOnB + (oNormalOnB * distance * 20));
}

void PhysicsDebugDrawer::reportErrorWarning(const char* i_szWarningString)
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_WARNING, PHYSICS_CONTEXT, i_szWarningString);	
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location,const char* textString)
{
}



//BOX2D
//#ifdef __APPLE__
//#include <GLUT/glut.h>
//#else
#include "freeglut/freeglut.h"
//#endif

#include <stdio.h>
#include <stdarg.h>

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();

	b2Vec2 p = center + radius * axis;
	glBegin(GL_LINES);
	glVertex2f(center.x, center.y);
	glVertex2f(p.x, p.y);
	glEnd();
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	glVertex2f(p2.x, p2.y);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	glVertex2f(p2.x, p2.y);

	glEnd();
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p.x, p.y);
	glEnd();
	glPointSize(1.0f);
}

void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
	char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf_s(buffer, string, arg);
	va_end(arg);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	//gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buffer[i]);
		//glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buffer[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void DebugDraw::DrawString(const b2Vec2& p, const char *string, ...)
{
	char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf_s(buffer, string, arg);
	va_end(arg);

	glColor3f(0.5f, 0.9f, 0.5f);
	glRasterPos2f(p.x, p.y);

	int32 length = (int32)strlen(buffer);
	for (int32 i = 0; i < length; ++i)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	}

	glPopMatrix();
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb->lowerBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->upperBound.y);
	glVertex2f(aabb->lowerBound.x, aabb->upperBound.y);
	glEnd();
}
