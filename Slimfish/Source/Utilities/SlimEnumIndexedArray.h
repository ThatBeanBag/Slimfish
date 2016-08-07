
#pragma once

#ifndef __SLIMENUMINDEXEDARRAY_H__
#define __SLIMENUMINDEXEDARRAY_H__

// Library Includes
#include <array>

// Local Includes

namespace Slim {

/** Convenient class that allows an array to be indexed by an enum.
@remarks
	The enumerator type must have MAX as its last element, otherwise you must specify the enum
	size manually by defining the EnumSize template argument.
@par
	The underlying type of the enumerator must be integral, so that it can be used to index into
	the array.
@par
	This class simply builds on top of std::array by providing additional accessors to to the
	array's elements using an enumerator as a index. Other than that it can be used as any other
	std::array.
*/
template<typename T, typename Enum, size_t EnumSize = static_cast<size_t>(Enum::MAX)>
class CEnumIndexedArray : public std::array<T, EnumSize> {
	static_assert(std::is_enum<Enum>::value && std::is_integral<typename std::underlying_type<Enum>::type>::value,
		"Underlying type of the enumerator must be integral to be used an index");

public:
	// Alias for the underlying array type.
	using ArrayType = std::array<T, EnumSize>;

	// Member Functions
public:
	// Accessors
	/** Access a specified element with bounds checking.  */
	ArrayType::reference at(Enum index) { return this->ArrayType::at(static_cast<ArrayType::size_type>(index)); }
	/** Access a specified element with bounds checking. */
	ArrayType::const_reference at(Enum index) const { return this->ArrayType::at(static_cast<ArrayType::size_type>(index)); }

	/** Access a specified element. */
	ArrayType::reference operator[](Enum index) { return this->ArrayType::operator[](static_cast<ArrayType::size_type>(index)); }
	/** Access a specified element. */
	ArrayType::const_reference operator[](Enum index) const { return this->ArrayType::operator[](static_cast<ArrayType::size_type>(index)); }
};

}

#endif	// __SLIMENUMINDEXEDARRAY_H__
