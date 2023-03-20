#pragma once
#include <iostream>
#include "ColourRGB.h"
#include "EngineExceptions.h"
#include <math.h>

extern "C" 
{
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"
}

class Texture 
{
private:
    int Height;
    int Width;
	std::vector<ColourRGB> Data;

public:
    Texture()
    {
        Height = 1;
        Width = 1;
        Data = std::vector<ColourRGB>(1, ColourRGB());
    }

	Texture(int NewHeight, int NewWidth, ColourRGB Fill)
	{
        Height = NewHeight;
        Width = NewWidth;
        Data = std::vector<ColourRGB>(Height * Width, Fill);
	}

    Texture(std::string FileName)
    {
        int NewWidth, NewHeight, ChunkSize;
        std::vector<unsigned char> Image;
        bool success = LoadImage(Image, FileName, NewWidth, NewHeight, ChunkSize);

        if (!success)
        {
            throw EngineException("Failed to load texture file", "There was an error in loading " + FileName + ". When using an .exe you must have it within the directory specified. If using visual studio ensure it is in the same folder as all the headers too");
            return;
        }

        Height = NewHeight;
        Width = NewWidth;

        Data = std::vector<ColourRGB>(Height * Width, ColourRGB());
        for (int y = 0; y < Height; y++)
        {
            for (int x = 0; x < Width; x++)
            {
                int index = ChunkSize * (y * Width + x);
                int r = static_cast<int>(Image[index + 0]);
                int g = static_cast<int>(Image[index + 1]);
                int b = static_cast<int>(Image[index + 2]);

                Data[y * Width + x] = ColourRGB(r, g, b);
            }
        }

    }

public:

    int GetHeight()
    {
        return Height;
    }

    int GetWidth()
    {
        return Width;
    }

    ColourRGB Get(int x, int y)
    {
        while (x < 0) { x += Width; }
        while (x >= Width) { x -= Width; }
        while (y < 0) { y += Height; }
        while (y >= Width) { y -= Height; }

        return Data[(y * Width) + x];
    }

    ColourRGB GetBiLinearApproximatedValue(float x, float y, bool UseTrueInterpolation = false)
    {
        while (x < 0) { x += 1.0f; }
        while (x >= Width) { x -= 1.0f; }
        while (y < 0) { y += 1.0f; }
        while (y >= Width) { y -= 1.0f; }

        ColourRGB TopLeft, TopRight, BottomLeft, BottomRight;
        GetCornerValues(x, y, TopLeft, TopRight, BottomLeft, BottomRight);

        ColourRGB TopValue = ColourRGB::Interpolate(TopLeft, TopRight, x, UseTrueInterpolation);
        ColourRGB BottomValue = ColourRGB::Interpolate(BottomLeft, BottomRight, x, UseTrueInterpolation);
        return ColourRGB::Interpolate(TopValue, BottomValue, y, UseTrueInterpolation);
    }

    /*ColourRGB GetBiCubicApproximatedValue(float x, float y, bool UseTrueInterpolation = false)
    {
        while (x < 0) { x += 1.0f; }
        while (x >= Width) { x -= 1.0f; }
        while (y < 0) { y += 1.0f; }
        while (y >= Width) { y -= 1.0f; }

        ColourRGB TopLeft, TopRight, BottomLeft, BottomRight;
        GetCornerValues(x, y, TopLeft, TopRight, BottomLeft, BottomRight);

    }*/

private:
    void GetCornerValues(float& x, float& y, ColourRGB& TopLeft, ColourRGB& TopRight, ColourRGB& BottomLeft, ColourRGB& BottomRight)
    {
        int XRounded = truncf(x * (float)Width);
        int YRounded = truncf(y * (float)Height);

        x = x * (float)Width - (float)XRounded;
        y = y * (float)Width - (float)XRounded;

        TopLeft = Get(XRounded, YRounded);
        TopRight = Get(XRounded + 1, YRounded);
        BottomLeft = Get(XRounded, YRounded + 1);
        BottomRight = Get(XRounded + 1, YRounded + 1);
    }

    bool LoadImage(std::vector<unsigned char>& ImageRef, const std::string& FileName, int& NewWidth, int& NewHeight, int& ChunkSize)
    {
        unsigned char* data = stbi_load(FileName.c_str(), &NewWidth, &NewHeight, &ChunkSize, 0);
        if (data != nullptr)
        {
            ImageRef = std::vector<unsigned char>(data, data + NewWidth * NewHeight * ChunkSize);
        }
        stbi_image_free(data);
        return (data != nullptr);
    }

};