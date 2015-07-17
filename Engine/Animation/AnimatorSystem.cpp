#include "AnimatorSystem.h"
#include "..\LOG\LOGManager.h"
#include "Motion.h"
#include "..\Common\TextureManager.h"
#include "AnimatorComponent.h"
#include "..\Manager\SystemManager.h"
#include "..\OtherComponents\TransformationComponent.h"
#include "..\Graphics\SpriteComponent.h"

ID_IMPLEMENTATION(AnimatorSystem);

AnimatorSystem::AnimatorSystem()
	:System()
{
	m_oUnregisterEvent.Subscribe(this,&AnimatorSystem::DeleteMotions,ObjectId("Test/Unregistered/AnimatorComponent"));

	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "AnimatorSystem created!");
}

AnimatorSystem::~AnimatorSystem()
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "AnimatorSystem destroyed!");
}

void AnimatorSystem::Init()
{
	MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "AnimatorSystem initialized!");
}

void AnimatorSystem::DeleteMotions(const char* i_szName)
{
	const EntityComponentTable* pAnimatorComponentMap=  SystemManager::GetSingleton().GetComponentTable(AnimatorComponent::ID);
	if(pAnimatorComponentMap)
	{
		for(EntityComponentTable::const_iterator ecIt = pAnimatorComponentMap->begin(); ecIt != pAnimatorComponentMap->end(); ++ecIt)
		{
			const MGDVector<Component*>& vComponents(ecIt->second);
			MGDVector<Component*>::const_iterator itComponent = vComponents.begin();
			for(; itComponent != vComponents.end(); ++itComponent)
			{
				if((*itComponent)->GetTextName().compare(i_szName) == 0)
				{
					AnimatorComponent* pAnimatorComponent= static_cast<AnimatorComponent*>(*itComponent);
					if(pAnimatorComponent)
					{
						std::vector<AnimatorComponentStruct>::iterator it = m_AnimatorComponents.begin();
						while(it != m_AnimatorComponents.end())
						{
							if(it->m_oOwnerId == pAnimatorComponent->GetOwnerID())
							{
								it = m_AnimatorComponents.erase(it);
							}
							else
							{
								++it;
							}
						}
					}	
				}
			}		
		}
	}
}


bool AnimatorSystem::ManageSpriteAnimation(real i_fTimestep, MotionSpriteAnimation* i_pMotion, TransformationComponent* i_pTransformationComponent)
{
	if(i_pMotion && i_pTransformationComponent)
	{
		TextureManager::Inst()->BindTexture(ObjectId(i_pMotion->m_szPath).GetId());

		i_pMotion->m_fCurrentTime += (i_fTimestep * 1000.0f);
		if(i_pMotion->m_fCurrentTime > i_pMotion->m_fDuration)
		{
			i_pMotion->m_uiCurrentImage++;

			if (i_pMotion->m_uiCurrentImage >= i_pMotion->m_uiNumImages)
			{
				if(i_pMotion->m_bLoop)
				{
					i_pMotion->m_uiCurrentImage = 0;
				}
				else
				{
					i_pMotion->Reset();
					return false;
				}
			}
			i_pMotion->m_fCurrentTime = 0.0f;
		}

		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glEnable( GL_TEXTURE_2D );        // Need this to display a texture
		//glDisable(GL_DEPTH_TEST);         //Turn Depth Testing Off
		
		real portion = i_pMotion->SingleImageDimesion[0] / i_pMotion->TotalDimesion[0];

		const Vec3& oPos(i_pTransformationComponent->GetPosition());
	

		glPushMatrix();
		glTranslatef(oPos.x, oPos.y, oPos.z);
		glRotatef(i_pTransformationComponent->GetRotationZ(),0,0,1);
		glBegin (GL_QUADS);
		if(i_pMotion->IsFlipped())
		{
			glTexCoord2f (portion * i_pMotion->m_uiCurrentImage + portion, 1.0f); /* upper right corner of image */
			glVertex3f (-i_pMotion->m_Size[0]/2, -i_pMotion->m_Size[1]/2, 0.0f);
			glTexCoord2f (portion * i_pMotion->m_uiCurrentImage, 1.0f); /* upper left corner of image */
			glVertex3f (i_pMotion->m_Size[0]/2, -i_pMotion->m_Size[1]/2, 0.0f);
			glTexCoord2f (portion * i_pMotion->m_uiCurrentImage,0.0f); /* lower left corner of image */
			glVertex3f (i_pMotion->m_Size[0]/2, i_pMotion->m_Size[1]/2, 0.0f);
			glTexCoord2f (portion * i_pMotion->m_uiCurrentImage + portion, 0.0f); /* lower right corner of image */
			glVertex3f (-i_pMotion->m_Size[0]/2, i_pMotion->m_Size[1]/2, 0.0f);
		}
		else
		{
			glTexCoord2f (portion * i_pMotion->m_uiCurrentImage, 1.0f); /* upper left corner of image */
			glVertex3f (-i_pMotion->m_Size[0]/2, -i_pMotion->m_Size[1]/2, 0.0f);
			glTexCoord2f (portion * i_pMotion->m_uiCurrentImage + portion, 1.0f); /* upper right corner of image */
			glVertex3f (i_pMotion->m_Size[0]/2, -i_pMotion->m_Size[1]/2, 0.0f);
			glTexCoord2f (portion * i_pMotion->m_uiCurrentImage + portion, 0.0f); /* lower right corner of image */
			glVertex3f (i_pMotion->m_Size[0]/2, i_pMotion->m_Size[1]/2, 0.0f);
			glTexCoord2f (portion * i_pMotion->m_uiCurrentImage,0.0f); /* lower left corner of image */
			glVertex3f (-i_pMotion->m_Size[0]/2, i_pMotion->m_Size[1]/2, 0.0f);
		}
		glEnd ();
		glPopMatrix();

		glDisable( GL_TEXTURE_2D );        // Need this to display a texture
		//glDisable(GL_BLEND);                  // Turn Blending On
		//glEnable(GL_DEPTH_TEST);         //Turn Depth Testing Off

		//glBindTexture(GL_TEXTURE_2D,0);
	}

	return true;
}

void AnimatorSystem::ManageInterpolationPosition(real i_fTimestep, MotionInterpolationPosition* i_pMotion, TransformationComponent* i_pTransformationComponent)
{
	if(i_pMotion && i_pTransformationComponent)
	{

		Vec3 oStartPosition = Vec3(i_pMotion->GetStartPosition().x,i_pMotion->GetStartPosition().y,0);
		Vec3 oEndPosition = Vec3(i_pMotion->GetTargetPosition().x,i_pMotion->GetTargetPosition().y,0);
		real duration = i_pMotion->m_fDuration;

		Vec3 distance = oEndPosition - oStartPosition;
		Vec3 velocity = distance / duration;

		if(abs(i_pTransformationComponent->GetPosition().distance(oEndPosition)) > 0.01f)
		{
			i_pTransformationComponent->Translate(velocity * (i_fTimestep*1000));
		}
		else
		{
			if(i_pMotion->m_bLoop)
			{
				i_pTransformationComponent->SetPosition(oStartPosition);
			}
		}
	}
}

void AnimatorSystem::ManageInterpolationRotation(real i_fTimestep, MotionInterpolationRotation* i_pMotion, TransformationComponent* i_pTransformationComponent)
{
	if(i_pMotion && i_pTransformationComponent)
	{
		real fStartRotation = i_pMotion->GetStartRotation();
		real fEndRotation = i_pMotion->GetTargetRotation();
		real duration = i_pMotion->m_fDuration;

		real fDegree = fEndRotation - fStartRotation;
		real fAngularVelocity = fDegree;

		if(abs(i_pTransformationComponent->GetRotationZ() - fEndRotation) > 0.01f)
		{
			i_pTransformationComponent->RotateAround(Vec3(0,0,1), fAngularVelocity * i_fTimestep);
		}
		else
		{
			if(i_pMotion->m_bLoop)
			{
				i_pTransformationComponent->SetRotation(Matrix33::IDENTITY);
			}
		}
	}
}

void AnimatorSystem::ManageInterpolationScale(real i_fTimestep, MotionInterpolationScale* i_pMotion, TransformationComponent* i_pTransformationComponent)
{
	if(i_pMotion && i_pTransformationComponent)
	{
		Vec3 oStartScale = Vec3(i_pMotion->GetStartScale().x,i_pMotion->GetStartScale().y,1);
		Vec3 oEndScale = Vec3(i_pMotion->GetTargetScale().x,i_pMotion->GetTargetScale().y,1);
		real duration = i_pMotion->m_fDuration;

		Vec3 distance = oEndScale - oStartScale;
		Vec3 velocity = distance / duration;

		if(abs(i_pTransformationComponent->GetScale().distance(oEndScale)) > 0.01f)
		{
			i_pTransformationComponent->SetScale(i_pTransformationComponent->GetScale() + (velocity * (i_fTimestep*1000)));
		}
		else
		{
			if(i_pMotion->m_bLoop)
			{
				i_pTransformationComponent->SetScale(oStartScale);
			}
		}
	}
}

void AnimatorSystem::ManageAlphaBlend(MotionAlphaBlend* i_pMotion, SpriteComponent* i_pSpriteComponent)
{
	if(i_pMotion && i_pSpriteComponent)
	{
		//oxygine::spSprite pSprite = i_pSpriteComponent->GetSprite();
		//if(pSprite)
		//{
		//	oxygine::Actor::TweenAlpha oTweenAlpha(i_pMotion->GetTargetAlpha());
		//	pSprite->addTween(oTweenAlpha,i_pMotion->m_fDuration, i_pMotion->m_bLoop);
		//}
	}
}

void AnimatorSystem::UpdateMotions( real i_fTimestep )
{

	std::vector<AnimatorComponentStruct>::iterator it = m_AnimatorComponents.begin();
	while(it != m_AnimatorComponents.end())
	{
		MGDVector<MotionProcess>::iterator itVec = const_cast<AnimatorComponentStruct&>(*it).m_vMotions.begin();
		while(itVec != it->m_vMotions.end())
		{
			if((*itVec).m_oType == MotionInterpolationPosition::ID)
			{
				ManageInterpolationPosition(i_fTimestep, static_cast<MotionInterpolationPosition*>((*itVec).m_pMotion), static_cast<TransformationComponent*>((*itVec).m_pComponent));
				++itVec;
				//delete motion

			}
			else if((*itVec).m_oType == MotionInterpolationRotation::ID)
			{
				ManageInterpolationRotation(i_fTimestep, static_cast<MotionInterpolationRotation*>((*itVec).m_pMotion), static_cast<TransformationComponent*>((*itVec).m_pComponent));
				++itVec;
			}
			else if((*itVec).m_oType == MotionInterpolationScale::ID)
			{
				ManageInterpolationScale(i_fTimestep, static_cast<MotionInterpolationScale*>((*itVec).m_pMotion), static_cast<TransformationComponent*>((*itVec).m_pComponent));
				++itVec;
			}
			else if((*itVec).m_oType == MotionSpriteAnimation::ID)
			{
				if(!ManageSpriteAnimation(i_fTimestep, static_cast<MotionSpriteAnimation*>((*itVec).m_pMotion), static_cast<TransformationComponent*>((*itVec).m_pComponent)))
				{
					itVec = const_cast<AnimatorComponentStruct&>(*it).RemoveMotion(itVec);
				}
				else
				{
					++itVec;
				}
			}
			else
			{
				itVec = const_cast<AnimatorComponentStruct&>(*it).RemoveMotion(itVec);
				MGD_ASSERT(0);
			}
		}


		if(it->m_vMotions.empty())
		{
			it = m_AnimatorComponents.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void AnimatorSystem::Update(real /*i_fFrametime*/, real i_fTimestep)
{
	const EntityComponentTable* pAnimatorComponentMap=  SystemManager::GetSingleton().GetComponentTable(AnimatorComponent::ID);
	if(pAnimatorComponentMap == NULL)
	{	
		MGD_LOG::LOGManager::GetSingleton().WriteLog(MGD_LOG::MGD_INFO, SYSTEMS_CONTEXT, "No AnimatorComponentMap to render");		
	}
	else
	{
		for(EntityComponentTable::const_iterator ecIt= pAnimatorComponentMap->begin(); ecIt != pAnimatorComponentMap->end(); ++ecIt)
		{
			AnimatorComponent* pAnimatorComponent = static_cast<AnimatorComponent*>(SystemManager::GetSingleton().EditSingleComponent(AnimatorComponent::ID, ecIt->first));
			if(pAnimatorComponent )
			{
				if(pAnimatorComponent->DoingTransition())
				{
					AnimatorComponentStruct oStruct(pAnimatorComponent->GetOwnerID(), pAnimatorComponent->GetLayerIndex());

					MGDVector<Motion*>& vMotions = pAnimatorComponent->GetMotions();
					for(uint32 uiIndex = 0; uiIndex < vMotions.size(); ++uiIndex)
					{
						MotionProcess oMotionProcess;

						if(vMotions[uiIndex]->GetID() == MotionSpriteAnimation::ID)
						{
							Component* pComponent= SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, pAnimatorComponent->GetOwnerID());
							TransformationComponent* pTransformationComponent = static_cast<TransformationComponent*>(pComponent);
							if(pTransformationComponent)
							{

								MotionSpriteAnimation* pMotion = static_cast<MotionSpriteAnimation*>(vMotions[uiIndex]);
								MGD_ASSERT(pMotion);
								if(pMotion)
								{
									oMotionProcess.m_oType = MotionSpriteAnimation::ID;
									oMotionProcess.m_pMotion = pMotion;
									oMotionProcess.m_pComponent = pTransformationComponent;
									//ManageSpriteAnimation(i_fTimestep, pMotion, pTransformationComponent);
								}
							}
						}
						else if(vMotions[uiIndex]->GetID() == MotionInterpolationPosition::ID)
						{
							Component* pComponent= SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, pAnimatorComponent->GetOwnerID());
							TransformationComponent* pTransformationComponent = static_cast<TransformationComponent*>(pComponent);
							if(pTransformationComponent)
							{
								MotionInterpolationPosition* pMotion = static_cast<MotionInterpolationPosition*>(vMotions[uiIndex]);
								MGD_ASSERT(pMotion);
								if(pMotion)
								{
									oMotionProcess.m_oType = MotionInterpolationPosition::ID;
									oMotionProcess.m_pMotion = pMotion;
									oMotionProcess.m_pComponent = pTransformationComponent;
									//ManageInterpolationPosition(pMotion, pTransformationComponent);
								}
							}
						}
						else if(vMotions[uiIndex]->GetID() == MotionInterpolationRotation::ID)
						{
							Component* pComponent= SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, pAnimatorComponent->GetOwnerID());
							TransformationComponent* pTransformationComponent = static_cast<TransformationComponent*>(pComponent);
							if(pTransformationComponent)
							{
								MotionInterpolationRotation* pMotion = static_cast<MotionInterpolationRotation*>(vMotions[uiIndex]);
								MGD_ASSERT(pMotion);
								if(pMotion)
								{
									oMotionProcess.m_oType = MotionInterpolationRotation::ID;
									oMotionProcess.m_pMotion = pMotion;
									oMotionProcess.m_pComponent = pTransformationComponent;
									//ManageInterpolationRotation(pMotion, pTransformationComponent);
								}
							}
						}
						else if(vMotions[uiIndex]->GetID() == MotionInterpolationScale::ID)
						{
							Component* pComponent= SystemManager::GetSingleton().EditSingleComponent(TransformationComponent::ID, pAnimatorComponent->GetOwnerID());
							TransformationComponent* pTransformationComponent = static_cast<TransformationComponent*>(pComponent);
							if(pTransformationComponent)
							{
								MotionInterpolationScale* pMotion = static_cast<MotionInterpolationScale*>(vMotions[uiIndex]);
								MGD_ASSERT(pMotion);
								if(pMotion)
								{
									oMotionProcess.m_oType = MotionInterpolationScale::ID;
									oMotionProcess.m_pMotion = pMotion;
									oMotionProcess.m_pComponent = pTransformationComponent;
									//ManageInterpolationScale(pMotion, pTransformationComponent);
								}
							}
						}
						else if(vMotions[uiIndex]->GetID() == MotionAlphaBlend::ID)
						{
							Component* pComponent= SystemManager::GetSingleton().EditSingleComponent(SpriteComponent::ID, pAnimatorComponent->GetOwnerID());
							SpriteComponent* pSpriteComponent = static_cast<SpriteComponent*>(pComponent);
							if(pSpriteComponent)
							{
								MotionAlphaBlend* pMotion = static_cast<MotionAlphaBlend*>(vMotions[uiIndex]);
								MGD_ASSERT(pMotion);
								if(pMotion)
								{
									oMotionProcess.m_oType = MotionAlphaBlend::ID;
									oMotionProcess.m_pMotion = pMotion;
									oMotionProcess.m_pComponent = pSpriteComponent;
									//ManageAlphaBlend(pMotion, NULL);
								}
							}
						}

						oStruct.m_vMotions.push_back(oMotionProcess);
					}

					AddAnimatorComponent(oStruct);
				}

				pAnimatorComponent->TransitionComplete();
			}
		}

	}

	UpdateMotions(i_fTimestep);
}

void AnimatorSystem::AddAnimatorComponent(const AnimatorComponentStruct& i_oElem)
{
	bool bFound = false;
	MGDVector<AnimatorComponentStruct>::iterator it = m_AnimatorComponents.begin();
	while(it != m_AnimatorComponents.end() && !bFound)
	{
		if(it->m_oOwnerId == i_oElem.m_oOwnerId)
		{
			bFound = true;
			*it = i_oElem;
		}

		++it;
	}

	if(!bFound)
	{
		m_AnimatorComponents.push_back(i_oElem);
	}

	std::sort(m_AnimatorComponents.begin(), m_AnimatorComponents.end(), m_oSorter);
}