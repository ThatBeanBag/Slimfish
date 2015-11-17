// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: SlimRenderer.h
// Description	: IRenderer declaration file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __SLIMRENDERER_H__
#define __SLIMRENDERER_H__

// Library Includes

// Local Includes
#include "SlimTypes.h"
#include "SlimTextureLayer.h"
#include "SlimIndexGpuBuffer.h"
#include "SlimShaderProgram.h"

namespace Slim {
	// Forward Declaration
	class CMatrix4x4;
	class CVector3;
	class ATexture;
	class AVertexGpuBuffer;
	class AIndexGpuBuffer;
	class AShaderProgram;
	class ARenderTexture;
	class CVertexDeclaration;
	class CColour;
	class CRenderPass;

	/** Abstract class encapsulating a rendering system.
	@remarks
		
	*/
	class ARenderer {
		// Member Functions
	public:
		ARenderer(int width, int height, bool isWindowed);

		/** Destructor
		 	@author Hayden Asplet	
		*/
		virtual ~ARenderer() {}

		/** Initialize the renderer.
		 	@author Hayden Asplet
			@return 
				True if initialization was successful, false if an error occurred
				failing the initialization step. 
		*/
		virtual bool VInitialize() = 0;

		/** Setup renderer for rendering. Called once every frame prior to rendering.
			@remarks
				Clears the frames for rendering onto and begins the scene.
		 	@author Hayden Asplet
		*/
		virtual void VPreRender() = 0;

		/** End rendering for this frame.
		 	@author Hayden Asplet
		 	@return void
		*/
		virtual void VPostRender() = 0;

		// TODO: Window functions that should be in a window class.
		void SetWindowed(bool windowed);
		void ToggleWindowed();
		bool IsWindowed() const;
		CPoint GetWindowSize();
		void Resize(size_t width, size_t height);

		/** Set the render pass for any succeeding render calls.
		 	@author Hayden Asplet
		*/
		void SetRenderPass(CRenderPass* pPass);

		/** Create a vertex buffer on the GPU.
		@author Hayden Asplet
		@param usage Indicates how the buffer is going to be used for optimization purposes.
		@param isInSystemMemory True if the buffer should be stored in system memory for reading/writing to.
		@return
			A pointer to the buffer for reading/writing depending on intended
			usage of the buffer.
		*/
		template<typename TVertexType>
		shared_ptr<AVertexGpuBuffer> CreateVertexBuffer(const std::vector<TVertexType>& verts, EGpuBufferUsage usage = EGpuBufferUsage::STATIC, bool isInSystemMemory = false);

		/** Create a index buffer on the GPU.
		@author Hayden Asplet
		@param usage Indicates how the buffer is going to be used for optimization purposes.
		@param isInSystemMemory True if the buffer should be stored in system memory for reading/writing to.
		@return
			A pointer to the buffer for reading/writing depending on intended
			usage of the buffer.
		*/
		shared_ptr<AIndexGpuBuffer> CreateIndexBuffer(const std::vector<int>& indices, EGpuBufferUsage usage = EGpuBufferUsage::STATIC, bool isInSystemMemory = false);

		/** Create a index buffer on the GPU.
		@author Hayden Asplet
		@param usage Indicates how the buffer is going to be used for optimization purposes.
		@param isInSystemMemory True if the buffer should be stored in system memory for reading/writing to.
		@return
			A pointer to the buffer for reading/writing depending on intended
			usage of the buffer.
		*/
		shared_ptr<AIndexGpuBuffer> CreateIndexBuffer(const std::vector<short>& indices, EGpuBufferUsage usage = EGpuBufferUsage::STATIC, bool isInSystemMemory = false);

		/** Create a vertex buffer on the GPU.
		 	@author Hayden Asplet
		 	@param numVertices The number of vertices to stored in the buffer.
		 	@param stride Stride or size of a single vertex.
			@param
				usage How the buffer is intended to be used e.g. statically, dynamically, write only
				etc. see EGpuBufferUsage.
			@param
				isOutput True if the buffer is to be used as output from a geometry shader. This specifies
				that the buffer can be bound to the stream output stage of the pipeline and cannot be used
				for drawing.
			@param
				isInSystemMemory True if the buffer should be stored in system memory and not in video
				memory on the GPU. This is the least optimal for buffers that need to sent to the GPU
				at some point, but can be used for heavily dynamic buffers that will be changed frequently
				by the CPU.
		 	@return 
				A pointer to the buffer for reading/writing depending on intended 
				usage of the buffer.
		*/
		virtual shared_ptr<AVertexGpuBuffer> VCreateVertexBuffer(size_t numVertices, size_t stride, const void* pSource, EGpuBufferUsage usage, bool isOutput = false, bool isInSystemMemory = false) = 0;

		/** Create a index buffer on the GPU.
			@author Hayden Asplet
			@param numIndices The number of indices to stored in the buffer.
			@param indexSize Stride or size of a single index.
			@param
				usage How the buffer is intended to be used e.g. statically, dynamically, write only
				etc. see EGpuBufferUsage.
			@param
				isOutput True if the buffer is to be used as output from a geometry shader. This specifies
				that the buffer can be bound to the stream output stage of the pipeline and cannot be used
				for drawing.
			@param
				isInSystemMemory True if the buffer should be stored in system memory and not in video
				memory on the GPU. This is the least optimal for buffers that need to sent to the GPU
				at some point, but can be used for heavily dynamic buffers that will be changed frequently
				by the CPU.
			@return
				A pointer to the buffer for reading/writing depending on intended
				usage of the buffer.
		*/
		virtual shared_ptr<AIndexGpuBuffer> VCreateIndexBuffer(size_t numIndices, AIndexGpuBuffer::EIndexType indexType, const void* pSource, EGpuBufferUsage usage, bool isOutput = false, bool isInSystemMemory = false) = 0;

		/** Create a shader from file.
		 	@author Hayden Asplet
		 	@param 
				name Name of the shader file.
		 	@param
				type The type of shader e.g. vertex, pixel or geometry shader.
			@param
				entry Name of the entry point to the program.
			@param 
				shaderModel Shader model of the program e.g. vs_4_0, ps_4_0 etc.
			@param 
				isStreamingOut True if the shader should be enabled to stream out to a buffer. Only
				applicable for geometry shaders. 
		 	@return
				Pointer to the shader programs; not valid if creation failed.
		*/
		virtual shared_ptr<AShaderProgram> VCreateShaderProgram(const std::string& name, EShaderProgramType type, const std::string& entry, const std::string& shaderModel/*, bool isStreamingOut = false*/) = 0;

		/** Loads a texture from file.
		 	@author Hayden Asplet
		*/
		virtual shared_ptr<ATexture> VLoadTexture(const std::string& name, ETextureType type = ETextureType::TYPE_2D, ETextureUsage usage = ETextureUsage::STATIC) = 0;

		/** Create a texture.
			@remarks
				Doesn't load or create the texture resource itself. This method is useful if
				you want to set the parameters of the texture prior to creating the resource itself.
		 	@author Hayden Asplet
		 	@param 
				name Name to give the texture. Purely a semantic unless you load the texture from disk
				in which case the name of texture specifies the relative location of the texture to load.
		 	@return Pointer the the newly created texture that is yet to be loaded.
		*/
		virtual shared_ptr<ATexture> VCreateTexture(const std::string& name) = 0;

		/** Create a render texture.
		 	@author Hayden Asplet
		 	@param 
				name Name of the render target. Purely a semantic name for identification.
			@param 
				width Width of the render target in pixels.
			@param 
				height Height of the render target in pixels.
			@param 
				depth Depth of the render target int pixels, only used if the render target is 3D.
		 	@param 
				textureType Type of texture e.g. TYPE_2D, TYPE_3D, TYPE_CUBEMAP etc. 3D and 1D textures
				can't use multi-sampling, thus any non-2D render targets will lack multi-sampling and 
				the parameters will be discarded.
			@param 
				msaaCount The number of multi-sampling samples. Only applicable to 2D render targets.
			@param 
				msaaQuality The quality of the multi-sampling samples. Only applicable to 2D render
				targets.
		*/
		virtual std::unique_ptr<ARenderTexture> VCreateRenderTexture(const std::string& name, size_t width, size_t height, size_t depth = 0,
			ETextureType textureType = ETextureType::TYPE_2D, size_t msaaCount = 1, size_t msaaQuality = 0) = 0;
		
		/** Create a render texture from an existing texture. 
			@remarks
				The provided texture will be written to in any render calls with the render texture set 
				as the target. If you don't want the texture to be written to, first make a copy of it.
			@author Hayden Asplet
		*/
		virtual std::unique_ptr<ARenderTexture> VCreateRenderTexture(std::shared_ptr<ATexture> pTexture) = 0;

		/** Perform a render operation, rendering a set of vertices.
		 	@author Hayden Asplet
		 	@param 
				vertexDeclaration Describes the layout of vertices and how they are to be interpreted 
				by the rendering pipeline.	
		 	@param 
				pVertexBuffer Buffer containing the vertex data.
		 	@param 
				pIndexBuffer Buffer containing the index data. This is optional vertices do not need to 
				be indexed however to avoid duplicate vertices it is recommended to use an index buffer.
		*/
		virtual void VRender(const CVertexDeclaration& vertexDeclaration, 
							 EPrimitiveType primitiveType, 
							 shared_ptr<AVertexGpuBuffer> pVertexBuffer, 
							 shared_ptr<AIndexGpuBuffer> pIndexBuffer = nullptr, 
							 size_t countOverride = 0, 
							 size_t instances = 1) = 0;

		/** Bind either a vertex, pixel or geometry shader program to the renderer.
			@remarks
				Only one of each type of shader can be bound at a time the rendering pipeline will use
				the shader to perform operations on vertices that are rendered using VRender().
		 	@author Hayden Asplet
		 	@param pShader Pointer to the vertex, pixel or geometry shader to bind.
		*/
		virtual void VSetShaderProgram(shared_ptr<AShaderProgram> pShader) = 0;

		/** Disable a previously set shader program.
		 	@author Hayden Asplet
		 	@param programType The shader program type to disable e.g. VERTEX, PIXEL, or GEOMETRY.
		*/
		virtual void VDisableShaderProgram(EShaderProgramType programType) = 0;

		/** Set the render target to render to.
		 	@author Hayden Asplet
		 	@param pRenderTarget Pointer to the render target to render to.
		*/
		virtual void VSetRenderTargets(std::vector<ARenderTexture*> renderTargets) = 0;

		/** Set the target buffers to stream out to. 
		 	@author Hayden Asplet
		 	@param buffers List of buffers to stream out to during the stream output stage.
		*/
		virtual void VSetStreamOutTargets(const std::vector<std::shared_ptr<AGpuBuffer> >& buffers) = 0;

		/** Set the target buffer to stream out to.
			@author Hayden Asplet
			@param pBuffer Buffer to stream out to during the stream output stage.
		*/
		virtual void VSetStreamOutTarget(const std::shared_ptr<AGpuBuffer>& pBuffer) = 0;

		/** Begin a stream output query.
			@remarks
				This should be called before streaming out data in order to retrieve the
				number of primitives streamed out with a call to VEndStreamOutputQuery().
		 	@author Hayden Asplet
		*/
		virtual void VBeginStreamOutQuery() = 0;

		/** End the stream output query and retrieve the number of primitives streamed out.
		 	@author Hayden Asplet
		 	@return 
				Number of primitives streamed out between the call to VBeginStreamOutputQuery()
				and this call.
		*/
		virtual size_t VEndStreamOutQuery() = 0;

		/** Set the world transform. */
		virtual void VSetWorldTransform(const CMatrix4x4& worldTransform) = 0;
		/** Set the view transform. */
		virtual void VSetViewTransform(const CMatrix4x4& viewTransform) = 0;
		/** Set the projection transform. */
		virtual void VSetProjectionTransform(const CMatrix4x4& projectionTransform) = 0;
		/** Set the ambient colour. */
		virtual void VSetAmbientColour(const CColour& colour) = 0;

		/** Set the background colour. @author Hayden Asplet */
		void SetBackgroundColour(const CColourValue& colour);
		/** Get the background colour. @author Hayden Asplet */
		const CColourValue& GetBackgroundColour() const;
		
		/** Set the fog options for future rendering of geometry.
		 	@author Hayden Asplet
		 	@param 
				fogType The type of fog to set. See EFogType for more details. Set to FOG_NONE
				to turn off fog entirely.
		 	@param 
				colour The colour of the fog.
		 	@param 
				start Distance at which to begin increasing the density of the fog when using
				the linear fog type FOG_LINEAR in the range [0, 1.0], where 0 is the near plane
				and 1.0 is the far plane. The destiny of the fog effectively interpolates 
				between the start and end distances.
		 	@param 
				end Distance at which the fog will completely obscure the geometry when using 
				the linear fog type FOG_LINEAR in the range [0, 1.0], where 0 is the near plane
				and 1.0 is the far plane.
		 	@param 
				exponentialDensity The density of the fog when using an exponential fog type 
				(FOG_EXPONENTIAL and FOG_EXPONENTIAL2) in the range [0, 1.0]. Higher density
				values will fully obscure the geometry at some point, whereas lower densities
				may never. Not used when fog type is linear or disabled.
		*/
		virtual void VSetFog(EFogType fogType, 
							 const CColourValue& colour = CColourValue::s_BLACK,
							 float start = 0.0f,
							 float end = 1.0f,
							 float exponentialDensity = 1.0f) = 0;

		/** Set the blending mode to describe how a source colour/alpha blends with a destination 
			colour/alpha in the rendering pipeline.
		 	@author Hayden Asplet
			@param 
				blendingMode Structure containing the blending operations and factors to describe 
				how a source colour/alpha blends with a destination colour/alpha in the rendering 
				pipeline.
		*/
		virtual void VSetBlendingMode(const TBlendingMode& blendingMode) = 0;

		/** Set the colour writes enabled that describe which colour channels are written to the back buffer.
			@author Hayden Asplet
		*/
		virtual void VSetColourWritesEnabled(const TColourWritesEnabled& colourWritesEnabled) = 0;

		/** Set the stencil buffer settings. @author Hayden Asplet */
		virtual void VSetStencilBufferSettings(const TStencilBufferSettings& settings) = 0;

		/** Set the depth buffer settings.
		 	@author Hayden Asplet
		 	@param enabled True if the depth buffer should be enabled.
		 	@param writeEnabled True if the depth buffer should be written to.
		 	@param compareFunction The comparison function to use when testing depth.
		*/
		virtual void VSetDepthBufferSettings(bool enabled, bool writeEnabled, EComparisonFunction compareFunction) = 0;

		/** Set the culling mode of the rasterizer. @author Hayden Asplet */
		virtual void VSetCullingMode(ECullingMode cullingMode) = 0;
		/** Set the fill mode of the rasterizer. @author Hayden Asplet */
		virtual void VSetFillMode(EFillMode fillMode) = 0;


		/** Set the texture layer options of a specified layer.
		 	@author Hayden Asplet
		 	@param layer The layer to set the texture layer options to.
		 	@param textureLayer The texture layer options to set.
		*/
		void SetTextureLayer(size_t layer, CTextureLayer& textureLayer);

		/** Disable a texture layer.
		 	@author Hayden Asplet
		*/
		void DisableTextureLayer(size_t layer);

		/** Disable a range of texture layers from a specified layer to g_MAX_TEXTURE_LAYERS.
		 	@author Hayden Asplet
		*/
		void DisableTextureLayerFrom(size_t layer);

		/** Draw text to the screen.
		 	@author Hayden Asplet
		 	@param text Text string to be rendered.
		 	@param position Position of the text in screen coordinates.
		 	@param colour Colour of the text.
		*/
		virtual void VDrawText(const std::string text, const CPoint& position, const CColour& colour) = 0;
	protected:
	private:
		// Windows functions that should be in a windows class.
		virtual void VSetWindowed(bool windowed) = 0;
		virtual void VOnResize() = 0;

		/** Internal method to set the texture filtering for a specific sampler type on a layer.
		 	@author Hayden Asplet
		 	@param	layer The layer/stage to set the texture filtering of.
		 	@param	samplerType The sampler type to set the filtering of (min, mag or mip).
		 	@param	filterType The filter type to set.
		*/
		//void SetTextureLayerFiltering(size_t layer, ETextureSamplerType samplerType, ETextureFilterType filterType);

		/** Internal convenience method to set the min, mag and mip texture filters for a texture layer.
		 	@author Hayden Asplet
		*/
		virtual void VSetTextureLayerFiltering(size_t layer, ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter) = 0;

		/** Internal delegating method to set the texture of layer.
			@note
				Derived classes must implement this. If disable is set to false the implementation should 
				ignore pTexture and simply disable the texture layer.
		 	@author Hayden Asplet
		 	@param	layer The layer or stage to set the texture of.
		 	@param	pTexture The texture resource to set can be NULL if you want to disable the layer.
		 	@param	disable True if you want to disable the layer.
		*/
		virtual void VSetTexture(size_t layer, const shared_ptr<ATexture>& pTexture, bool disable = false) = 0;


		/** Internal delegating method to set the colour and alpha blending operations and argument of 
			a texture layer.
			@note Derived classes must implement this.
		 	@author Hayden Asplet
		*/
		virtual void VSetTextureLayerBlendState(size_t layer, const TTextureLayerBlendState& blendState) = 0;

		/** Internal delegating method to set the texture addressing modes for the u, v and w texture 
			coordinates of a texture layer.
			@note Derived classes must implement this.
		 	@author Hayden Asplet
		*/
		virtual void VSetTextureAddressModes(size_t layer, const TTextureUVWAddressModes& addressModes) = 0;

		/** Internal delegating method to set the texture border colour of a texture layer when using 
			a TADDRESS_BORDER texture addressing mode.
			@note Derived classes must implement this.
		 	@author Hayden Asplet
		*/
		virtual void VSetTextureBorderColour(size_t layer, const CColourValue& colour) = 0;

		/** Internal delegating method to set the vertex declaration to draw with.
		 	@author Hayden Asplet
		*/
		virtual void VSetVertexDeclaration(const CVertexDeclaration& vertexDeclaration) = 0;

		/** Internal delegating method to set the vertex buffer to use as input to the pipeline when drawing.
		 	@author Hayden Asplet
		*/
		virtual void VSetVertexBuffer(const shared_ptr<AVertexGpuBuffer>& pVertexBuffer) = 0;

		/** Internal delegating method to set the index buffer to use as input to the pipeline when drawing.
		 	@author Hayden Asplet
		*/
		virtual void VSetIndexBuffer(const shared_ptr<AIndexGpuBuffer>& pIndexBuffer) = 0;

		/** Internal delegating method to set the background the colour. */
		virtual void VSetBackgroundColour(const CColourValue& colour) = 0;

		/** Internal delegating method to build states to prepare for rendering. */
		virtual void VBuildStates() = 0;

		// Member Variables
	public:
	protected:
		bool m_IsVSyncEnabled;
		bool m_IsWindowed;
		int m_Width;
		int m_Height;
	private:
		CColourValue m_BackgroundColour;
		size_t m_disableTextureLayerFrom;	// Texture layer from this to g_MAX_TEXTURE_LAYERS are currently disabled.
	};

	// Implementation

	template<typename TVertexType>
	shared_ptr<AVertexGpuBuffer> ARenderer::CreateVertexBuffer(const std::vector<TVertexType>& verts, EGpuBufferUsage usage, bool isInSystemMemory)
	{
		return VCreateVertexBuffer(verts.size(), sizeof(TVertexType), reinterpret_cast<const void*>(&verts[0]), usage, false, isInSystemMemory);
	}
}
#endif	// __SLIMRENDERER_H__