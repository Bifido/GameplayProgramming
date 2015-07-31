/*
*
*
*	Lua Functions:
*	
*	void SetPosition(Pos) -> Pos = LuaObject = { x = val, y = val, z = val}
*	LuaObject GetPosition() -> LuaObject = { x = val, y = val, z = val}
*	void Translate(LuaObject) -> LuaObject = { x = val, y = val, z = val}
*	viod LookAt(Point) -> Point = LuaObject = { x = val, y = val, z = val}
*	
*		
*/

#pragma once

#include "../Core/Component.h"

#include "../Common/Utilities.h"
#include "../Physics/MotionState.h"
#include "../Event/RegistryEventPublisher.h"

class TransformationComponent : public Component
{
public:
	ID_DECLARATION;

	friend class BulletMotionState;
	friend class Box2DMotionState;

	TransformationComponent(std::string i_szOwnerID, bool i_bIsCreatedFromTemplate = false);
	virtual ~TransformationComponent();

	virtual void			Init();
	virtual bool			SetupFromXml(const tinyxml2::XMLElement* pNode);
	virtual void			CreateFromTemplate(Component* i_pComponent, const ObjectId& i_oOwner)
	{
		if (i_pComponent)
		{
			TransformationComponent* i_pTransformationComponent = static_cast<TransformationComponent*>(i_pComponent);
			if (i_pTransformationComponent)
			{
				Component::CreateFromTemplate(i_oOwner);

				m_oOrientation = i_pTransformationComponent->GetRotation();
				m_oOrigin = i_pTransformationComponent->GetPosition();
				m_oScale = i_pTransformationComponent->GetScale();
			}
		}
	}

	Vec3					GetDirection()	const;
	void					SetDirection(Vec3& i_oDirection);
	Vec3					GetUp()			const;
	Vec3					GetSide()		const;

	Vec3					GetX()			const;
	Vec3					GetY()			const;
	Vec3					GetZ()			const;

	const Vec3&				GetPosition()	const;
	void					SetPosition(const Vec3& i_Position);
	void					Translate(const Vec3& i_Translation);
	
	const Quaternion&		GetRotation()	const;
	Matrix33				GetRotationMatrix() const;
	real					GetRotationX() const;
	real					GetRotationY() const;
	real					GetRotationZ() const;
	void					SetRotation(const Quaternion& i_Rotation);
	void					SetRotation(const Matrix33& i_Rotation);
	void					RotateAround(const Vec3& i_Axis, const real i_Angle);
	
	void					LookAt( const Vec3& i_oPoint );
	const Vec3&				GetScale()		const;
	void					SetScale(const Vec3& i_Scale);
	bool					HasScale()		const;


	//*********** LUA *********************
	//static LuaPlus::LuaObject CreateFromScript();
	static void RegisterScriptFunction();

	void SetPositionFromScript(LuaPlus::LuaObject i_oVec3LuaObject);
	LuaPlus::LuaObject GetPositionFromScript();
	void TranslateFromScript(LuaPlus::LuaObject i_oVec3LuaObject);
	void RotateFromScript(LuaPlus::LuaObject i_oVec3LuaObject, real i_fDegree);
	void LookAtFromScript(LuaPlus::LuaObject i_oVec3LuaObject);
	//************************************

private:

	void			SetPositionFromPhysics(const Vec3& i_Position);
	void			SetRotationFromPhysics(const Quaternion& i_oRotation);
	void			SetRotationFromPhysics(const Matrix33& i_oRotation);

	/***************** LUA *************/
	void			CreateLuaObject();
	/*********************************/
	RegistryEventPublisher<void>	m_oEventRot;
	RegistryEventPublisher<void>	m_oEventPos;

	Quaternion						m_oOrientation;	
	Vec3							m_oOrigin;
	Vec3							m_oScale;
};