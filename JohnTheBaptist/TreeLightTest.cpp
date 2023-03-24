#include "Engine.h"

class UserProgram : public Engine
{
private:
    InstanceHeirachy::SubWorld* TreeHolder;

public:
    UserProgram()
    {

    }

private:
    void PreFrame(float DeltaTime) override
    {
        InstanceHeirachy::Camera* Camera = GetCamera();
        //Standard Camera Movement

        if (IsKeyDown(EngineIO::Key::UP))
        {
            Camera->RotateX(DeltaTime);
        }
        if (IsKeyDown(EngineIO::Key::DOWN))
        {
            Camera->RotateX(-DeltaTime);
        }
        if (IsKeyDown(EngineIO::Key::LEFT))
        {
            Camera->SetMatrix(Camera->ExtractTranslationMatrix() * Matrix4x4::GetYRotationMatrix(-DeltaTime) * Camera->ExtractRotationMatrix());
        }
        if (IsKeyDown(EngineIO::Key::RIGHT))
        {
            Camera->SetMatrix(Camera->ExtractTranslationMatrix() * Matrix4x4::GetYRotationMatrix(DeltaTime) * Camera->ExtractRotationMatrix());
        }

        Camera->SetVerticalFoV(Camera->GetVerticalFoV() - 0.0001f * GetScrollWheelDelta());

        Matrix4x4 CameraRotationMatrix = Camera->ExtractRotationMatrix();
        Matrix4x4 InverseCameraRotation = CameraRotationMatrix.Inverse();

        if (IsKeyDown(EngineIO::Key::W))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(0, DeltaTime, 0));
        }
        if (IsKeyDown(EngineIO::Key::S))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(0, -DeltaTime, 0));
        }
        if (IsKeyDown(EngineIO::Key::A))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(DeltaTime, 0, 0));
        }
        if (IsKeyDown(EngineIO::Key::D))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(-DeltaTime, 0, 0));
        }
        if (IsKeyDown(EngineIO::Key::SHIFT))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(0, 0, -DeltaTime));
        }
        if (IsKeyDown(EngineIO::Key::CTRL))
        {
            Camera->Translate(CameraRotationMatrix * Vector3D(0, 0, DeltaTime));
        }

        //This is the KEY TESTING POINT
        /*
        Here we are taking our SubWorld (TreeHolder) -> a collection of objects
        and moving it vertically sinesoidally. This does not affect
        the MeshInstances objects that are children of it, however it will
        affect how they rendered, ie with the offset of the subworld applied

        you will also notice only meshes that descend from this subworld are
        affected by lights in this subworld. Meshes parented to the worldroot
        are not illuminated by the pointlight in this TreeHolder subworld
        */
        TreeHolder->SetMatrix(Matrix4x4::GetTranslationMatrix(Vector3D(0.0f, 6.0f * sinf(GetRunTime() * 0.6f), 0.0f)));
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
        InstanceHeirachy::Camera* Camera = GetCamera();
        Camera->SetMatrix(Matrix4x4::GetRotationMatrix(Vector3D(0, 0, 0)) * Matrix4x4::GetTranslationMatrix(Vector3D(0.0f, 4.0f, 0.0f)));


        //Create our subworld, and set it as a child of our WorldRoot
        TreeHolder = new InstanceHeirachy::SubWorld(World);
        TreeHolder->SetName("TreeHolder");
        TreeHolder->SetParent(WorldRoot);

        //This ambient light is a child of the World Root, so every object will be affected by its light
        InstanceHeirachy::AmbientLight* Ambient = new InstanceHeirachy::AmbientLight(World);
        Ambient->SetLightColour(ColourRGB((int)(0.5f * 255.0f), (int)(0.5f * 255.0f), (int)(0.5f * 255.0f)));
        Ambient->SetParent(WorldRoot);

        //This PointLight is a child of our TreeHolder subworld, so only meshes in the subworld
        InstanceHeirachy::PointLight* PointLight = new InstanceHeirachy::PointLight(World, Vector3D(0, 5, 0));
        PointLight->SetLightColour(ColourRGB((int)(1.0f * 255.0f), (int)(1.0f * 255.0f), (int)(1.0f * 255.0f)));
        PointLight->SetParent(TreeHolder);

        //Create 5 test subjects, half are in the SubWorld, half arent
        //So half the trees will oscillate and be illuminated by a pointlight
        //The rest will be static and only illuminated
        for (int i = 0; i < 5; i++)
        {
            InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
            (
                World,
                std::shared_ptr<Mesh>(new Mesh("TreeLowPoly.obj")),
                Matrix4x4::GetTranslationMatrix(Vector3D(30.0f * cosf(i * PI * 0.4f), 0, 30.0f * sinf(i * PI * 0.4f)))
            );
            if (i % 2 == 0)
            {
                MeshInstance->SetParent(TreeHolder);
            }
            else
            {
                MeshInstance->SetParent(WorldRoot);
            }
        }

        return true;
    }

    bool OnDestroy() override
    {
        //We dont need to clean up our SubWorld* TreeHolder
        //as this will be handled by the Engine
        //but if we allocated other items to the heap
        //that aren't Instances, we should clean up here
        return true;
    }

};


int main()
{
    UserProgram Program;
    if (Program.Start("User Program", 1000, 800))
    {
        //Program Successfully Started
    }
    else
    {
        //Program Failed to Start
    }
    return 0;
}


