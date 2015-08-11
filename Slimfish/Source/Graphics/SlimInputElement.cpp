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
#include "SlimInputElement.h"

// Local Includes

namespace Slim {

	AVertexDeclaration::AVertexDeclaration()
	{

	}

	AVertexDeclaration::~AVertexDeclaration()
	{

	}

	void AVertexDeclaration::AddElement(const std::string& semanticName, size_t offsetInBuffer, CInputElement::EFormat format)
	{
		AddElement(CInputElement(semanticName, offsetInBuffer, format));
	}

	void AVertexDeclaration::AddElement(const CInputElement& inputElement)
	{
		m_ElementList.push_back(inputElement);
	}

	void AVertexDeclaration::RemoveElement(size_t index)
	{
		auto iter = m_ElementList.begin();
		advance(iter, index);
		m_ElementList.erase(iter);
	}

	void AVertexDeclaration::ClearElements()
	{
		m_ElementList.clear();
	}

	const CInputElement& AVertexDeclaration::GetElement(size_t index) const
	{
		auto iter = m_ElementList.begin();
		advance(iter, index);
		return (*iter);
	}

	int AVertexDeclaration::GetNumElements() const
	{
		return m_ElementList.size();
	}

}