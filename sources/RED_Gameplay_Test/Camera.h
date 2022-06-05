/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#pragma once

class Camera
{

public:
	Camera();
	~Camera();
	void OnUpdate( float deltaTime, DirectX::Keyboard& keyboard, DirectX::Mouse& mouse, DirectX::GamePad& gamepad );
	const Vector3& GetPosition() const;
	const Vector3& GetViewDirection() const;

private:
	void RotationInput( DirectX::Mouse& mouse, DirectX::GamePad& gamepad );
	Vector3 MovementInput( DirectX::Keyboard& keyboard, DirectX::GamePad& gamepad );

	Vector3 m_cameraPos;
	Vector3 m_viewDirection;
	float m_pitch;
	float m_yaw;

};

