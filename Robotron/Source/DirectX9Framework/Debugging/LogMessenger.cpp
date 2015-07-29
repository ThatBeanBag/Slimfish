// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: LogMessenger.cpp
// Description	: CLogMessenger implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes
#include <fstream>

// This Include
#include "LogMessenger.h"

// Local Includes

const char* CLogMessenger::s_kpLOG_FILENAME = "error.log";

std::unique_ptr<CLogMessenger> CLogMessenger::m_pInstance = nullptr;

CLogMessenger::CLogMessenger()
{
	SetDisplayFlags("ERROR", g_keDEF_ERROR_FLAG);
	SetDisplayFlags("WARNING", g_keDEF_WARNING_FLAG);
	SetDisplayFlags("INFO", g_keDEF_LOG_FLAG);
}

CLogMessenger::~CLogMessenger()
{

}

CLogMessenger* CLogMessenger::GetInstance()
{
	if (m_pInstance == nullptr) {
		m_pInstance.reset(new CLogMessenger);
	}
	
	return m_pInstance.get();
}

void CLogMessenger::Initialise(const std::string& _krstrConfigFile)
{
	return;	// TODO: Initialise logger from file.
}

void CLogMessenger::Log(const std::string& _krstrTag, const std::string& _krstrWhat, const char* _pcFunction, const char* _pcFile, unsigned int _uiLineNum)
{
	std::map<std::string, Logger::ELogFlag>::iterator findIter;

	// Mutex lock block.
	{
		std::lock_guard<std::mutex> lockGuard(m_tagMutex);

		findIter = m_mapTags.find(_krstrTag);

		if (findIter == m_mapTags.end()) {
			return;
		}
	}	// Unlock mutex.

	std::string strOut = GetOutputString(_krstrTag, _krstrWhat, _pcFunction, _pcFile, _uiLineNum);
	OutputStringToLogs(strOut, findIter->second);	// PP: Logging
}

void CLogMessenger::SetDisplayFlags(const std::string& _krstrTag, Logger::ELogFlag eFlags)
{
	std::lock_guard<std::mutex> lockGuard(m_tagMutex);

	auto findIter = m_mapTags.find(_krstrTag);

	if (findIter == m_mapTags.end()) {
	// Does this flag not exist?
		m_mapTags.insert(std::make_pair(_krstrTag, eFlags));
	}
	else {
		findIter->second = eFlags;
	}
}

/**
* Logs an error to the logger file and/or debugger and displays a message box of the error.
*
* The error will be logged to the file and/or debugger based on the tag's flags. To set the
* flags of a tag and therefore the location of where a message gets logged use SetDisplayFlags().
* 
* The message box will have abort, retry and ignore buttons. Abort will break the program. Retry
* will attempt to continue the execution of the program. Ignore will attempt to continue 
* execution and will also remove all new error messages.
*
* @author: 	Hayden Asplet
* @param:	const std::string & _krstrWhat - the message.
* @param:	bool _bIsFatal - true if the tag should be "FATAL", false if the tag should be "ERROR."
* @param:	const char * _pcFunction - the function the macro was called from. Retrieved by using __FUNCTION__.
* @param:	const char * _pcFile - the file the macro was called from. Retrieved by using __FILE__.
* @param:	unsigned int _uiLineNum - the line num the macro was called from. Retrieved by using __LINE__.
* @return:  CLogMessenger::EDialogResult
*/
CLogMessenger::EDialogResult CLogMessenger::Error(const std::string& _krstrWhat, bool _bIsFatal, const char* _pcFunction, const char* _pcFile, unsigned int _uiLineNum)
{
	std::string strTag;

	if (_bIsFatal) {
		strTag = "FATAL";
	}
	else {
		strTag = "ERROR";
	}

	// Log the error message.
	Log(strTag, _krstrWhat, _pcFunction, _pcFile, _uiLineNum);

	std::string strOut = GetOutputString(strTag, _krstrWhat, _pcFunction, _pcFile, _uiLineNum);

	int iResult = MessageBoxA(NULL, strOut.c_str(), strTag.c_str(), MB_ABORTRETRYIGNORE | MB_ICONERROR);

	switch (iResult) {
		case IDIGNORE: {
			return RESULT_IGNORE;
		}
		case IDABORT: {
			// Break when the user aborts.
			__debugbreak();	// Assembly language instruction to break.
			return RESULT_ABORT;
		}
		case IDRETRY: {
			return RESULT_RETRY;
		}
		default:  {
			return RESULT_RETRY;
		}
	}
}

/**
* Gets the final output string of a log message.
* 
* Helper function to construct the final output string of a message.
* The final message is formatted as follows:
* [tag]message
* Function: _pcFunction
* File: _pcFile
* Line: _uiLineNum
*
* @author: 	Hayden Asplet
* @param:	const std::string & _krstrTag - the tag, WARNING, ERROR etc.
* @param:	const std::string & _krstrWhat - the message.
* @param:	const char * _pcFunction - the function the macro was called from. Retrieved by using __FUNCTION__.
* @param:	const char * _pcFile - the file the macro was called from. Retrieved by using __FILE__.
* @param:	unsigned int _uiLineNum - the line num the macro was called from. Retrieved by using __LINE__.
* @return:  const std::string - the final output message.
*/
const std::string CLogMessenger::GetOutputString(const std::string& _krstrTag, const std::string& _krstrWhat, const char* _pcFunction, const char* _pcFile, unsigned int _uiLineNum)
{
	std::string strOut;

	if (!_krstrTag.empty()) {
		strOut = '[' + _krstrTag + ']' + _krstrWhat;
	}
	else {
		strOut = _krstrWhat;
	}

	if (_pcFunction != nullptr) {
		strOut += "\nFunction: ";
		strOut += _pcFunction;
	}

	if (_pcFile != nullptr) {
		strOut += "\nFile: ";
		strOut += _pcFile;
	}

	if (_uiLineNum != 0) {
		strOut += "\nLine: ";
		strOut += ToString(_uiLineNum);
	}

	strOut += '\n';

	return std::move(strOut);
}

void CLogMessenger::OutputStringToLogs(const std::string& _krstrOut, Logger::ELogFlag _eFlags)
{
	if (_eFlags == Logger::LOGFLAG_WRITE_TO_DEBUGGER || Logger::LOGFLAG_WRITE_TO_DEBUGGER_AND_FILE) {
		// Output to debug window.
		OutputDebugStringA(_krstrOut.c_str());
	}
	
	if (_eFlags == Logger::LOGFLAG_WRITE_TO_FILE || Logger::LOGFLAG_WRITE_TO_DEBUGGER_AND_FILE) {
		// Output to logger file.
		std::ofstream ofs(s_kpLOG_FILENAME);

		if (ofs.good()) {
			ofs << _krstrOut;
		}
		
		ofs.close();
	}
}


