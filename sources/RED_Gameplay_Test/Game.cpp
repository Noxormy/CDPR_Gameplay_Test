/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "ShootingManager.h"

// #define DEBUG

Game::Game()
{
    m_camera = std::make_unique< Camera >();
    m_city = std::make_unique< City >();
    m_flocking_manager = std::make_unique< FlockingManager >();
	m_crosshair = std::make_unique< Crosshair >();
	m_shooting_manager = std::make_unique< ShootingManager >();
}

Game::~Game() = default;

void Game::OnInitialize()
{
    m_city->OnInitialize();
   	m_flocking_manager->OnInitialize();
	m_crosshair->OnInitialize();

	for(const Skyscraper& obstacle : m_city->GetSkyscrapers()) {
		m_flocking_manager->AddObstacle(obstacle.position, Vector3{obstacle.width / 2.f, obstacle.height / 2.f, obstacle.width / 2.f});
	}
	m_flocking_manager->Spawn(100);
}

void Game::OnUpdate( float deltaTime, DirectX::Keyboard& keyboard, DirectX::Mouse& mouse, DirectX::GamePad& gamepad )
{
	m_camera->OnUpdate( deltaTime, keyboard, mouse, gamepad );
	m_city->OnUpdate( deltaTime );
	m_flocking_manager->OnUpdate(deltaTime);
    m_crosshair->OnUpdate( deltaTime, mouse, gamepad );
    m_shooting_manager->OnUpdate( deltaTime, *m_camera.get(), *m_flocking_manager.get(), keyboard, mouse, gamepad );
}

void Game::OnRender( cdp_framework::RenderContextPtr& renderContext )
{
#ifndef DEBUG
	m_city->OnRender( renderContext );
#endif
    m_flocking_manager->OnRender(renderContext);
	m_crosshair->OnRender( renderContext );
}

void Game::OnShutdown()
{
	m_city->OnShutdown();
   	m_flocking_manager->OnShutdown();
	m_crosshair->OnShutdown();
}
