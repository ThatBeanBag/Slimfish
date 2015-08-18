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

// This Include
#include "SlimVertexDeclaration.h"

// Local Includes

namespace Slim {

CInputElement::CInputElement(const std::string& semanticName, EFormat format)
{
	m_SemanticName = semanticName;
	m_Size = GetSizeFromFormat(format);
	m_Format = format;
}

CInputElement::~CInputElement()
{

}

const std::string& CInputElement::GetSemanticName() const
{
	return m_SemanticName;
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
		case Slim::CInputElement::FORMAT_SHORT: {
			return sizeof(short);
		}
		case Slim::CInputElement::FORMAT_SHORT2: {
			return sizeof(short) * 2;
		}
		case Slim::CInputElement::FORMAT_SHORT3: {
			return sizeof(short) * 3;
		}
		case Slim::CInputElement::FORMAT_SHORT4: {
			return sizeof(short) * 4;
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
	:m_PrimitiveType(PRIMITIVE_TYPE_TRIANGLELIST)
{

}

CVertexDeclaration::CVertexDeclaration(EPrimitiveType primitiveType)
	:m_PrimitiveType(primitiveType)
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
	m_ElementList.push_back(inputElement);
}

void CVertexDeclaration::RemoveElement(size_t index)
{
	auto iter = m_ElementList.begin();
	advance(iter, index);
	m_ElementList.erase(iter);
}

void CVertexDeclaration::ClearElements()
{
	m_ElementList.clear();
}

const CInputElement& CVertexDeclaration::GetElement(size_t index) const
{
	return m_ElementList[index];
}

int CVertexDeclaration::GetNumElements() const
{
	return m_ElementList.size();
}


void CVertexDeclaration::SetPrimitiveType(EPrimitiveType primitiveType)
{
	m_PrimitiveType = primitiveType;
}

Slim::CVertexDeclaration::EPrimitiveType CVertexDeclaration::GetPrimitiveType() const
{
	return m_PrimitiveType;
}

}