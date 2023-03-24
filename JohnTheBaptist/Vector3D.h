#pragma once
#include <math.h>

class Vector3D
{
protected:
	float X;
	float Y;
	float Z;
	float W;

	//Constructors
	//Note that W is for matrix multplication purposes
	//and isn't used for standard vector oporations
public:
	Vector3D()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
		W = 1.0f;
	}

	Vector3D(float newX, float newY, float newZ)
	{
		X = newX;
		Y = newY;
		Z = newZ;
		W = 1.0f;
	}

	Vector3D(float newX, float newY, float newZ, float newW)
	{
		X = newX;
		Y = newY;
		Z = newZ;
		W = newW;
	}

	static void DebugVector3D(Vector3D& Vector)
	{
		std::cout << "Vector3D:" << std::endl;
		std::cout << Vector.X << std::endl << Vector.Y << std::endl << Vector.Z << std::endl << Vector.W << std::endl;
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

	float GetZ()
	{
		return Z;
	}

	float GetW()
	{
		return W;
	}

	void SetW(float w)
	{
		W = w;
	}

	//Standard Vector Oporation
public:
	float GetMagnitude()
	{
		float SquaredSums = (float)(X * X) + (float)(Y * Y) + (float)(Z * Z);
		return sqrtf(SquaredSums);
	}

	void SetMagnitude(float Magnitude)
	{
		float Current = GetMagnitude();
		float Multiplier = Magnitude / Current;
		X *= Multiplier;
		Y *= Multiplier;
		Z *= Multiplier;
		W *= Multiplier;
	}
	static Vector3D Normalise(Vector3D Vector)
	{
		float Magnitude = Vector.GetMagnitude();
		return Vector3D(
			Vector.X / Magnitude,
			Vector.Y / Magnitude,
			Vector.Z / Magnitude,
			Vector.W / Magnitude);
	}

	static Vector3D Interpolate(Vector3D From, Vector3D To, float Alpha)
	{
		return From + ((To - From) * Alpha);
	}

	static float Dot(Vector3D A, Vector3D B)
	{
		return (A.X * B.X) + (A.Y * B.Y) + (A.Z * B.Z);
	}

	static Vector3D Cross(Vector3D A, Vector3D B)
	{
		return Vector3D(
			A.Y * B.Z - B.Y * A.Z,
			A.Z * B.X - B.Z * A.X,
			A.X * B.Y - B.X * A.Y,
			0.0f);
	}


	//Arithmetic Functions
public:
	static Vector3D Multiply(Vector3D A, float Scalar)
	{
		return Vector3D(
			A.X * Scalar,
			A.Y * Scalar,
			A.Z * Scalar);
	}

	static Vector3D Divide(Vector3D A, float Scalar)
	{
		return Vector3D(
			A.X / Scalar,
			A.Y / Scalar,
			A.Z / Scalar);
	}

	static Vector3D Add(Vector3D A, Vector3D B)
	{
		return Vector3D(
			A.X + B.X,
			A.Y + B.Y,
			A.Z + B.Z);
	}

	static Vector3D Subtract(Vector3D A, Vector3D B)
	{
		return Vector3D(
			A.X - B.X,
			A.Y - B.Y,
			A.Z - B.Z);
	}

	//operator overiding
public:
    bool operator == (Vector3D Vector)
    {
        return (this->X == Vector.X && this->Y == Vector.Y && this->Z == Vector.Z);
    }

	Vector3D operator * (float Scalar)
	{
		return Multiply(*this, Scalar);
	}

	Vector3D operator / (float Scalar)
	{
		return Divide(*this, Scalar);
	}

	Vector3D operator + (Vector3D B)
	{
		return Add(*this, B);
	}

	Vector3D operator - (Vector3D B)
	{
		return Subtract(*this, B);
	}

	Vector3D operator -()
	{
		return Vector3D(-this->X, -this->Y, -this->Z);
	}

	Vector3D& operator *= (float Scalar)
	{
		this->X *= Scalar;
		this->Y *= Scalar;
		this->Z *= Scalar;
		return *this;
	}

	Vector3D& operator /= (float Scalar)
	{
		this->X /= Scalar;
		this->Y /= Scalar;
		this->Z /= Scalar;
		return *this;
	}

	Vector3D& operator += (Vector3D B)
	{
		this->X += B.X;
		this->Y += B.Y;
		this->Z += B.Z;
		return *this;
	}

	Vector3D& operator -= (Vector3D B)
	{
		this->X -= B.X;
		this->Y -= B.Y;
		this->Z -= B.Z;
		return *this;
	}
};

