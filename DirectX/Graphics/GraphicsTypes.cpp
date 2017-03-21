#include "stdafx.h"
#include "GraphicsTypes.hpp"

namespace Graphics
{
	const GraphicsBlendState GraphicsBlendState::Default = {
		false,								//-- BlendEnable
		GraphicsBlendFactor::One,			//-- SrcBlend
		GraphicsBlendFactor::Zero,			//-- DstBlend
		GraphicsBlendFactor::One,			//-- SrcBlendAlpha
		GraphicsBlendFactor::Zero,			//-- DstBlendAlpha
		GraphicsBlendOp::Add,				//-- BlendOp
		GraphicsBlendOp::Add,				//-- BlendOpAlpha
		ur_byte(GraphicsChannelFlag::All)	//-- RenderTargetWriteMask
	};

	const GraphicsDepthStencilOpDesc GraphicsDepthStencilOpDesc::Default = {
		GraphicsStencilOp::Keep,			//-- StencilFailOp
		GraphicsStencilOp::Keep,			//-- StencilDepthFailOp
		GraphicsStencilOp::Keep,			//-- StencilPassOp
		GraphicsCmpFunc::Always				//-- StencilFunc
	};

	const uint8 GraphicsDepthStencilState::DefaultStencilReadMask = 0xff;

	const uint8 GraphicsDepthStencilState::DefaultStencilWriteMask = 0xff;

	const GraphicsDepthStencilState GraphicsDepthStencilState::Default = {
		true,								//-- DepthEnable
		true,								//-- DepthWriteEnable
		GraphicsCmpFunc::Less,				//-- DepthFunc
		false,								//-- StencilEnable
		DefaultStencilReadMask,				//-- StencilReadMask
		DefaultStencilWriteMask,			//-- StencilWriteMask
		GraphicsDepthStencilOpDesc::Default,//-- FrontFace
		GraphicsDepthStencilOpDesc::Default	//-- BackFace
	};

	const GraphicsSamplerState GraphicsSamplerState::Default = {
		GraphicsFilter::Linear,				//-- MinFilter
		GraphicsFilter::Linear,				//-- MagFilter
		GraphicsFilter::Linear,				//-- MipFilter
		GraphicsTextureAddressMode::Clamp,	//-- AddressU
		GraphicsTextureAddressMode::Clamp,	//-- AddressV
		GraphicsTextureAddressMode::Clamp,	//-- AddressW
		0.0f,								//-- MipLodBias
		-std::numeric_limits<float>::max(),	//-- MipLodMin
		+std::numeric_limits<float>::max(),	//-- MipLodMax
		1,									//-- MaxAnisotropy;
		ur_float4(1.0f),					//-- BorderColor
		GraphicsCmpFunc::Never				//-- CmpFunc
	};

	const GraphicsRasterizerState GraphicsRasterizerState::Default = {
		GraphicsFillMode::Solid,			//-- FillMode
		GraphicsCullMode::CCW,				//-- CullMode
		0,									//-- DepthBias
		0.0f,								//-- DepthBiasClamp
		0.0f,								//-- SlopeScaledDepthBias
		true,								//-- DepthClipEnable
		false,								//-- ScissorEnable
		false								//-- MultisampleEnable
	};

	const GraphicsRenderState GraphicsRenderState::Default = {
		GraphicsPrimitiveTopology::TriangleList,	//-- PrimitiveTopology
		{											//-- SamplerState
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default,
			GraphicsSamplerState::Default
		},
		GraphicsRasterizerState::Default,		//-- RasterizerState
		GraphicsDepthStencilState::Default,		//-- DepthStencilState
		{										//-- BlendState
			GraphicsBlendState::Default,
			GraphicsBlendState::Default,
			GraphicsBlendState::Default,
			GraphicsBlendState::Default,
			GraphicsBlendState::Default,
			GraphicsBlendState::Default,
			GraphicsBlendState::Default,
			GraphicsBlendState::Default
		},
		0x0										//-- StencilRef
	};
}