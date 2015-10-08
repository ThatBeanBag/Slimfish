// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11ShaderProgram.cpp
// Description	: CSlimD3D11ShaderProgram implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D11ShaderProgram.h"

// Local Includes
#include "SlimD3D11GpuBuffer.h"

namespace Slim {

	CD3D11ShaderProgram::CD3D11ShaderProgram(ID3D11Device* pD3DDevice, ID3D11DeviceContext* pImmediateContext, const std::string& name, EShaderProgramType type)
		:AShaderProgram(name, type),
		m_pD3DDevice(pD3DDevice),
		m_pImmediateContext(pImmediateContext),
		m_pVertexShader(nullptr),
		m_pGeometryShader(nullptr),
		m_pPixelShader(nullptr),
		m_pShaderReflection(nullptr),
		m_IsStreamingOutput(false)
	{

	}

	CD3D11ShaderProgram::~CD3D11ShaderProgram()
	{
		//SLIM_SAFE_RELEASE(m_pVertexShader);
		//SLIM_SAFE_RELEASE(m_pPixelShader);
		//SLIM_SAFE_RELEASE(m_pGeometryShader);
		//SLIM_SAFE_RELEASE(m_pShaderReflection);

		// Release constant buffers.
		m_NameToConstantBuffer.clear();

		//for (size_t i = 0; i < m_ConstantBuffers.size(); ++i) {
		//	SLIM_SAFE_RELEASE(m_ConstantBuffers[i]);
		//}
		//
		//for (auto iter = m_BoundVertexDeclarations.begin(); iter != m_BoundVertexDeclarations.end(); ++iter) {
		//	SLIM_SAFE_RELEASE(iter->second);
		//}
	}

	bool CD3D11ShaderProgram::VLoad()
	{
		return CompileShader();
	}

	void CD3D11ShaderProgram::VUpdateShaderParams(std::string constantBufferName, shared_ptr<CShaderParams> pShaderParams)
	{
		TConstantBufferMap::iterator findIter = m_NameToConstantBuffer.find(constantBufferName);
		if (findIter == m_NameToConstantBuffer.end()) {
			// TODO: throw exception.
			return;
		}

		const TConstantBuffer& constantBuffer = findIter->second;

		assert(constantBuffer.m_pBuffer);


		D3D11_MAPPED_SUBRESOURCE mappedData;
		HRESULT hResult = m_pImmediateContext->Map(constantBuffer.m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		if (FAILED(hResult)) {
			// TODO: throw exception.
			return;
		}

		void* pData = mappedData.pData;
		assert(pData);

		for (size_t i = 0; i < constantBuffer.m_Variables.size(); ++i) {
			// Use the name from the list of names in the constant buffer, if it exists.
			// This is for variables that are structures, in which case the variable name alone
			// will not suffice.
			std::string variableName = constantBuffer.m_Variables[i].m_Name;

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

			assert(constantBuffer.m_Variables[i].m_Size == size);

			memcpy(reinterpret_cast<char*>(pData)+ constantBuffer.m_Variables[i].m_StartOffset, pSource, size);
		}

		m_pImmediateContext->Unmap(constantBuffer.m_pBuffer, 0);
	}

	shared_ptr<CShaderParams> CD3D11ShaderProgram::VCreateShaderParams(const std::string& constantBufferName)
	{
		m_pParams.reset(new CShaderParams());

		TConstantBufferMap::iterator findIter = m_NameToConstantBuffer.find(constantBufferName);
		if (findIter == m_NameToConstantBuffer.end()) {
			SLIM_WARNING() << "Failed to create shader params, couldn't find constant buffer " << constantBufferName;
			return nullptr;
		}

		TConstantBuffer& constantBuffer = findIter->second;

		// Clear current variables so we can overwrite them.
		constantBuffer.m_Variables.clear();

		ID3D11ShaderReflectionConstantBuffer* pReflectionConstantBuffer = constantBuffer.m_pReflectionConstantBuffer;
		D3D11_SHADER_BUFFER_DESC shaderBufferDesc = constantBuffer.m_Desc;

		// Get the variable's descriptions of the constant buffer.
		for (size_t i = 0; i < shaderBufferDesc.Variables; ++i) {
			ID3D11ShaderReflectionVariable* pVariable = pReflectionConstantBuffer->GetVariableByIndex(i);
			ID3D11ShaderReflectionType* pVariableReflectionType = pVariable->GetType();

			D3D11_SHADER_VARIABLE_DESC shaderVariableDesc;
			HRESULT hResult = pVariable->GetDesc(&shaderVariableDesc);

			// Recursively create the shader parameter as it might be a structure.
			CreateShaderParam("", shaderVariableDesc.Name, i, shaderVariableDesc.StartOffset, pVariableReflectionType, constantBuffer);
		}

		return m_pParams;
	}


	/*void CD3D11ShaderProgram::VBindVertexDeclaration(CVertexDeclaration* pVertexDeclaration)
	{
		assert(pVertexDeclaration);

		ID3D11InputLayout* pLayout = nullptr;

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

	const CD3D11ShaderProgram::TByteCode& CD3D11ShaderProgram::GetByteCode() const
	{
		assert(m_ByteCode.size() > 0);
		return m_ByteCode;
	}

	ID3D11VertexShader* CD3D11ShaderProgram::GetD3DVertexShader()
	{
		return m_pVertexShader.Get();
	}

	ID3D11PixelShader* CD3D11ShaderProgram::GetD3DPixelShader()
	{
		return m_pPixelShader.Get();
	}

	ID3D11GeometryShader* CD3D11ShaderProgram::GetD3DGeometryShader()
	{
		return m_pGeometryShader.Get();
	}

	const CD3D11ShaderProgram::TConstantBufferList& CD3D11ShaderProgram::GetD3DConstantBuffers()
	{
		return m_ConstantBuffers;
	}

	ID3D11InputLayout* CD3D11ShaderProgram::GetD3DInputLayout(const CVertexDeclaration* pVertexDeclaration)
	{
		assert(pVertexDeclaration);

		ID3D11InputLayout* pLayout = nullptr;

		TVertexDeclToInputLayout::iterator findIter = m_BoundVertexDeclarations.find(pVertexDeclaration);
		if (findIter != m_BoundVertexDeclarations.end()) {
			// Do we already have an input layout for this vertex declaration?
			pLayout = findIter->second.Get();
		}
		else {
			pLayout = CreateD3DInputLayout(pVertexDeclaration);
			m_BoundVertexDeclarations[pVertexDeclaration] = pLayout;
		}

		return pLayout;
	}

	bool CD3D11ShaderProgram::CompileShader()
	{
		ID3D10Blob* pCompilationErrors = nullptr;
		D3D10_SHADER_MACRO* pDefines = nullptr;
		UINT compileFlags = 0;

#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
		compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		compileFlags |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

		ID3D10Blob* pCompiledShader;

		HRESULT hResult = D3DX11CompileFromFileA(
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
			SLIM_SAFE_RELEASE(pCompilationErrors);
			SLIM_SAFE_RELEASE(pCompiledShader);

			return false;
		}

		m_ByteCode.resize(pCompiledShader->GetBufferSize());
		memcpy(&m_ByteCode[0], pCompiledShader->GetBufferPointer(), m_ByteCode.size());

		SLIM_SAFE_RELEASE(pCompiledShader);

		// Create constant buffers.
		hResult = D3DReflect(reinterpret_cast<void*>(&m_ByteCode[0]), 
							 m_ByteCode.size(),
							 IID_ID3D11ShaderReflection, 
							 reinterpret_cast<void**>(m_pShaderReflection.GetAddressOf()));

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
			case EShaderProgramType::VERTEX: {
				CreateVertexShader();
				break;
			}
			case EShaderProgramType::PIXEL: {
				CreatePixelShader();
				break;
			}
			case EShaderProgramType::GEOMETRY: {
				CreateGeometryShader();
				break;
			}
			default: {
				break;
			}
		}

		return true;
	}

	void CD3D11ShaderProgram::CreateVertexShader()
	{
		assert(!m_ByteCode.empty());

		HRESULT hResult = m_pD3DDevice->CreateVertexShader(&m_ByteCode[0], m_ByteCode.size(), nullptr, m_pVertexShader.GetAddressOf());
		if (FAILED(hResult)) {
			//SLIM_SAFE_RELEASE(m_pVertexShader);
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create vertex shader for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}
		else {
			assert(m_pVertexShader);
		}
	}

	void CD3D11ShaderProgram::CreatePixelShader()
	{
		assert(!m_ByteCode.empty());

		HRESULT hResult = m_pD3DDevice->CreatePixelShader(&m_ByteCode[0], m_ByteCode.size(), nullptr, m_pPixelShader.GetAddressOf());
		if (FAILED(hResult)) {
			//SLIM_SAFE_RELEASE(m_pPixelShader);
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create pixel shader for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}
		else {
			assert(m_pPixelShader);
		}
	}

	void CD3D11ShaderProgram::CreateGeometryShader()
	{
		assert(!m_ByteCode.empty());

		HRESULT hResult = 0;

		if (m_IsStreamingOutput) {
			std::vector<D3D11_SO_DECLARATION_ENTRY> soDeclarations(m_ShaderDesc.OutputParameters);
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
				m_ShaderDesc.OutputParameters,	// Number of output entries.
				&streamStride,					// Output stream stride.
				1,								// Number of stream strides.
				0,								// Rasterized stream.
				nullptr,						// Class linkage.
				m_pGeometryShader.GetAddressOf());			// Shader.
		}
		else {
			hResult = m_pD3DDevice->CreateGeometryShader(
				&m_ByteCode[0],
				m_ByteCode.size(),
				nullptr,
				m_pGeometryShader.GetAddressOf());
		}

		if (FAILED(hResult)) {
			//SLIM_SAFE_RELEASE(m_pGeometryShader);
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create geometry shader for " << GetName() << " with error: " << GetErrorMessage(hResult);
		}
		else {
			assert(m_pGeometryShader);
		}
	}

	void CD3D11ShaderProgram::CreateConstantBuffer(ID3D11ShaderReflectionConstantBuffer* pReflectionConstantBuffer)
	{
		D3D11_SHADER_BUFFER_DESC shaderBufferDesc;
		HRESULT hResult = pReflectionConstantBuffer->GetDesc(&shaderBufferDesc);

		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to get description of D3D11 constant buffer for shader " << m_Name << " with error: " << GetErrorMessage(hResult);
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

				TConstantVariable constantVariable;
				constantVariable.m_Name = shaderVariableDesc.Name;
				constantVariable.m_Size = shaderVariableDesc.Size;
				constantVariable.m_StartOffset = shaderVariableDesc.Size;
				// Add description to the list.
				constantBuffer.m_Variables.push_back(constantVariable);
			}
		}

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

		// The buffer must be dynamic as the CPU must write to it.
		// This is because constant buffers are our means of communication between the CPU and GPU.
		bufferDesc.ByteWidth = shaderBufferDesc.Size;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		ID3D11Buffer* pBuffer;	// The actual buffer.

		hResult = m_pD3DDevice->CreateBuffer(&bufferDesc, nullptr, &pBuffer);
		if (FAILED(hResult)) {
			SLIM_SAFE_RELEASE(pBuffer);
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create constant buffer " << shaderBufferDesc.Name << " for shader " << m_Name
				<< " with error: " << GetErrorMessage(hResult);
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

	ID3D11InputLayout* CD3D11ShaderProgram::CreateD3DInputLayout(const CVertexDeclaration* pVertexDeclaration)
	{
		assert(pVertexDeclaration);

		std::vector<D3D11_INPUT_ELEMENT_DESC> d3dInputElements(pVertexDeclaration->GetNumElements());
		int accumulativeOffset = 0;	// For determining byte offset, based on previous elements in the buffer.

		// Fill the input element descriptions.
		for (size_t i = 0; i < d3dInputElements.size(); ++i) {
			const CInputElement& element = pVertexDeclaration->GetElement(i);

			ZeroMemory(&d3dInputElements[i], sizeof(D3D11_INPUT_ELEMENT_DESC));

			d3dInputElements[i].SemanticName = element.GetSemanticName().c_str();
			d3dInputElements[i].AlignedByteOffset = accumulativeOffset;
			d3dInputElements[i].Format = D3D11Conversions::GetFormat(element.GetFormat());
			d3dInputElements[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

			accumulativeOffset += element.GetSize();
		}

		ID3D11InputLayout* pLayout = nullptr;

		// Create the layout, binding the shader to the vertex declaration.
		HRESULT hResult = m_pD3DDevice->CreateInputLayout(
			&d3dInputElements[0],				// Input element descriptions.
			d3dInputElements.size(),			// Number of elements.
			&m_ByteCode[0],						// Byte code.
			m_ByteCode.size(),					// Length of byte code.
			&pLayout);							// Output input layout.

		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to create input layout when binding vertex declaration to shader " << m_Name << " with error: " << GetErrorMessage(hResult);
			return nullptr;
		}

		return pLayout;
	}

	void CD3D11ShaderProgram::CreateShaderParam(const std::string& prefix, const std::string& name, size_t index, size_t offset, ID3D11ShaderReflectionType* pVariableReflectionType, TConstantBuffer& constantBuffer)
	{
		D3D11_SHADER_TYPE_DESC variableTypeDesc;
		HRESULT hResult = pVariableReflectionType->GetDesc(&variableTypeDesc);

		if (FAILED(hResult)) {
			SLIM_THROW(EExceptionType::RENDERING) << "Failed to get param type description from shader program " << m_Name << " with error: " << GetErrorMessage(hResult);
			return;
		}

		std::string paramName = name;	// Make a copy to remove constness.

		if (paramName[0] == '$') {
			paramName.erase(paramName.begin());
		}

		if (variableTypeDesc.Class == D3D10_SVC_STRUCT) {
		// Is this param a structure?
			std::string nextPrefix = prefix + paramName + ".";

			for (size_t i = 0; i < variableTypeDesc.Members; ++i) {
				// Recursively call until we get a to primitive variable.
				CreateShaderParam(
					nextPrefix, 
					pVariableReflectionType->GetMemberTypeName(i), 
					i,
					variableTypeDesc.Offset + offset,
					pVariableReflectionType->GetMemberTypeByIndex(i), 
					constantBuffer);
			}
		}
		else if (variableTypeDesc.Type == D3D10_SVT_FLOAT ||
			variableTypeDesc.Type == D3D10_SVT_INT ||
			variableTypeDesc.Type == D3D10_SVT_BOOL) {
			paramName = prefix + paramName;

			EShaderConstantType type = D3D11Conversions::GetShaderConstantType(variableTypeDesc);
			m_pParams->AddConstant(paramName, type);

			// Save the constant variable details for when we update shader params in the buffer.
			TConstantVariable constantVariable;
			constantVariable.m_Name = paramName;
			constantVariable.m_StartOffset = variableTypeDesc.Offset + offset;
			constantVariable.m_Size = CShaderConstant(type).GetSizeInBytes();
			constantBuffer.m_Variables.push_back(constantVariable);
		}
	}

}



