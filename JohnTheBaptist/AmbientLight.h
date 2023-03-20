#pragma once

#include "Light.h"
#include "Vector3D.h"

namespace InstanceHeirachy
{

    class AmbientLight : public virtual Light
    {
    private:

    public:
        AmbientLight(World* WorldRef)
        {
            Register(WorldRef);
            Type = InstanceType::AmbientLightType;
            BaseType = InstanceBaseType::LightBaseType;
        }
    };

}