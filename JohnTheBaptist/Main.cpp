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

        TreeHolder = new InstanceHeirachy::SubWorld(World);
        TreeHolder->SetName("TreeHolder");
        TreeHolder->SetParent(WorldRoot);

        InstanceHeirachy::AmbientLight* Ambient = new InstanceHeirachy::AmbientLight(World);
        Ambient->SetLightColour(ColourRGB((int)(0.5f * 255.0f), (int)(0.5f * 255.0f), (int)(0.5f * 255.0f)));
        Ambient->SetParent(WorldRoot);

        InstanceHeirachy::PointLight* PointLight = new InstanceHeirachy::PointLight(World, Vector3D(0, 5, 0));
        PointLight->SetLightColour(ColourRGB((int)(1.0f * 255.0f), (int)(1.0f * 255.0f), (int)(1.0f * 255.0f)));
        PointLight->SetParent(TreeHolder);

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
		//When Application is closed -> please save any needed data
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


