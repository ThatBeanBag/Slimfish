// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimExceptions.cpp
// Description	: CSlimExceptions implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimExceptions.h"

// Local Includes

namespace Slim {
	CException::CException(EExceptionType type, unsigned int lineno, const std::string& file, const std::string& function, const std::string& description)
		:m_Type(type),
		m_LineNumber(lineno),
		m_File(file),
		m_FunctionName(function),
		m_Description(description)
	{
		// Create the full description.
		m_FullDescription = file + '(' + std::to_string(lineno) + "): " + function + ": " + description;

#ifdef SLIM_LOG_EXCEPTIONS
		CLogger::GetInstance()->LogMessage(m_FullDescription, LOG_ERROR);
#endif
	}

	CException::~CException() throw()
	{

	}

	const unsigned int CException::GetSourceLineNumber() const
	{
		return m_LineNumber;
	}

	const std::string& CException::GetSourceFile() const
	{
		return m_File;
	}

	const std::string& CException::GetSourceFunction() const
	{
		return m_FunctionName;
	}

	const std::string& CException::GetShortDescription() const
	{
		return m_Description;
	}

	const std::string& CException::GetDescription() const
	{
		return m_FullDescription;
	}

	const EExceptionType& CException::GetType() const
	{
		return m_Type;
	}

	const char* CException::what() const throw()
	{
		return GetDescription().c_str();
	}

	CExceptionStream::CExceptionStream(EExceptionType type, unsigned int lineno, const std::string& file, const std::string& function) 
		:m_Type(type),
		m_LineNumber(lineno),
		m_File(file),
		m_FunctionName(function)
	{

	}

	CExceptionStream::CExceptionStream(CExceptionStream&& other) 
		:m_Type(other.m_Type),
		m_LineNumber(other.m_LineNumber),
		m_File(std::move(other.m_File)),
		m_FunctionName(std::move(other.m_FunctionName)),
		m_DescriptionStream(std::move(other.m_DescriptionStream))
	{

	}

	CExceptionStream::~CExceptionStream()
	{
		//throw CException(m_Type, m_LineNumber, m_File, m_FunctionName, m_DescriptionStream.str());
	}

	CExceptionStream& CExceptionStream::operator<<(const CEndExcept& val)
	{
		throw CException(m_Type, m_LineNumber, m_File, m_FunctionName, m_DescriptionStream.str());
	}

}


