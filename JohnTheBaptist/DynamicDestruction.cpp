//#include "Engine.h"
//
//class UserProgram : public Engine
//{
//private:
//    int Offset = 0;
//    Texture Tex;
//
//public:
//    UserProgram()
//    {
//        //Tex = Texture("TextureTest.png");
//    }
//
//private:
//    void PreFrame(float DeltaTime) override
//    {
//        InstanceHeirachy::Camera* Camera = GetCamera();
//
//        if (IsKeyDown(EngineIO::Key::UP))
//        {
//            Camera->RotateX(DeltaTime);
//        }
//        if (IsKeyDown(EngineIO::Key::DOWN))
//        {
//            Camera->RotateX(-DeltaTime);
//        }
//        if (IsKeyDown(EngineIO::Key::LEFT))
//        {
//            Camera->SetMatrix(Camera->ExtractTranslationMatrix() * Matrix4x4::GetYRotationMatrix(-DeltaTime) * Camera->ExtractRotationMatrix());
//        }
//        if (IsKeyDown(EngineIO::Key::RIGHT))
//        {
//            Camera->SetMatrix(Camera->ExtractTranslationMatrix() * Matrix4x4::GetYRotationMatrix(DeltaTime) * Camera->ExtractRotationMatrix());
//        }
//
//        Camera->SetVerticalFoV(Camera->GetVerticalFoV() - 0.0001f * GetScrollWheelDelta());
//
//        Matrix4x4 CameraRotationMatrix = Camera->ExtractRotationMatrix();
//        Matrix4x4 InverseCameraRotation = CameraRotationMatrix.Inverse();
//
//        if (IsKeyDown(EngineIO::Key::W))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(0, DeltaTime, 0));
//        }
//        if (IsKeyDown(EngineIO::Key::S))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(0, -DeltaTime, 0));
//        }
//        if (IsKeyDown(EngineIO::Key::A))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(DeltaTime, 0, 0));
//        }
//        if (IsKeyDown(EngineIO::Key::D))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(-DeltaTime, 0, 0));
//        }
//        if (IsKeyDown(EngineIO::Key::SHIFT))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(0, 0, -DeltaTime));
//        }
//        if (IsKeyDown(EngineIO::Key::CTRL))
//        {
//            Camera->Translate(CameraRotationMatrix * Vector3D(0, 0, DeltaTime));
//        }
//
//        InstanceHeirachy::World* World = GetWorld();
//        InstanceHeirachy::Instance* WorldRoot = GetWorldRoot();
//
//        if (IsKeyPressed(EngineIO::Key::SPACE))
//        {
//            Offset += 5;
//
//            InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
//            (
//                World,
//                std::shared_ptr<Mesh>(new Mesh("TouranSmooth.obj")),
//                Camera->GetMatrix() * Matrix4x4::GetTranslationMatrix(Vector3D(0.0f, -1.0f, -5.0f)) * Matrix4x4::GetRotationMatrix(Vector3D(0, 0, 0))
//            );
//            MeshInstance->SetParent(WorldRoot);
//        }
//
//        if (IsKeyPressed(EngineIO::Key::F))
//        {
//            Vector3D CamPos = Camera->ExtractVectorPosition();
//            InstanceHeirachy::InstanceSearchParameter Search = InstanceHeirachy::InstanceSearchParameter();
//            Search.SetType(InstanceHeirachy::InstanceType::MeshInstanceType);
//            Search.SetEnabled(true);
//            std::vector<InstanceHeirachy::Instance*> EnabledMeshChildren = WorldRoot->GetAllChildMatchingState(Search);
//
//            if (EnabledMeshChildren.size() >= 1)
//            {
//                InstanceHeirachy::MeshInstance* ClosestMesh = dynamic_cast<InstanceHeirachy::MeshInstance*>(EnabledMeshChildren.at(0));
//                float Distance = 1000000.0f;
//                for (InstanceHeirachy::Instance* Instance : EnabledMeshChildren)
//                {
//                    InstanceHeirachy::MeshInstance* Mesh = dynamic_cast<InstanceHeirachy::MeshInstance*>(Instance);
//                    float CompDistance = (Mesh->ExtractVectorPosition() - CamPos).GetMagnitude();
//                    if (CompDistance < Distance)
//                    {
//                        Distance = CompDistance;
//                        ClosestMesh = Mesh;
//                    }
//                }
//                ClosestMesh->Destroy();
//            }
//
//        }
//
//    }
//
//    bool PostFrame(float DeltaTime) override
//    {
//        //After 3D Rendering
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
//        Camera->SetMatrix(Matrix4x4::GetRotationMatrix(Vector3D(0, 0, 0)) * Matrix4x4::GetTranslationMatrix(Vector3D(0.0f, 1.0f, 5.0f)));
//
//        InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
//        (
//            World,
//            std::shared_ptr<Mesh>(new Mesh("TouranSmooth.obj")),
//            Matrix4x4::GetTranslationMatrix(Vector3D(0, 0, 0))
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
//    if (Program.Start("User Program", 400, 400))
//    {
//        //Program Successfully Started
//    }
//    else
//    {
//        //Program Failed to Start
//    }
//    return 0;
//}
//
//
