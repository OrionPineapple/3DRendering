#pragma once
//Includes all of the instance classes

#include "World.h"
#include "Instance.h"
#include "Folder.h"

//Matrix Based
#include "MatrixInstance.h"
#include "MeshInstance.h"
#include "Subworld.h"
#include "Camera.h"

//Lighting Based
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

namespace InstanceHeirachy
{
    void World::DeleteInstancePointer(Instance* Ref)
    {
        MeshInstance* MeshRef = NULL;
        Folder* FoldRef = NULL;
        Camera* CamRef = NULL;

        switch (Ref->GetType())
        {
        case InstanceType::AmbientLightType:
            throw EngineException("Unhandled instance type", "Need to implement deletion for this pointer type");
            break;
        case InstanceType::DirectionalLightType:
            throw EngineException("Unhandled instance type", "Need to implement deletion for this pointer type");
            break;
        case InstanceType::PointLightType:
            throw EngineException("Unhandled instance type", "Need to implement deletion for this pointer type");
            break;

        case InstanceType::MeshInstanceType:
            MeshRef = dynamic_cast<MeshInstance*>(Ref);
            Ref = NULL;
            delete MeshRef;
            break;
        case InstanceType::SubWorldType:
            throw EngineException("Unhandled instance type", "Need to implement deletion for this pointer type");
            break;
        case InstanceType::FolderType:
            FoldRef = dynamic_cast<Folder*>(Ref);
            Ref = NULL;
            delete FoldRef;
            break;
        case InstanceType::CameraType:
            CamRef = dynamic_cast<Camera*>(Ref);
            Ref = NULL;
            delete CamRef;
            break;
        default:
            throw EngineException("Unhandled instance type", "Need to implement deletion for this pointer type");
            break;
        }
    }
}