#pragma once
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "Point.h"
#include "Texture.h"
#include <mutex>

class Triangle
{
protected:
	std::shared_ptr<Point> A;
	std::shared_ptr<Point> B;
	std::shared_ptr<Point> C;

	bool VertexNormalsEnabled = false;
	Vector3D NormalA;
	Vector3D NormalB;
	Vector3D NormalC;

private:
	Triangle()
	{

	}

public:
	Triangle(std::shared_ptr<Point> ARef, std::shared_ptr<Point> BRef, std::shared_ptr<Point> CRef)
	{
		A = std::shared_ptr<Point>(ARef);
		B = std::shared_ptr<Point>(BRef);
		C = std::shared_ptr<Point>(CRef);
		VertexNormalsEnabled = false;
	}

	Triangle(std::shared_ptr<Point> ARef, std::shared_ptr<Point> BRef, std::shared_ptr<Point> CRef, Vector3D NA, Vector3D NB, Vector3D NC)
	{
		A = std::shared_ptr<Point>(ARef);
		B = std::shared_ptr<Point>(BRef);
		C = std::shared_ptr<Point>(CRef);

		VertexNormalsEnabled = true;
		NormalA = Vector3D::Normalise(NA);
		NormalB = Vector3D::Normalise(NB);
		NormalC = Vector3D::Normalise(NC);
	}

public:
	static void Project(const Triangle Pre, Matrix4x4 Projection, Vector3D &A, Vector3D &B, Vector3D &C)
	{
		A = Matrix4x4::ProjectPoint(Pre.A->GetPosition(), Projection);
		B = Matrix4x4::ProjectPoint(Pre.B->GetPosition(), Projection);
		C = Matrix4x4::ProjectPoint(Pre.C->GetPosition(), Projection);
	}

public:
	void SetVertexNormals(Vector3D NA, Vector3D NB, Vector3D NC)
	{
		VertexNormalsEnabled = true;
		NormalA = NA;
		NormalB = NB;
		NormalC = NC;
	}

	void SetVertexNormals()
	{
		VertexNormalsEnabled = false;
	}

	bool HasVertexNormals()
	{
		return VertexNormalsEnabled;
	}

	Vector3D GetVertexNormalA()
	{
		return NormalA;
	}

	Vector3D GetVertexNormalB()
	{
		return NormalB;
	}

	Vector3D GetVertexNormalC()
	{
		return NormalC;
	}

public:
	std::shared_ptr<Point> GetPointA()
	{
		return A;
	}

	std::shared_ptr<Point> GetPointB()
	{
		return B;
	}

	std::shared_ptr<Point> GetPointC()
	{
		return C;
	}

public:
	Vector3D GetCentre()
	{
		return (A->GetPosition() + B->GetPosition() + C->GetPosition()) / 3.0f;
	}

	Vector3D GetNormal()
	{
		Vector3D Normal = Vector3D::Normalise(Vector3D::Cross(B->GetPosition() - A->GetPosition(), C->GetPosition() - A->GetPosition()));
		Normal.SetW(0.0f);
		return Normal;
	}
};