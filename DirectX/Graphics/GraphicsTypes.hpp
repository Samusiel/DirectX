#pragma once

namespace Graphics
{	
	/************************************************************************/
	/* Graphics enumerations                                                */
	/************************************************************************/
	enum class GraphicsFormat : uint8
	{
		Unknown = 0,
		R32G32B32A32,
		R32G32B32,
		R16G16B16A16,
		R32G32,
		R32G8X24,
		R8G8B8A8,
		B8G8R8A8,
		B8G8R8X8,
		R16G16,
		R32,
		R24G8,
		R8G8,
		B5G6R5,
		B5G5R5A1,
		R16,
		R8,
		A8,
		R1,
		BC1,
		BC2,
		BC3,
		BC4,
		BC5,
		BC6H,
		BC7,
		Count
	};

	enum class GraphicsFormatView : uint8
	{
		Typeless,
		Unorm,
		Snorm,
		Uint,
		Sint,
		Float,
		Count
	};

	enum class GraphicsUsage : uint8
	{
		Default,
		Immutable,
		Dynamic,
		Staging
	};

	enum class GraphicsBindFlag : uint8
	{
		VertexBuffer	= 1 << 0,
		IndexBuffer		= 1 << 1,
		ConstantBuffer	= 1 << 2,
		ShaderResource	= 1 << 3,
		StreamOutput	= 1 << 4,
		RenderTarget	= 1 << 5,
		DepthStencil	= 1 << 6,
		UnorderedAccess = 1 << 7
	};

	enum class GraphicsAccessFlag : uint8
	{
		Read	= 1 << 0,
		Write	= 1 << 1
	};

	enum class GraphicsGPUAccess : uint8
	{
		Read,
		Write,
		ReadWrite,
		WriteDiscard,
		WriteNoOverwrite
	};


	/************************************************************************/
	/* Render view port                                                     */
	/************************************************************************/
	struct GraphicsViewPort
	{
		float X;
		float Y;
		float Width;
		float Height;
		float DepthMin;
		float DepthMax;
	};


	/************************************************************************/
	/* Texture 2D description                                               */
	/************************************************************************/
	struct GraphicsTextureDesc
	{
		uint32 Width;
		uint32 Height;
		uint32 Levels;
		GraphicsFormat Format;
		GraphicsFormatView FormatView;
		GraphicsUsage Usage;
		uint32 BindFlags;
		uint32 AccessFlags;
	};

	
	/************************************************************************/
	/* Presentation parameters                                              */
	/************************************************************************/
	struct GraphicsPresentParams
	{
		GraphicsFormat BufferFormat;
		GraphicsFormat DepthStencilFormat;
		bool Fullscreen;
		bool DepthStencilEnabled;
		uint32 FullscreenRefreshRate;
		uint32 BufferWidth;
		uint32 BufferHeight;
		uint32 BufferCount;
		uint32 MutisampleCount;
		uint32 MutisampleQuality;
	};


	/************************************************************************/
	/* Graphics resource data                                               */
	/************************************************************************/
	struct GraphicsResourceData
	{
		void *Ptr;
		uint32 RowPitch;
		uint32 SlicePitch;
	};

	
	/************************************************************************/
	/* Buffer description                                                   */
	/************************************************************************/
	struct GraphicsBufferDesc
	{
		uint32 Size;
		uint32 BindFlags;
		uint32 AccessFlags;
		uint32 StructureStride;
		GraphicsUsage Usage;
	};


	/************************************************************************/
	/* Graphics primitive topology type                                     */
	/************************************************************************/
	enum class GraphicsPrimitiveTopology : uint8
	{
		Undefined,
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip
	};


	/************************************************************************/
	/* Graphics color channel flags                                         */
	/************************************************************************/
	enum class GraphicsChannelFlag : uint8
	{
		None	= 0,
		Red		= 1,
		Green	= 2,
		Blue	= 4,
		Alpha	= 8,
		All		= (Red | Green | Blue | Alpha)
	};


	/************************************************************************/
	/* Graphics blend factor types                                          */
	/************************************************************************/
	enum class GraphicsBlendFactor : uint8
	{
		Zero,
		One,
		SrcColor,
		InvSrcColor,
		SrcAlpha,
		InvSrcAlpha,
		DstAlpha,
		InvDstAlpha,
		DstColor,
		InvDstColor
	};


	/************************************************************************/
	/* Graphics blend operation types                                       */
	/************************************************************************/
	enum class GraphicsBlendOp : uint8
	{
		Add,
		Subtract,
		RevSubtract,
		Min,
		Max
	};


	/************************************************************************/
	/* Graphics blend state description                                     */
	/************************************************************************/
	struct GraphicsBlendState
	{
		bool BlendEnable;
		GraphicsBlendFactor SrcBlend;
		GraphicsBlendFactor DstBlend;
		GraphicsBlendFactor SrcBlendAlpha;
		GraphicsBlendFactor DstBlendAlpha;
		GraphicsBlendOp BlendOp;
		GraphicsBlendOp BlendOpAlpha;
		uint8 RenderTargetWriteMask;

		static const GraphicsBlendState Default;
	};


	/************************************************************************/
	/* Graphics comparison function types                                   */
	/************************************************************************/
	enum class GraphicsCmpFunc : uint8
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};


	/************************************************************************/
	/* Graphics stencil operation types                                     */
	/************************************************************************/
	enum class GraphicsStencilOp : uint8
	{
		Keep,
		Zero,
		Replace,
		IncrSat,
		DecrSat,
		Invert,
		Incr,
		Decr
	};


	/************************************************************************/
	/* Graphics depth stencil operations description                        */
	/************************************************************************/
	struct GraphicsDepthStencilOpDesc
	{
		GraphicsStencilOp StencilFailOp;
		GraphicsStencilOp StencilDepthFailOp;
		GraphicsStencilOp StencilPassOp;
		GraphicsCmpFunc StencilFunc;

		static const GraphicsDepthStencilOpDesc Default;
	};


	/************************************************************************/
	/* Graphics depth stencil state description                             */
	/************************************************************************/
	struct GraphicsDepthStencilState
	{
		bool DepthEnable;
		bool DepthWriteEnable;
		GraphicsCmpFunc DepthFunc;
		bool StencilEnable;
		uint8 StencilReadMask;
		uint8 StencilWriteMask;
		GraphicsDepthStencilOpDesc FrontFace;
		GraphicsDepthStencilOpDesc BackFace;

		static const uint8 DefaultStencilReadMask;
		static const uint8 DefaultStencilWriteMask;
		static const GraphicsDepthStencilState Default;
	};


	/************************************************************************/
	/* Graphics filter types                                                */
	/************************************************************************/
	enum class GraphicsFilter : uint8
	{
		Point,
		Linear,
		Anisotropic
	};


	/************************************************************************/
	/* Graphics texture coordinates resolving mode                          */
	/************************************************************************/
	enum class GraphicsTextureAddressMode : uint8
	{
		Wrap,
		Mirror,
		Clamp,
		Border,
		MirrorOnce
	};


	/************************************************************************/
	/* Graphics sampler state description                                   */
	/************************************************************************/
	struct GraphicsSamplerState
	{
		GraphicsFilter MinFilter;
		GraphicsFilter MagFilter;
		GraphicsFilter MipFilter;
		GraphicsTextureAddressMode AddressU;
		GraphicsTextureAddressMode AddressV;
		GraphicsTextureAddressMode AddressW;
		float MipLodBias;
		float MipLodMin;
		float MipLodMax;
		uint32 MaxAnisotropy;
		Vector4 BorderColor;
		GraphicsCmpFunc CmpFunc;

		static const GraphicsSamplerState Default;
	};


	/************************************************************************/
	/* Graphics primitives fill mode                                        */
	/************************************************************************/
	enum class GraphicsFillMode : uint8
	{
		Wireframe,
		Solid
	};


	/************************************************************************/
	/* Graphics primitives culling mode                                     */
	/************************************************************************/
	enum class GraphicsCullMode : uint8
	{
		None,
		CW,
		CCW
	};


	/************************************************************************/
	/* Graphics rasterizer state description                                */
	/************************************************************************/
	struct GraphicsRasterizerState
	{
		GraphicsFillMode FillMode;
		GraphicsCullMode CullMode;
		int32 DepthBias;
		float DepthBiasClamp;
		float SlopeScaledDepthBias;
		bool DepthClipEnable;
		bool ScissorEnable;
		bool MultisampleEnable;

		static const GraphicsRasterizerState Default;
	};


	/************************************************************************/
	/* Graphics full render state description                               */
	/************************************************************************/
	struct GraphicsRenderState
	{
		static const uint32 MaxRenderTargets = 8;
		static const uint32 MaxSamplerStates = 16;

		GraphicsPrimitiveTopology PrimitiveTopology;
		GraphicsSamplerState SamplerState[MaxSamplerStates];
		GraphicsRasterizerState RasterizerState;
		GraphicsDepthStencilState DepthStencilState;
		GraphicsBlendState BlendState[MaxRenderTargets];
		uint32 StencilRef;

		static const GraphicsRenderState Default;
	};


	/************************************************************************/
	/* Graphics shader input semantic types                                 */
	/************************************************************************/
	enum class GraphicsSemantic : uint8
	{
		Position,
		TexCoord,
		Color,
		Tangtent,
		Normal,
		Binormal
	};


	/************************************************************************/
	/* Graphics input layout element description                            */
	/************************************************************************/
	struct GraphicsInputElement
	{
		GraphicsSemantic Semantic;
		uint32 SemanticIdx;
		uint32 SlotIdx;
		GraphicsFormat Format;
		GraphicsFormatView FormatView;
		uint32 InstanceStepRate;
	};
}