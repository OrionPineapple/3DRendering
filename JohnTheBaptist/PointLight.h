#pragma once

#include "Light.h"
#include "Vector3D.h"

namespace InstanceHeirachy
{

    class PointLight : public virtual Light
    {
    private:
        Vector3D Point;

    public:
        PointLight(World* WorldRef, Vector3D Position)
        {
            Register(WorldRef);
            Point = Position;
            Type = InstanceType::PointLightType;
            BaseType = InstanceBaseType::LightBaseType;
        }

        void SetPoint(Vector3D Position)
        {
            Point = Position;
        }

        Vector3D GetPoint()
        {
            return Point;
        }
    };

}