#include "MotionState.h"
#include "..\OtherComponents\TransformationComponent.h"
#include "../Manager\SystemManager.h"
#include "../Physics\PhysicsUtilities.h"

MotionState::MotionState(ObjectId i_oComponentName, ObjectId i_oOwnerId)
	: m_oComponentName(i_oComponentName)
	, m_oOwnerId(i_oOwnerId)
{
}

MotionState::~MotionState()
{
}

BulletMotionState::BulletMotionState(ObjectId i_oComponentName, ObjectId i_oOwnerId)
	: MotionState(i_oComponentName, i_oOwnerId)
{
}

BulletMotionState::~BulletMotionState()
{
}

void BulletMotionState::getWorldTransform( btTransform &worldTrans ) const
{
	const TransformationComponent* pTransformationComponent= NULL;
	const MGDVector<Component*>* pvComponents = SystemManager::GetSingleton().GetComponent(TransformationComponent::ID, m_oOwnerId);
	if(pvComponents)
	{
		MGD_ASSERT(pvComponents->size() == 1);

		MGDVector<Component*>::const_iterator itComponent = pvComponents->begin();
		for(; itComponent != pvComponents->end(); ++itComponent)
		{
			pTransformationComponent = static_cast<const TransformationComponent*>(*itComponent);
			if(pTransformationComponent)
			{
				btVector3& oPosition(worldTrans.getOrigin());
				btQuaternion& oRotation(worldTrans.getRotation());

				Physics::Vec3TobtVector3(pTransformationComponent->GetPosition(), oPosition);
				Physics::QuaternionTobtQuaternion(pTransformationComponent->GetRotation(), oRotation);			
			}
		}
	}
}

void BulletMotionState::setWorldTransform( const btTransform &worldTrans )
{
	TransformationComponent* pTransformationComponent= NULL;
	MGDVector<Component*>* pvComponents = SystemManager::GetSingleton().EditComponent(TransformationComponent::ID, m_oOwnerId);
	if(pvComponents)
	{
		MGD_ASSERT(pvComponents->size() == 1);

		MGDVector<Component*>::iterator itComponent = pvComponents->begin();
		for(; itComponent != pvComponents->end(); ++itComponent)
		{
			pTransformationComponent = static_cast<TransformationComponent*>(*itComponent);
			if(pTransformationComponent)
			{
				Vec3 oPos;
				Quaternion oRotation;
				Physics::btVector3ToVec3(worldTrans.getOrigin(), oPos);
				Physics::btQuaternionToQuaternion(worldTrans.getRotation(), oRotation);

				pTransformationComponent->SetPositionFromPhysics(oPos);
				pTransformationComponent->SetRotationFromPhysics(oRotation);			
			}
		}
	}
}


Box2DMotionState::Box2DMotionState(ObjectId i_oComponentName, ObjectId i_oOwnerId)
	: MotionState(i_oComponentName, i_oOwnerId)
{
}

Box2DMotionState::~Box2DMotionState()
{
}

void Box2DMotionState::getWorldTransform( b2Body& o_pBody ) const
{
	const TransformationComponent* pTransformationComponent= NULL;
	const MGDVector<Component*>* pvComponents = SystemManager::GetSingleton().GetComponent(TransformationComponent::ID, m_oOwnerId);
	if(pvComponents)
	{
		MGD_ASSERT(pvComponents->size() == 1);

		MGDVector<Component*>::const_iterator itComponent = pvComponents->begin();
		for(; itComponent != pvComponents->end(); ++itComponent)
		{
			pTransformationComponent = static_cast<const TransformationComponent*>(*itComponent);
			if(pTransformationComponent)
			{
				o_pBody.SetTransform(b2Vec2(pTransformationComponent->GetPosition().x, pTransformationComponent->GetPosition().y), 0.0f);

				//o_fAngle
			}
		}
	}
}

void Box2DMotionState::setWorldTransform( const Vec2& i_oPos, real i_fAngle )
{
	TransformationComponent* pTransformationComponent= NULL;
	MGDVector<Component*>* pvComponents = SystemManager::GetSingleton().EditComponent(TransformationComponent::ID, m_oOwnerId);
	if(pvComponents)
	{
		MGD_ASSERT(pvComponents->size() == 1);

		MGDVector<Component*>::iterator itComponent = pvComponents->begin();
		for(; itComponent != pvComponents->end(); ++itComponent)
		{
			pTransformationComponent = static_cast<TransformationComponent*>(*itComponent);
			if(pTransformationComponent)
			{
				pTransformationComponent->SetPositionFromPhysics(Vec3(i_oPos.x, i_oPos.y, 0.0f));
				//pTransformationComponent->SetRotationFromPhysics(o_fAngle);			
			}
		}
	}
}
