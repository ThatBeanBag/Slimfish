// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderer.h
// Description	: IRenderer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMRENDERER_H__
#define __SLIMRENDERER_H__

// Library Includes

// Local Includes

namespace Slim {
	// Forward Declaration
	class CMatrix4x4;
	class CVec3;
	struct TColour;

	/************************************************************************/
	/* IRenderer
	/*
	/* IRenderer defines the interface for rendering engines. 
	/************************************************************************/
	class IRenderer {
		// Member Functions
	public:
		/**
		* Destructs the renderer.
		* 
		* @author: 	Hayden Asplet
		*/
		virtual ~IRenderer() {}

		/**
		* Sets the world transform.
		* 
		* @author: 	Hayden Asplet
		* @param:	worldTransform - world transform to set.
		* @return:  void
		*/
		virtual void VSetWorldTransform(const CMatrix4x4& worldTransform) = 0;

		/**
		* Sets the view transform
		* 
		* @author: 	Hayden Asplet
		* @param:	viewTransform - view transform to set.
		* @return:  void
		*/
		virtual void VSetViewTransform(const CMatrix4x4& viewTransform) = 0;

		/**
		* Sets the projection transform.
		* 
		* @author: 	Hayden Asplet
		* @param:	projectionTransform - projection transform to set.
		* @return:  void
		*/
		virtual void VSetProjectionTransform(const CMatrix4x4& projectionTransform) = 0;

		/**
		* Sets the ambient colour
		* 
		* @author: 	Hayden Asplet
		* @param:	colour - ambient colour to set.
		* @return:  void
		*/
		virtual void VSetAmbientColour(const TColour& colour) = 0;

		/**
		* Sets the background the colour.
		* 
		* @author: 	Hayden Asplet
		* @param:	colour - background colour to set.
		* @return:  void
		*/
		virtual void VSetBackgroundColour(const TColour& colour) = 0;
	protected:
	private:
		// Member Variables
	public:
	protected:
	private:

	};

}
#endif	// __SLIMRENDERER_H__