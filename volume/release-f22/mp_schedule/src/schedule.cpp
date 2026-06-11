/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings
 * where each row in the text file is a row in the V2D and each comma-separated
 * value is stripped of whitespace and stored as its own string.
 *
 * Your V2D should match the exact structure of the input file -- so the first
 * row, first column in the original file should be the first row, first column
 * of the V2D.
 *
 * @param filename The filename of a CSV-formatted text file.
 */
using namespace std;
V2D file_to_V2D(const string &filename) {
  // Your code here!
  V2D v2d{};
  vector<string> v{};
  string line;
  string str = file_to_string(filename);
  stringstream ss(str);

  while (getline(ss, line)) {
    // printf("line: %s\n", line.c_str());
    SplitString(line, ',', v);
    v2d.push_back(v);
		v.clear();
  }
  return v2d;
}

/**
 * Given a course roster and a list of students and their courses,
 * perform data correction and return a course roster of valid students (and
 * only non-empty courses).
 *
 * A 'valid student' is a student who is both in the course roster and the
 * student's own listing contains the course A course which has no students (or
 * all students have been removed for not being valid) should be removed
 *
 * @param cv A 2D vector of strings where each row is a course ID followed by
 * the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed
 * by the courses they are taking
 */
V2D clean(const V2D &cv, const V2D &student) {
  // YOUR CODE HERE

  V2D v2d{};
  vector<string> interim{};

  // e1ec30: for each course...
  for (size_t i = 0; i < cv.size(); i++) {
    vector<string> row = cv[i];
    // printf("course: %s\n", row[0].c_str());

    // e1ec30: for each student...
    for (size_t j = 1; j < row.size(); j++) {
      string student_name = row[j];
      // printf("student name: %s\n", student_name.c_str());

      // e1ec30: check if they are in the student list
      for (size_t k = 0; k < student.size(); k++) {
        // printf("comparing: (%s, %s)\n", student[k][0].c_str(),
        // student_name.c_str());
        if (Trim(student[k][0]) == Trim(student_name)) {
          // e1ec30: check if they are offering the course.
          for (size_t l = 1; l < student[k].size(); l++) {
            // printf("comparing courses: (%s, %s)\n", row[0].c_str(),
            // student[k][l].c_str());
            if (Trim(student[k][l]) == Trim(row[0])) {
              if (interim.empty())
                interim.push_back(Trim(row[0]));
              interim.push_back(Trim(student_name));
            }
          }
          break;
        }
      }
    }
    if (!interim.empty())
      v2d.push_back(interim);
    interim.clear();
  }

  return v2d;
}

/**
 * Given a collection of courses and a list of available times, create a valid
 * scheduling (if possible).
 *
 * A 'valid schedule' should assign each course to a timeslot in such a way that
 * there are no conflicts for exams In other words, two courses who share a
 * student should not share an exam time. Your solution should try to minimize
 * the total number of timeslots but should not exceed the timeslots given.
 *
 * The output V2D should have one row for each timeslot, even if that timeslot
 * is not used.
 *
 * As the problem is NP-complete, your first scheduling might not result in a
 * valid match. Your solution should continue to attempt different schedulings
 * until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with
 * the string '-1' as the only value.
 *
 * @param courses A 2D vector of strings where each row is a course ID followed
 * by the students in the course
 * @param timeslots A vector of strings giving the total number of unique
 * timeslots
 */
V2D schedule(const V2D &courses, const vector<string> &timeslots) {
  // Your code here!
  unordered_set<string> students;
  return {};
}
