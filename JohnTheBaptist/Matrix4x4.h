#pragma once
#include <math.h>
#include "Vector3D.h"

class Matrix4x4
{
private:
	float Components[4][4] = {	{1, 0, 0, 0}, 
								{0, 1, 0, 0}, 
								{0, 0, 1, 0}, 
								{0, 0, 0, 1} };
	//Initialised to Identity Matrix


	//Constructors
public:
	Matrix4x4() 
	{
		//returns identity matrix
	}

	static Matrix4x4 GetTranslationMatrix(Vector3D Translation)
	{
        //translation uses the 4th column as a vertical vector
		Matrix4x4 Matrix = Matrix4x4();
		Matrix.SetComponent(0, 3, Translation.GetX());
		Matrix.SetComponent(1, 3, Translation.GetY());
		Matrix.SetComponent(2, 3, Translation.GetZ());
		return Matrix;
	}

	static Matrix4x4 GetRotationMatrix(Vector3D Rotation)
	{
		//Applies operation in YXZ order
		//yaw then pitch then roll
		//this means we should multiply in the order 

		return GetYRotationMatrix(Rotation.GetY())
			* GetXRotationMatrix(Rotation.GetX())
			* GetZRotationMatrix(Rotation.GetZ());

	}

	static Matrix4x4 GetInverseRotationMatrix(Vector3D Rotation)
	{
        //apply rotations in opposite order using a negative to reverse
		return GetZRotationMatrix(-Rotation.GetZ())
			* GetXRotationMatrix(-Rotation.GetX())
			* GetYRotationMatrix(-Rotation.GetY());

        //proof:
        /*
        YXZ * Z-1 X-1 Y-1 == I
        Z-1 X-1 Y-1 * YXZ == I
        */
	}

	static Matrix4x4 GetXRotationMatrix(float x)
	{
		//always in radians
        //refer to any maths formulae booklet for proof
		Matrix4x4 Rotation = Matrix4x4();

		float cosine = cos(x);
		float sine = sin(x);

		Rotation.Components[1][1] = cosine;
		Rotation.Components[2][2] = cosine;

		Rotation.Components[1][2] = -sine;
		Rotation.Components[2][1] = sine;

		return Rotation;
	}

	static Matrix4x4 GetYRotationMatrix(float y)
	{
		//always in radians
        //refer to any maths formulae booklet for proof
		Matrix4x4 Rotation = Matrix4x4();

		float cosine = cos(y);
		float sine = sin(y);

		Rotation.Components[0][0] = cosine;
		Rotation.Components[2][2] = cosine;

		Rotation.Components[2][0] = -sine;
		Rotation.Components[0][2] = sine;

		return Rotation;
	}

	static Matrix4x4 GetZRotationMatrix(float z)
	{
		//always in radians
        //refer to any maths formulae booklet for proof
		Matrix4x4 Rotation = Matrix4x4();

		float cosine = cos(z);
		float sine = sin(z);

		Rotation.Components[0][0] = cosine;
		Rotation.Components[1][1] = cosine;

		Rotation.Components[0][1] = -sine;
		Rotation.Components[1][0] = sine;

		return Rotation;
	}

	static void DebugMatrix4x4(Matrix4x4 Matrix)
	{
		std::cout << "Matrix4x4:" << std::endl;
        std::cout << Matrix.GetDeterminant() << std::endl;
		for (int Row = 0; Row < 4; Row++)
		{
			for (int Column = 0; Column < 4; Column++)
			{
				std::cout << Matrix.Components[Row][Column] << "	" << "	" << "  ";
			}
			std::cout << std::endl;
		}
	}

	//Projection

    Matrix4x4 ExtractTranslationMatrix()
    {
        //as the translation is only in the 4th column
        //we can get an identity matrix, and copy it
        Matrix4x4 A;
        A.SetComponent(0, 3, this->GetComponent(0, 3));
        A.SetComponent(1, 3, this->GetComponent(1, 3));
        A.SetComponent(2, 3, this->GetComponent(2, 3));
        A.SetComponent(3, 3, 1);
        return A;
    }

    Vector3D ExtractVectorPosition()
    {
        return Vector3D(this->GetComponent(0, 3), this->GetComponent(1, 3), this->GetComponent(2, 3));
    }

    Matrix4x4 ExtractRotationMatrix()
    {
        //as the rotation is only in inner 3x3
        //we can get an identity matrix, and copy it
        Matrix4x4 A = Matrix4x4(*this);
        A.SetComponent(0, 3, 0);
        A.SetComponent(1, 3, 0);
        A.SetComponent(2, 3, 0);
        A.SetComponent(3, 3, 1);
        return A;
    }

	static Matrix4x4 CreateProjectionMatrix(float AspectRatio, float VerticalFOV, float ZNear, float ZFar)
	{
		Matrix4x4 Matrix;
        //apply the projection matrix in anaylsis

		float t = 1.0f / tan(VerticalFOV / 2.0f);
		float q = (ZFar / (ZFar - ZNear));


		Matrix.SetComponent(0, 0, AspectRatio * t);
		Matrix.SetComponent(1, 1, t);
		Matrix.SetComponent(2, 2, q);
		Matrix.SetComponent(3, 3, 0);

		Matrix.SetComponent(3, 2, 1);
		Matrix.SetComponent(2, 3, -1.0f * ZNear * q);

		return Matrix;
	}

	static Vector3D ProjectPoint(Vector3D Point, Matrix4x4 Matrix)
	{
		Point = Matrix * Point;
		float w = Point.GetW();
		if (w == 0.0f)
		{
			w = 1.0f;
		}
		w = 1.0f / w;
        //divison by w for depth/parralax
		return Vector3D(Point.GetX() * w, Point.GetY() * w, Point.GetZ() * w, Point.GetW());
	}


	//Getters and Setters
public:
	float GetComponent(int Row, int Column)
	{
		return Components[Row][Column];
	}

	void SetComponent(int Row, int Column, float Value)
	{
		Components[Row][Column] = Value;
	}

	//Standard Matrix Oporations
public:
	float GetDeterminant()
	{
        //Doing this algorithmically was too slow
        //so i spent far too long multiplying this out
        return Components[0][0] * (Components[1][1] * (Components[2][2] * Components[3][3] 
            - Components[3][2] * Components[2][3]) 
            - Components[1][2] * (Components[2][1] * Components[3][3] 
                - Components[3][1] * Components[2][3]) 
            + Components[1][3] * (Components[2][1] * Components[3][2] 
                - Components[3][1] * Components[2][2])) 
            - Components[0][1] * (Components[1][0] * (Components[2][2] * Components[3][3] 
                - Components[3][2] * Components[2][3]) 
                - Components[1][2] * (Components[2][0] * Components[3][3] 
                    - Components[3][0] * Components[2][3]) 
                + Components[1][3] * (Components[2][0] * Components[3][2] 
                    - Components[3][0] * Components[2][2])) 
            + Components[0][2] * (Components[1][0] * (Components[2][1] * Components[3][3] 
                - Components[3][1] * Components[2][3]) 
                - Components[1][1] * (Components[2][0] * Components[3][3] 
                    - Components[3][0] * Components[2][3]) 
                + Components[1][3] * (Components[2][0] * Components[3][1] 
                    - Components[3][0] * Components[2][1])) 
            - Components[0][3] * (Components[1][0] * (Components[2][1] * Components[3][2] 
                - Components[3][1] * Components[2][2]) 
                - Components[1][1] * (Components[2][0] * Components[3][2] 
                    - Components[3][0] * Components[2][2]) 
                + Components[1][2] * (Components[2][0] * Components[3][1] 
                    - Components[3][0] * Components[2][1]));
	}

    bool IsIdentity()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == j)
                {
                    if (Components[i][j] != 1)
                    {
                        return false;
                    }
                }
                else
                {
                    if (Components[i][j] != 0)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    Matrix4x4 Multiply(Matrix4x4 B)
    {
        Matrix4x4 Output = Matrix4x4();
        for (int Row = 0; Row < 4; Row++)
        {
            for (int Column = 0; Column < 4; Column++)
            {
                Output.SetComponent(Row, Column, DotRowColumn(Row, Column, B));
            }
        }
        return Output;
    }

    Matrix4x4 Multiply(float Scalar)
    {
        Matrix4x4 Output = Matrix4x4();
        for (int Row = 0; Row < 4; Row++)
        {
            for (int Column = 0; Column < 4; Column++)
            {
                float Value = GetComponent(Row, Column) * Scalar;
                Output.SetComponent(Row, Column, Value);
            }
        }
        return Output;
    }

    void Translate(Vector3D& Vector)
    {
        SetComponent(0, 3, GetComponent(0, 3) + Vector.GetX());
        SetComponent(1, 3, GetComponent(1, 3) + Vector.GetY());
        SetComponent(2, 3, GetComponent(2, 3) + Vector.GetZ());
    }

    Vector3D VectorMultiply(Vector3D& Vector)
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;

        float vx = Vector.GetX();
        float vy = Vector.GetY();
        float vz = Vector.GetZ();
        float vw = Vector.GetW();

        x = Components[0][0] * vx + Components[0][1] * vy + Components[0][2] * vz + Components[0][3] * vw;
        y = Components[1][0] * vx + Components[1][1] * vy + Components[1][2] * vz + Components[1][3] * vw;
        z = Components[2][0] * vx + Components[2][1] * vy + Components[2][2] * vz + Components[2][3] * vw;
        w = Components[3][0] * vx + Components[3][1] * vy + Components[3][2] * vz + Components[3][3] * vw;

        return Vector3D(x, y, z, w);
    }

    Matrix4x4 Inverse()
    {
        Matrix4x4 I;

        //This took forever
        //but doing it algorithmically is WAAYYY too slow

        I.Components[0][0] = (Components[1][1] * Components[2][2] * Components[3][3]) + (Components[1][2] * Components[2][3] * Components[3][1]) + (Components[1][3] * Components[2][1] * Components[3][2]) - (Components[1][1] * Components[2][3] * Components[3][2]) - (Components[1][2] * Components[2][1] * Components[3][3]) - (Components[1][3] * Components[2][2] * Components[3][1]);
        I.Components[0][1] = (Components[0][1] * Components[2][3] * Components[3][2]) + (Components[0][2] * Components[2][1] * Components[3][3]) + (Components[0][3] * Components[2][2] * Components[3][1]) - (Components[0][1] * Components[2][2] * Components[3][3]) - (Components[0][2] * Components[2][3] * Components[3][1]) - (Components[0][3] * Components[2][1] * Components[3][2]);
        I.Components[0][2] = (Components[0][1] * Components[1][2] * Components[3][3]) + (Components[0][2] * Components[1][3] * Components[3][1]) + (Components[0][3] * Components[1][1] * Components[3][2]) - (Components[0][1] * Components[1][3] * Components[3][2]) - (Components[0][2] * Components[1][1] * Components[3][3]) - (Components[0][3] * Components[1][2] * Components[3][1]);
        I.Components[0][3] = (Components[0][1] * Components[1][3] * Components[2][2]) + (Components[0][2] * Components[1][1] * Components[2][3]) + (Components[0][3] * Components[1][2] * Components[2][1]) - (Components[0][1] * Components[1][2] * Components[2][3]) - (Components[0][2] * Components[1][3] * Components[2][1]) - (Components[0][3] * Components[1][1] * Components[2][2]);

        I.Components[1][0] = (Components[1][0] * Components[2][3] * Components[3][2]) + (Components[1][2] * Components[2][0] * Components[3][3]) + (Components[1][3] * Components[2][2] * Components[3][0]) - (Components[1][0] * Components[2][2] * Components[3][3]) - (Components[1][2] * Components[2][3] * Components[3][0]) - (Components[1][3] * Components[2][0] * Components[3][2]);
        I.Components[1][1] = (Components[0][0] * Components[2][2] * Components[3][3]) + (Components[0][2] * Components[2][3] * Components[3][0]) + (Components[0][3] * Components[2][0] * Components[3][2]) - (Components[0][0] * Components[2][3] * Components[3][2]) - (Components[0][2] * Components[2][0] * Components[3][3]) - (Components[0][3] * Components[2][2] * Components[3][0]);
        I.Components[1][2] = -1 * ((Components[0][0] * Components[1][2] * Components[3][3]) - (Components[0][2] * Components[1][0] * Components[3][3]) + (Components[0][3] * Components[1][0] * Components[3][2]) - (Components[0][0] * Components[1][3] * Components[3][2]) + (Components[0][2] * Components[3][0] * Components[1][3]) - (Components[0][3] * Components[1][2] * Components[3][0]));
        I.Components[1][3] = (Components[0][0] * Components[1][2] * Components[2][3]) + (Components[0][2] * Components[1][3] * Components[2][0]) + (Components[0][3] * Components[1][0] * Components[2][2]) - (Components[0][0] * Components[1][3] * Components[2][2]) - (Components[0][2] * Components[1][0] * Components[2][3]) - (Components[0][3] * Components[1][2] * Components[2][0]);

        I.Components[2][0] = (Components[1][0] * Components[2][1] * Components[3][3]) + (Components[1][1] * Components[2][3] * Components[3][0]) + (Components[1][3] * Components[2][0] * Components[3][1]) - (Components[1][0] * Components[2][3] * Components[3][1]) - (Components[1][1] * Components[2][0] * Components[3][3]) - (Components[1][3] * Components[2][1] * Components[3][0]);
        I.Components[2][1] = (Components[0][0] * Components[2][3] * Components[3][1]) + (Components[0][1] * Components[2][0] * Components[3][3]) + (Components[0][3] * Components[2][1] * Components[3][0]) - (Components[0][0] * Components[2][1] * Components[3][3]) - (Components[0][1] * Components[2][3] * Components[3][0]) - (Components[0][3] * Components[2][0] * Components[3][1]);
        I.Components[2][2] = (Components[0][0] * Components[1][1] * Components[3][3]) + (Components[0][1] * Components[1][3] * Components[3][0]) + (Components[0][3] * Components[1][0] * Components[3][1]) - (Components[0][0] * Components[1][3] * Components[3][1]) - (Components[0][1] * Components[1][0] * Components[3][3]) - (Components[0][3] * Components[1][1] * Components[3][0]);
        I.Components[2][3] = (Components[0][0] * Components[1][3] * Components[2][1]) + (Components[0][1] * Components[1][0] * Components[2][3]) + (Components[0][3] * Components[1][1] * Components[2][0]) - (Components[0][0] * Components[1][1] * Components[2][3]) - (Components[0][1] * Components[1][3] * Components[2][0]) - (Components[0][3] * Components[1][0] * Components[2][1]);

        I.Components[3][0] = (Components[1][0] * Components[2][2] * Components[3][1]) + (Components[1][1] * Components[2][0] * Components[3][2]) + (Components[1][2] * Components[2][1] * Components[3][0]) - (Components[1][0] * Components[2][1] * Components[3][2]) - (Components[1][1] * Components[2][2] * Components[3][0]) - (Components[1][2] * Components[2][0] * Components[3][1]);
        I.Components[3][1] = (Components[0][0] * Components[2][1] * Components[3][2]) + (Components[0][1] * Components[2][2] * Components[3][0]) + (Components[0][2] * Components[2][0] * Components[3][1]) - (Components[0][0] * Components[2][2] * Components[3][1]) - (Components[0][1] * Components[2][0] * Components[3][2]) - (Components[0][2] * Components[2][1] * Components[3][0]);
        I.Components[3][2] = (Components[0][0] * Components[1][2] * Components[3][1]) + (Components[0][1] * Components[1][0] * Components[3][2]) + (Components[0][2] * Components[1][1] * Components[3][0]) - (Components[0][0] * Components[1][1] * Components[3][2]) - (Components[0][1] * Components[1][2] * Components[3][0]) - (Components[0][2] * Components[1][0] * Components[3][1]);
        I.Components[3][3] = (Components[0][0] * Components[1][1] * Components[2][2]) + (Components[0][1] * Components[1][2] * Components[2][0]) + (Components[0][2] * Components[1][0] * Components[2][1]) - (Components[0][0] * Components[1][2] * Components[2][1]) - (Components[0][1] * Components[1][0] * Components[2][2]) - (Components[0][2] * Components[1][1] * Components[2][0]);

        float Det = GetDeterminant();
        if (Det != 0)
        {
            I /= Det;
        }

        return I;
    }

    Matrix4x4 Add(Matrix4x4& B)
    {
        Matrix4x4 Output = Matrix4x4();
        for (int Row = 0; Row < 4; Row++)
        {
            for (int Column = 0; Column < 4; Column++)
            {
                float Value = GetComponent(Row, Column) + B.GetComponent(Row, Column);
                Output.SetComponent(Row, Column, Value);
            }
        }
        return Output;
    }

    Matrix4x4 Subtract(Matrix4x4& B)
    {
        Matrix4x4 Output = Matrix4x4();
        for (int Row = 0; Row < 4; Row++)
        {
            for (int Column = 0; Column < 4; Column++)
            {
                float Value = GetComponent(Row, Column) - B.GetComponent(Row, Column);
                Output.SetComponent(Row, Column, Value);
            }
        }
        return Output;
    }

    void RotateBy(Vector3D& Rotation)
    {
        *this = Multiply(GetRotationMatrix(Rotation));
    }

    void RotateByX(float X)
    {
        *this = Multiply(GetXRotationMatrix(X));
    }

    void RotateByY(float Y)
    {
        *this = Multiply(GetYRotationMatrix(Y));
    }

    void RotateByZ(float Z)
    {
        *this = Multiply(GetZRotationMatrix(Z));
    }

    //operator overiding
public:
    Matrix4x4 operator * (float Scalar)
    {
        return Multiply(Scalar);
    }

    Matrix4x4 operator / (float Scalar)
    {
        return Multiply(1.0f / Scalar);
    }

    Matrix4x4 operator * (Matrix4x4 Matrix)
    {
        return Multiply(Matrix);
    }

    Vector3D operator * (Vector3D Vector)
    {
        return VectorMultiply(Vector);
    }

    Matrix4x4 operator + (Vector3D Vector)
    {
        return Matrix4x4(*this) * GetTranslationMatrix(Vector);
    }

    Matrix4x4 operator + (Matrix4x4 B)
    {
        return Add(B);
    }

    Matrix4x4 operator - (Matrix4x4 B)
    {
        return Subtract(B);
    }

    Matrix4x4 operator -()
    {
        return Multiply(-1.0f);
    }

    Matrix4x4 operator !()
    {
        return Inverse();
    }

    Matrix4x4& operator *= (float Scalar)
    {
        *this = Multiply(Scalar);
        return *this;
    }

    Matrix4x4& operator /= (float Scalar)
    {
        *this = Multiply(1.0f / Scalar);
        return *this;
    }

    Matrix4x4& operator *= (Matrix4x4& Matrix)
    {
        *this = Multiply(Matrix);
        return *this;
    }

    //note that there is not *= operator for Vectors, 
    //as multiplication by a vector returns a vector not a matrix

    Matrix4x4& operator += (Matrix4x4& B)
    {
        *this = Add(B);
        return *this;
    }

    Matrix4x4& operator -= (Matrix4x4& B)
    {
        *this = Subtract(B);
        return *this;
    }

private:
    //Helper Operations

    float DotRowColumn(int Row, int Column, Matrix4x4 B)
    {
        float Sum = 0.0f;

        for (int i = 0; i < 4; i++)
        {
            Sum += GetComponent(Row, i) * B.GetComponent(i, Column);
        }

        return Sum;
    }
};

