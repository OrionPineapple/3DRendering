#pragma once
#include "MatrixInstance.h"

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
    };
}
