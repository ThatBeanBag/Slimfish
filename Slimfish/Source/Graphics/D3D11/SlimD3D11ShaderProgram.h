// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11ShaderProgram.h
// Description	: CSlimD3D11ShaderProgram declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D11SHADERPROGRAM_H__
#define __SLIMD3D11SHADERPROGRAM_H__

// Library Includes

// Local Includes
#include "../SlimShaderProgram.h"
#include "SlimD3D11Forward.h"

namespace Slim {

/** Specialization of AShaderProgram for DirectX 11.
@remarks
		
*/
class CD3D11ShaderProgram : public AShaderProgram {
	struct TConstantVariable {
		std::string m_Name;
		size_t m_StartOffset;
		size_t m_Size;
	};

	struct TConstantBuffer {
		ID3D11ShaderReflectionConstantBuffer* m_pReflectionConstantBuffer;
		D3D11_SHADER_BUFFER_DESC m_Desc;
		ID3D11Buffer* m_pBuffer;
		std::vector<TConstantVariable> m_Variables;
	};

	typedef std::map<std::string, TConstantBuffer> TConstantBufferMap;
	typedef std::map<const CVertexDeclaration*, ComPtr<ID3D11InputLayout> > TVertexDeclToInputLayout;
public:
	typedef std::vector<ComPtr<ID3D11Buffer> > TConstantBufferList;
	typedef std::vector<unsigned char> TByteCode;

	// Member Functions
public:
	CD3D11ShaderProgram(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext, const std::string& name, EShaderProgramType type);
	~CD3D11ShaderProgram();

	virtual bool VLoad() override;
	virtual void VUpdateShaderParams(std::string constantBufferName, shared_ptr<CShaderParams> pShaderParams);
	virtual shared_ptr<CShaderParams> VCreateShaderParams(const std::string& constantBufferName);

	const TByteCode& GetByteCode() const;

	ID3D11VertexShader* GetD3DVertexShader();
	ID3D11PixelShader* GetD3DPixelShader();
	ID3D11GeometryShader* GetD3DGeometryShader();

	const TConstantBufferList& GetD3DConstantBuffers();
	ID3D11InputLayout* GetD3DInputLayout(const CVertexDeclaration* pVertexDeclaration);
protected:
private:
	bool CompileShader();

	void CreateVertexShader();
	void CreatePixelShader();
	void CreateGeometryShader();

	void CreateConstantBuffer(ID3D11ShaderReflectionConstantBuffer* pReflectionConstantBuffer);

	ID3D11InputLayout* CreateD3DInputLayout(const CVertexDeclaration* pVertexDeclaration);

	void CreateShaderParam(const std::string& prefix, const std::string& name, size_t index, size_t offset, ID3D11ShaderReflectionType* pVariableReflectionType, TConstantBuffer& constantBuffer);

	// Member Variables
public:
protected:
private:
	ID3D11Device* m_pD3DDevice;
	ID3D11DeviceContext* m_pImmediateContext;

	TByteCode m_ByteCode;

	// Shaders
	ComPtr<ID3D11VertexShader> m_pVertexShader;
	ComPtr<ID3D11PixelShader> m_pPixelShader;
	ComPtr<ID3D11GeometryShader> m_pGeometryShader;
	
	// Vertex declarations
	TVertexDeclToInputLayout m_BoundVertexDeclarations;

	// Constant buffer
	TConstantBufferMap m_NameToConstantBuffer;
	TConstantBufferList m_ConstantBuffers;

	// Shader reflection
	ComPtr<ID3D11ShaderReflection> m_pShaderReflection;
	D3D11_SHADER_DESC m_ShaderDesc;

	shared_ptr<CShaderParams> m_pParams;

	bool m_IsStreamingOutput;	// True if the geometry shader can stream output (only used when type is SHADER_TYPE_GEOMETRY).
};

}

#endif	// __SLIMD3D11SHADERPROGRAM_H__