//#include <string>
//#include <iostream>
//#include "Matrix4x4.h"
//
//const float PI = 3.14159265359f;
//void Test1()
//{
//    //Translation
//
//    std::cout << "Translation: ";
//
//    Matrix4x4 Matrix = Matrix4x4::GetTranslationMatrix(Vector3D(5.0f, 0.2f, -7.0f));
//
//    std::cout << std::endl << "Expect a Translation of (5, 0.2, -7): " << std::endl;
//
//    /*
//    1 0 0 5
//    0 1 0 0.2
//    0 0 1 -7
//    0 0 0 1
//    */
//        
//    Matrix4x4::DebugMatrix4x4(Matrix);
//
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//}
//
//void Test2()
//{
//    //Rotation
//
//    std::cout << "Rotation: ";
//
//    //Applies operation in YXZ order
//    Matrix4x4 Matrix = Matrix4x4::GetRotationMatrix(Vector3D(PI * 0.25, PI, -PI * 1.5f));
//
//    std::cout << std::endl << "Expect a Rotation of (Pi / 4, Pi, - 3 Pi / 2): " << std::endl;
//
//    /*
//    1       1       0       0
//    0.707   0       -0.707  0
//    0       0       1       -7
//    -707    0       -0.707  0
//    */
//
//    Matrix4x4::DebugMatrix4x4(Matrix);
//
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//}
//
//void Test3()
//{
//    //Addition and Subraction
//    std::cout << "Addition and Subtraction: ";
//
//    Matrix4x4 Matrix;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            Matrix.SetComponent(i, j, i * j + 4 * j - 7 * i);
//        }
//    }
//
//    Matrix4x4 Matrix2;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            Matrix2.SetComponent(i, j, i * i + j * j + 2 * i + 3 * j);
//        }
//    }
//
//    bool success = true;
//
//    Matrix4x4 Matrix3 = Matrix + Matrix2;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            float Component = Matrix3.GetComponent(i, j);
//            float ShouldBe = i * j + 4 * j - 7 * i + i * i + j * j + 2 * i + 3 * j;
//            if (Component != ShouldBe)
//            {
//                success = false;
//            }
//        }
//    }
//
//    Matrix4x4 Matrix4 = Matrix - Matrix2;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            float Component = Matrix4.GetComponent(i, j);
//            float ShouldBe = (i * j + 4 * j - 7 * i) - (i * i + j * j + 2 * i + 3 * j);
//            if (Component != ShouldBe)
//            {
//                success = false;
//            }
//        }
//    }
//
//
//    if (success)
//    {
//        std::cout << "Success";
//    }
//    else
//    {
//        std::cout << "Fail";
//    }
//
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//}
//
//void Test4()
//{
//    //Inversing Matrices
//    std::cout << "Inversing: ";
//
//    std::cout << std::endl << "All of these outputted matrices should be identity matrices" << std::endl;
//
//    Matrix4x4 Matrix = Matrix4x4::GetTranslationMatrix(Vector3D(2, 3, 4)) * Matrix4x4::GetRotationMatrix(Vector3D(1, 2, 3));
//    Matrix4x4 Matrix2 = Matrix4x4::GetTranslationMatrix(Vector3D(0, -3, 9)) * Matrix4x4::GetRotationMatrix(Vector3D(1.2, 0, 0.3));
//
//
//    //all four should be identity matrices
//    Matrix4x4::DebugMatrix4x4(Matrix2 * Matrix2.Inverse());
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//    Matrix4x4::DebugMatrix4x4(Matrix2.Inverse() * Matrix2);
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//    Matrix4x4::DebugMatrix4x4(Matrix * Matrix.Inverse());
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//    Matrix4x4::DebugMatrix4x4(Matrix.Inverse() * Matrix);
//
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//}
//
//void Test5()
//{
//    //Dot Product
//    std::cout << "Multiplication: ";
//
//    Matrix4x4 Matrix;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            Matrix.SetComponent(i, j, i * j + 4 * j - 7 * i);
//        }
//    }
//
//    /*
//    0   4   8   12
//    -7  -2  3   8  
//    -14 -8  -2  4
//    -21 -14 -7  0
//    */
//
//    Matrix4x4 Matrix2;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//        {
//            Matrix2.SetComponent(i, j, i * i + j * j + 2 * i + 3 * j);
//        }
//    }
//
//    /*
//    0   4   10  18
//    3   7   13  21
//    8   12  18  26
//    15  19  25  33
//    */
//
//
//    std::cout << std::endl << "First two multiply to give the last in the respective order" << std::endl;
//
//    Matrix4x4::DebugMatrix4x4(Matrix);
//    Matrix4x4::DebugMatrix4x4(Matrix2);
//    Matrix4x4::DebugMatrix4x4(Matrix * Matrix2);
//
//    /*
//    256     352     496     688
//    138     146     158     174
//    20      -60     -180    -340
//    -98     -266    -518    -854
//    */
//
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//    std::cout << std::endl << std::endl;
//}
//
//void Test6()
//{
//    //Magnitude and Normalisation
//    std::cout << "Projection: ";
//
//    Vector3D Vec = Vector3D(2.0f, 8.0f, 16.0f);
//    Vector3D ProjectedVec = Matrix4x4::ProjectPoint(Vec, Matrix4x4::CreateProjectionMatrix(1.0f, PI * 0.5, 2.0f, 10.0f));
//
//    std::cout << "Point of (2, 8, 16) projected by aspect ratio 1, fov pi/2, znear 2.0f, zfar 10.0f" << std::endl;
//    std::cout << std::endl << ProjectedVec.GetX() << ", " << ProjectedVec.GetY() << ", " << ProjectedVec.GetZ();
//
//    std::cout << std::endl << std::endl;
//}
//
//int main()
//{
//    Test1();
//    Test2();
//    Test3();
//    Test4();
//    Test5();
//    Test6();
//    return 0;
//}