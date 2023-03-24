#pragma once
#include "Instance.h"
#include "Matrix4x4.h"

namespace InstanceHeirachy 
{
	//should be used as a virtual class as well eg class A : public virtual MatrixInstance

    //this is any object with positional and rotational information
    //we can apply all the standard matrix operations to this
    //so we can rotate and translate components in our world 

	class MatrixInstance : public virtual Instance
	{
	protected:
		Matrix4x4 Matrix;


        MatrixInstance()
        {
            BaseType = InstanceBaseType::MatrixInstanceBaseType;
        }
	public:

		void Transform(Matrix4x4 Transformation)
		{
			Matrix *= Transformation;
		}

		void Translate(Vector3D Vector)
		{
			Matrix.Translate(Vector);
		}

		void RotateX(float XRad)
		{
            Matrix.RotateByX(XRad);
		}

		void RotateY(float YRad)
		{
            Matrix.RotateByY(YRad);
		}

		void RotateZ(float ZRad)
		{
            Matrix.RotateByZ(ZRad);
		}

		void Rotate(Vector3D& Rotation)
		{
            Matrix.RotateBy(Rotation);
		}

		Matrix4x4 GetMatrix()
		{
			return Matrix;
		}

        void SetMatrix(Matrix4x4 NewMatix)
        {
            Matrix = NewMatix;
        }

        Matrix4x4 ExtractTranslationMatrix()
        {
            return Matrix.ExtractTranslationMatrix();
        }

        Matrix4x4 ExtractRotationMatrix()
        {
            return Matrix.ExtractRotationMatrix();
        }

        Vector3D ExtractVectorPosition()
        {
            return Matrix.ExtractVectorPosition();
        }
	};
}

