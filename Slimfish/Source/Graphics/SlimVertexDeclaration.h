// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimVertexDeclaration.h
// Description	: AVertexDelcaration and CInputElement declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMVERTEXDECLARATION_H__
#define __SLIMVERTEXDECLARATION_H__

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
	CInputElement(const std::string& semanticName, EFormat format);

	/** Destructor.
		@author Hayden Asplet
	*/
	~CInputElement();

	/** Get the name of the semantic @author Hayden Asplet */
	const std::string& GetSemanticName() const;

	/** Get the offset in bytes of the element in the buffer. @author Hayden Asplet */
	const size_t GetSize() const;

	/** Get the format/type of the input element. @author Hayden Asplet */
	const EFormat GetFormat() const;
protected:
private:
	static const size_t GetSizeFromFormat(EFormat format);

	// Member Variables
public:
protected:
private:
	std::string m_SemanticName;	// The literal string of the name of the semantic.
	size_t m_Size;				// The size in bytes of the element.
	EFormat m_Format;			// The format or type of the element.
};

/** Abstract class encapsulating a list of vertex input elements.
@remarks
	Describes how vertex data in a buffer is to be interpreted by the rendering pipeline.
*/
class CVertexDeclaration {
	typedef std::vector<CInputElement> TElementList;
public:
	/** List of primitive types for describing how vertex data gets interpreted by the 
		rendering system.
	*/
	enum EPrimitiveType {
		// A list of points.
		PRIMITIVE_TYPE_POINTLIST,
		// A list of lines, 2 vertices to a line.
		PRIMITIVE_TYPE_LINELIST,
		// A strip of connected lines, each new vertex adds a new line (excluding the first).
		PRIMITIVE_TYPE_LINESTRIP,
		// A list of triangles, 3 vertices per triangle.
		PRIMITIVE_TYPE_TRIANGLELIST,
		// A strip of connected triangles, each new vertex adds a new triangle (excluding the first two).
		PRIMITIVE_TYPE_TRIANGLESTRIP,
		// A fan of triangles, similar to PRIMITIVE_TYPE_TRIANGLESTRIP except each triangle shares one 
		// central vertex (the first vertex).
		//PRIMITIVE_TYPE_TRIANGLEFAN	// Not used in directX 10.
	};

	// Member Functions
public:
	/** Default constructor.
		@note Sets the primitive type to PRIMITIVE_TYPE_TRIANGLELIST by default.
	 	@author Hayden Asplet
	*/
	CVertexDeclaration();

	/** Construct a vertex declaration by specifying the primitive type to use.
	 	@author Hayden Asplet
	*/
	CVertexDeclaration(EPrimitiveType primitiveType);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CVertexDeclaration();

	/** Add an element to the list of elements in the declaration.
	 	@author Hayden Asplet
		@param semanticName The literal string of the name of the semantic.
		@param format The format or type of the element.
	*/
	void AddElement(const std::string& semanticName, CInputElement::EFormat format);

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

	/** Get the number of elements in the list. @author Hayden Asplet */
	int GetNumElements() const;

	/** Set the primitive type of the vertex declaration. @author Hayden Asplet */
	void SetPrimitiveType(EPrimitiveType primitiveType);
	/** Get the primitive type described by this vertex declaration. @author Hayden Asplet */
	EPrimitiveType GetPrimitiveType() const;

	void SetRebuilt();
	const bool NeedsRebuilding() const;
protected:
private:
	/** Internal delegating method for derived classes to implement. See AddElement().
	 	@author Hayden Asplet
	*/
	//virtual void VAddElement(const CInputElement& inputElement) = 0;

	/** Internal delegating method for derived classes to implement. See RemoveElement().
	 	@author Hayden Asplet
	*/
	//virtual void VRemoveElement(size_t index) = 0;

	/** Internal delegating method for derived classes to implement. See ClearElements().
	 	@author Hayden Asplet
	*/
	//virtual void VClearElements() = 0;
		
	// Member Variables
public:
protected:
private:
	TElementList m_ElementList;	// The list of input elements that make up the vertex declaration.
	EPrimitiveType m_PrimitiveType;

	bool m_NeedsRebuilding;
};

}

#endif	// __SLIMVERTEXDECLARATION_H__