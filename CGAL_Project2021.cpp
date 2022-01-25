#include <CGAL/basic.h>
#include <CGAL/IO/Arr_iostream.h>
#include "arr_exact_construction_segments.h"
#include "arr_print.h"
#include <boost/timer.hpp>
#include <CGAL/Timer.h>
#include "point_location_utils.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>

#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>

#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::vector<Point_2> Points;

typedef CGAL::Arr_naive_point_location<Arrangement>		Naive_pl;
typedef CGAL::Arr_landmarks_point_location<Arrangement> Landmarks_pl;
typedef CGAL::Arr_walk_along_line_point_location<Arrangement>  Walk_pl;
typedef CGAL::Arr_trapezoid_ric_point_location<Arrangement>  Trap_pl;


using namespace std;

int main()
{

	int x, y, x1, y1;                     //coordinates of lines
	int number_of_seg, number_of_points;  //input numbers from the user for lines and points
	int point1, point2;                   //coordinates for the points to be founded by the algorithms
	int select, select2;                  //variable to choose which algorithms execute each time (select for exercise 3) (select2 for exercise 6)
	int readData;                         //varibale to control if the arrangement will be randomly created or if it will be read from a file (exercise 1)
	int newPoint1, newPoint2;             //use the variables 'newPoint1' & 'newPoint2' to get from the user the new coordinates of the new point to search for (exercise 6)

	double secs;                          //variable to count the time where is need
	
	string answerRepeat;				  //check if the user wants to repeat the process of searching with algorithms (exercise 3)
	string answerSave;					  //check if the user wants to save the file (exercise 5)
	string answerNewPointLocation;        //check if the user wants to execute the new point location to the already saved arrangement A (=>arrA)

	bool saved = false;				      //variable 'saved' type bool, to check if the user saved the file, so the exercise 6 can work properly. If it is false, that means the user didn't save the file. If true, it has been saved

	Arrangement arrA;  //create an empty arrangement
	boost::timer timer;  //create the 'timer' to count how much time the questions need to complete
	
	//1 ***Input of line segments
	cout << "If you want to create random line segments press --> (1)\nIf you want to load data from a file press --------> (2)" << endl;
	cin >> readData;

	//1a random create points on line segments
	if (readData == 1)
	{
		srand(time(0));      //initalize the random function with 0, so it's NOT generate the same values over and over again

		cout << "Please give the number of the 'n' (line segments): " << endl;
		cin >> number_of_seg;
		try
		{
			timer.restart();

			for (int i = 0; i < number_of_seg; i++)
			{
				x = rand() % 1000 + 1;
				y = rand() % 1000 + 1;
				Point p1 = Point(x, y);

				x1 = rand() % 100 + 1;
				y1 = rand() % 100 + 1;
				Point p2 = Point(x1, y1);

				Segment ssA = Segment(p1, p2);

				insert(arrA, ssA);  // insert each random generated segment (line) into the empty arrangement
			}
		}
		catch (CGAL::Failure_exception & e) {
			cerr << e.what() << endl;
		}
		secs = timer.elapsed();
		cout << "For the random creation of " << number_of_seg << " line segments, it took " << secs << " seconds." << endl;
	}
	//1b read data from a file
	else if (readData == 2)
	{
		timer.restart();
		ifstream in_file("read_Data_File.txt");
		in_file >> arrA;
		in_file.close();
		print_arrangement_size(arrA);
		secs = timer.elapsed();
		cout << "To read the file it took " << secs << " seconds" << endl;
	}
	cout << endl;
	//1 ***
	
	vector <Point> vectorPointLocation; // create a vector to hold all the points, and use the vector to locate the points with the 4 algorithms
	Point searchPoint;  //create a variable 'searchPoint' type 'Point' because the function 'locate_point' can't take argument of type vector
	
	//2 ***Convex Hull of A
	Point_2 *pointsOnConvex, *result, *ptr;
	cout << "Please give the number of the points you want to put into the arrangement 'arrA'" << endl;
	cin >> number_of_points;
	
	pointsOnConvex = new Point_2[number_of_points]; // dynamically allocate the exact size of memory and create an array 'pointOnConvex' with the input size of user
	result = new Point_2[number_of_points];         // dynamically allocate the exact size of memory and create an array 'result' with the input size of user

	for (int i = 0; i < number_of_points; i++)
	{
		point1 = rand() % 100 + 1;
		point2 = rand() % 10 + 1;

		vectorPointLocation.push_back(Point(point1, point2)); //store all the point in the vector 'vectorPointLocation'
		*(pointsOnConvex+i) = Point_2(point1, point2);
	}
	timer.restart();
	ptr = CGAL::convex_hull_2(pointsOnConvex, pointsOnConvex + number_of_points, result);
	secs = timer.elapsed();
	cout << "The number of the points on convex hull is -" << ptr - result << "- and found in: " << secs << "\nThose are:" << endl;
	for (int i = 0; i < ptr - result; i++)
		cout << *(result+i) << endl;
	
	delete[] result;         //after showing the result we don't need the array, so delete it TO AVOID MEMORY LEAKAGE
	delete[] pointsOnConvex; //after finding the points on the convex, we don't need the array, so delete it TO AVOID MEMORY LEAKAGE 
	//2 ***

	//5 *** Store arrA in file: DONE
	cin.ignore(256, '\n');
	cout << endl << "Would you like to save the arrangement 'arrA' into a file ??\nWrite 'Yes'/'yes'/'y' if you want to save it. Oherwise press any key to continue" << endl;
	cin >> answerSave;
	if (answerSave == "Yes" || answerSave == "yes" || answerSave == "y")
	{
		cout << "Writing" << endl;
		print_arrangement_size(arrA);
		ofstream out_file("save_arrA.txt");
		out_file << arrA;
		out_file.close();
		cout << "The file saved with the name 'save_arrA'";
		saved = true;
		cout << endl;
	}
	else
		cout << "The arrangement did NOT save in file." << endl;
	cout << endl;
	//5***

	//3 *** Point_Location of each point with the 4 algorithms
	Naive_pl naive_pl(arrA);

	Walk_pl walk_pl(arrA);

	Landmarks_pl landmarks_pl;
	landmarks_pl.attach(arrA);

	Trap_pl trap_pl;
	trap_pl.attach(arrA);

	while (true)
	{
		cout << "Now choose with which algorithm you want to search for the points:" << endl;
		cout << "Naive Point Location -------> (1)\nWalk Along Point Location --> (2)\nLandmarks Point Location ---> (3)\nTrapezoid Point Location ---> (4)" << endl;
		cin >> select;
		
		timer.restart();

		switch (select)
		{
		case 1:
			cout << "\t\t\tNaive Point Location Selected" << endl << endl;
			for(int i=0; i<number_of_points; i++)
			{
				searchPoint = vectorPointLocation.at(i);
				cout << "\t"; 
				locate_point(naive_pl, searchPoint);
			}
			secs = timer.elapsed();
			cout << "\nThe time for the Naive Point Location algorithm to find " << number_of_points << " points was " << secs << " seconds" << endl;
			break;

		case 2:
			cout << "\t\t\tWalk Along Point Location" << endl << endl;
			for (int i = 0; i < number_of_points; i++)
			{
				searchPoint = vectorPointLocation.at(i);
				cout << "\t";
				shoot_vertical_ray(walk_pl, searchPoint);
			}
			secs = timer.elapsed();
			cout << "\nThe time for the Walk Along algorithm to find " << number_of_points << " points was " << secs << " seconds" << endl;
			break;

		case 3:
			cout << "\t\t\tLandmarks Point Location" << endl << endl;
			for (int i = 0; i < number_of_points; i++)
			{
				searchPoint = vectorPointLocation.at(i);
				cout << "\t";
				locate_point(landmarks_pl, searchPoint);
			}
			secs = timer.elapsed();
			cout << "\nThe time for the Landmark Point Location algorithm to find " << number_of_points << " points was " << secs << " seconds" << endl;
			break;

		case 4:
			cout << "\t\t\tTrapezoid Point Location" << endl << endl;
			for (int i = 0; i < number_of_points; i++)
			{
				searchPoint = vectorPointLocation.at(i);
				cout << "\t";
				shoot_vertical_ray(trap_pl, searchPoint);
			}
			secs = timer.elapsed();
			cout << "\nThe time for the Trapezoid Point Location algorithm to find " << number_of_points << " points was " << secs << " seconds" << endl;
			break;

		default:
			cout << "Not proper choice" << endl;
			break;

		}

		cout << "\n\nDo you want to continue with another algorithm?\nYes or No" << endl;
		cin >> answerRepeat;

		if (answerRepeat == "Yes" || answerRepeat == "yes")
		{
			cout << endl << endl;
			continue;
		}
		else if (answerRepeat == "No" || answerRepeat == "no")
			break;
	}
	// 3 ***

	// 6*** New search for point in the save arrangement A (=>arrA)
	cout << "\n\nWould you like to search a new point in the arrangement 'arrA'.\nWrite 'Yes'/'yes'/'y' or press any key to continue." << endl;
	cin >> answerNewPointLocation;
	if (answerNewPointLocation == "Yes" || answerNewPointLocation == "yes" || answerNewPointLocation == "y")
	{
		if (saved == true)
		{
			Point newPointLocation2;
			ifstream in_file("save_arrA.txt");
			in_file >> arrA;
			in_file.close();
			print_arrangement_size(arrA);
			cout << "Please give the coordinates of the new point.\nGive the value for the 'x' axis" << endl;
			cin >> newPoint1;
			cout << "Give the value for the 'y' axis" << endl;
			cin >> newPoint2;

			newPointLocation2 = Point(newPoint1, newPoint2);
			cout << "\nChoose with which algorithm you want to search for the new point" << endl;
			cout << "Naive Point Location -------> (1)\nWalk Along Point Location --> (2)\nLandmarks Point Location ---> (3)\nTrapezoid Point Location ---> (4)" << endl;
			cin >> select2;

			switch (select2)
			{
			case 1:
				locate_point(naive_pl, newPointLocation2);
				break;
			case 2:
				shoot_vertical_ray(walk_pl, newPointLocation2);
				break;
			case 3:
				locate_point(landmarks_pl, newPointLocation2);
				break;
			case 4:
				shoot_vertical_ray(trap_pl, newPointLocation2);
				break;

			default:
				cout << "Not proper choice" << endl;
				break;
			}
			
		}
		else
			cout << "You didn't save the file, so you can't search in it. First save the file!!!" << endl;
	}
	else
		cout << "Not searching a new point in the arrangement 'arrA'" << endl;


	return 0;
}