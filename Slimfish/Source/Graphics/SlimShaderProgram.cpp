// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimShaderProgram.cpp
// Description	: CSlimShaderProgram implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimShaderProgram.h"

// Local Includes

namespace Slim {

	AShaderProgram::AShaderProgram(const std::string& name, EShaderType type) :m_Name(name),
		m_ShaderType(type)
	{

	}

	AShaderProgram::~AShaderProgram()
	{

	}

	const AShaderProgram::EShaderType AShaderProgram::GetType()
	{
		return m_ShaderType;
	}

	const std::string& AShaderProgram::GetName()
	{
		return m_Name;
	}

}

