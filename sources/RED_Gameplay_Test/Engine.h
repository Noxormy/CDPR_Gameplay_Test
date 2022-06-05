/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "IEngine.h"
#include "IGame.h"
#include "IRenderContext.h"

namespace cdp_framework
{

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Engine final : public cdp_framework::IEngine, public DX::IDeviceNotify
{
	friend class RenderContext;

public:
	Engine() noexcept( false );
	virtual ~Engine() override;

	Engine( Engine&& ) = default;
	Engine& operator= ( Engine&& ) = default;

	Engine( Engine const& ) = delete;
	Engine& operator= ( Engine const& ) = delete;

	// IEngine
	void LookAt( const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target ) override;

	Vector2 GetMousePosition() const override;
	bool IsKeyPressed( const DirectX::Keyboard::Keys key ) const override;
	DirectX::GamePad& GetGamePad() const override;
	DirectX::Mouse& GetMouse() const override;
	DirectX::Keyboard& GetKeyboard() const override;

	virtual PrimitivePtr CreateBoxPrimitive( const DirectX::SimpleMath::Vector3& dimensions ) override;
	virtual PrimitivePtr CreateSpherePrimitive( const float radius ) override;
	virtual void RenderPrimitive ( const PrimitivePtr& primitive, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& position,
								   const DirectX::SimpleMath::Vector3& rotation, const DirectX::FXMVECTOR& color = DirectX::Colors::White ) override;

	virtual ModelPtr LoadModel( const std::wstring& path ) override;
	virtual void RenderModel( const ModelPtr& model, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& rotation ) override;


    TexturePtr CreateTexture( const std::wstring& path, Vector2* outTextureSize = nullptr ) override;

    void RenderTexture( const TexturePtr& texture, const Vector2& position, const FXMVECTOR& color = Colors::White, const float rotation = 0.0f, const Vector2& origin = Vector2::Zero, const float scale = 1.0f, SpriteEffects effects = SpriteEffects::SpriteEffects_None ) override;

	void RenderText( const std::string& text, const DirectX::SimpleMath::Vector2& position, float scale, const DirectX::FXMVECTOR& color = DirectX::Colors::White ) override;

    Vector2 GetWindowSize() override;
    // IEngine

	// Initialization and management
	void Initialize( HWND window, int width, int height );

	// Basic game loop
	void Tick();

	// IDeviceNotify
	void OnDeviceLost() override;
	void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowMoved();
	void OnWindowSizeChanged( int width, int height );

	// Properties
	void GetDefaultSize( int& width, int& height ) const noexcept;

private:
	void Update( DX::StepTimer const& timer );
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void CreateRenderContext();

	void XM_CALLCONV DrawGrid( DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xdivs, size_t ydivs, DirectX::GXMVECTOR color );

	void CreateGame();

	// Device resources.
	std::shared_ptr< DX::DeviceResources >		                                    m_deviceResources;

	// Rendering loop timer.
	DX::StepTimer								                                    m_timer;

	// Input devices.
	std::unique_ptr< DirectX::Keyboard >											m_keyboard;
	std::unique_ptr< DirectX::Mouse >												m_mouse;
	std::unique_ptr< DirectX::GamePad >												m_gamePad;

	// DirectXTK objects.
	std::unique_ptr< DirectX::CommonStates >										m_states;
	std::unique_ptr< DirectX::BasicEffect >											m_batchEffect;
	std::unique_ptr< DirectX::EffectFactory >										m_fxFactory;

	std::unique_ptr< DirectX::PrimitiveBatch< DirectX::VertexPositionColor > >		m_batch;
	std::unique_ptr< DirectX::SpriteBatch >											m_sprites;
	std::unique_ptr< DirectX::SpriteFont >											m_font;

    Microsoft::WRL::ComPtr< ID3D11InputLayout >										m_batchInputLayout;

    // DirectXTK for Audio objects.
    std::unique_ptr< DirectX::AudioEngine >											m_audEngine;
    uint32_t																		m_audioEvent;
    float																			m_audioTimerAcc;
    bool																			m_retryDefault;
    // DirectXTK for Audio objects.

	DirectX::SimpleMath::Matrix														m_world;
	DirectX::SimpleMath::Matrix														m_view = DirectX::SimpleMath::Matrix::Identity;
	DirectX::SimpleMath::Matrix														m_projection;

	std::string																		m_fps;

	std::unique_ptr< cdp_framework::IGame >											m_game;

	std::unique_ptr< IRenderContext >												m_RenderContext;
};

} // cdp_framework