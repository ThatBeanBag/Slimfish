// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Tables.h
// Description	: Tables declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __TRITABLE_H__
#define __TRITABLE_H__

// Library Includes

// Local Includes

namespace Tables {
	extern const float g_EDGE_START[12][4];
	extern const float g_EDGE_END[12][4];
	extern const int g_EDGE_AXIS[12][4];

	extern const int g_TRI_TABLE[256][16];
	extern const int g_TRI_TABLE2[5120];

	template <typename T, std::size_t N, std::size_t M>
	std::size_t GetSize(T(&)[N][M])
	{
		return N * M;
	}

	template <typename T, std::size_t N>
	std::size_t GetSize(T(&)[N])
	{
		return N;
	}
}

#endif	// __TRITABLE_H__