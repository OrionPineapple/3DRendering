#pragma once
#include "MatrixInstance.h"
#include "Mesh.h"
#include "EngineIO.h"

namespace InstanceHeirachy
{
    class MeshInstance : public virtual MatrixInstance
    {

    private:
        std::shared_ptr<Mesh> MyMesh;

    public:
        void OutputName() override { std::cout << Name; };
        MeshInstance(World* WorldRef, std::shared_ptr<Mesh> NewMesh)
        {
            Register(WorldRef);
            BaseType = InstanceBaseType::MatrixInstanceBaseType;
            Type = InstanceType::MeshInstanceType;
            MyMesh = NewMesh;
        }

        MeshInstance(World* WorldRef, std::shared_ptr<Mesh> NewMesh, Matrix4x4 InitMatrix)
        {
            Register(WorldRef);
            BaseType = InstanceBaseType::MatrixInstanceBaseType;
            Type = InstanceType::MeshInstanceType;
            MyMesh = NewMesh;
            SetMatrix(InitMatrix);
        }

        std::shared_ptr<Mesh> GetReferenceToMesh()
        {
            return std::shared_ptr<Mesh>(MyMesh);
        }

    };
}
