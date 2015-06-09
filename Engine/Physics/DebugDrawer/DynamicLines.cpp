#include "DynamicLines.h"
#include "../../Common/MGDAssert.h"

//------------------------------------------------------------------------------------------------
DynamicLines::DynamicLines(OperationType opType)
{
	initialize(opType,false);
	setMaterial("BaseWhiteNoLighting");
	m_bDirty = true;
}
//------------------------------------------------------------------------------------------------
DynamicLines::~DynamicLines()
{
}
//------------------------------------------------------------------------------------------------
void DynamicLines::setOperationType(OperationType opType)
{
	mRenderOp.operationType = opType;
}
//------------------------------------------------------------------------------------------------
Ogre::RenderOperation::OperationType DynamicLines::getOperationType() const
{
	return mRenderOp.operationType;
}
//------------------------------------------------------------------------------------------------
void DynamicLines::addPoint(const Vec3 &p)
{
	m_Points.push_back(p);
	m_bDirty = true;
}
//------------------------------------------------------------------------------------------------
void DynamicLines::addPoint(real x, real y, real z)
{
	m_Points.push_back(Vec3(x,y,z));
	m_bDirty = true;
}
//------------------------------------------------------------------------------------------------
const Vec3& DynamicLines::getPoint(uint8 index) const
{
	MGD_ASSERT(index < m_Points.size() && "Point index is out of bounds!!");
	return m_Points[index];
}
//------------------------------------------------------------------------------------------------
uint8 DynamicLines::getNumPoints(void) const
{
	return (uint8)m_Points.size();
}
//------------------------------------------------------------------------------------------------
void DynamicLines::setPoint(uint8 index, const Vec3 &value)
{
	assert(index < m_Points.size() && "Point index is out of bounds!!");

	m_Points[index] = value;
	m_bDirty = true;
}
//------------------------------------------------------------------------------------------------
void DynamicLines::clear()
{
	m_Points.clear();
	m_bDirty = true;
}
//------------------------------------------------------------------------------------------------
void DynamicLines::update()
{
	if (m_bDirty) fillHardwareBuffers();
}
//------------------------------------------------------------------------------------------------
void DynamicLines::createVertexDeclaration()
{
	Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
	decl->addElement(POSITION_BINDING, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
}
//------------------------------------------------------------------------------------------------
void DynamicLines::fillHardwareBuffers()
{
	int size = m_Points.size();

	prepareHardwareBuffers(size,0);

	if (!size) {
		mBox.setExtents(Vec3::ZERO,Vec3::ZERO);
		m_bDirty=false;
		return;
	}

	Vec3 vaabMin = m_Points[0];
	Vec3 vaabMax = m_Points[0];

	Ogre::HardwareVertexBufferSharedPtr vbuf =
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

	real *prPos = static_cast<real*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
	{
		for(int i = 0; i < size; i++)
		{
			*prPos++ = m_Points[i].x;
			*prPos++ = m_Points[i].y;
			*prPos++ = m_Points[i].z;

			if(m_Points[i].x < vaabMin.x)
				vaabMin.x = m_Points[i].x;
			if(m_Points[i].y < vaabMin.y)
				vaabMin.y = m_Points[i].y;
			if(m_Points[i].z < vaabMin.z)
				vaabMin.z = m_Points[i].z;

			if(m_Points[i].x > vaabMax.x)
				vaabMax.x = m_Points[i].x;
			if(m_Points[i].y > vaabMax.y)
				vaabMax.y = m_Points[i].y;
			if(m_Points[i].z > vaabMax.z)
				vaabMax.z = m_Points[i].z;
		}
	}
	vbuf->unlock();

	mBox.setExtents(vaabMin, vaabMax);

	m_bDirty = false;
}
