#pragma once

namespace System
{
	class Window;
};
using namespace System;

namespace Graphics
{
	//-- Forward Declarations
	class GraphicsEntity;
	class GraphicsContext;
	class GraphicsTexture;
	class GraphicsRenderTarget;
	class GraphicsSwapChain;
	class GraphicsBuffer;
	class GraphicsShader;
	class GraphicsVertexShader;
	class GraphicsPixelShader;
	class GraphicsInputLayout;
	class GraphicsPipelineState;

	/************************************************************************/
	/* Base graphics system                                                 */
	/************************************************************************/
	class GraphicsSystem
	{
	public:
		GraphicsSystem();

		virtual ~GraphicsSystem();

		virtual Result Initialize(Window *window);

		virtual Result Render();

		virtual Result CreateContext(std::unique_ptr<GraphicsContext> &context);

		virtual Result CreateTexture(std::unique_ptr<GraphicsTexture> &gtexture);

		virtual Result CreateRenderTarget(std::unique_ptr<GraphicsRenderTarget> &rt);

		virtual Result CreateSwapChain(std::unique_ptr<GraphicsSwapChain> &swapChain);

		virtual Result CreateBuffer(std::unique_ptr<GraphicsBuffer> &buffer);

		virtual Result CreateVertexShader(std::unique_ptr<GraphicsVertexShader> &vertexShader);

		virtual Result CreatePixelShader(std::unique_ptr<GraphicsPixelShader> &pixelShader);

		virtual Result CreateInputLayout(std::unique_ptr<GraphicsInputLayout> &inputLayout);

		virtual Result CreatePipelineState(std::unique_ptr<GraphicsPipelineState> &pipelineState);
	};

	/************************************************************************/
	/* Base graphics system entity                                          */
	/************************************************************************/
	class GraphicsEntity
	{
	public:

		GraphicsEntity(GraphicsSystem &system);

		virtual ~GraphicsEntity();

		inline GraphicsSystem& GetGraphicsSystem();

	private:

		GraphicsSystem &m_graphicsSystem;
	};

	/************************************************************************/
	/* Base graphics context                                                */
	/************************************************************************/
	class GraphicsContext : public GraphicsEntity
	{
	public:

		GraphicsContext(GraphicsSystem &system);

		virtual ~GraphicsContext();

		virtual Result Initialize();

		virtual Result Begin();

		virtual Result End();

		virtual Result SetRenderTarget(GraphicsRenderTarget *rt);

		virtual Result SetViewPort(const GraphicsViewPort &viewPort);

		virtual Result SetScissorRect(const Rect &rect);

		virtual Result ClearTarget(
			GraphicsRenderTarget *rt,
			bool clearColor, const Vector4 &color,
			bool clearDepth, const float &depth,
			bool clearStencil, const uint8 &stencil);

		virtual Result SetPipelineState(GraphicsPipelineState *state);

		virtual Result SetTexture(GraphicsTexture *texture, uint32 slot);

		virtual Result SetConstantBuffer(GraphicsBuffer *buffer, uint32 slot);

		virtual Result SetVertexBuffer(GraphicsBuffer *buffer, uint32 slot, uint32 stride, uint32 offset);

		virtual Result SetIndexBuffer(GraphicsBuffer *buffer, uint32 bitsPerIndex, uint32 offset);

		virtual Result SetVertexShader(GraphicsVertexShader *shader);

		virtual Result SetPixelShader(GraphicsPixelShader *shader);

		virtual Result Draw(uint32 vertexCount, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset);

		virtual Result DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset);

		virtual Result UpdateBuffer(GraphicsBuffer *buffer, GraphicsGPUAccess gpuAccess, bool doNotWait, const GraphicsResourceData *data, uint32 offset, uint32 size);

		typedef std::function<Result(GraphicsResourceData *data)> UpdateBufferCallback;
		virtual Result UpdateBuffer(GraphicsBuffer *buffer, GraphicsGPUAccess gpuAccess, bool doNotWait, const UpdateBufferCallback &callback);
	};

	/************************************************************************/
	/* Base graphics texture                                                */
	/************************************************************************/
	class GraphicsTexture : public GraphicsEntity
	{
	public:

		GraphicsTexture(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsTexture();

		Result Initialize(const GraphicsTextureDesc &desc, const GraphicsResourceData *data);

		Result Initialize(
			uint32 width,
			uint32 height,
			uint32 levels = 1,
			GraphicsFormat format = GraphicsFormat::R8G8B8A8,
			GraphicsUsage usage = GraphicsUsage::Default,
			uint32 bindFlags = uint32(GraphicsBindFlag::ShaderResource),
			uint32 accessFlags = uint32(0));

		inline const GraphicsTextureDesc& GetDesc() const;

	protected:

		virtual Result OnInitialize(const GraphicsResourceData *data);

	private:

		GraphicsTextureDesc desc;
	};

	/************************************************************************/
	/* Base graphics render target                                          */
	/************************************************************************/
	class GraphicsRenderTarget : public GraphicsEntity
	{
	public:

		GraphicsRenderTarget(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsRenderTarget();

		Result Initialize(const GraphicsTextureDesc &desc, bool hasDepthStencil, GraphicsFormat dsFormat);

		Result Initialize(
			uint32 width = 0,
			uint32 height = 0,
			uint32 levels = 1,
			GraphicsFormat format = GraphicsFormat::R8G8B8A8,
			bool hasDepthStencil = true, GraphicsFormat dsFormat = GraphicsFormat::R24G8);

		inline GraphicsTexture* GetTragetBuffer() const;

		inline GraphicsTexture* GetDepthStencilBuffer() const;

	protected:

		virtual Result InitializeTargetBuffer(std::unique_ptr<GraphicsTexture> &resultBuffer, const GraphicsTextureDesc &desc);

		virtual Result InitializeDepthStencil(std::unique_ptr<GraphicsTexture> &resultBuffer, const GraphicsTextureDesc &desc);

		virtual Result OnInitialize();

	private:

		std::unique_ptr<GraphicsTexture> targetBuffer;
		std::unique_ptr<GraphicsTexture> depthStencilBuffer;
	};

	/************************************************************************/
	/* Base graphics render targets swap chain                              */
	/************************************************************************/
	class GraphicsSwapChain : public GraphicsEntity
	{
	public:

		GraphicsSwapChain(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsSwapChain();

		virtual Result Initialize(const GraphicsPresentParams &params);

		Result Initialize(
			uint32 bufferWidth,
			uint32 bufferHeight,
			bool fullscreen = false,
			GraphicsFormat bufferFormat = GraphicsFormat::R8G8B8A8,
			bool depthStencilEnabled = true,
			GraphicsFormat depthStencilFormat = GraphicsFormat::R24G8,
			uint32 bufferCount = 2,
			uint32 fullscreenRefreshRate = 59,
			uint32 mutisampleCount = 1,
			uint32 mutisampleQuality = 0);

		virtual Result Present();

		virtual GraphicsRenderTarget* GetTargetBuffer();
	};

	/************************************************************************/
	/* Base graphics buffer                                                 */
	/************************************************************************/
	class GraphicsBuffer : public GraphicsEntity
	{
	public:

		GraphicsBuffer(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsBuffer();

		Result Initialize(const GraphicsBufferDesc &desc, const GraphicsResourceData *data);

		Result Initialize(
			uint32 sizeInBytes,
			GraphicsUsage usage = GraphicsUsage::Default,
			uint32 bindFlags = uint32(0),
			uint32 accessFlags = uint32(0),
			const GraphicsResourceData *data = nullptr);

		inline const GraphicsBufferDesc& GetDesc() const;

	protected:

		virtual Result OnInitialize(const GraphicsResourceData *data);

	private:

		GraphicsBufferDesc desc;
	};

	/************************************************************************/
	/* Base graphics shader program                                         */
	/************************************************************************/
	class GraphicsShader : public GraphicsEntity
	{
	public:

		GraphicsShader(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsShader();

		Result Initialize(std::unique_ptr<byte[]> &byteCode, size_t sizeInBytes);

		inline const byte* GetByteCode() const;

		inline size_t GetSizeInBytes() const;

	protected:

		virtual Result OnInitialize();

	private:

		std::unique_ptr<byte[]> byteCode;
		size_t sizeInBytes;
	};

	/************************************************************************/
	/* Base graphics vertex shader                                          */
	/************************************************************************/
	class GraphicsVertexShader : public GraphicsShader
	{
	public:

		GraphicsVertexShader(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsVertexShader();

	protected:

		virtual Result OnInitialize();
	};

	/************************************************************************/
	/* Base graphics pixel shader                                           */
	/************************************************************************/
	class GraphicsPixelShader : public GraphicsShader
	{
	public:

		GraphicsPixelShader(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsPixelShader();

	protected:

		virtual Result OnInitialize();
	};

	/************************************************************************/
	/* Graphics input layout                                                */
	/************************************************************************/
	class GraphicsInputLayout : public GraphicsEntity
	{
	public:

		GraphicsInputLayout(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsInputLayout();

		Result Initialize(const GraphicsShader &shader, const GraphicsInputElement *elements, uint32 count);

		inline const std::vector<GraphicsInputElement> GetElements() const;

	protected:

		virtual Result OnInitialize(const GraphicsShader &shader, const GraphicsInputElement *elements, uint32 count);

	private:

		std::vector<GraphicsInputElement> elements;
	};

	/************************************************************************/
	/* Graphics pipeline state full description                             */
	/************************************************************************/
	class GraphicsPipelineState : public GraphicsEntity
	{
	public:

		GraphicsInputLayout *InputLayout;
		GraphicsVertexShader *VertexShader;
		GraphicsPixelShader *PixelShader;

	public:

		GraphicsPipelineState(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsPipelineState();

		Result SetRenderState(const GraphicsRenderState &renderState);

		inline const GraphicsRenderState& GetRenderState();

	protected:

		virtual Result OnSetRenderState(const GraphicsRenderState &renderState);

	private:

		GraphicsRenderState renderState;
	};
}