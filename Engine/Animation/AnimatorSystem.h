#pragma once
#include "../Core/System.h"
#include "../Common/Utilities.h"
#include "../Event/RegistryEventHandler.h"
#include <set>

class Component;
class SpriteComponent;
class TransformationComponent;
class Motion;
class MotionSpriteAnimation;
class MotionInterpolationPosition;
class MotionInterpolationRotation;
class MotionInterpolationScale;
class MotionAlphaBlend;

class AnimatorSystem : public System
{
public:
	ID_DECLARATION;

	AnimatorSystem();
	virtual ~AnimatorSystem();

	virtual void	Init();
	virtual void	Update(real i_fFrametime, real i_fTimestep);
	void UpdateMotions( real i_fTimestep );

	bool ManageSpriteAnimation(real i_fTimestep, MotionSpriteAnimation* i_pMotion, TransformationComponent* i_pTransformationComponent);
	void ManageInterpolationPosition(real i_fTimestep, MotionInterpolationPosition* i_pMotion, TransformationComponent* i_pTransformationComponent);
	void ManageInterpolationRotation(real i_fTimestep, MotionInterpolationRotation* i_pMotion, TransformationComponent* i_pTransformationComponent);
	void ManageInterpolationScale(real i_fTimestep, MotionInterpolationScale* i_pMotion, TransformationComponent* i_pTransformationComponent);
	void ManageAlphaBlend(MotionAlphaBlend* i_pMotion, SpriteComponent* i_pSpriteComponent);

private:

	struct MotionProcess
	{
		ObjectId m_oType;
		Motion* m_pMotion;
		Component* m_pComponent;
	};

	struct AnimatorComponentStruct
	{
		AnimatorComponentStruct( const ObjectId& i_oOwnerId, int32 i_iLayerIndex )
			: m_oOwnerId(i_oOwnerId)
			, m_iLayerIndex(i_iLayerIndex)
		{
		}



		MGDVector<MotionProcess>::iterator RemoveMotion(MGDVector<MotionProcess>::iterator it)
		{
			return m_vMotions.erase(it);
		}

		ObjectId m_oOwnerId;
		int32 m_iLayerIndex;
		MGDVector<MotionProcess> m_vMotions;
	};

	struct Sort
	{
		bool operator()(const AnimatorComponentStruct &foo1, const AnimatorComponentStruct &foo2)
		{
			return foo1.m_iLayerIndex < foo2.m_iLayerIndex;
		}
	};

	void DeleteMotions(const char* i_szName);
	void AddAnimatorComponent(const AnimatorComponentStruct& i_oElem);


	MGDVector<AnimatorComponentStruct> m_AnimatorComponents;
	RegistryEventHandler<AnimatorSystem, const char*> m_oUnregisterEvent;
	Sort m_oSorter;
};

