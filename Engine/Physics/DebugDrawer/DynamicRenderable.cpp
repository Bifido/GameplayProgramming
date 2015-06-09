#include "DynamicRenderable.h"
#include "OgreVertexIndexData.h"
#include "OgreHardwareVertexBuffer.h"
#include "OgreHardwareBufferManager.h"
#include "OgreMath.h"
#include "OgreHardwareBuffer.h"
#include "..\..\Common\Utilities.h"

DynamicRenderable::DynamicRenderable()
{
}
//------------------------------------------------------------------------------------------------
DynamicRenderable::~DynamicRenderable()
{
	delete mRenderOp.vertexData;
	delete mRenderOp.indexData;
}
//------------------------------------------------------------------------------------------------
void DynamicRenderable::initialize(Ogre::RenderOperation::OperationType operationType,
	bool useIndices)
{
	// Initialize render operation
	mRenderOp.operationType = operationType;
	mRenderOp.useIndexes = useIndices;
	mRenderOp.vertexData = new Ogre::VertexData;
	if (mRenderOp.useIndexes)
		mRenderOp.indexData = new Ogre::IndexData;

	// Reset buffer capacities
	mVertexBufferCapacity = 0;
	mIndexBufferCapacity = 0;

	// Create vertex declaration
	createVertexDeclaration();
}
//------------------------------------------------------------------------------------------------
void DynamicRenderable::prepareHardwareBuffers(size_t vertexCount,
	size_t indexCount)
{
	// Prepare vertex buffer
	size_t newVertCapacity = mVertexBufferCapacity;
	if ((vertexCount > mVertexBufferCapacity) ||
		(!mVertexBufferCapacity))
	{
		// vertexCount exceeds current capacity!
		// It is necessary to reallocate the buffer.

		// Check if this is the first call
		if (!newVertCapacity)
			newVertCapacity = 1;

		// Make capacity the next power of two
		while (newVertCapacity < vertexCount)
			newVertCapacity <<= 1;
	}
	else if (vertexCount < mVertexBufferCapacity>>1) {
		// Make capacity the previous power of two
		while (vertexCount < newVertCapacity>>1)
			newVertCapacity >>= 1;
	}
	if (newVertCapacity != mVertexBufferCapacity)
	{
		mVertexBufferCapacity = newVertCapacity;
		// Create new vertex buffer
		Ogre::HardwareVertexBufferSharedPtr vbuf =
			Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			mRenderOp.vertexData->vertexDeclaration->getVertexSize(0),
			mVertexBufferCapacity,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?

		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);
	}
	// Update vertex count in the render operation
	mRenderOp.vertexData->vertexCount = vertexCount;

	if (mRenderOp.useIndexes)
	{
		size_t newIndexCapacity = mIndexBufferCapacity;
		// Prepare index buffer
		if ((indexCount > newIndexCapacity) ||
			(!newIndexCapacity))
		{
			// indexCount exceeds current capacity!
			// It is necessary to reallocate the buffer.

			// Check if this is the first call
			if (!newIndexCapacity)
				newIndexCapacity = 1;

			// Make capacity the next power of two
			while (newIndexCapacity < indexCount)
				newIndexCapacity <<= 1;

		}
		else if (indexCount < newIndexCapacity>>1)
		{
			// Make capacity the previous power of two
			while (indexCount < newIndexCapacity>>1)
				newIndexCapacity >>= 1;
		}

		if (newIndexCapacity != mIndexBufferCapacity)
		{
			mIndexBufferCapacity = newIndexCapacity;
			// Create new index buffer
			mRenderOp.indexData->indexBuffer =
				Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
				Ogre::HardwareIndexBuffer::IT_16BIT,
				mIndexBufferCapacity,
				Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
		}

		// Update index count in the render operation
		mRenderOp.indexData->indexCount = indexCount;
	}
}
//------------------------------------------------------------------------------------------------
real DynamicRenderable::getBoundingRadius(void) const
{
	return Ogre::Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
}
//------------------------------------------------------------------------------------------------
real DynamicRenderable::getSquaredViewDepth(const Ogre::Camera* cam) const
{
	Vec3 vMin, vMax, vMid, vDist;
	vMin = mBox.getMinimum();
	vMax = mBox.getMaximum();
	vMid = ((vMax - vMin) * 0.5) + vMin;
	vDist = cam->getDerivedPosition() - vMid;

	return vDist.squaredLength();
}
