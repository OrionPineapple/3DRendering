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
        //This stores all needed render information
        //This removes unneeded folders and parenting
        //allows quick iteration over all lights

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
            //we need to find all the subworlds, meshinstances and lights inside
            //this subworld and return it as a stackframe

            std::queue<Instance*> InhabitantsToSort;
            SubWorldStackFrame Frame;
            Frame.Transform = GetMatrix();
            Frame.Name = GetName();
                
            InstanceSearchParameter Search = InstanceSearchParameter();
            Search.SetEnabled(true);
            //if it is disabled, we must completely ignore it, and not search its children either

            for (Instance* ChildRef : GetAllChildrenMatchingState(Search))
            {
                InhabitantsToSort.push(ChildRef);
            }

            while (InhabitantsToSort.size() > 0)
            {
                //each child we find, we enqueue it
                //we keep filtering through until no more  
                //instance children are left to process 

                Instance* Inhabitant = InhabitantsToSort.front();

                switch (Inhabitant->GetBaseType())
                {
                case InstanceBaseType::LightBaseType:
                {
                    //we dont care what type of light, that can be dealt with later
                    Light* LightRef = dynamic_cast<Light*>(Inhabitant);
                    Frame.Lights.push_back(LightRef); 
                }
                    break;

                case InstanceBaseType::FolderBaseType:
                    for (Instance* ChildRef : Inhabitant->GetAllChildrenMatchingState(Search))
                    {
                        //folders dont have any render use
                        //only hold children
                        //which we must search
                        InhabitantsToSort.push(ChildRef);
                    }
                    break;

                case InstanceBaseType::MatrixInstanceBaseType:
                    
                    switch (Inhabitant->GetType())
                    {
                    case InstanceType::MeshInstanceType:
                    {
                        //we want to render this
                        MeshInstance* MeshInstanceRef = dynamic_cast<MeshInstance*>(Inhabitant);
                        Frame.Meshes.push_back(MeshInstanceRef); 
                    }
                        break;

                    case InstanceType::SubWorldType:
                    {
                        //we dont want to search this yet
                        //this is a completly different subworld

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
