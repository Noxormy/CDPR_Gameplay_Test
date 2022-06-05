/**
* Copyright (c) 2021 CD Projekt Red. All Rights Reserved.
*/

#include "pch.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

// define fixture
////////////////////////////////////////////////

class VectorTest : public ::testing::Test
{

protected:

    void SetUp() override
    {
        a = Vector3{ 1.0f, 2.0f, 3.0f };
        b = Vector3{ 4.0f, 5.0f, 6.0f };
    }

    void TearDown() override
    {
    }

    Vector3 a;
    Vector3 b;
};

// test cases
////////////////////////////////////////////////

TEST_F( VectorTest, Add )
{
    const Vector3 result = a + b;

    ASSERT_FLOAT_EQ( result.x, 5.0f );
    ASSERT_FLOAT_EQ( result.y, 7.0f );
    ASSERT_FLOAT_EQ( result.z, 9.0f );
}

TEST_F( VectorTest, Substract )
{
    const Vector3 result = b - a;

    ASSERT_FLOAT_EQ( result.x, 3.0f );
    ASSERT_FLOAT_EQ( result.y, 3.0f );
    ASSERT_FLOAT_EQ( result.z, 3.0f );
}

TEST_F( VectorTest, Multiply )
{
    const Vector3 result = a * 2.0f;

    ASSERT_FLOAT_EQ( result.x, 2.0f );
    ASSERT_FLOAT_EQ( result.y, 4.0f );
    ASSERT_FLOAT_EQ( result.z, 6.0f );
}

TEST_F( VectorTest, DotProduct )
{
    const float result = a.Dot( b );

    ASSERT_FLOAT_EQ( result, 32.0f );
}

TEST_F( VectorTest, CrossProduct )
{
    const Vector3 result = a.Cross( b );

    ASSERT_FLOAT_EQ( result.x, -3.0f );
    ASSERT_FLOAT_EQ( result.y, 6.0f );
    ASSERT_FLOAT_EQ( result.z, -3.0f );
}