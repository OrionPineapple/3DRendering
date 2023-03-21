#pragma once


#include "ColourRGB.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <fstream>
#include <map>
#include <string>

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
        std::ifstream FileInput;
        std::string line = "";
        FileInput.open(FileName, std::ios::in);

        std::string CurrentKey = "";
        Material CurrentMaterial;

        while (FileInput) {
            std::getline(FileInput, line);
            //std::cout << line << std::endl;
            std::strstream streamhandler;
            streamhandler << line;

            if (line[0] == 'K')
            {
                //Ambient
                if (line[1] == 'a')
                {
                    float a, b, c;
                    char space_holder;
                    streamhandler >> space_holder >> space_holder >> a >> b >> c;
                    CurrentMaterial.SetAmbient(ColourRGB(a, b, c));
                }

                //Specular
                else if (line[1] == 's')
                {
                    float a, b, c;
                    char space_holder;
                    streamhandler >> space_holder >> space_holder >> a >> b >> c;
                    CurrentMaterial.SetSpecular(ColourRGB(a, b, c));
                }

                //Diffuse
                else if (line[1] == 'd')
                {
                    float a, b, c;
                    char space_holder;
                    streamhandler >> space_holder >> space_holder >> a >> b >> c;
                    CurrentMaterial.SetDiffuse(ColourRGB(a, b, c));
                }
            }

            else if (line.rfind("newmtl ", 0) == 0)
            {
                MaterialMap[CurrentKey] = CurrentMaterial;
                CurrentKey = line.substr(7, line.size() - 7);
            }
        }
        FileInput.close();
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

