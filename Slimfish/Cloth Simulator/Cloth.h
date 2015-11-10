// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: Cloth.h
// Description	: CCloth declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __CLOTH_H__
#define __CLOTH_H__

// Library Includes

// Local Includes
#include "PointMass.h"

class CCloth {
	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CCloth();
	
	/** Destructor
		@author Hayden Asplet
	*/
	~CCloth();

	/** Initialise the cloth.
	 	@author Hayden Asplet
	*/
	void Initalise();

	/** Update the cloth simulation.
	 	@author Hayden Asplet
	 	@param deltaTime Time since last update.
	*/
	void Update(float deltaTime);

	/** Attach the pins based on how many hooks that cloth has.
	 	@author Hayden Asplet
	*/
	void PinCloth();

	/** Unpin the cloth from the hooks.
	 	@author Hayden Asplet
	*/
	void UnPin();

	void BurnCloth(size_t x, size_t y);

	CPointMass* GetPointMass(size_t x, size_t y);
	const std::vector<CPointMass*>& GetPinnedPoints();

	/** Set the  number point masses on the x axis.  @author Hayden Asplet */
	void SetNumPointMassesX(size_t numPointMassesX);
	/** Get the  number point masses on the x axis.  @author Hayden Asplet */
	const size_t GetNumPointMassesX() const;
	/** Set the number point masses on the y axis. @author Hayden Asplet */
	void SetNumPointMassesY(size_t numPointMassesY);
	/** Get the number point masses on the y axis. @author Hayden Asplet */
	const size_t GetNumPointMassesY() const;
	/** Set the num hooks. @author Hayden Asplet */
	void SetNumHooks(size_t numHooks);
	/** Get the num hooks. @author Hayden Asplet */
	const size_t GetNumHooks() const;
	/** Set the link breaking distance. @author Hayden Asplet */
	void SetLinkBreakingDistance(float linkBreakingDistance);
	/** Get the link breaking distance. @author Hayden Asplet */
	const float GetLinkBreakingDistance() const;
	/** Set the point mass damping. @author Hayden Asplet */
	void SetPointMassDamping(float pointMassDamping);
	/** Get the point mass damping. @author Hayden Asplet */
	const float GetPointMassDamping() const;
	/** Set the point mass mass. @author Hayden Asplet */
	void SetPointMassMass(float pointMassMass);
	/** Get the point mass mass. @author Hayden Asplet */
	const float GetPointMassMass() const;
	/** Set the hung from height. @author Hayden Asplet */
	void SetHungFromHeight(float hungFromHeight);
	/** Get the hung from height. @author Hayden Asplet */
	const float GetHungFromHeight() const;
	/** Set the resting distance. @author Hayden Asplet */
	void SetRestingDistance(float restingDistance);
	/** Get the resting distance. @author Hayden Asplet */
	const float GetRestingDistance() const;
	/** Set the stiffness. @author Hayden Asplet */
	void SetStiffness(float stiffness);
	/** Get the stiffness. @author Hayden Asplet */
	const float GetStiffness() const;
	/** Get the point masses. @author Hayden Asplet */
	const std::vector<std::unique_ptr<CPointMass> >& GetPointMasses() const;
protected:
private:
	void UpdateBurning(float deltaTime);

	// Member Variables
public:
protected:
private:
	std::vector<std::unique_ptr<CPointMass> > m_PointMasses;

	size_t m_NumMassesX;
	size_t m_NumMassesY;
	size_t m_NumHooks;

	float m_LinkBreakingDistance;
	float m_PointMassDamping;
	float m_PointMassMass;
	float m_HungFromHeight;
	float m_RestingDistance;
	float m_Stiffness;

	std::vector<CPointMass*> m_PinnedPoints;

	// Burning.
	bool m_IsBurning;
	float m_BurningTimer;
	float m_BurningRate = { 0.07f };
	float m_PropagateBurningThreshold = { 0.5f };
	float m_MinBurningDistance = { 0.1f };
	float m_MaxBurningDistance = { 4.0f };
	float m_ChanceToPropagate = { 0.01f };
	//std::vector<CPoint> m_BurningPoints;

	std::vector<CPointMass*> m_BurningPoints;
};

#endif	// __CLOTH_H__