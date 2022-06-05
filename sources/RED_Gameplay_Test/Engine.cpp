/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#include "pch.h"
#include "Engine.h"
#include "Game.h"
#include "RenderContext.h"

extern void ExitGame() noexcept;

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace cdp_framework
{

	Engine::Engine() noexcept( false )
	{
		m_deviceResources = std::make_shared< DX::DeviceResources >();
		m_deviceResources->RegisterDeviceNotify( this );
	}

	Engine::~Engine()
    {
        if ( m_audEngine )
        {
            m_audEngine->Suspend();
        }
	}

	// Initialize the Direct3D resources required to run.
	void Engine::Initialize( HWND window, int width, int height )
	{
		CreateGame();

		m_gamePad = std::make_unique< GamePad >();

		m_keyboard = std::make_unique< Keyboard >();

		m_mouse = std::make_unique< Mouse >();
		m_mouse->SetWindow( window );

		m_deviceResources->SetWindow( window, width, height );

		m_deviceResources->CreateDeviceResources();
		CreateDeviceDependentResources();

		m_deviceResources->CreateWindowSizeDependentResources();
		CreateWindowSizeDependentResources();

		CreateRenderContext();

        // Create DirectXTK for Audio objects
        AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
        eflags = eflags | AudioEngine_Debug;
#endif
        m_audEngine = std::make_unique< AudioEngine >( eflags );
        m_audioEvent = 0;
        m_audioTimerAcc = 10.f;
        m_retryDefault = false;
        // Create DirectXTK for Audio objects

		m_game->OnInitialize();

        m_timer.SetFixedTimeStep( true );
        m_timer.SetTargetElapsedSeconds( 1.0 / 60 );
	}

	// Executes the basic game loop.
	void Engine::Tick()
	{
		m_timer.Tick( [&]()
		{
			Update( m_timer );
		} );

        // Only update audio engine once per frame
        if ( !m_audEngine->IsCriticalError() && m_audEngine->Update() )
        {
            // Setup a retry in 1 second
            m_audioTimerAcc = 1.f;
            m_retryDefault = true;
        }

		Render();
	}

	// Updates the world.
	void Engine::Update( DX::StepTimer const& timer )
	{
		float elapsedTime = static_cast< float >( timer.GetElapsedSeconds() );

		uint32_t fps = timer.GetFramesPerSecond();
		m_fps = ( fps > 0 ) ? std::to_string( fps ) + " FPS" : " - FPS";

		m_game->OnUpdate( elapsedTime, *m_keyboard.get(), *m_mouse.get(), *m_gamePad.get() );

		m_world = Matrix::Identity;

		m_batchEffect->SetView( m_view );
		m_batchEffect->SetWorld( Matrix::Identity );
        
        m_audioTimerAcc -= static_cast< float >( timer.GetElapsedSeconds() );
        if ( m_audioTimerAcc < 0 )
        {
            if ( m_retryDefault )
            {
                m_retryDefault = false;
                if ( m_audEngine->Reset() )
                {
                    // Restart looping audio
                }
            }
            else
            {
                m_audioTimerAcc = 4.f;

                if ( m_audioEvent >= 11 )
                    m_audioEvent = 0;
            }
        }

		auto pad = m_gamePad->GetState( 0 );
		if( pad.IsConnected() )
		{
			if( pad.IsViewPressed() )
			{
				ExitGame();
			}
		}

		auto kb = m_keyboard->GetState();
		if( kb.Escape )
		{
			ExitGame();
		}

	}

	// Draws the scene.
	void Engine::Render()
	{
		// Don't try to render anything before the first Update.
		if( m_timer.GetFrameCount() == 0 )
		{
			return;
		}

		Clear();

		m_deviceResources->PIXBeginEvent( L"Render" );

		// Render procedurally generated dynamic grid
		const XMVECTORF32 xaxis = { 20.f, 0.f, 0.f };
		const XMVECTORF32 yaxis = { 0.f, 0.f, 20.f };
		DrawGrid( xaxis, yaxis, g_XMZero, 20, 20, Colors::DarkSlateGray );

		// Render Game
		m_game->OnRender( m_RenderContext );

		// Render FPS
		m_sprites->Begin();
		m_font->DrawString( m_sprites.get(), m_fps.c_str(), XMFLOAT2( 10, 10 ), Colors::Yellow );
		m_sprites->End();

		// Show the new frame.
		m_deviceResources->Present();
	}

	// Helper method to clear the back buffers.
	void Engine::Clear()
	{
		m_deviceResources->PIXBeginEvent( L"Clear" );

		// Clear the views.
		auto context = m_deviceResources->GetD3DDeviceContext();
		auto renderTarget = m_deviceResources->GetRenderTargetView();
		auto depthStencil = m_deviceResources->GetDepthStencilView();

		context->ClearRenderTargetView( renderTarget, Colors::CornflowerBlue );
		context->ClearDepthStencilView( depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
		context->OMSetRenderTargets( 1, &renderTarget, depthStencil );

		// Set the viewport.
		auto viewport = m_deviceResources->GetScreenViewport();
		context->RSSetViewports( 1, &viewport );

		m_deviceResources->PIXEndEvent();
	}

	void XM_CALLCONV Engine::DrawGrid( FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color )
	{
		m_deviceResources->PIXBeginEvent( L"Draw grid" );

		auto context = m_deviceResources->GetD3DDeviceContext();
		context->OMSetBlendState( m_states->Opaque(), nullptr, 0xFFFFFFFF );
		context->OMSetDepthStencilState( m_states->DepthNone(), 0 );
		context->RSSetState( m_states->CullCounterClockwise() );

		m_batchEffect->Apply( context );

		context->IASetInputLayout( m_batchInputLayout.Get() );

		m_batch->Begin();

		xdivs = std::max<size_t>( 1, xdivs );
		ydivs = std::max<size_t>( 1, ydivs );

		for( size_t i = 0; i <= xdivs; ++i )
		{
			float percent = static_cast< float >( i ) / static_cast< float >( xdivs );
			percent = ( percent * 2.0f ) - 1.0f;
			XMVECTOR scale = XMVectorScale( xAxis, percent );
			scale = XMVectorAdd( scale, origin );

			VertexPositionColor v1( XMVectorSubtract( scale, yAxis ), color );
			VertexPositionColor v2( XMVectorAdd( scale, yAxis ), color );
			m_batch->DrawLine( v1, v2 );
		}

		for( size_t i = 0; i <= ydivs; i++ )
		{
			float percent = static_cast< float >( i ) / static_cast< float >( ydivs );
			percent = ( percent * 2.0f ) - 1.0f;
			XMVECTOR scale = XMVectorScale( yAxis, percent );
			scale = XMVectorAdd( scale, origin );

			VertexPositionColor v1( XMVectorSubtract( scale, xAxis ), color );
			VertexPositionColor v2( XMVectorAdd( scale, xAxis ), color );
			m_batch->DrawLine( v1, v2 );
		}

		m_batch->End();

		m_deviceResources->PIXEndEvent();
	}

	// Message handlers
	void Engine::OnActivated()
	{
	}

	void Engine::OnDeactivated()
	{
	}

	void Engine::OnSuspending()
	{
        m_audEngine->Suspend();
	}

	void Engine::OnResuming()
	{
		m_timer.ResetElapsedTime();
        m_audEngine->Resume();
    }

	void Engine::OnWindowMoved()
	{
		auto r = m_deviceResources->GetOutputSize();
		m_deviceResources->WindowSizeChanged( r.right, r.bottom );
	}

	void Engine::OnWindowSizeChanged( int width, int height )
	{
		if( !m_deviceResources->WindowSizeChanged( width, height ) )
			return;

		CreateWindowSizeDependentResources();
	}

	// Properties
	void Engine::GetDefaultSize( int& width, int& height ) const noexcept
	{
		width = 1280;
		height = 720;
	}

	void Engine::LookAt( const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target )
	{
		m_view = Matrix::CreateLookAt( eye, target, Vector3::UnitY );
	}

	DirectX::SimpleMath::Vector2 Engine::GetMousePosition() const
	{
		const auto& state = m_mouse->GetState();
        return Vector2( static_cast< float >( state.x ), static_cast< float >( state.y ) );
	}

	bool Engine::IsKeyPressed( const DirectX::Keyboard::Keys key ) const
	{
		if( m_keyboard )
		{
			m_keyboard->GetState().IsKeyDown( key );
		}

		return false;
	}

	DirectX::GamePad& Engine::GetGamePad() const
	{
		return *m_gamePad.get();
	}

	DirectX::Mouse& Engine::GetMouse() const
	{
		return *m_mouse.get();
	}

	DirectX::Keyboard& Engine::GetKeyboard() const
	{
		return *m_keyboard.get();
	}

	std::unique_ptr< DirectX::GeometricPrimitive > Engine::CreateBoxPrimitive( const DirectX::SimpleMath::Vector3& dimensions )
	{
		return DirectX::GeometricPrimitive::CreateBox( m_deviceResources->GetD3DDeviceContext(), dimensions );
	}

	std::unique_ptr< DirectX::GeometricPrimitive > Engine::CreateSpherePrimitive( const float radius )
	{
		return DirectX::GeometricPrimitive::CreateSphere( m_deviceResources->GetD3DDeviceContext(), radius );
	}

	void Engine::RenderPrimitive( const PrimitivePtr& primitive, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation, const FXMVECTOR& color )
	{
		XMVECTOR orientation = Quaternion::CreateFromYawPitchRoll( rotation.x, rotation.y, rotation.z );
		FXMMATRIX local = m_world * XMMatrixTransformation( g_XMZero, Quaternion::Identity, scale, g_XMZero, orientation, position );

		primitive->Draw( local, m_view, m_projection, color );
	}

	ModelPtr Engine::LoadModel( const std::wstring& path )
	{
		auto idx = path.find_last_of( L"." );
		if( idx != std::wstring::npos )
		{
			const std::wstring extension = path.substr( idx + 1 );

			if( _wcsicmp( extension.c_str(), L"sdkmesh" ) == 0)
			{
				return ::Model::CreateFromSDKMESH( m_deviceResources->GetD3DDevice(), path.c_str(), *m_fxFactory );
			}

			if( _wcsicmp( extension.c_str(), L"cmo" ) == 0 )
			{
				return ::Model::CreateFromCMO( m_deviceResources->GetD3DDevice(), path.c_str(), *m_fxFactory );
			}

            if ( _wcsicmp( extension.c_str(), L"vbo" ) == 0 )
            {
                return ::Model::CreateFromVBO( m_deviceResources->GetD3DDevice(), path.c_str() );
            }
		}

		assert( false ); // Not supported model format
		return ModelPtr();
	}

	void Engine::RenderModel( const ModelPtr& model, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation )
	{
		XMVECTOR orientation = Quaternion::CreateFromYawPitchRoll( rotation.x, rotation.y, rotation.z );
		FXMMATRIX local = m_world * XMMatrixTransformation( g_XMZero, Quaternion::Identity, scale, g_XMZero, orientation, position );
		model->Draw( m_deviceResources->GetD3DDeviceContext(), *m_states, local, m_view, m_projection );
	}

    TexturePtr Engine::CreateTexture( const std::wstring& path, Vector2* outTextureSize /*= nullptr */ )
    {
        HRESULT result = S_FALSE;
        TexturePtr texture;
        ResourcePtr resource;
        auto idx = path.find_last_of( L"." );
        if ( idx != std::wstring::npos )
        {
            const std::wstring extension = path.substr( idx + 1 );
            if ( _wcsicmp( extension.c_str(), L"dds" ) == 0 )
            {
                result = CreateDDSTextureFromFile( m_deviceResources->GetD3DDevice(), path.c_str(), resource.GetAddressOf(), texture.ReleaseAndGetAddressOf() );
            }
            else // bmp, jpeg, png, tiff, gif
            {
                result = CreateWICTextureFromFile( m_deviceResources->GetD3DDevice(), path.c_str(), resource.GetAddressOf(), texture.ReleaseAndGetAddressOf() );
            }
        }

        if ( result != S_OK )
        {
            assert( false ); // Failed to load texture
        }
        else
        {
            if ( resource && outTextureSize )
            {
                Texture2DPtr crosshairTexture;
                DX::ThrowIfFailed( resource.As( &crosshairTexture ) );

                CD3D11_TEXTURE2D_DESC crosshairDesc;
                crosshairTexture->GetDesc( &crosshairDesc );

                outTextureSize->x = static_cast< float >( crosshairDesc.Width );
                outTextureSize->y = static_cast< float >( crosshairDesc.Height );
            }
        }

        return texture;
    }

    void Engine::RenderTexture( const TexturePtr& texture, const Vector2& position, const FXMVECTOR& color /*= Colors::White*/, const float rotation /*= 0.0f*/, const Vector2& origin /*= Vector2::Zero*/, const float scale /*= 1.0f*/, SpriteEffects effects /*= SpriteEffects::SpriteEffects_None */ )
    {
        m_sprites->Begin();
        m_sprites->Draw( texture.Get(), position, nullptr, color, rotation, origin, scale, effects );
        m_sprites->End();
    }

    void Engine::RenderText( const std::string& text, const DirectX::SimpleMath::Vector2& position, float scale, const FXMVECTOR& color )
	{
		m_sprites->Begin();
		m_font->DrawString( m_sprites.get(), text.c_str(), position, color, 0, Vector2::Zero, scale );
		m_sprites->End();
	}


    DirectX::SimpleMath::Vector2 Engine::GetWindowSize()
    {
        RECT outputSize = m_deviceResources->GetOutputSize();
        Vector2 windowSize{ static_cast< float >( outputSize.right ), static_cast< float >( outputSize.bottom ) };
        return windowSize;
    }

	// These are the resources that depend on the device.
	void Engine::CreateDeviceDependentResources()
	{
		auto context = m_deviceResources->GetD3DDeviceContext();
		auto device = m_deviceResources->GetD3DDevice();

		m_states = std::make_unique< CommonStates >( device );

		m_fxFactory = std::make_unique< EffectFactory >( device );

		m_sprites = std::make_unique< SpriteBatch >( context );

		m_batch = std::make_unique< PrimitiveBatch< VertexPositionColor > >( context );

		m_batchEffect = std::make_unique< BasicEffect >( device );
		m_batchEffect->SetVertexColorEnabled( true );

		{
			void const* shaderByteCode;
			size_t byteCodeLength;

			m_batchEffect->GetVertexShaderBytecode( &shaderByteCode, &byteCodeLength );

			DX::ThrowIfFailed(
				device->CreateInputLayout( VertexPositionColor::InputElements,
					VertexPositionColor::InputElementCount,
					shaderByteCode, byteCodeLength,
					m_batchInputLayout.ReleaseAndGetAddressOf() )
			);
		}

        m_font = std::make_unique< SpriteFont >( device, L"../../data/assets/SegoeUI_18.spritefont" );
	}

	// Allocate all memory resources that change on a window SizeChanged event.
	void Engine::CreateWindowSizeDependentResources()
	{
		auto size = m_deviceResources->GetOutputSize();
		float aspectRatio = static_cast< float >( size.right ) / static_cast< float >( size.bottom );
		float fovAngleY = 70.0f * XM_PI / 180.0f;

		// This is a simple example of change that can be made when the app is in
		// portrait or snapped view.
		if( aspectRatio < 1.0f )
		{
			fovAngleY *= 2.0f;
		}

		// This sample makes use of a right-handed coordinate system using row-major matrices.
		m_projection = Matrix::CreatePerspectiveFieldOfView(
			fovAngleY,
			aspectRatio,
			0.01f,
			100.0f
		);

		m_batchEffect->SetProjection( m_projection );
	}

	void Engine::CreateRenderContext()
	{
		m_RenderContext = std::make_unique< RenderContext >( this );
	}

	void Engine::OnDeviceLost()
	{
		m_states.reset();
		m_fxFactory.reset();
		m_sprites.reset();
		m_batch.reset();
		m_batchEffect.reset();
		m_font.reset();
		m_batchInputLayout.Reset();
		m_game->OnShutdown();
	}

	void Engine::OnDeviceRestored()
	{
		CreateDeviceDependentResources();

		CreateWindowSizeDependentResources();
	}

	void Engine::CreateGame()
	{
		m_game = std::make_unique< Game >();
	}

}