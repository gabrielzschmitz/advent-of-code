/**
 * @file part1.cpp
 * @brief Pipe Maze - Finding the Farthest Point in the Loop
 *
 * Explore the alien landscape of the floating metal island with a maze of
 * pipes. The pipes are arranged in a two-dimensional grid, with various shapes
 * like |, -, L, J, 7, F, and S. The goal is to find the single continuous loop
 * that contains the animal (starting at S) and determine the number of steps
 * along the loop to reach the point farthest from the starting position.
 *
 * Pipe Types:
 * | - Vertical pipe connecting north and south.
 * - - Horizontal pipe connecting east and west.
 * L - 90-degree bend connecting north and east.
 * J - 90-degree bend connecting north and west.
 * 7 - 90-degree bend connecting south and west.
 * F - 90-degree bend connecting south and east.
 * . - Ground; no pipe in this tile.
 * S - Starting position of the animal; there is a pipe on this tile.
 *
 * The loop is formed by pipes connected to S and the pipes they connect to,
 * forming a continuous loop. Calculate the distance of each tile in the loop
 * from the starting position and find the farthest point. The distance is
 * measured by the number of steps along the loop, regardless of its direction.
 *
 * Example Input:
 * .....
 * .S-7.
 * .|.|.
 * .L-J.
 * .....
 *
 * Example Output:
 * Total: 4
 *
 * @author [gabrielzschmitz]
 * @date [10/12/2023]
 */
#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

enum State {
  movingLeft,
  movingRight,
  movingUp,
  movingDown,
};

class Coordinates {
 private:
  int x;
  int y;

 public:
  Coordinates(int newX, int newY) {
    x = newX;
    y = newY;
  }
  int getX() { return x; }
  int getY() { return y; }
};

int interiorPoints(int area, int boundary_points) {
  int inter_points = area + 1 - (boundary_points / 2);
  return inter_points;
}

int area(list<Coordinates> &points) {
  int areaVal = 0;
  list<Coordinates>::iterator current, next;
  current = points.begin();
  next = points.begin();
  int firstX = current->getX();
  int firstY = current->getY();
  for (next++; next != points.end(); current++, next++) {
    int x1 = current->getX();
    int y2 = next->getY();
    int x2 = next->getX();
    int y1 = current->getY();
    int currEval = (x1 * y2) - (x2 * y1);
    areaVal += currEval;
  }

  int x2 = current->getX();
  int y2 = current->getY();

  int currEval = (x2 * firstY) - (firstX * y2);

  areaVal += currEval;

  return areaVal / 2;
}

int main(int argc, char **argv) {
  ifstream input_file;
  input_file.open("input");
  string line;

  list<string> grid;
  while (getline(input_file, line)) {
    grid.push_back(line);
  }
  input_file.close();

  list<string>::iterator startLine;
  int startPos = 0;
  bool startFound = false;

  int height = 0;

  list<Coordinates> points;

  // find start position
  for (startLine = grid.begin(); startLine != grid.end() && !startFound;
       startLine++) {
    for (startPos = 0; startPos < startLine->length() && !startFound;
         startPos++) {
      if ((*startLine)[startPos] == 'S') {
        startFound = true;
      }
    }
    height++;
  }
  startLine--;
  startPos--;

  State state;
  State startState;
  State endState;
  int distance = 0;
  list<string>::iterator row = startLine, rowAbove = startLine,
                         rowBelow = startLine;
  rowAbove--;
  rowBelow++;
  int column = startPos;
  if ((column < row->length() - 1) &&
      ((*row)[column + 1] == '-' || (*row)[column + 1] == 'J' ||
       (*row)[column + 1] == '7')) {
    state = movingRight;
    column++;
  } else if ((column > 0) &&
             ((*row)[column - 1] == '-' || (*row)[column - 1] == 'L' ||
              (*row)[column - 1] == 'F')) {
    state = movingLeft;
    column--;
  } else if ((row != grid.begin()) &&
             ((*rowAbove)[column] == '|' || (*rowAbove)[column] == '7' ||
              (*rowAbove)[column] == 'F')) {
    state = movingUp;
    row--;
    height--;
  } else if (rowBelow != grid.end() &&
             ((*rowBelow)[column] == '|' || (*rowBelow)[column] == 'L' ||
              (*rowBelow)[column] == 'J')) {
    state = movingDown;
    row++;
    height++;
  } else {
    cerr << "No viable path found!\n";
  }
  startState = state;
  distance++;

  while ((*row)[column] != 'S') {
    switch (state) {
      case movingLeft:
        if ((*row)[column] == '-') {
          column--;
        } else if ((*row)[column] == 'F') {
          points.push_back(Coordinates(column + 1, height + 1));
          state = movingDown;
          row++;
          height++;
        } else if ((*row)[column] == 'L') {
          points.push_back(Coordinates(column + 1, height + 1));
          state = movingUp;
          row--;
          height--;
        }
        break;
      case movingRight:
        if ((*row)[column] == '-') {
          column++;
        } else if ((*row)[column] == '7') {
          points.push_back(Coordinates(column + 1, height + 1));
          state = movingDown;
          row++;
          height++;
        } else if ((*row)[column] == 'J') {
          points.push_back(Coordinates(column + 1, height + 1));
          state = movingUp;
          row--;
          height--;
        }
        break;
      case movingUp:
        if ((*row)[column] == '|') {
          row--;
          height--;
        } else if ((*row)[column] == '7') {
          points.push_back(Coordinates(column + 1, height + 1));
          state = movingLeft;
          column--;
        } else if ((*row)[column] == 'F') {
          points.push_back(Coordinates(column + 1, height + 1));
          state = movingRight;
          column++;
        }
        break;
      case movingDown:
        if ((*row)[column] == '|') {
          row++;
          height++;
        } else if ((*row)[column] == 'J') {
          points.push_back(Coordinates(column + 1, height + 1));
          state = movingLeft;
          column--;
        } else if ((*row)[column] == 'L') {
          points.push_back(Coordinates(column + 1, height + 1));
          state = movingRight;
          column++;
        }
        break;
    }
    distance++;
  }
  endState = state;

  if (startState != endState) {
    points.push_front(Coordinates(column + 1, height + 1));
  }

  cout << "Steps = " << (distance / 2) << endl;

  return 0;
}
