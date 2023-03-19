#pragma once
#include "Instance.h"

namespace InstanceHeirachy
{
    class Folder : public virtual Instance
    {
    public:
        Folder(World* WorldRef)
        {
            Register(WorldRef);
            Type = InstanceType::FolderType;
            BaseType = InstanceBaseType::FolderBaseType;
            CanHaveChildren = true;
        }
    };
}
