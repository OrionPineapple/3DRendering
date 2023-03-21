#pragma once
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

//Matrix Based
#include "MatrixInstance.h"
#include "MeshInstance.h"

namespace InstanceHeirachy
{
    class SubWorld : public virtual MatrixInstance
    {
    public:
        SubWorld(World* WorldRef)
        {
            Register(WorldRef);
            Type = InstanceType::SubWorldType;
            BaseType = InstanceBaseType::MatrixInstanceBaseType;
            CanHaveChildren = true;
        }

        void GetFilteredSubWorldInhabitants(std::vector<Light*> Lights, std::vector<SubWorld*> SubWorlds, std::vector<MeshInstance*> Meshes)
        {

        }
    };
}
