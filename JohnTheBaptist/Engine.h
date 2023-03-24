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
    const std::string PixelReadError = "Cannot get pixel out of bounds of the screen";
    const std::string PixelWriteError = "Cannot draw out of bounds of the screen";
    const std::string DepthReadError = "Cannot get depth out of the bounds of the screen";

private:
    //Render Stuff
	EngineIO::PixelGameEngine EngineController;
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
        WorldRoot->SetName("WorldRoot");
        Camera->SetName("Camera");
	}

	bool Start(std::string Name, int Height = DefaultHeight, int Width = DefaultWidth)
	{
		EngineController.sAppName = Name;

        //connect up our events to the IO that controls the window
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

public:
    void Draw(int x, int y, ColourRGB Colour)
    {
        //check its within the bounds of the screen
        if (x < 0 || y < 0 || x >= Camera->GetScreenWidth() || y >= Camera->GetScreenHeight())
        {
            throw EngineException(PixelWriteError, "The coordinates: (" + std::to_string(x) + ", " + std::to_string(y) + ") are invalid)");
            return;
        }
        EngineController.Draw(x, y, EngineIO::PixelF((float)Colour.GetRed() / (float)255, (float)Colour.GetGreen() / (float)255, (float)Colour.GetBlue() / (float)255));
    }

    ColourRGB GetPixel(int x, int y)
    {
        //check its within the bounds of the screen
        if (x < 0 || y < 0 || x >= Camera->GetScreenWidth() || y >= Camera->GetScreenHeight()) 
        { 
            throw EngineException(PixelWriteError, "The coordinates: (" + std::to_string(x) + ", " + std::to_string(y) + ") are invalid)"); 
            return ColourRGB(); 
        }

        EngineIO::Pixel P = EngineController.GetDrawn(x, y);
        return ColourRGB(P.r, P.g, P.b);
    }

private:
	float Interpolate(float from, float to, float alpha)
	{
		return (to - from) * alpha + from;
	}

	void FillTriangle(Triangle& ProjectedTriangle, Vector3D A, Vector3D B, Vector3D C, Vector3D& CameraPosition, ColourRGB& Ambient, std::vector<InstanceHeirachy::PointLight*>& PointLights, std::vector<InstanceHeirachy::DirectionalLight*>& DirectionalLights, Vector3D& Position, Vector3D& Normal)
	{
        //We must fill this triangle onto the screen

		float width = (float)Camera->GetScreenWidth();
		float height = (float)Camera->GetScreenHeight();

        //tranform to screen space
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

        //first we sorted the points into height order
        //this is so that we can iterate through its pixels easily

		if (y_a == y_b)
		{
            //if two ys are equal, that means this is a flat
            //which is far easier to draw from
			FillTriangleFromFlat(
				x_a, y_a, z_a, 
				x_b, y_b, z_b, 
				x_c, y_c, z_c,
                ProjectedTriangle, CameraPosition, Ambient, PointLights, DirectionalLights, Position, Normal);
		}
		else if (y_b == y_c)
		{
            //if two ys are equal, that means this is a flat
            //which is far easier to draw from
			FillTriangleFromFlat(
				x_c, y_c, z_c,
				x_b, y_b, z_b,
				x_a, y_a, z_a,
                ProjectedTriangle, CameraPosition, Ambient, PointLights, DirectionalLights, Position, Normal);
		}
		else 
		{
			//split triangle at height y_b
			int y_k = y_b;
			float alpha = ((float)y_b - (float)y_a) / ((float)y_c - (float)y_a);

			int x_k = Interpolate(x_a, x_c, alpha);
			float z_k = Interpolate(z_a, z_c, alpha);

            //this triangle does not have a flat base
            //so we split it into halfs that do
            //we must interpolate values for the point k
            //which makes a flat line with b

			FillTriangleFromFlat(
				x_k, y_k, z_k,
				x_b, y_b, z_b,
				x_a, y_a, z_a,
                ProjectedTriangle, CameraPosition, Ambient, PointLights, DirectionalLights, Position, Normal);
			FillTriangleFromFlat(
				x_b, y_b, z_b,
				x_k, y_k, z_k,
				x_c, y_c, z_c,
                ProjectedTriangle, CameraPosition, Ambient, PointLights, DirectionalLights, Position, Normal);
			//b,k is inline
		}
	}

	void FillTriangleWithNormals(Triangle& ProjectedTriangle, Vector3D& A, Vector3D& B, Vector3D& C, Vector3D& CameraPosition, ColourRGB& Ambient, std::vector<InstanceHeirachy::PointLight*>& PointLights, std::vector<InstanceHeirachy::DirectionalLight*>& DirectionalLights, Vector3D& Position)
	{
        //Normals add another layer of confusion
        //Check FillTriangle() first

		Vector3D NA = ProjectedTriangle.GetVertexNormalA();
		Vector3D NB = ProjectedTriangle.GetVertexNormalB();
		Vector3D NC = ProjectedTriangle.GetVertexNormalC();

		float width = (float)Camera->GetScreenWidth();
		float height = (float)Camera->GetScreenHeight();

        //tranform to screen space
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
            //if two ys are equal, that means this is a flat
            //which is far easier to draw from
			FillTriangleFromFlatWithNormals(
				x_a, y_a, z_a, NA,
				x_b, y_b, z_b, NB,
				x_c, y_c, z_c, NC,
				ProjectedTriangle, CameraPosition, Ambient, PointLights, DirectionalLights, Position);
		}
		else if (y_b == y_c)
		{
            //if two ys are equal, that means this is a flat
            //which is far easier to draw from
			FillTriangleFromFlatWithNormals(
				x_c, y_c, z_c, NC,
				x_b, y_b, z_b, NB,
				x_a, y_a, z_a, NA,
				ProjectedTriangle, CameraPosition, Ambient, PointLights, DirectionalLights, Position);
		}
		else
		{
            //this triangle does not have a flat base
            //so we split it into halfs that do
            //we must interpolate values for the point k
            //which makes a flat line with b

			int y_k = y_b;
			float alpha = ((float)y_b - (float)y_a) / ((float)y_c - (float)y_a);

			int x_k = Interpolate(x_a, x_c, alpha);
			float z_k = Interpolate(z_a, z_c, alpha);
			Vector3D NK = Vector3D::Interpolate(NA, NC, alpha);

			FillTriangleFromFlatWithNormals(
				x_k, y_k, z_k, NK,
				x_b, y_b, z_b, NB,
				x_a, y_a, z_a, NA,
				ProjectedTriangle, CameraPosition, Ambient, PointLights, DirectionalLights, Position);
			FillTriangleFromFlatWithNormals(
				x_b, y_b, z_b, NB,
				x_k, y_k, z_k, NK,
				x_c, y_c, z_c, NC,
				ProjectedTriangle, CameraPosition, Ambient, PointLights, DirectionalLights, Position);

			//b,k is inline
		}
	}

	void FillTriangleFromFlatWithNormals(int& x_a, int& y_a, float& z_a, Vector3D& NA, int& x_b, int& y_b, float& z_b, Vector3D& NB, int& x_c, int& y_c, float& z_c, Vector3D& NC, Triangle& Tri, Vector3D& CameraPosition, ColourRGB& Ambient, std::vector<InstanceHeirachy::PointLight*>& PointLights, std::vector<InstanceHeirachy::DirectionalLight*>& DirectionalLights, Vector3D& Position)
	{
        //normals add another layer of confusion
        //start with the method FillTriangleFromFlat

        Material Matt = Tri.GetMaterial();
		//y_a == y_b
		int direction_y = 1;
		if (y_c < y_a)
		{
			direction_y = -1;
		}
		for (int y = y_a; y != y_c; y += direction_y)
		{
            //for each line across the triangle
            //we define our start and end points
            //then we get our values such as depth and normal
            //these too must be interpolated to get our start and end values

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
                //for each x in this flat line
                //we get our depth and normal
				float beta = ((float)x - (float)start_x) / ((float)end_x - (float)start_x);
				float z = Interpolate(start_z, end_z, beta);

                if (x < 0 || y < 0 || x >= Camera->GetScreenWidth() || y >= Camera->GetScreenHeight()) { continue; }
                //check that the depth at this point is greater than our current depth
                //if it was lower then it means there is something infront of this triangle
                //and that we shouldnt draw
				if (GetDepthBufferAt(x, y) > z)
				{
					Vector3D Normal = Vector3D::Normalise(Vector3D::Interpolate(start_n, end_n, beta));

					SetDepthBufferAt(x, y, z);
                    Draw(x, y, ShadePoint(Position, Normal, Matt, CameraPosition, Ambient, PointLights, DirectionalLights));
                    //shade in out point using normals and values
				}
			}
		}
	}

	void FillTriangleFromFlat(int x_a, int y_a, float z_a, int x_b, int y_b, float z_b, int x_c, int y_c, float z_c, Triangle& Tri, Vector3D& CameraPosition, ColourRGB& Ambient, std::vector<InstanceHeirachy::PointLight*>& PointLights, std::vector<InstanceHeirachy::DirectionalLight*>& DirectionalLights, Vector3D& Position, Vector3D& Normal)
	{
        Material Matt = Tri.GetMaterial();
		//y_a == y_b
		int direction_y = 1;
		if (y_c < y_a)
		{
			direction_y = -1;
		}

		for (int y = y_a; y != y_c; y += direction_y)
		{
            //for each line across the triangle
            //we define our start and end points
            //then we get our values such as depth and normal
            //these too must be interpolated to get our start and end values
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
                //for each x in this flat line
                //we get our depth
				float beta = ((float)x - (float)start_x) / ((float)end_x - (float)start_x);
				float z = Interpolate(start_z, end_z, beta);

                if (x < 0 || y < 0 || x >= Camera->GetScreenWidth() || y >= Camera->GetScreenHeight()) { continue; }

                //check that the depth at this point is greater than our current depth
                //if it was lower then it means there is something infront of this triangle
                //and that we shouldnt draw 

				if (GetDepthBufferAt(x, y) > z)
				{
					SetDepthBufferAt(x, y, z);
					Draw(x, y, ShadePoint(Position, Normal, Matt, CameraPosition, Ambient, PointLights, DirectionalLights));
                    //shade in out point using normals and values
				}
			}
		}
	}

    ColourRGB ShadePoint(Vector3D Position, Vector3D Normal, Material& Matt, Vector3D& CameraPos, ColourRGB& Ambient, std::vector<InstanceHeirachy::PointLight*>& PointLights, std::vector<InstanceHeirachy::DirectionalLight*>& DirectionalLights)
    {
        //use phong reflection equation

        Vector3D ToCamera = Vector3D::Normalise(CameraPos - Position);
        Normal = Vector3D::Normalise(Normal);
        ColourRGB PixelAmbient = ColourRGB::Multiply(Ambient, Matt.GetAmbient());
        //apply the normal to the point

        ColourRGB PixelDiffuse = ColourRGB(0, 0, 0);
        ColourRGB PixelSpecular = ColourRGB(0, 0, 0);

        for (InstanceHeirachy::PointLight* PointLight : PointLights)
        {
            Vector3D ToLight = Vector3D::Normalise(PointLight->GetPoint() - Position);
            float Dot = Vector3D::Dot(Normal, ToLight);
            PixelDiffuse = ColourRGB::GetMaxinum(PixelDiffuse, ColourRGB::Multiply(Matt.GetDiffuse(), PointLight->GetLightColour() * Dot));

            Vector3D ReflectedRay = Vector3D::Normalise((Normal * 2 * Dot) - ToLight);
            Dot = Vector3D::Dot(ReflectedRay, ToCamera);
            PixelSpecular = ColourRGB::GetMaxinum(PixelSpecular, ColourRGB::Multiply(Matt.GetSpecular(), PointLight->GetLightColour() * Dot));
            //for each pointlight, apply the specular and diffuse equations
        }

        for (InstanceHeirachy::DirectionalLight* DirectionalLight : DirectionalLights)
        {
            Vector3D ToLight = -DirectionalLight->GetDirection();
            float Dot = Vector3D::Dot(Normal, ToLight);
            PixelDiffuse = ColourRGB::GetMaxinum(PixelDiffuse, ColourRGB::Multiply(Matt.GetDiffuse(), DirectionalLight->GetLightColour() * Dot));

            Vector3D ReflectedRay = Vector3D::Normalise((Normal * 2 * Dot) - ToLight);
            Dot = Vector3D::Dot(ReflectedRay, ToCamera);
            PixelSpecular = ColourRGB::GetMaxinum(PixelSpecular, ColourRGB::Multiply(Matt.GetSpecular(), DirectionalLight->GetLightColour() * Dot));
            //for each directional light, apply the specular and diffuse equations
        }

        return ColourRGB::GetMaxinum(PixelAmbient, ColourRGB::GetMaxinum(PixelSpecular, PixelDiffuse));
    }

	void ProjectAndDraw(Triangle& Tri, Matrix4x4& Projection, Matrix4x4& InverseRotate, Vector3D& Normal, Vector3D& CameraPosition, ColourRGB& Ambient, std::vector<InstanceHeirachy::PointLight*>& PointLights, std::vector<InstanceHeirachy::DirectionalLight*>& DirectionalLights, Vector3D& Position)
	{
        //project our triangle into 2D space
		Vector3D A, B, C;
		Triangle::Project(Tri, Projection, A, B, C);

        if (Tri.HasVertexNormals())
        {
            FillTriangleWithNormals(Tri, A, B, C, CameraPosition, Ambient, PointLights, DirectionalLights, Position);
        }
		else
		{
			FillTriangle(Tri, A, B, C, CameraPosition, Ambient, PointLights, DirectionalLights, Position, Normal);
		}
	}

	int HowManyPointsBehindPlane(InstanceHeirachy::Plane& CullingPlane, Triangle& Tri) 
	{
        //counts how much of a triangle is in or out of a plane
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
        //if a triangle has one point above a plane
        //we find the two intersection points
        //and append a culled triangle to the render list

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
        //we have two points above our plane
        //and two intersection points
        //we must therfore append two triangles

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
        //if a triangle isnt completely within the plane, cut off its points until it is
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

    void RenderSubWorld(std::vector<InstanceHeirachy::SubWorldStackFrame>& Stack, Matrix4x4& Projection, Matrix4x4& InverseCameraRotation, Matrix4x4& InverseCameraTranslation, Vector3D& CameraPos, ColourRGB& Ambient, std::vector<InstanceHeirachy::PointLight*>& PointLights, std::vector<InstanceHeirachy::DirectionalLight*>& DirectionalLights)
    {
        //We must get the ultimate transform matrix
        //from all of the ascending subworlds
        //then get all of the applicable lights (only lights from ascendants)
        //and render each of the mesh instances within


        InstanceHeirachy::SubWorldStackFrame Frame = Stack.at(Stack.size() - 1);

        Matrix4x4 SubWorldOffset;

        for (InstanceHeirachy::SubWorldStackFrame TransformFrame : Stack)
        {
            SubWorldOffset = Frame.Transform * SubWorldOffset;
        }

        for (InstanceHeirachy::MeshInstance* MeshToRender : Frame.Meshes)
        {
            //get the matrix data for our mesh
            Matrix4x4 Transform = MeshToRender->GetMatrix() * SubWorldOffset;
            Matrix4x4 InverseTransform = Transform.Inverse();

            Matrix4x4 Translate = Transform.ExtractTranslationMatrix();
            Matrix4x4 InverseTranslate = Translate.Inverse();

            Matrix4x4 Rotate = Transform.ExtractRotationMatrix();
            Matrix4x4 InverseRotate = Rotate.Inverse();

            //prepare a projection matrix
            Matrix4x4 FinalProjection = Projection * InverseCameraRotation * InverseCameraTranslation * Transform;
            std::vector<InstanceHeirachy::Plane> CullingPlanes = Camera->GetCullingPlanes(InverseTransform, InverseRotate);

            //Get our mesh data
            std::shared_ptr<Mesh> MeshRef = MeshToRender->GetReferenceToMesh();
            std::vector<std::shared_ptr<Triangle>> TriangleArray = *(MeshRef->GetTriangleReferences());

            //for all the triangles
            for (int i = 0; i < TriangleArray.size(); i++)
            {
                std::shared_ptr<Triangle> Tri = TriangleArray[i];

                //check it is visible to the camera
                Vector3D Centre = Transform * Tri->GetCentre();
                Vector3D ToCamera = CameraPos - Centre;
                Vector3D Normal = Rotate * Tri->GetNormal();
                float dot = Vector3D::Dot(ToCamera, Normal);

                if (dot <= 0.0f) { continue; }

                std::vector<Triangle> TrianglesToRender;
                TrianglesToRender.push_back(Triangle(*Tri));

                for (InstanceHeirachy::Plane CullingPlane : CullingPlanes)
                {
                    //cull the triangle so that only visible sections are drawn
                    TrianglesToRender = CullTriangles(TrianglesToRender, CullingPlane);
                }

                for (Triangle RenderableTri : TrianglesToRender)
                {
                    //render the triangle using the correct material
                    RenderableTri.SetMaterial(Tri->GetMaterial());
                    Vector3D Pos = Transform * Centre;
                    Vector3D Nor = Transform.ExtractRotationMatrix() * Normal;
                    ProjectAndDraw(RenderableTri, FinalProjection, InverseRotate, Normal, CameraPos, Ambient, PointLights, DirectionalLights, Pos);
                }

            }
        }
    }

    void GatherLightData(std::vector<InstanceHeirachy::SubWorldStackFrame> Stack, ColourRGB& Ambient, std::vector<InstanceHeirachy::PointLight*>& PointLights, std::vector<InstanceHeirachy::DirectionalLight*>& DirectionalLights)
    {
        //seperate the lights of ambient, point and direction
        //into the applicable vector or light
        //by going through each ascending SubWorld

        Ambient = ColourRGB(0, 0, 0);
        for (InstanceHeirachy::SubWorldStackFrame Frame : Stack)
        {
            for (InstanceHeirachy::Light* Light : Frame.Lights)
            {
                InstanceHeirachy::AmbientLight* AmbientLight;
                InstanceHeirachy::PointLight* PointLight;
                InstanceHeirachy::DirectionalLight* DirectionalLight;


                switch (Light->GetType())
                {
                case InstanceHeirachy::InstanceType::AmbientLightType:
                    //ambient only needs to get the maxinum channel
                    //as it has no position or direction
                    AmbientLight = dynamic_cast<InstanceHeirachy::AmbientLight*>(Light);
                    Ambient = ColourRGB::GetMaxinum(Ambient, AmbientLight->GetLightColour());
                    break;

                case InstanceHeirachy::InstanceType::PointLightType:
                    PointLight = dynamic_cast<InstanceHeirachy::PointLight*>(Light);
                    PointLights.push_back(PointLight);
                    break;

                case InstanceHeirachy::InstanceType::DirectionalLightType:
                    DirectionalLight = dynamic_cast<InstanceHeirachy::DirectionalLight*>(Light);
                    DirectionalLights.push_back(DirectionalLight);
                    break;
                }
            }
        }
    }

	bool OnUpdate(float DeltaTime)
	{
        //Standard Reset of all important frame data
		RunTime += DeltaTime;
		EngineController.Clear(EngineIO::Pixel(0, 0, 0, 0));
		std::fill(DepthBuffer.begin(), DepthBuffer.end(), Camera->GetMaxinumDepth());
        
        //allows the client to move objects, handle inputs etc before rendering
        PreFrame(DeltaTime);

        //Render Pass

        std::vector<InstanceHeirachy::SubWorldStackFrame> Stack;
        Stack.push_back(WorldRoot->GetSubWorldStackFrame());
        InstanceHeirachy::SubWorldStackFrame Frame = Stack.at(Stack.size() - 1);

        Vector3D CameraPosition = Camera->ExtractVectorPosition();
        Matrix4x4 CameraTranslation = Camera->ExtractTranslationMatrix();
        Matrix4x4 InverseCameraTranslation = CameraTranslation.Inverse();
        Matrix4x4 CameraRotationMatrix = Camera->ExtractRotationMatrix();
        Matrix4x4 InverseCameraRotation = CameraRotationMatrix.Inverse();
        Matrix4x4 Projection = Matrix4x4::CreateProjectionMatrix(Camera->GetAspectRatio(), Camera->GetVerticalFoV(), Camera->GetMininumDepth(), Camera->GetMaxinumDepth());

        //Apply a PRE-ORDER tree traversal
        //Each subworld is a node
        while (Stack.size() > 0)
        {
            Frame = Stack.at(Stack.size() - 1);

            //std::cout << "Rendered: " << Frame.Name << std::endl;
            ColourRGB Ambient;
            std::vector<InstanceHeirachy::PointLight*> PointLights;
            std::vector<InstanceHeirachy::DirectionalLight*> DirectionalLights;
            GatherLightData(Stack, Ambient, PointLights, DirectionalLights);

            RenderSubWorld(Stack, Projection, InverseCameraRotation, InverseCameraTranslation, CameraPosition, Ambient, PointLights, DirectionalLights);
            
            if (Frame.SubWorldIndex >= Frame.SubWorlds.size())
            {
                //searched all nodes
                //backtrack until all other nodes have been search 
                //if the world root has all nodes searched then the render is done
                while (Frame.SubWorldIndex >= Frame.SubWorlds.size() && Stack.size() > 0)
                {
                    Frame = Stack.at(Stack.size() - 1);
                    Stack.erase(Stack.end() - 1);
                }
            }
            else
            {
                //iterate to the next subworld
                Stack.push_back(Frame.SubWorlds[Frame.SubWorldIndex]->GetSubWorldStackFrame());
                Frame.SubWorldIndex++;
            }
        }

        //Apply users 2D or post processing effects
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

    bool IsKeyDown(EngineIO::Key Key)
    {
        return EngineController.GetKey(Key).bHeld;
    }

    bool IsKeyPressed(EngineIO::Key Key)
    {
        return EngineController.GetKey(Key).bPressed;
    }

    bool IsKeyReleased(EngineIO::Key Key)
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
        if (x < 0 || y < 0 || x >= Camera->GetScreenWidth() || y >= Camera->GetScreenHeight())
        {
            throw EngineException(DepthReadError, "The coordinates: (" + std::to_string(x) + ", " + std::to_string(y) + ") are invalid)");
            return -1.0f;
        }
		return DepthBuffer[x + (Camera->GetScreenWidth() * y)];
	}

private:
	void SetDepthBufferAt(int x, int y, float z)
	{
        if (x < 0) { return; }
        if (y < 0) { return; }
        if (x >= Camera->GetScreenWidth()) { return; }
        if (y >= Camera->GetScreenHeight()) { return; }
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