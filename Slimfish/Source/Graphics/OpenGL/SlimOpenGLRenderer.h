#pragma once

#ifndef __SLIMOPENGLRENDERER_H__
#define __SLIMOPENGLRENDERER_H__

// Library Includes

// Local Includes
#include "../SlimRenderer.h"
#include "SlimOpenGLForward.h"

namespace Slim {

class COpenGLRenderer : public ARenderer {
	// Member Functions
public:
	/** Default Constructor.
		@author Hayden Asplet
	*/
	COpenGLRenderer(int width, int height, bool isWindowed);

	/** Destructor.
		@author Hayden Asplet
	*/
	virtual ~COpenGLRenderer();

	/** @copydoc ARenderer::VInitialize */
	virtual bool VInitialize() override;
	/** @copydoc ARenderer::VPreRender */
	virtual void VPreRender() override;
	/** @copydoc ARenderer::VPostRender */
	virtual void VPostRender() override;

	/** @copydoc ARenderer::VCreateVertexBuffer */
	virtual shared_ptr<AVertexGpuBuffer> VCreateVertexBuffer(size_t numVertices, size_t stride, const void* pSource, EGpuBufferUsage usage, bool isOutput, bool isInSystemMemory) override;
	/** @copydoc ARenderer::VCreateVertexBuffer */
	virtual shared_ptr<AIndexGpuBuffer> VCreateIndexBuffer(size_t numIndices, AIndexGpuBuffer::EIndexType indexType, const void* pSource, EGpuBufferUsage usage, bool isOutput = false, bool isInSystemMemory = false) override;

	/** @copydoc ARenderer::VCreateShaderProgram */
	virtual shared_ptr<AShaderProgram> VCreateShaderProgram(const std::string& name, EShaderProgramType type, const std::string& entry, const std::string& shaderModel);

	/** @copydoc ARenderer::VLoadTexture */
	virtual shared_ptr<ATexture> VLoadTexture(const std::string& name, ETextureType type, ETextureUsage usage) override;

	/** @copydoc ARenderer::VCreateTexture */
	virtual shared_ptr<ATexture> VCreateTexture(const std::string& name) override;

	/** @copydoc ARenderer::VCreateRenderTexture */
	virtual std::unique_ptr<ARenderTexture> VCreateRenderTexture(const std::string& name, size_t width, size_t height, size_t depth,
		ETextureType textureType, size_t msaaCount, size_t msaaQuality) override;

	/** @copydoc ARenderer::VCreateRenderTexture */
	virtual std::unique_ptr<ARenderTexture> VCreateRenderTexture(std::shared_ptr<ATexture> pTexture);

	/** @copydoc ARenderer::VRender */
	virtual void VRender(const CVertexDeclaration& vertexDeclaration,
		EPrimitiveType primitiveType,
		shared_ptr<AVertexGpuBuffer> pVertexBuffer,
		shared_ptr<AIndexGpuBuffer> pIndexBuffer = nullptr,
		size_t countOverride = 0,
		size_t instances = 1);

	/** @copydoc ARenderer::VSetShaderProgram */
	virtual void VSetShaderProgram(shared_ptr<AShaderProgram> pShader);
	/** @copydoc ARenderer::VDisableShaderProgram */
	virtual void VDisableShaderProgram(EShaderProgramType programType) override;

	/** @copydoc ARenderer::VSetRenderTarget */
	virtual void VSetRenderTargets(std::vector<ARenderTexture*> renderTargets) override;

	/** @copydoc ARenderer::VSetStreamOutTargets */
	virtual void VSetStreamOutTargets(const std::vector<std::shared_ptr<AGpuBuffer> >& buffers) override;

	/** @copydoc ARenderer::VSetStreamOutTarget */
	virtual void VSetStreamOutTarget(const std::shared_ptr<AGpuBuffer>& pBuffer) override;

	/** @copydoc ARenderer::VBeginStreamOutQuery */
	virtual void VBeginStreamOutQuery() override;

	/** @copydoc ARenderer::VEndStreamOutQuery */
	virtual size_t VEndStreamOutQuery() override;

	/** @copydoc ARenderer::VSetWorldTransform */
	virtual void VSetWorldTransform(const CMatrix4x4& worldTransform) override;
	/** @copydoc ARenderer::VSetViewTransform */
	virtual void VSetViewTransform(const CMatrix4x4& viewTransform) override;
	/** @copydoc ARenderer::VSetProjectionTransform */
	virtual void VSetProjectionTransform(const CMatrix4x4& projectionTransform) override;

	/** @copydoc ARenderer::VSetAmbientColour */
	virtual void VSetAmbientColour(const CColour& colour) override;
	/** @copydoc ARenderer::VSetBackgroundColour */
	virtual void VSetBackgroundColour(const CColourValue& colour) override;

	/** @copydoc ARenderer::VSetFog */
	virtual void VSetFog(EFogType fogType, const CColourValue& colour = CColourValue::s_BLACK, float start = 0.0f, float end = 1.0f, float exponentialDensity = 1.0f) override;
	/** @copydoc ARenderer::VSetBlending */
	virtual void VSetBlendingMode(const TBlendingMode& blendingMode) override;

	/** @copydoc ARenderer::VSetColourWritesEnabled */
	virtual void VSetColourWritesEnabled(const TColourWritesEnabled& colourWritesEnabled) override;
	/** @copydoc ARenderer::VSetStencilBufferSettings */
	virtual void VSetStencilBufferSettings(const TStencilBufferSettings& settings) override;
	/** @copydoc ARenderer::VSetDepthBufferSettings */
	virtual void VSetDepthBufferSettings(bool enabled, bool writeEnabled, EComparisonFunction compareFunction) override;
	/** @copydoc ARenderer::VSetCullingMode */
	virtual void VSetCullingMode(ECullingMode cullingMode) override;
	/** @copydoc ARenderer::VSetFillMode */
	virtual void VSetFillMode(EFillMode fillMode) override;
protected:
private:
	// Windows functions that should be in a windows class.
	virtual void VSetWindowed(bool windowed);
	virtual void VOnResize();

	/* @copydoc ARenderer::VSetTextureLayerFiltering */
	virtual void VSetTextureLayerFiltering(size_t layer, ETextureFilterType minFilter, ETextureFilterType magFilter, ETextureFilterType mipFilter);
	/* @copydoc ARenderer::VSetTextureLayerFiltering */
	virtual void VSetTexture(size_t layer, const shared_ptr<ATexture>& pTexture, bool disable = false) override;
	/* @copydoc ARenderer::VSetTextureBlendState */
	virtual void VSetTextureLayerBlendState(size_t layer, const TTextureLayerBlendState& blendState) override;
	/* @copydoc ARenderer::VSetTextureAddressModes */
	virtual void VSetTextureAddressModes(size_t layer, const TTextureUVWAddressModes& addressModes) override;
	/* @copydoc ARenderer::VSetTextureBorderColour */
	virtual void VSetTextureBorderColour(size_t layer, const CColourValue& colour) override;

	/* @copydoc ARenderer::VSetVertexDeclaration */
	virtual void VSetVertexDeclaration(const CVertexDeclaration& vertexDeclaration);
	/* @copydoc ARenderer::VSetVertexBuffer */
	virtual void VSetVertexBuffer(const shared_ptr<AVertexGpuBuffer>& pVertexBuffer);
	/* @copydoc ARenderer::VSetIndexBuffer */
	virtual void VSetIndexBuffer(const shared_ptr<AIndexGpuBuffer>& pIndexBuffer);

	/* @copydoc ARenderer::VBuildStates */
	virtual void VBuildStates();

	// Member Variables
public:
protected:
private:
	
};

}

#endif	// __SLIMOPENGLRENDERER_H__


