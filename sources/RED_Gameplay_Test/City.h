/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#pragma once
#include "IRenderContext.h"

struct Skyscraper final
{
	std::unique_ptr< DirectX::GeometricPrimitive > shape;
	Vector3 position;
	float width;
	float length;
	float height;
};

class City
{

public:
	City();
	~City();

	void OnInitialize();
	void OnUpdate( float deltaTime );
	void OnRender( cdp_framework::RenderContextPtr& renderContext );
	void OnShutdown();

	const std::vector< Skyscraper >& GetSkyscrapers() const;

private:
    void Load();

	std::vector< Skyscraper > m_skyscrapers;
};

