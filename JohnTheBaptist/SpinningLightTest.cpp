//#include "Engine.h"
//
//class UserProgram : public Engine
//{
//private:
//    InstanceHeirachy::DirectionalLight* RedLight;
//    InstanceHeirachy::DirectionalLight* BlueLight;
//    InstanceHeirachy::DirectionalLight* GreenLight;
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
//        BlueLight->SetDirection(Vector3D(sinf(GetRunTime() + 0.666f * PI), 0.0f, cosf(GetRunTime() + 0.6666f * PI)));
//        RedLight->SetDirection(Vector3D(sinf(GetRunTime() + 1.33333 * PI), 0.0f, cosf(GetRunTime() + 1.333333f * PI)));
//        GreenLight->SetDirection(Vector3D(sinf(GetRunTime()), 0.0f, cosf(GetRunTime())));
//
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
//        Camera->SetMatrix(Matrix4x4::GetRotationMatrix(Vector3D(0, 0, 0)) * Matrix4x4::GetTranslationMatrix(Vector3D(0.0f, 0.0f, 4.0f)));
//
//        RedLight = new InstanceHeirachy::DirectionalLight(World, Vector3D(0, 0, 0));
//        RedLight->SetLightColour(ColourRGB(255, 0, 0));
//        RedLight->SetParent(WorldRoot);
//
//        BlueLight = new InstanceHeirachy::DirectionalLight(World, Vector3D(0, 0, 0));
//        BlueLight->SetLightColour(ColourRGB(0, 0, 255));
//        BlueLight->SetParent(WorldRoot);
//
//        GreenLight = new InstanceHeirachy::DirectionalLight(World, Vector3D(0, 0, 0));
//        GreenLight->SetLightColour(ColourRGB(0, 255, 0));
//        GreenLight->SetParent(WorldRoot);
//
//        InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
//        (
//            World,
//            std::shared_ptr<Mesh>(new Mesh("Cube.obj")),
//            Matrix4x4::GetTranslationMatrix(Vector3D(0, 0, 0))
//        );
//        MeshInstance->SetParent(WorldRoot);
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
//    if (Program.Start("User Program", 1000, 800))
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
