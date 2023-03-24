//#include "Vector2D.h"
//#include <string>
//#include <iostream>
//
//void Test1()
//{
//    //Basic Setting and Getting
//    std::cout << "Setting And Getting: ";
//
//    Vector2D Vec = Vector2D(5.0f, 6.0f);
//
//    if (Vec.GetX() == 5.0f && Vec.GetY() == 6.0f)
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
//    Vector2D Vec = Vector2D(-5.7f, -0.3f);
//
//    if (Vec.GetX() == -5.7f && Vec.GetY() == -0.3f)
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
//    Vector2D Vec = Vector2D(4.0f, 5.0f);
//    Vector2D Vec2 = Vector2D(6.0f, -5.0f);
//
//
//    if (Vec + Vec2 == Vector2D(10.0f, 0.0f) && Vec2 - Vec == Vector2D(2.0f, -10.0f))
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
//    Vector2D Vec = Vector2D(4.0f, 5.0f);
//    Vector2D Vec2 = Vector2D(6.0f, -24.0f);
//
//
//    if (Vec * 3.0f == Vector2D(12.0f, 15.0f) && Vec2 / 2.0f == Vector2D(3.0f, -12.0f))
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
//    Vector2D Vec = Vector2D(4.0f, 5.0f);
//    Vector2D Vec2 = Vector2D(6.0f, -24.0f);
//
//
//    if (Vector2D::Dot(Vec, Vec2) == -96.0f && Vector2D::Dot(Vec, Vec2) == Vector2D::Dot(Vec2, Vec))
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
//    Vector2D Vec = Vector2D(4.0f, 3.0f);
//    Vector2D Vec2 = Vector2D(6.0f, -24.0f);
//
//
//    if (Vec.GetMagnitude() == 5.0f && Vector2D::Normalise(Vec2).GetMagnitude() == 1.0f)
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
//    Vector2D Vec = Vector2D(4.0f, 8.0f);
//    Vector2D Vec2 = Vector2D(6.0f, -24.0f);
//
//
//    if (Vector2D::Interpolate(Vec, Vec2, 0.3f) == Vector2D::Interpolate(Vec2, Vec, 0.7f) && Vector2D::Interpolate(Vec, Vec2, 0.5f) == Vector2D(5.0f, -8.0f))
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
