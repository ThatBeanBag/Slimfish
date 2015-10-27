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

	AShaderProgram::AShaderProgram(const std::string& name, EShaderProgramType type) 
		:m_Name(name),
		m_ShaderType(type)
	{

	}

	AShaderProgram::~AShaderProgram()
	{

	}

	void AShaderProgram::UpdateShaderParams(std::string constantBufferName, shared_ptr<CShaderParams> pShaderParams)
	{
		if (pShaderParams) {
			VUpdateShaderParams(constantBufferName, pShaderParams);
		}
		else {
			auto findIter = m_Params.find(constantBufferName);
			if (findIter != m_Params.end()) {
				VUpdateShaderParams(constantBufferName, findIter->second);
			}
			else {
				SLIM_THROW(EExceptionType::ENTRY_NOT_FOUND) << "Couldn't find parameters for " << constantBufferName << " to update.";
			}
		}
	}

	shared_ptr<CShaderParams> AShaderProgram::GetShaderParams(const std::string& constantBufferName)
	{
		// Try finding the parameters.
		auto findIter = m_Params.find(constantBufferName);
		if (findIter != m_Params.end()) {
			return findIter->second;
		}
		else {
			// Create the shader parameters.
			auto pParams = VCreateShaderParams(constantBufferName);
			if (pParams) {
				// Add the parameters to the list.
				m_Params.insert(std::make_pair(constantBufferName, pParams));
				return pParams;
			}
		}

		return nullptr;
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

