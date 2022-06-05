#pragma once


namespace DX
{
	class DeviceResources;
}

namespace cdp_framework
{
	class IRenderContext
	{
	public:
		// *********************** Basic API ***********************
		virtual ~IRenderContext() = default;

		virtual void RenderPrimitive ( const PrimitivePtr& primitive, const Vector3& scale, const Vector3& position,
									   const Vector3& rotation, const FXMVECTOR& color = Colors::White ) = 0;
		virtual void RenderTexture( const TexturePtr& texture, const Vector2& position, const FXMVECTOR& color = Colors::White, const float rotation = 0.0f,
								    const Vector2& origin = Vector2::Zero, const float scale = 1.0f, SpriteEffects effects = SpriteEffects::SpriteEffects_None ) = 0;
		virtual void RenderModel( const ModelPtr& model, const Vector3& scale, const Vector3& position, const Vector3& rotation ) = 0;
		virtual void RenderText( const std::string& text, const Vector2& position, float scale, const FXMVECTOR& color = Colors::White ) = 0;

		// *********************** Native API ***********************
		virtual std::shared_ptr< DX::DeviceResources > GetDeviceRescourcesNative() = 0;
	};

	using RenderContextPtr = std::unique_ptr< IRenderContext >;

} // cdp_framework
