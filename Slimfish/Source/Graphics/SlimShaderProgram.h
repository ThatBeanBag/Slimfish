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
	using TShaderParamsMap = std::map<std::string, std::shared_ptr<CShaderParams> >;

	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	 	@param 
			name Name of the shader file.
	 	@param 
			type Type of the shader program e.g. PIXEL, GEOMETRY or VERTEX.
	*/
	AShaderProgram(const std::string& name, EShaderProgramType type);

	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~AShaderProgram();

	/** Load and compile the shader.
	 	@author Hayden Asplet
	 	@return True if the shader loaded and compiled successfully.
	*/
	virtual bool VLoad() = 0;

	/** Update a buffer on the gpu from the associated shader parameters.
		@note Throws if the buffer couldn't be found.
		@author Hayden Asplet
		@param constantBufferName Name of the constant buffer of the program.
		@param pShaderParams Pointer to the parameters to update to. If null,
			the parameters may be found automatically if they have been created,
			however, this is slower.
	*/
	virtual void UpdateShaderParams(std::string constantBufferName, shared_ptr<CShaderParams> pShaderParams = nullptr);

	/** Get the shader parameters for a specific constant buffer.
		@remarks
			Creates the shader parameters if they have not been created for the 
			specified constant buffer.
	 	@author Hayden Asplet
	 	@param 
			constantBufferName The name of the buffer as specified in the shader.
	 	@return 
			Pointer to the shader parameters.
	*/
	virtual shared_ptr<CShaderParams> GetShaderParams(const std::string& constantBufferName);

	/** Set the name of the entry point function to the program. @author Hayden Asplet*/
	void SetEntryPoint(const std::string& entryPoint);

	/** Get the name of the entry point function to the program. @author Hayden Asplet*/
	const std::string& GetEntryPoint() const;

	/** Set the shader model of the program e.g. vs_4_0. @author Hayden Asplet */
	void SetShaderModel(const std::string& shaderModel);
	/** Get the shader model of the program e.g. vs_4_0. @author Hayden Asplet */
	const std::string& GetShaderModel() const;

	/** Get the type of shader program e.g. PIXEL, GEOMETRY, or VERTEX. @author Hayden Asplet */
	const EShaderProgramType GetType();

	/** Get the name of the shader file. @author Hayden Asplet */
	const std::string& GetName();
protected:
private:
	/** Create the shader parameters for a specific constant buffer.
	 	@author Hayden Asplet
	 	@param 
			constantBufferName The name of the buffer as specified in the shader.
	 	@return 
			Pointer to the created shader parameters.
	*/
	virtual shared_ptr<CShaderParams> VCreateShaderParams(const std::string& constantBufferName) = 0;

	/** Update the associated buffer on the gpu from the shader parameters.
		@author Hayden Asplet
		@param constantBufferName Name of the constant buffer of the program.
		@param pShaderParams Pointer to the parameters to update to.
	*/
	virtual void VUpdateShaderParams(std::string constantBufferName, shared_ptr<CShaderParams> pShaderParams) = 0;

	// Member Variables
public:
protected:
	std::string m_Name;
	EShaderProgramType m_ShaderType;

	std::string m_ShaderModel;
	std::string m_EntryPoint;

private:
	TShaderParamsMap m_Params;
};

}

#endif	// __SLIMSHADERPROGRAM_H__