/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#include "pch.h"
#include "Crosshair.h"
#include "Engine.h"
#include "StepTimer.h"
#include "DeviceResources.h"

Crosshair::Crosshair() = default;
Crosshair::~Crosshair() = default;

void Crosshair::OnInitialize()
{
    Vector2 textureSize;
    m_crosshair = GetEngine().CreateTexture( L"../../data/assets/crosshair.png", &textureSize );

    m_origin.x = textureSize.x * 0.5f;
    m_origin.y = textureSize.y * 0.5f;
}

void Crosshair::OnUpdate( float deltaTime, DirectX::Mouse& mouse, DirectX::GamePad& gamepad )
{
	UNREFERENCED_PARAMETER( deltaTime );

    auto padState = gamepad.GetState( 0 );
    auto mouseState = mouse.GetState();
    if ( ( padState.IsConnected() && padState.IsRightTriggerPressed() ) || mouseState.leftButton )
    {
        m_color = Colors::Red.v;
        m_scale = 0.11f;
        m_rotation = -25.0f;
    }
    else
    {
        m_color = Colors::DarkRed.v;
        m_scale = 0.1f;
        m_rotation = 0.0f;
    }

    if ( ( padState.IsConnected() && padState.IsLeftTriggerPressed() ) || mouseState.rightButton )
    {
        m_color = Colors::Yellow.v;
        m_scale = 0.09f;
        m_rotation = 25.0f;
    }
}

void Crosshair::OnRender( cdp_framework::RenderContextPtr& renderContext )
{
    Vector2 windowSize = GetEngine().GetWindowSize();
    renderContext->RenderTexture( m_crosshair, windowSize * 0.5f, m_color, m_rotation, m_origin, m_scale );
}

void Crosshair::OnShutdown()
{
    m_crosshair.Reset();
}
