//
//#include "Engine.h"
//
//class UserProgram : public Engine
//{
//private:
//    int Width = 70;
//    int Length = 70;
//    float Seperation = 0.04f;
//    InstanceHeirachy::MeshInstance* MeshInstance;
//
//public:
//    UserProgram()
//    {
//
//    }
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
//
//
//        std::vector<std::shared_ptr<Point>>* VectorOfPoints = MeshInstance->GetReferenceToMesh().get()->GetPointReferences();
//        for (int x = 0; x < Width; x++)
//        {
//            for (int y = 0; y < Length; y++)
//            {
//                float Displacement = 0.09f * sinf(0.15f * x + 0.08f * y + GetRunTime());
//                Vector3D Position = Vector3D((float)x * Seperation, Displacement, (float)y * Seperation);
//                VectorOfPoints->at(x + (y * Width)).get()->SetPosition(Position);
//            }
//        }
//    }
//
//    bool PostFrame(float DeltaTime) override
//    {
//        //After 3D Rendering
//
//        return true;
//    }
//
//    bool OnStart() override
//    {
//        //Preload assets using Engine API before rendering begins
//
//        InstanceHeirachy::World* World = GetWorld();
//        InstanceHeirachy::Instance* WorldRoot = GetWorldRoot();
//        InstanceHeirachy::Camera* Camera = GetCamera();
//        Camera->SetMatrix(Matrix4x4::GetRotationMatrix(Vector3D(0, PI, 0)) * Matrix4x4::GetTranslationMatrix(Vector3D(0.0f, 2.0f, 2.0f)));
//
//        Mesh* PlanarMesh = new Mesh();
//        std::vector<std::shared_ptr<Point>>* VectorOfPoints = PlanarMesh->GetPointReferences();
//        std::vector<std::shared_ptr<Triangle>>* VectorOfTriangles = PlanarMesh->GetTriangleReferences();
//
//
//        for (int x = 0; x < Width; x++)
//        {
//            for (int y = 0; y < Length; y++)
//            {
//                Vector3D Position = Vector3D((float)x * Seperation, 0, (float)y * Seperation);
//                VectorOfPoints->push_back(std::shared_ptr<Point>(new Point(Position)));
//            }
//        }
//
//        for (int x = 0; x < Width - 1; x++)
//        {
//            for (int y = 0; y < Length - 1; y++)
//            {
//                std::shared_ptr<Point> C = VectorOfPoints->at((x)+(y * Width));
//                std::shared_ptr<Point> B = VectorOfPoints->at((x + 1) + (y * Width));
//                std::shared_ptr<Point> A = VectorOfPoints->at((x)+((y + 1) * Width));
//                VectorOfTriangles->push_back(std::shared_ptr<Triangle>(new Triangle(A, B, C)));
//            }
//        }
//
//        for (int x = 0; x < Width - 1; x++)
//        {
//            for (int y = 0; y < Length - 1; y++)
//            {
//                std::shared_ptr<Point> C = VectorOfPoints->at((x + 1) + (y * Width));
//                std::shared_ptr<Point> B = VectorOfPoints->at((x + 1) + ((y + 1) * Width));
//                std::shared_ptr<Point> A = VectorOfPoints->at((x)+((y + 1) * Width));
//                VectorOfTriangles->push_back(std::shared_ptr<Triangle>(new Triangle(A, B, C)));
//            }
//        }
//
//        MeshInstance = new InstanceHeirachy::MeshInstance
//        (
//            World,
//            std::shared_ptr<Mesh>(PlanarMesh)
//        );
//        MeshInstance->SetParent(WorldRoot);
//
//        return true;
//    }
//
//    bool OnDestroy() override
//    {
//        //When Application is closed -> please save any needed data
//        return true;
//    }
//
//};
//
//
//int main()
//{
//    UserProgram Program;
//    if (Program.Start("3D Wave Test", 800, 800))
//    {
//        //Program Successfully Started
//    }
//    else
//    {
//        //Program Failed to Start
//    }
//    return 0;
//}