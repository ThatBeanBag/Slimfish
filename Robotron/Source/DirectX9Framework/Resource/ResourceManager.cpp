// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ResourceManager.cpp
// Description	: CResourceManager implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

// PCH
#include "GameStd.h"

// Library Includes

// This Include
#include "ResourceManager.h"

// Local Includes
#include "..\3rdParty\tinyxml\tinyxml.h"

/*
unique_ptr<CResourceManager<IMesh>> CResourceManager<IMesh>>::s_pInstance = nullptr;
unique_ptr<CResourceManager<ISprite>> CResourceManager<ISprite>>::s_pInstance = nullptr;
unique_ptr<CResourceManager<ITexture>> CResourceManager<ITexture>>::s_pInstance = nullptr;
unique_ptr<CResourceManager<CXmlResource> CResourceManager<CXmlResource>>::s_pInstance = nullptr;*/

/*
CResourceManager::CResourceManager()
{

}

CResourceManager::~CResourceManager()
{

}

shared_ptr<IMesh> CResourceManager::LoadMesh(const std::string& _krstrFileName)
{
	// Try to see if the mesh is already loaded in.
	auto findIter = m_meshes.find(_krstrFileName);
	if (findIter != m_meshes.end()) {
	// Has the mesh already been loaded in?
		DEBUG_ASSERT(findIter->second);
		return findIter->second;
	}
	
	// Have to load the mesh from file.
	shared_ptr<IMesh> pMesh = g_pApp->GetRenderer()->VCreateMeshFromFile(_krstrFileName);
	if (pMesh) {
	// Did the mesh succeed to load?
		// Insert into resource map and return.
		m_meshes[_krstrFileName] = pMesh;
		return pMesh;
	}
	else {
		DEBUG_WARNING("Failed to load mesh from resource \"" + _krstrFileName + "\"");
	}

	// Failed to load.
	return nullptr;
}

shared_ptr<ITexture> CResourceManager::LoadTexture(const std::string& _krstrFileName)
{
	// Try to see if the texture is already loaded in.
	auto findIter = m_textures.find(_krstrFileName);
	if (findIter != m_textures.end()) {
		// Has the mesh already been loaded in?
		DEBUG_ASSERT(findIter->second);
		return findIter->second;
	}

	// Have to load the texture from file.
	shared_ptr<ITexture> pTexture = g_pApp->GetRenderer()->VCreateTextureFromFile(_krstrFileName);
	if (pTexture) {
		// Did the sprite succeed to load?
		// Insert into resource map and return.
		m_textures[_krstrFileName] = pTexture;
		return pTexture;
	}
	else {
		DEBUG_WARNING("Failed to load texture from resource \"" + _krstrFileName + "\"");
	}

	// Failed to load.
	return nullptr;
}

shared_ptr<ISprite> CResourceManager::LoadSprite(const std::string& _krstrFileName)
{
	// Try to see if the sprite is already loaded in.
	auto findIter = m_sprites.find(_krstrFileName);
	if (findIter != m_sprites.end()) {
		// Has the sprite already been loaded in?
		DEBUG_ASSERT(findIter->second);
		return findIter->second;
	}

	// Have to load the sprite from file.
	shared_ptr<ISprite> pSprite = g_pApp->GetRenderer()->VCreateSpriteFromFile(_krstrFileName);
	if (pSprite) {
		// Did the sprite succeed to load?
		// Insert into resource map and return.
		m_sprites[_krstrFileName] = pSprite;
		return pSprite;
	}
	else {
		DEBUG_WARNING("Failed to load sprite from resource \"" + _krstrFileName + "\"");
	}

	// Failed to load.
	return nullptr;
}

shared_ptr<TiXmlDocument> CResourceManager::LoadXmlDocument(const std::string& _krstrFileName)
{
	// Try to see if the sprite is already loaded in.
	auto findIter = m_xmlDocuments.find(_krstrFileName);
	if (findIter != m_xmlDocuments.end()) {
		// Has the sprite already been loaded in?
		DEBUG_ASSERT(findIter->second);
		return findIter->second;
	}

	// Load the document.
	shared_ptr<TiXmlDocument> pXmlDocument(new TiXmlDocument);
	if (!pXmlDocument || !pXmlDocument->LoadFile(_krstrFileName.c_str())) {
	// Did the xml document fail to load?
		DEBUG_ERROR("Failed to load XML document \"" + _krstrFileName + "\"");
		return nullptr;
	}

	// Successful loading insert into map and return new document.
	m_xmlDocuments[_krstrFileName] = pXmlDocument;
	return pXmlDocument;
}

bool CResourceManager::Unload(const std::string _krstrFileName)
{
	auto findIterXml = m_xmlDocuments.find(_krstrFileName);
	if (findIterXml != m_xmlDocuments.end()) {
		m_xmlDocuments.erase(findIterXml);
		return true;
	}

	auto findIterMeshes = m_meshes.find(_krstrFileName);
	if (findIterMeshes != m_meshes.end()) {
		m_meshes.erase(findIterMeshes);
		return true;
	}

	auto findIterTextures = m_textures.find(_krstrFileName);
	if (findIterTextures != m_textures.end()) {
		m_textures.erase(findIterTextures);
		return true;
	}

	auto findIterSprite = m_sprites.find(_krstrFileName);
	if (findIterSprite != m_sprites.end()) {
		m_sprites.erase(findIterSprite);
		return true;
	}

	return false;
}

void CResourceManager::Flush()
{
	while (!m_meshes.empty()) {
		m_meshes.pop();
	}

	while (!m_textures.empty()) {
		m_meshes.pop();
	}

	while (!m_sprites.empty()) {
		m_meshes.pop();
	}

	while (!m_xmlDocuments.empty()) {
		m_meshes.pop();
	}
}
*/
