/*main.cpp*/

//
// <Amratya Saraswat>
// U. of Illinois, Chicago
// CS 341, Fall 2019
// Project #03: GradeUtil UI
// This program is C++11 dependent
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

// includes for gradeutil
  #include "gradeutil.h"

College InputGradeData(string filename)
{
    College college;
    ifstream file(filename);
    string line, value;

    if (!file.good())
    {
        cout << "**Error: unable to open input file '" << filename << "'." << endl;
        return college;
    }

    // first line contains semester,year
    getline(file, line);
    stringstream ss(line);

    getline(ss, college.Name, ',');
    getline(ss, college.Semester, ',');
    getline(ss, value);
    college.Year = stoi(value);

    // second line contains column headers --- skip
    getline(file, line);

    //
    // now start inputting and parse course data:
    //

    while (getline(file, line))
    {
        Course c = ParseCourse(line);

        //
        // search for correct dept to ask course to, otherwise create a new dept:
        //
        auto dept_iter = std::find_if(college.Depts.begin(),
                                      college.Depts.end(),
                                      [&](const Dept &d) {
                                          return (d.Name == c.Dept);
                                      });

        if (dept_iter == college.Depts.end())
        {
            //
            // doesn't exist, so we have to create a new dept
            // and insert course:
            //
            Dept d(c.Dept);

            d.Courses.push_back(c);

            college.Depts.push_back(d);
        }
        else
        {
            // dept exists, so insert course into existing dept:
            dept_iter->Courses.push_back(c);
        }

    } //while

    //
    // done:
    //
    return college;
}
bool checkDept(vector <Dept> department , string input) {
  int counter = 0;
  for (auto element : department){
    if (element.Name == input){
      counter =1;
    }
  }
  if (counter == 0){
    return false;
  }
  else {
    return true;
  }
}
string convertGradingType(const Course& c){
  if (c.getGradingType() == 0){
    return "letter";
  }
  else if(c.getGradingType() == 1){
    return "satisfactory";
  }
  else{
    return "unknown";
  }
}

// vector<Course> sortDNS(vector <Course> myCourseVec){
//   sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
//       if (c1.Dept < c2.Dept)
//           return true;
//         else if (c1.Dept > c2.Dept)
//           return false;
//         else // equal, determine by section number:
//           if (c1.Number < c2.Number)
//             return true;
//           else if (c1.Number > c2.Number)
//             return false;
//           else 
//             if (c1.Section < c2.Section)
//               return true;
//             else
//               return false;
//      });
//   return myCourseVec;
// }

// TODO: define your own functions
void printSummary(const College &a){
  int x,y;
  cout << "# of courses taught: " << a.NumCourses() << endl;
  cout << "# of students taught: " << a.NumStudents() << endl;
  cout << "grade distribution (A-F): " << GetGradeDistribution(a).PercentA << "%, " << 
           GetGradeDistribution(a).PercentB << "%, "<<
           GetGradeDistribution(a).PercentC << "%, "<<
           GetGradeDistribution(a).PercentD << "%, "<<
           GetGradeDistribution(a).PercentF << "% "<<endl;
  cout << "DFW rate: " << GetDFWRate(a,x,y)<< "%"<<endl << endl;

}
void printSummary(const Dept &department){
  int x,y;
  cout << department.Name << ": " << endl;
  cout << " # courses taught: " << department.NumCourses() << endl;
  cout << " # students taught: " << department.NumStudents() << endl; 
  cout << " grade distribution (A-F): " << GetGradeDistribution(department).PercentA << "%, "<<
          GetGradeDistribution(department).PercentB << "%, "<<
          GetGradeDistribution(department).PercentC << "%, "<<
          GetGradeDistribution(department).PercentD << "%, "<<
          GetGradeDistribution(department).PercentF << "% "<<endl;
  cout << " DFW rate: " << GetDFWRate(department,x,y)<< "%" << endl;
}

void displaySummary(const College &college, string input){
  vector<Dept> depts = college.Depts;  
  if (input == "all"){
     sort(depts.begin(), depts.end(), [](const Dept& d1, const Dept& d2){
          return d1.Name < d2.Name;
     });
       
       for (const Dept& department : depts) {
            printSummary(department);
       }
   }
   else{
     int counter =0; 
    for (const Dept& department : depts) {
      if (department.Name == input){
          printSummary(department);
          counter ++;
      }
    }
     if (counter == 0){
       cout << "**dept not found " << endl;
     }
  }
}
void printSearch(const Course &c){
  int x,y;
  cout << c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
  cout << " # students: " << c.getNumStudents() << endl;
  cout << " course type: " << convertGradingType(c) << endl;
  cout << " grade distribution (A-F): " << GetGradeDistribution(c).PercentA << "%, "<<
          GetGradeDistribution(c).PercentB << "%, "<<
          GetGradeDistribution(c).PercentC << "%, "<<
          GetGradeDistribution(c).PercentD << "%, "<<
          GetGradeDistribution(c).PercentF << "% "<<endl;
  cout << " DFW rate: " << GetDFWRate(c, x, y) << "%" << endl;
}
void displaySearch(const College &college, string input){
  if (input == "all"){
    vector<Dept> depts = college.Depts; 
     string instructorPrefix;
     int courseNum;
     cout << "course # or instructor prefix? ";
     cin >> instructorPrefix;
     stringstream ss(instructorPrefix); // create stringstream object
     ss >> courseNum; // try to convert input to a course #:
     if ( ss.fail() ){
       vector<Course> instructorVec = FindCourses(college, instructorPrefix);
         if (instructorVec.size()== 0){
           cout << "**none found" << endl;
           return;
         }  
       for (auto &element : instructorVec){
       printSearch(element);
       }
     }
     else{
       // conversion worked, courseNum contains numeric value
       vector<Course> instructorVec = FindCourses(college, courseNum);
         if (instructorVec.size()== 0){
           cout << "**none found" << endl;
           return;
         }  
       for (auto &element : instructorVec){
         printSearch(element);
       }
     }
  }
  else{
    vector<Dept> depts = college.Depts;
   
    string instructorPrefix;
     
     int courseNum;
     cout << "course # or instructor prefix? ";
     cin >> instructorPrefix;
     if (checkDept(depts, input) == false){
        cout << "**dept not found" << endl;
          return;
    }
     stringstream ss(instructorPrefix); // create stringstream object
     ss >> courseNum; // try to convert input to a course #:
     if ( ss.fail()){
           vector<Course> instructorVec = FindCourses(college, instructorPrefix);
         if (instructorVec.size()== 0){
           cout << "**none found" << endl;
           return;
         }  
         for (auto &element : instructorVec){
             if (element.Dept == input) {
               printSearch(element);
             }
           }
      }
     else{
       vector<Course> instructorVec = FindCourses(college, courseNum);
       if (instructorVec.size()== 0){
           cout << "**none found" << endl;
           return;
       }
       int counterCourseNum = 0;
       for (auto elem : instructorVec){
          if (elem.Dept == input) {
            counterCourseNum = 1;
          }
       }
       if (counterCourseNum == 0) {
         cout << "**none found" << endl;
       }
       for (auto &element : instructorVec){
           if (element.Dept == input) {
               printSearch(element);
             }
       }
     }
  }
}
void printSatisfactory(const Course &c){
  //if (c.getGradingType() == Course::Satisfactory){
          cout << c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
          cout << " # students: " << c.getNumStudents() << endl;
          cout << " course type: " << convertGradingType(c) << endl;
  //}
  
}
void displaySatisfactory(const College &college, string input){
  if (input == "all") {
    vector <Course> myCourseVec;
    for (Dept department : college.Depts){
      for (Course c : department.Courses){
        myCourseVec.push_back(c);
      }
    }
    sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
      if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // equal, determine by section number:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else 
            if (c1.Section < c2.Section)
              return true;
            else
              return false;
     });
        for (auto element : myCourseVec){
          if (element.getGradingType() == Course::Satisfactory){
             printSatisfactory(element);
           } 
        }
   } 
   else {
//     for (const Dept& department : college.Depts) {
//       if (department.Name == input){
//          for (const Course& c : department.Courses){
//               printSatisfactory(c);
//           }
//         }
//       }
      vector <Course> myCourseVec;
      for (Dept department : college.Depts){
        for (Course c : department.Courses){
          if (c.Dept == input){
            myCourseVec.push_back(c);
          }
        }
      }
    sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
      if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // equal, determine by section number:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else 
            if (c1.Section < c2.Section)
              return true;
            else
              return false;
     });
     int checkSatisfactory = 0;
     for (auto element : myCourseVec){
       if (element.getGradingType() == Course::Satisfactory){
         printSatisfactory(element);
         checkSatisfactory = 1;
       }     
     }
     if (checkSatisfactory == 0) {
       cout << "**none found" << endl;
     }
  } 
}
void printDFW(const Course &c){
  int x,y;
  cout << c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
  cout << " # students: " << c.getNumStudents() << endl;
  cout << " course type: " << convertGradingType(c) << endl;
  cout << " grade distribution (A-F): " << GetGradeDistribution(c).PercentA << "%, "<<
          GetGradeDistribution(c).PercentB << "%, "<<
          GetGradeDistribution(c).PercentC << "%, "<<
          GetGradeDistribution(c).PercentD << "%, "<<
          GetGradeDistribution(c).PercentF << "% "<<endl;
  cout << " DFW rate: " << GetDFWRate(c, x, y) << "%"<<  endl;
}

void displayDFW(const College college, string input){
  if (input == "all"){
    int a,b;
    
    double dfwThreshold;
    cout << "dfw threshold? ";
    cin >> dfwThreshold;
//     for (const Dept& department : college.Depts){
//       for(const Course& c : department.Courses){
//         if (GetDFWRate(c, a, b) > dfwThreshold){
//           printDFW(c);
//         } 
//       }
//     }
    vector <Course> myCourseVec;
    for (Dept department : college.Depts){
      for (Course c : department.Courses){
        myCourseVec.push_back(c);
      }
    }
    sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
      int a,b;
      if (GetDFWRate(c1, a, b) > GetDFWRate(c2, a, b)){
        return true;
      }
      else if (GetDFWRate(c1, a, b) < GetDFWRate(c2, a, b)){
        return false;
      }
      else{
        if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // equal, determine by section number:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else 
            if (c1.Section < c2.Section)
              return true;
            else
              return false;
      }
      
     });
    int checkdfwCounter = 0;
    for (auto element : myCourseVec) {
      if (GetDFWRate(element, a, b) > dfwThreshold){
          printDFW(element);
          checkdfwCounter = 2;
      }
    }
    if (checkdfwCounter == 0) {
      cout << "**none found" << endl;
    }
  }
  else{
    double dfwThreshold;
    int a,b;
    cout << "dfw threshold? ";
    cin >> dfwThreshold;
//     for (const Dept& department : college.Depts){
//         if (department.Name == input){
//             for(const Course& c : department.Courses){
//                 if (GetDFWRate(c, a, b) > dfwThreshold){
//                 printDFW(c);
//                 }
//             }
//          }
//     }
    vector<Dept> depts = college.Depts;
    if (checkDept(depts ,input) == false){
      cout << "**dept not found" << endl;
      return;
    }
    vector <Course> myCourseVec;
    for (Dept department : college.Depts){
      for (Course c : department.Courses){
        if (c.Dept == input) {
          myCourseVec.push_back(c);
        } 
      }
    }
    sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
      int a,b;
      if (GetDFWRate(c1, a, b) > GetDFWRate(c2, a, b)){
        return true;
      }
      else if (GetDFWRate(c1, a, b) < GetDFWRate(c2, a, b)){
        return false;
      }
      else {
        if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // equal, determine by section number:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else 
            if (c1.Section < c2.Section)
              return true;
            else
              return false;
      }
      
     });
    int checkdfw =0;
    for (auto element : myCourseVec) {
      if (GetDFWRate(element, a, b) > dfwThreshold){
          printDFW(element);
          checkdfw = 1;
      }
    }
    if (checkdfw == 0){
      cout << "**none found" << endl;
    }
  }
}

void printLetterB(const Course &c){
  int x,y;
  cout << c.Dept << " " << c.Number << " " << "(section " << c.Section << "): " << c.Instructor << endl;
  cout << " # students: " << c.getNumStudents() << endl;
  cout << " course type: " << convertGradingType(c) << endl;
  cout << " grade distribution (A-F): " << GetGradeDistribution(c).PercentA << "%, "<<
          GetGradeDistribution(c).PercentB << "%, "<<
          GetGradeDistribution(c).PercentC << "%, "<<
          GetGradeDistribution(c).PercentD << "%, "<<
          GetGradeDistribution(c).PercentF << "% "<<endl;
  cout << " DFW rate: " << GetDFWRate(c, x, y) << "%" << endl;
}
void DisplayLetterB(const College college, string input){
   if (input == "all"){
    //int a,b;
    double letterBThreshold;
    cout << "letter B threshold? ";
    cin >> letterBThreshold;
    
    vector <Course> myCourseVec;
    for (Dept department : college.Depts){
      for (Course c : department.Courses){
        myCourseVec.push_back(c);
      }
    }
    sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
      if (GetGradeDistribution(c1).PercentB > GetGradeDistribution(c2).PercentB){
        return true;
      }
      else if (GetGradeDistribution(c1).PercentB < GetGradeDistribution(c2).PercentB){
        return false;
      }
      else{
        if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // equal, determine by section number:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else 
            if (c1.Section < c2.Section)
              return true;
            else
              return false;
      }
      
     });
      int checkLetterB = 0;
      for (auto elem : myCourseVec){
          if (GetGradeDistribution(elem).PercentB > letterBThreshold){
            printLetterB(elem);
            checkLetterB = 1;
          }
        }
     if (checkLetterB == 0){
       cout << "**none found" << endl;
     }
  }
  else{
    double letterBThreshold;
    cout << "letter B threshold? ";
    cin >> letterBThreshold;
    vector<Dept> depts = college.Depts;
    if (checkDept(depts, input) == false){
        cout << "**dept not found" << endl;
          return;
    }
    
     vector <Course> myCourseVec;
    for (Dept department : college.Depts){
      for (Course c : department.Courses){
        if (c.Dept == input) {
          myCourseVec.push_back(c);
        } 
      }
    }
    sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
      if (GetGradeDistribution(c1).PercentB > GetGradeDistribution(c2).PercentB){
        return true;
      }
      else if (GetGradeDistribution(c1).PercentB < GetGradeDistribution(c2).PercentB){
        return false;
      }
      else {
        if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // equal, determine by section number:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else 
            if (c1.Section < c2.Section)
              return true;
            else
              return false;
      }
      
     });
        int check = 0;
        for (auto elem : myCourseVec){
          if (GetGradeDistribution(elem).PercentB > letterBThreshold){
            printLetterB(elem);
            check = 2;
          }
        }
        if (check == 0){
          cout << "**none found" << endl;
        }
  }
}
double gpaCalculatorAll(const Dept& d) {
  double A,B,C,D,F,numerator,denominator,GPA = 0;
  double counter =0;
  for (Course elem : d.Courses) {
    if (elem.getGradingType() == Course::Letter){
       A = GetGradeDistribution(elem).NumA;
       B = GetGradeDistribution(elem).NumB;
       C = GetGradeDistribution(elem).NumC;
       D = GetGradeDistribution(elem).NumD;
       F = GetGradeDistribution(elem).NumF;
       numerator = A*4.0 + B*3.0 + C*2.0 + D*1.0;
       denominator = A + B + C + D +F;
       GPA += double(numerator/denominator);
      counter++;
    }
 
  }
  double avg = (GPA/counter);
    return avg;
  
}
void sortallAvg(vector <Dept> &dept){
   sort(dept.begin(), dept.end(),[](const Dept& c1, const Dept& c2){
      if ( gpaCalculatorAll(c1)> gpaCalculatorAll(c2)){
        return true;
      }
      else if (gpaCalculatorAll(c1)< gpaCalculatorAll(c2)){
        return false;
      }      
     });
}
double gpaCalculator(const Course& c){
  double numerator = ((c.NumA)*4) + ((c.NumB)*3) + ((c.NumC)*2) +((c.NumD)*1) + ((c.NumF)*0);
  double denominator = c.NumA + c.NumB + c.NumC + c.NumD + c.NumF;
  return (numerator/denominator);
}

void printAverageDept(const Dept& d){
  cout << "Overall GPA for " << d.Name << " : " << gpaCalculatorAll(d)<< endl;
}
void printAverageAll(const Course &c){
  cout << "Overall GPA for " << c.Dept << " " << c.Number << "(" <<c.Section << ")" << " : " << gpaCalculator(c)<< endl;
}
void displayAverage(const College college, string input){
  if (input == "all"){
    vector <Course> myCourseVec;
    for (Dept department : college.Depts){
      for (Course c : department.Courses){
        if (c.getGradingType() == Course::Letter){
          myCourseVec.push_back(c);
        }
      }
    }
    vector<Dept> depts = college.Depts;
    sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
      //int a,b;
      if (gpaCalculator(c1) > gpaCalculator(c2)){
        return true;
      }
      else if (gpaCalculator(c1) < gpaCalculator(c2)){
        return false;
      }
      else{
        if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // equal, determine by section number:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else 
            if (c1.Section < c2.Section)
              return true;
            else
              return false;
      }
     });
    vector <Dept> newDept;
    for (Dept d : college.Depts){
      GradeStats stats = GetGradeDistribution(d);
      if (stats.N != 0) {
         newDept.push_back(d);
      }
    }
    
     sortallAvg(newDept);
     for(auto d : newDept) {
       printAverageDept(d);
     }
    
//     for (auto element : myCourseVec){
//       printAverageDept(elem);
//     }
    
  }
  else{
    vector <Course> myCourseVec;
    for (Dept department : college.Depts){
      for (Course c : department.Courses){
        if (c.Dept == input) {
          if (c.getGradingType() == Course::Letter) {
            myCourseVec.push_back(c);
          }
        } 
      }
    }
    sort(myCourseVec.begin(), myCourseVec.end(),[](const Course& c1, const Course& c2){
      //int a,b;
      if (gpaCalculator(c1) > gpaCalculator(c2)){
        return true;
      }
      else if (gpaCalculator(c1) < gpaCalculator(c2)){
        return false;
      }
      else {
        if (c1.Dept < c2.Dept)
          return true;
        else if (c1.Dept > c2.Dept)
          return false;
        else // equal, determine by section number:
          if (c1.Number < c2.Number)
            return true;
          else if (c1.Number > c2.Number)
            return false;
          else 
            if (c1.Section < c2.Section)
              return true;
            else
              return false;
      }      
     });
    for (auto element : myCourseVec) {
        printAverageAll(element);
      }
  }
}
  
int main()
{
    string filename;

    cout << std::fixed;
    cout << std::setprecision(2);

    //
    // 1. Input the filename and then the grade data:
    //
    cin >> filename;
    //filename = "fall-2018.csv";

    College college = InputGradeData(filename);

    // 2. TODO: print out summary of the college
    // DEFINE your own functions
      //cout << filename << endl;
      cout << "** " << "College of "<<college.Name << ", " << college.Semester<<" " << college.Year << " **" << endl;
      printSummary(college);
      cout << "Enter a command> ";
    //
    // 3. TODO: Start executing commands from the user:
    // DEFINE your own functions
    string userInput;
    cin >> userInput;

    while(userInput != "#"){
      
      if(userInput == "summary"){
        cout << "dept name, or all? ";
        string input1;
        cin >> input1;
        displaySummary(college, input1);
      } 
      else if (userInput == "search"){
        cout << "dept name, or all? ";
        string input2;
        cin >> input2;
        displaySearch(college, input2);
        
      }
      else if (userInput == "satisfactory"){
        cout << "dept name, or all? ";
        string input3;
        cin >> input3;
        displaySatisfactory(college, input3);
        
      }

      else if (userInput == "dfw"){
        cout << "dept name, or all? ";
        string input4;
        cin >> input4;
        displayDFW(college, input4);
        
      }
      else if (userInput == "letterB"){
        cout << "dept name, or all? ";
        string input5;
        cin >> input5;
        DisplayLetterB(college, input5);
      }
      else if (userInput == "average"){
        cout << "dept name, or all? ";
        string input6;
        cin >> input6;
        displayAverage(college, input6);
      }
      else{
      cout << "**unknown command" << endl;
      }
      cout << "Enter a command> ";
      cin >> userInput;
    }
    

    //
    // done:
    //
    return 0;
}