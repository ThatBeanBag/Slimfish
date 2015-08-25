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
			D3D11_SHADER_VARIABLE_DESC variable = constantBuffer.m_Variables[i];
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

		constantBuffer.m_pBuffer->Unmap();
	}

	shared_ptr<CShaderParams> CD3D10ShaderProgram::CreateShaderParams(const std::string& constantBufferName)
	{
		TConstantBufferMap::iterator findIter = m_NameToConstantBuffer.find(constantBufferName);
		if (findIter == m_NameToConstantBuffer.end()) {
			SLIM_WARNING() << "Failed to create shader params, couldn't find constant buffer " << constantBufferName;
			return nullptr;
		}

		const TConstantBuffer& constantBuffer = findIter->second;

		ID3D11ShaderReflectionConstantBuffer* pReflectionConstantBuffer = constantBuffer.m_pReflectionConstantBuffer;
		D3D11_SHADER_BUFFER_DESC shaderBufferDesc = constantBuffer.m_Desc;

		// Get the variable's descriptions of the constant buffer.
		for (size_t i = 0; i < shaderBufferDesc.Variables; ++i) {
			ID3D11ShaderReflectionVariable* pVariable = pReflectionConstantBuffer->GetVariableByIndex(i);
			ID3D11ShaderReflectionType* pVariableReflectionType = pVariable->GetType();

			D3D11_SHADER_VARIABLE_DESC shaderVariableDesc;
			HRESULT hResult = pVariable->GetDesc(&shaderVariableDesc);

			CreateShaderParam("", shaderVariableDesc.Name, i, pVariableReflectionType);
		}
	}

	/*void CD3D10ShaderProgram::VBindVertexDeclaration(CVertexDeclaration* pVertexDeclaration)
	{
		assert(pVertexDeclaration);

		ID3D10InputLayout* pLayout = nullptr;

		TVertexDeclToInputLayout::iterator findIter = m_BoundVertexDeclarations.find(pVertexDeclaration);
		if (findIter != m_BoundVertexDeclarations.end()) {
		// Do we already have an input layout for this vertex declaration?
			pLayout = findIter->second;

			/ *if (pVertexDeclaration->NeedsRebuilding()) {
			// Has the vertex declaration changed since the last binding?
				// Release old layout.
				SafeRelease(pLayout);

				pLayout = CreateD3DInputLayout(pVertexDeclaration);

				// Insert the new layout into the list of vertex declarations.
				findIter->second = pLayout;
			}* /
		}
		else {
			pLayout = CreateD3DInputLayout(pVertexDeclaration);

			if (pLayout) {
				m_BoundVertexDeclarations[pVertexDeclaration] = pLayout;
			}
		}
	}*/

	const CD3D10ShaderProgram::TByteCode& CD3D10ShaderProgram::GetByteCode() const
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

	ID3D10InputLayout* CD3D10ShaderProgram::GetD3DInputLayout(const CVertexDeclaration* pVertexDeclaration)
	{
		assert(pVertexDeclaration);

		ID3D10InputLayout* pLayout = nullptr;

		TVertexDeclToInputLayout::iterator findIter = m_BoundVertexDeclarations.find(pVertexDeclaration);
		if (findIter != m_BoundVertexDeclarations.end()) {
			// Do we already have an input layout for this vertex declaration?
			pLayout = findIter->second;
		}
		else {
			pLayout = CreateD3DInputLayout(pVertexDeclaration);
			m_BoundVertexDeclarations[pVertexDeclaration] = pLayout;
		}

		return pLayout;
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
		//compileFlags |= D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;

		ID3D10Blob* pCompiledShader;

		HRESULT hResult = D3DX10CompileFromFileA(
			m_Name.c_str(),			// File name.
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
							 IID_ID3D11ShaderReflection, 
							 reinterpret_cast<void**>(&m_pShaderReflection));

		if (SUCCEEDED(hResult)) {
			hResult = m_pShaderReflection->GetDesc(&m_ShaderDesc);

			if (SUCCEEDED(hResult)) {

				for (size_t i = 0; i < m_ShaderDesc.ConstantBuffers; ++i) {
					ID3D11ShaderReflectionConstantBuffer* pReflectionConstantBuffer = m_pShaderReflection->GetConstantBufferByIndex(i);

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
			std::vector<D3D10_SO_DECLARATION_ENTRY> soDeclarations(m_ShaderDesc.OutputParameters);
			int totalComponentCount = 0;

			for (size_t i = 0; i < soDeclarations.size(); ++i) {
				D3D11_SIGNATURE_PARAMETER_DESC parameterDesc;
				m_pShaderReflection->GetOutputParameterDesc(i, &parameterDesc);

				soDeclarations[i].SemanticName = parameterDesc.SemanticName;
				soDeclarations[i].SemanticIndex = parameterDesc.SemanticIndex;

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

				soDeclarations[i].StartComponent = 0;
				soDeclarations[i].ComponentCount = componentCount;
				soDeclarations[i].OutputSlot = 0;
			}

			size_t streamStride = totalComponentCount * sizeof(float);

			hResult = m_pD3DDevice->CreateGeometryShaderWithStreamOutput(
				&m_ByteCode[0],			// Pointer to the compiled shader.
				m_ByteCode.size(),		// Shader size in bytes.
				&soDeclarations[0],				// Stream output declarations.
				soDeclarations.size(),			// Number of output entries.
				streamStride,					// Output stream stride.
				&m_pGeometryShader);
		}
		else {
			hResult = m_pD3DDevice->CreateGeometryShader(
				&m_ByteCode[0],
				m_ByteCode.size(),
				&m_pGeometryShader);
		}

		if (FAILED(hResult)) {
			SLIM_ERROR() << "Failed to create geometry shader " << m_Name;
			SafeRelease(m_pGeometryShader);
		}
		else {
			assert(m_pGeometryShader);
		}
	}

	void CD3D10ShaderProgram::CreateConstantBuffer(ID3D11ShaderReflectionConstantBuffer* pReflectionConstantBuffer)
	{
		D3D11_SHADER_BUFFER_DESC shaderBufferDesc;
		HRESULT hResult = pReflectionConstantBuffer->GetDesc(&shaderBufferDesc);

		if (FAILED(hResult)) {
			SLIM_ERROR() << "Failed to get description of D3D10 constant buffer for shader " << m_Name;
			return;
		}

		TConstantBuffer constantBuffer;
		constantBuffer.m_Desc = shaderBufferDesc;
		constantBuffer.m_pBuffer = nullptr;
		constantBuffer.m_pReflectionConstantBuffer = pReflectionConstantBuffer;

		// Get the variable's descriptions of the constant buffer.
		for (size_t i = 0; i < shaderBufferDesc.Variables; ++i) {
			ID3D11ShaderReflectionVariable* pVariable;

			pVariable = pReflectionConstantBuffer->GetVariableByIndex(i);

			if (pVariable) {
				D3D11_SHADER_VARIABLE_DESC shaderVariableDesc;
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
			SLIM_ERROR() << "Failed to create constant buffer " << shaderBufferDesc.Name << " for D3D10 shader " << m_Name;
			SafeRelease(pBuffer);
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

	ID3D10InputLayout* CD3D10ShaderProgram::CreateD3DInputLayout(const CVertexDeclaration* pVertexDeclaration)
	{
		assert(pVertexDeclaration);

		std::vector<D3D10_INPUT_ELEMENT_DESC> d3dInputElements(pVertexDeclaration->GetNumElements());
		int accumulativeOffset = 0;	// For determining byte offset, based on previous elements in the buffer.

		// Fill the input element descriptions.
		for (size_t i = 0; i < d3dInputElements.size(); ++i) {
			const CInputElement& element = pVertexDeclaration->GetElement(i);

			ZeroMemory(&d3dInputElements[i], sizeof(D3D10_INPUT_ELEMENT_DESC));

			d3dInputElements[i].SemanticName = element.GetSemanticName().c_str();
			d3dInputElements[i].AlignedByteOffset = accumulativeOffset;
			d3dInputElements[i].Format = D3D10Conversions::GetFormat(element.GetFormat());
			d3dInputElements[i].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;

			accumulativeOffset += element.GetSize();
		}

		ID3D10InputLayout* pLayout = nullptr;

		// Create the layout, binding the shader to the vertex declaration.
		HRESULT hResult = m_pD3DDevice->CreateInputLayout(
			&d3dInputElements[0],				// Input element descriptions.
			d3dInputElements.size(),			// Number of elements.
			&m_ByteCode[0],						// Byte code.
			m_ByteCode.size(),					// Length of byte code.
			&pLayout);							// Output input layout.

		if (FAILED(hResult)) {
			SLIM_ERROR() << "Failed to create input layout when binding vertex declaration to shader " << m_Name;
			return nullptr;
		}

		return pLayout;
	}

	void CD3D10ShaderProgram::CreateShaderParam(const std::string& prefix, const std::string& name, size_t index, ID3D11ShaderReflectionType* pVariableReflectionType)
	{
		D3D11_SHADER_TYPE_DESC variableTypeDesc;
		HRESULT hResult = pVariableReflectionType->GetDesc(&variableTypeDesc);

		if (FAILED(hResult)) {
			SLIM_ERROR() << "Failed to get param type description from D3D10 shader program " << m_Name;
			return;
		}

		std::string paramName = name;	// Make a copy to remove constness.

		if (paramName[0] == '$') {
			paramName.erase(paramName.begin());
		}

		if (variableTypeDesc.Type == D3D10_SVC_STRUCT) {
		// Is this a structure.
			std::string prefix = prefix + paramName + ".";

			for (size_t i = 0; i < variableTypeDesc.Members; ++i) {
				// Recursively call until we get a to primitive variable.
				CreateShaderParam(prefix, pVariableReflectionType->GetMemberTypeName(i), i, pVariableReflectionType->GetMemberTypeByIndex(i));
			}
		}
		else if (variableTypeDesc.Type == D3D10_SVT_FLOAT || 
				 variableTypeDesc.Type == D3D10_SVT_INT || 
				 variableTypeDesc.Type == D3D10_SVT_BOOL) {
			paramName = prefix + paramName;

			m_pParams->AddConstant(paramName, D3D10Conversions::GetShaderConstantType(variableTypeDesc));
		}
	}

}



