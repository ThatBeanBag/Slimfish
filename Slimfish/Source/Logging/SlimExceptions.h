// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimExceptions.h
// Description	: Declaration file for various exception classes.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMEXCEPTIONS_H__
#define __SLIMEXCEPTIONS_H__

// Library Includes
#include <exception>

// Local Includes

namespace Slim {

	class CException : public std::exception {
		// Member Functions
	public:
		CException();
		~CException();

	protected:
	private:
		// Member Variables
	public:
	protected:
		unsigned int m_Line;
		std::string m_File;
		std::string m_FunctionName;
		std::string m_Description;
	private:
	};
}


#endif	// __SLIMEXCEPTIONS_H__