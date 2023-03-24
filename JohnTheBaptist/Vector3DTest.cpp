//
//#include <string>
//#include <iostream>
//#include "Vector3D.h"
//
//void Test1()
//{
//    //Basic Setting and Getting
//    std::cout << "Setting And Getting: ";
//
//    Vector3D Vec = Vector3D(5.0f, 6.0f, 0.2f);
//
//    if (Vec.GetX() == 5.0f && Vec.GetY() == 6.0f && Vec.GetZ() == 0.2f)
//    {
//        std::cout << "Success";
//    }
//    else
//    {
//        std::cout << "Fail";
//    }
//
//    std::cout << std::endl << std::endl;
//}
//
//void Test2()
//{
//    //Negative Setting and Getting
//    std::cout << "Setting and Getting 2: ";
//
//    Vector3D Vec = Vector3D(-5.7f, -0.3f, -150.05f);
//
//    if (Vec.GetX() == -5.7f && Vec.GetY() == -0.3f && Vec.GetZ() == -150.05f)
//    {
//        std::cout << "Success";
//    }
//    else
//    {
//        std::cout << "Fail";
//    }
//
//    std::cout << std::endl << std::endl;
//}
//
//void Test3()
//{
//    //Addition and Subraction
//    std::cout << "Addition and Subtraction: ";
//
//    Vector3D Vec = Vector3D(4.0f, 5.0f, -14.0f);
//    Vector3D Vec2 = Vector3D(6.0f, -5.0f, 50.0f);
//
//
//    if (Vec + Vec2 == Vector3D(10.0f, 0.0f, 36.0f) && Vec2 - Vec == Vector3D(2.0f, -10.0f, 64.0f))
//    {
//        std::cout << "Success";
//    }
//    else
//    {
//        std::cout << "Fail";
//    }
//
//    std::cout << std::endl << std::endl;
//}
//
//void Test4()
//{
//    //Multiply and Divide
//    std::cout << "Multiplication and Division: ";
//
//    Vector3D Vec = Vector3D(4.0f, 5.0f, -0.2f);
//    Vector3D Vec2 = Vector3D(6.0f, -24.0f, 3.0f);
//
//
//    if (Vec * 3.0f == Vector3D(12.0f, 15.0f, -0.6f) && Vec2 / 2.0f == Vector3D(3.0f, -12.0f, 1.5f))
//    {
//        std::cout << "Success";
//    }
//    else
//    {
//        std::cout << "Fail";
//    }
//
//    std::cout << std::endl << std::endl;
//}
//
//void Test5()
//{
//    //Dot Product
//    std::cout << "Dot Product: ";
//
//    Vector3D Vec = Vector3D(4.0f, 5.0f, 0.2f);
//    Vector3D Vec2 = Vector3D(6.0f, -24.0f, -5.0f);
//
//
//    if (Vector3D::Dot(Vec, Vec2) == -97.0f && Vector3D::Dot(Vec, Vec2) == Vector3D::Dot(Vec2, Vec))
//    {
//        std::cout << "Success";
//    }
//    else
//    {
//        std::cout << "Fail";
//    }
//
//    std::cout << std::endl << std::endl;
//}
//
//void Test6()
//{
//    //Magnitude and Normalisation
//    std::cout << "Magnitude and Normalisation: ";
//
//    Vector3D Vec = Vector3D(2.0f, 8.0f, 16.0f);
//    Vector3D Vec2 = Vector3D(6.0f, -24.0f, 0.2f);
//
//
//    if (Vec.GetMagnitude() == 18.0f && Vector3D::Normalise(Vec2).GetMagnitude() == 1.0f)
//    {
//        std::cout << "Success";
//    }
//    else
//    {
//        std::cout << "Fail";
//    }
//
//    std::cout << std::endl << std::endl;
//}
//
//void Test7()
//{
//    //Interpolation
//    std::cout << "Interpolation: ";
//
//    Vector3D Vec = Vector3D(4.0f, 8.0f, -3.0f);
//    Vector3D Vec2 = Vector3D(6.0f, -24.0f, 7.0f);
//
//
//    if (Vector3D::Interpolate(Vec, Vec2, 0.3f) == Vector3D::Interpolate(Vec2, Vec, 0.7f) && Vector3D::Interpolate(Vec, Vec2, 0.5f) == Vector3D(5.0f, -8.0f, 2.0f))
//    {
//        std::cout << "Success";
//    }
//    else
//    {
//        std::cout << "Fail";
//    }
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
//    Test7();
//    return 0;
//}
//
//
