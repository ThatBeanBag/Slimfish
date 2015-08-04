// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimTextureState.h
// Description	: CTextureState declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMTEXTURESTATE_H__
#define __SLIMTEXTURESTATE_H__

// Library Includes

// Local Includes
#include "SlimColour.h"
#include "SlimTexture.h"

namespace Slim {

	/** Enumeration representing blending operations for texture layers.
	@remarks
		List of all operations for blending between texture layers these operations
		are only available when multi-texturing is supported.
	*/
	enum ETextureLayerBlendOperation {
		// Use arg1 without additional modifications.
		TLAYEROP_SELECT1,
		// Use arg2 without additional modifications.
		TLAYEROP_SELECT2,
		// Multiple arg1 and arg2 together.
		// arg1 * arg2
		TLAYEROP_MODULATE,
		// As TLAYEROP_MODULATE but multiply result by two for brightening.
		TLAYEROP_MODULATE_2X,
		// As TLAYEROP_MODULATE4X but multiply result by four for brightening.
		TLAYEROP_MODULATE_4X,
		// Add arg1 and arg2 together. 
		// arg1 + arg2
		TLAYEROP_ADD,
		// As TLAYEROP_ADD but subtract 0.5 from the result so the values range from -0.5 to 0.5. 
		// arg1 + arg2 - 0.5
		TLAYEROP_ADD_SIGNED,
		// As TLAYEROP_ADD_SIGNED but multiply result by two.
		TLAYEROP_ADD_SIGNED_2X,
		// As TLAYEROP_ADD but subtract the product from the sum
		// arg1 + arg2 - arg1 * arg2
		TLAYEROP_ADD_SMOOTH,
		// Subtract arg1 from arg2.
		// arg1 - arg2
		TLAYEROP_SUBTRACT,
		// Use linearly blended alpha value from vertices to scale arg1, then add arg2 scaled by 1 - alpha of vertices.
		TLAYEROP_BLEND_DIFFUSE_ALPHA,
		// As TLAYEROP_BLEND_DIFFUSE_ALPHA but use alpha from texture instead.
		TLAYEROP_BLEND_TEXTURE_ALPHA,
		// As TLAYEROP_BLEND_DIFFUSE_ALPHA but use alpha from previous layer instead.
		TLAYEROP_BLEND_CURRENT_ALPHA,
		// As TLAYEROP_BLEND_DIFFUSE_ALPHA but use alpha from manual value instead.
		TLAYEROP_BLEND_MANUAL,
		// Use dot product of arg1 and arg2.
		TLAYEROP_DOTPRODUCT,
		// Use linearly blended colour value from vertices to scale arg1, then add arg2 scaled by 1 - colour of vertices.
		TLAYEROP_BLEND_DIFFUSE_COLOUR
	};

	/** List of all texture layer blending sources to perform blending operations on.
	@remarks
		Used almost exclusively with CTextureLayer::SetColourOperation() and
		CTextureLayer::SetAlphaOperation() as well as within the 
		TTextureLayerBlendState structure.
	*/
	enum ETextureLayerBlendSource {
		// The colour/alpha built up from previous layers.
		TLAYERSRC_CURRENT,
		// The colour/alpha from the texture of the layer.
		TLAYERSRC_TEXTURE,
		// The colour/alpha of the diffuse colour of the vertices.
		TLAYERSRC_DIFFUSE,
		// The colour/alpha of the specular colour of the vertices.
		TLAYERSRC_SPECULAR,
		// The colour/alpha supplied manually.
		TLAYERSRC_MANUAL
	};

	/** Structure representing the blending of colour and alpha for texture layers.
	@remarks
		This is used by CTextureLayer to manage how texture layers are blended together.
		It provides a thin wrapper around texture stage states.
	*/
	struct TTextureLayerBlendState {
		// Blending operations
		ETextureLayerBlendOperation m_colourBlendOperation;	// The colour operation to apply.
		ETextureLayerBlendOperation m_alphaBlendOperation;	// The alpha operation to apply.

		// Source arguments to use in the blend operation
		ETextureLayerBlendSource m_colourSource1; 
		ETextureLayerBlendSource m_colourSource2; 
		ETextureLayerBlendSource m_alphaSource1;  
		ETextureLayerBlendSource m_alphaSource2;

		// Manual arguments (used only if blend sources are set to TLAYERSRC_MANUAL)
		TColourValue m_colourArg1;	// First colour source.
		TColourValue m_colourArg2;	// Second colour source.
		float m_alphaArg1;			// First alpha source.
		float m_alphaArg2;			// Second alpha source.
		float m_alphaManualBlend;	// Manual blending value for alpha blending (used only when blend operation is TLAYEROP_BLEND_MANUAL).
		float m_colourManualBlend;	// Manual blending value for colour blending (used only when blend operation is TLAYEROP_BLEND_MANUAL).
	};

	/** List of sampler types.
	*/
	enum ETextureSamplerType {
		// Sampler used when shrinking texture.
		TSAMP_MIN,
		// Sampler used when magnifying texture.
		TSAMP_MAG,
		// Sampler used when determining mipmap.
		TSAMP_MIP
	};

	/** List of low-level filtering types.
	*/
	enum ETextureFilterType {
		// No filtering.
		TFILTER_NONE,
		// Closest pixel filtering.
		TFILTER_POINT,
		// Average of a 2x2 area.
		TFILTER_LINEAR,
		// As TFILTER_LINEAR but takes into account the angle of the texture plane.
		TFILTER_ANISOTROPIC
	};

	/** Broad filtering types providing shortcuts to commonly used filtering combinations.
	*/
	enum ETextureFilterTypeBroad {
		// Equivalent to min = TFILTER_POINT, mag = TFILTER_POINT, mip = TFILTER_NONE
		TFILTERB_NONE,
		// Equivalent to min = TFILTER_LINEAR, mag = TFILTER_LINEAR, mip = TFILTER_POINT
		TFILTERB_BILINEAR,
		// Equivalent to min = TFILTER_LINEAR, mag = TFILTER_LINEAR, mip = TFILTER_LINEAR
		TFILTERB_TRILINEAR,
		// Equivalent to min = TFILTER_ANISOTROPIC, mag = TFILTER_ANISOTROPIC, mip = TFILTER_LINEAR
		TFILTERB_ANISOTROPIC
	};

	/** List of texture-addressing modes e.g. wrap, mirror, border and clamp.
	@remarks 
		Address modes specifically control what texture coordinates outside of the 
		range [0, 1.0] do. For example TADDRESS_WRAP will repeat the texture if 
		coordinates exceed the [0, 1.0] range.
	*/
	enum ETextureAddressMode {
		// Tile the texture at each junction.
		TADDRESS_WRAP,
		// As TADDRESS_WRAP but flip (mirror) texture at every junction. For u values between 0 and 1
		// the texture is address same as TADDRESS_WRAP, but between 1 and 2 the texture is mirrored.
		TADDRESS_MIRROR,
		// Texture coordinates outside of the [0, 1.0] range are set to the border colour.
		TADDRESS_BORDER,
		// Texture coordinates outside of the [0, 1.0] range are set to the texture colour at 0 or 
		// 1.0 respectively.
		TADDRESS_CLAMP
	};

	/** Structure representing address modes for u, v and w texture coordinates.
	*/
	struct TTextureUVWAddressModes {
		ETextureAddressMode m_u;
		ETextureAddressMode	m_v;
		ETextureAddressMode	m_w;
	};

	/** Class encapsulating the texture stage states and filtering options
	@remarks
		Provides the necessities for multi-texturing. Mainly facilitates blending operations
		of texture stages and the transforming of uv coordinates for fixed function texture
		animations. Also encapsulates texture filtering options for min, mag and mipmap sampling
		for specific texture stages/layers. This class is used mainly by CMaterial to provide
		multi-textured materials for renderables in the scene.
	*/
	class CTextureLayer {
		// Member Functions
	public:
		/** Default constructor
		 	@author
		 		Hayden Asplet
		*/
		CTextureLayer();

		/** Destructor
		 	@author
		 		Hayden Asplet		
		*/
		~CTextureLayer();

		/** Set the texture of the layer.
		 	@author
		 		Hayden Asplet
		*/
		void SetTexture(const shared_ptr<ATexture>& pTexture);

		/** Get the texture resource of layer.
		 	@author
		 		Hayden Asplet
			@return
				shared_ptr to the texture resource.
		*/
		const shared_ptr<ATexture>& GetTexture();

		/** Set the broad texture filter of the layer.
			@remarks 
				Convenience function for setting commonly used combinations
				of filtering and sampler types
		 	@author
		 		Hayden Asplet
		*/
		void SetTextureFilter(ETextureFilterTypeBroad filterType);

		/** Set the min mag and mip texture filters of the layer.
		 	@author
		 		Hayden Asplet
		 	@param	minFilter filter used when shrinking texture.
		 	@param	magFilter filter used when magnifying texture.
		 	@param	mipFilter filter used when determining mipmap.
		*/
		void SetTextureFilter(ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter);

		/** Get the texture filter options of a specific sampler type (min, mag and mip).
		 	@author
		 		Hayden Asplet
		 	@param	samplerType The sampler type to get the texture filter of e.g.g min, mag or mip.
		*/
		ETextureFilterType GetTextureFilter(ETextureSamplerType samplerType);

		/** Set the colour operation and arguments of the texture layer.
		 	@author
		 		Hayden Asplet
		 	@param 
				operation Blending operation to perform on the texture 
				layer e.g. modulate, add, subtract.
		 	@param 
				source1 The first source of colour to use in the 
				operation e.g. texture, diffuse, current.
		 	@param 
				source2 The second source of colour to use in the 
				operation e.g. texture, diffuse, current.
		 	@param 
				argument1 First manual colour argument (only used if 
				source1 = TLAYERSRC_MANUAL).
		 	@param 
				argument2 Second manual colour argument (only used if 
				source2 = TLAYERSRC_MANUAL).
		 	@param 
				manualBlend Manually supplied blending value (only used if 
				operation = TLAYEROP_BLEND_MANUAL)
				@see ETextureLayerBlendOperation
		*/
		void SetColourOperation(ETextureLayerBlendOperation operation,
								ETextureLayerBlendSource source1 = TLAYERSRC_TEXTURE,
								ETextureLayerBlendSource source2 = TLAYERSRC_CURRENT,
								const TColourValue& argument1 = TColourValue::s_WHITE,
								const TColourValue& argument2 = TColourValue::s_WHITE,
								float manualBlend = 0.0f);

		/** Set the alpha operation and arguments of the texture layer.
		 	@author
		 		Hayden Asplet
			@param
				operation Blending operation to perform on the texture
				layer e.g. modulate, add, subtract.
			@param
				source1 The first source of alpha to use in the
				operation e.g. texture, diffuse, current.
			@param
				source2 The second source of alpha to use in the
				operation e.g. texture, diffuse, current.
			@param
				argument1 First manual alpha argument (only used if
				source1 = TLAYERSRC_MANUAL).
			@param
				argument2 Second manual alpha argument (only used if
				source2 = TLAYERSRC_MANUAL).
			@param
				manualBlend Manually supplied blending value (only used if
				operation = TLAYEROP_BLEND_MANUAL)
				@see ETextureLayerBlendOperation
		*/
		void SetAlphaOperation(ETextureLayerBlendOperation operation,
							   ETextureLayerBlendSource source1 = TLAYERSRC_TEXTURE,
							   ETextureLayerBlendSource source2 = TLAYERSRC_CURRENT,
							   float argument1 = TColourValue::s_ALPHA_OPAQUE,
							   float argument2 = TColourValue::s_ALPHA_OPAQUE,
							   float manualBlend = 0.0f);

		/** Get the entire blending state (both the alpha and colour blending operations and arguments).
		 	@author
		 		Hayden Asplet
		*/
		const TTextureLayerBlendState& GetBlendState() const;

		/** Set the translation to transform the uv coordinates by for the texture on this layer.
			@note Only has an effect in fixed function pipeline.
		 	@author
		 		Hayden Asplet
		*/
		void SetTextureTranslation(float uTranslation, float vTranslation);

		/** Set the scale to transform the uv coordinates by for the texture on this layer.
			@note Only has an effect in fixed function pipeline.
			@author
				Hayden Asplet
		*/
		void SetTextureScale(float uScale, float vScale);

		/** Set the rotation to transform the uv coordinates by for the texture on this layer.
			@note Only has an effect in fixed function pipeline.
			@author
				Hayden Asplet
		*/
		void SetTextureRotation(float radAngle);

		/** Set the transform of the texture to transform any uv coordinates of the texture on this layer.
			@remarks
				This method is used to create various animated texture effects like scrolling, rotating
				or scaling. Using SetTextureSetTextureTranslation(), SetTextureScale() and 
				SetTextureRotation() will offer the same results, however will concatenate the matrix 
				for you therefore using the three methods to perform animations is preferable.
			@note
				Setting the texture's transform (including translation, scale and rotation) has no effect
				in the programmable pipeline and is used entirely for fixed function. In the programmable
				pipeline this effect is achieved in shader code.
		 	@author
		 		Hayden Asplet
		 	@param	textureTransform Matrix to transform the texture coordinates on this layer by.
		*/
		void SetTextureTransform(const CMatrix4x4& textureTransform);

		/** Get the transform of texture coordinates on this layer.
			@note 
				May re-calculate the transform matrix if SetTextureTranslation(), SetTextureScale() or
				SetTextureRotation() has been used since call of this method.
		 	@author
		 		Hayden Asplet
		*/
		const CMatrix4x4& GetTextureTransform();

		/** Set the texture addressing modes (what happens when textures coordinates exceed range [0, 1.0]).
		 	@author
		 		Hayden Asplet
		*/
		void SetTextureAddressModes(const TTextureUVWAddressModes& textureAddressingModes);

		/** Set the texture addressing modes (what happens when textures coordinates exceed range [0, 1.0]).
			@note
				This is convenience function to set ALL addressing modes of each coordinate (u, v and w) to
				a single texture addressing mode at once.
		 	@author
		 		Hayden Asplet
		 	@param	addressingMode Addressing mode to apply to ALL coordinates (u, v and w) at once.
		*/
		void SetTextureAddressModes(ETextureAddressMode addressingMode);


		/** Set the texture addressing modes (what happens when textures coordinates exceed range [0, 1.0]).
		 	@author
		 		Hayden Asplet
		*/
		void SetTextureAddressModes(ETextureAddressMode uMode, ETextureAddressMode vMode, ETextureAddressMode wMode);

		/** Get the texture addressing modes (what happens when textures coordinates exceed range [0, 1.0]).
		 	@author
		 		Hayden Asplet
		*/
		const TTextureUVWAddressModes& GetTextureAddressModes() const;

		/** Set the texture border colour to use when using TADDRESS_BORDER as an addressing mode for
			texture coordinates.
			@note
				If you are not using TADDRESS_BORDER at all for any addressing mode for u, v or w then
				this method has no use.
		 	@author
		 		Hayden Asplet
		*/
		void SetTextureBorderColour(const TColourValue& colour);

		/** Get the texture border colour to use when using TADDRESS_BORDER as an addressing mode for
			texture coordinates.
		 	@author
		 		Hayden Asplet
		*/
		const TColourValue& GetTextureBorderColour() const;
	protected:
	private:
		// Member Variables
	public:
	protected:
	private:
		TTextureLayerBlendState m_blendState;	// Blending operations and arguments for colour and alpha for this layer.

		shared_ptr<ATexture> m_pTexture;	// The texture of this layer.

		ETextureFilterType m_minFilter;		// Filter option when shrinking texture.
		ETextureFilterType m_magFilter;		// Filter option when magnifying texture.
		ETextureFilterType m_mipFilter;		// Filter option when determining mipmap of texture.

		CMatrix4x4 m_textureTransform;		// Matrix to transform the texture coordinates on this layer by (only used in fixed function).
		float m_uTranslation;	// Translation of u coordinate.
		float m_vTranslation;	// Translation of v coordinate.
		float m_uScale;			// Scale of u coordinate.
		float m_vScale;			// Scale of v coordinate.
		float m_radAngle;		// Angle of u, v coordinates.
		bool m_isTransformDirty;	// True if the transform needs to be re-calculated before use.

		TTextureUVWAddressModes m_textureAddressingModes;	// Addressing modes of and u, v and w texture coordinates.

		TColourValue m_borderColour;	// The border colour to use when using TADDRESS_BORDER.
	};
}

#endif	// __SLIMTEXTURESTATE_H__