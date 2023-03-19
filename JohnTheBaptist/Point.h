#pragma once
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"

class Point 
{
protected:
	Vector3D WorldPoint;
    Vector2D TexturePoint;
    bool DoesHaveTexturePoint = false;

public:
	Point()
	{
		WorldPoint = Vector3D(0, 0, 0);
	}

	Point(Vector3D WorldPosition)
	{
		WorldPoint = Vector3D(WorldPosition);
	}

    Point(Point& A, Point& B, float alpha)
    {
        WorldPoint = Vector3D::Interpolate(A.WorldPoint, B.WorldPoint, alpha);

        if (A.HasTexturePoint() && B.HasTexturePoint())
        {
            TexturePoint = Vector2D::Interpolate(A.TexturePoint, B.TexturePoint, alpha);
        }
    }

public:
	void Transform(Matrix4x4 Matrix)
	{
		WorldPoint = Matrix * WorldPoint;
	}

	void Scale(float Scalar)
	{
		WorldPoint *= Scalar;
	}

	void Translate(Vector3D Translation)
	{
		WorldPoint += Translation;
	}

public:

	void SetTexturePoint(Vector2D Texture)
	{
        DoesHaveTexturePoint = true;
        TexturePoint = Texture;
	}


	void SetTexturePoint()
	{
        DoesHaveTexturePoint = false;
	}

public:
	Vector3D GetPosition()
	{
		return WorldPoint;
	}

	Vector2D GetTexturePoint()
	{
        return TexturePoint;
	}

    bool HasTexturePoint()
    {
        return  DoesHaveTexturePoint;
    }


};