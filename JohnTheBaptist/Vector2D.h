#pragma once
#include <math.h>

class Vector2D
{
protected: 
	float X;
	float Y;
	float W;

	//Constructors
	//Note that W is for matrix multplication purposes
	//and isn't used for standard vector oporations
public:
	Vector2D()
	{
		X = 0.0f;
		Y = 0.0f;
		W = 1.0f;
	}

	Vector2D(float newX, float newY)
	{
		X = newX;
		Y = newY;
		W = 1.0f;
	}

	Vector2D(float newX, float newY, float newW)
	{
		X = newX;
		Y = newY;
		W = newW;
	}

public:
	float GetX()
	{
		return X;
	}

	float GetY()
	{
		return Y;
	}

	float GetW()
	{
		return W;
	}

	//Standard Vector Oporation
public:
	float GetMagnitude()
	{
		float SquaredSums = (float)(X * X) + (float)(Y * Y);
		return sqrtf(SquaredSums);
	}

	void SetMagnitude(float Magnitude)
	{
		float Current = GetMagnitude();
		float Multiplier = Magnitude / Current;
		X *= Multiplier;
		Y *= Multiplier;
		W *= Multiplier;
	}
	static Vector2D Normalise(Vector2D Vector)
	{
		float Magnitude = Vector.GetMagnitude();
		return Vector2D(
			Vector.X / Magnitude,
			Vector.Y / Magnitude,
			Vector.W / Magnitude);
	}

	static Vector2D Interpolate(Vector2D From, Vector2D To, float Alpha)
	{
		return From + ((To - From) * Alpha);
	}

	static float Dot(Vector2D A, Vector2D B)
	{
		return (A.X * B.X) + (A.Y * B.Y);
	}


	//Arithmetic Functions
public:
	static Vector2D Multiply(Vector2D A, float Scalar)
	{
		return Vector2D(
			A.X * Scalar,
			A.Y * Scalar);
	}

	static Vector2D Divide(Vector2D A, float Scalar)
	{
		return Vector2D(
			A.X / Scalar,
			A.Y / Scalar);
	}

	static Vector2D Add(Vector2D A, Vector2D B)
	{
		return Vector2D(
			A.X + B.X,
			A.Y + B.Y);
	}

	static Vector2D Subtract(Vector2D A, Vector2D B)
	{
		return Vector2D(
			A.X - B.X,
			A.Y - B.Y);
	}

	//operator overiding
public:
    bool operator == (Vector2D Vector)
    {
        return (this->X == Vector.X && this->Y == Vector.Y);
    }

	Vector2D operator * (float Scalar)
	{
		return Multiply(*this, Scalar);
	}

	Vector2D operator / (float Scalar)
	{
		return Divide(*this, Scalar);
	}

	Vector2D operator + (Vector2D B)
	{
		return Add(*this, B);
	}

	Vector2D operator - (Vector2D B)
	{
		return Subtract(*this, B);
	}

	Vector2D operator -()
	{
		return Vector2D(-this->X, -this->Y);
	}

	Vector2D& operator *= (float Scalar)
	{
		this->X *= Scalar;
		this->Y *= Scalar;
		return *this;
	}

	Vector2D& operator /= (float Scalar)
	{
		this->X /= Scalar;
		this->Y /= Scalar;
		return *this;
	}

	Vector2D& operator += (Vector2D B)
	{
		this->X += B.X;
		this->Y += B.Y;
		return *this;
	}

	Vector2D& operator -= (Vector2D B)
	{
		this->X -= B.X;
		this->Y -= B.Y;
		return *this;
	}
};

