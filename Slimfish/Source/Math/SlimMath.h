// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimMath.h
// Description	: Standard math functions declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMMATH_H__
#define __SLIMMATH_H__

// Library Includes
#include <random>

// Local Includes

namespace Slim {

/** Class that defines many common math functions and constants.
@remarks
	This class is not intended to be used as a class at all. Instead it's
	to be used like a namespace (hence the lack of proper naming conventions). 
	The reason for the class is to encapsulate implementation details that 
	otherwise would be exposed had Math be a namespace. 
*/
class Math : private ASingleton<Math> {
	// Member Functions
public:
	Math();

	/** Destructor.
	 	@author Hayden Asplet
	*/
	~Math() = default;

	/** Convert from radians to degrees. @author Hayden Asplet */
	static inline float RadiansToDegrees(float radians)
	{
		return radians * s_RADIANS_TO_DEGREES;
	}

	/** Convert from degrees to radians. @author Hayden Asplet */
	static inline float DegreesToRadians(float degrees)
	{
		return degrees * s_DEGREES_TO_RADIANS;
	}

	/** Retrieve a random number in the range [0, 1]. @author Hayden Asplet */
	static inline float Random() 
	{
		std::uniform_real_distribution<float> distribution(0, 1);
		return distribution(ASingleton::GetInstance()->m_RandomGenerator);
	}

	static inline void Seed(size_t seed)
	{
		ASingleton::GetInstance()->m_Seed = seed;
		ASingleton::GetInstance()->m_RandomGenerator.seed(seed);
	}

	static inline size_t GetSeed()
	{
		return ASingleton::GetInstance()->m_Seed;
	}

	/** Retrieve a random integer in the range [min,max).
		@remarks
			Returns the minimum value whenever the range is [min,min] (asin whenever min == max - 1).
			In the case where the range is [min,min] the result is therefore not random.
	 	@author Hayden Asplet
		@param min The minimum value in the range (inclusive).
		@param max The maximum value in the range (exclusive).
	*/
	static inline int Random(int min, int max) 
	{
		assert(min < max);
		if (min == max - 1) {
			// Only one value between [min,max) and that's min.
			return min;
		}
		else {
			std::uniform_int_distribution<int> distribution(min, max - 1);
			return distribution(ASingleton::GetInstance()->m_RandomGenerator);
		}
	}

	/** Retrieves a random floating-point number within the range [min, max].
	 	@author Hayden Asplet
	 	@param min The minimum value in the range (inclusive).
		@param max The maximum value in the range (inclusive).
	*/
	static inline float Random(float min, float max) {
		assert(min < max);
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(ASingleton::GetInstance()->m_RandomGenerator);
	}
protected:
private:

	// Member Variables
public:
	static const float s_PI;
	static const float s_DEGREES_TO_RADIANS;
	static const float s_RADIANS_TO_DEGREES;
protected:
private:
	std::mt19937 m_RandomGenerator;
	size_t m_Seed;
};

}

#endif	// __SLIMMATH_H__