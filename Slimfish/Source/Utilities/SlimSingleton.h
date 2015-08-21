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
	/** Template class implementing the basic and common singleton design pattern
		@remarks
			Classes should derive from this class if they desire to be made a 
			singleton.
	*/
	template<class T>
	class ASingleton {
		// Member Functions
	public:
		/** Default constructor
			@note 
				This is made private as singletons cannot be instantiated.
		 	@author
		 		Hayden Asplet
		*/
		ASingleton();

		/** Destructor
			@note
				Pure virtual so that ASingleton can't be created.
		 	@author
		 		Hayden Asplet
		*/
		virtual  ~ASingleton() {}

		/** Get a pointer to the singleton
			@remarks
				Creates a single instance of the singleton if one is yet to be created.
		 	@author
		 		Hayden Asplet
		*/
		static T* GetInstance();
	protected:
	private:

		/** Deleted copy constructor.
			@author
				Hayden Asplet
		*/
		ASingleton(const ASingleton<T>& _krOther) = delete;

		/** Delete assignment operator.
		 	@author
		 		Hayden Asplet
		*/
		ASingleton& operator=(const ASingleton<T>& _krOther) = delete;

		// Member Variables
	public:
	protected:
	private:
		static std::unique_ptr<T> m_pSingleton;
	};

	// Implementation

	template<class T>
	ASingleton<T>::ASingleton()
	{

	}

	template<class T>
	T* ASingleton<T>::GetInstance()
	{
		if (!m_pSingleton) {
			m_pSingleton.reset(new T());
		}

		assert(m_pSingleton);

		return m_pSingleton.get();
	}
}

#endif	// __SLIMSINGLETON_H__