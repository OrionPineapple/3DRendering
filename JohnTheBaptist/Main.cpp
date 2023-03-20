
#include "Engine.h"

class UserProgram : public Engine
{
private:
    int Offset = 0;

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

        if (IsKeyPressed(olc::Key::SPACE))
        {
            Offset += 5;
            InstanceHeirachy::World* World = GetWorld();
            InstanceHeirachy::Instance* WorldRoot = GetWorldRoot();

            InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
            (
                World,
                std::shared_ptr<Mesh>(new Mesh("TouranSmooth.obj"))
            );
            MeshInstance->Translate(Vector3D(0, Offset, 0));
            MeshInstance->SetParent(WorldRoot);
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

        InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
            (
                World,
                std::shared_ptr<Mesh>(new Mesh("TouranSmooth.obj")),
                Matrix4x4::GetTranslationMatrix(Vector3D(0, -1, -10))
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
	if (Program.Start("User Program", 500, 500)) 
	{
		//Program Successfully Started
	}
	else
	{
		//Program Failed to Start
	}
	return 0;
}
