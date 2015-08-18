// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10ShaderProgram.cpp
// Description	: CSlimD3D10ShaderProgram implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10ShaderProgram.h"

// Local Includes
#include "SlimD3D10GpuBuffer.h"

namespace Slim {

	CD3D10ShaderProgram::CD3D10ShaderProgram(ID3D10Device* pD3DDevice, const std::string& name, EShaderType type)
		:AShaderProgram(name, type),
		m_pD3DDevice(pD3DDevice),
		m_pVertexShader(nullptr),
		m_pGeometryShader(nullptr),
		m_pPixelShader(nullptr),
		m_pShaderReflection(nullptr),
		m_IsStreamingOutput(false)
	{

	}

	CD3D10ShaderProgram::~CD3D10ShaderProgram()
	{
		SafeRelease(m_pVertexShader);
		SafeRelease(m_pPixelShader);
		SafeRelease(m_pGeometryShader);
		SafeRelease(m_pShaderReflection);

		// Release constant buffers.
		m_NameToConstantBuffer.clear();

		for (size_t i = 0; i < m_ConstantBuffers.size(); ++i) {
			SafeRelease(m_ConstantBuffers[i]);
		}
	}

	void CD3D10ShaderProgram::SetEntryPoint(const std::string& entryPoint)
	{
		m_EntryPoint = entryPoint;
	}

	void CD3D10ShaderProgram::SetShaderModel(const std::string& shaderModel)
	{
		m_ShaderModel = shaderModel;
	}

	bool CD3D10ShaderProgram::VLoad()
	{
		return CompileShader();
	}

	void CD3D10ShaderProgram::VUpdateProgramParams(std::string constantBufferName, shared_ptr<CShaderParams> pShaderParams)
	{
		TConstantBufferMap::iterator findIter = m_NameToConstantBuffer.find(constantBufferName);
		if (findIter == m_NameToConstantBuffer.end()) {
			// TODO: throw exception.
			return;
		}

		const TConstantBuffer& constantBuffer = findIter->second;

		assert(constantBuffer.m_pBuffer);

		void* pData = nullptr;

		HRESULT hResult = constantBuffer.m_pBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, &pData);
		if (FAILED(hResult)) {
			// TODO: throw exception.
			return;
		}

		assert(pData);

		for (size_t i = 0; i < constantBuffer.m_Variables.size(); ++i) {
			D3D10_SHADER_VARIABLE_DESC variable = constantBuffer.m_Variables[i];
			std::string variableName = variable.Name;

			CShaderConstant shaderConstant = pShaderParams->GetConstant(variableName);

			const void* pSource = nullptr;
			size_t size = 0;

			if (shaderConstant.IsFloat()) {
				size = shaderConstant.m_Size * sizeof(float);
				pSource = reinterpret_cast<const void*>(&pShaderParams->GetConstantFloatList()[shaderConstant.m_Index]);
			}
			else {
				size = shaderConstant.m_Size * sizeof(int);
				pSource = reinterpret_cast<const void*>(&pShaderParams->GetConstantIntList()[shaderConstant.m_Index]);
			}

			assert(variable.Size == size);

			memcpy(reinterpret_cast<char*>(pData) + variable.StartOffset, pSource, size);
		}
	}

	const CD3D10ShaderProgram::TByteCode& CD3D10ShaderProgram::GetByteCode()
	{
		assert(m_ByteCode.size() > 0);
		return m_ByteCode;
	}

	ID3D10VertexShader* CD3D10ShaderProgram::GetD3DVertexShader()
	{
		return m_pVertexShader;
	}

	ID3D10PixelShader* CD3D10ShaderProgram::GetD3DPixelShader()
	{
		return m_pPixelShader;
	}

	ID3D10GeometryShader* CD3D10ShaderProgram::GetD3DGeometryShader()
	{
		return m_pGeometryShader;
	}

	const CD3D10ShaderProgram::TConstantBufferList& CD3D10ShaderProgram::GetD3DConstantBuffers()
	{
		return m_ConstantBuffers;
	}

	bool CD3D10ShaderProgram::CompileShader()
	{
		ID3D10Blob* pCompilationErrors = nullptr;
		D3D10_SHADER_MACRO* pDefines = nullptr;
		UINT compileFlags = 0;

#ifdef _DEBUG
		compileFlags |= D3D10_SHADER_DEBUG;
		compileFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
		compileFlags |= D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;

		ID3D10Blob* pCompiledShader;

		HRESULT hResult = D3DX10CompileFromFileA(m_Name.c_str(),		// File name.
												 pDefines,				// Macro definitions.
												 nullptr,				// Include handler.
												 m_EntryPoint.c_str(),	// Name of the entry point function of the shader. 
												 m_ShaderModel.c_str(),	// String specifying the shader model to use e.g. vs_4_1.
												 compileFlags,			// Compilation flags.
												 0,						// Effect file compilation flags (only used for effect file loading).
												 NULL,					// Thread pump. NULL specifies that the function should not return until it's completed.
												 &pCompiledShader,		// Pointer to the compiled shader.
												 &pCompilationErrors,	// Compilation errors.
												 NULL);					// HRESULT.
		if (FAILED(hResult)) {
			std::string errorMessage = "Failed to compile shader from file " + m_Name + " Errors:\n" + static_cast<const char*>(pCompilationErrors->GetBufferPointer());
			// TODO: Display errorMessage.
			SafeRelease(pCompilationErrors);
			SafeRelease(pCompiledShader);

			return false;
		}

		m_ByteCode.resize(pCompiledShader->GetBufferSize());
		memcpy(&m_ByteCode[0], pCompiledShader->GetBufferPointer(), m_ByteCode.size());

		SafeRelease(pCompiledShader);

		// Create constant buffers.
		hResult = D3DReflect(reinterpret_cast<void*>(&m_ByteCode[0]), 
							 m_ByteCode.size(),
							 IID_ID3D10ShaderReflection, 
							 reinterpret_cast<void**>(&m_pShaderReflection));

		if (SUCCEEDED(hResult)) {
			hResult = m_pShaderReflection->GetDesc(&m_ShaderDesc);

			if (SUCCEEDED(hResult)) {

				for (size_t i = 0; i < m_ShaderDesc.ConstantBuffers; ++i) {
					ID3D10ShaderReflectionConstantBuffer* pReflectionConstantBuffer = m_pShaderReflection->GetConstantBufferByIndex(i);

					assert(pReflectionConstantBuffer);
					CreateConstantBuffer(pReflectionConstantBuffer);
				}
			}
		}

		// Create the shader from the compiled byte code.
		switch (m_ShaderType) {
			case SHADER_TYPE_VERTEX: {
				CreateVertexShader();
				break;
			}
			case SHADER_TYPE_PIXEL: {
				CreatePixelShader();
				break;
			}
			case SHADER_TYPE_GEOMETRY: {
				CreateGeometryShader();
				break;
			}
			default: {
				break;
			}
		}

		return true;
	}

	void CD3D10ShaderProgram::CreateVertexShader()
	{
		assert(!m_ByteCode.empty());

		HRESULT hResult = m_pD3DDevice->CreateVertexShader(&m_ByteCode[0], m_ByteCode.size(), &m_pVertexShader);
		if (FAILED(hResult)) {
			// TODO: Display an error.

			SafeRelease(m_pVertexShader);
		}
		else {
			assert(m_pVertexShader);
		}
	}

	void CD3D10ShaderProgram::CreatePixelShader()
	{
		assert(!m_ByteCode.empty());

		HRESULT hResult = m_pD3DDevice->CreatePixelShader(&m_ByteCode[0], m_ByteCode.size(), &m_pPixelShader);
		if (FAILED(hResult)) {
			// TODO: Display an error.
			SafeRelease(m_pPixelShader);
		}
		else {
			assert(m_pPixelShader);
		}
	}

	void CD3D10ShaderProgram::CreateGeometryShader()
	{
		assert(!m_ByteCode.empty());

		HRESULT hResult = 0;

		if (m_IsStreamingOutput) {
			std::unique_ptr<D3D10_SO_DECLARATION_ENTRY[]> pSODeclarations(new D3D10_SO_DECLARATION_ENTRY[m_ShaderDesc.OutputParameters]);
			int totalComponentCount = 0;

			for (size_t i = 0; i < m_ShaderDesc.OutputParameters; ++i) {
				D3D10_SIGNATURE_PARAMETER_DESC parameterDesc;
				m_pShaderReflection->GetOutputParameterDesc(i, &parameterDesc);

				pSODeclarations[i].SemanticName = parameterDesc.SemanticName;
				pSODeclarations[i].SemanticIndex = parameterDesc.SemanticIndex;

				int componentCount = 0;
				if (parameterDesc.Mask & 1) {
					componentCount++;
				}
				if (parameterDesc.Mask & 2) {
					componentCount++;
				}
				if (parameterDesc.Mask & 4) {
					componentCount++;
				}
				if (parameterDesc.Mask & 8) {
					componentCount++;
				}

				totalComponentCount += componentCount;

				pSODeclarations[i].StartComponent = 0;
				pSODeclarations[i].ComponentCount = componentCount;
				pSODeclarations[i].OutputSlot = 0;
			}

			size_t streamStride = totalComponentCount * sizeof(float);

			hResult = m_pD3DDevice->CreateGeometryShaderWithStreamOutput(
				&m_ByteCode[0],			// Pointer to the compiled shader.
				m_ByteCode.size(),		// Shader size in bytes.
				pSODeclarations.get(),					// Stream output declarations.
				m_ShaderDesc.OutputParameters,			// Number of output entries.
				streamStride,							// Output stream stride.
				&m_pGeometryShader);
		}
		else {
			hResult = m_pD3DDevice->CreateGeometryShader(
				&m_ByteCode[0],
				m_ByteCode.size(),
				&m_pGeometryShader);
		}

		if (FAILED(hResult)) {
			// TODO: Display an error.
			SafeRelease(m_pGeometryShader);
		}
		else {
			assert(m_pGeometryShader);
		}
	}

	void CD3D10ShaderProgram::CreateConstantBuffer(ID3D10ShaderReflectionConstantBuffer* pReflectionConstantBuffer)
	{
		D3D10_SHADER_BUFFER_DESC shaderBufferDesc;
		HRESULT hResult = pReflectionConstantBuffer->GetDesc(&shaderBufferDesc);

		if (FAILED(hResult)) {
			// TODO: display error.
			return;
		}

		TConstantBuffer constantBuffer;
		constantBuffer.m_Desc = shaderBufferDesc;
		constantBuffer.m_pBuffer = nullptr;

		// Get the variable's descriptions of the constant buffer.
		for (size_t i = 0; i < shaderBufferDesc.Variables; ++i) {
			ID3D10ShaderReflectionVariable* pVariable;

			pVariable = pReflectionConstantBuffer->GetVariableByIndex(i);

			if (pVariable) {
				D3D10_SHADER_VARIABLE_DESC shaderVariableDesc;
				hResult = pVariable->GetDesc(&shaderVariableDesc);
				// Add description to the list.
				constantBuffer.m_Variables.push_back(shaderVariableDesc);
			}
		}

		D3D10_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D10_BUFFER_DESC));

		// The buffer must be dynamic as the CPU must write to it.
		// This is because constant buffers are our means of communication between the CPU and GPU.
		bufferDesc.ByteWidth = shaderBufferDesc.Size;
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

		ID3D10Buffer* pBuffer;	// The actual buffer.

		hResult = m_pD3DDevice->CreateBuffer(&bufferDesc, NULL, &pBuffer);
		if (FAILED(hResult)) {
			// TODO: log error "Cannot create constant buffer for D3D10 Shader.".
			return;
		}

		// m_ConstantBuffers owns the reference, whereas the m_NameToConstantBuffer map justs holds a reference to our ref.
		// Both m_ConstantBuffers and m_NameToConstantBuffer go out of scope at the same time so adding a ref would be redundant.
		m_ConstantBuffers.push_back(pBuffer);
		constantBuffer.m_pBuffer = pBuffer;

		// Insert the buffer in the map so we can find it later.
		std::string name = shaderBufferDesc.Name;
		m_NameToConstantBuffer[name] = constantBuffer;
	}

}



