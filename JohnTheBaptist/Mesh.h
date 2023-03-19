#pragma once

#include <math.h>
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "Point.h"
#include "Texture.h"
#include "Triangle.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <fstream>

class Mesh
{
protected:
	std::vector<std::shared_ptr<Point>> PointArray;
	std::vector< std::shared_ptr<Triangle>> TriangleArray;

public:
	Mesh()
	{
        PointArray = std::vector<std::shared_ptr<Point>>();
        TriangleArray = std::vector<std::shared_ptr<Triangle>>();
	}

private:
	//mesh loading and saving helper functions
	static std::vector<std::string> SplitString(std::string StringToSplit, std::string Splitter)
	{
		int PreviousIndex = 0;
		int NextIndex = StringToSplit.find(Splitter, PreviousIndex);
		int Length = StringToSplit.length();
		std::vector<std::string> SplitResult = std::vector<std::string>();

		while (NextIndex != std::string::npos)
		{
			SplitResult.push_back(StringToSplit.substr(PreviousIndex, NextIndex - PreviousIndex));
			PreviousIndex = NextIndex + Splitter.length();
			NextIndex = StringToSplit.find(Splitter, PreviousIndex);
		}

		SplitResult.push_back(StringToSplit.substr(PreviousIndex, Length - PreviousIndex));
		return SplitResult;
	}

	static void InterpretFaceData(std::string &vertex, int &Point, int &Normal, bool &UseNormal)
	{
		std::vector<std::string> SplitData = SplitString(vertex, "/");
		std::strstream streamhandler;
		if (SplitData.size() == 1)
		{
			UseNormal = false;
			streamhandler << SplitData[0];
			streamhandler >> Point;
		}
		else 
		{
			streamhandler << SplitData[0];
			streamhandler >> Point;
			
			streamhandler.clear();

			streamhandler << SplitData[2];
			streamhandler >> Normal;
		}
	}

public:
    Mesh(std::string ObjFileName)
	{
        PointArray = std::vector<std::shared_ptr<Point>>();
        TriangleArray = std::vector<std::shared_ptr<Triangle>>();

		std::ifstream FileInput;
		std::string line = "";
		FileInput.open(ObjFileName, std::ios::in);

		std::vector<Vector3D> NormalData;

		while (FileInput) {
			std::getline(FileInput, line);
			//std::cout << line << std::endl;
			std::strstream streamhandler;
			streamhandler << line;

			if (line[0] == 'v')
			{
				//VertexNormalData
				if (line[1] == 'n') 
				{
					float a, b, c;
					char space_holder;
					streamhandler >> space_holder >> space_holder >> a >> b >> c;
					NormalData.push_back(Vector3D(a, b, c, 0.0f));
				}

				//VertexData
				else if (line[1] == ' ')
				{
					float a, b, c;
					char space_holder;
					streamhandler >> space_holder >> a >> b >> c;
					PointArray.push_back(std::move(std::shared_ptr<Point>(new Point(Vector3D(a, b, c)))));
				}


			}

			else if (line[0] == 'f')
			{
				std::vector<std::string> FaceData = SplitString(line.substr(2, line.size() - 1), " ");

				if (FaceData.size() == 3)
				{
					//only one triangle, easy to handle
					int p1, p2, p3;
					int n1, n2, n3;
					bool NormalsUsed = true;

					InterpretFaceData(FaceData[0], p1, n1, NormalsUsed);
					InterpretFaceData(FaceData[1], p2, n2, NormalsUsed);
					InterpretFaceData(FaceData[2], p3, n3, NormalsUsed);

					if (p1 < 1) { p1 = PointArray.size() + p1 + 1; }
					if (p2 < 1) { p2 = PointArray.size() + p2 + 1; }
					if (p3 < 1) { p3 = PointArray.size() + p3 + 1; }

					if (NormalsUsed)
					{
						TriangleArray.push_back(std::shared_ptr<Triangle>(
							new Triangle(
								PointArray[p1 - 1],
								PointArray[p2 - 1],
								PointArray[p3 - 1],
								NormalData[n1 - 1],
								NormalData[n2 - 1],
								NormalData[n3 - 1])));
					}
					else 
					{
						TriangleArray.push_back(std::shared_ptr<Triangle>(
							new Triangle(
								PointArray[p1 - 1],
								PointArray[p2 - 1],
								PointArray[p3 - 1])));
					}
				}

				else if (FaceData.size() == 4)
				{
					//a square is a trivial case, no ear clipping needed
					//Triangle 1 2 3 and 3 4 1

					int p1, p2, p3, p4;
					int n1, n2, n3, n4;
					bool NormalsUsed = true;

					InterpretFaceData(FaceData[0], p1, n1, NormalsUsed);
					InterpretFaceData(FaceData[1], p2, n2, NormalsUsed);
					InterpretFaceData(FaceData[2], p3, n3, NormalsUsed);
					InterpretFaceData(FaceData[3], p4, n4, NormalsUsed);

					if (p1 < 1) { p1 = PointArray.size() + p1 + 1; }
					if (p2 < 1) { p2 = PointArray.size() + p2 + 1; }
					if (p3 < 1) { p3 = PointArray.size() + p3 + 1; }
					if (p4 < 1) { p4 = PointArray.size() + p4 + 1; }

					if (NormalsUsed)
					{
						TriangleArray.push_back(std::shared_ptr<Triangle>(
							new Triangle(
								PointArray[p1 - 1],
								PointArray[p2 - 1],
								PointArray[p3 - 1],
								NormalData[n1 - 1],
								NormalData[n2 - 1],
								NormalData[n3 - 1])));

						TriangleArray.push_back(std::shared_ptr<Triangle>(
							new Triangle(
								PointArray[p3 - 1],
								PointArray[p4 - 1],
								PointArray[p1 - 1],
								NormalData[n3 - 1],
								NormalData[n4 - 1],
								NormalData[n1 - 1])));
					}
					else
					{
						TriangleArray.push_back(std::shared_ptr<Triangle>(
							new Triangle(
								PointArray[p1 - 1],
								PointArray[p2 - 1],
								PointArray[p3 - 1])));


						TriangleArray.push_back(std::shared_ptr<Triangle>(
							new Triangle(
								PointArray[p3 - 1],
								PointArray[p4 - 1],
								PointArray[p1 - 1])));
					}
				}

				else 
				{
					//Prepare data for Ear Clipping Triangulation

                    std::vector<int> PointIndex;
                    std::vector<int> NormalIndex;
                    bool NormalsUsed = true;

                    int point, normal;

                    for (int i = 0; i < FaceData.size(); i++)
                    {
                        InterpretFaceData(FaceData[i], point, normal, NormalsUsed);
                        PointIndex.push_back(point - 1);
                        NormalIndex.push_back(normal - 1);
                    }

                    TriangulateFace(PointIndex, NormalData, NormalIndex, NormalsUsed);
				}


				//int p1, p2, p3;
				//char space_holder;
				//streamhandler >> space_holder >> p1 >> p2 >> p3;

				//std::shared_ptr<Triangle> NewTriangle = std::shared_ptr<Triangle>(new Triangle(NewMesh.PointArray[p1 - 1], NewMesh.PointArray[p2 - 1], NewMesh.PointArray[p3 - 1]));
				////order is p3 then p2 then p1 as .obj uses anti-clockwise, we use clockwise
				//NewMesh.TriangleArray.push_back(std::move(NewTriangle));
			}
		}

		FileInput.close();

		std::cout << std::endl << "Mesh: " << ObjFileName << std::endl << "Tris: " << TriangleArray.size() << std::endl << "Points: " << PointArray.size() << std::endl;

		//for each line in file

		//if line is 'v' then
		//create new point in PointArray with its coordinates
		//eg v -1.0 1.000000 1.000000

		//if line is 'f' then create triangle with reference to points given in line
		//eg f 1 3 4
		//1 would reference the first point instanced which would be PointArray[0] :)
	}

public:
	bool MeshToObjectFile(std::string FileName, std::string FileDescription = "This is a mesh file from this game engine!!!!", bool Overwrite = false)
	{
		//if not overwrite then check if the file exists firstly

		//First of all write a brief Description
		
		//Then output all of the vertexes
		//with format 
		//v {x} {y} {z}
		//v -1.0 5.6 3.3

		//then output the triangles
		//with format 
		//f {i1} {i2} {i3}
		//where i represents the index of the point of the triangle written to the file previosly
		//it is important to note that i starts at 1 wheras our array starts at 0
		//f 2 3 5

		std::ofstream OutputOBJ(FileName);
		OutputOBJ << "#" << FileDescription << std::endl;

		for(std::shared_ptr<Point> Vertex : PointArray)
		{
			Vector3D Vector = Vertex->GetPosition();
			OutputOBJ << "v " << Vector.GetX() << " " << Vector.GetY() << " " << Vector.GetZ() << std::endl;
		}

		for (std::shared_ptr<Triangle> Face : TriangleArray)
		{
			std::shared_ptr<Point> A = Face->GetPointA();
			std::shared_ptr<Point> B = Face->GetPointB();
			std::shared_ptr<Point> C = Face->GetPointC();

			int IndexA = FindIndexOfVertex(A) + 1;
			int IndexB = FindIndexOfVertex(B) + 1;
			int IndexC = FindIndexOfVertex(C) + 1;

			OutputOBJ << "f " << IndexA << " " << IndexB << " " << IndexC << std::endl;
		}


		OutputOBJ.close();
		return true;
	}

public:
    std::vector<std::shared_ptr<Point>> GetPointReferences()
    {
        return std::vector<std::shared_ptr<Point>>(PointArray);
    }

    std::vector<std::shared_ptr<Triangle>> GetTriangleReferences()
    {
        return std::vector<std::shared_ptr<Triangle>>(TriangleArray);
    }

public://temporary -> use MatrixInstance as that has reference to centre point
	void Transform(Matrix4x4 Matrix)
	{
		for (int i = 0; i < PointArray.size(); i++)
		{
			PointArray[i]->Transform(Matrix);
		}

		for (int i = 0; i < TriangleArray.size(); i++)
		{
			if (TriangleArray[i]->HasVertexNormals())
			{
				TriangleArray[i]->SetVertexNormals(
					Matrix * TriangleArray[i]->GetVertexNormalA(),
					Matrix * TriangleArray[i]->GetVertexNormalA(),
					Matrix * TriangleArray[i]->GetVertexNormalA());
			}
		}
	}

	void Scale(float Scalar)
	{
		for (int i = 0; i < PointArray.size(); i++)
		{
			PointArray[i]->Scale(Scalar);
		}
	}

	void Translate(Vector3D Translation)
	{
		for (int i = 0; i < PointArray.size(); i++)
		{
			(PointArray[i]->Translate(Translation));
		}
	}

private:
	//helper functions

	int FindIndexOfVertex(std::shared_ptr<Point> &Vertex)
	{
		//done by reference to shared pointer to stop another strong reference
		for (int i = 0; i < PointArray.size(); i++)
		{
			if (PointArray[i] == Vertex) 
			{
				return i;
			}
		}
		return -1;
	}

    //All below is for use in triangulation

    std::vector<Vector2D> SquashToPlane(std::vector<int>& PointIndexes)
    {
        Vector3D AB = PointArray[PointIndexes[1]]->GetPosition() - PointArray[PointIndexes[0]]->GetPosition();
        Vector3D AC = PointArray[PointIndexes[2]]->GetPosition() - PointArray[PointIndexes[0]]->GetPosition();

        Vector3D PlaneNormal = Vector3D::Cross(AB, AC);

        Vector3D UnitVectorY = Vector3D::Normalise(Vector3D::Cross(PlaneNormal, AB));
        Vector3D UnitVectorX = Vector3D::Normalise(AB);

        std::vector<Vector2D> PointXY;

        for (int i = 0; i < PointIndexes.size(); i++)
        {
            Vector3D PointXYZ = PointArray[PointIndexes[i]]->GetPosition();
            PointXY.push_back(
                Vector2D(
                Vector3D::Dot(UnitVectorX, PointXYZ),
                Vector3D::Dot(UnitVectorY, PointXYZ)
            ));
        }

        return PointXY;
    }

    float GetAngleOf3Points(Vector2D A, Vector2D B, Vector2D C)
    {
        Vector2D AB = Vector2D::Normalise(B - A);
        Vector2D BC = Vector2D::Normalise(C - A);

        //if we were wrapping anticlockwise, we would use our LeftVector
        Vector2D RightVector = -Vector2D(AB.GetY(), -AB.GetX());
        float DotRight = Vector2D::Dot(RightVector, BC);

        if (Vector2D::Dot(AB, BC) > 0.0f)
        {
            //angle is > pi/2 or < 3 pi / 2
            return acosf(DotRight) + (0.5f * PI);
        }
        else
        {
            //angle is < pi/2 or > 3 pi / 2
            if (DotRight >= 0.0f)
            {
                return acosf(DotRight);
            }
            else
            {
                return acosf(DotRight) + (1.5f * PI);
            }
        }
        //This will output the angle in radians of the set [0.0f, 2 pi)
    }

    bool IsTriangleEmpty(std::vector<Vector2D>& PointXY, Vector2D A, Vector2D B, Vector2D C)
    {
        Vector2D NormalAB = B - A;
        Vector2D NormalBC = C - B;
        Vector2D NormalCA = A - C;

        std::cout << std::endl << "A: " << A.GetX() << " :: " << A.GetY();
        std::cout << std::endl << "B: " << B.GetX() << " :: " << B.GetY();
        std::cout << std::endl << "C: " << C.GetX() << " :: " << C.GetY();

        NormalAB = -Vector2D(NormalAB.GetY(), -NormalAB.GetX());
        NormalBC = -Vector2D(NormalBC.GetY(), -NormalBC.GetX());
        NormalCA = -Vector2D(NormalCA.GetY(), -NormalCA.GetX());

        

        for (int i = 0; i < PointXY.size(); i++)
        {
            if (PointXY[i] == A || PointXY[i] == B || PointXY[i] == C)
            {
                continue;
            }

            std::cout << std::endl << std::endl << PointXY[i].GetX() << " :: " << PointXY[i].GetY();
            std::cout << std::endl << "Dot: " << (Vector2D::Dot(NormalAB, PointXY[i] - A) >= 0.0f) << " " << (Vector2D::Dot(NormalBC, PointXY[i] - B) >= 0.0f) << " " << (Vector2D::Dot(NormalCA, PointXY[i] - C) >= 0.0f);
            
            if (
                Vector2D::Dot(NormalAB, PointXY[i] - A) >= 0.0f &&
                Vector2D::Dot(NormalBC, PointXY[i] - B) >= 0.0f &&
                Vector2D::Dot(NormalCA, PointXY[i] - C) >= 0.0f
                ) 
            {
                return false;
            }
        }

        return true;
    }

    void TriangulateFace(std::vector<int>& PointIndexes, std::vector<Vector3D>& NormalData, std::vector<int>& NormalIndexes, bool UseNormals)
    {
        //.obj face data should ALWAYS be simple polygons with all points being coplaner

        //Note:
        /*
        
       "Face elements use surface normals to indicate their orientation. If
        vertices are ordered counterclockwise around the face, both the
        face and the normal will point toward the viewer. If the vertex
        ordering is clockwise, both will point away from the viewer. If
        vertex normals are assigned, they should point in the general
        direction of the surface normal, otherwise unpredictable results
        may occur."

        -> This means that face ordering could be CCW or CW, leading to vastly innaccurate results
        
        */

        //Step One is to create an array of 2D points for the face
        //All points within a face are coplaner so this is possible

        std::vector<Vector2D> PointXY = SquashToPlane(PointIndexes);
        std::vector<Vector2D> AllPoints = std::vector<Vector2D>(PointXY);

        //Vector2D A = PointXY[1];
        //Vector2D B = PointXY[3];
        //Vector2D C = PointXY[6];
        //std::cout << std::endl << "TESTTEST " << IsTriangleEmpty(AllPoints, A, B, C);

        while (PointXY.size() > 3)
        {
            for (int i = 0; i < PointXY.size(); i++)
            {
                int Size = PointXY.size();
                int j = (i + 1) % (Size - 1);
                int k = (i + 2) % (Size - 1);

                Vector2D A = PointXY[i];
                Vector2D B = PointXY[j];
                Vector2D C = PointXY[k];

                float Angle = GetAngleOf3Points(A, B, C);
                if (Angle < PI)
                {
                    if (IsTriangleEmpty(AllPoints, A, B, C))
                    {
                        std::cout << std::endl << std::endl << "T: " << PointIndexes[i] << " " << PointIndexes[j] << " " << PointIndexes[k];

                        if (UseNormals) 
                        {
                            TriangleArray.push_back(std::shared_ptr<Triangle>(
                                new Triangle(
                                    PointArray[PointIndexes[i]],
                                    PointArray[PointIndexes[j]],
                                    PointArray[PointIndexes[k]],
                                    NormalData[NormalIndexes[i]],
                                    NormalData[NormalIndexes[j]],
                                    NormalData[NormalIndexes[k]])));
                        }
                        else
                        {

                            TriangleArray.push_back(std::shared_ptr<Triangle>(
                                new Triangle(
                                    PointArray[PointIndexes[i]],
                                    PointArray[PointIndexes[j]],
                                    PointArray[PointIndexes[k]])));
                        }

                        std::vector<Vector2D>::iterator iterator1 = PointXY.begin() + i;
                        PointXY.erase(iterator1);

                        std::vector<int>::iterator iterator2 = PointIndexes.begin() + i;
                        PointIndexes.erase(iterator2);

                        std::vector<int>::iterator iterator3 = NormalIndexes.begin() + i;
                        NormalIndexes.erase(iterator3);
                    }

                    std::cout << std::endl << "T: " << PointIndexes[0] << " " << PointIndexes[1] << " " << PointIndexes[2];
                }
            }
        }

        if (UseNormals)
        {
            TriangleArray.push_back(std::shared_ptr<Triangle>(
                new Triangle(
                    PointArray[PointIndexes[0]],
                    PointArray[PointIndexes[1]],
                    PointArray[PointIndexes[2]],
                    NormalData[NormalIndexes[0]],
                    NormalData[NormalIndexes[1]],
                    NormalData[NormalIndexes[2]])));
        }
        else
        {

            TriangleArray.push_back(std::shared_ptr<Triangle>(
                new Triangle(
                    PointArray[PointIndexes[0]],
                    PointArray[PointIndexes[1]],
                    PointArray[PointIndexes[2]])));
        }
    }

};