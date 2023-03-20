#pragma once

#include "Light.h"
#include "Vector3D.h"

namespace InstanceHeirachy
{

    class DirectionalLight : public virtual Light
    {
    private:
        Vector3D Direction;

    public:
        DirectionalLight(World* WorldRef, Vector3D Direct)
        {
            Register(WorldRef);
            Direction = Direct;
            Type = InstanceType::DirectionalLightType;
            BaseType = InstanceBaseType::LightBaseType;
        }

        void SetDirection(Vector3D Direct)
        {
            Direction = Direct;
        }

        Vector3D GetDirection()
        {
            return Direction;
        }
    };

}