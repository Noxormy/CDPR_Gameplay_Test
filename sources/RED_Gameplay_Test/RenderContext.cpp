#include "pch.h"
#include "RenderContext.h"
#include "Engine.h"

namespace cdp_framework
{
	void cdp_framework::RenderContext::RenderPrimitive( const PrimitivePtr& primitive, const Vector3& scale, const Vector3& position, const Vector3& rotation, const FXMVECTOR& color /*= Colors::White*/ )
	{
		assert( m_engine );

		XMVECTOR orientation = Quaternion::CreateFromYawPitchRoll( rotation.x, rotation.y, rotation.z );
		FXMMATRIX local = m_engine->m_world * XMMatrixTransformation( g_XMZero, Quaternion::Identity, scale, g_XMZero, orientation, position );

		primitive->Draw( local, m_engine->m_view, m_engine->m_projection, color );
	}

	RenderContext::RenderContext( Engine* engine) :
		m_engine( engine )
	{
	}

	void RenderContext::RenderTexture( const TexturePtr& texture, const Vector2& position, const FXMVECTOR& color /*= Colors::White*/, const float rotation /*= 0.0f*/, const Vector2& origin /*= Vector2::Zero*/, const float scale /*= 1.0f*/, SpriteEffects effects /*= SpriteEffects::SpriteEffects_None */ )
	{
		assert( m_engine );

		m_engine->m_sprites->Begin();
		m_engine->m_sprites->Draw( texture.Get(), position, nullptr, color, rotation, origin, scale, effects );
		m_engine->m_sprites->End();
	}

	void RenderContext::RenderModel( const ModelPtr& model, const Vector3& scale, const Vector3& position, const Vector3& rotation )
	{
		assert( m_engine );

		XMVECTOR orientation = Quaternion::CreateFromYawPitchRoll( rotation.x, rotation.y, rotation.z );
		FXMMATRIX local = m_engine->m_world * XMMatrixTransformation( g_XMZero, Quaternion::Identity, scale, g_XMZero, orientation, position );
		model->Draw( m_engine->m_deviceResources->GetD3DDeviceContext(), *m_engine->m_states, local, m_engine->m_view, m_engine->m_projection );
	}

	void RenderContext::RenderText( const std::string& text, const Vector2& position, float scale, const FXMVECTOR& color /*= Colors::White */ )
	{
		assert( m_engine );

		m_engine->m_sprites->Begin();
		m_engine->m_font->DrawString( m_engine->m_sprites.get(), text.c_str(), position, color, 0, Vector2::Zero, scale );
		m_engine->m_sprites->End();
	}

	std::shared_ptr< DX::DeviceResources > RenderContext::GetDeviceRescourcesNative()
	{
		if( m_engine )
		{
			return m_engine->m_deviceResources;
		}
		return nullptr;
	}
}