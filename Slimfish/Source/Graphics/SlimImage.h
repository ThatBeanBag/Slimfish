// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimImage.h
// Description	: CSlimImage declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMIMAGE_H__
#define __SLIMIMAGE_H__

// Library Includes

// Local Includes

namespace Slim {

	/** Class representing a single image consisting of a 2D array of colours.
		@remarks
			This class provides the functionality to read and write to/from
			images that have been loaded or generated that may be saved onto
			the disc for later use.
	*/
	class CImage {
		typedef std::vector<std::vector<CColour>> TPixels;

		// Member Functions
	public:
		/** Default constructor	
		 	@author Hayden Asplet
		*/
		CImage(size_t width, size_t height);

		/** Construct an image from a 2D array of pixels.
		 	@author Hayden Asplet
		*/
		explicit CImage(const TPixels& pixels);

		/** Destructor.
		 	@author Hayden Asplet
		*/
		~CImage();

		/** Retrieve a row of pixels.
		 	@author Hayden Asplet
		 	@param y The y value of the row to retrieve.
		 	@return The row of pixels stored in a vector.
		*/
		const std::vector<CColour>& operator[](size_t y) const;

		/** Retrieve a row of pixels for modification.
			@author Hayden Asplet
			@param y The y value of the row to retrieve.
			@return The row of pixels stored in a vector.
		*/
		std::vector<CColour>& operator[](size_t y);

		/** Retrieve a single pixel.
		 	@author Hayden Asplet
			@param x The x component of the pixel coordinate in the image.
			@param y The y component of the pixel coordinate in the image.
		*/
		const CColour& GetPixel(size_t x, size_t y);

		/** Set a single pixel.
		 	@author Hayden Asplet
		 	@param colour The colour to set the pixel to.
		 	@param x The x component of the pixel coordinate in the image.
		 	@param y The y component of the pixel coordinate in the image.
		*/
		void SetPixel(const CColour& colour, size_t x, size_t y);

		/** Set the size of the image.
			@note Any new pixels will be black.
		 	@author Hayden Asplet
		*/
		void SetSize(size_t width, size_t height);

		/** Retrieve the width of the image. @author Hayden Asplet */
		size_t GetWidth() const;

		/** Retrieve the height of the image. @author Hayden Asplet */
		size_t GetHeight() const;

		/** Get the size of the image
			@remarks
				Retrieves the total number of pixels of the image aka width * height.
			@author Hayden Asplet 
		*/
		size_t GetSize() const;
	protected:
	private:
		// Member Variables
	public:
	protected:
	private:
		TPixels m_Pixels;

		size_t m_Width;
		size_t m_Height;
	};

}

#endif // __SLIMIMAGE_H__