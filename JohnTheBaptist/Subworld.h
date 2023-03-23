#pragma once
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

//Matrix Based
#include "MatrixInstance.h"
#include "MeshInstance.h"

#include <queue>

namespace InstanceHeirachy
{
    class SubWorld;

    struct SubWorldStackFrame
    {
        std::string Name;
        Matrix4x4 Transform;
        std::vector<Light*> Lights;
        std::vector<MeshInstance*> Meshes;
        std::vector<SubWorld*> SubWorlds;
        int SubWorldIndex = 0;
    };

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

        SubWorldStackFrame GetSubWorldStackFrame()
        {
            std::queue<Instance*> InhabitantsToSort;
            SubWorldStackFrame Frame;
            Frame.Transform = GetMatrix();
            Frame.Name = GetName();
                
            InstanceSearchParameter Search = InstanceSearchParameter();
            Search.SetEnabled(true);

            for (Instance* ChildRef : GetAllChildrenMatchingState(Search))
            {
                InhabitantsToSort.push(ChildRef);
            }

            while (InhabitantsToSort.size() > 0)
            {
                Instance* Inhabitant = InhabitantsToSort.front();

                switch (Inhabitant->GetBaseType())
                {
                case InstanceBaseType::LightBaseType:
                {
                    Light* LightRef = dynamic_cast<Light*>(Inhabitant);
                    Frame.Lights.push_back(LightRef); 
                }
                    break;

                case InstanceBaseType::FolderBaseType:
                    for (Instance* ChildRef : Inhabitant->GetAllChildrenMatchingState(Search))
                    {
                        InhabitantsToSort.push(ChildRef);
                    }
                    break;

                case InstanceBaseType::MatrixInstanceBaseType:
                    
                    switch (Inhabitant->GetType())
                    {
                    case InstanceType::MeshInstanceType:
                    {
                        MeshInstance* MeshInstanceRef = dynamic_cast<MeshInstance*>(Inhabitant);
                        Frame.Meshes.push_back(MeshInstanceRef); 
                    }
                        break;

                    case InstanceType::SubWorldType:
                    {
                        SubWorld* SubWorldRef = dynamic_cast<SubWorld*>(Inhabitant);
                        Frame.SubWorlds.push_back(SubWorldRef);
                    }
                        break;
                    }

                    break;
                }

                InhabitantsToSort.pop();
            }

            return Frame;
        }
    };
}
