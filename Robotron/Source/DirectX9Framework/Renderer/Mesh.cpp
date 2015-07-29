// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Mesh.cpp
// Description	: IMesh implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "Mesh.h"

// Local Includes

IMesh::~IMesh()
{

}

shared_ptr<IMesh> IMesh::LoadResource(const std::string& _krstrFileName)
{
	return g_pApp->GetRenderer()->VCreateMeshFromFile(_krstrFileName);
}
