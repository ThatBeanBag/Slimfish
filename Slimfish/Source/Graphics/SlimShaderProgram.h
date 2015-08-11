// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimShaderProgram.h
// Description	: AShaderProgram declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMSHADERPROGRAM_H__
#define __SLIMSHADERPROGRAM_H__

// Library Includes

// Local Includes

namespace Slim {

	/** Abstract class representing a program that runs on the GPU.
	@remarks
		
	*/
	class AShaderProgram {
		// Member Functions
	public:
		AShaderProgram();
		virtual ~AShaderProgram();


	protected:
	private:
		// Member Variables
	public:
	protected:
	private:
	};

}

#endif	// __SLIMSHADERPROGRAM_H__