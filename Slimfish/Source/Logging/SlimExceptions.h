// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimExceptions.h
// Description	: Declaration file for various exception classes.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMEXCEPTIONS_H__
#define __SLIMEXCEPTIONS_H__

// Library Includes
#include <exception>

// Local Includes

namespace Slim {

	/** List of exception types.
	@remarks
		These exception types are similar to return codes, except they are purposefully
		more broad and less specific than typical return codes. They are used similarly
		to return codes in that they help to identify the type of error that occurred.
		However the more verbose details are left up to the exception description.
	*/
	enum class EExceptionType {
		// A general rendering error, such as the device failing to create.
		RENDERING,
		// An error typically related to the failure to open a file, because it does not exist.
		MISSING_FILE,
		// An error where an entry in a container is not found.
		ENTRY_NOT_FOUND,
		// Error regarding a duplicate entry found in a container of unique entry.
		DUPLICATE_ENTRY,
		// Standard out of range error, where an indexing exceeds the range of the container.
		OUT_OF_RANGE,
		// An unknown error, it is ill-advised to use this at all.
		UNKNOWN
	};

	/** Generic exception class for broad and common errors.
	@remarks
		
	*/
	class CException : public std::exception {
		// Member Functions
	public:
		/** Constructor.
		 	@author Hayden Asplet
		 	@param type The type of exception e.g.  EXCEPTION_RENDERING, EXCEPTION_FILE_NOT_FOUND etc.
		 	@param lineno The line number of the source (where the exception was thrown from).
		 	@param file The source file from which the exception was thrown from.
		 	@param function The source function from which the exception was thrown from.
		 	@param description A short description of the error.
		*/
		CException(EExceptionType type, unsigned int lineno, const std::string& file, const std::string& function, const std::string& description);

		/** Destructor.
		 	@author Hayden Asplet
		*/
		~CException() throw();

		/** Get the type of exception thrown e.g. EXCEPTION_RENDERING, EXCEPTION_FILE_NOT_FOUND etc.
			@author Hayden Asplet 
		*/
		const EExceptionType& GetType() const;

		/** Get the line number of the source (where the exception was thrown from).
			@note This is mainly used for debugging purposes and to log the exception.
		 	@author Hayden Asplet
		*/
		const unsigned int GetSourceLineNumber() const;

		/** Get the source file from where the exception was thrown.
			@note This is mainly used for debugging purposes and to log the exception.
		 	@author Hayden Asplet
		*/
		const std::string& GetSourceFile() const;

		/** Get the function from the source (where the exception was thrown from).
		 	@author Hayden Asplet
		*/
		const std::string& GetSourceFunction() const;

		/** Get the short description of the exception.
			@remarks
				Excludes the line number, file and function of where the exception was
				thrown from. For a full description use GetDescription() instead.
		 	@author Hayden Asplet
		*/
		const std::string& GetShortDescription() const;

		/** Get the full description of the exception.
			@remarks
				Includes the line number, file and function of where the exception was
				thrown from. This is particularly useful for debugging. For a simpler
				less verbose description use GetShortDescription().
		 	@author Hayden Asplet
		*/
		const std::string& GetDescription() const;

		/** Overidden function from std::exception for obtaining the description of the exception.
		 	@author Hayden Asplet
		*/
		virtual const char* what() const throw() override;
	protected:
	private:
		// Member Variables
	public:
	protected:
		EExceptionType m_Type;
		unsigned int m_LineNumber;
		std::string m_File;
		std::string m_FunctionName;
		std::string m_Description;
		std::string m_FullDescription;
	private:
	};

	/** Empty class to signal the end of an exception throw message stream.
	@remarks
		Adding this to a stream output of an exception stream will manually cause the application
		to throw a CException. It is not necessary to do this when using SLIM_THROW() as that macro
		automatically ensures the stream will throw as it's destructor will be called immediately
		after the line executes.
	*/
	class CEndThrow {};

	/** 
	@remarks
		
	*/
	class CExceptionStream {
		// Member Functions
	public:
		/** Constructor.
		 	@author Hayden Asplet
		 	@param type The type of exception e.g.  EXCEPTION_RENDERING, EXCEPTION_FILE_NOT_FOUND etc.
		 	@param lineno The line number of the source (where the exception was thrown from).
		 	@param file The source file from which the exception was thrown from.
		 	@param function The source function from which the exception was thrown from.
		*/
		CExceptionStream(EExceptionType type, unsigned int lineno, const std::string& file, const std::string& function);

		/** Move constructor.
		 	@author Hayden Asplet
		*/
		CExceptionStream(CExceptionStream&& other);

		/** Destructor.
		 	@author Hayden Asplet
		*/
		~CExceptionStream();

		// Deleted copy constructor and asigment operator.
		CExceptionStream(const CExceptionStream& other) = delete;
		CExceptionStream& operator=(const CExceptionStream& other) = delete;

		/** Insert into the stream.
		 	@author Hayden Asplet
		*/
		template<typename T>
		CExceptionStream& operator<<(const T& val);

		/** Template specialization for CEndExcept, which signals the end of an exception description.
			@remarks
				This invokes the throwing of a CException exception based on the parameters passed into
				the constructor of this class as well as the message concatenated through the insertion
				operator <<.
		 	@author Hayden Asplet
		*/
		CExceptionStream& operator<<(const CEndThrow& val);
	protected:
	private:
		
		// Member Variables
	public:
	protected:
	private:
		EExceptionType m_Type;
		unsigned int m_LineNumber;
		std::string m_File;
		std::string m_FunctionName;
		std::ostringstream m_DescriptionStream;
		bool m_bThrowOnDestruction;
	};

	template<typename T>
	CExceptionStream& CExceptionStream::operator<<(const T& val)
	{
		m_DescriptionStream << val;
		return *this;
	}

	/** Create an exception stream to stream exception messages to.
		@remarks
			This function is used entirely so that an exception stream can be created and used in a single 
			line. This function is to be used only by SLIM_THROW to ensure that the exception stream is
			created and destroyed automatically using an if statement to create a block without the need 
			for a closing bracket.
		@note
			Do NOT call this, use SLIM_THROW instead.
	 	@author Hayden Asplet
	*/
	CExceptionStream CreateExceptionStream(EExceptionType type, unsigned int lineno, const std::string& file, const std::string& function);

#define SLIM_THROW(type) \
	if(true) \
		CreateExceptionStream(type, __LINE__, __FILE__, __FUNCTION__)
}




#endif	// __SLIMEXCEPTIONS_H__