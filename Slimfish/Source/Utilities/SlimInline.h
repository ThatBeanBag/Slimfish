// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimInline.h
// Description	: CSlimInline declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMINLINE_H__
#define __SLIMINLINE_H__

// Library Includes

// Local Includes

namespace Slim {

#define SLIM_SAFE_DELETE(object) \
	do \
	{ \
		if(object) { \
			delete object; \
			object = nullptr; \
		} \
	} \
	while (false)

#define SLIM_SAFE_DELETE_ARRAY(object) \
	do \
	{ \
		if(object) { \
			delete[] object; \
			object = nullptr; \
		} \
	} \
	while (false)

#define SLIM_SAFE_RELEASE(object) \
	do \
	{ \
		if(object) { \
			object->Release(); \
			object = nullptr; \
		} \
	} \
	while (false)
}

#endif // __SLIMINLINE_H__