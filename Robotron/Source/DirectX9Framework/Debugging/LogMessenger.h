// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: LogMessenger.h
// Description	: CLogMessenger declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __LOGMESSENGER_H__
#define __LOGMESSENGER_H__

// Library Includes
#include <mutex>
#include <map>

// Local Includes
#include "Logger.h"
#include "../GameStd.h"	// TODO: Remove this.

class CLogMessenger {
public:
	enum EDialogResult {
		RESULT_ABORT,
		RESULT_RETRY,
		RESULT_IGNORE
	};

	// Member Functions
public:
	~CLogMessenger();

	static CLogMessenger* GetInstance();

	void Initialise(const std::string& _krstrConfigFile);
	void Log(const std::string& _krstrTag,
			 const std::string& _krstrWhat,
			 const char* _pcFunction,
			 const char* _pcFile,
			 unsigned int _uiLineNum);
	void SetDisplayFlags(const std::string& _krstrTag, Logger::ELogFlag eFlags);

	EDialogResult Error(const std::string& _krstrWhat,
						bool _bIsFatal,
						const char* _pcFunction,
						const char* _pcFile,
						unsigned int _uiLineNum);
protected:
private:
	CLogMessenger();
	CLogMessenger(const CLogMessenger& _krOther) = delete;
	CLogMessenger& operator=(const CLogMessenger& _krOther) = delete;

	const std::string GetOutputString(const std::string& _krstrTag,
									  const std::string& _krstrWhat,
									  const char* _pcFunction,
									  const char* _pcFile,
									  unsigned int _uiLineNum);
	void OutputStringToLogs(const std::string& _krstrOut, Logger::ELogFlag _eFlags);

	// Member Variables
public:
	static const char* s_kpLOG_FILENAME;
protected:
private:
	static std::unique_ptr<CLogMessenger> m_pInstance;

	std::map<std::string, Logger::ELogFlag> m_mapTags;

	// Thread Safety
	std::mutex m_tagMutex;
};

#endif	// __LOGMESSENGER_H__