// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimShaderProgram.h
// Description	: AShaderProgram declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMSHADERPROGRAM_H__
#define __SLIMSHADERPROGRAM_H__

// Library Includes

// Local Includes
#include "SlimVertexDeclaration.h"
#include "SlimShaderParams.h"

namespace Slim {

/** List of shader program types.
*/
enum class EShaderProgramType {
	VERTEX,
	PIXEL,
	GEOMETRY,
};

/** Abstract class representing a program that runs on the GPU.
@remarks
		
*/
class AShaderProgram {
public:

	// Member Functions
public:
	AShaderProgram(const std::string& name, EShaderProgramType type);
	virtual ~AShaderProgram();

	virtual bool VLoad() = 0;
	virtual void VUpdateShaderParams(std::string constantBufferName, shared_ptr<CShaderParams> pShaderParams) = 0;
	virtual shared_ptr<CShaderParams> VCreateShaderParams(const std::string& constantBufferName) = 0;

	void SetEntryPoint(const std::string& entryPoint);
	const std::string& GetEntryPoint() const;

	void SetShaderModel(const std::string& shaderModel);
	const std::string& GetShaderModel() const;

	const EShaderProgramType GetType();
	const std::string& GetName();
protected:
private:
	// Member Variables
public:
protected:
	std::string m_Name;
	EShaderProgramType m_ShaderType;

	std::string m_ShaderModel;
	std::string m_EntryPoint;

	shared_ptr<CShaderParams> m_Params;
private:
};

}

#endif	// __SLIMSHADERPROGRAM_H__