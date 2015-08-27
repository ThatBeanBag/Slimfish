// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimLog.h
// Description	: CLog declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMLOG_H__
#define __SLIMLOG_H__

// Library Includes
#include <fstream>
#include <sstream>

// Local Includes

namespace Slim {

/** List of logging levels used when determining verbosity.
@remarks

*/
enum ELogLevel {
	LOG_DEBUG,
	// Log a message, if the user just wants to log information.
	LOG_INFO = 0,
	// Potential source of problems, but application will continuing running fine.
	LOG_WARNING = 1,
	// Errors that can be recovered from, but application may fail and have serious issues. 
	// Will cause a break into the program.
	LOG_ERROR = 2,
};

class CLog;

/** Helper class for logging messages using stream syntax.
@remarks
	This class is only used internally by the logging system and should not be used
	for any other purpose. It's intent is to provide a stream way of logging messages.
*/
class CLogMessageStream {
public:
	/** Class for determining the end of a stream. */
	class End {};

	// Member Functions
public:
	CLogMessageStream(CLog* pOutputLog, ELogLevel loggingLevel);
	CLogMessageStream(CLogMessageStream&& other);
	CLogMessageStream(const CLogMessageStream& other) = delete;

	~CLogMessageStream();

	template<typename T>
	CLogMessageStream& operator<<(const T& val);
	CLogMessageStream& operator<<(const End& end);
protected:
private:

	// Member Variables
public:
protected:
private:
	CLog* m_pOutputLog;
	ELogLevel m_LoggingLevel;

	std::ostringstream m_messageCache;
};

class CLog {
	// Member Functions
public:
	/** Construct a log file.
		@remarks
			Opens the file to prepare for writing to.
		@author Hayden Asplet
		@param
			filename Name and file path of the log file to write to.
		@param
			outputToDebug True if the logger should also output to the debug window.
		@param
			loggingLevel The logging level to log to e.g. if LOG_WARNING, will only log messages
			with logging of LOG_WARNING or greater (LOG_WARNING and LOG_ERROR will be logged).
	*/
	CLog(const std::string& filename, bool outputToDebug = true, ELogLevel loggingLevel = LOG_INFO);

	/** Destructor.
		@remarks Closes the log file.
		@author Hayden Asplet
	*/
	~CLog();

	/** Log a message at the logging level.
	@remarks
		This method should not be called directly instead use the macros SLIM_LOG() so that
		the filename, lineno, and function are captured automatically. This method is used
		primarily by CLogMessageStream and CLogger to facilitate those macros.
	@author Hayden Asplet
	@param
		message The message to output.
	@param
		loggingLevel The logging level of the message e.g. LOG_MESSAGE, LOG_WARNING or LOG_ERROR.
		This will determine whether the message is omitted based on the logging level of the log.
	*/
	void LogMessage(const std::string& message, ELogLevel loggingLevel);

	CLogMessageStream CreateStream(ELogLevel loggingLevel);

	/** Set the logging level to log to.
	@remarks
		Will log to the specified level and greater e.g. if LOG_WARNING, will only log messages
		with logging of LOG_WARNING or greater (LOG_WARNING and LOG_ERROR will be logged).
	@author Hayden Asplet
	*/
	void SetLoggingLevel(ELogLevel loggingLevel);

	/** Get the logging level. @author Hayden Asplet */
	ELogLevel GetLoggingLevel();

	/** Get the filename of the log. @author Hayden Asplet */
	const std::string& GetFilename() const;
protected:
private:

	// Member Variables
public:
protected:
private:
	std::ofstream m_LogFile;	// The log file itself.
	ELogLevel m_LoggingLevel;	// The logging level, log everything with this level and up.
	std::string m_Filename;

	bool m_OutputToDebug; // True if the logger should also output to the debug window.
};

template<typename T>
CLogMessageStream& CLogMessageStream::operator<<(const T& val)
{
	m_messageCache << val;
	return *this;
}



}

#endif	// __SLIMLOG_H__