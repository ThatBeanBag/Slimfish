// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TriTable.h
// Description	: CTriTable declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TRITABLE_H__
#define __TRITABLE_H__

// Library Includes

// Local Includes

namespace TriTable {
	extern const float g_EDGE_START[12][4];
	extern const float g_EDGE_DIRECTION[12][4];
	extern const float g_EDGE_END[12][4];
	extern const unsigned int g_EDGE_AXIS[12][4];

	extern const int g_VALUES[256][16];
	extern const int g_SIZE;
}

#endif	// __TRITABLE_H__