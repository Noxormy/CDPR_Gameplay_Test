/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#pragma once
#include "IRenderContext.h"

class Crosshair
{

public:
    Crosshair();
    ~Crosshair();

    void OnInitialize();
    void OnUpdate( float deltaTime, DirectX::Mouse& mouse, DirectX::GamePad& gamepad );
    void OnRender( cdp_framework::RenderContextPtr& renderContext );
    void OnShutdown();

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>        m_crosshair     = nullptr;

    Vector2                                                 m_origin        = Vector2::Zero;
    Color                                                   m_color         = Colors::DarkRed.v;
    float                                                   m_rotation      = 0.0f;
    float                                                   m_scale         = 0.1f;
};

