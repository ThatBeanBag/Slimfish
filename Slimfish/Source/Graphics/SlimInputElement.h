// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimInputElement.h
// Description	: CSlimInputElement declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMINPUTELEMENT_H__
#define __SLIMINPUTELEMENT_H__

// Library Includes
#include <string>
#include <list>

// Local Includes

namespace Slim {

/** Class representing a single vertex input element.
@remarks
		
*/
class CInputElement {
public:
	/** List of formats of input elements.
	*/
	enum EFormat {
		// Single 32-bit floating-point format.
		FORMAT_FLOAT,
		// Combination of 2 32-bit floating-point values format.
		FORMAT_FLOAT2,
		// Combination of 3 32-bit floating-point values format.
		FORMAT_FLOAT3,
		// Combination of 4 32-bit floating-point values format.
		FORMAT_FLOAT4,
		// Single 16 bit short format.
		FORMAT_SHORT,
		// Combination of 2 16-bit short values format.
		FORMAT_SHORT2,
		// Combination of 2 16-bit short values format.
		FORMAT_SHORT3,
		// Combination of 2 16-bit short values format.
		FORMAT_SHORT4,
		// Standard colour format.
		FORMAT_COLOUR_RGBA
	};

	// Member Functions
public:
	/** Constructor.
		@author Hayden Asplet
		@param semanticName The literal string of the name of the semantic.
		@param offsetInBuffer The offset in bytes of the element in the buffer of the vertex declaration.
		@param format The format or type of the element.
	*/
	CInputElement(const std::string& semanticName, size_t offsetInBuffer, EFormat format);

	/** Destructor.
		@author Hayden Asplet
	*/
	~CInputElement();
protected:
private:
	// Member Variables
public:
protected:
private:
	std::string m_SemanticName;	// The literal string of the name of the semantic.
	size_t m_OffsetInBuffer;	// The offset in bytes of the element in the buffer of the vertex declaration.
	EFormat m_Format;			// The format or type of the element.
};

/** Abstract class encapsulating a list of vertex input elements.
@remarks
		
*/
class AVertexDeclaration {
	typedef std::list<CInputElement> TElementList;

	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	AVertexDeclaration();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	virtual ~AVertexDeclaration();

	/** Add an element to the list of elements in the declaration.
	 	@author Hayden Asplet
		@param semanticName The literal string of the name of the semantic.
		@param offsetInBuffer The offset in bytes of the element in the buffer of the vertex declaration.
		@param format The format or type of the element.
	*/
	void AddElement(const std::string& semanticName, size_t offsetInBuffer, CInputElement::EFormat format);

	/** Add an input element to the list of the elements in the vertex declaration.
	 	@author Hayden Asplet
	*/
	void AddElement(const CInputElement& inputElement);

	/** Remove an element from the declaration.
	 	@author Hayden Asplet
	 	@param index Index of the element to remove.
	*/
	void RemoveElement(size_t index);

	/** Remove all elements of the declaration.
	 	@author Hayden Asplet
	*/
	void ClearElements(); 

	/** Get an element from the declaration.
	 	@author Hayden Asplet
	 	@param index Index of the element to get.
	*/
	const CInputElement& GetElement(size_t index) const;

	/** Get the number of elements in the list.
	 	@author Hayden Asplet
	 	@return Number of elements currently in the declaration.
	*/
	int GetNumElements() const;
protected:
private:
	virtual void VAddElement(const CInputElement& inputElement) = 0;
	virtual void VRemoveElement(size_t index) = 0;
	virtual void VClearElements() = 0;
		
	// Member Variables
public:
protected:
	TElementList m_ElementList;
private:
};

}

#endif	// __SLIMINPUTELEMENT_H__