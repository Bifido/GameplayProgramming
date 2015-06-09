#pragma once
#include "../Common/DataTypes.h"
#include "../Core/Singleton.h"

static const ObjectId ACTOR_FACTORY_CONTEXT("ACTOR_FACTORY_CONTEXT");
static const ObjectId FSM_FACTORY_CONTEXT("FSM_FACTORY_CONTEXT");
static const ObjectId COMPONENTS_CONTEXT("COMPONENTS_CONTEXT");
static const ObjectId SYSTEMS_CONTEXT("SYSTEMS_CONTEXT");
static const ObjectId RENDERING_CONTEXT("RENDERING_CONTEXT");
static const ObjectId FSM_CONTEXT("FSM_CONTEXT");
static const ObjectId INPUT_CONTEXT("INPUT_CONTEXT");
static const ObjectId MANAGER_CONTEXT("MANAGER_CONTEXT");
static const ObjectId ASSERT_CONTEXT("ASSERT_CONTEXT");
static const ObjectId APPLICATION_CONTEXT("APPLICATION_CONTEXT");
static const ObjectId PHYSICS_CONTEXT("PHYSICS_CONTEXT");
static const ObjectId GUI_CONTEXT("GUI_CONTEXT");
static const ObjectId MAIN_CONTEXT("MAIN_CONTEXT");

namespace MGD_LOG
{
	enum LogSeverity
	{
		MGD_INFO = 0,
		MGD_WARNING,
		MGD_ERROR,
		MGD_FATAL,
		MGD_NUM_SEVERETIES,

	};

	class LOGManager : public Singleton<LOGManager>
	{
	public:
		static void CreateInstance();
		~LOGManager();

		void AddLogContext(const ObjectId& i_oContext)
		{
			m_LogContextAvailable.push_back(i_oContext);
		}

		void WriteLog(LogSeverity i_oLogSeverity, const ObjectId& i_oContext, const char* i_szMassage, ...);

	private:
		LOGManager();
		MGDVector<ObjectId> m_LogContextAvailable;

		static const char* m_szFileName;
	};
}