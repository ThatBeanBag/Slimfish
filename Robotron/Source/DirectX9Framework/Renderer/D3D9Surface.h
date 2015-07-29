// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: D3D9Surface.h
// Description	: CD3D9Surface declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __D3D9SURFACE_H__
#define __D3D9SURFACE_H__

// Library Includes

// Local Includes

class CD3D9Surface {
	// Member Functions
public:
	CD3D9Surface();
	~CD3D9Surface();

	
protected:
private:
	// Member Variables
public:
protected:
private:
	IDirect3DSurface9* m_pSurface;

};

#endif	// __D3D9SPRITE_H__