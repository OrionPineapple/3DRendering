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
		Red = 0;
		Green = 0;
		Blue = 0;
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
		SquaredSums /= (256.0f * 256.0f);
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

	static ColourRGB Interpolate(ColourRGB From, ColourRGB To, float Alpha)
	{
		//Legacy method of linear interpolation between colours
		//only included because most applications do it this way
		//this is wrong however because RGB values are not linear, they are rooted
		return ColourRGB(
			From.Red + Alpha * (To.Red - From.Red),
			From.Green + Alpha * (To.Green - From.Green),
			From.Blue + Alpha * (To.Blue - From.Blue));
	}

	static ColourRGB TrueInterpolate(ColourRGB From, ColourRGB To, float Alpha)
	{
		//an interpolation of two colours that accounts for their non linear values
		//but still a correct and "true" linear intepolation
		float FromR = From.Red * From.Red / 256.0f;
		float FromG = From.Green * From.Green / 256.0f;
		float FromB = From.Blue * From.Blue / 256.0f;

		float ToR = To.Red * To.Red / 256.0f;
		float ToG = To.Green * To.Green / 256.0f;
		float ToB = To.Blue * To.Blue / 256.0f;

		float EndR = FromR + Alpha * (ToR - FromR);
		float EndG = FromG + Alpha * (ToG - FromG);
		float EndB = FromB + Alpha * (ToB - FromB);

		return ColourRGB(
			(int)sqrtf(EndR),
			(int)sqrtf(EndG),
			(int)sqrtf(EndB));
	}

	static ColourRGB GetMininum(ColourRGB A, ColourRGB B)
	{
		//Gets the mininum of each channel of colour
		return ColourRGB(
			(A.Red < B.Red && A.Red || B.Red),
			(A.Green < B.Green&& A.Green || B.Green),
			(A.Blue < B.Blue && A.Blue || B.Blue));
	}

	static ColourRGB GetMaxinum(ColourRGB A, ColourRGB B)
	{
		//Gets the maxinum of each channel of colour
		return ColourRGB(
			(A.Red > B.Red&& A.Red || B.Red),
			(A.Green > B.Green&& A.Green || B.Green),
			(A.Blue > B.Blue&& A.Blue || B.Blue));
	}

	static ColourRGB Multiply(ColourRGB A, ColourRGB B)
	{
		//multiply two colours
		return ColourRGB(
			(A.Red * B.Red) / 256,
			(A.Blue * B.Blue) / 256,
			(A.Green * B.Green) / 256);
	}
};

