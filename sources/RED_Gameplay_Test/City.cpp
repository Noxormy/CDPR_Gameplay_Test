/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#include "pch.h"
#include "City.h"
#include "Engine.h"
#include "StepTimer.h"
#include "DeviceResources.h"

City::City() = default;
City::~City() = default;

void City::OnInitialize()
{
    Load();
}

void City::Load()
{
	OnShutdown();

	std::ifstream stream( "../../data/city/city.json" );
	std::string fileData( ( std::istreambuf_iterator< char >( stream ) ), std::istreambuf_iterator< char >() );

	Json::Document document;
	document.Parse( fileData.c_str() );
	assert( !document.HasParseError() );

	assert( document.IsObject() );

	// skyscrapers
	assert( document.HasMember( "skyscrapers" ) );
	const Json::Value& arrayObject = document[ "skyscrapers" ];
	assert( arrayObject.IsArray() );

	for ( Json::SizeType i = 0; i < arrayObject.Size(); i++ )
	{
		Skyscraper newSkyscraper;

		// pos_x
		assert( arrayObject[ i ].HasMember( "pos_x" ) );
		assert( arrayObject[ i ][ "pos_x" ].IsDouble() );
		newSkyscraper.position.x = arrayObject[ i ][ "pos_x" ].GetDouble();

		// pos_z
		assert( arrayObject[ i ].HasMember( "pos_z" ) );
		assert( arrayObject[ i ][ "pos_z" ].IsDouble() );
		newSkyscraper.position.z = arrayObject[ i ][ "pos_z" ].GetDouble();

		// width
		assert( arrayObject[ i ].HasMember( "width" ) );
		assert( arrayObject[ i ][ "width" ].IsDouble() );
		newSkyscraper.width = arrayObject[ i ][ "width" ].GetDouble();

		// length
		assert( arrayObject[ i ].HasMember( "length" ) );
		assert( arrayObject[ i ][ "length" ].IsDouble() );
		newSkyscraper.length = arrayObject[ i ][ "length" ].GetDouble();

		// height
		assert( arrayObject[ i ].HasMember( "height" ) );
		assert( arrayObject[ i ][ "height" ].IsDouble() );
		newSkyscraper.height = arrayObject[ i ][ "height" ].GetDouble();

		newSkyscraper.position.y = newSkyscraper.height * 0.5f;

		m_skyscrapers.push_back( std::move( newSkyscraper ) );
	}

	// create primitives
	for( Skyscraper& skyscraper : m_skyscrapers )
	{
		skyscraper.shape = GetEngine().CreateBoxPrimitive( Vector3( skyscraper.width, skyscraper.height, skyscraper.length ) );
	}
}

void City::OnUpdate( float deltaTime )
{
	UNREFERENCED_PARAMETER( deltaTime );
}

void City::OnRender( cdp_framework::RenderContextPtr& renderContext )
{
	for ( Skyscraper& skyscraper : m_skyscrapers )
	{
		renderContext->RenderPrimitive( skyscraper.shape, Vector3::One, skyscraper.position, Vector3::Zero, Colors::BlueViolet );
	}
}

void City::OnShutdown()
{
	for ( Skyscraper& skyscraper : m_skyscrapers )
	{
		skyscraper.shape.reset();
	}

	m_skyscrapers.clear();
}

const std::vector< Skyscraper >& City::GetSkyscrapers() const
{
	return m_skyscrapers;
}
