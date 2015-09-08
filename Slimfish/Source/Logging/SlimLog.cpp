// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimLog.cpp
// Description	: CSlimLog implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimLog.h"

// Local Includes
#include <ctime>
#include <iomanip>

namespace Slim {

	CLogMessageStream::CLogMessageStream(CLog* pOutputLog, ELogLevel loggingLevel) 
		:m_pOutputLog(pOutputLog),
		m_LoggingLevel(loggingLevel)
	{

	}

	CLogMessageStream::CLogMessageStream(CLogMessageStream&& other)
		:m_pOutputLog(other.m_pOutputLog),
		m_LoggingLevel(other.m_LoggingLevel)
	{
		other.m_pOutputLog = nullptr;
		m_messageCache = std::move(other.m_messageCache);
	}

	CLogMessageStream::~CLogMessageStream()
	{
		if (m_messageCache.tellp() > 0) {
			m_pOutputLog->LogMessage(m_messageCache.str(), m_LoggingLevel);
		}
	}

	CLogMessageStream& CLogMessageStream::operator<<(const End& end)
	{
		m_pOutputLog->LogMessage(m_messageCache.str(), m_LoggingLevel);
		m_messageCache.str("");
		return *this;
	}

	CLog::CLog(const std::string& filename, bool outputToDebug /*= true*/, ELogLevel loggingLevel /*= LOG_MESSAGE*/) :m_Filename(filename),
		m_OutputToDebug(outputToDebug),
		m_LoggingLevel(loggingLevel)
	{
		m_LogFile.open(filename.c_str());
	}

	CLog::~CLog()
	{
		m_LogFile.close();
	}

	void CLog::LogMessage(const std::string& message, ELogLevel loggingLevel)
	{
		if (m_LoggingLevel < m_LoggingLevel) {
		// Is the logging level of the message to low for us to care?
			// Don't log it.
			return;
		}

		// Add the time stamp to the log message.
		struct tm pTime;
		time_t ctTime;
		time(&ctTime);
		localtime_s(&pTime, &ctTime);

		m_LogFile << std::setw(2) << std::setfill('0') << pTime.tm_hour << ':'
				  << std::setw(2) << std::setfill('0') << pTime.tm_min << ':'
				  << std::setw(2) << std::setfill('0') << pTime.tm_sec << ": ";

		// Output the message.
		m_LogFile << message << std::endl;


		if (m_OutputToDebug) {
			std::string debugString = message + "\n";
			OutputDebugStringA(debugString.c_str());
		}
		
		// Flush to ensure the file gets written to.
		m_LogFile.flush();
	}

	CLogMessageStream CLog::CreateStream(ELogLevel loggingLevel)
	{
		return std::move(CLogMessageStream(this, loggingLevel));
	}

	void CLog::SetLoggingLevel(ELogLevel loggingLevel)
	{
		m_LoggingLevel = loggingLevel;
	}

	ELogLevel CLog::GetLoggingLevel()
	{
		return m_LoggingLevel;
	}

	const std::string& CLog::GetFilename() const
	{
		return m_Filename;
	}

}

