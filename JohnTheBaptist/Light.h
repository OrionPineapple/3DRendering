#pragma once

#include "Instance.h"
#include "ColourRGB.h"

namespace InstanceHeirachy
{

    class Light : public virtual Instance
    {
        //no public contructor as this is only to be inherited from

    private:
        ColourRGB Colour;

    protected:
        Light()
        {
            BaseType = InstanceBaseType::LightBaseType;
        }
    public:
        ColourRGB GetLightColour()
        {
            return Colour;
        }

        void SetLightColour(ColourRGB NewColour)
        {
            Colour = NewColour;
        }
    };

}