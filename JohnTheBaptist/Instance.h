#pragma once
#include <string>
#include <vector>
#include <mutex>
#include "EngineExceptions.h"
#include "World.h"
//Important to use as a virtual class -> eg class A : public Virtual Instance

namespace InstanceHeirachy
{

	struct InstanceSearchParameter
	{
    private: 
        bool UseName = false;
		std::string Name; 
		
    public:

		void SetName(std::string NewName)
		{
			UseName = true;
			Name = NewName;
		}

		void SetName()
		{
			UseName = false;
			Name = ""; //reduces memory use if its a long name
		}

        bool GetUseName()
        {
            return UseName;
        }

        std::string GetName()
        {
            return Name;
        }

    private:
		bool UseBaseType = false;
		InstanceBaseType BaseType;

    public:
		void SetBaseType(InstanceBaseType NewBaseType)
		{
			UseBaseType = true;
			BaseType = NewBaseType;
		}

		void SetBaseType()
		{
			UseBaseType = false;
		}

        bool GetUseBaseType()
        {
            return UseBaseType;
        }

        InstanceBaseType GetBaseType()
        {
            return BaseType;
        }

    private:
		bool UseType = false;
		InstanceType Type;

    public:
        void SetType()
        {
            UseType = false;
        }

		void SetType(InstanceType NewType)
		{
			UseType = true;
			Type = NewType;
		}

        bool GetUseType()
        {
            return UseType;
        }

        InstanceType GetType()
        {
            return Type;
        }

    private:
		bool UseEnabled = false;
		bool Enabled;

    public:
		void SetEnabled(bool NewEnabled)
		{
			UseEnabled = true;
			Enabled = NewEnabled;
		}

		void SetEnabled()
		{
			UseEnabled = false;
		}

        bool GetUseEnabled()
        {
            return UseEnabled;
        }

        bool GetEnabled()
        {
            return Enabled;
        }

    private:
		int MaxChildren = -1;

    public:
		void SetMaxChildren(int NewMax)
		{
			if (NewMax < 0) 
			{
				SetMaxChildren();
				return;
			}

			MaxChildren = NewMax;
		}

		void SetMaxChildren()
		{
			MaxChildren = -1;
        }

        int GetMaxChildren()
        {
            return MaxChildren;
        }

    private:
		int MinChildren = -1;

    public:
		void SetMinChildren(int NewMin)
		{
			if (NewMin < 0)
			{
				SetMinChildren();
				return;
			}

			MinChildren = NewMin;
		}

		void SetMinChildren()
		{
			MinChildren = -1;
		}

        int GetMinChildren()
        {
            return MinChildren;
        }
	};

	class Instance
	{
	protected:
		std::string Name;
		int ParentID = -1;
		std::vector<int> ChildrenIDs;
		bool Enabled = true;

		InstanceBaseType BaseType;
		InstanceType Type;
		bool CanHaveChildren = false;

        bool Locked = false;
        int InstanceID = -1;
        World* WorldReference = NULL;

	protected:
        void Register(World* WorldRef)
        {
            int ID = WorldRef->RegisterInstance(this);
            if (Locked)
            {
                throw EngineException("Cannot set InstanceID to a locked Instance", "Attempt to change InstanceID of " + Name + ". The InstanceID is locked!");
                return;
            }

            Locked = true;
            InstanceID = ID;
            WorldReference = WorldRef;
        }

    public:
        InstanceBaseType GetBaseType()
        {
            return BaseType;
        }

        InstanceType GetType()
        {
            return Type;
        }

        virtual void OutputName() { std::cout << Name; };
		std::string GetName()
		{
			return Name;
		}

		void SetName(std::string NewName)
		{
			Name = NewName;
		}

		int GetParentID()
		{
			return ParentID;
		}

        Instance* GetParent()
        {
            return WorldReference->GetInstance(ParentID);
        }

		bool SetParent(Instance* NewParent)
		{
			if (NewParent == NULL)
			{
                throw EngineException("Cannot set Parent to a NULL Instance", "Attempt to set parent of " + Name + " but that instance does not exist");
			}
			if (!NewParent->CanHaveChildren)
			{
                throw EngineException("Cannot set Parent to an Instance that cannot have children", "Attempt to set parent of " + Name + " as " + NewParent->GetName());
			}

			//parse through the current parents children and remove all references to self
			//there only should be one, however double check for duplicates
            if (ParentID != -1)
            {
                Instance* ParentRef = GetParent();
                std::vector<int>::iterator pos = std::find(ParentRef->ChildrenIDs.begin(), ParentRef->ChildrenIDs.end(), InstanceID);
                if (pos != ParentRef->ChildrenIDs.end())
                {
                    ParentRef->ChildrenIDs.erase(pos);
                }
            }

            ParentID = NewParent->InstanceID;
			NewParent->ChildrenIDs.push_back(InstanceID);
			return true;
		}

        std::vector<int> GetChildrenIDs()
        {
            return ChildrenIDs;
        }

		std::vector<Instance*> GetChildren()
		{
            std::vector<Instance*> Children;
            for (int ChildID : ChildrenIDs)
            {
                Instance* Child = WorldReference->GetInstance(ChildID);
                Children.push_back(Child);
            }
            return Children;
		}

		Instance* GetChildMatchingState(InstanceSearchParameter Parameters)
		{
            for (int ChildID : ChildrenIDs)
            {
                Instance* Child = WorldReference->GetInstance(ChildID);
                if (DoesInstanceMatchState(Child, Parameters))
                {
                    return Child;
                }
            }
			return NULL;
		}

		std::vector<Instance*> GetAllChildMatchingState(InstanceSearchParameter Parameters)
		{
			std::vector<Instance*> Matching;
			for (int ChildID : ChildrenIDs)
			{
                Instance* Child = WorldReference->GetInstance(ChildID);
				if (DoesInstanceMatchState(Child, Parameters))
				{
					Matching.push_back(Child);
				}
			}
			return Matching;
		}

		Instance* GetFirstAncestorMatchingState(InstanceSearchParameter State)
		{
			Instance* Current = WorldReference->GetInstance(ParentID);
			while (Current != NULL)
			{
				if (DoesInstanceMatchState(Current, State))
				{
					return Current;
				}
				Current = WorldReference->GetInstance(Current->ParentID);
			}
			return Current;
		}

	public:
		static bool DoesInstanceMatchState(Instance* InstancePtr, InstanceSearchParameter& State)
		{
			//Check Name
			if (State.GetUseName())
			{
                if (State.GetName() != InstancePtr->Name)
                {
                    return false;
                }
			}

			//Check BaseType
			if (State.GetUseBaseType())
			{
                if (State.GetBaseType() != InstancePtr->BaseType)
                {
                    return false;
                }
			}

			//Check Type
			if (State.GetUseType())
			{
                if (State.GetType() != InstancePtr->Type)
                {
                    return false;
                }
			}

			//Check Enabled
			if (State.GetUseEnabled())
			{
                if (State.GetEnabled() != InstancePtr->Enabled)
                {
                    return false;
                }
			}

			int ChildCount = InstancePtr->ChildrenIDs.size();

			//Check MaxChildren
			if (State.GetMaxChildren() != -1 && State.GetMaxChildren() < ChildCount)
			{
				return false;
			}

			//Check MinChildren
			if (State.GetMinChildren() != -1 && State.GetMinChildren() > ChildCount)
			{
				return false;
			}

			return true;
		}
	};
}

