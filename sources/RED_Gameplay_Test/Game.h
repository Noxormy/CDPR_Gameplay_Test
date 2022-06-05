/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#pragma once
#include "IGame.h"
#include "IRenderContext.h"
#include "City.h"
#include "Camera.h"
#include "Crosshair.h"
#include "FlockingManager.h"
#include "ShootingManager.h"


class Game final : public cdp_framework::IGame
{
public:
	Game();
	virtual ~Game() override;

	Game( Game&& ) = default;
	Game& operator= ( Game&& ) = default;

	Game( Game const& ) = delete;
	Game& operator= ( Game const& ) = delete;

	void OnInitialize() override;
	void OnUpdate( float deltaTime, DirectX::Keyboard& keyboard, DirectX::Mouse& mouse, DirectX::GamePad& gamepad ) override;
	void OnRender( cdp_framework::RenderContextPtr& renderContext ) override;
	void OnShutdown() override;

private:
	std::unique_ptr< Camera >				                m_camera;
    std::unique_ptr< City >					                m_city;
    std::unique_ptr< FlockingManager >			            m_flocking_manager;
	std::unique_ptr< Crosshair >			                m_crosshair;
    std::unique_ptr< ShootingManager >						m_shooting_manager;
};

