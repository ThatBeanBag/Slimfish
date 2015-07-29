// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Logger.h
// Description	: CErrorMessenger declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __LOGGER_H__
#define __LOGGER_H__

// Library Includes
#include <string>

// Local Includes

namespace Logger {
	enum ELogFlag {
		LOGFLAG_WRITE_TO_DEBUGGER,
		LOGFLAG_WRITE_TO_FILE,
		LOGFLAG_WRITE_TO_DEBUGGER_AND_FILE
	};

	class CErrorMessenger {
		// Member Functions
	public:
		CErrorMessenger();
		~CErrorMessenger();

		void Show(const std::string& _krErrorMessage,
				  bool _bIsFatal,
				  const char* _pcFunction,
				  const char* _pcFile,
				  unsigned int _uiLineNum);

	protected:
	private:
		// Member Variables
	public:
	protected:
	private:
		bool m_bIsEnabled;
	};

	void InitialiseLogger(const std::string& _krLoggerConfigFile);

	void Log(const std::string& _krstrTag,
			 const std::string& _krstrWhat,
			 const char* _pcFunction,
			 const char* _pcFile,
			 unsigned int _uiLineNum);
	void SetDisplayFlags(const std::string& _krTag, ELogFlag _eFlags);
};

extern const Logger::ELogFlag g_keDEF_ERROR_FLAG;
extern const Logger::ELogFlag g_keDEF_WARNING_FLAG;
extern const Logger::ELogFlag g_keDEF_LOG_FLAG;

// Do, while blocks are necessary so that the macros can be interpreted as a single statement.

#define FATAL(strWhat) \
	do { \
		static Logger::CErrorMessenger errorMessenger; \
		std::string str((strWhat)); \
		errorMessenger.Show(str, true, __FUNCTION__, __FILE__, __LINE__);	\
	} \
	while (0); \

#ifndef NDEBUG

#define DEBUG_ERROR(strWhat) \
	do { \
		static Logger::CErrorMessenger errorMessenger; \
		std::string str((strWhat)); \
		errorMessenger.Show(str, false, __FUNCTION__, __FILE__, __LINE__);	\
	} \
	while (0); \

#define DEBUG_WARNING(strWhat) \
	do { \
		std::string str((strWhat)); \
		Logger::Log("WARNING", str, __FUNCTION__, __FILE__, __LINE__);	\
	} \
	while (0); \

#define DEBUG_INFO(strWhat) \
	do { \
		std::string str((strWhat)); \
		Logger::Log("INFO", str, nullptr, nullptr, 0); \
	} \
	while(0) \

#define DEBUG_LOG(tag, strWhat) \
	do { \
		std::string strTag((tag)); \
		std::string str((strWhat)); \
		Logger::Log(strTag, str, nullptr, nullptr, 0); \
	} \
	while (0) \

#define DEBUG_ASSERT(expression) \
	do { \
		if (!(expression)) { \
			static Logger::CErrorMessenger errorMessenger; \
			errorMessenger.Show(#expression, false, __FUNCTION__, __FILE__, __LINE__);	\
		} \
	} \
	while (0) \

#else	// NDEBUG is defined

// The release mode definitions for the debugging macros above, that are written in such a way
// as to be ignored by the compiler.
#define DEBUG_ERROR(strWhat) do { (void)sizeof(strWhat); } while (0)
#define DEBUG_WARNING(strWhat) do { (void)sizeof(strWhat); } while (0)
#define DEBUG_INFO(strWhat) do { (void)sizeof(strWhat); } while (0)
#define DEBUG_LOG(strTag, strWhat) do { (void)sizeof(strWhat); } while (0)
#define DEBUG_ASSERT(strWhat) do { (void)sizeof(strWhat); } while (0)

#endif // NDEBUG


#endif	// __LOGGER_H__