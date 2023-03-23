#include "Engine.h"

class UserProgram : public Engine
{
public:
	UserProgram()
	{

	}

private:
    void PreFrame(float DeltaTime) override
    {
        //Runs before any 3D rendering takes place
        //Change any 3D objects here such as Meshes or Camera Movement

        //Here is a simple Camera Movement system

        InstanceHeirachy::Camera* Camera = GetCamera();

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


    }

	bool PostFrame(float DeltaTime) override
	{
		//After 3D Rendering
        //Perfect for 2D gui or reading the DepthBuffer

		return true;
	}

	bool OnStart() override
	{
		//Preload assets using Engine API before rendering begins

        //This example here will:
        //Get the World -> All instances use the world in their constructor for management
        //Get the WorldRoot -> this is the ultimate parent of all instances
        //Get the Camera -> controls render viewpoint and settings, here we initialise the rotation and translation

        InstanceHeirachy::World* World = GetWorld();
        InstanceHeirachy::Instance* WorldRoot = GetWorldRoot();
        InstanceHeirachy::Camera* Camera = GetCamera();

        Camera->SetMatrix(Matrix4x4::GetRotationMatrix(Vector3D(0, 0, 0)) * Matrix4x4::GetTranslationMatrix(Vector3D(0.0f, 0.0f, 0.0f)));




        //This is an example of loading a mesh and setting it as a child of the WorldRoot
        InstanceHeirachy::MeshInstance* MeshInstance = new InstanceHeirachy::MeshInstance
        (
            World,
            std::shared_ptr<Mesh>(new Mesh("Cube.obj")),
            Matrix4x4::GetTranslationMatrix(Vector3D(0, 0, -5.0f))
        );
        MeshInstance->SetParent(WorldRoot);



        //Here we create an ambient light for everything

        InstanceHeirachy::AmbientLight* Ambient = new InstanceHeirachy::AmbientLight(World);
        Ambient->SetLightColour(ColourRGB(100, 100, 100));
        Ambient->SetParent(WorldRoot);


        //Here we create a directional light for everything

        InstanceHeirachy::DirectionalLight* Direction = new InstanceHeirachy::DirectionalLight(World , Vector3D(-3, -2, -1));
        Direction->SetLightColour(ColourRGB(255, 255, 255));
        Direction->SetParent(WorldRoot);


		return true;
	}

	bool OnDestroy() override
	{
		//When Application is closed this function is called before destruction
        //Save any data you need to and delete all pointers not handled by the engine
        //Instances will be handled by the engine

		return true;
	}

};

const int Width = 1000;
const int Height = 800;
const std::string Name = "User Program";

int main()
{
	UserProgram Program;
	if (Program.Start(Name, Width, Height))
	{
		//Program Successfully Started
	}
	else
	{
		//Program Failed to Start
	}
	return 0;
}


