/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#pragma once

namespace cdp_framework
{
	class IEngine
	{
	public:
		virtual ~IEngine() = default;

		// *********************** Camera ***********************
		virtual void LookAt( const Vector3& eye, const Vector3& target ) = 0;
        // *********************** Camera ***********************

        // *********************** Input ***********************
		virtual bool IsKeyPressed( const Keyboard::Keys key ) const = 0;
		virtual Vector2 GetMousePosition() const = 0;
		virtual GamePad& GetGamePad() const = 0;
		virtual Mouse& GetMouse() const = 0;
		virtual Keyboard& GetKeyboard() const = 0;
        // *********************** Input ***********************

        // *********************** Primitives ***********************
		virtual PrimitivePtr CreateBoxPrimitive( const Vector3& dimensions ) = 0;
		virtual PrimitivePtr CreateSpherePrimitive( const float radius ) = 0;
		virtual void RenderPrimitive ( const PrimitivePtr& primitive, const Vector3& scale, const Vector3& position,
									   const Vector3& rotation, const FXMVECTOR& color = Colors::White ) = 0;
        // *********************** Primitives ***********************

        // *********************** Texture ***********************
        /** Loading and creating texture/sprite object
         * Supported formats: dds, bmp, jpeg, png, tiff, gif
         * @param path filepath to texture/image
         * @param outTextureSize texture size(width, height) in pixels
         * @return TexturePtr pointer to texture
         */
        virtual TexturePtr CreateTexture( const std::wstring& path, Vector2* outTextureSize = nullptr ) = 0;
		virtual void RenderTexture( const TexturePtr& texture, const Vector2& position, const FXMVECTOR& color = Colors::White, const float rotation = 0.0f, const Vector2& origin = Vector2::Zero, const float scale = 1.0f, SpriteEffects effects = SpriteEffects::SpriteEffects_None ) = 0;
        // *********************** Primitives ***********************

        // *********************** Models ***********************
        /** Loading and creating model/mesh object
         * Supported formats: sdkmesh, cmo, vbo
         * @param path filepath to model
         * @return ModelPtr pointer to model
         */
		virtual ModelPtr LoadModel( const std::wstring& path ) = 0;
		virtual void RenderModel( const ModelPtr& model, const Vector3& scale, const Vector3& position, const Vector3& rotation ) = 0;
        // *********************** Models ***********************

        // *********************** Text ***********************
		virtual void RenderText( const std::string& text, const Vector2& position, float scale, const FXMVECTOR& color = Colors::White ) = 0;
        // *********************** Text ***********************

        // *********************** Window ***********************
        virtual Vector2 GetWindowSize() = 0;
        // *********************** Window ***********************
    };
}