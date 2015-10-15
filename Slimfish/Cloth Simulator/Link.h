// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Link.h
// Description	: CLink declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __LINK_H__
#define __LINK_H__

// Library Includes

// Local Includes

// Forward Declaration.
class CPointMass;

class CLink {
	// Member Functions
public:
	CLink(CPointMass* pPointMassA, CPointMass* pPointMassB, float restingDistance, float stiffness, float breakForce);
	~CLink();

	void Solve();

	/** Set the pointMassA. @author Hayden Asplet */
	void SetPointMassA(CPointMass* pointMassA);
	/** Get the pointMassA. @author Hayden Asplet */
	const CPointMass* GetPointMassA() const;
	/** Set the pointMassB. @author Hayden Asplet */
	void SetPointMassB(CPointMass* pointMassB);
	/** Get the pointMassB. @author Hayden Asplet */
	const CPointMass* GetPointMassB() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	float m_RestingDistance;
	float m_Stiffness;
	float m_BreakForce;
	CPointMass* m_pPointMassA;
	CPointMass* m_pPointMassB;
};

#endif	// __LINK_H__