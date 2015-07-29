// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimSingleton.h
// Description	: ASingleton declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMSINGLETON_H__
#define __SLIMSINGLETON_H__

// Library Includes

// Local Includes

namespace Slim {
	/************************************************************************/
	/* ASingleton
	/*
	/* A template class which implements that basic singleton design pattern
	/* using unique_ptr.
	/*
	/* This class is intended to be base class for classes that desire to be
	/* singletons.
	/************************************************************************/
	template<class T>
	class ASingleton {
		// Member Functions
	public:
		virtual  ~ASingleton() = 0;	// pure virtual so that ASingleton is not instance able.

		/**
		* Retrieval method for getting a pointer to the singleton.
		*
		* @author: 	Hayden Asplet
		* @return:  ASingleton* - pointer to the singleton.
		*/
		static ASingleton* GetInstance();
	protected:
	private:
		/**
		* Constructor
		*
		* @author: 	Hayden Asplet
		*/
		ASingleton();

		/**
		* Copy constructor
		*
		* Singletons shouldn't be copied so this method is deleted.
		*/
		ASingleton(const ASingleton<T>& _krOther) = delete;
		/**
		* Assignment operator
		*
		* Singletons shouldn't be copied so this method is deleted.
		*/
		ASingleton& operator=(const ASingleton<T>& _krOther) = delete;

		// Member Variables
	public:
	protected:
	private:
		static std::unique_ptr<ASingleton> m_pSingleton;	// Singleton instance.
	};

	// Implementation

	template<class T>
	ASingleton<T>::ASingleton()
	{

	}

	template<class T>
	ASingleton* ASingleton<T>::GetInstance()
	{
		if (!m_pSingleton) {
			m_pSingleton.reset(new ASingleton<T>());
		}

		assert(m_pSingleton);

		return m_pSingleton.get();
	}
}

#endif	// __SLIMSINGLETON_H__