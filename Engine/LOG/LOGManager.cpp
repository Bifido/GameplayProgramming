#include "LOGManager.h"
#include "glog\logging.h"
#include "tinyxml2.h"
#include <stdio.h>

namespace MGD_LOG
{
	const char* LOGManager::m_szFileName("Data/LogConfiguration.xml");

	void LOGManager::CreateInstance()
	{
		if(GetSingletonPtr() == NULL)
			new LOGManager();
	}

	LOGManager::LOGManager()
	{
		typedef tinyxml2::XMLDocument XMLDoc;
		typedef tinyxml2::XMLElement XMLElem;

		XMLDoc xmlContexts;
		tinyxml2::XMLError res = xmlContexts.LoadFile(m_szFileName);

		XMLElem* actualXMLElem;

		if (!res){
			actualXMLElem = xmlContexts.RootElement();

			if (actualXMLElem != nullptr && strcmp(actualXMLElem->Name(), "LogConfiguration") == 0){
				actualXMLElem = actualXMLElem->FirstChildElement("Context");

				while (actualXMLElem != nullptr){
					const char* actualValue = actualXMLElem->Attribute("value");

					if (actualValue != nullptr && strcmp(actualValue,"") != 0){ 
						ObjectId actualId(actualValue);

						AddLogContext(actualId);
					}

					actualXMLElem = actualXMLElem->NextSiblingElement("Context");
				} 
			}
		}
		//TODO -> Studente MGD
		
		/*	Funzione che aggiunge il contesto

			AddLogContext(CONTESTO_COME_OBJECTID);
		*/
	}

	LOGManager::~LOGManager()
	{
	}

	void LOGManager::WriteLog( LogSeverity i_oLogSeverity, const ObjectId& i_oContext, const char* i_szMassage, ...)
	{
		MGDVector<ObjectId>::iterator it = m_LogContextAvailable.begin();
		for(;it != m_LogContextAvailable.end();++it)
		{
			if( (*it) == i_oContext )
			{
				// ******** TODO-> improve this part of code
				va_list argList;
				std::string szMessage;
				va_start(argList, i_szMassage);
				const size_t nLength  = _vscprintf( i_szMassage, argList ) + 1;
				
				szMessage.resize(nLength);

				vsprintf_s(const_cast<char*>(szMessage.c_str()), nLength + 1, i_szMassage, argList);
				
				va_end(argList);
				// *******

				switch(i_oLogSeverity)
				{
				case MGD_INFO:
					{
						LOG(INFO) << szMessage;
						break;
					}
				case MGD_WARNING:
					{
						LOG(WARNING) << szMessage;
						break;
					}
				case MGD_ERROR:
					{
						LOG(ERROR) << szMessage;
						break;
					}
				case MGD_FATAL:
					{
						LOG(FATAL) << szMessage;
						break;
					}
				default:
					{
						LOG(ERROR) << "Incorrect LogSeverity!";
						break;
					}
				}
			}
		}
	}
}


