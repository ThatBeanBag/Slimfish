// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimAxisAlignedBoundingBox.h
// Description	: CSlimAxisAlignedBoundingBox declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMAXISALIGNEDBOUNDINGBOX_H__
#define __SLIMAXISALIGNEDBOUNDINGBOX_H__

// Library Includes

// Local Includes

namespace Slim {

/** Class representing an axis-aligned bounding box.
@remarks
	
*/
class CAxisAlignedBoundingBox {
	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
		*/
	CAxisAlignedBoundingBox();

	/** Construct an axis aligned bounding box from minimum and maximum points.
		@author Hayden Asplet
		*/
	CAxisAlignedBoundingBox(const CVector3& min, const CVector3& max);

	/** Destructor
		@author Hayden Asplet
		*/
	~CAxisAlignedBoundingBox();

	/** Extend the bounding box to also entirely encompass a given bounding box.
		@remarks
		If the bounding box already encompasses the input bounding box the box is left unchanged.
		Thus the resultant box encompasses both the existing bounding box and the new one.
		@author Hayden Asplet
		*/
	void Encompass(const CAxisAlignedBoundingBox& other);

	/** Extend the bounding box to also entirely encompass a given point.
		@remarks
		If the bounding box already encompasses the given point the box is left unchanged.
		Thus the resultant box encompasses both the existing bounding box and the given point.
		@author Hayden Asplet
		*/
	void Encompass(const CVector3& point);

	/** Transform the bounding box by a transformation matrix.
		@remarks
		The resultant box remains axis-aligned. Effectively each corner of the
		box is transform and the minimum and maximum points are deduced from the
		transformed points to form the resultant axis-aligned bounding box.
		@author Hayden Asplet
		*/
	void Transform(const CMatrix4x4& transform);

	/** Get the centre position of the box.
		@remarks
		Retrieves the point halfway between the minimum and maximum points.
		@author Hayden Asplet
		*/
	const CVector3 GetCentre();

	/** Set the minimum corner of the box. @author Hayden Asplet */
	void SetMin(const CVector3& min);
	/** Get the minimum corner of the box. @author Hayden Asplet */
	const CVector3& GetMin() const;
	/** Set the maximum corner of the box. @author Hayden Asplet */
	void SetMax(const CVector3& max);
	/** Get the maximum corner of the box. @author Hayden Asplet */
	const CVector3& GetMax() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	CVector3 m_Min;
	CVector3 m_Max;
};

}

#endif	// __SLIMAXISALIGNEDBOUNDINGBOX_H__