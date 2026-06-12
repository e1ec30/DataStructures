/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <cmath>
#include <cstdlib>
#include <ctime>
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
    std::for_each(v.begin(), v.end(), [](std::string& s){
        s = Trim(s);
        });
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
        if (student[k][0] == student_name) {
          // e1ec30: check if they are offering the course.
          for (size_t l = 1; l < student[k].size(); l++) {
            // printf("comparing courses: (%s, %s)\n", row[0].c_str(),
            // student[k][l].c_str());
            if (student[k][l] == row[0]) {
              if (interim.empty())
                interim.push_back(row[0]);
              interim.push_back(student_name);
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
  //

  ScheduleGraph sg(courses, timeslots);
  std::vector<bool> put; // to keep track of unused allocations
  put.resize(timeslots.size(), false);
  std::vector<int> sched = sg.schedule();
  std::map<std::string, std::vector<std::string>> res;
  std::vector<std::string> interim;
  V2D result;

  if (sched.size() != 0) {
    for (size_t i = 0; i < sched.size(); i++) {
      res[timeslots[sched[i]]].push_back(courses[i][0]);
      put[sched[i]] = true;
    }
    for (size_t i = 0; i < timeslots.size(); i++) {
      if (!put[i]) res[timeslots[i]].push_back("UNUSED ALLOCATION"); // this seems unnecessary but I want all the tests to pass
    }
    for (auto [key, val] : res) {
      interim.push_back(key);
      interim.insert(interim.end(), val.begin(), val.end());
      result.push_back(interim);
      interim.clear();
    }
    return result;
  } else
    return {{"-1"}};
}

void ScheduleGraph::printMat() {
  puts("{ ");
  for (size_t i = 0; i < adjMat_.size(); i++) {
    for (size_t j = 0; j < adjMat_.size(); j++) {
      if (adjMat_[i][j])
        printf("TRUE, ");
      else
        printf("FALSE, ");
      if (j + 1 == adjMat_.size())
        printf("\n");
    }
  }
  puts(" }");
}

std::vector<int> ScheduleGraph::schedule() {
  // try every starting vertex
  for (size_t i = 0; i < adjMat_.size(); i++) {
    colors_[i] = 0; // give it the first color and try assigning to the rest
    if (assignColors(0))
      return colors_;
  }
  return {};
}

bool ScheduleGraph::assignColors(int v) {

  // puts("{ ");
  // std::for_each(colors_.begin(), colors_.end(), [](int elem){
  //     std::cout << elem << ", ";
  //     });
  // puts(" }");

  if ((size_t)v == adjMat_.size())
    return true;
  for (size_t c = 0; c < totalColors_; c++) {
    if (canColor(c, v)) {
      colors_[v] = c;

      if (assignColors(v + 1))
        return true;

      colors_[v] = -1; // backtrack
    }
  }
  return false;
}

bool ScheduleGraph::canColor(int c, int v) {
  for (size_t i = 0; i < adjMat_[v].size(); i++) {
    if (adjMat_[v][i] && colors_[i] == c)
      return false;
  }
  return true;
}

ScheduleGraph::ScheduleGraph(const V2D &courses,
                             const std::vector<std::string> &timeslots) {
  colors_.resize(courses.size(), -1);
  adjMat_.resize(courses.size(), {});
  totalColors_ = timeslots.size();
  for (auto &x : adjMat_)
    x.resize(courses.size(), false);

  // e1ec30: for each course
  for (size_t i = 0; i < courses.size(); i++) {
    const std::string &course_name = courses[i][0];

    // e1ec30: for each of its students
    for (size_t j = 1; j < courses[i].size(); j++) {
      const std::string &student_name = courses[i][j];

      // e1ec30: check other courses for that student
      for (size_t k = i + 1; k < courses.size(); k++) {
        for (size_t l = 1; l < courses[k].size(); l++) {
          if (courses[k][l] == student_name) {
            // printf("connecting: %s -> %s; they share student: %s\n",
                   // course_name.c_str(), courses[k][0].c_str(),
                   // student_name.c_str());
            adjMat_[i][k] = true;
            adjMat_[k][i] = true;
          }
        }
      }
    }
  }
}
