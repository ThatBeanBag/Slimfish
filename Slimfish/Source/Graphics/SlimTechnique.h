// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimTechnique.h
// Description	: CTechnique declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMTECHNIQUE_H__
#define __SLIMTECHNIQUE_H__

// Library Includes

// Local Includes

namespace Slim {

class CRenderPass;

/** Class representing a single rendering technique.
	@remarks
		A single rendering technique consists of one or more render passes.
		Techniques can be used to define levels of detail for systems with
		limited hardware or for objects in the distance that may require a
		less intensive rendering technique.
*/
class CTechnique {
	using TRenderPasses = std::vector<std::unique_ptr<CRenderPass> >;

	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CTechnique();

	/** Destructor.
		@author Hayden Asplet
	*/
	~CTechnique();

	/** Copy constructor.
	 	@author Hayden Asplet.
	*/
	CTechnique(const CTechnique& other);

	/** Assignment operator.
	 	@author Hayden Asplet
	*/
	CTechnique& operator=(const CTechnique& other);

	/** Create a new render pass and add it to the technique.
	 	@author Hayden Asplet
	 	@return Pointer to the newly created render pass.
	*/
	CRenderPass* CreateRenderPass();

	/** Add a render pass to the technique.
	 	@author Hayden Asplet
	 	@param pass Pass to copy and add to the technique.
	*/
	CRenderPass* AddRenderPass(const CRenderPass& pass);

	/** Retrieve a render pass of the technique at a given index.
	 	@author Hayden Asplet
	*/
	CRenderPass* GetRenderPass(size_t index);

	/** Remove a specific render pass of the technique.
	 	@author Hayden Asplet
	*/
	void RemoveRenderPass(size_t index);

	/** Clear all render passes of the technique.
	 	@author Hayden Asplet
	*/
	void ClearRenderPasses();

	/** Get the number of render passes the technique has.
	 	@author Hayden Asplet
	*/
	const size_t GetNumRenderPasses() const;
protected:
private:
	/** Internal convenience method for copying techniques.
	 	@author Hayden Asplet
		@param other The technique to copy.
	*/
	void Copy(const CTechnique& other);

	// Member Variables
public:
protected:
private:
	TRenderPasses m_RenderPasses;
};

}

#endif // __SLIMTECHNIQUE_H__