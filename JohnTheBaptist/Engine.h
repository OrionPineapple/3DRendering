#pragma once
const float PI = 3.14159265359f;
#include "EngineIO.h"
#include "InstanceHeirachy.h"
#include "Texture.h"
#include <string>
#include "Mesh.h"
#include <mutex>
#include <iostream>
#include <map>

const int DefaultHeight = 400;
const int DefaultWidth = 400;

class Engine 
{
private:
    //Render Stuff
	olc::PixelGameEngine EngineController;
	float RunTime;
	std::vector<float> DepthBuffer;
    Vector2D PreviousMouseCoordinates;

    //Instance Stuff
    InstanceHeirachy::Camera* Camera;
    InstanceHeirachy::World World;
    InstanceHeirachy::SubWorld* WorldRoot;

public:
	Engine(float FoV = PI * 0.5f)
	{
		RunTime = 0.0f;
        Camera = new InstanceHeirachy::Camera(&World, FoV, &EngineController);
        WorldRoot = new InstanceHeirachy::SubWorld(&World);
	}

	bool Start(std::string Name, int Height = DefaultHeight, int Width = DefaultWidth)
	{
		EngineController.sAppName = Name;

		EngineController.OnUserCreate = std::bind(&Engine::OnEngineCreate, this);
		EngineController.OnUserUpdate = std::bind(&Engine::OnUpdate, this, std::placeholders::_1);
		EngineController.OnUserDestroy = std::bind(&Engine::OnEngineDestroy, this);

		if (!EngineController.Construct(Height, Width, 1, 1))
		{
			return false;
		}

		for (int i = 0; i < Height * Width; i++)
		{
			DepthBuffer.push_back(Camera->GetMaxinumDepth());
		}

		EngineController.Start();
	}
private:
	bool OnEngineCreate()
	{
		return OnStart();
	}

	void Draw(int x, int y)
	{
		EngineController.Draw(x, y);
	}

    void Draw(int x, int y, float r, float g, float b)
    {
        EngineController.Draw(x, y, olc::PixelF(r, g, b));
    }

public:
    void Draw(int x, int y, ColourRGB Colour)
    {
        EngineController.Draw(x, y, olc::PixelF((float)Colour.GetRed() / (float)255, (float)Colour.GetGreen() / (float)255, (float)Colour.GetBlue() / (float)255));
    }

private:
	void Draw(int x, int y, float brightness)
	{
        //EngineController.Draw(x, y, olc::PixelF(1.0f, brightness, brightness, 1.0f));
		EngineController.Draw(x, y, olc::PixelF(brightness, brightness, brightness, 1.0f));
		//EngineController.Draw(x, y, olc::PixelF(1.0f - brightness, brightness, 0.5f + brightness * 0.5f, 1.0f));
	}

	float Interpolate(float from, float to, float alpha)
	{
		return (to - from) * alpha + from;
	}

	void FillTriangle(Triangle& ProjectedTriangle, Vector3D A, Vector3D B, Vector3D C, float q)
	{
		float width = (float)Camera->GetScreenWidth();
		float height = (float)Camera->GetScreenHeight();

		int x_a, x_b, x_c, y_a, y_b, y_c;
		float z_a, z_b, z_c;
		x_a = (A.GetX() + 1.0f) * 0.5f * width;
		x_b = (B.GetX() + 1.0f) * 0.5f * width;
		x_c = (C.GetX() + 1.0f) * 0.5f * width;

		y_a = (A.GetY() + 1.0f) * 0.5f * height;
		y_b = (B.GetY() + 1.0f) * 0.5f * height;
		y_c = (C.GetY() + 1.0f) * 0.5f * height;

		z_a = -A.GetW();
		z_b = -B.GetW();
		z_c = -C.GetW();

		if (y_a > y_b)
		{
			std::swap(x_a, x_b);
			std::swap(y_a, y_b);
			std::swap(z_a, z_b);
		}

		if (y_b > y_c)
		{
			std::swap(x_c, x_b);
			std::swap(y_c, y_b);
			std::swap(z_c, z_b);
		}

		if (y_a > y_b)
		{
			std::swap(x_a, x_b);
			std::swap(y_a, y_b);
			std::swap(z_a, z_b);
		}

		if (y_a == y_b)
		{
			FillTriangleFromFlat(
				x_a, y_a, z_a, 
				x_b, y_b, z_b, 
				x_c, y_c, z_c, q);
		}
		else if (y_b == y_c)
		{
			FillTriangleFromFlat(
				x_c, y_c, z_c,
				x_b, y_b, z_b,
				x_a, y_a, z_a, q);
		}
		else 
		{
			//split triangle at height y_b
			int y_k = y_b;
			float alpha = ((float)y_b - (float)y_a) / ((float)y_c - (float)y_a);

			int x_k = Interpolate(x_a, x_c, alpha);
			float z_k = Interpolate(z_a, z_c, alpha);

			FillTriangleFromFlat(
				x_k, y_k, z_k,
				x_b, y_b, z_b,
				x_a, y_a, z_a, q);
			FillTriangleFromFlat(
				x_b, y_b, z_b,
				x_k, y_k, z_k,
				x_c, y_c, z_c, q);
			//b,k is inline
		}

        //PlacePoint(x_a, y_a, 1.0f);
        //PlacePoint(x_b, y_b, 1.0f);
        //PlacePoint(x_c, y_c, 1.0f);
	}

	void FillTriangleWithNormals(Triangle& ProjectedTriangle, Vector3D& A, Vector3D& B, Vector3D& C, Vector3D& LightDirection)
	{
		Vector3D NA = ProjectedTriangle.GetVertexNormalA();
		Vector3D NB = ProjectedTriangle.GetVertexNormalB();
		Vector3D NC = ProjectedTriangle.GetVertexNormalC();

		float width = (float)Camera->GetScreenWidth();
		float height = (float)Camera->GetScreenHeight();

		int x_a, x_b, x_c, y_a, y_b, y_c;
		float z_a, z_b, z_c;
		x_a = (A.GetX() + 1.0f) * 0.5f * width;
		x_b = (B.GetX() + 1.0f) * 0.5f * width;
		x_c = (C.GetX() + 1.0f) * 0.5f * width;

		y_a = (A.GetY() + 1.0f) * 0.5f * height;
		y_b = (B.GetY() + 1.0f) * 0.5f * height;
		y_c = (C.GetY() + 1.0f) * 0.5f * height;

		z_a = -A.GetW();
		z_b = -B.GetW();
		z_c = -C.GetW();

		if (y_a > y_b)
		{
			std::swap(x_a, x_b);
			std::swap(y_a, y_b);
			std::swap(z_a, z_b);
			std::swap(NA, NB);
		}

		if (y_b > y_c)
		{
			std::swap(x_c, x_b);
			std::swap(y_c, y_b);
			std::swap(z_c, z_b);
			std::swap(NC, NB);
		}

		if (y_a > y_b)
		{
			std::swap(x_a, x_b);
			std::swap(y_a, y_b);
			std::swap(z_a, z_b);
			std::swap(NA, NB);
		}

		if (y_a == y_b)
		{
			FillTriangleFromFlatWithNormals(
				x_a, y_a, z_a, NA,
				x_b, y_b, z_b, NB,
				x_c, y_c, z_c, NC,
				LightDirection);
		}
		else if (y_b == y_c)
		{
			FillTriangleFromFlatWithNormals(
				x_c, y_c, z_c, NC,
				x_b, y_b, z_b, NB,
				x_a, y_a, z_a, NA,
				LightDirection);
		}
		else
		{
			//split triangle at height y_b
			int y_k = y_b;
			float alpha = ((float)y_b - (float)y_a) / ((float)y_c - (float)y_a);

			int x_k = Interpolate(x_a, x_c, alpha);
			float z_k = Interpolate(z_a, z_c, alpha);
			Vector3D NK = Vector3D::Interpolate(NA, NC, alpha);

			FillTriangleFromFlatWithNormals(
				x_k, y_k, z_k, NK,
				x_b, y_b, z_b, NB,
				x_a, y_a, z_a, NA,
				LightDirection);
			FillTriangleFromFlatWithNormals(
				x_b, y_b, z_b, NB,
				x_k, y_k, z_k, NK,
				x_c, y_c, z_c, NC,
				LightDirection);

			//b,k is inline
		}
	}

	void FillTriangleFromFlatWithNormals(int& x_a, int& y_a, float& z_a, Vector3D& NA, int& x_b, int& y_b, float& z_b, Vector3D& NB, int& x_c, int& y_c, float& z_c, Vector3D& NC, Vector3D& LightDirection)
	{
		//y_a == y_b
		int direction_y = 1;
		if (y_c < y_a)
		{
			direction_y = -1;
		}
		for (int y = y_a; y != y_c; y += direction_y)
		{
			float alpha = ((float)y - (float)y_a) / ((float)y_c - (float)y_a);

			int start_x = Interpolate(x_a, x_c, alpha);
			int end_x = Interpolate(x_b, x_c, alpha);

			float start_z = Interpolate(z_a, z_c, alpha);
			float end_z = Interpolate(z_b, z_c, alpha);

			Vector3D start_n = Vector3D::Interpolate(NA, NC, alpha);
			Vector3D end_n = Vector3D::Interpolate(NB, NC, alpha);

			int direction_x = 1;
			if (end_x < start_x)
			{
				direction_x = -1;
			}

			for (int x = start_x; x != end_x + direction_x; x += direction_x)
			{
				float beta = ((float)x - (float)start_x) / ((float)end_x - (float)start_x);
				float z = Interpolate(start_z, end_z, beta);

				if (GetDepthBufferAt(x, y) > z)
				{
					Vector3D n = Vector3D::Normalise(Vector3D::Interpolate(start_n, end_n, beta));
					float q = Vector3D::Dot(n, LightDirection);
					if (q < 0.1f) { q = 0.1f; }

					SetDepthBufferAt(x, y, z);
					Draw(x, y, q);
				}
			}
		}
	}

	void PlacePoint(int x, int y, float bright = 0.0f)
	{
		int Spread = 3;
		for (int i = x - Spread; i <= x + Spread; i++)
		{
			for (int j = y - Spread; j <= y + Spread; j++)
			{
				Draw(i, j, bright);
			}
		}
	}

	void FillTriangleFromFlat(int x_a, int y_a, float z_a, int x_b, int y_b, float z_b, int x_c, int y_c, float z_c, float q)
	{
		//y_a == y_b
		int direction_y = 1;
		if (y_c < y_a)
		{
			direction_y = -1;
		}

		for (int y = y_a; y != y_c; y += direction_y)
		{
			float alpha = ((float)y - (float)y_a) / ((float)y_c - (float)y_a);

			int start_x = Interpolate(x_a, x_c, alpha);
			int end_x = Interpolate(x_b, x_c, alpha);

			float start_z = Interpolate(z_a, z_c, alpha);
			float end_z = Interpolate(z_b, z_c, alpha);

			int direction_x = 1;
			if (end_x < start_x)
			{
				direction_x = -1;
			}
			
			for (int x = start_x; x != end_x + direction_x; x += direction_x)
			{
				float beta = ((float)x - (float)start_x) / ((float)end_x - (float)start_x);
				float z = Interpolate(start_z, end_z, beta);
				
				if (GetDepthBufferAt(x, y) > z)
				{
					SetDepthBufferAt(x, y, z);
					Draw(x, y, q);
				}
			}
		}
	}

	void ProjectAndDraw(Triangle& Tri, Matrix4x4& Projection, Matrix4x4& InverseRotate, Vector3D Normal, Vector3D Centre)
	{
		Vector3D ToLight = Vector3D::Normalise(InverseRotate * (Camera->ExtractVectorPosition() - Centre));
		Vector3D A, B, C;
		Triangle::Project(Tri, Projection, A, B, C);

		if (Tri.HasVertexNormals())
		{
			FillTriangleWithNormals(Tri, A, B, C, ToLight);
		}
		else
		{
			//ToLight = Vector3D::Normalise(Camera->ExtractVectorPosition() - Centre);
            ToLight = Vector3D::Normalise(Vector3D(2, 2, 2));
			float light_value = Vector3D::Dot(ToLight, Normal);
			if (light_value < 0.1f) { light_value = 0.1f; }
			FillTriangle(Tri, A, B, C, light_value);
		}
	}

	int HowManyPointsBehindPlane(InstanceHeirachy::Plane& CullingPlane, Triangle& Tri) 
	{
        int Count = 0;
		if (Vector3D::Dot(
			Tri.GetPointA()->GetPosition() - CullingPlane.Position,
			CullingPlane.UnitDirection)
			>= 0.0f) 
		{
            Count++;
		}

		if (Vector3D::Dot(
			Tri.GetPointB()->GetPosition() - CullingPlane.Position,
			CullingPlane.UnitDirection)
			>= 0.0f)
		{
            Count++;
		}

		if (Vector3D::Dot(
			Tri.GetPointC()->GetPosition() - CullingPlane.Position,
			CullingPlane.UnitDirection)
			>= 0.0f)
		{
            Count++;
		}

        return Count++;
	}

    float GetIntersectionLineAndPlane(InstanceHeirachy::Plane& Plane, Vector3D A, Vector3D B)
    {
        float Denominator = Vector3D::Dot(B - A, Plane.UnitDirection);
        if (Denominator == 0.0f) { return -1.0f; }
        return -Vector3D::Dot(A - Plane.Position, Plane.UnitDirection) / Denominator;
    }

    void ClipTriangle1Point(std::vector<Triangle>& Triangles, InstanceHeirachy::Plane& Plane, Triangle& Tri)
    {
        Point A = *Tri.GetPointA();
        Point B = *Tri.GetPointB();
        Point C = *Tri.GetPointC();

        float AB = GetIntersectionLineAndPlane(Plane, A.GetPosition(), B.GetPosition());
        float BC = GetIntersectionLineAndPlane(Plane, B.GetPosition(), C.GetPosition());
        float CA = GetIntersectionLineAndPlane(Plane, C.GetPosition(), A.GetPosition());

        if (AB < 0.0f || AB > 1.0f)
        {
            //Create Triangle about C

            Point NewA = Point(C, A, CA);
            Point NewB = Point(B, C, BC);

            if (Tri.HasVertexNormals())
            {
                Vector3D NewNormalA = Vector3D::Interpolate(Tri.GetVertexNormalC(), Tri.GetVertexNormalA(), CA);
                Vector3D NewNormalB = Vector3D::Interpolate(Tri.GetVertexNormalB(), Tri.GetVertexNormalC(), BC);
                Triangle NewTri = Triangle(std::make_shared<Point>(NewA), std::make_shared<Point>(NewB), Tri.GetPointC(), NewNormalA, NewNormalB, Tri.GetVertexNormalC());
                Triangles.push_back(NewTri);
            }
            else
            {
                Triangle NewTri = Triangle(std::make_shared<Point>(NewA), std::make_shared<Point>(NewB), Tri.GetPointC());
                Triangles.push_back(NewTri);
            }
        }
        else if (CA < 0.0f || CA > 1.0f)
        {
            //Create Triangle about B

            Point NewA = Point(A, B, AB);
            Point NewC = Point(B, C, BC);

            if (Tri.HasVertexNormals())
            {
                Vector3D NewNormalA = Vector3D::Interpolate(Tri.GetVertexNormalC(), Tri.GetVertexNormalA(), CA);
                Vector3D NewNormalC = Vector3D::Interpolate(Tri.GetVertexNormalB(), Tri.GetVertexNormalC(), BC);
                Triangle NewTri = Triangle(std::make_shared<Point>(NewA), Tri.GetPointB(), std::make_shared<Point>(NewC), NewNormalA, Tri.GetVertexNormalB(), NewNormalC);
                Triangles.push_back(NewTri);
            }
            else
            {
                Triangle NewTri = Triangle(std::make_shared<Point>(NewA), Tri.GetPointB(), std::make_shared<Point>(NewC));
                Triangles.push_back(NewTri);
            }
        }
        else
        {
            //Create Triangle about A

            Point NewB = Point(A, B, AB);
            Point NewC = Point(C, A, CA);

            if (Tri.HasVertexNormals())
            {
                Vector3D NewNormalB = Vector3D::Interpolate(Tri.GetVertexNormalA(), Tri.GetVertexNormalB(), AB);
                Vector3D NewNormalC = Vector3D::Interpolate(Tri.GetVertexNormalC(), Tri.GetVertexNormalA(), CA);
                Triangle NewTri = Triangle(Tri.GetPointA(), std::make_shared<Point>(NewB), std::make_shared<Point>(NewC), Tri.GetVertexNormalA(), NewNormalB, NewNormalC);
                Triangles.push_back(NewTri);
            }
            else
            {
                Triangle NewTri = Triangle(Tri.GetPointA(), std::make_shared<Point>(NewB), std::make_shared<Point>(NewC));
                Triangles.push_back(NewTri);
            }
        }
    }

    void ClipTriangle2Point(std::vector<Triangle>& Triangles, InstanceHeirachy::Plane& Plane, Triangle& Tri)
    {
        Point A = *Tri.GetPointA();
        Point B = *Tri.GetPointB();
        Point C = *Tri.GetPointC();

        float AB = GetIntersectionLineAndPlane(Plane, A.GetPosition(), B.GetPosition());
        float BC = GetIntersectionLineAndPlane(Plane, B.GetPosition(), C.GetPosition());
        float CA = GetIntersectionLineAndPlane(Plane, C.GetPosition(), A.GetPosition());

        if (AB < 0.0f || AB > 1.0f)
        {
            //Create Triangle without C

            Point X = Point(C, A, CA);
            Point Y = Point(B, C, BC);

            if (Tri.HasVertexNormals())
            {
                Vector3D NormalX = Vector3D::Interpolate(Tri.GetVertexNormalC(), Tri.GetVertexNormalA(), CA);
                Vector3D NormalY = Vector3D::Interpolate(Tri.GetVertexNormalB(), Tri.GetVertexNormalC(), BC);

                Triangle NewTri = Triangle(std::shared_ptr<Point>(new Point(A)), std::shared_ptr<Point>(new Point(B)), std::shared_ptr<Point>(new Point(Y)), Tri.GetVertexNormalA(), Tri.GetVertexNormalB(), NormalY);

                Triangle NewTri2 = Triangle(std::shared_ptr<Point>(new Point(Y)), std::shared_ptr<Point>(new Point(X)), std::shared_ptr<Point>(new Point(A)), NormalY, NormalX, Tri.GetVertexNormalA());

                Triangles.push_back(NewTri);
                Triangles.push_back(NewTri2);
            }
            else
            {
                Triangle NewTri = Triangle(std::shared_ptr<Point>(new Point(A)), std::shared_ptr<Point>(new Point(B)), std::shared_ptr<Point>(new Point(Y)));

                Triangle NewTri2 = Triangle(std::shared_ptr<Point>(new Point(Y)), std::shared_ptr<Point>(new Point(X)), std::shared_ptr<Point>(new Point(A)));

                Triangles.push_back(NewTri);
                Triangles.push_back(NewTri2);
            }
        }
        else if (CA < 0.0f || CA > 1.0f)
        {
            //Create Triangle without B

            Point X = Point(B, C, BC);
            Point Y = Point(A, B, AB);

            if (Tri.HasVertexNormals())
            {
                Vector3D NormalX = Vector3D::Interpolate(Tri.GetVertexNormalB(), Tri.GetVertexNormalC(), BC);
                Vector3D NormalY = Vector3D::Interpolate(Tri.GetVertexNormalA(), Tri.GetVertexNormalB(), AB);

                Triangle NewTri = Triangle(std::shared_ptr<Point>(new Point(C)), std::shared_ptr<Point>(new Point(A)), std::shared_ptr<Point>(new Point(Y)), Tri.GetVertexNormalC(), Tri.GetVertexNormalA(), NormalY);

                Triangle NewTri2 = Triangle(std::shared_ptr<Point>(new Point(Y)), std::shared_ptr<Point>(new Point(X)), std::shared_ptr<Point>(new Point(C)), NormalY, NormalX, Tri.GetVertexNormalC());

                Triangles.push_back(NewTri);
                Triangles.push_back(NewTri2);
            }
            else
            {
                Triangle NewTri = Triangle(std::shared_ptr<Point>(new Point(C)), std::shared_ptr<Point>(new Point(A)), std::shared_ptr<Point>(new Point(Y)));

                Triangle NewTri2 = Triangle(std::shared_ptr<Point>(new Point(Y)), std::shared_ptr<Point>(new Point(X)), std::shared_ptr<Point>(new Point(C)));

                Triangles.push_back(NewTri);
                Triangles.push_back(NewTri2);
            }
        }
        else
        {
            //Create Triangle without A

            Point X = Point(A, B, AB);
            Point Y = Point(C, A, CA);

            if (Tri.HasVertexNormals())
            {
                Vector3D NormalX = Vector3D::Interpolate(Tri.GetVertexNormalA(), Tri.GetVertexNormalB(), AB);
                Vector3D NormalY = Vector3D::Interpolate(Tri.GetVertexNormalC(), Tri.GetVertexNormalA(), CA);
                Triangle NewTri = Triangle(std::shared_ptr<Point>(new Point(B)), std::shared_ptr<Point>(new Point(C)), std::shared_ptr<Point>(new Point(Y)), Tri.GetVertexNormalB(), Tri.GetVertexNormalC(), NormalY);

                Triangle NewTri2 = Triangle(std::shared_ptr<Point>(new Point(Y)), std::shared_ptr<Point>(new Point(X)), std::shared_ptr<Point>(new Point(B)), NormalY, NormalX, Tri.GetVertexNormalB());

                Triangles.push_back(NewTri);
                Triangles.push_back(NewTri2);
            }
            else
            {
                Triangle NewTri = Triangle(std::shared_ptr<Point>(new Point(B)), std::shared_ptr<Point>(new Point(C)), std::shared_ptr<Point>(new Point(Y)));
                Triangle NewTri2 = Triangle(std::shared_ptr<Point>(new Point(Y)), std::shared_ptr<Point>(new Point(X)), std::shared_ptr<Point>(new Point(B)));

                Triangles.push_back(NewTri);
                Triangles.push_back(NewTri2);
            }
        }
    }

    std::vector<Triangle> CullTriangles(std::vector<Triangle>& Triangles, InstanceHeirachy::Plane& CullingPlane)
    {
        std::vector<Triangle> NewTriangles;
        for (int i = 0; i < Triangles.size(); i++)
        {
            switch (HowManyPointsBehindPlane(CullingPlane, Triangles[i]))
            {
            case 3:
                NewTriangles.push_back(Triangles[i]);
                break;
            case 0:
                break;
            case 1:
                ClipTriangle1Point(NewTriangles, CullingPlane, Triangles[i]);
                break;
            case 2:
                ClipTriangle2Point(NewTriangles, CullingPlane, Triangles[i]);
                break;
            default:
                break;
            }
        }
        return NewTriangles;
    }

	bool OnUpdate(float DeltaTime)
	{
        //Standard Reset of all important frame data
		RunTime += DeltaTime;
		EngineController.Clear(olc::Pixel(0, 0, 0, 0));
		std::fill(DepthBuffer.begin(), DepthBuffer.end(), Camera->GetMaxinumDepth());
        
        //allows the client to move objects, handle inputs etc before rendering
        PreFrame(DeltaTime);

        //Find all applicable rendarable objects
        InstanceHeirachy::InstanceSearchParameter SearchParameter;
        //SearchParameter.SetEnabled(true);
        //SearchParameter.SetBaseType(InstanceHeirachy::InstanceBaseType::MatrixInstanceBaseType);
        //SearchParameter.SetType(InstanceHeirachy::InstanceType::MeshInstanceType);
        std::vector<InstanceHeirachy::Instance*> RenderableChildren = WorldRoot->GetAllChildMatchingState(SearchParameter);
        //std::cout << RenderableChildren.size();

        //Get Camera Matrices
        Vector3D CamPos = Camera->ExtractVectorPosition();
        Matrix4x4 CameraTranslation = Camera->ExtractTranslationMatrix();
        Matrix4x4 InverseCameraTranslation = CameraTranslation.Inverse();

        Matrix4x4 CameraRotationMatrix = Camera->ExtractRotationMatrix();
        Matrix4x4 InverseCameraRotation = CameraRotationMatrix.Inverse();

        Matrix4x4 Projection = Matrix4x4::CreateProjectionMatrix(Camera->GetAspectRatio(), Camera->GetVerticalFoV(), Camera->GetMininumDepth(), Camera->GetMaxinumDepth());

        //Render each object
        for (InstanceHeirachy::Instance* InstanceToRender : RenderableChildren)
        {
            InstanceHeirachy::MeshInstance* MeshToRender = dynamic_cast<InstanceHeirachy::MeshInstance*>(InstanceToRender);
            

            Matrix4x4 Translate = MeshToRender->ExtractTranslationMatrix();
            Matrix4x4 InverseTranslate = Translate.Inverse();

            Matrix4x4 Rotate = MeshToRender->ExtractRotationMatrix();
            Matrix4x4 InverseRotate = Rotate.Inverse();

            Matrix4x4 Transform = MeshToRender->GetMatrix();
            Matrix4x4 InverseTransform = Transform.Inverse();
            //Matrix4x4 InverseTransform = InverseRotate * InverseTranslate;

            Matrix4x4 FinalProjection = Projection * InverseCameraRotation * InverseCameraTranslation * Transform;
            std::vector<InstanceHeirachy::Plane> CullingPlanes = Camera->GetCullingPlanes(InverseTransform, InverseRotate);

            std::shared_ptr<Mesh> MeshRef = MeshToRender->GetReferenceToMesh();
            std::vector<std::shared_ptr<Triangle>> TriangleArray = *(MeshRef->GetTriangleReferences());
            for (int i = 0; i < TriangleArray.size(); i++)
            {
                std::shared_ptr<Triangle> Tri = TriangleArray[i];

                Vector3D Centre = Transform * Tri->GetCentre();
                Vector3D ToCamera = CamPos - Centre;
                Vector3D Normal = Rotate * Tri->GetNormal();
                float dot = Vector3D::Dot(ToCamera, Normal);

                if (dot <= 0.0f) { continue; }

                std::vector<Triangle> TrianglesToRender;
                TrianglesToRender.push_back(Triangle(*Tri));

                for (InstanceHeirachy::Plane CullingPlane : CullingPlanes)
                {
                    TrianglesToRender = CullTriangles(TrianglesToRender, CullingPlane);
                }

                for (Triangle RenderableTri : TrianglesToRender)
                {
                    ProjectAndDraw(RenderableTri, FinalProjection, InverseRotate, Normal, RenderableTri.GetCentre());
                }

            }
        }

        bool Result = PostFrame(DeltaTime);
        PreviousMouseCoordinates = GetMouseCoordinates();
        return Result;
	}

	bool OnEngineDestroy()
	{
		return OnDestroy();
	}

public:
    virtual void PreFrame(float DeltaTime) { };
	virtual bool PostFrame(float DeltaTime) { return true; };
	virtual bool OnStart() { return true; };
	virtual bool OnDestroy() { return true; };

public:
    float GetRunTime()
    {
        return RunTime;
    }

	void AlterScreen(int NewHeight, int NewWidth)
	{
		EngineController.SetScreenSize(NewHeight, NewWidth);
	}

	void SetMaxFPS(int FPS)
	{
		EngineController.SetMaxFPS(FPS);
	}

	void SetMaxFPS()
	{
		//Uncapped FPS
		EngineController.SetMaxFPS(-1);
	}

	int GetMaxFPS()
	{
		return EngineController.GetMaxFPS();
	}

    bool IsKeyDown(olc::Key Key)
    {
        return EngineController.GetKey(Key).bHeld;
    }

    bool IsKeyPressed(olc::Key Key)
    {
        return EngineController.GetKey(Key).bPressed;
    }

    bool IsKeyReleased(olc::Key Key)
    {
        return EngineController.GetKey(Key).bReleased;
    }

    Vector2D GetMouseCoordinates()
    {
        return Vector2D(EngineController.GetMouseX(), EngineController.GetMouseY());
    }

    Vector2D GetMouseDelta()
    {
        return GetMouseCoordinates() - PreviousMouseCoordinates;
    }

    int GetScrollWheelDelta()
    {
        return EngineController.GetMouseWheel();
    }

public:
	float GetDepthBufferAt(int x, int y)
	{
		/*if (x < 0) { return -1.0f; }
		if (y < 0) { return -1.0f; }
		if (x > Camera->GetScreenWidth()) { return -1.0f; }
		if (y > Camera->GetScreenHeight()) { return -1.0f; }*/
		return DepthBuffer[x + (Camera->GetScreenWidth() * y)];
	}

private:
	void SetDepthBufferAt(int x, int y, float z)
	{
		DepthBuffer[x + (Camera->GetScreenWidth() * y)] = z;
	}

public:
    //Instance Changing Stuff

    InstanceHeirachy::World* GetWorld()
    {
        return &World;
    }

    InstanceHeirachy::Instance* GetWorldRoot()
    {
        return WorldRoot;
    }


    InstanceHeirachy::Camera* GetCamera()
    {
        return Camera;
    }
};