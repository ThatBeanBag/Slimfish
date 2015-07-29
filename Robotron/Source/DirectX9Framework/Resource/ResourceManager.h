// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: ResourceManager.h
// Description	: CResourceManager declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

// Library Includes

// Local Includes

class TiXmlDocument;

template<typename TResource>
class CResourceManager {
	// Member Functions
	typedef std::map<std::string, shared_ptr<TResource> > TResourceMap;
public:
	~CResourceManager();

	static std::unique_ptr<CResourceManager>& GetInstance();

	shared_ptr<TResource> Load(const std::string _krstrFileName);
	bool Unload(const std::string _krstrFileName);
	bool Reload(const std::string _krstrFileName);
	void Flush();
protected:
private:
	CResourceManager();

	CResourceManager(const CResourceManager& _krResourceManager) = delete;
	CResourceManager& operator=(const CResourceManager& _krResourceManager) = delete;
	
	// Member Variables
public:
protected:
private:
	static unique_ptr<CResourceManager> s_pInstance;

	TResourceMap m_resourceMap;
};

template<typename TResource>
bool CResourceManager<TResource>::Reload(const std::string _krstrFileName)
{
	Unload(_krstrFileName);
	return Load(_krstrFileName) != nullptr;
}

template<typename TResource>
unique_ptr<CResourceManager<TResource> > CResourceManager<TResource>::s_pInstance = nullptr;

template<typename TResource>
CResourceManager<TResource>::~CResourceManager()
{

}

template<typename TResource>
std::unique_ptr<CResourceManager<TResource> >& CResourceManager<TResource>::GetInstance()
{
	if (!s_pInstance) {
		s_pInstance = std::unique_ptr<CResourceManager<TResource> >(new CResourceManager<TResource>);
	}
	
	return s_pInstance;
}

template<typename TResource>
shared_ptr<TResource> CResourceManager<TResource>::Load(const std::string _krstrFileName)
{
	// TODO: Check for already loaded resource.
	auto findIter = m_resourceMap.find(_krstrFileName);
	if (findIter != m_resourceMap.end()) {
		return findIter->second;
	}

	shared_ptr<TResource> pResource = TResource::LoadResource(_krstrFileName);
	if (pResource) {
		m_resourceMap[_krstrFileName] = pResource;
	}

	return pResource;
}

template<typename TResource>
bool CResourceManager<TResource>::Unload(const std::string _krstrFileName)
{
	auto findIter = m_resourceMap.find(_krstrFileName);
	if (findIter != m_resourceMap.end()) {
		m_resourceMap.erase(findIter);
		return true;
	}

	return false;
}

template<typename TResource>
void CResourceManager<TResource>::Flush()
{
	while (!m_resourceMap.empty()) {
		m_resourceMap.pop();
	}
}

template<typename TResource>
CResourceManager<TResource>::CResourceManager()
{

}

/************************************************************************/
/* CResourceManager - A basic resource manager that helps to ensure only 
/* one instance of a resource is loaded in at a time.
/************************************************************************/
/*
class CResourceManager {
	// Member Functions
	typedef std::map<std::string, shared_ptr<IMesh> > TMeshMap;
	typedef std::map<std::string, shared_ptr<ITexture> > TTextureMap;
	typedef std::map<std::string, shared_ptr<ISprite> > TSpriteMap;
	typedef std::map<std::string, shared_ptr<TiXmlDocument> > TXmlDocumentMap;
public:
	CResourceManager();
	~CResourceManager();

	shared_ptr<IMesh> LoadMesh(const std::string& _krstrFileName);
	shared_ptr<ITexture> LoadTexture(const std::string& _krstrFileName);
	shared_ptr<ISprite> LoadSprite(const std::string& _krstrFileName);
	shared_ptr<TiXmlDocument> LoadXmlDocument(const std::string& _krstrFileName);
	bool Unload(const std::string _krstrFileName);
	void Flush();
protected:
private:
	// Member Variables
public:
protected:
private:
	TMeshMap m_meshes;
	TTextureMap m_textures;
	TSpriteMap m_sprites;
	TXmlDocumentMap m_xmlDocuments;
};*/

#endif	// __RESOURCEMANAGER_H__