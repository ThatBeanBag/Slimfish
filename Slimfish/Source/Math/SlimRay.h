// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRay.h
// Description	: CSlimRay declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMRAY_H__
#define __SLIMRAY_H__

// Library Includes

// Local Includes

namespace Slim {

/** Class representing a ray in 3D space.
@remarks
	This class is used primarily for performing ray casting.
@par
	It helps to convert a 2D point on the screen to a 3D ray in world space. Using a
	camera it's easy to determine the ray at which the user clicked on the screen.

*/
class CRay {
	// Member Functions
public:
	/** Default Constructor.
	 	@author Hayden Asplet
	*/
	CRay() = default;

	/** Construct a ray from an origin and a direction.
	 	@author Hayden Asplet
	*/
	CRay(const CVector3& origin, const CVector3& direction);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~CRay();

	/** Get a point along the ray.
	 	@author Hayden Asplet
	 	@param distance Distance along the ray to point is at.
	*/
	const CVector3 GetPoint(float distance) const;

	/** Get the distance from the ray to a point in 3D space.
		@note 
			Does not take into account the length (distance the ray extends) of the ray.
		@author Hayden Asplet
	*/
	const float GetDistanceToPoint(const CVector3& point) const;

	/** Get the squared distance from the ray to a point in 3D space.
		@remarks
			This method is similar to GetDistanceToPoint(), but it avoids sqaure rooting.
			This helps to improve performance in situations where the distance is used in a
			comparison.
	 	@author Hayden Asplet
	*/
	const float GetDistanceToPointSquared(const CVector3& point) const;

	/** Get the point of intersection on the ray from a given point.
	 	@author Hayden Asplet
	*/
	const CVector3 GetIntersectionPointOnRay(const CVector3& point) const;

	/** Set the origin. @author Hayden Asplet */
	void SetOrigin(const CVector3& origin);
	/** Get the origin. @author Hayden Asplet */
	const CVector3& GetOrigin() const;
	/** Set the direction. @author Hayden Asplet */
	void SetDirection(const CVector3& direction);
	/** Get the direction. @author Hayden Asplet */
	const CVector3& GetDirection() const;

	/** Creates a ray from a start to and end point.
	 	@author Hayden Asplet
	 	@param start Origin or start of the ray in 3D space.
	 	@param end End of the ray in 3D space.
	*/
	static const CRay BuildFromPoints(const CVector3& start, const CVector3& end);
protected:
private:
	// Member Variables
public:
protected:
private:
	// Origin or start of the ray in 3D space.
	CVector3 m_Origin;
	// Direction of the ray.
	CVector3 m_Direction;
};

}

#endif	// __SLIMRAY_H__
