#pragma once
#include "ColourRGB.h"

class Texture 
{
protected:
	int ReferenceCount;
	ColourRGB Data[];

public:
	Texture(int Height, int Width, ColourRGB Fill)
	{
		ReferenceCount = 0;
		*Data = *(new ColourRGB[Height * Width]);

	}

public:
	void IncrementRefCount()
	{
		ReferenceCount++;
	}

	void DecrementRefCount()
	{
		ReferenceCount--;
		if (ReferenceCount == 0)
		{
			//destroy self
		}
	}

};