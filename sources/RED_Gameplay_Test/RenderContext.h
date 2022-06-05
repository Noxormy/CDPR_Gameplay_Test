#pragma once
#include "IRenderContext.h"

namespace cdp_framework
{
	class Engine;

	class RenderContext : public IRenderContext
	{

	public:
		RenderContext() = delete;
		RenderContext( Engine* );
		~RenderContext() = default;

		RenderContext( RenderContext&& ) = default;
		RenderContext& operator= ( RenderContext&& ) = default;

		RenderContext( RenderContext const& ) = delete;
		RenderContext& operator= ( RenderContext const& ) = delete;


		virtual void RenderPrimitive ( const PrimitivePtr& primitive, const Vector3& scale, const Vector3& position,
								       const Vector3& rotation, const FXMVECTOR& color = Colors::White ) override;

		virtual void RenderTexture( const TexturePtr& texture, const Vector2& position, const FXMVECTOR& color = Colors::White, const float rotation = 0.0f,
									const Vector2& origin = Vector2::Zero, const float scale = 1.0f, SpriteEffects effects = SpriteEffects::SpriteEffects_None ) override;

		virtual void RenderModel( const ModelPtr& model, const Vector3& scale, const Vector3& position, const Vector3& rotation ) override;

		virtual void RenderText( const std::string& text, const Vector2& position, float scale, const FXMVECTOR& color = Colors::White ) override;

		virtual std::shared_ptr< DX::DeviceResources > GetDeviceRescourcesNative() override;

	private:
		Engine* m_engine = nullptr;
	};

} // cdp_framework