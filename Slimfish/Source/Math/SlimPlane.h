// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimPlane.h
// Description	: CPlane declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMPLANE_H__
#define __SLIMPLANE_H__

// Library Includes

// Local Includes

namespace Slim {

class CPlane {
	// Member Functions
public:
	/** Default constructor. Initialize the plane to zero. 
	 	@author Hayden Asplet
	*/
	CPlane();

	/** Construct a plane from the plane equation. (ax + by + cz = d).
	 	@author Hayden Asplet
	*/
	CPlane(float a, float b, float c, float d);

	/** Construct a plane from a point on the plane and the normal of the plane.
	 	@author Hayden Asplet
	*/
	CPlane(const CVector3& point, const CVector3& normal);

	/** Construct a plane from three points on the plane.
		@remarks
			Calculates the normal from the winding order of the 3 points. A clockwise order
			points the normal forward and a counter-clockwise order points the normal backwards.
	 	@author Hayden Asplet
	*/
	CPlane(const CVector3& pointA, const CVector3& pointB, const CVector3& pointC);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CPlane();

	/** Get the normal of the plane.
	 	@author Hayden Asplet
	*/
	const CVector3 GetNormal() const;

	/** Get the constant of the plane equation.
	 	@author Hayden Asplet
	*/
	const float GetConstant() const;

	/** Set the a component of the plane equation. @author Hayden Asplet */
	void SetA(float a);
	/** Get the a component of the plane equation. @author Hayden Asplet */
	const float GetA() const;
	/** Set the b component of the plane equation. @author Hayden Asplet */
	void SetB(float b);
	/** Get the b component of the plane equation. @author Hayden Asplet */
	const float GetB() const;
	/** Set the c component of the plane equation. @author Hayden Asplet */
	void SetC(float c);
	/** Get the c component of the plane equation. @author Hayden Asplet */
	const float GetC() const;
	/** Set the d component of the plane equation. @author Hayden Asplet */
	void SetD(float d);
	/** Get the d component of the plane equation. @author Hayden Asplet */
	const float GetD() const;

	static const CPlane Normalise(const CPlane& plane);
protected:
private:
	// Member Variables
public:
protected:
private:
	float m_A;
	float m_B;
	float m_C;
	float m_D;
};

}

#endif	// __SLIMPLANE_H__