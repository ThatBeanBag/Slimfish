// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimLogger.h
// Description	: CLogger declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMLOGGER_H__
#define __SLIMLOGGER_H__

// Library Includes
#include <fstream>
#include <sstream>

// Local Includes
#include "../Utilities/SlimSingleton.h"
#include "SlimLog.h"

namespace Slim {

/** Singleton class managing log files. 
@remarks
	
*/
class CLogger : public ASingleton < CLogger > {
	typedef std::map<std::string, std::unique_ptr<CLog>> TLogMap;

	// Member Functions
public:
	~CLogger();

	/** Create a log to log messages to.
		@remarks
			This method should be called at the very start of the programs execution to make sure
			there's a log to write to.
	 	@author Hayden Asplet
	 	@param 
			filename Name of the file to write messages to.
	 	@param 
			outputToDebug True if the log should also write message to the debug window.
	 	@param 
			loggingLevel The logging level to log to e.g. if LOG_WARNING, will only log messages
			with logging of LOG_WARNING or greater (LOG_WARNING and LOG_ERROR will be logged).
	 	@param 
			setAsCurrent True if the new log should be set as the current log to write to.
	*/
	void CreateLog(const std::string& filename, bool outputToDebug = true, ELogLevel loggingLevel = LOG_INFO, bool setAsCurrent = true);

	/** Log a message to the current log.
	@remarks
		This method should not be called directly instead use the macros SLIM_LOG() so that
		the filename, lineno, and function are captured automatically.
	@author Hayden Asplet
	@param
		message The message to output.
	@param
		loggingLevel The logging level of the message e.g. LOG_MESSAGE, LOG_WARNING or LOG_ERROR.
		This will determine whether the message is omitted based on the logging level of the log.
	*/
	void LogMessage(const std::string& message, ELogLevel loggingLevel);

	/** Set the current log.
		@note
			May fail if no log of the specified name exists, in which case the old log will
			still be used.
		@author Hayden Asplet
		@return
			True if the log of the specified name was found.
		*/
	bool SetCurrentLog(const std::string& name);

	/** Set the current log to the given log.
		@author Hayden Asplet
		*/
	void SetCurrentLog(CLog* log);

	/** Retrieve the current log that is being written to.
		@author Hayden Asplet
		@return Pointer to the current log.
		*/
	CLog* GetCurrentLog();

	/** Get a specific log by name.
		@author Hayden Asplet
		@return
		Pointer to the log. May be NULL if name couldn't be found.
		*/
	CLog* GetLog(const std::string& name);

	/** Close a log and remove it from the logging system. @author Hayden Asplet */
	void CloseLog(const std::string& name);

	CLogMessageStream CreateStream(ELogLevel loggingLevel, const char* file, unsigned long lineno, const char* function);
protected:
private:
	/** Helper function for finding a log in the map of logs.
		@author Hayden Asplet
		*/
	CLog* FindLog(const std::string& name);

	// Member Variables
public:
protected:
private:
	TLogMap m_Logs;

	CLog* m_pCurrentLog;	// The current log.
};


#define SLIM_BREAK_ON_ERROR 1

#ifndef SLIM_BREAK_ON_ERROR

#define SLIM_LOG(logLevel)	\
	CLogger::GetInstance()->CreateStream(logLevel, __FILE__, __LINE__, __FUNCTION__)

#define SLIM_LOG_IF(condition, logLevel)\
	if(condition) \
		CLogger::GetInstance()->CreateStream(logLevel, __FILE__, __LINE__, __FUNCTION__)

#else

#define SLIM_LOG(logLevel)\
	if(logLevel >= LOG_ERROR)\
		__debugbreak();\
	CLogger::GetInstance()->CreateStream(logLevel, __FILE__, __LINE__, __FUNCTION__)

#define SLIM_LOG_IF(condition, logLevel)\
	if(condition && logLevel >= LOG_ERROR)\
		__debugbreak();\
	if(condition)\
		CLogger::GetInstance()->CreateStream(logLevel, __FILE__, __LINE__, __FUNCTION__)

#endif

// Convenience macros for logging at specific levels.
#define SLIM_INFO()\
	SLIM_LOG(LOG_INFO)
#define SLIM_INFO_IF(condition)\
	SLIM_LOG_IF(condition, LOG_INFO)
#define SLIM_WARNING()\
	SLIM_LOG(LOG_WARNING)
#define SLIM_WARNING_IF(condition)\
	SLIM_LOG_IF(condition, LOG_WARNING)
#define SLIM_ERROR() \
	SLIM_LOG(LOG_ERROR)
#define SLIM_ERROR_IF(condition)\
	SLIM_LOG_IF(condition, LOG_ERROR)

}

#endif	// __SLIMLOGGER_H__