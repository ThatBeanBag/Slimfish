// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: collision_shape.h
// Description	: ACollisionShape declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __COLLISION_SHAPE_H__
#define __COLLISION_SHAPE_H__

// Library Includes

// Local Includes
#include <Math\SlimPlane.h>

class ACollisionShape {
public:
	/** List of the types of collisions shapes a physics object may have.
	@remarks
		Add more type's here to extend the range of shapes a physics object may have.
	*/
	enum class EType {
		SPHERE,
		BOX,
		CAPSULE,
		PLANE,
		CYLINDER,
		PYRAMID,
		CONE,			// Unsupported.
		COMPOUND,		// Unsupported.
		CONVEX_MESH,	// Unsupported.
		CONCAVE_MESH,	// Unsupported.
		TOTAL
	};

	// Member Functions
public:
	/** Default constructor. 
	 	@author Hayden Asplet
	*/
	ACollisionShape();

	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~ACollisionShape();

	/** Check to see if a point is inside the shape.
	 	@author Hayden Asplet
	*/
	virtual const bool IsInside(const CVector3& point) = 0;

	/** Get the type of the collision shape.
		@note Sub classes should override this and return their type.
		@author Hayden Asplet
	*/
	virtual const EType VGetType() const = 0;

	/** Get the volume of the collision shape. @author Hayden Asplet */
	virtual const float VGetVolume() const = 0;

	/** Set the transform. @author Hayden Asplet */
	void SetTransform(const CMatrix4x4& transform);
	/** Get the transform. @author Hayden Asplet */
	const CMatrix4x4& GetTransform() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	CMatrix4x4 m_Transform;
};

class CCollisionSphere : public ACollisionShape {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	 	@param _fRadius Radius of the sphere.
	*/
	CCollisionSphere(float _fRadius);

	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~CCollisionSphere();

	virtual const bool IsInside(const CVector3& point);
	virtual const EType VGetType() const override;
	virtual const float VGetVolume() const override;

	/** Set the radius. @author Hayden Asplet */
	void SetRadius(float radius);
	/** Get the radius. @author Hayden Asplet */
	const float GetRadius() const;
protected:
private:
	
	// Member Variables
public:
protected:
private:
	float m_fRadius;
};

class CCollisionPyramid : public ACollisionShape {
	// Member Functions
public:
	/** Default constructor.
		@author Hayden Asplet
	*/
	CCollisionPyramid(float width, float height, float length);

	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~CCollisionPyramid();

	virtual const bool IsInside(const CVector3& point);
	virtual const EType VGetType() const override;
	virtual const float VGetVolume() const override;

	/** Set the width. @author Hayden Asplet */
	void SetWidth(float width);
	/** Get the width. @author Hayden Asplet */
	const float GetWidth() const;
	/** Set the height. @author Hayden Asplet */
	void SetHeight(float height);
	/** Get the height. @author Hayden Asplet */
	const float GetHeight() const;
	/** Set the length. @author Hayden Asplet */
	void SetLength(float length);
	/** Get the length. @author Hayden Asplet */
	const float GetLength() const;
protected:
private:

	// Member Variables
public:
protected:
private:
	float m_Width;
	float m_Height;
	float m_Length;
};

class CCollisionCapsule : public ACollisionShape {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
		@param _fRadius Radius of the capsule.
		@param _fHeight Height of the capsule.
	*/
	CCollisionCapsule(float _fRadius, float _fHeight);

	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~CCollisionCapsule();

	virtual const bool IsInside(const CVector3& point);
	virtual const EType VGetType() const override;
	virtual const float VGetVolume() const override;

	/** Set the radius. @author Hayden Asplet */
	void SetRadius(float radius);
	/** Get the radius. @author Hayden Asplet */
	const float GetRadius() const;
	/** Set the height. @author Hayden Asplet */
	void SetHeight(float height);
	/** Get the height. @author Hayden Asplet */
	const float GetHeight() const;


protected:
private:

	// Member Variables
public:
protected:
private:
	float m_fRadius;
	float m_fHeight;
};

class CCollisionBox : public ACollisionShape {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
		@param _fWidth Width of the box (size along x-axis).
		@param _fHeight Height of the box (size along y-axis).
		@param _fLength Length of the box (size along z-axis).
	*/
	CCollisionBox(float _fWidth, float _fHeight, float _fLength);

	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~CCollisionBox();

	virtual const bool IsInside(const CVector3& point);
	virtual const EType VGetType() const override;
	virtual const float VGetVolume() const override;
	
	/** Set the width. @author Hayden Asplet */
	void SetWidth(float width);
	/** Get the width. @author Hayden Asplet */
	const float GetWidth() const;
	/** Set the height. @author Hayden Asplet */
	void SetHeight(float height);
	/** Get the height. @author Hayden Asplet */
	const float GetHeight() const;
	/** Set the length. @author Hayden Asplet */
	void SetLength(float length);
	/** Get the length. @author Hayden Asplet */
	const float GetLength() const;


protected:
private:
	
	// Member Variables
public:
protected:
private:
	float m_fWidth;
	float m_fHeight;
	float m_fLength;
};

class CCollisionCylinder : public ACollisionShape {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	*/
	CCollisionCylinder(float _fRadius, float _fHeight);

	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~CCollisionCylinder();

	virtual const bool IsInside(const CVector3& point);
	virtual const EType VGetType() const override;
	virtual const float VGetVolume() const override;

	/** Set the radius. @author Hayden Asplet */
	void SetRadius(float radius);
	/** Get the radius. @author Hayden Asplet */
	const float GetRadius() const;
	/** Set the height. @author Hayden Asplet */
	void SetHeight(float height);
	/** Get the height. @author Hayden Asplet */
	const float GetHeight() const;
protected:
private:

	// Member Variables
public:
protected:
private:
	float m_fRadius;
	float m_fHeight;
};

class CCollisionPlane : public ACollisionShape {
	// Member Functions
public:
	/** Default constructor.
	 	@author Hayden Asplet
	 	@param _krNormal Normal of the plane.
	 	@param _fConstant The constant or D component of the plane equation.
	*/
	CCollisionPlane(const CPlane& plane);

	/** Destructor.
	 	@author Hayden Asplet
	*/
	virtual ~CCollisionPlane();
	
	virtual const bool IsInside(const CVector3& point);
	virtual const EType VGetType() const override;
	virtual const float VGetVolume() const override;

	/** Set the plane. @author Hayden Asplet */
	void SetPlane(const CPlane& plane);
	/** Get the plane. @author Hayden Asplet */
	const CPlane& GetPlane() const;
protected:
private:

	// Member Variables
public:
protected:
private:
	CPlane m_Plane;
};

#endif	// __COLLISION_SHAPE_H__