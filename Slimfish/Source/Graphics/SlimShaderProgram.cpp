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

	AShaderProgram::AShaderProgram(const std::string& name, EShaderProgramType type) :m_Name(name),
		m_ShaderType(type)
	{

	}

	AShaderProgram::~AShaderProgram()
	{

	}

	void AShaderProgram::SetEntryPoint(const std::string& entryPoint)
	{
		m_EntryPoint = entryPoint;
	}

	const std::string& AShaderProgram::GetEntryPoint() const
	{
		return m_EntryPoint;
	}

	void AShaderProgram::SetShaderModel(const std::string& shaderModel)
	{
		m_ShaderModel = shaderModel;
	}

	const std::string& AShaderProgram::GetShaderModel() const
	{
		return m_ShaderModel;
	}

	const EShaderProgramType AShaderProgram::GetType()
	{
		return m_ShaderType;
	}

	const std::string& AShaderProgram::GetName()
	{
		return m_Name;
	}
}

