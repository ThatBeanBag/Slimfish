// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10VertexDeclaration.h
// Description	: CSlimD3D10InputElement declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10VERTEXDECLARATION_H__
#define __SLIMD3D10VERTEXDECLARATION_H__

// Library Includes

// Local Includes
#include "SlimD3D10Forward.h"

/*
namespace Slim {

/ ** Specialization of AVertexDeclaration for DirectX 10.
	@remarks

	* /
class CD3D10VertexDeclaration : public CVertexDeclaration {
	// Member Functions
public:
	/ ** Default Constructor.
	 	@author Hayden Asplet
	* /
	CD3D10VertexDeclaration();

	/ ** Destructor.
	 	@author Hayden Asplet
	* /
	~CD3D10VertexDeclaration();

protected:
private:
	virtual void VAddElement(const CInputElement& inputElement) override;
	virtual void VRemoveElement(size_t index) override;
	virtual void VClearElements() override;

	/ ** Get the D3D10 specific vertex declaration.
		@remarks
			May rebuild the vertex declaration if the list of elements has changed since the last call to 
			this function. If any changes have been made to the size of the elements list this method will
			re-allocate memory for the array of input element descriptions.
	 	@author Hayden Asplet
	 	@return Array of D3D10 specific input element descriptions.
	* /
	D3D10_INPUT_ELEMENT_DESC* GetD3DVertexDeclaration();

	// Member Variables
public:
protected:
private:
	bool m_Rebuild;			// True if the input elements need to be rebuilt.
	size_t m_NumElements;	// Number of input elements, used to help determine if re-allocation is necessary.

	std::unique_ptr<D3D10_INPUT_ELEMENT_DESC[]> m_pD3DInputElements;	// The D3D10 specific vertex declaration.
};

}*/

#endif // __SLIMD3D10VERTEXDECLARATION_H__