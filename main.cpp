#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>

int order = 4;

struct Pair {
    int x;
    int y;

    Pair(int x, int y) : x(x), y(y) {}
    Pair(const Pair& p) : x(p.x), y(p.y) {}

    std::string toString() {
        std::stringstream res;
        res << "(" << x << ", " << y << ")";
        return res.str();
    }
};

Pair getCoord(int index) {
    // bounds checking; Number of points is ( 2 ^ order ) ^ 2 since it's a square
    // array for now
    if (index >= std::pow(4, order))
      return Pair(-1, -1);

    Pair order1[] = {
      Pair(0, 0), Pair(0, 1),
      Pair(1, 1), Pair(1, 0)
    };

    // Depth of recursion is mapped by (log base 4) + 1
    // Index < 4 : order 1
    // Index < 16 : order 2 ...
    int depth = order;

    int quadrant = index & 3;
    Pair coord(order1[quadrant]);
    // Start at order 1
    for (int i = 1; i < depth; i++) {
      // Gets if in quadrant 0, 1, 2, or 3
      index >>= 2;
      int nextq = index & 3;

      // First quadrant ? Reflect across y = x
      // Multiply by matrix
      // [ 0 1 ]
      // [ 1 0 ]
      // Equivalent to swapping x and y
      if (nextq == 0) {
        int temp = coord.y;
        coord.y = coord.x;
        coord.x = temp;
        // Fourth quadrant ?
      } else if (nextq == 3) {
        // Translate to center the origin
        double dist = (std::pow(2, i) - 1) / 2.0; // dist is half the width of a quadrant
        double xTranslated = ((double) coord.x) - dist;
        double yTranslated = ((double) coord.y) - dist;
        double temp = yTranslated;
        // Swap and take the negatives
        yTranslated = -xTranslated;
        xTranslated = -temp;
        // Translate back to regular
        coord.x = (int) (xTranslated + dist);
        coord.y = (int) (yTranslated + dist);
      }

      // Right quadrants ?
      // Add 2^order to x
      // Moves coordinate over by half of the width of the new square
      if (nextq == 2 || nextq == 3) {
        coord.x += std::pow(2, i);
      }

      // Bottom quadrants ?
      // Add 2^order to y
      // Moves coordinate over by half of the width of the new square
      if (nextq == 1 || nextq == 2) {
        coord.y += std::pow(2, i);
      }
    }
    return coord;
  }

int main(int argc, char** argv) {
    std::cout << "Enter order" << std::endl;
    std::cin >> order;
    std::vector<Pair> pairs;

    for (int i = 0; i < std::pow(4, order); i++) {
        pairs.push_back(getCoord(i));
    }
    
    for (int i = 0; i < std::pow(4, order); i++) {
        std::cout << i << ": " << pairs.at(i).toString() << std::endl;
    }

    return 0;
}