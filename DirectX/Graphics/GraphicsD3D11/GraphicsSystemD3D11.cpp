#include "stdafx.h"
#include "GraphicsSystemD3D11.hpp"
#include "System/WinSystem/WinWindow.h"
#include "Math/Math.hpp"

namespace Graphics
{
	//------------------------------------------------------------------------
	WinWindow *GraphicsSystemD3D11::GetWinWindow() const
	{
		return this->m_pWindow;
	}

	//------------------------------------------------------------------------
	IDXGIFactory1* GraphicsSystemD3D11::GetDXGIFactory() const
	{
		return this->m_pDXGIFactory.get();
	}

	//------------------------------------------------------------------------
	ID3D11Device* GraphicsSystemD3D11::GetDevice() const
	{
		return this->m_pD3DDevice.get();
	}

	//------------------------------------------------------------------------
	ID3D11DeviceContext* GraphicsSystemD3D11::GetDeviceContext() const
	{
		return this->m_pD3DImmediateContext.get();
	}

	//------------------------------------------------------------------------
	void GraphicsSystemD3D11::AddCommandList(std::shared_ptr<ID3D11CommandList> &d3dCommandList)
	{
		std::lock_guard<std::mutex> lock(m_commandListMutex);

		m_d3dCommandLists[m_commandListId].push_back(d3dCommandList);
	}

	//------------------------------------------------------------------------
	ID3D11Texture2D* GraphicsTextureD3D11::GetD3DTexture() const
	{
		return this->m_pD3DTexture.get();
	}

	//------------------------------------------------------------------------
	ID3D11ShaderResourceView* GraphicsTextureD3D11::GetSRV() const
	{
		return this->m_pD3DSRV.get();
	}

	//------------------------------------------------------------------------
	ID3D11RenderTargetView* GraphicsRenderTargetD3D11::GetRTView() const
	{
		return this->m_pD3DRTView.get();
	}

	//------------------------------------------------------------------------
	ID3D11DepthStencilView* GraphicsRenderTargetD3D11::GetDSView() const
	{
		return this->m_pD3DDSView.get();
	}

	//------------------------------------------------------------------------
	ID3D11Buffer* GraphicsBufferD3D11::GetD3DBuffer() const
	{
		return this->m_pD3DBuffer.get();
	}

	//------------------------------------------------------------------------
	ID3D11VertexShader* GraphicsVertexShaderD3D11::GetD3DShader() const
	{
		return this->m_pD3DVertexShader.get();
	}

	//------------------------------------------------------------------------
	ID3D11PixelShader* GraphicsPixelShaderD3D11::GetD3DShader() const
	{
		return this->m_pD3DPixelShader.get();
	}

	//------------------------------------------------------------------------
	ID3D11InputLayout* GraphicsInputLayoutD3D11::GetD3DInputLayout() const
	{
		return this->m_pD3DInputLayout.get();
	}

	//------------------------------------------------------------------------
	ID3D11SamplerState* GraphicsPipelineStateD3D11::GetD3DSamplerState(uint32 idx) const
	{
		return (idx < GraphicsRenderState::MaxSamplerStates ? this->m_pD3DSamplerState[idx].get() : nullptr);
	}

	//------------------------------------------------------------------------
	ID3D11RasterizerState* GraphicsPipelineStateD3D11::GetD3DRasterizerState() const
	{
		return this->m_pD3DRasterizerState.get();
	}

	//------------------------------------------------------------------------
	ID3D11DepthStencilState* GraphicsPipelineStateD3D11::GetD3DDepthStencilState() const
	{
		return this->m_pD3DDepthStencilState.get();
	}

	//------------------------------------------------------------------------
	ID3D11BlendState* GraphicsPipelineStateD3D11::GetD3DBlendState() const
	{
		return this->m_pD3DBlendState.get();
	}

	//------------------------------------------------------------------------
	static const D3D_FEATURE_LEVEL DefaultFeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	/************************************************************************/
	/* GraphicsSystemD3D11                                                  */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsSystemD3D11::GraphicsSystemD3D11()
	{
		this->m_d3dFeatureLevel = D3D_FEATURE_LEVEL(-1);
		this->m_commandListId = 0;
	}

	//------------------------------------------------------------------------
	GraphicsSystemD3D11::~GraphicsSystemD3D11()
	{
		this->ReleaseDeviceObjects();
		this->ReleaseDXGIObjects();
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::Initialize(Window *window)
	{
		if (window == nullptr)
			return Result(InvalidArgs);

		if (typeid(*window).hash_code() != typeid(WinWindow).hash_code())
			return Result(InvalidArgs);
		
		m_pWindow = static_cast<WinWindow*>(window);
		Result res = this->InitializeDXGI();
		if (Succeeded(res))
		{
			res = this->InitializeDevice(nullptr, DefaultFeatureLevels, ARRAYSIZE(DefaultFeatureLevels));
		}

		return Succeeded(res) ? Result(Success) : Result(res.code());
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::Render()
	{
		if (this->m_pD3DDevice == nullptr || this->m_pD3DImmediateContext == nullptr)
			return Result(NotInitialized);

		{ // swap command lists
			std::lock_guard<std::mutex> lock(this->m_commandListMutex);
			this->m_commandListId = !this->m_commandListId;
		}

		// execute command lists
		for (auto &d3dCommandList : this->m_d3dCommandLists[!this->m_commandListId])
		{
			this->m_pD3DImmediateContext->ExecuteCommandList(d3dCommandList.get(), FALSE);
		}
		this->m_d3dCommandLists[!this->m_commandListId].clear();

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreateContext(std::unique_ptr<GraphicsContext> &GraphicsContext)
	{
		GraphicsContext.reset(new GraphicsContextD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreateTexture(std::unique_ptr<GraphicsTexture> &GraphicsTexture)
	{
		GraphicsTexture.reset(new GraphicsTextureD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreateRenderTarget(std::unique_ptr<GraphicsRenderTarget> &GraphicsRT)
	{
		GraphicsRT.reset(new GraphicsRenderTargetD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreateSwapChain(std::unique_ptr<GraphicsSwapChain> &GraphicsSwapChain)
	{
		GraphicsSwapChain.reset(new GraphicsSwapChainD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreateBuffer(std::unique_ptr<GraphicsBuffer> &GraphicsBuffer)
	{
		GraphicsBuffer.reset(new GraphicsBufferD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreateVertexShader(std::unique_ptr<GraphicsVertexShader> &GraphicsVertexShader)
	{
		GraphicsVertexShader.reset(new GraphicsVertexShaderD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreatePixelShader(std::unique_ptr<GraphicsPixelShader> &GraphicsPixelShader)
	{
		GraphicsPixelShader.reset(new GraphicsPixelShaderD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreateInputLayout(std::unique_ptr<GraphicsInputLayout> &GraphicsInputLayout)
	{
		GraphicsInputLayout.reset(new GraphicsInputLayoutD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::CreatePipelineState(std::unique_ptr<GraphicsPipelineState> &GraphicsPipelineState)
	{
		GraphicsPipelineState.reset(new GraphicsPipelineStateD3D11(*this));
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::InitializeDXGI()
	{
		this->ReleaseDXGIObjects();

		HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&this->m_pDXGIFactory));
		if (FAILED(hr))
			return Result(Failure);

		UINT adapterId = 0;
		do
		{
			IDXGIAdapter1* dxgiAdapter;
			hr = this->m_pDXGIFactory->EnumAdapters1(adapterId++, &dxgiAdapter);
			if (SUCCEEDED(hr))
			{
				this->m_dxgiAdapters.push_back(std::shared_ptr<IDXGIAdapter1>(dxgiAdapter));
			}
		} while (SUCCEEDED(hr));

		return (!this->m_dxgiAdapters.empty() ?	Result(Success) : Result(Failure));
	}

	//------------------------------------------------------------------------
	Result GraphicsSystemD3D11::InitializeDevice(IDXGIAdapter *pAdapter,
		const D3D_FEATURE_LEVEL *featureLevels,
		UINT levelsCount)
	{
		this->ReleaseDeviceObjects();

		// create D3D device and immediate context

		if (this->m_dxgiAdapters.empty())
			return Result(Failure);

		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_UNKNOWN;
		UINT flags = 0;
#ifdef _DEBUG
		//flags |= D3D11_CREATE_DEVICE_DEBUG; // in Windows 10 the Windows SDK does not install the Direct3D debug layer
#endif

		if (pAdapter == nullptr)
		{
			pAdapter = (this->m_dxgiAdapters.size() == 1 ? this->m_dxgiAdapters.begin()->get() : (this->m_dxgiAdapters.begin() + 1)->get());
		}

		ID3D11Device *pDevice;
		ID3D11DeviceContext *pDeviceContext;
		HRESULT hr = D3D11CreateDevice(pAdapter, driverType, nullptr, flags, featureLevels, levelsCount, D3D11_SDK_VERSION,
			&pDevice, &this->m_d3dFeatureLevel, &pDeviceContext);

		m_pD3DDevice.reset( pDevice );
		m_pD3DImmediateContext.reset(pDeviceContext);

		return (SUCCEEDED(hr) ?	Result(Success) : Result(Failure));
	}

	//------------------------------------------------------------------------
	void GraphicsSystemD3D11::ReleaseDXGIObjects()
	{
		this->m_dxgiAdapters.clear();
		this->m_pDXGIFactory = nullptr;
	}

	//------------------------------------------------------------------------
	void GraphicsSystemD3D11::ReleaseDeviceObjects()
	{
		this->m_pD3DImmediateContext = nullptr;
		this->m_pD3DDevice = nullptr;
		this->m_d3dFeatureLevel = D3D_FEATURE_LEVEL(-1);
	}


	/************************************************************************/
	/* GraphicsRenderTargetD3D11                                            */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsContextD3D11::GraphicsContextD3D11(GraphicsSystem &system) :
		GraphicsContext(system)
	{
	}

	//------------------------------------------------------------------------
	GraphicsContextD3D11::~GraphicsContextD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::Initialize()
	{
		this->m_pD3DContext = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		if (d3dDevice == nullptr)
			return Result(NotInitialized);

		ID3D11DeviceContext *pContext;
		HRESULT hr = d3dDevice->CreateDeferredContext(0, &pContext);
		m_pD3DContext.reset( pContext );
		if (FAILED(hr))
			return Result(Failure);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::Begin()
	{
		// nothing to do
		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::End()
	{
		if (this->m_pD3DContext == nullptr)
			return Result(NotInitialized);

		ID3D11CommandList *d3dCommandList;
		HRESULT hr = m_pD3DContext->FinishCommandList(FALSE, &d3dCommandList);
		if (FAILED(hr))
			return Result(Failure);

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		d3dSystem.AddCommandList(std::shared_ptr<ID3D11CommandList>(d3dCommandList));

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetRenderTarget(GraphicsRenderTarget *rt)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		ID3D11RenderTargetView *rtView[] = { rt != nullptr ?
			static_cast<GraphicsRenderTargetD3D11*>(rt)->GetRTView() :
			nullptr };
		ID3D11DepthStencilView *dsView = rt != nullptr ?
			static_cast<GraphicsRenderTargetD3D11*>(rt)->GetDSView() :
			nullptr;
		this->m_pD3DContext->OMSetRenderTargets(1, rtView, dsView);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetViewPort(const GraphicsViewPort *viewPort)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		this->m_pD3DContext->RSSetViewports(1, viewPort != nullptr ?
			&GraphicsViewPortToD3D11(*viewPort) : nullptr);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetScissorRect(const Rect *rect)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		uint32 numRects = (rect != nullptr ? 1 : 0);
		D3D11_RECT d3dRect;
		if (numRects > 0) d3dRect = RectIToD3D11(*rect);
		this->m_pD3DContext->RSSetScissorRects(numRects, (numRects > 0 ? &d3dRect : nullptr));

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::ClearTarget(
		GraphicsRenderTarget *rt,
		bool clearColor, const Vector4 &color,
		bool clearDepth, const float &depth,
		bool clearStencil, const uint32 &stencil)
	{
		GraphicsRenderTargetD3D11 *rt_d3d11 = static_cast<GraphicsRenderTargetD3D11*>(rt);
		if (rt_d3d11 == nullptr)
			return Result(Failure);

		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		if (clearColor)
		{
			m_pD3DContext->ClearRenderTargetView(rt_d3d11->GetRTView(), &color.x);
		}

		if (clearDepth || clearStencil)
		{
			UINT flags = 0 |
				(clearDepth ? D3D11_CLEAR_DEPTH : 0) |
				(clearStencil ? D3D11_CLEAR_STENCIL : 0);
			m_pD3DContext->ClearDepthStencilView(rt_d3d11->GetDSView(), flags, depth, stencil);
		}

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetPipelineState(GraphicsPipelineState *state)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		GraphicsPipelineStateD3D11 *state_d3d11 = static_cast<GraphicsPipelineStateD3D11*>(state);
		if (state_d3d11 == nullptr)
			return Result(Failure);

		if (state == nullptr)
			return Result(InvalidArgs);

		m_pD3DContext->IASetPrimitiveTopology(GraphicsPrimitiveTopologyToD3D11(state_d3d11->GetRenderState().PrimitiveTopology));

		if (state->InputLayout != nullptr)
		{
			GraphicsInputLayoutD3D11 *layout_d3d11 = static_cast<GraphicsInputLayoutD3D11*>(state->InputLayout);
			m_pD3DContext->IASetInputLayout(layout_d3d11->GetD3DInputLayout());
		}

		ID3D11SamplerState *d3dSamplers[GraphicsRenderState::MaxSamplerStates];
		for (uint32 i = 0; i < GraphicsRenderState::MaxSamplerStates; ++i)
		{
			d3dSamplers[i] = state_d3d11->GetD3DSamplerState(i);
		};

		if (state->VertexShader != nullptr)
		{
			GraphicsVertexShaderD3D11 *vs_d3d11 = static_cast<GraphicsVertexShaderD3D11*>(state->VertexShader);
			m_pD3DContext->VSSetShader(vs_d3d11->GetD3DShader(), nullptr, 0);
			m_pD3DContext->VSSetSamplers(0, GraphicsRenderState::MaxSamplerStates, d3dSamplers);
		}

		if (state->PixelShader != nullptr)
		{
			GraphicsPixelShaderD3D11 *ps_d3d11 = static_cast<GraphicsPixelShaderD3D11*>(state->PixelShader);
			m_pD3DContext->PSSetShader(ps_d3d11->GetD3DShader(), nullptr, 0);
			m_pD3DContext->PSSetSamplers(0, GraphicsRenderState::MaxSamplerStates, d3dSamplers);
		}

		m_pD3DContext->RSSetState(state_d3d11->GetD3DRasterizerState());

		m_pD3DContext->OMSetDepthStencilState(state_d3d11->GetD3DDepthStencilState(), (UINT)state_d3d11->GetRenderState().StencilRef);

		m_pD3DContext->OMSetBlendState(state_d3d11->GetD3DBlendState(), nullptr, 0xffffffff);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetTexture(GraphicsTexture *texture, uint32 slot)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		GraphicsTextureD3D11 *textureD3D11 = static_cast<GraphicsTextureD3D11*>(texture);
		uint32 numResources = (textureD3D11 != nullptr ? 1 : 0);
		ID3D11ShaderResourceView *srvs[] = { textureD3D11 != nullptr ? textureD3D11->GetSRV() : nullptr };
		m_pD3DContext->VSSetShaderResources(slot, numResources, srvs);
		m_pD3DContext->GSSetShaderResources(slot, numResources, srvs);
		m_pD3DContext->HSSetShaderResources(slot, numResources, srvs);
		m_pD3DContext->DSSetShaderResources(slot, numResources, srvs);
		m_pD3DContext->PSSetShaderResources(slot, numResources, srvs);
		m_pD3DContext->CSSetShaderResources(slot, numResources, srvs);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetConstantBuffer(GraphicsBuffer *buffer, uint32 slot)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		GraphicsBufferD3D11 *bufferD3D11 = static_cast<GraphicsBufferD3D11*>(buffer);
		uint32 numBuffers = (bufferD3D11 != nullptr ? 1 : 0);
		ID3D11Buffer* buffers[] = { bufferD3D11 != nullptr ? bufferD3D11->GetD3DBuffer() : nullptr };
		m_pD3DContext->VSSetConstantBuffers(slot, numBuffers, buffers);
		m_pD3DContext->GSSetConstantBuffers(slot, numBuffers, buffers);
		m_pD3DContext->HSSetConstantBuffers(slot, numBuffers, buffers);
		m_pD3DContext->DSSetConstantBuffers(slot, numBuffers, buffers);
		m_pD3DContext->PSSetConstantBuffers(slot, numBuffers, buffers);
		m_pD3DContext->CSSetConstantBuffers(slot, numBuffers, buffers);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetVertexBuffer(GraphicsBuffer *buffer, uint32 slot, uint32 stride, uint32 offset)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		GraphicsBufferD3D11 *bufferD3D11 = static_cast<GraphicsBufferD3D11*>(buffer);
		uint32 numBuffers = (bufferD3D11 != nullptr ? 1 : 0);
		ID3D11Buffer* buffers[] = { bufferD3D11 != nullptr ? bufferD3D11->GetD3DBuffer() : nullptr };
		m_pD3DContext->IASetVertexBuffers(slot, numBuffers, buffers, &stride, &offset);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetIndexBuffer(GraphicsBuffer *buffer, uint32 bitsPerIndex, uint32 offset)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		DXGI_FORMAT indexFmt = GraphicsBitsPerIndexToDXGIFormat(bitsPerIndex);
		if (indexFmt == DXGI_FORMAT_UNKNOWN)
			return Result(Failure);

		GraphicsBufferD3D11 *bufferD3D11 = static_cast<GraphicsBufferD3D11*>(buffer);
		m_pD3DContext->IASetIndexBuffer(
			bufferD3D11 != nullptr ? bufferD3D11->GetD3DBuffer() : nullptr,
			indexFmt, offset);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetVertexShader(GraphicsVertexShader *shader)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		GraphicsVertexShaderD3D11 *shaderD3D11 = static_cast<GraphicsVertexShaderD3D11*>(shader);
		m_pD3DContext->VSSetShader(
			shaderD3D11 != nullptr ? shaderD3D11->GetD3DShader() : nullptr,
			nullptr, 0);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::SetPixelShader(GraphicsPixelShader *shader)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		GraphicsPixelShaderD3D11 *shaderD3D11 = static_cast<GraphicsPixelShaderD3D11*>(shader);
		m_pD3DContext->PSSetShader(
			shaderD3D11 != nullptr ? shaderD3D11->GetD3DShader() : nullptr,
			nullptr, 0);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::Draw(uint32 vertexCount, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		if (instanceCount > 0)
		{
			m_pD3DContext->DrawInstanced((UINT)vertexCount, (UINT)instanceCount, (UINT)vertexOffset, (UINT)instanceOffset);
		}
		else
		{
			m_pD3DContext->Draw((UINT)vertexCount, (UINT)vertexOffset);
		}

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::DrawIndexed(uint32 indexCount, uint32 indexOffset, uint32 vertexOffset, uint32 instanceCount, uint32 instanceOffset)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		if (instanceCount > 0)
		{
			m_pD3DContext->DrawIndexedInstanced((UINT)indexCount, (UINT)instanceCount, (UINT)indexOffset, (UINT)vertexOffset, (UINT)instanceOffset);
		}
		else
		{
			m_pD3DContext->DrawIndexed((UINT)indexCount, (UINT)indexOffset, (UINT)vertexOffset);
		}

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsContextD3D11::UpdateBuffer(GraphicsBuffer *buffer, GraphicsGPUAccess gpuAccess, bool doNotWait, UpdateBufferCallback callback)
	{
		if (this->m_pD3DContext == nullptr)
			return Result(Failure);

		GraphicsBufferD3D11 *bufferD3D11 = static_cast<GraphicsBufferD3D11*>(buffer);
		if (bufferD3D11 == nullptr || bufferD3D11->GetD3DBuffer() == nullptr)
			return Result(InvalidArgs);

		D3D11_MAP d3dMapType = GraphicsGPUAccessFlagToD3D11(gpuAccess);
		UINT d3dMapFlags = (doNotWait ? D3D11_MAP_FLAG_DO_NOT_WAIT : 0);
		D3D11_MAPPED_SUBRESOURCE d3dMappedRes;
		HRESULT hr = m_pD3DContext->Map(bufferD3D11->GetD3DBuffer(), 0, d3dMapType, d3dMapFlags, &d3dMappedRes);
		if (FAILED(hr))
			return Result(Failure);

		GraphicsResourceData GraphicsData;
		GraphicsData.Ptr = d3dMappedRes.pData;
		GraphicsData.RowPitch = (uint32)d3dMappedRes.RowPitch;
		GraphicsData.SlicePitch = (uint32)d3dMappedRes.DepthPitch;

		Result res = callback(&GraphicsData);

		m_pD3DContext->Unmap(bufferD3D11->GetD3DBuffer(), 0);

		return res;
	}


	/************************************************************************/
	/* GraphicsTextureD3D11                                                 */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsTextureD3D11::GraphicsTextureD3D11(GraphicsSystem &system) :
		GraphicsTexture(system)
	{
	}

	//------------------------------------------------------------------------
	GraphicsTextureD3D11::~GraphicsTextureD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsTextureD3D11::Initialize(const GraphicsTextureDesc &desc, std::shared_ptr<ID3D11Texture2D> &d3dTexture)
	{
		d3dTexture = nullptr;
		m_pD3DSRV = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		if (d3dDevice == nullptr)
			return Result(NotInitialized);

		m_pD3DTexture = nullptr;

		if (desc.BindFlags & uint32(GraphicsBindFlag::ShaderResource))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC d3dSrvDesc;
			d3dSrvDesc.Format = GraphicsFormatToDXGI(this->GetDesc().Format, this->GetDesc().FormatView);
			d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			d3dSrvDesc.Texture2D.MipLevels = -1;
			d3dSrvDesc.Texture2D.MostDetailedMip = 0;
			ID3D11ShaderResourceView *pSRV = nullptr;
			HRESULT hr = d3dDevice->CreateShaderResourceView(m_pD3DTexture.get(), &d3dSrvDesc, &pSRV);
			if (FAILED(hr))
			{
				m_pD3DTexture = nullptr;
				return Result(Failure);
			}
			else
			{
				m_pD3DSRV.reset( pSRV );
			}
		}

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsTextureD3D11::OnInitialize(const GraphicsResourceData *data)
	{
		m_pD3DTexture = nullptr;
		m_pD3DSRV = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		if (d3dDevice == nullptr)
			return Result(NotInitialized);

		D3D11_SUBRESOURCE_DATA d3dData;
		D3D11_SUBRESOURCE_DATA *d3dDataPtr = nullptr;
		if (data != nullptr)
		{
			d3dData.pSysMem = data->Ptr;
			d3dData.SysMemPitch = data->RowPitch;
			d3dData.SysMemSlicePitch = data->SlicePitch;
			d3dDataPtr = &d3dData;
		}

		D3D11_TEXTURE2D_DESC d3dDesc = GraphicsTextureDescToD3D11(this->GetDesc());
		d3dDesc.Format = GraphicsFormatToDXGI(this->GetDesc().Format, GraphicsFormatView::Typeless);
		ID3D11Texture2D *pTexture = nullptr;
		HRESULT hr = d3dDevice->CreateTexture2D(&d3dDesc, d3dDataPtr, &pTexture);
		if (FAILED(hr))
		{
			return Result(Failure);
		}
		else
		{
			m_pD3DTexture.reset( pTexture );
		}

		if (this->GetDesc().BindFlags & uint32(GraphicsBindFlag::ShaderResource))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC d3dSrvDesc;
			d3dSrvDesc.Format = GraphicsFormatToDXGI(this->GetDesc().Format, this->GetDesc().FormatView);
			d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			d3dSrvDesc.Texture2D.MipLevels = -1;
			d3dSrvDesc.Texture2D.MostDetailedMip = 0;
			ID3D11ShaderResourceView *pSRV = nullptr;
			hr = d3dDevice->CreateShaderResourceView(m_pD3DTexture.get(), &d3dSrvDesc, &pSRV);
			if (FAILED(hr))
			{
				m_pD3DTexture = nullptr;
				return Result(Failure);
			}
			else
			{
				m_pD3DSRV.reset(pSRV);
			}
		}

		return Result(Success);
	}


	/************************************************************************/
	/* GraphicsRenderTargetD3D11                                            */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsRenderTargetD3D11::GraphicsRenderTargetD3D11(GraphicsSystem &system) :
		GraphicsRenderTarget(system)
	{
	}

	//------------------------------------------------------------------------
	GraphicsRenderTargetD3D11::~GraphicsRenderTargetD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsRenderTargetD3D11::OnInitialize()
	{
		m_pD3DRTView = nullptr;
		m_pD3DDSView = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();

		GraphicsTextureD3D11 *d3dTargetBuffer = static_cast<GraphicsTextureD3D11*>(this->GetTragetBuffer());
		if (d3dTargetBuffer != nullptr)
		{
			const GraphicsTextureDesc &bufferDesc = this->GetTragetBuffer()->GetDesc();
			D3D11_RENDER_TARGET_VIEW_DESC d3dRTViewDesc;
			d3dRTViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			d3dRTViewDesc.Format = GraphicsFormatToDXGI(bufferDesc.Format, bufferDesc.FormatView);
			d3dRTViewDesc.Texture2D.MipSlice = 0;

			ID3D11RenderTargetView *pRTView = nullptr;
			HRESULT hr = d3dDevice->CreateRenderTargetView(d3dTargetBuffer->GetD3DTexture(), &d3dRTViewDesc, &pRTView);
			if (FAILED(hr))
			{
				return Result(Failure);
			}
			else
			{
				m_pD3DRTView.reset(pRTView);
			}
		}

		GraphicsTextureD3D11 *d3dDepthStencilBuffer = static_cast<GraphicsTextureD3D11*>(this->GetDepthStencilBuffer());
		if (d3dDepthStencilBuffer != nullptr)
		{
			const GraphicsTextureDesc &bufferDesc = this->GetDepthStencilBuffer()->GetDesc();
			D3D11_DEPTH_STENCIL_VIEW_DESC d3dDSViewDesc;
			d3dDSViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			switch (bufferDesc.Format)
			{
			case GraphicsFormat::R32G8X24:
				d3dDSViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
				break;
			case GraphicsFormat::R32:
				d3dDSViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
				break;
			case GraphicsFormat::R24G8:
				d3dDSViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				break;
			case GraphicsFormat::R16:
				d3dDSViewDesc.Format = DXGI_FORMAT_D16_UNORM;
				break;
			default:
				d3dDSViewDesc.Format = DXGI_FORMAT_UNKNOWN;
			}
			d3dDSViewDesc.Flags = 0;
			d3dDSViewDesc.Texture2D.MipSlice = 0;

			ID3D11DepthStencilView *pDSView = nullptr;
			HRESULT hr = d3dDevice->CreateDepthStencilView(d3dDepthStencilBuffer->GetD3DTexture(), &d3dDSViewDesc, &pDSView);
			if (FAILED(hr))
			{
				return Result(Failure);
			}
			else
			{
				m_pD3DDSView.reset(pDSView);
			}
		}

		return Result(Success);
	}


	/************************************************************************/
	/* GraphicsSwapChainD3D11                                               */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsSwapChainD3D11::GraphicsSwapChainD3D11(GraphicsSystem &system) :
		GraphicsSwapChain(system)
	{
		memset(&this->dxgiChainDesc, 0, sizeof(this->dxgiChainDesc));
	}

	//------------------------------------------------------------------------
	GraphicsSwapChainD3D11::~GraphicsSwapChainD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsSwapChainD3D11::Initialize(const GraphicsPresentParams &params)
	{
		m_pTargetBuffer = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		IDXGIFactory1 *dxgiFactory = d3dSystem.GetDXGIFactory();
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		auto *window = d3dSystem.GetWinWindow();
		if (window == nullptr)
			return Result(NotInitialized);

		this->dxgiChainDesc.OutputWindow = window->getHwnd();
		this->dxgiChainDesc.Windowed = !params.Fullscreen;
		this->dxgiChainDesc.BufferDesc.Width = params.BufferWidth;
		this->dxgiChainDesc.BufferDesc.Height = params.BufferHeight;
		this->dxgiChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		this->dxgiChainDesc.BufferCount = params.BufferCount;
		this->dxgiChainDesc.BufferDesc.Format = GraphicsFormatToDXGI(params.BufferFormat, GraphicsFormatView::Unorm);
		this->dxgiChainDesc.BufferDesc.RefreshRate.Numerator = params.FullscreenRefreshRate;
		this->dxgiChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		this->dxgiChainDesc.SampleDesc.Count = params.MutisampleCount;
		this->dxgiChainDesc.SampleDesc.Quality = params.MutisampleQuality;
		this->dxgiChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain *pSwapChain = nullptr;
		HRESULT hr = dxgiFactory->CreateSwapChain(d3dDevice, &this->dxgiChainDesc, &pSwapChain);
		if (FAILED(hr))
		{
			return Result(Failure);
		}
		else
		{
			m_pDXGISwapChain.reset(pSwapChain);
		}

		ID3D11Texture2D *d3dTargetBuffer = nullptr;
		hr = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)d3dTargetBuffer);
		if (FAILED(hr))
		{
			return Result(Failure);
		}

		GraphicsTextureDesc desc;
		desc.Width = params.BufferWidth;
		desc.Height = params.BufferHeight;
		desc.Levels = 1;
		desc.Format = params.BufferFormat;
		desc.FormatView = GraphicsFormatView::Unorm;
		desc.Usage = GraphicsUsage::Default;
		desc.BindFlags = uint32(GraphicsBindFlag::RenderTarget);
		desc.AccessFlags = uint32(0);

		std::unique_ptr<GraphicsSwapChainD3D11::RenderTarget> newRenderTarget(new RenderTarget(d3dSystem, std::shared_ptr<ID3D11Texture2D>(d3dTargetBuffer)));
		Result res = newRenderTarget->Initialize(desc, params.DepthStencilEnabled, params.DepthStencilFormat);
		if (Failed(res))
			return Result(res.code());

		m_pTargetBuffer = std::move(newRenderTarget);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	Result GraphicsSwapChainD3D11::Present()
	{
		if (m_pDXGISwapChain != nullptr)
			return Result(NotInitialized);

		if (FAILED(m_pDXGISwapChain->Present(0, 0)))
			return Result(Failure);

		return Result(Success);
	}

	//------------------------------------------------------------------------
	GraphicsRenderTarget* GraphicsSwapChainD3D11::GetTargetBuffer()
	{
		return m_pTargetBuffer.get();
	}

	//------------------------------------------------------------------------
	GraphicsSwapChainD3D11::RenderTarget::RenderTarget(GraphicsSystem &GraphicsSystem, std::shared_ptr<ID3D11Texture2D> &dxgiSwapChainBuffer) :
		GraphicsRenderTargetD3D11(GraphicsSystem),
		m_pDXGISwapChainBuffer(dxgiSwapChainBuffer)
	{
	}

	//------------------------------------------------------------------------
	GraphicsSwapChainD3D11::RenderTarget::~RenderTarget()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsSwapChainD3D11::RenderTarget::InitializeTargetBuffer(std::unique_ptr<GraphicsTexture> &resultBuffer, const GraphicsTextureDesc &desc)
	{
		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());

		std::unique_ptr<GraphicsTextureD3D11> newTargetBuffer(new GraphicsTextureD3D11(d3dSystem));
		Result res = newTargetBuffer->Initialize(desc, m_pDXGISwapChainBuffer);
		if (Failed(res))
			return Result(res.code());

		resultBuffer = std::move(newTargetBuffer);

		return Result(Success);
	}


	/************************************************************************/
	/* GraphicsBufferD3D11                                                  */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsBufferD3D11::GraphicsBufferD3D11(GraphicsSystem &system) :
		GraphicsBuffer(system)
	{
	}

	//------------------------------------------------------------------------
	GraphicsBufferD3D11::~GraphicsBufferD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsBufferD3D11::OnInitialize(const GraphicsResourceData *data)
	{
		m_pD3DBuffer = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		if (d3dDevice == nullptr)
			return Result(NotInitialized);

		D3D11_SUBRESOURCE_DATA d3dData;
		D3D11_SUBRESOURCE_DATA *d3dDataPtr = nullptr;
		if (data != nullptr)
		{
			d3dData.pSysMem = data->Ptr;
			d3dData.SysMemPitch = data->RowPitch;
			d3dData.SysMemSlicePitch = data->SlicePitch;
			d3dDataPtr = &d3dData;
		}

		ID3D11Buffer *pBuffer = nullptr;
		D3D11_BUFFER_DESC d3dDesc = GraphicsBufferDescToD3D11(this->GetDesc());
		HRESULT hr = d3dDevice->CreateBuffer(&d3dDesc, d3dDataPtr, &pBuffer);
		if (FAILED(hr))
		{
			return Result(Failure);
		}
		else
		{
			m_pD3DBuffer.reset(pBuffer);
		}

		return Result(Success);
	}


	/************************************************************************/
	/* GraphicsVertexShaderD3D11                                            */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsVertexShaderD3D11::GraphicsVertexShaderD3D11(GraphicsSystem &system) :
		GraphicsVertexShader(system)
	{
	}

	//------------------------------------------------------------------------
	GraphicsVertexShaderD3D11::~GraphicsVertexShaderD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsVertexShaderD3D11::OnInitialize()
	{
		m_pD3DVertexShader = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		if (d3dDevice == nullptr)
			return Result(NotInitialized);

		ID3D11VertexShader *pVertexShader = nullptr;
		HRESULT hr = d3dDevice->CreateVertexShader(this->GetByteCode(), this->GetSizeInBytes(), nullptr, &pVertexShader);
		if (FAILED(hr))
		{
			return Result(Failure);
		}
		else
		{
			m_pD3DVertexShader.reset(pVertexShader);
		}

		return Result(Success);
	}


	/************************************************************************/
	/* GraphicsPixelShaderD3D11                                             */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsPixelShaderD3D11::GraphicsPixelShaderD3D11(GraphicsSystem &system) :
		GraphicsPixelShader(system)
	{
	}

	//------------------------------------------------------------------------
	GraphicsPixelShaderD3D11::~GraphicsPixelShaderD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsPixelShaderD3D11::OnInitialize()
	{
		m_pD3DPixelShader = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		if (d3dDevice == nullptr)
			return Result(NotInitialized);

		ID3D11PixelShader *pPixelShader = nullptr;
		HRESULT hr = d3dDevice->CreatePixelShader(this->GetByteCode(), this->GetSizeInBytes(), nullptr, &pPixelShader);
		if (FAILED(hr))
		{
			return Result(Failure);
		}
		else
		{
			m_pD3DPixelShader.reset( pPixelShader );
		}

		return Result(Success);
	}


	/************************************************************************/
	/* GraphicsInputLayoutD3D11                                             */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsInputLayoutD3D11::GraphicsInputLayoutD3D11(GraphicsSystem &system) :
		GraphicsInputLayout(system)
	{
	}

	//------------------------------------------------------------------------
	GraphicsInputLayoutD3D11::~GraphicsInputLayoutD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsInputLayoutD3D11::OnInitialize(const GraphicsShader &shader, const GraphicsInputElement *elements, uint32 count)
	{
		m_pD3DInputLayout = nullptr;

		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		if (d3dDevice == nullptr)
			return Result(NotInitialized);

		std::vector<D3D11_INPUT_ELEMENT_DESC> d3dElements(count);
		const GraphicsInputElement *GraphicsElement = elements;
		for (auto &d3dElement : d3dElements)
		{
			d3dElement = GraphicsInputElementToD3D11(*GraphicsElement++);
		}

		ID3D11InputLayout *pInputLayout = nullptr;
		HRESULT hr = d3dDevice->CreateInputLayout(d3dElements.data(), count, shader.GetByteCode(), shader.GetSizeInBytes(), &pInputLayout);
		if (FAILED(hr))
		{
			return Result(Failure);
		}
		else
		{
			m_pD3DInputLayout.reset(pInputLayout);
		}

		return Result(Success);
	}


	/************************************************************************/
	/* GraphicsPipelineStateD3D11                                           */
	/************************************************************************/
	//------------------------------------------------------------------------
	GraphicsPipelineStateD3D11::GraphicsPipelineStateD3D11(GraphicsSystem &system) :
		GraphicsPipelineState(system)
	{
		memset(m_hashSamplerState, 0, sizeof(m_hashSamplerState));
		m_hashRasterizerState = 0;
		m_hashDepthStencilState = 0;
		m_hashBlendState = 0;
	}

	//------------------------------------------------------------------------
	GraphicsPipelineStateD3D11::~GraphicsPipelineStateD3D11()
	{
	}

	//------------------------------------------------------------------------
	Result GraphicsPipelineStateD3D11::OnSetRenderState(const GraphicsRenderState &renderState)
	{
		GraphicsSystemD3D11 &d3dSystem = static_cast<GraphicsSystemD3D11&>(this->GetGraphicsSystem());
		ID3D11Device *d3dDevice = d3dSystem.GetDevice();
		if (d3dDevice == nullptr)
			return Result(NotInitialized);

		// SamplerState
		for (uint32 i = 0; i < GraphicsRenderState::MaxSamplerStates; ++i)
		{
			size_t hash = ComputeHash((const cbyte*)&renderState.SamplerState[i], sizeof(renderState.SamplerState[i]));
			if (m_hashSamplerState[i] != hash)
			{
				m_hashSamplerState[i] = hash;
				m_pD3DSamplerState[i] = nullptr;
				D3D11_SAMPLER_DESC desc = GraphicsSamplerStateToD3D11(renderState.SamplerState[i]);

				ID3D11SamplerState *pDamplerState = nullptr;
				HRESULT hr = d3dDevice->CreateSamplerState(&desc, &pDamplerState);
				if (FAILED(hr))
				{
					return Result(Failure);
				}
				else
				{
					m_pD3DSamplerState[i].reset(pDamplerState);
				}
			}
		}

		// RasterizerState
		size_t hash = ComputeHash((const cbyte*)&renderState.RasterizerState, sizeof(renderState.RasterizerState));
		if (m_hashRasterizerState != hash)
		{
			m_hashRasterizerState = hash;
			m_pD3DRasterizerState = nullptr;
			D3D11_RASTERIZER_DESC desc = GraphicsRasterizerStateToD3D11(renderState.RasterizerState);

			ID3D11RasterizerState *pRasterizerState = nullptr;
			HRESULT hr = d3dDevice->CreateRasterizerState(&desc, &pRasterizerState);
			if (FAILED(hr))
			{
				return Result(Failure);
			}
			else
			{
				m_pD3DRasterizerState.reset(pRasterizerState);
			}
		}

		// DepthStencilState
		hash = ComputeHash((const cbyte*)&renderState.DepthStencilState, sizeof(renderState.DepthStencilState));
		if (m_hashDepthStencilState != hash)
		{
			m_hashDepthStencilState = hash;
			m_pD3DDepthStencilState = nullptr;
			D3D11_DEPTH_STENCIL_DESC desc = GraphicsDepthStencilStateToD3D11(renderState.DepthStencilState);

			ID3D11DepthStencilState *pDepthState = nullptr;
			HRESULT hr = d3dDevice->CreateDepthStencilState(&desc, &pDepthState);
			if (FAILED(hr))
			{
				return Result(Failure);
			}
			else
			{
				m_pD3DDepthStencilState.reset(pDepthState);
			}
		}

		// BlendState
		hash = ComputeHash((const cbyte*)&renderState.BlendState, sizeof(renderState.BlendState));
		if (m_hashBlendState != hash)
		{
			m_hashBlendState = hash;
			m_pD3DBlendState = nullptr;
			D3D11_BLEND_DESC desc;
			desc.AlphaToCoverageEnable = false;
			desc.IndependentBlendEnable = true;
			for (uint8 irt = 0; irt < GraphicsRenderState::MaxRenderTargets; ++irt)
			{
				desc.RenderTarget[irt] = GraphicsBlendStateToD3D11(renderState.BlendState[irt]);
			}

			ID3D11BlendState *pBlendState = nullptr;
			HRESULT hr = d3dDevice->CreateBlendState(&desc, &pBlendState);
			if (FAILED(hr))
			{
				return Result(Failure);
			}
			else
			{
				m_pD3DBlendState.reset(pBlendState);
			}
		}

		return Result(Success);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Utilities
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//------------------------------------------------------------------------
	static DXGI_FORMAT GraphicsFormatToDXGI_LUT[uint32(GraphicsFormat::Count)][uint32(GraphicsFormatView::Count)] = {
		// Typeless, Unorm, Snorm, Uint, Sint, Float,
		// Unknown
		{ DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// R32G32B32A32
		{ DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R32G32B32A32_UINT, DXGI_FORMAT_R32G32B32A32_SINT, DXGI_FORMAT_R32G32B32A32_FLOAT },
		// R32G32B32
		{ DXGI_FORMAT_R32G32B32_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R32G32B32_UINT, DXGI_FORMAT_R32G32B32_SINT, DXGI_FORMAT_R32G32B32_FLOAT },
		// R16G16B16A16
		{ DXGI_FORMAT_R16G16B16A16_TYPELESS, DXGI_FORMAT_R16G16B16A16_UNORM, DXGI_FORMAT_R16G16B16A16_SNORM, DXGI_FORMAT_R16G16B16A16_UINT, DXGI_FORMAT_R16G16B16A16_SINT, DXGI_FORMAT_R16G16B16A16_FLOAT },
		// R32G32
		{ DXGI_FORMAT_R32G32_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R32G32_UINT, DXGI_FORMAT_R32G32_SINT, DXGI_FORMAT_R32G32_FLOAT },
		// R32G8X24
		{ DXGI_FORMAT_R32G8X24_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS },
		// R8G8B8A8
		{ DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_SNORM, DXGI_FORMAT_R8G8B8A8_UINT, DXGI_FORMAT_R8G8B8A8_SINT, DXGI_FORMAT_UNKNOWN },
		// B8G8R8A8
		{ DXGI_FORMAT_B8G8R8A8_TYPELESS, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// B8G8R8X8
		{ DXGI_FORMAT_B8G8R8X8_TYPELESS, DXGI_FORMAT_B8G8R8X8_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// R16G16
		{ DXGI_FORMAT_R16G16_TYPELESS, DXGI_FORMAT_R16G16_UNORM, DXGI_FORMAT_R16G16_SNORM, DXGI_FORMAT_R16G16_UINT, DXGI_FORMAT_R16G16_SINT, DXGI_FORMAT_R16G16_FLOAT },
		// R32
		{ DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_R32_UINT, DXGI_FORMAT_R32_SINT, DXGI_FORMAT_R32_FLOAT },
		// R24G8
		{ DXGI_FORMAT_R24G8_TYPELESS, DXGI_FORMAT_R24_UNORM_X8_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// R8G8
		{ DXGI_FORMAT_R8G8_TYPELESS, DXGI_FORMAT_R8G8_UNORM, DXGI_FORMAT_R8G8_SNORM, DXGI_FORMAT_R8G8_UINT, DXGI_FORMAT_R8G8_SINT, DXGI_FORMAT_UNKNOWN },
		// B5G6R5
		{ DXGI_FORMAT_B5G6R5_UNORM, DXGI_FORMAT_B5G6R5_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// B5G5R5A1
		{ DXGI_FORMAT_B5G5R5A1_UNORM, DXGI_FORMAT_B5G5R5A1_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// R16
		{ DXGI_FORMAT_R16_TYPELESS, DXGI_FORMAT_R16_UNORM, DXGI_FORMAT_R16_SNORM, DXGI_FORMAT_R16_UINT, DXGI_FORMAT_R16_SINT, DXGI_FORMAT_R16_FLOAT },
		// R8
		{ DXGI_FORMAT_R8_TYPELESS, DXGI_FORMAT_R8_UNORM, DXGI_FORMAT_R8_SNORM, DXGI_FORMAT_R8_UINT, DXGI_FORMAT_R8_SINT, DXGI_FORMAT_UNKNOWN },
		// A8
		{ DXGI_FORMAT_A8_UNORM, DXGI_FORMAT_A8_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// R1
		{ DXGI_FORMAT_R1_UNORM, DXGI_FORMAT_R1_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// BC1
		{ DXGI_FORMAT_BC1_TYPELESS, DXGI_FORMAT_BC1_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// BC2
		{ DXGI_FORMAT_BC2_TYPELESS, DXGI_FORMAT_BC2_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// BC3
		{ DXGI_FORMAT_BC3_TYPELESS, DXGI_FORMAT_BC3_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// BC4
		{ DXGI_FORMAT_BC4_TYPELESS, DXGI_FORMAT_BC4_UNORM, DXGI_FORMAT_BC4_SNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// BC5
		{ DXGI_FORMAT_BC5_TYPELESS, DXGI_FORMAT_BC5_UNORM, DXGI_FORMAT_BC5_SNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN },
		// BC6H
		{ DXGI_FORMAT_BC6H_TYPELESS, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_BC6H_UF16 },
		// BC7
		{ DXGI_FORMAT_BC7_TYPELESS, DXGI_FORMAT_BC7_UNORM, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN, DXGI_FORMAT_UNKNOWN }
	};

	//------------------------------------------------------------------------
	DXGI_FORMAT GraphicsFormatToDXGI(GraphicsFormat fmt, GraphicsFormatView view)
	{
		return GraphicsFormatToDXGI_LUT[uint32(fmt)][uint32(view)];
	}

	//------------------------------------------------------------------------
	DXGI_FORMAT GraphicsBitsPerIndexToDXGIFormat(uint32 bitsPerIndex)
	{
		switch (bitsPerIndex)
		{
		case 16: return DXGI_FORMAT_R16_UINT;
		case 32: return DXGI_FORMAT_R32_UINT;
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	//------------------------------------------------------------------------
	D3D11_USAGE GraphicsUsageToD3D11(GraphicsUsage usage)
	{
		return (D3D11_USAGE)usage;
	}

	//------------------------------------------------------------------------
	D3D11_BIND_FLAG GraphicsBindFlagToD3D11(GraphicsBindFlag flag)
	{
		return (D3D11_BIND_FLAG)flag;
	}

	//------------------------------------------------------------------------
	UINT GraphicsBindFlagsToD3D11(uint32 flags)
	{
		return (UINT)flags;
	}

	//------------------------------------------------------------------------
	D3D11_CPU_ACCESS_FLAG GraphicsAccessFlagToD3D11_CPUAccessFlag(GraphicsAccessFlag flag)
	{
		switch (flag)
		{
		case GraphicsAccessFlag::Read: return D3D11_CPU_ACCESS_READ;
		case GraphicsAccessFlag::Write: return D3D11_CPU_ACCESS_WRITE;
		}
		return (D3D11_CPU_ACCESS_FLAG)0;
	}

	//------------------------------------------------------------------------
	UINT GraphicsAccessFlagsToD3D11_CPUAccessFlags(uint32 flags)
	{
		UINT d3dFlags = 0;
		if (flags & (uint32)GraphicsAccessFlag::Write) d3dFlags |= D3D11_CPU_ACCESS_WRITE;
		if (flags & (uint32)GraphicsAccessFlag::Read) d3dFlags |= D3D11_CPU_ACCESS_READ;
		return (UINT)d3dFlags;
	}

	//------------------------------------------------------------------------
	D3D11_MAP GraphicsGPUAccessFlagToD3D11(GraphicsGPUAccess gpuAccess)
	{
		switch (gpuAccess)
		{
		case GraphicsGPUAccess::Read: return D3D11_MAP_READ;
		case GraphicsGPUAccess::Write: return D3D11_MAP_WRITE;
		case GraphicsGPUAccess::ReadWrite: return D3D11_MAP_READ_WRITE;
		case GraphicsGPUAccess::WriteDiscard: return D3D11_MAP_WRITE_DISCARD;
		case GraphicsGPUAccess::WriteNoOverwrite: return D3D11_MAP_WRITE_NO_OVERWRITE;
		}
		return (D3D11_MAP)0;
	}

	//------------------------------------------------------------------------
	D3D11_VIEWPORT GraphicsViewPortToD3D11(const GraphicsViewPort &viewPort)
	{
		D3D11_VIEWPORT d3dViewPort;
		d3dViewPort.TopLeftX = viewPort.X;
		d3dViewPort.TopLeftY = viewPort.Y;
		d3dViewPort.Width = viewPort.Width;
		d3dViewPort.Height = viewPort.Height;
		d3dViewPort.MinDepth = viewPort.DepthMin;
		d3dViewPort.MaxDepth = viewPort.DepthMax;
		return d3dViewPort;
	}

	//------------------------------------------------------------------------
	D3D11_RECT RectIToD3D11(const Rect &rect)
	{
		D3D11_RECT d3dRect;
		d3dRect.left = (LONG)rect.Min.x;
		d3dRect.right = (LONG)rect.Max.x;
		d3dRect.top = (LONG)rect.Min.y;
		d3dRect.bottom = (LONG)rect.Max.y;
		return d3dRect;
	}

	//------------------------------------------------------------------------
	D3D11_TEXTURE2D_DESC GraphicsTextureDescToD3D11(const GraphicsTextureDesc &desc)
	{
		D3D11_TEXTURE2D_DESC d3dDesc;
		d3dDesc.Width = desc.Width;
		d3dDesc.Height = desc.Height;
		d3dDesc.MipLevels = desc.Levels;
		d3dDesc.ArraySize = 1;
		d3dDesc.Format = GraphicsFormatToDXGI(desc.Format, desc.FormatView);
		d3dDesc.SampleDesc.Count = 1;
		d3dDesc.SampleDesc.Quality = 0;
		d3dDesc.Usage = GraphicsUsageToD3D11(desc.Usage);
		d3dDesc.BindFlags = GraphicsBindFlagsToD3D11(desc.BindFlags);
		d3dDesc.CPUAccessFlags = GraphicsAccessFlagsToD3D11_CPUAccessFlags(desc.AccessFlags);
		d3dDesc.MiscFlags = 0;
		return d3dDesc;
	}

	//------------------------------------------------------------------------
	D3D11_BUFFER_DESC GraphicsBufferDescToD3D11(const GraphicsBufferDesc &desc)
	{
		D3D11_BUFFER_DESC d3dDesc;
		d3dDesc.ByteWidth = desc.Size;
		d3dDesc.Usage = GraphicsUsageToD3D11(desc.Usage);
		d3dDesc.BindFlags = GraphicsBindFlagsToD3D11(desc.BindFlags);
		d3dDesc.CPUAccessFlags = GraphicsAccessFlagsToD3D11_CPUAccessFlags(desc.AccessFlags);
		d3dDesc.MiscFlags = 0;
		d3dDesc.StructureByteStride = desc.StructureStride;
		return d3dDesc;
	}

	//------------------------------------------------------------------------
	LPCSTR GraphicsSemanticToD3D11(GraphicsSemantic semantic)
	{
		switch (semantic)
		{
		case GraphicsSemantic::Position: return "POSITION";
		case GraphicsSemantic::TexCoord: return "TEXCOORD";
		case GraphicsSemantic::Color: return "COLOR";
		case GraphicsSemantic::Tangtent: return "TANGENT";
		case GraphicsSemantic::Normal: return "NORMAL";
		case GraphicsSemantic::Binormal: return "BINORMAL";
		}
		return "UNKNOWN";
	}

	//------------------------------------------------------------------------
	D3D11_INPUT_ELEMENT_DESC GraphicsInputElementToD3D11(const GraphicsInputElement &element)
	{
		D3D11_INPUT_ELEMENT_DESC d3dElement;
		d3dElement.SemanticName = GraphicsSemanticToD3D11(element.Semantic);
		d3dElement.SemanticIndex = element.SemanticIdx;
		d3dElement.Format = GraphicsFormatToDXGI(element.Format, element.FormatView);
		d3dElement.InputSlot = element.SlotIdx;
		d3dElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		d3dElement.InputSlotClass = (element.InstanceStepRate > 0 ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA);
		d3dElement.InstanceDataStepRate = element.InstanceStepRate;
		return d3dElement;
	}

	//------------------------------------------------------------------------
	D3D11_PRIMITIVE_TOPOLOGY GraphicsPrimitiveTopologyToD3D11(GraphicsPrimitiveTopology topology)
	{
		switch (topology)
		{
		case GraphicsPrimitiveTopology::PointList: return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		case GraphicsPrimitiveTopology::LineList: return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		case GraphicsPrimitiveTopology::LineStrip: return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case GraphicsPrimitiveTopology::TriangleList: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case GraphicsPrimitiveTopology::TriangleStrip: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		}
		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}

	//------------------------------------------------------------------------
	D3D11_RENDER_TARGET_BLEND_DESC GraphicsBlendStateToD3D11(const GraphicsBlendState &state)
	{
		D3D11_RENDER_TARGET_BLEND_DESC desc;
		desc.BlendEnable = state.BlendEnable;
		desc.SrcBlend = GraphicsBlendFactorToD3D11(state.SrcBlend);
		desc.DestBlend = GraphicsBlendFactorToD3D11(state.DstBlend);
		desc.BlendOp = GraphicsBlendOpToD3D11(state.BlendOp);
		desc.SrcBlendAlpha = GraphicsBlendFactorToD3D11(state.SrcBlendAlpha);
		desc.DestBlendAlpha = GraphicsBlendFactorToD3D11(state.DstBlendAlpha);
		desc.BlendOpAlpha = GraphicsBlendOpToD3D11(state.BlendOpAlpha);
		desc.RenderTargetWriteMask = state.RenderTargetWriteMask;
		return desc;
	}

	//------------------------------------------------------------------------
	D3D11_BLEND GraphicsBlendFactorToD3D11(GraphicsBlendFactor blendFactor)
	{
		switch (blendFactor)
		{
		case GraphicsBlendFactor::Zero: return D3D11_BLEND_ZERO;
		case GraphicsBlendFactor::One: return D3D11_BLEND_ONE;
		case GraphicsBlendFactor::SrcColor: return D3D11_BLEND_SRC_COLOR;
		case GraphicsBlendFactor::InvSrcColor: return D3D11_BLEND_INV_SRC_COLOR;
		case GraphicsBlendFactor::SrcAlpha: return D3D11_BLEND_SRC_ALPHA;
		case GraphicsBlendFactor::InvSrcAlpha: return D3D11_BLEND_INV_SRC_ALPHA;
		case GraphicsBlendFactor::DstAlpha: return D3D11_BLEND_DEST_ALPHA;
		case GraphicsBlendFactor::InvDstAlpha: return D3D11_BLEND_INV_DEST_ALPHA;
		case GraphicsBlendFactor::DstColor: return D3D11_BLEND_DEST_COLOR;
		case GraphicsBlendFactor::InvDstColor: return D3D11_BLEND_INV_DEST_COLOR;
		}
		return D3D11_BLEND(0);
	}

	//------------------------------------------------------------------------
	D3D11_BLEND_OP GraphicsBlendOpToD3D11(GraphicsBlendOp blendOp)
	{
		switch (blendOp)
		{
		case GraphicsBlendOp::Add: return D3D11_BLEND_OP_ADD;
		case GraphicsBlendOp::Subtract: return D3D11_BLEND_OP_SUBTRACT;
		case GraphicsBlendOp::RevSubtract: return D3D11_BLEND_OP_REV_SUBTRACT;
		case GraphicsBlendOp::Min: return D3D11_BLEND_OP_MIN;
		case GraphicsBlendOp::Max: return D3D11_BLEND_OP_MAX;
		}
		return D3D11_BLEND_OP(0);
	}

	//------------------------------------------------------------------------
	D3D11_COMPARISON_FUNC GraphicsCmpFuncToD3D11(GraphicsCmpFunc func)
	{
		switch (func)
		{
		case GraphicsCmpFunc::Never: return D3D11_COMPARISON_NEVER;
		case GraphicsCmpFunc::Less: return D3D11_COMPARISON_LESS;
		case GraphicsCmpFunc::Equal: return D3D11_COMPARISON_EQUAL;
		case GraphicsCmpFunc::LessEqual: return D3D11_COMPARISON_LESS_EQUAL;
		case GraphicsCmpFunc::Greater: return D3D11_COMPARISON_GREATER;
		case GraphicsCmpFunc::NotEqual: return D3D11_COMPARISON_NOT_EQUAL;
		case GraphicsCmpFunc::GreaterEqual: return D3D11_COMPARISON_GREATER_EQUAL;
		case GraphicsCmpFunc::Always: return D3D11_COMPARISON_ALWAYS;
		}
		return (D3D11_COMPARISON_FUNC)0;
	}

	//------------------------------------------------------------------------
	D3D11_FILTER GraphicsFilterToD3D11(GraphicsFilter minFilter, GraphicsFilter magFilter, GraphicsFilter mipFilter)
	{
		if (GraphicsFilter::Point == minFilter && GraphicsFilter::Point == magFilter && GraphicsFilter::Point == magFilter) return D3D11_FILTER_MIN_MAG_MIP_POINT;
		if (GraphicsFilter::Point == minFilter && GraphicsFilter::Point == magFilter && GraphicsFilter::Linear == magFilter) return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		if (GraphicsFilter::Point == minFilter && GraphicsFilter::Linear == magFilter && GraphicsFilter::Point == magFilter) return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		if (GraphicsFilter::Point == minFilter && GraphicsFilter::Linear == magFilter && GraphicsFilter::Linear == magFilter) return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		if (GraphicsFilter::Linear == minFilter && GraphicsFilter::Point == magFilter && GraphicsFilter::Point == magFilter) return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		if (GraphicsFilter::Linear == minFilter && GraphicsFilter::Point == magFilter && GraphicsFilter::Linear == magFilter) return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		if (GraphicsFilter::Linear == minFilter && GraphicsFilter::Linear == magFilter && GraphicsFilter::Point == magFilter) return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		if (GraphicsFilter::Linear == minFilter && GraphicsFilter::Linear == magFilter && GraphicsFilter::Linear == magFilter) return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		if (GraphicsFilter::Anisotropic == minFilter || GraphicsFilter::Anisotropic == magFilter || GraphicsFilter::Anisotropic == magFilter) return D3D11_FILTER_ANISOTROPIC;
		return (D3D11_FILTER)0;
	}

	//------------------------------------------------------------------------
	D3D11_TEXTURE_ADDRESS_MODE GraphicsTextureAddressModeToD3D11(GraphicsTextureAddressMode mode)
	{
		switch (mode)
		{
		case GraphicsTextureAddressMode::Wrap: return D3D11_TEXTURE_ADDRESS_WRAP;
		case GraphicsTextureAddressMode::Mirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
		case GraphicsTextureAddressMode::Clamp: return D3D11_TEXTURE_ADDRESS_CLAMP;
		case GraphicsTextureAddressMode::Border: return D3D11_TEXTURE_ADDRESS_BORDER;
		case GraphicsTextureAddressMode::MirrorOnce: return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		}
		return (D3D11_TEXTURE_ADDRESS_MODE)0;
	}

	//------------------------------------------------------------------------
	D3D11_SAMPLER_DESC GraphicsSamplerStateToD3D11(const GraphicsSamplerState &state)
	{
		D3D11_SAMPLER_DESC desc;
		desc.Filter = GraphicsFilterToD3D11(state.MinFilter, state.MagFilter, state.MipFilter);
		desc.AddressU = GraphicsTextureAddressModeToD3D11(state.AddressU);
		desc.AddressV = GraphicsTextureAddressModeToD3D11(state.AddressV);
		desc.AddressW = GraphicsTextureAddressModeToD3D11(state.AddressW);
		desc.MipLODBias = (FLOAT)state.MipLodBias;
		desc.MaxAnisotropy = (UINT)state.MaxAnisotropy;
		desc.ComparisonFunc = GraphicsCmpFuncToD3D11(state.CmpFunc);
		desc.BorderColor[0] = (FLOAT)state.BorderColor.x;
		desc.BorderColor[1] = (FLOAT)state.BorderColor.y;
		desc.BorderColor[2] = (FLOAT)state.BorderColor.z;
		desc.BorderColor[3] = (FLOAT)state.BorderColor.w;
		desc.MinLOD = (FLOAT)state.MipLodMin;
		desc.MaxLOD = (FLOAT)state.MipLodMax;
		return desc;
	}

	//------------------------------------------------------------------------
	D3D11_FILL_MODE GraphicsFillModeToD3D11(GraphicsFillMode mode)
	{
		switch (mode)
		{
		case GraphicsFillMode::Wireframe: return D3D11_FILL_WIREFRAME;
		case GraphicsFillMode::Solid: return D3D11_FILL_SOLID;
		}
		return (D3D11_FILL_MODE)0;
	}

	//------------------------------------------------------------------------
	D3D11_CULL_MODE GraphicsCullModeToD3D11(GraphicsCullMode mode)
	{
		switch (mode)
		{
		case GraphicsCullMode::None: return D3D11_CULL_NONE;
		case GraphicsCullMode::CW: return D3D11_CULL_FRONT;
		case GraphicsCullMode::CCW: return D3D11_CULL_BACK;
		}
		return (D3D11_CULL_MODE)0;
	}

	//------------------------------------------------------------------------
	D3D11_RASTERIZER_DESC GraphicsRasterizerStateToD3D11(const GraphicsRasterizerState& state)
	{
		D3D11_RASTERIZER_DESC desc;
		desc.FillMode = GraphicsFillModeToD3D11(state.FillMode);
		desc.CullMode = GraphicsCullModeToD3D11(state.CullMode);
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = (INT)state.DepthBias;
		desc.DepthBiasClamp = (FLOAT)state.DepthBiasClamp;
		desc.SlopeScaledDepthBias = (FLOAT)state.SlopeScaledDepthBias;
		desc.DepthClipEnable = (BOOL)state.DepthClipEnable;
		desc.ScissorEnable = (BOOL)state.ScissorEnable;
		desc.MultisampleEnable = (BOOL)state.MultisampleEnable;
		desc.AntialiasedLineEnable = FALSE;
		return desc;
	}

	//------------------------------------------------------------------------
	D3D11_STENCIL_OP GraphicsStencilOpToD3D11(GraphicsStencilOp stencilOp)
	{
		switch (stencilOp)
		{
		case GraphicsStencilOp::Keep: return D3D11_STENCIL_OP_KEEP;
		case GraphicsStencilOp::Zero: return D3D11_STENCIL_OP_ZERO;
		case GraphicsStencilOp::Replace: return D3D11_STENCIL_OP_REPLACE;
		case GraphicsStencilOp::IncrSat: return D3D11_STENCIL_OP_INCR_SAT;
		case GraphicsStencilOp::DecrSat: return D3D11_STENCIL_OP_DECR_SAT;
		case GraphicsStencilOp::Invert: return D3D11_STENCIL_OP_INVERT;
		case GraphicsStencilOp::Incr: return D3D11_STENCIL_OP_INCR;
		case GraphicsStencilOp::Decr: return D3D11_STENCIL_OP_DECR;
		}
		return (D3D11_STENCIL_OP)0;
	}

	//------------------------------------------------------------------------
	D3D11_DEPTH_STENCILOP_DESC GraphicsDepthStencilOpDescToD3D11(const GraphicsDepthStencilOpDesc& GraphicsDesc)
	{
		D3D11_DEPTH_STENCILOP_DESC desc;
		desc.StencilFailOp = GraphicsStencilOpToD3D11(GraphicsDesc.StencilFailOp);
		desc.StencilDepthFailOp = GraphicsStencilOpToD3D11(GraphicsDesc.StencilDepthFailOp);
		desc.StencilPassOp = GraphicsStencilOpToD3D11(GraphicsDesc.StencilPassOp);
		desc.StencilFunc = GraphicsCmpFuncToD3D11(GraphicsDesc.StencilFunc);
		return desc;
	}

	//------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_DESC GraphicsDepthStencilStateToD3D11(const GraphicsDepthStencilState &state)
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		desc.DepthEnable = (BOOL)state.DepthEnable;
		desc.DepthWriteMask = (state.DepthWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO);
		desc.DepthFunc = GraphicsCmpFuncToD3D11(state.DepthFunc);
		desc.StencilEnable = (BOOL)state.StencilEnable;
		desc.StencilReadMask = (UINT8)state.StencilReadMask;
		desc.StencilWriteMask = (UINT8)state.StencilWriteMask;
		desc.FrontFace = GraphicsDepthStencilOpDescToD3D11(state.FrontFace);
		desc.BackFace = GraphicsDepthStencilOpDescToD3D11(state.BackFace);
		return desc;
	}
}