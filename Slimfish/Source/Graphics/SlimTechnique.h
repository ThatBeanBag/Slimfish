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

	CRenderPass* CreateRenderPass();
	CRenderPass* AddRenderPass(const CRenderPass& pass);
	CRenderPass* GetRenderPass(size_t index);

	void RemoveRenderPass(size_t index);
	void ClearRenderPasses();

	const size_t GetNumRenderPasses() const;
protected:
private:
	void Copy(const CTechnique& other);

	// Member Variables
public:
protected:
private:
	TRenderPasses m_renderPasses;
};

}

#endif // __SLIMTECHNIQUE_H__