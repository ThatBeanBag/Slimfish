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

	template<class T>
	inline void SafeDelete(T* pObject) 
	{
		if (pObject) {
			delete pObject;
			pObject = nullptr;
		}
	}

	template<class T>
	inline void SafeDeleteArray(T* pArray)
	{
		if (pArray) {
			delete[] pArray;
			pArray = nullptr;
		}
	}

	template<class T>
	inline void SafeRelease(T* pObject) 
	{
		if (pObject) {
			pObject->Release();
			pObject = nullptr;
		}
	}

}

#endif // __SLIMINLINE_H__