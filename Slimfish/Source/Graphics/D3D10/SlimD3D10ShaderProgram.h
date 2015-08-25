// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10ShaderProgram.h
// Description	: CSlimD3D10ShaderProgram declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10SHADERPROGRAM_H__
#define __SLIMD3D10SHADERPROGRAM_H__

// Library Includes

// Local Includes
#include "../SlimShaderProgram.h"
#include "SlimD3D10Forward.h"

namespace Slim {

/** Specialization of AShaderProgram for directX 10.
@remarks
		
*/
class CD3D10ShaderProgram : public AShaderProgram {
	struct TConstantBuffer {
		ID3D11ShaderReflectionConstantBuffer* m_pReflectionConstantBuffer;
		D3D11_SHADER_BUFFER_DESC m_Desc;
		ID3D10Buffer* m_pBuffer;
		std::vector<D3D11_SHADER_VARIABLE_DESC> m_Variables;
	};

	typedef std::map<std::string, TConstantBuffer> TConstantBufferMap;
	typedef std::map<const CVertexDeclaration*, ID3D10InputLayout*> TVertexDeclToInputLayout;
public:
	typedef std::vector<ID3D10Buffer*> TConstantBufferList;
	typedef std::vector<unsigned char> TByteCode;

	// Member Functions
public:
	CD3D10ShaderProgram(ID3D10Device* pD3DDevice, const std::string& name, EShaderType type);
	~CD3D10ShaderProgram();

	virtual bool VLoad() override;
	virtual void VUpdateProgramParams(std::string constantBufferName, shared_ptr<CShaderParams> pShaderParams);
	virtual shared_ptr<CShaderParams> CreateShaderParams(const std::string& constantBufferName);

	const TByteCode& GetByteCode() const;

	ID3D10VertexShader* GetD3DVertexShader();
	ID3D10PixelShader* GetD3DPixelShader();
	ID3D10GeometryShader* GetD3DGeometryShader();

	const TConstantBufferList& GetD3DConstantBuffers();
	ID3D10InputLayout* GetD3DInputLayout(const CVertexDeclaration* pVertexDeclaration);
protected:
private:
	bool CompileShader();

	void CreateVertexShader();
	void CreatePixelShader();
	void CreateGeometryShader();

	void CreateConstantBuffer(ID3D11ShaderReflectionConstantBuffer* pReflectionConstantBuffer);

	ID3D10InputLayout* CreateD3DInputLayout(const CVertexDeclaration* pVertexDeclaration);

	void CreateShaderParam(const std::string& prefix, const std::string& name, size_t index, ID3D11ShaderReflectionType* pVariableReflectionType);

	// Member Variables
public:
protected:
private:
	ID3D10Device* m_pD3DDevice;

	TByteCode m_ByteCode;

	// Shaders
	ID3D10VertexShader* m_pVertexShader;
	ID3D10PixelShader* m_pPixelShader;
	ID3D10GeometryShader* m_pGeometryShader;
	
	// Vertex declarations
	TVertexDeclToInputLayout m_BoundVertexDeclarations;

	// Constant buffer
	TConstantBufferMap m_NameToConstantBuffer;
	TConstantBufferList m_ConstantBuffers;

	// Shader reflection
	ID3D11ShaderReflection* m_pShaderReflection;
	D3D11_SHADER_DESC m_ShaderDesc;

	shared_ptr<CShaderParams> m_pParams;

	bool m_IsStreamingOutput;	// True if the geometry shader can stream output (only used when type is SHADER_TYPE_GEOMETRY).
};

}

#endif	// __SLIMD3D10SHADERPROGRAM_H__