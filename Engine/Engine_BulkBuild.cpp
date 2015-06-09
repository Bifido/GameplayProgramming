#if defined(_BULK)

//SCRIPT
#include "Script/ScriptSystem.cpp"
#include "Manager/LuaManager.cpp"
#include "Script/ScriptComponent.cpp"

//INPUT
#include "Input/InputManager3D.cpp"
#include "Input/InputManager2D.cpp"
#include "Input/InputManagerFactory.cpp"

//CORE
#include "Manager/SystemManager.cpp"
#include "Core/System.cpp"
//#include "Core/Actor.cpp"
#include "Core/Component.cpp"

//COMMON
#include "Common/MGDAssert.cpp"
#include "Common/TextureManager.cpp"

//EVENTS
#include "Event/EventRegistry.cpp"

//FSM
#include "FSM/FSM.cpp"
#include "FSM/State.cpp"
#include "FSM/Transition.cpp"
#include "FSM/FSMManager.cpp"

//LOG
#include "LOG/LOGManager.cpp"

//PHYSICS
#include "Physics/PhysicsSystem.cpp"
#include "Physics/PhysicsComponent.cpp"
#include "Physics/BulletWorld.cpp"
#include "Physics/BulletObject.cpp"
#include "Physics/MotionState.cpp"
#include "Physics/DebugDrawer/PhysicsDebugDrawer.cpp"
#include "Physics/DebugDrawer/DynamicLines.cpp"
#include "Physics/DebugDrawer/DynamicRenderable.cpp"
#include "Physics/Box2DObject.cpp"
#include "Physics/Box2DWorld.cpp"

//FACTORIES
#include "Factories/ActorFactory.cpp"
#include "Factories/FSMFactory.cpp"

//GRAPHICS
#include "Graphics/Renderer2D.cpp"
#include "Graphics/Renderer3D.cpp"
#include "Graphics/RendererFactory.cpp"
#include "Graphics/GfxComponent.cpp"
#include "Graphics/IGfxComponent.cpp"
#include "Graphics/CameraComponent.cpp"
#include "Graphics/LightComponent.cpp"
#include "Graphics/RenderingSystem.cpp"
#include "Graphics/MeshGfxComponent.cpp"
#include "Graphics/BaseGfxComponent.cpp"
#include "Graphics/SpriteComponent.cpp"

//RESOURCES
#include "Resources/ResourceManager.cpp"

//GUI
#include "GUI/GUISystem.cpp"
#include "GUI/GUIViewComponent.cpp"

//GUI WIDGTES
#include "GUI/Widgets/Interface/IGUIWidget.cpp"
#include "GUI/Widgets/Button.cpp"
#include "GUI/Widgets/StaticText.cpp"

//OTHER COMPONENTS
#include "OtherComponents/TransformationComponent.cpp"

//ANIMATION
#include "Animation/AnimatorComponent.cpp"
#include "Animation/AnimatorSystem.cpp"
#include "Animation/Motion.cpp"


#endif