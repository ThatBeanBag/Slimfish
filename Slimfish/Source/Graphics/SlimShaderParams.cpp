// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimShaderParams.cpp
// Description	: CSlimShaderParams implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimShaderParams.h"

// Local Includes

namespace Slim {

	CShaderConstant::CShaderConstant() 
		:m_Type(TYPE_INT), 
		m_Index(0), 
		m_Size(0)
	{

	}

	CShaderConstant::CShaderConstant(EConstantType type, size_t index /*= 0*/) 
		:m_Type(type),
		m_Index(index),
		m_Size(GetSizeFromType(type))
	{

	}

	CShaderConstant::~CShaderConstant()
	{

	}

	bool CShaderConstant::IsFloat() const
	{
		switch (m_Type) {
			case TYPE_FLOAT:		// Fall through.
			case TYPE_FLOAT2:		// Fall through.
			case TYPE_FLOAT3:		// Fall through.
			case TYPE_FLOAT4:		// Fall through.
			case TYPE_MATRIX4X4: {
				return true;
			}
			default: {
				return false;
			}
		}
	}

	bool CShaderConstant::IsInt() const
	{
		switch (m_Type) {
			case TYPE_INT:			// Fall through.
			case TYPE_INT2:			// Fall through.
			case TYPE_INT3:			// Fall through.
			case TYPE_INT4: {
				return true;
			}
			default: {
				return false;
			}
		}

		return false;
	}

	bool CShaderConstant::IsSampler() const
	{
		switch (m_Type) {
			case TYPE_SAMPLER1D:	// Fall through.
			case TYPE_SAMPLER2D:	// Fall through.
			case TYPE_SAMPLER3D:	// Fall through.
			case TYPE_SAMPLERCUBE: {
				return true;
			}
			default: {
				return false;
			}
		}

		return false;
	}

	const size_t CShaderConstant::GetSizeInBytes() const 
	{
		if (IsFloat()) {
			return m_Size * sizeof(float);
		}
		else {
			return m_Size * sizeof(int);
		}
	}

	const size_t CShaderConstant::GetSizeFromType(EConstantType type)
	{
		switch (type) {
			case TYPE_FLOAT:		// Fall through.
			case TYPE_INT:			// Fall through.
			case TYPE_SAMPLER1D:	// Fall through.
			case TYPE_SAMPLER2D:	// Fall through.
			case TYPE_SAMPLER3D:	// Fall through.
			case TYPE_SAMPLERCUBE: {
				return 1;
			}
			case TYPE_INT2:			// Fall through.
			case TYPE_FLOAT2: {
				return 2;
			}
			case TYPE_INT3:			// Fall through.
			case TYPE_FLOAT3: {
				return 3;
			}
			case TYPE_INT4:			// Fall through.
			case TYPE_FLOAT4: {
				return 4;
			}
			case TYPE_MATRIX4X4: {
				return 16;
			}
			default: {
				return 1;
			}
		}

		return 1;
	}

	CShaderParams::CShaderParams()
	{

	}

	CShaderParams::~CShaderParams()
	{

	}

	void CShaderParams::AddConstant(const std::string& name, CShaderConstant::EConstantType type)
	{
		if (m_NamedConstantMap.find(name) != m_NamedConstantMap.end()) {
			// TODO: throw up and error.
		}

		CShaderConstant shaderConstant(type);

		// Insert the constant by saving the index and resizing the list of constants to make room for the new constant.
		if (shaderConstant.IsFloat()) {
			shaderConstant.m_Index = m_ConstantFloats.size();
			m_ConstantFloats.resize(m_ConstantFloats.size() + shaderConstant.m_Size);
		}
		else {
			shaderConstant.m_Index = m_ConstantInts.size();
			m_ConstantInts.resize(m_ConstantInts.size() + shaderConstant.m_Size);
		}

		// Add the constant to the map, so we can find it later.
		m_NamedConstantMap[name] = shaderConstant;
	}

	void CShaderParams::ClearConstants()
	{
		m_ConstantFloats.clear();
		m_ConstantInts.clear();
		m_NamedConstantMap.clear();
	}

	void CShaderParams::SetConstant(const std::string& name, const int* pValue, size_t count)
	{
		NamedConstantMap::iterator findIter = m_NamedConstantMap.find(name);
		if (findIter == m_NamedConstantMap.end()) {
			// TODO: throw up an error.
			return;
		}

		CShaderConstant shaderConstant = findIter->second;

		for (size_t i = 0; i < count && i + shaderConstant.m_Index < m_ConstantInts.size(); ++i) {
			m_ConstantInts[shaderConstant.m_Index + i] = pValue[i];
		}

		//memcpy(&m_ConstantInts[shaderConstant.m_Index], pValue, sizeof(int) * shaderConstant.m_Size);
	}

	void CShaderParams::SetConstant(const std::string& name, const float* pValue, size_t count)
	{
		NamedConstantMap::iterator findIter = m_NamedConstantMap.find(name);
		if (findIter == m_NamedConstantMap.end()) {
			// TODO: throw up an error.
			return;
		}

		CShaderConstant shaderConstant = findIter->second;

		for (size_t i = 0; i < count && i + shaderConstant.m_Index < m_ConstantFloats.size(); ++i) {
			m_ConstantFloats[shaderConstant.m_Index + i] = pValue[i];
		}

		//memcpy(&m_ConstantFloats[shaderConstant.m_Index], pValue, sizeof(float) * shaderConstant.m_Size);
	}

	void CShaderParams::SetConstant(const std::string& name, const CColourValue& value)
	{
		float pValue[4];
		pValue[0] = value.m_r;
		pValue[1] = value.m_g;
		pValue[2] = value.m_b;
		pValue[3] = value.m_a;

		SetConstant(name, pValue, 4);
	}

	void CShaderParams::SetConstant(const std::string& name, const CMatrix4x4& value)
	{
		SetConstant(name, value[0], 16);
	}

	void CShaderParams::SetConstant(const std::string& name, const CVector3& value)
	{
		float pValue[3];
		pValue[0] = value.GetX();
		pValue[1] = value.GetY();
		pValue[2] = value.GetZ();

		SetConstant(name, pValue, 3);
	}

	void CShaderParams::SetConstant(const std::string& name, int value)
	{
		SetConstant(name, &value, 1);
	}

	void CShaderParams::SetConstant(const std::string& name, float value)
	{
		SetConstant(name, &value, 1);
	}

	const CShaderConstant CShaderParams::GetConstant(const std::string& name)
	{
		NamedConstantMap::iterator findIter = m_NamedConstantMap.find(name);

		if (findIter == m_NamedConstantMap.end()) {
			// TODO: throw exception.
		}

		return findIter->second;
	}

	const CShaderParams::ConstantFloatList& CShaderParams::GetConstantFloatList() const
	{
		return m_ConstantFloats;
	}

	const CShaderParams::ConstantIntList& CShaderParams::GetConstantIntList() const
	{
		return m_ConstantInts;
	}

}

