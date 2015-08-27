// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimLogger.cpp
// Description	: CSlimLogger implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimLogger.h"

// Local Includes

namespace Slim {

	std::unique_ptr<CLogger> ASingleton<CLogger>::m_pSingleton = nullptr;

	CLogger::~CLogger()
	{

	}

	void CLogger::CreateLog(const std::string& filename, bool outputToDebug /*= true*/, ELogLevel loggingLevel /*= LOG_INFO*/, bool setAsCurrent /*= true*/)
	{
		std::unique_ptr<CLog> pLog(new CLog(filename, outputToDebug, loggingLevel));

		if (setAsCurrent) {
			m_pCurrentLog = pLog.get();
		}

		m_Logs[filename] = std::move(pLog);
	}

	void CLogger::LogMessage(const std::string& message, ELogLevel loggingLevel)
	{
		assert(m_pCurrentLog);
		m_pCurrentLog->LogMessage(message, loggingLevel);
	}

	bool CLogger::SetCurrentLog(const std::string& name)
	{
		CLog* pLog = FindLog(name);
		if (pLog) {
			m_pCurrentLog = pLog;
		}
		else {
			return false;
		}

		return true;
	}

	void CLogger::SetCurrentLog(CLog* log)
	{
		m_pCurrentLog = log;
	}

	CLog* CLogger::GetCurrentLog()
	{
		return m_pCurrentLog;
	}

	CLog* CLogger::GetLog(const std::string& name)
	{
		return FindLog(name);
	}

	void CLogger::CloseLog(const std::string& name)
	{
		TLogMap::iterator findIter = m_Logs.find(name);
		if (findIter != m_Logs.end()) {
			if (findIter->second.get() == m_pCurrentLog) {
			// Are we closing the current log?
				// Set pointer to null as it will be invalidated.
				m_pCurrentLog = nullptr;
			}

			m_Logs.erase(findIter);

			// Select a new current log.
			if (!m_pCurrentLog && !m_Logs.empty()) {
				m_pCurrentLog = m_Logs.begin()->second.get();
			}
		}
	}

	CLogMessageStream CLogger::CreateStream(ELogLevel loggingLevel, const char* file, unsigned long lineno, const char* function)
	{
		assert(m_pCurrentLog);

		CLogMessageStream stream(m_pCurrentLog, loggingLevel);
		stream << file << '(' << lineno << "): " << function << " <";
		switch (loggingLevel) {
			case LOG_INFO: {
				stream << "INFO";
				break;
			}
			case LOG_WARNING: {
				stream << "WARNING";
				break;
			}
			case LOG_ERROR: {
				stream << "ERROR";
				break;
			}
			default: {
				stream << "UNKNOWN";
			}
		}

		stream << ">: ";

		return std::move(stream);
	}

	CLog* CLogger::FindLog(const std::string& name)
	{
		TLogMap::iterator findIter = m_Logs.find(name);
		if (findIter != m_Logs.end()) {
			return findIter->second.get();
		}
		else {
			return nullptr;
		}
	}

}
