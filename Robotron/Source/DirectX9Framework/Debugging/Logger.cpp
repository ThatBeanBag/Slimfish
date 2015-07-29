// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Logger.cpp
// Description	: CLogger implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Logger.h"

// Local Includes
#include "LogMessenger.h"

const Logger::ELogFlag g_keDEF_ERROR_FLAG = Logger::LOGFLAG_WRITE_TO_DEBUGGER;
const Logger::ELogFlag g_keDEF_WARNING_FLAG = Logger::LOGFLAG_WRITE_TO_DEBUGGER;
const Logger::ELogFlag g_keDEF_LOG_FLAG = Logger::LOGFLAG_WRITE_TO_DEBUGGER;

Logger::CErrorMessenger::CErrorMessenger()
	:m_bIsEnabled(true)
{

}

Logger::CErrorMessenger::~CErrorMessenger()
{

}

void Logger::CErrorMessenger::Show(const std::string& _krErrorMessage, bool _bIsFatal, const char* _pcFunction, const char* _pcFile, unsigned int _uiLineNum)
{
	if (m_bIsEnabled) {
		CLogMessenger* pLogger = CLogMessenger::GetInstance();

		if (pLogger) {
			if (pLogger->Error(_krErrorMessage, _bIsFatal, _pcFunction, _pcFile, _uiLineNum) == CLogMessenger::RESULT_IGNORE) {
				m_bIsEnabled = false;
			}
		}
	}
}

void Logger::InitialiseLogger(const std::string& _krLoggerConfigFile)
{
	CLogMessenger* pLogger = CLogMessenger::GetInstance();

	if (pLogger) {
		pLogger->Initialise(_krLoggerConfigFile);
	}
}

void Logger::Log(const std::string& _krstrTag, const std::string& _krstrWhat, const char* _pcFunction, const char* _pcFile, unsigned int _uiLineNum)
{
	CLogMessenger* pLogger = CLogMessenger::GetInstance();

	if (pLogger) {
		pLogger->Log(_krstrTag, _krstrWhat, _pcFunction, _pcFile, _uiLineNum);
	}
}

void Logger::SetDisplayFlags(const std::string& _krTag, ELogFlag _eFlags)
{
	CLogMessenger* pLogger = CLogMessenger::GetInstance();

	if (pLogger) {
		pLogger->SetDisplayFlags(_krTag, _eFlags);
	}
}