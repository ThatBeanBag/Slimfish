// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimInputElement.cpp
// Description	: CSlimInputElement implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes
#include <algorithm>

// This Include
#include "SlimVertexDeclaration.h"

// Local Includes

namespace Slim {

CInputElement::CInputElement(const std::string& semanticName, EFormat format)
	:m_SemanticName(semanticName),
	m_SemanticIndex(0),
	m_Size(GetSizeFromFormat(format)),
	m_Format(format)
{
}

CInputElement::~CInputElement()
{

}

const std::string& CInputElement::GetSemanticName() const
{
	return m_SemanticName;
}

void CInputElement::SetSemanticIndex(size_t semanticIndex)
{
	m_SemanticIndex = semanticIndex;
}

const size_t CInputElement::GetSemanticIndex() const
{
	return m_SemanticIndex;
}

const size_t CInputElement::GetSize() const
{
	return m_Size;
}

const CInputElement::EFormat CInputElement::GetFormat() const
{
	return m_Format;
}

const size_t CInputElement::GetSizeFromFormat(EFormat format)
{
	switch (format) {
		case Slim::CInputElement::FORMAT_FLOAT: {
			return sizeof(float);
		}
		case Slim::CInputElement::FORMAT_FLOAT2: {
			return sizeof(float) * 2;
		}
		case Slim::CInputElement::FORMAT_FLOAT3: {
			return sizeof(float) * 3;
		}
		case Slim::CInputElement::FORMAT_FLOAT4: {
			return sizeof(float) * 4;
		}
		case Slim::CInputElement::FORMAT_INT: // Fall through. 
		case Slim::CInputElement::FORMAT_UINT: {
			return sizeof(int);
		}
		case Slim::CInputElement::FORMAT_INT2: // Fall through. 
		case Slim::CInputElement::FORMAT_UINT2: {
			return sizeof(int) * 2;
		}
		case Slim::CInputElement::FORMAT_INT3: // Fall through. 
		case Slim::CInputElement::FORMAT_UINT3: {
			return sizeof(int) * 3;
		}
		case Slim::CInputElement::FORMAT_INT4: // Fall through. 
		case Slim::CInputElement::FORMAT_UINT4: {
			return sizeof(int) * 4;
		}
		case Slim::CInputElement::FORMAT_COLOUR_RGBA: {
			return sizeof(float) * 4;
		}
		default: {
			break;
		}
	}

	return 0;
}

CVertexDeclaration::CVertexDeclaration()
	:m_NeedsRebuilding(true)
{

}

CVertexDeclaration::~CVertexDeclaration()
{

}

void CVertexDeclaration::AddElement(const std::string& semanticName, CInputElement::EFormat format)
{
	AddElement(CInputElement(semanticName, format));
}

void CVertexDeclaration::AddElement(const CInputElement& inputElement)
{
	// Count all elements with the same semantic name to get the index.
	auto hasSameName = [&](const CInputElement& element) {
		return element.GetSemanticName() == inputElement.GetSemanticName();
	};

	auto semanticIndex = std::count_if(m_ElementList.begin(), m_ElementList.end(), hasSameName);

	m_ElementList.push_back(inputElement);
	m_ElementList.back().SetSemanticIndex(semanticIndex);

	m_NeedsRebuilding = true;
}

void CVertexDeclaration::RemoveElement(size_t index)
{
	auto iter = m_ElementList.begin();
	advance(iter, index);
	m_ElementList.erase(iter);

	m_NeedsRebuilding = true;
}

void CVertexDeclaration::ClearElements()
{
	m_ElementList.clear();
	m_NeedsRebuilding = true;
}

const CInputElement& CVertexDeclaration::GetElement(size_t index) const
{
	return m_ElementList[index];
}

int CVertexDeclaration::GetNumElements() const
{
	return m_ElementList.size();
}

void CVertexDeclaration::SetRebuilt()
{
	m_NeedsRebuilding = false;
}

const bool CVertexDeclaration::NeedsRebuilding() const
{
	return m_NeedsRebuilding;
}

}