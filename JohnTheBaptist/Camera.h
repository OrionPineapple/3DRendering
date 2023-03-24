#pragma once
#include "World.h"
#include "Instance.h"
#include "MatrixInstance.h"
#include "EngineIO.h"


namespace InstanceHeirachy
{
    struct Plane
    {
        Vector3D Position;
        Vector3D UnitDirection;
    };

    //should be used as a virtual class as well eg class A : public virtual MatrixInstance
    class Camera : public virtual MatrixInstance
    {
    private:
        float MininumDepth = 0.1f;
        float MaxinumDepth = 1000.0f;
        float VerticalFoV;
        float AspectRatio;
        EngineIO::PixelGameEngine *EngineController;

        const float MininumVerticalFoV = 0.1f;
        const float MaxinumVerticalFoV = 6.28f;

    public:
        Camera(World* WorldRef, float FoV, EngineIO::PixelGameEngine* Controller)
        {
            Register(WorldRef);
            Type = InstanceType::CameraType;
            SetVerticalFoV(FoV);
            EngineController = Controller;
        }

        float GetVerticalFoV()
        {
            return VerticalFoV;
        }

        float GetHorizontalFoV()
        {
            //we only store vertical fov, so we must convert this across using the formula
            return atanf(tanf(VerticalFoV * 0.5f) * (1.0f / GetAspectRatio())) * 2.0f;
        }

        void SetVerticalFoV(float FoV) 
        {
            //Mininum and Maxinum just before and after 0, 2pi
            //otherwise everything will break mathematically
            if (FoV < MininumVerticalFoV)
            {
                FoV = MininumVerticalFoV;
            }
            else if (FoV > MaxinumVerticalFoV)
            {
                FoV = MaxinumVerticalFoV;
            }

            VerticalFoV = FoV;
        }

        void SetHorizontalFoV(float FoV)
        {
            //we only store vertical fov, so we must convert this across using the formula
            SetVerticalFoV(atanf(GetAspectRatio() * tanf(FoV * 0.5f)) * 2.0f);
        }

        int GetScreenHeight()
        {
            return  EngineController->ScreenHeight();
        }

        int GetScreenWidth()
        {
            return EngineController->ScreenWidth();
        }

        float GetAspectRatio()
        {
            return ((float)GetScreenHeight() / (float)GetScreenWidth());
        }

        float GetMininumDepth()
        {
            return MininumDepth;
        }

        float GetMaxinumDepth()
        {
            return MaxinumDepth;
        }

        float SetMininumDepth(float MinDepth)
        {
            //Must be less than MaxDepth and bigger than 0.0f
            if (MinDepth < 0.0f)
            {
                MinDepth = 0.0f;
            }
            else if (MinDepth > MaxinumDepth)
            {
                MinDepth = MaxinumDepth;
            }
            MininumDepth = MinDepth;
        }

        float SetMaxinumDepth(float MaxDepth)
        {
            //Must be bigger than MinDepth
            if (MaxDepth < MininumDepth)
            {
                MaxDepth = MininumDepth;
            }
            MaxinumDepth = MaxDepth;
        }

        std::vector<Plane> GetCullingPlanes(Matrix4x4 InverseTransform, Matrix4x4 InverseRotate)
        {
            //Culling planes define where the camera can see
            //If a point is above (ie dot product with normal > 0)
            //all planes then the camera can see it

            //The 6 planes are back, front, top, bottom, left, right

            Matrix4x4 CameraRotationMatrix = InverseRotate * ExtractRotationMatrix();
            Vector3D CameraPosition = InverseTransform * ExtractVectorPosition();

            std::vector<Plane> CullingPlanes;
            float a = (GetVerticalFoV() * 0.5f);
            float b = (GetHorizontalFoV() * 0.5f);


            float HalfVerticalFoV = (sinf(a)) / (cosf(a) + sinf(a));
            float HalfHorizontalFoV = (sinf(b)) / (cosf(b) + sinf(b));

            HalfVerticalFoV = 1.0f - HalfVerticalFoV;
            HalfHorizontalFoV = 1.0f - HalfHorizontalFoV;


            Vector3D CameraForward = CameraRotationMatrix * Vector3D(0.0f, 0.0f, -1.0f);
            Vector3D CameraUpward = CameraRotationMatrix * Vector3D(0.0f, 1.0f, 0.0f);
            Vector3D CameraRight = CameraRotationMatrix * Vector3D(-1.0f, 0.0f, 0.0f);

            Plane MinZ;
            MinZ.Position = CameraPosition + (CameraForward * MininumDepth);
            MinZ.UnitDirection = CameraForward;

            Plane MaxZ;
            MaxZ.Position = CameraPosition + (CameraForward * MaxinumDepth);
            MaxZ.UnitDirection = -CameraForward;

            /*
            By using convex combination of vectors, we can determine direction vectors for each of these culling planes
            
            */

            Plane TopPlane;
            TopPlane.Position = CameraPosition;
            TopPlane.UnitDirection = Vector3D::Normalise(Vector3D::Interpolate(CameraForward, -CameraUpward, HalfVerticalFoV));

            Plane BottomPlane;
            BottomPlane.Position = CameraPosition;
            BottomPlane.UnitDirection = Vector3D::Normalise(Vector3D::Interpolate(CameraForward, CameraUpward, HalfVerticalFoV));

            Plane RightPlane;
            RightPlane.Position = CameraPosition;
            RightPlane.UnitDirection = Vector3D::Normalise(Vector3D::Interpolate(CameraForward, -CameraRight, HalfHorizontalFoV));

            Plane LeftPlane;
            LeftPlane.Position = CameraPosition;
            LeftPlane.UnitDirection = Vector3D::Normalise(Vector3D::Interpolate(CameraForward, CameraRight, HalfHorizontalFoV));

            CullingPlanes.push_back(MinZ);
            CullingPlanes.push_back(MaxZ);
            CullingPlanes.push_back(TopPlane);
            CullingPlanes.push_back(BottomPlane);
            CullingPlanes.push_back(LeftPlane);
            CullingPlanes.push_back(RightPlane);

            return CullingPlanes;
        }
    };
}
