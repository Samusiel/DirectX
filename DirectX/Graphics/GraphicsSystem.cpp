#include "stdafx.h"
#include "GraphicsSystem.hpp"

namespace Graphics
{
	/************************************************************************/
	/* Getters                                                              */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsSystem& GraphicsEntity::GetGraphicsSystem()
	{
		return this->m_graphicsSystem;
	}

	//------------------------------------------------------------------------
	const GraphicsTextureDesc& GraphicsTexture::GetDesc() const
	{
		return this->desc;
	}

	//------------------------------------------------------------------------
	GraphicsTexture* GraphicsRenderTarget::GetTragetBuffer() const
	{
		return this->targetBuffer.get();
	}

	//------------------------------------------------------------------------
	GraphicsTexture* GraphicsRenderTarget::GetDepthStencilBuffer() const
	{
		return this->depthStencilBuffer.get();
	}

	//------------------------------------------------------------------------
	const GraphicsBufferDesc& GraphicsBuffer::GetDesc() const
	{
		return this->desc;
	}

	//------------------------------------------------------------------------
	const cbyte* GraphicsShader::GetByteCode() const
	{
		return this->byteCode.get();
	}

	//------------------------------------------------------------------------
	size_t GraphicsShader::GetSizeInBytes() const
	{
		return this->sizeInBytes;
	}

	//------------------------------------------------------------------------
	const std::vector<GraphicsInputElement> GraphicsInputLayout::GetElements() const
	{
		return this->elements;
	}

	//------------------------------------------------------------------------
	const GraphicsRenderState& GraphicsPipelineState::GetRenderState()
	{
		return this->renderState;
	}

	//------------------------------------------------------------------------
	GraphicsSystem::GraphicsSystem()
	{
	}

	//------------------------------------------------------------------------
	GraphicsSystem::~GraphicsSystem()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::Initialize(Window *window)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::Render()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreateContext(std::unique_ptr<GraphicsContext> &context)
	{
		context.reset(new GraphicsContext(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreateTexture(std::unique_ptr<GraphicsTexture> &texture)
	{
		texture.reset(new GraphicsTexture(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreateRenderTarget(std::unique_ptr<GraphicsRenderTarget> &rt)
	{
		rt.reset(new GraphicsRenderTarget(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreateSwapChain(std::unique_ptr<GraphicsSwapChain> &swapChain)
	{
		swapChain.reset(new GraphicsSwapChain(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreateBuffer(std::unique_ptr<GraphicsBuffer> &buffer)
	{
		buffer.reset(new GraphicsBuffer(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreateVertexShader(std::unique_ptr<GraphicsVertexShader> &vertexShader)
	{
		vertexShader.reset(new GraphicsVertexShader(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreatePixelShader(std::unique_ptr<GraphicsPixelShader> &pixelShader)
	{
		pixelShader.reset(new GraphicsPixelShader(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreateInputLayout(std::unique_ptr<GraphicsInputLayout> &inputLayout)
	{
		inputLayout.reset(new GraphicsInputLayout(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystem::CreatePipelineState(std::unique_ptr<GraphicsPipelineState> &pipelineState)
	{
		pipelineState.reset(new GraphicsPipelineState(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	GraphicsEntity::GraphicsEntity(GraphicsSystem &system):
		m_graphicsSystem(system)
	{
	}

	//------------------------------------------------------------------------
	GraphicsEntity::~GraphicsEntity()
	{
	}

	//------------------------------------------------------------------------
	GraphicsContext::GraphicsContext(GraphicsSystem &system) :
		GraphicsEntity(system)
	{

	}

	//------------------------------------------------------------------------
	GraphicsContext::~GraphicsContext()
	{

	}

	//------------------------------------------------------------------------
	Result GraphicsContext::Initialize()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::Begin()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::End()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetRenderTarget(GraphicsRenderTarget *rt)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetViewPort(const GraphicsViewPort &viewPort)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetScissorRect(const Math::Rect &rect)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::ClearTarget(
		GraphicsRenderTarget *rt,
		bool clearColor, const Vector4 &color,
		bool clearDepth, const float &depth,
		bool clearStencil, const uint8 &stencil)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetPipelineState(GraphicsPipelineState *state)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetTexture(GraphicsTexture *texture, uint32 slot)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetConstantBuffer(GraphicsBuffer *buffer, uint32 slot)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetVertexBuffer(GraphicsBuffer *buffer, uint32 slot, uint32 stride, uint32 offset)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetIndexBuffer(GraphicsBuffer *buffer, uint32 bitsPerIndex, uint32 offset)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetVertexShader(GraphicsVertexShader *shader)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::SetPixelShader(GraphicsPixelShader *shader)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::Draw(uint32 vertexCount, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::UpdateBuffer(GraphicsBuffer *buffer, GraphicsGPUAccess gpuAccess, bool doNotWait, const GraphicsResourceData *srcData, uint32 offset, uint32 size)
	{
		const auto &copyFunc = [&](GraphicsResourceData *dstData) -> Result
		{
			if (buffer == nullptr || dstData == nullptr || srcData == nullptr)
				return Result(InvalidArgs);

			uint32 copySize = (size > 0 ? size : buffer->GetDesc().Size);
			if (offset + copySize > buffer->GetDesc().Size)
				return Result(InvalidArgs);

			memcpy((cbyte*)dstData->Ptr + offset, srcData->Ptr, copySize);

			return Result(Success);
		};

		return this->UpdateBuffer(buffer, gpuAccess, doNotWait, copyFunc);
	}

	//------------------------------------------------------------------------
	Result GraphicsContext::UpdateBuffer(GraphicsBuffer *buffer, GraphicsGPUAccess gpuAccess, bool doNotWait, const UpdateBufferCallback &callback)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	GraphicsTexture::GraphicsTexture(GraphicsSystem &GraphicsSystem) :
		GraphicsEntity(GraphicsSystem)
	{
		memset(&this->desc, 0, sizeof(this->desc));
	}

	//------------------------------------------------------------------------
	GraphicsTexture::~GraphicsTexture()
	{

	}

	//------------------------------------------------------------------------
	Result GraphicsTexture::Initialize(const GraphicsTextureDesc &desc, const GraphicsResourceData *data)
	{
		this->desc = desc;

		return this->OnInitialize(data);
	}

	//------------------------------------------------------------------------
	Result GraphicsTexture::Initialize(
		uint32 width,
		uint32 height,
		uint32 levels,
		GraphicsFormat format,
		GraphicsUsage usage,
		uint32 bindFlags,
		uint32 accessFlags)
	{
		GraphicsTextureDesc desc;
		desc.Width = width;
		desc.Height = height;
		desc.Levels = levels;
		desc.Format = format;
		desc.FormatView = GraphicsFormatView::Unorm;
		desc.Usage = usage;
		desc.BindFlags = bindFlags;
		desc.AccessFlags = accessFlags;

		return this->Initialize(desc, nullptr);
	}

	//------------------------------------------------------------------------
	Result GraphicsTexture::OnInitialize(const GraphicsResourceData *data)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	GraphicsRenderTarget::GraphicsRenderTarget(GraphicsSystem &GraphicsSystem) :
		GraphicsEntity(GraphicsSystem)
	{
	}

	//------------------------------------------------------------------------
	GraphicsRenderTarget::~GraphicsRenderTarget()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsRenderTarget::Initialize(const GraphicsTextureDesc &desc, bool hasDepthStencil, GraphicsFormat dsFormat)
	{
		std::unique_ptr<GraphicsTexture> newTargetBuffer;
		std::unique_ptr<GraphicsTexture> newDepthStencilBuffer;

		Result res = this->InitializeTargetBuffer(newTargetBuffer, desc);
		if (Failed(res))
			return Result(res.code());

		if (hasDepthStencil)
		{
			GraphicsTextureDesc dsDesc = desc;
			dsDesc.BindFlags = uint32(GraphicsBindFlag::DepthStencil) | uint32(GraphicsBindFlag::ShaderResource);
			dsDesc.Format = dsFormat;

			res = this->InitializeDepthStencil(newDepthStencilBuffer, dsDesc);
			if (Failed(res))
				return Result(res.code());
		}

		this->targetBuffer = std::move(newTargetBuffer);
		this->depthStencilBuffer = std::move(newDepthStencilBuffer);

		return this->OnInitialize();
	}

	//------------------------------------------------------------------------
	Result GraphicsRenderTarget::Initialize(
		uint32 width,
		uint32 height,
		uint32 levels,
		GraphicsFormat format,
		bool hasDepthStencil, GraphicsFormat dsFormat)
	{
		GraphicsTextureDesc desc;
		desc.Width = width;
		desc.Height = height;
		desc.Levels = levels;
		desc.Format = format;
		desc.FormatView = GraphicsFormatView::Unorm;
		desc.Usage = GraphicsUsage::Default;
		desc.BindFlags = uint32(GraphicsBindFlag::RenderTarget) | uint32(GraphicsBindFlag::ShaderResource);
		desc.AccessFlags = uint32(0);

		return this->Initialize(desc, hasDepthStencil, dsFormat);
	}

	//------------------------------------------------------------------------
	Result GraphicsRenderTarget::InitializeTargetBuffer(std::unique_ptr<GraphicsTexture> &resultBuffer, const GraphicsTextureDesc &desc)
	{
		Result res = this->GetGraphicsSystem().CreateTexture(resultBuffer);
		if (Succeeded(res))
		{
			res = resultBuffer->Initialize(desc, nullptr);
		}
		return res;
	}

	//------------------------------------------------------------------------
	Result GraphicsRenderTarget::InitializeDepthStencil(std::unique_ptr<GraphicsTexture> &resultBuffer, const GraphicsTextureDesc &desc)
	{
		Result res = this->GetGraphicsSystem().CreateTexture(resultBuffer);
		if (Succeeded(res))
		{
			res = resultBuffer->Initialize(desc, nullptr);
		}
		return res;
	}

	//------------------------------------------------------------------------
	Result GraphicsRenderTarget::OnInitialize()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	GraphicsSwapChain::GraphicsSwapChain(GraphicsSystem &GraphicsSystem) :
		GraphicsEntity(GraphicsSystem)
	{
	}

	//------------------------------------------------------------------------
	GraphicsSwapChain::~GraphicsSwapChain()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsSwapChain::Initialize(const GraphicsPresentParams &params)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	Result GraphicsSwapChain::Initialize(
		uint32 bufferWidth,
		uint32 bufferHeight,
		bool fullscreen,
		GraphicsFormat bufferFormat,
		bool depthStencilEnabled,
		GraphicsFormat depthStencilFormat,
		uint32 bufferCount,
		uint32 fullscreenRefreshRate,
		uint32 mutisampleCount,
		uint32 mutisampleQuality)
	{
		GraphicsPresentParams params;
		params.BufferWidth = bufferWidth;
		params.BufferHeight = bufferHeight;
		params.BufferCount = bufferCount;
		params.BufferFormat = bufferFormat;
		params.Fullscreen = fullscreen;
		params.FullscreenRefreshRate = fullscreenRefreshRate;
		params.DepthStencilEnabled = depthStencilEnabled;
		params.DepthStencilFormat = depthStencilFormat;
		params.MutisampleCount = mutisampleCount;
		params.MutisampleQuality = mutisampleQuality;

		return this->Initialize(params);
	}

	//------------------------------------------------------------------------
	Result GraphicsSwapChain::Present()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	GraphicsRenderTarget* GraphicsSwapChain::GetTargetBuffer()
	{
		return nullptr;
	}

	//------------------------------------------------------------------------
	GraphicsBuffer::GraphicsBuffer(GraphicsSystem &GraphicsSystem) :
		GraphicsEntity(GraphicsSystem)
	{
	}

	//------------------------------------------------------------------------
	GraphicsBuffer::~GraphicsBuffer()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsBuffer::Initialize(const GraphicsBufferDesc &desc, const GraphicsResourceData *data)
	{
		this->desc = desc;

		return this->OnInitialize(data);
	}

	//------------------------------------------------------------------------
	Result GraphicsBuffer::Initialize(
		uint32 sizeInBytes,
		GraphicsUsage usage,
		uint32 bindFlags,
		uint32 accessFlags,
		const GraphicsResourceData *data)
	{
		GraphicsBufferDesc desc;
		desc.Size = sizeInBytes;
		desc.Usage = usage;
		desc.BindFlags = bindFlags;
		desc.AccessFlags = accessFlags;
		desc.StructureStride = 0;

		return this->Initialize(desc, data);
	}

	//------------------------------------------------------------------------
	Result GraphicsBuffer::OnInitialize(const GraphicsResourceData *data)
	{
		return Result(NotImplemented);
	}
	
	//------------------------------------------------------------------------
	GraphicsShader::GraphicsShader(GraphicsSystem &GraphicsSystem) :
		GraphicsEntity(GraphicsSystem)
	{
		this->sizeInBytes = 0;
	}

	//------------------------------------------------------------------------
	GraphicsShader::~GraphicsShader()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsShader::Initialize(std::unique_ptr<cbyte[]> &byteCode, size_t sizeInBytes)
	{
		this->byteCode = std::move(byteCode);
		this->sizeInBytes = sizeInBytes;

		if (nullptr == this->byteCode || 0 == this->sizeInBytes)
			return Result(InvalidArgs);

		return this->OnInitialize();
	}

	//------------------------------------------------------------------------
	Result GraphicsShader::OnInitialize()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	GraphicsVertexShader::GraphicsVertexShader(GraphicsSystem &GraphicsSystem) :
		GraphicsShader(GraphicsSystem)
	{
	}

	//------------------------------------------------------------------------
	GraphicsVertexShader::~GraphicsVertexShader()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsVertexShader::OnInitialize()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	GraphicsPixelShader::GraphicsPixelShader(GraphicsSystem &GraphicsSystem) :
		GraphicsShader(GraphicsSystem)
	{
	}

	//------------------------------------------------------------------------
	GraphicsPixelShader::~GraphicsPixelShader()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsPixelShader::OnInitialize()
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	GraphicsInputLayout::GraphicsInputLayout(GraphicsSystem &GraphicsSystem) :
		GraphicsEntity(GraphicsSystem)
	{
	}

	//------------------------------------------------------------------------
	GraphicsInputLayout::~GraphicsInputLayout()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsInputLayout::Initialize(const GraphicsShader &shader, const GraphicsInputElement *elements, uint32 count)
	{
		this->elements.resize(count);

		size_t size = sizeof(GraphicsInputElement) * count;
		if (nullptr == elements || 0 == size)
			return Result(InvalidArgs);

		memcpy(this->elements.data(), elements, size);

		return this->OnInitialize(shader, elements, count);
	}

	//------------------------------------------------------------------------
	Result GraphicsInputLayout::OnInitialize(const GraphicsShader &shader, const GraphicsInputElement *elements, uint32 count)
	{
		return Result(NotImplemented);
	}

	//------------------------------------------------------------------------
	GraphicsPipelineState::GraphicsPipelineState(GraphicsSystem &GraphicsSystem) :
		GraphicsEntity(GraphicsSystem)
	{
	}

	//------------------------------------------------------------------------
	GraphicsPipelineState::~GraphicsPipelineState()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsPipelineState::SetRenderState(const GraphicsRenderState &renderState)
	{
		this->renderState = renderState;
		return this->OnSetRenderState(renderState);
	}

	//------------------------------------------------------------------------
	Result GraphicsPipelineState::OnSetRenderState(const GraphicsRenderState &renderState)
	{
		return Result(Success);
	}
}