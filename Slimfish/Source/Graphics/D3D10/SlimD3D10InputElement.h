// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimD3D10InputElement.h
// Description	: CSlimD3D10InputElement declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMD3D10INPUTELEMENT_H__
#define __SLIMD3D10INPUTELEMENT_H__

// Library Includes

// Local Includes
#include "SlimD3D10Forward.h"
#include "../SlimInputElement.h"

namespace Slim {

/** Specialization of AVertexDeclaration for DirectX 10.
	@remarks

	*/
class CD3D10VertexDeclaration : public AVertexDeclaration {
	// Member Functions
public:
	CD3D10VertexDeclaration();
	~CD3D10VertexDeclaration();

protected:
private:
	virtual void VAddElement(const CInputElement& inputElement) override;
	virtual void VRemoveElement(size_t index) override;
	virtual void VClearElements() override;

	D3D10_INPUT_ELEMENT_DESC* Build();

	// Member Variables
public:
protected:
private:
	bool m_Rebuild;
};

}

#endif // __SLIMD3D10INPUTELEMENT_H__