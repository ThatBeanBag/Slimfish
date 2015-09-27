// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D11VertexDeclaration.cpp
// Description	: CD3D11VertexDeclaration implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "SlimStd.h"

// Library Includes

// This Include
#include "SlimD3D11VertexDeclaration.h"

// Local Includes

/*
namespace Slim {

	CD3D11VertexDeclaration::CD3D11VertexDeclaration()
		:m_Rebuild(true),
		m_pD3DInputElements(nullptr)
	{

	}

	CD3D11VertexDeclaration::~CD3D11VertexDeclaration()
	{

	}

	void CD3D11VertexDeclaration::VAddElement(const CInputElement& inputElement)
	{
		m_Rebuild = true;
	}

	void CD3D11VertexDeclaration::VRemoveElement(size_t index)
	{
		m_Rebuild = true;
	}

	void CD3D11VertexDeclaration::VClearElements()
	{
		m_Rebuild = true;
	}

	D3D11_INPUT_ELEMENT_DESC* CD3D11VertexDeclaration::GetD3DVertexDeclaration()
	{
		size_t numElements = m_ElementList.size();

		if (numElements == 0) {
			m_pD3DInputElements.reset(nullptr);
		}

		if (m_Rebuild && numElements != 0) {
		// Do we need to rebuild?

			if (m_NumElements != numElements) {
				// Has our size changed? Do we need to re-allocate memory.
				m_NumElements = numElements;
				// Re-allocate memory.
				m_pD3DInputElements.reset(new D3D11_INPUT_ELEMENT_DESC[numElements]);
			}

			ZeroMemory(m_pD3DInputElements.get(), sizeof(D3D11_INPUT_ELEMENT_DESC) * m_NumElements);

			int currentElement = 0;
			int accumulativeOffset = 0;

			for (auto iter = m_ElementList.begin(); iter != m_ElementList.end(); ++iter) {
				// Fill the structure.
				m_pD3DInputElements[currentElement].SemanticName = iter->GetSemanticName().c_str();
				m_pD3DInputElements[currentElement].AlignedByteOffset = accumulativeOffset;
				m_pD3DInputElements[currentElement].Format = D3D11Conversions::GetFormat(iter->GetFormat());
				m_pD3DInputElements[currentElement].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

				accumulativeOffset += iter->GetSize();

				// Move to the next input element.
				currentElement++;
			}
		}
		
		return m_pD3DInputElements.get();
	}
}
*/
