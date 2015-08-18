// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimShaderParams.h
// Description	: CSlimShaderParams declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMSHADERPARAMS_H__
#define __SLIMSHADERPARAMS_H__

// Library Includes

// Local Includes

namespace Slim {

/** Class representing a constant definition in a shader.
@remarks
		
*/
class CShaderConstant {
public:
	enum EConstantType {
		TYPE_INT,
		TYPE_INT2,
		TYPE_INT3,
		TYPE_INT4,
		TYPE_FLOAT,
		TYPE_FLOAT2,
		TYPE_FLOAT3,
		TYPE_FLOAT4,
		TYPE_MATRIX4X4,
		TYPE_SAMPLER1D,
		TYPE_SAMPLER2D,
		TYPE_SAMPLER3D,
		TYPE_SAMPLERCUBE
	};

	// Member Functions
public:

	CShaderConstant();
	/** Constructor
	 	@author Hayden Asplet
	*/
	CShaderConstant(EConstantType type, size_t index = 0);

	/** Destructor
	 	@author Hayden Asplet
	*/
	~CShaderConstant();

	/** Return true if this constant is a float. @author Hayden Asplet */
	bool IsFloat() const;
	/** Return true if this constant is an int. @author Hayden Asplet */
	bool IsInt() const;
	/** Return true if this constant is a sampler. @author Hayden Asplet */
	bool IsSampler() const;

	static const size_t GetSizeFromType(EConstantType type);
protected:
private:
	
	// Member Variables
public:
	EConstantType m_Type;
	size_t m_Index;
	size_t m_Size;
protected:
private:
};

/** Class representing a group of parameters for shader programs.
@remarks
	Parameters are constant values that shaders will use to perform their operations.
@par
	Parameters are mapped to matching names that are used to identify individual parameters
	from within shader programs. 
*/
class CShaderParams {
	typedef std::vector<float> ConstantFloatList;
	typedef std::vector<int> ConstantIntList;
	typedef std::map<std::string, CShaderConstant> NamedConstantMap;

	// Member Functions
public:
	/** Constructor
	 	@author Hayden Asplet
	*/
	CShaderParams();

	/** Destructor
		@author Hayden Asplet
	*/
	~CShaderParams();

	/** Adds a constant definition to the list of parameters.
		@note This must be called before SetConstant()
	 	@author Hayden Asplet
	 	@param name The name of the constant definition.
	 	@param type The type of constant definition e.g. TYPE_INT, TYPE_INT2, TYPE_FLOAT, TYPE_MATRI4X4 etc.
	*/
	void AddConstant(const std::string& name, CShaderConstant::EConstantType type);

	/** Clears all the constant definitions. @author Hayden Asplet */
	void ClearConstants();

	/** Set a constant definition as a float value.
		@remarks
			The constant definition has to be added first using AddConstant() and the appropriate
			constant type must be specified.
	 	@author Hayden Asplet
	*/
	void SetConstant(const std::string& name, float value);

	/** Set a constant definition as an int value.
		@remarks
			The constant definition has to be added first using AddConstant() and the appropriate
			constant type must be specified.
		@author Hayden Asplet
	*/
	void SetConstant(const std::string& name, int value);

	/** Set a constant definition as a vector.
		@remarks
			The constant definition has to be added first using AddConstant() and the appropriate
			constant type must be specified.
		@author Hayden Asplet
	*/
	void SetConstant(const std::string& name, const CVector3& value);

	/** Set a constant definition as a 4 by 4 matrix.
		@remarks
			The constant definition has to be added first using AddConstant() and the appropriate
			constant type must be specified.
		@author Hayden Asplet
	*/
	void SetConstant(const std::string& name, const CMatrix4x4& value);

	/** Set a constant definition as a colour value.
		@remarks
			The constant definition has to be added first using AddConstant() and the appropriate
			constant type must be specified.
		@author Hayden Asplet
	*/
	void SetConstant(const std::string& name, const TColourValue& value);

	/** Set a constant definition as an array of floats.
		@remarks
			The constant definition has to be added first using AddConstant() and the appropriate
			constant type must be specified.
		@author Hayden Asplet
	*/
	void SetConstant(const std::string& name, const float* pValue, size_t count);

	/** Set a constant definition as an array of ints.
		@remarks
			The constant definition has to be added first using AddConstant() and the appropriate
			constant type must be specified.
		@author Hayden Asplet
	*/
	void SetConstant(const std::string& name, const int* pValue, size_t count);

	/** Get a constant definition.
		@note TODO: describe throwing of error.
	 	@author Hayden Asplet
	*/
	const CShaderConstant GetConstant(const std::string& name);

	/** Get the list of constant float elements. @author Hayden Asplet */
	const ConstantFloatList& GetConstantFloatList() const;

	/** Get the list of constant int elements. @author Hayden Asplet */
	const ConstantIntList& GetConstantIntList() const;
protected:
private:
	// Member Variables
public:
protected:
private:
	ConstantFloatList m_ConstantFloats;
	ConstantIntList m_ConstantInts;
	NamedConstantMap m_NamedConstantMap;
};

}

#endif	// __SLIMSHADERPARAMS_H__