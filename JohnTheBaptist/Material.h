#pragma once

#include "ColourRGB.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <fstream>
#include <map>
#include <string>

const float MaxColourValue = 255.0f;

class Material
{
private:
    ColourRGB AmbientColour;
    ColourRGB DiffuseColour;
    ColourRGB SpecularColour;


public:
    Material()
    {

    }

    Material(ColourRGB Ka, ColourRGB Kd, ColourRGB Ks)
    {
        AmbientColour = Ka;
        DiffuseColour = Kd;
        SpecularColour = Ks;
    }
    
    static void MaterialMapWithFile(std::string FileName, std::map<std::string, Material>& MaterialMap)
    {
        //Load in our File
        std::ifstream FileInput;
        std::string line = "";
        FileInput.open(FileName, std::ios::in);

        //Create our Map of Materials with strings as keys
        //This is because each material is defined by a name
        std::string CurrentKey = "";
        Material CurrentMaterial;

        if (!FileInput)
        {
            //Couldnt locate the file so throw an exception
            FileInput.close();
            throw EngineException("Could not load Material", "Could not locate file: " + FileName);
            return;
        }

        while (FileInput) {
            std::getline(FileInput, line);
            std::strstream streamhandler;
            streamhandler << line;

            //if the line begins with K then it is defining a colour attribute
            if (line[0] == 'K')
            {
                //Ambient
                if (line[1] == 'a')
                {
                    float a, b, c;
                    char space_holder;
                    streamhandler >> space_holder >> space_holder >> a >> b >> c;
                    CurrentMaterial.SetAmbient(ColourRGB((int)(a * MaxColourValue), (int)(b * MaxColourValue), (int)(c * MaxColourValue)));
                    //Parse and set each colour
                }

                //Specular
                else if (line[1] == 's')
                {
                    float a, b, c;
                    char space_holder;
                    streamhandler >> space_holder >> space_holder >> a >> b >> c;
                    CurrentMaterial.SetSpecular(ColourRGB((int)(a * MaxColourValue), (int)(b * MaxColourValue), (int)(c * MaxColourValue)));
                    //Parse and set each colour
                }

                //Diffuse
                else if (line[1] == 'd')
                {
                    float a, b, c;
                    char space_holder;
                    streamhandler >> space_holder >> space_holder >> a >> b >> c;
                    CurrentMaterial.SetDiffuse(ColourRGB((int)(a * MaxColourValue), (int)(b * MaxColourValue), (int)(c * MaxColourValue)));
                    //Parse and set each colour
                }
            }

            else if (line.rfind("newmtl ", 0) == 0)
            {
                if (CurrentKey != "")
                {
                    MaterialMap[CurrentKey] = CurrentMaterial;
                }
                CurrentKey = line.substr(7, line.size() - 7);

                //once a new material is defined, store the previous one, and save the new name
            }
        }
        MaterialMap[CurrentKey] = CurrentMaterial;
        FileInput.close();
        //no need to return as it is by reference
    }

public:
    ColourRGB GetAmbient()
    {
        return AmbientColour;
    }

    ColourRGB GetSpecular()
    {
        return SpecularColour;
    }

    ColourRGB GetDiffuse()
    {
        return DiffuseColour;
    }

    void SetAmbient(ColourRGB NewAmbient)
    {
        AmbientColour = NewAmbient;
    }

    void SetSpecular(ColourRGB NewSpecular)
    {
        SpecularColour = NewSpecular;
    }

    void SetDiffuse(ColourRGB NewDiffuse)
    {
        DiffuseColour = NewDiffuse;
    }

};

