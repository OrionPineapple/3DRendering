#pragma once

#include <map>
#include "EngineExceptions.h"

namespace InstanceHeirachy
{
    class Instance;

    const enum class InstanceBaseType
    {
        LightBaseType = 1,
        MatrixInstanceBaseType = 2,
        FolderBaseType = 3,
    };

    const enum class InstanceType
    {
        AmbientLightType = 10,
        DirectionalLightType = 11,
        PointLightType = 12,

        MeshInstanceType = 14,
        SubWorldType = 15,
        FolderType = 16,
        CameraType = 17,
    };

    class World
    {
    private:
        int NextInstanceID = -1;
        std::map<int, Instance*> WorldInstances;

    public: 
        ~World()
        {
            //it is important to clean up our memory of pointers
            std::map<int, Instance*>::iterator it = WorldInstances.begin();

            while (it != WorldInstances.end())
            {
                Instance* Ref = it->second;
                WorldInstances[it->first] = NULL;
                DeleteInstancePointer(Ref);
                it++;
            }

            WorldInstances.clear();
        }

        int RegisterInstance(Instance* NewInstance)
        {
            NextInstanceID++;
            WorldInstances[NextInstanceID] = NewInstance;
            return NextInstanceID;
        }

        Instance* GetInstance(int InstanceID)
        {
            return WorldInstances[InstanceID];
        }

        void DeleteInstance(int InstanceID)
        {
            //this is called by the instance itself
            Instance* Ref = WorldInstances[InstanceID];
            WorldInstances.erase(InstanceID);

            DeleteInstancePointer(Ref);
        }
        
    private:
        void DeleteInstancePointer(Instance* Ref);
        //this must be defined later as the compiler needs to know what each instance type is first :(
        //however each instance needs to know what the world is too
        //so we define deletion of each type of instance in the InstanceHeirachy
    };
}