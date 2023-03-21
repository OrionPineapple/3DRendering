//#include "Engine.h"
//
//class UserProgram : public Engine
//{
//private:
//    int Offset = 0;
//    Texture Tex;
//
//public:
//	UserProgram()
//	{
//        Tex = Texture("TextureTest.png");
//	}
//
//private:
//    void PreFrame(float DeltaTime) override
//    {
//        InstanceHeirachy::Camera* Camera = GetCamera();
//
//        if (IsKeyDown(olc::Key::UP))
//        {
//            Camera->RotateX(DeltaTime);
//        }
//        if (IsKeyDown(olc::Key::DOWN))
//        {
//            Camera->RotateX(-DeltaTime);
//        }
//        if (IsKeyDown(olc::Key::LEFT))
//        {
//            Camera->SetMatrix(Camera->ExtractTranslationMatrix() * Matrix4x4::GetYRotationMatrix(-DeltaTime) * Camera->ExtractRotationMatrix());
//        }
//        if (IsKeyDown(olc::Key::RIGHT))
//        {
//            Camera->SetMatrix(Camera->ExtractTranslationMatrix() * Matrix4x4::GetYRotationMatrix(DeltaTime) * Camera->ExtractRotationMatrix());
//        }
//
//        Camera->SetVerticalFoV(Camera->GetVerticalFoV() - 0.0001f * GetScrollWheelDelta());
//
//        Matrix4x4 CameraRotationMatrix = Camera->ExtractRotationMatrix();
//        Matrix4x4 InverseCameraRotation = CameraRotationMatrix.Inverse();
//
//        if (IsKeyDown(olc::Key::W))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(0, DeltaTime, 0));
//        }
//        if (IsKeyDown(olc::Key::S))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(0, -DeltaTime, 0));
//        }
//        if (IsKeyDown(olc::Key::A))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(DeltaTime, 0, 0));
//        }
//        if (IsKeyDown(olc::Key::D))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(-DeltaTime, 0, 0));
//        }
//        if (IsKeyDown(olc::Key::SHIFT))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(0, 0, -DeltaTime));
//        }
//        if (IsKeyDown(olc::Key::CTRL))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(0, 0, DeltaTime));
//        }
//
//        if (IsKeyPressed(olc::Key::SPACE))
//        {
//            Offset += 5;
//            InstanceHeirachy::World* World = GetWorld();
//            InstanceHeirachy::Instance* WorldRoot = GetWorldRoot();
//
//            InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
//            (
//                World,
//                std::shared_ptr<Mesh>(new Mesh("TouranSmooth.obj"))
//            );
//            MeshInstance->Translate(Vector3D(0, Offset, 0));
//            MeshInstance->SetParent(WorldRoot);
//        }
//
//    }
//
//	bool PostFrame(float DeltaTime) override
//	{
//		//After 3D Rendering
//
//        /*for (int y = 0; y < 400; y++)
//        {
//            for (int x = 0; x < 400; x++)
//            {
//                ColourRGB Colour = Tex.Get(x, y);
//                Draw(x, y, Colour);
//            }
//        }*/
//
//		return true;
//	}
//
//	bool OnStart() override
//	{
//		//Preload assets using Engine API before rendering begins
//
//        InstanceHeirachy::World* World = GetWorld();
//        InstanceHeirachy::Instance* WorldRoot = GetWorldRoot();
//
//        InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
//            (
//                World,
//                std::shared_ptr<Mesh>(new Mesh("TouranSmooth.obj")),
//                Matrix4x4::GetTranslationMatrix(Vector3D(0, -1, -10))
//            );
//        MeshInstance->SetParent(WorldRoot);
//
//		return true;
//	}
//
//	bool OnDestroy() override
//	{
//		//When Application is closed -> please save any needed data
//		return true;
//	}
//
//};
//
//
//int main()
//{
//	UserProgram Program;
//	if (Program.Start("User Program", 400, 400)) 
//	{
//		//Program Successfully Started
//	}
//	else
//	{
//		//Program Failed to Start
//	}
//	return 0;
//}


#include "Engine.h"

class UserProgram : public Engine
{
private:
    int Width = 50;
    int Length = 50;
    float Seperation = 1.0f;
    InstanceHeirachy::MeshInstance* MeshInstance;

public:
    UserProgram()
    {

    }

private:
    void PreFrame(float DeltaTime) override
    {
        InstanceHeirachy::Camera* Camera = GetCamera();

        if (IsKeyDown(olc::Key::UP))
        {
            Camera->RotateX(DeltaTime);
        }
        if (IsKeyDown(olc::Key::DOWN))
        {
            Camera->RotateX(-DeltaTime);
        }
        if (IsKeyDown(olc::Key::LEFT))
        {
            Camera->SetMatrix(Camera->ExtractTranslationMatrix() * Matrix4x4::GetYRotationMatrix(-DeltaTime) * Camera->ExtractRotationMatrix());
        }
        if (IsKeyDown(olc::Key::RIGHT))
        {
            Camera->SetMatrix(Camera->ExtractTranslationMatrix() * Matrix4x4::GetYRotationMatrix(DeltaTime) * Camera->ExtractRotationMatrix());
        }

        Camera->SetVerticalFoV(Camera->GetVerticalFoV() - 0.0001f * GetScrollWheelDelta());

        Matrix4x4 CameraRotationMatrix = Camera->ExtractRotationMatrix();
        Matrix4x4 InverseCameraRotation = CameraRotationMatrix.Inverse();

        if (IsKeyDown(olc::Key::W))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(0, DeltaTime, 0));
        }
        if (IsKeyDown(olc::Key::S))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(0, -DeltaTime, 0));
        }
        if (IsKeyDown(olc::Key::A))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(DeltaTime, 0, 0));
        }
        if (IsKeyDown(olc::Key::D))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(-DeltaTime, 0, 0));
        }
        if (IsKeyDown(olc::Key::SHIFT))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(0, 0, -DeltaTime));
        }
        if (IsKeyDown(olc::Key::CTRL))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(0, 0, DeltaTime));
        }



        std::vector<std::shared_ptr<Point>>* VectorOfPoints = MeshInstance->GetReferenceToMesh().get()->GetPointReferences();
        for (int x = 0; x < Width; x++)
        {
            for (int y = 0; y < Length; y++)
            {
                float Displacement = 0.1f * sinf(x + y + GetRunTime());
                Vector3D Position = Vector3D((float)x * Seperation, Displacement, (float)y * Seperation);
                VectorOfPoints->at(x + (y * Width)).get()->SetPosition(Position);
            }
        }
    }

    bool PostFrame(float DeltaTime) override
    {
        //After 3D Rendering

        return true;
    }

    bool OnStart() override
    {
        //Preload assets using Engine API before rendering begins

        InstanceHeirachy::World* World = GetWorld();
        InstanceHeirachy::Instance* WorldRoot = GetWorldRoot();

        Mesh* PlanarMesh = new Mesh();
        std::vector<std::shared_ptr<Point>>* VectorOfPoints = PlanarMesh->GetPointReferences();
        std::vector<std::shared_ptr<Triangle>>* VectorOfTriangles = PlanarMesh->GetTriangleReferences();


        for (int x = 0; x < Width; x++)
        {
            for (int y = 0; y < Length; y++)
            {
                Vector3D Position = Vector3D((float)x * Seperation, 0, (float)y * Seperation);
                VectorOfPoints->push_back(std::shared_ptr<Point>(new Point(Position)));
            }
        }

        for (int x = 0; x < Width - 1; x++)
        {
            for (int y = 0; y < Length - 1; y++)
            {
                std::shared_ptr<Point> A = VectorOfPoints->at((x)+(y * Width));
                std::shared_ptr<Point> B = VectorOfPoints->at((x + 1) + (y * Width));
                std::shared_ptr<Point> C = VectorOfPoints->at((x)+((y + 1) * Width));
                VectorOfTriangles->push_back(std::shared_ptr<Triangle>(new Triangle(A, B, C)));
            }
        }

        for (int x = 0; x < Width - 1; x++)
        {
            for (int y = 0; y < Length - 1; y++)
            {
                std::shared_ptr<Point> A = VectorOfPoints->at((x + 1) + (y * Width));
                std::shared_ptr<Point> B = VectorOfPoints->at((x + 1) + ((y + 1) * Width));
                std::shared_ptr<Point> C = VectorOfPoints->at((x)+((y + 1) * Width));
                VectorOfTriangles->push_back(std::shared_ptr<Triangle>(new Triangle(A, B, C)));
            }
        }

        MeshInstance = new InstanceHeirachy::MeshInstance
        (
            World,
            std::shared_ptr<Mesh>(PlanarMesh)
        );
        MeshInstance->SetParent(WorldRoot);

        return true;
    }

    bool OnDestroy() override
    {
        //When Application is closed -> please save any needed data
        return true;
    }

};


int main()
{
    UserProgram Program;
    if (Program.Start("User Program", 800, 500))
    {
        //Program Successfully Started
    }
    else
    {
        //Program Failed to Start
    }
    return 0;
}
