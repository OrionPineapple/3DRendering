#pragma once
#include <math.h>

class ColourRGB
{
protected:
	//Each are integer values between 0 and 255 
	int Red; 
	int Green;
	int Blue;

public:
	ColourRGB()
	{
		Red = 255;
		Green = 255;
		Blue = 255;
	}

	ColourRGB(int R, int G, int B)
	{
		Red = R;
		Green = G;
		Blue = B;
	}

public:
	float GetBrightness()
	{
		//Returns a value between 0 and 1
		//0 being pitch black and 1 being full bright white
		float SquaredSums = (float)(Red * Red) + (float)(Green * Green) + (float)(Blue * Blue);
		SquaredSums /= (255.0f * 255.0f);
		return sqrtf(SquaredSums);
	}

	void SetBrightness(float Brightness)
	{
		//brightness is expected to be a value between 0 and 1
		float Current = GetBrightness();
		float Multiplier = sqrtf(Brightness / Current);
		Red *= Multiplier;
		Green *= Multiplier;
		Blue *= Multiplier;
	}

	int GetRed()
	{
		return Red;
	}

	int GetGreen()
	{
		return Green;
	}

	int GetBlue()
	{
		return Blue;
	}

	static ColourRGB Interpolate(ColourRGB From, ColourRGB To, float Alpha, bool UseTrueInterpolation)
	{
        if (UseTrueInterpolation)
        {
            //an interpolation of two colours that accounts for their non linear values
            //but still a correct and "true" linear intepolation
            float FromR = From.Red * From.Red / 255.0f;
            float FromG = From.Green * From.Green / 255.0f;
            float FromB = From.Blue * From.Blue / 255.0f;

            float ToR = To.Red * To.Red / 255.0f;
            float ToG = To.Green * To.Green / 255.0f;
            float ToB = To.Blue * To.Blue / 255.0f;

            float EndR = FromR + Alpha * (ToR - FromR);
            float EndG = FromG + Alpha * (ToG - FromG);
            float EndB = FromB + Alpha * (ToB - FromB);

            return ColourRGB(
                (int)sqrtf(EndR),
                (int)sqrtf(EndG),
                (int)sqrtf(EndB));
        }


		//Legacy method of linear interpolation between colours
		//only included because most applications do it this way
		//this is wrong however because RGB values are not linear, they are rooted
		return ColourRGB(
			From.Red + Alpha * (To.Red - From.Red),
			From.Green + Alpha * (To.Green - From.Green),
			From.Blue + Alpha * (To.Blue - From.Blue));
	}

	static ColourRGB GetMininum(ColourRGB A, ColourRGB B)
	{
		//Gets the mininum of each channel of colour

        int Re = B.Red;
        int Gr = B.Green;
        int Bl = B.Blue;

        if (A.Red < B.Red) { Re = A.Red; }
        if (A.Green < B.Green) { Gr = A.Green; }
        if (A.Blue < B.Blue) { Bl = A.Blue; }

        return ColourRGB(Re, Gr, Bl);
	}

	static ColourRGB GetMaxinum(ColourRGB A, ColourRGB B)
	{
		//Gets the maxinum of each channel of colour
        int Re = B.Red;
        int Gr = B.Green;
        int Bl = B.Blue;

        if (A.Red > B.Red) { Re = A.Red; }
        if (A.Green > B.Green) { Gr = A.Green; }
        if (A.Blue > B.Blue) { Bl = A.Blue; }

        return ColourRGB(Re, Gr, Bl);
	}

	static ColourRGB Multiply(ColourRGB A, ColourRGB B)
	{
		//multiply two colours by converting from 0->255 to 0->1
        //then mutliply and convert back to the 0->255 format
		return ColourRGB(
			(int)(((float)A.Red * (float)B.Red) / 255.0f),
			(int)(((float)A.Green * (float)B.Green) / 255.0f),
			(int)(((float)A.Blue * (float)B.Blue) / 255.0f));
	}

    ColourRGB operator * (float Scalar)
    {
        //multiples a colour by a scalar to alter intensity
        return ColourRGB((int)(Scalar * (float)Red), (int)(Scalar * (float)Green), (int)(Scalar * (float)Blue));
    }
};

