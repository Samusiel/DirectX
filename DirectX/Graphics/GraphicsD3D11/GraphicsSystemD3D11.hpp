#pragma once

#include "Graphics/GraphicsSystem.hpp"

#include <dxgi.h>
#include <d3d11.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace System
{
	class WinWindow;
}

namespace Graphics
{
	/************************************************************************/
	/* Direct3D11 graphics system                                           */
	/************************************************************************/
	class GraphicsSystemD3D11 : public GraphicsSystem
	{
	public:

		GraphicsSystemD3D11();

		virtual ~GraphicsSystemD3D11();

		virtual Result Initialize(Window *window);

		virtual Result Render();

		virtual Result CreateContext(std::unique_ptr<GraphicsContext> &GraphicsContext);

		virtual Result CreateTexture(std::unique_ptr<GraphicsTexture> &GraphicsTexture);

		virtual Result CreateRenderTarget(std::unique_ptr<GraphicsRenderTarget> &GraphicsRT);

		virtual Result CreateSwapChain(std::unique_ptr<GraphicsSwapChain> &GraphicsSwapChain);

		virtual Result CreateBuffer(std::unique_ptr<GraphicsBuffer> &GraphicsBuffer);

		virtual Result CreateVertexShader(std::unique_ptr<GraphicsVertexShader> &GraphicsVertexShader);

		virtual Result CreatePixelShader(std::unique_ptr<GraphicsPixelShader> &GraphicsPixelShader);

		virtual Result CreateInputLayout(std::unique_ptr<GraphicsInputLayout> &GraphicsInputLayout);

		virtual Result CreatePipelineState(std::unique_ptr<GraphicsPipelineState> &GraphicsPipelineState);

		inline WinWindow *GetWinWindow() const;

		inline IDXGIFactory1* GetDXGIFactory() const;

		inline ID3D11Device* GetDevice() const;

		inline ID3D11DeviceContext* GetDeviceContext() const;

		inline void AddCommandList(std::shared_ptr<ID3D11CommandList> &d3dCommandList);

	private:

		Result InitializeDXGI();

		Result InitializeDevice(IDXGIAdapter *pAdapter,
			const D3D_FEATURE_LEVEL *featureLevels,
			UINT levelsCount);

		void ReleaseDXGIObjects();

		void ReleaseDeviceObjects();

		WinWindow										*m_pWindow;
		std::shared_ptr<IDXGIFactory1>					m_pDXGIFactory;
		std::vector<std::shared_ptr<IDXGIAdapter1>>		m_dxgiAdapters;
		D3D_FEATURE_LEVEL								m_d3dFeatureLevel;
		std::shared_ptr<ID3D11Device>					m_pD3DDevice;
		std::shared_ptr<ID3D11DeviceContext>			m_pD3DImmediateContext;
		std::vector<std::shared_ptr<ID3D11CommandList>> m_d3dCommandLists[2];
		uint32											m_commandListId;
		std::mutex										m_commandListMutex;
	};

	/************************************************************************/
	/* Direct3D11 graphics context                                          */
	/************************************************************************/
	class GraphicsContextD3D11 : public GraphicsContext
	{
	public:

		GraphicsContextD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsContextD3D11();

		virtual Result Initialize();

		virtual Result Begin();

		virtual Result End();

		virtual Result SetRenderTarget(GraphicsRenderTarget *rt);

		virtual Result SetViewPort(const GraphicsViewPort *viewPort);

		virtual Result SetScissorRect(const Rect *rect);

		virtual Result ClearTarget(
			GraphicsRenderTarget *rt,
			bool clearColor, const Vector4 &color,
			bool clearDepth, const float &depth,
			bool clearStencil, const uint32 &stencil);

		virtual Result SetPipelineState(GraphicsPipelineState *state);

		virtual Result SetTexture(GraphicsTexture *texture, uint32 slot);

		virtual Result SetConstantBuffer(GraphicsBuffer *buffer, uint32 slot);

		virtual Result SetVertexBuffer(GraphicsBuffer *buffer, uint32 slot, uint32 stride, uint32 offset);

		virtual Result SetIndexBuffer(GraphicsBuffer *buffer, uint32 bitsPerIndex, uint32 offset);

		virtual Result SetVertexShader(GraphicsVertexShader *shader);

		virtual Result SetPixelShader(GraphicsPixelShader *shader);

		virtual Result Draw(uint32 vertexCount, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset);

		virtual Result DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset);

		virtual Result UpdateBuffer(GraphicsBuffer *buffer, GraphicsGPUAccess gpuAccess, bool doNotWait, UpdateBufferCallback callback);

	private:

		std::shared_ptr<ID3D11DeviceContext> m_pD3DContext;
	};

	/************************************************************************/
	/* Direct3D11 graphics texture                                          */
	/************************************************************************/
	class GraphicsTextureD3D11 : public GraphicsTexture
	{
	public:

		GraphicsTextureD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsTextureD3D11();

		Result Initialize(const GraphicsTextureDesc &desc, std::shared_ptr<ID3D11Texture2D> &d3dTexture);

		inline ID3D11Texture2D* GetD3DTexture() const;

		inline ID3D11ShaderResourceView* GetSRV() const;

	protected:

		virtual Result OnInitialize(const GraphicsResourceData *data);

	private:

		std::shared_ptr<ID3D11Texture2D>			m_pD3DTexture;
		std::shared_ptr<ID3D11ShaderResourceView>	m_pD3DSRV;
	};

	/************************************************************************/
	/* Direct3D11 graphics render target                                    */
	/************************************************************************/
	class GraphicsRenderTargetD3D11 : public GraphicsRenderTarget
	{
	public:

		GraphicsRenderTargetD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsRenderTargetD3D11();

		inline ID3D11RenderTargetView* GetRTView() const;

		inline ID3D11DepthStencilView* GetDSView() const;

	protected:

		virtual Result OnInitialize();

	private:

		std::shared_ptr<ID3D11RenderTargetView> m_pD3DRTView;
		std::shared_ptr<ID3D11DepthStencilView> m_pD3DDSView;
	};

	/************************************************************************/
	/* Render target swap chain                                             */
	/************************************************************************/
	class GraphicsSwapChainD3D11 : public GraphicsSwapChain
	{
	public:

		GraphicsSwapChainD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsSwapChainD3D11();

		virtual Result Initialize(const GraphicsPresentParams &params);

		virtual Result Present();

		virtual GraphicsRenderTarget* GetTargetBuffer();

	protected:

		class RenderTarget : public GraphicsRenderTargetD3D11
		{
		public:

			RenderTarget(GraphicsSystem &GraphicsSystem, std::shared_ptr<ID3D11Texture2D> &dxgiSwapChainBuffer);

			virtual ~RenderTarget();

		protected:

			virtual Result InitializeTargetBuffer(std::unique_ptr<GraphicsTexture> &resultBuffer, const GraphicsTextureDesc &desc);

		private:

			std::shared_ptr<ID3D11Texture2D> m_pDXGISwapChainBuffer;
		};

	private:

		std::unique_ptr<RenderTarget>	m_pTargetBuffer;
		DXGI_SWAP_CHAIN_DESC			dxgiChainDesc;
		std::shared_ptr<IDXGISwapChain> m_pDXGISwapChain;
	};

	/************************************************************************/
	/* Direct3D11 graphics buffer                                           */
	/************************************************************************/
	class GraphicsBufferD3D11 : public GraphicsBuffer
	{
	public:

		GraphicsBufferD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsBufferD3D11();

		inline ID3D11Buffer* GetD3DBuffer() const;

	protected:

		virtual Result OnInitialize(const GraphicsResourceData *data);

	private:

		std::shared_ptr<ID3D11Buffer> m_pD3DBuffer;
	};

	/************************************************************************/
	/* Direct3D11 graphics vertex shader                                    */
	/************************************************************************/
	class GraphicsVertexShaderD3D11 : public GraphicsVertexShader
	{
	public:

		GraphicsVertexShaderD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsVertexShaderD3D11();

		inline ID3D11VertexShader* GetD3DShader() const;

	protected:

		virtual Result OnInitialize();

	private:

		std::shared_ptr<ID3D11VertexShader> m_pD3DVertexShader;
	};

	/************************************************************************/
	/* Direct3D11 graphics pixel shader                                     */
	/************************************************************************/
	class GraphicsPixelShaderD3D11 : public GraphicsPixelShader
	{
	public:

		GraphicsPixelShaderD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsPixelShaderD3D11();

		inline ID3D11PixelShader* GetD3DShader() const;

	protected:

		virtual Result OnInitialize();

	private:

		std::shared_ptr<ID3D11PixelShader> m_pD3DPixelShader;
	};

	/************************************************************************/
	/* Direct3D11 graphics input layout                                     */
	/************************************************************************/
	class GraphicsInputLayoutD3D11 : public GraphicsInputLayout
	{
	public:

		GraphicsInputLayoutD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsInputLayoutD3D11();

		inline ID3D11InputLayout* GetD3DInputLayout() const;

	protected:

		virtual Result OnInitialize(const GraphicsShader &shader, const GraphicsInputElement *elements, uint32 count);

	private:

		std::shared_ptr<ID3D11InputLayout> m_pD3DInputLayout;
	};

	/************************************************************************/
	/* Direct3D11 graphics pipeline state                                   */
	/************************************************************************/
	class GraphicsPipelineStateD3D11 : public GraphicsPipelineState
	{
	public:

		GraphicsPipelineStateD3D11(GraphicsSystem &GraphicsSystem);

		virtual ~GraphicsPipelineStateD3D11();

		inline ID3D11SamplerState* GetD3DSamplerState(uint32 idx) const;

		inline ID3D11RasterizerState* GetD3DRasterizerState() const;

		inline ID3D11DepthStencilState* GetD3DDepthStencilState() const;

		inline ID3D11BlendState* GetD3DBlendState() const;

	protected:

		virtual Result OnSetRenderState(const GraphicsRenderState &renderState);

	private:

		std::shared_ptr<ID3D11SamplerState>			m_pD3DSamplerState[GraphicsRenderState::MaxSamplerStates];
		std::shared_ptr<ID3D11RasterizerState>		m_pD3DRasterizerState;
		std::shared_ptr<ID3D11DepthStencilState>	m_pD3DDepthStencilState;
		std::shared_ptr<ID3D11BlendState>			m_pD3DBlendState;
		size_t m_hashSamplerState[GraphicsRenderState::MaxSamplerStates];
		size_t m_hashRasterizerState;
		size_t m_hashDepthStencilState;
		size_t m_hashBlendState;
	};

	/************************************************************************/
	/* Utilities                                                            */
	/************************************************************************/

	extern DXGI_FORMAT GraphicsFormatToDXGI(GraphicsFormat fmt, GraphicsFormatView view);

	extern DXGI_FORMAT GraphicsBitsPerIndexToDXGIFormat(uint32 bitsPerIndex);

	extern D3D11_USAGE GraphicsUsageToD3D11(GraphicsUsage usage);

	extern D3D11_BIND_FLAG GraphicsBindFlagToD3D11(GraphicsBindFlag flag);

	extern UINT GraphicsBindFlagsToD3D11(uint32 flags);

	extern D3D11_CPU_ACCESS_FLAG GraphicsAccessFlagToD3D11_CPUAccessFlag(GraphicsAccessFlag flag);

	extern UINT GraphicsAccessFlagsToD3D11_CPUAccessFlags(uint32 flags);

	extern D3D11_MAP GraphicsGPUAccessFlagToD3D11(GraphicsGPUAccess gpuAccess);

	extern D3D11_VIEWPORT GraphicsViewPortToD3D11(const GraphicsViewPort &viewPort);

	extern D3D11_RECT RectIToD3D11(const Rect &rect);

	extern D3D11_TEXTURE2D_DESC GraphicsTextureDescToD3D11(const GraphicsTextureDesc &desc);

	extern D3D11_BUFFER_DESC GraphicsBufferDescToD3D11(const GraphicsBufferDesc &desc);

	extern LPCSTR GraphicsSemanticToD3D11(GraphicsSemantic semantic);

	extern D3D11_INPUT_ELEMENT_DESC GraphicsInputElementToD3D11(const GraphicsInputElement &element);

	extern D3D11_PRIMITIVE_TOPOLOGY GraphicsPrimitiveTopologyToD3D11(GraphicsPrimitiveTopology topology);

	extern D3D11_RENDER_TARGET_BLEND_DESC GraphicsBlendStateToD3D11(const GraphicsBlendState &state);

	extern D3D11_BLEND GraphicsBlendFactorToD3D11(GraphicsBlendFactor blendFactor);

	extern D3D11_BLEND_OP GraphicsBlendOpToD3D11(GraphicsBlendOp blendOp);

	extern D3D11_COMPARISON_FUNC GraphicsCmpFuncToD3D11(GraphicsCmpFunc func);

	extern D3D11_FILTER GraphicsFilterToD3D11(GraphicsFilter minFilter, GraphicsFilter magFilter, GraphicsFilter mipFilter);

	extern D3D11_TEXTURE_ADDRESS_MODE GraphicsTextureAddressModeToD3D11(GraphicsTextureAddressMode mode);

	extern D3D11_SAMPLER_DESC GraphicsSamplerStateToD3D11(const GraphicsSamplerState &state);

	extern D3D11_FILL_MODE GraphicsFillModeToD3D11(GraphicsFillMode mode);

	extern D3D11_CULL_MODE GraphicsCullModeToD3D11(GraphicsCullMode mode);

	extern D3D11_RASTERIZER_DESC GraphicsRasterizerStateToD3D11(const GraphicsRasterizerState& state);

	extern D3D11_STENCIL_OP GraphicsStencilOpToD3D11(GraphicsStencilOp stencilOp);

	extern D3D11_DEPTH_STENCILOP_DESC GraphicsDepthStencilOpDescToD3D11(const GraphicsDepthStencilOpDesc& desc);

	extern D3D11_DEPTH_STENCIL_DESC GraphicsDepthStencilStateToD3D11(const GraphicsDepthStencilState &state);
}