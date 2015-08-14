// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10InputElement.cpp
// Description	: CD3D10VertexDeclaration implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D10InputElement.h"

// Local Includes

namespace Slim {

	CD3D10VertexDeclaration::CD3D10VertexDeclaration()
	{

	}

	CD3D10VertexDeclaration::~CD3D10VertexDeclaration()
	{

	}

	void CD3D10VertexDeclaration::VAddElement(const CInputElement& inputElement)
	{
		m_Rebuild = true;
	}

	void CD3D10VertexDeclaration::VRemoveElement(size_t index)
	{
		m_Rebuild = true;
	}

	void CD3D10VertexDeclaration::VClearElements()
	{
		m_Rebuild = true;
	}

	D3D10_INPUT_ELEMENT_DESC* CD3D10VertexDeclaration::Build()
	{
		D3D10_INPUT_ELEMENT_DESC* pD3DElementDesc = new D3D10_INPUT_ELEMENT_DESC[m_ElementList.size()];

		int currentElement = 0;

		for (auto iter = m_ElementList.begin(); iter != m_ElementList.end(); ++iter) {
			ZeroMemory(&pD3DElementDesc[currentElement], sizeof(D3D10_INPUT_ELEMENT_DESC));

			pD3DElementDesc[currentElement].SemanticName = iter->GetSemanticName().c_str();
			pD3DElementDesc[currentElement].AlignedByteOffset = iter->GetOffset();
			pD3DElementDesc[currentElement].Format = D3D10Conversions::Get(iter->GetFormat());
			pD3DElementDesc[currentElement].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		}

		// Probably going to leak.
		return pD3DElementDesc;
	}

}
