#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <cctype>
#include <iomanip>

namespace sajfutdinov {
  struct Point {
    int x, y;
    Point() : x(0), y(0) {};
  };

  struct Polygon {
    std::vector<Point> points;
  };

  struct PrintPoint {
    void operator()(const Point& p) const {
      std::cout << "(" << p.x << ";" << p.y << ") ";
    }
  };

  struct PrintPolygon {
    void operator()(const Polygon& poly) const {
      std::for_each(poly.points.begin(), poly.points.end(), PrintPoint());
      std::cout << "\n";
    }
  };

  struct AreaCalculator {
    double operator()(const Polygon& poly) const {
      double area = 0.0;
      size_t n = poly.points.size();
      for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += (poly.points[i].x + poly.points[j].x) * (poly.points[i].y - poly.points[j].y);
      }
      return std::abs(area) / 2.0;
    }
  };

  void calculateArea(const std::vector<Polygon>& polygons, const std::string& type) {
    int count = 0;
    double totalArea = 0.0;
    for (const auto& poly : polygons) {
      if (type == "MEAN")
      {
        totalArea += AreaCalculator()(poly);
        count++;
        continue;
      }
      if (std::all_of(type.begin(), type.end(), [](char c) { return std::isdigit(c); }))
      {
        if (poly.points.size() == std::stoul(type))
        {
          totalArea += AreaCalculator()(poly);
          count++;
          continue;
        }
      }
      if ((poly.points.size() % 2 == 0 && type == "EVEN") || (poly.points.size() % 2 != 0 && type == "ODD"))
      {
        totalArea += AreaCalculator()(poly);
        count++;
        continue;
      }
    }
    std::cout << std::fixed << std::fixed << std::setprecision(1) << (type == "MEAN" ? totalArea / count : totalArea) << "\n";
  }

  void calculateMax(const std::vector<Polygon>& polygons, const std::string& type) {
    if ((type != "AREA") && (type != "VERTEXES"))
    {
      std::cerr << "<INVALID COMMMAND>\n";
      return;
    }
    auto maxIt = std::max_element(polygons.begin(), polygons.end(), [&type](const Polygon& a, const Polygon& b) {
      if (type == "AREA") {
        return AreaCalculator()(a) < AreaCalculator()(b);
      }
      else {
        return a.points.size() < b.points.size();
      }
      });
    if (type == "AREA") {
      std::cout << std::fixed << std::fixed << std::setprecision(1) << AreaCalculator()(*maxIt) << "\n";
    }
    else if (type == "VERTEXES") {
      std::cout << std::fixed << std::fixed << std::setprecision(1) << maxIt->points.size() << "\n";
    }
  }

  void calculateMin(const std::vector<Polygon>& polygons, const std::string& type) {
    if ((type != "AREA") && (type != "VERTEXES"))
    {
      std::cerr << "<INVALID COMMMAND>\n";
      return;
    }
    auto minIt = std::min_element(polygons.begin(), polygons.end(), [&type](const Polygon& a, const Polygon& b)
      {
        if (type == "AREA")
        {
          return AreaCalculator()(a) < AreaCalculator()(b);
        }
        else
        {
          return a.points.size() < b.points.size();
        }
      });
    if (type == "AREA") {
      std::cout << std::fixed << std::fixed << std::setprecision(1) << AreaCalculator()(*minIt) << "\n";
    }
    else if (type == "VERTEXES") {
      std::cout << std::fixed << std::fixed << std::setprecision(1) << minIt->points.size() << "\n";
    }
  }

  void countPolygons(const std::vector<Polygon>& polygons, const std::string& type) {
    if (type == "EVEN") {
      auto count = std::count_if(polygons.begin(), polygons.end(), [](const Polygon& poly) {
        return poly.points.size() % 2 == 0;
        });
      std::cout << count << "\n";
    }
    else if (type == "ODD") {
      auto count = std::count_if(polygons.begin(), polygons.end(), [](const Polygon& poly) {
        return poly.points.size() % 2 != 0;
        });
      std::cout << count << "\n";
    }
    else if (std::all_of(type.begin(), type.end(), [](char c) { return std::isdigit(c); })) {
      size_t numpoints = std::stoul(type);
      auto count = std::count_if(polygons.begin(), polygons.end(), [numpoints](const Polygon& poly) {
        return poly.points.size() == numpoints;
        });
      std::cout << count << "\n";
    }
    else
    {
      std::cerr << "<INVALID COMMAND>\n";
      return;
    }
  }

  void lessArea(const std::vector<Polygon>& polygons, const std::string& stringPolygon)
  {
    Polygon lessAreaPolygon;
    unsigned long int numpoints = stringPolygon[0] - '0';
    for (size_t i = 1; i < 6 * numpoints - 1; i = i + 6)
    {
      if (stringPolygon.length() != (numpoints * 6 + 1)) break;
      if (stringPolygon[i] == ' ' && stringPolygon[i + 1] == '(' && stringPolygon[i + 3] == ';' && stringPolygon[i + 5] == ')')
      {
        Point lessAreaPoint;
        lessAreaPoint.x = stringPolygon[i + 2] - '0';
        lessAreaPoint.y = stringPolygon[i + 4] - '0';
        lessAreaPolygon.points.push_back(lessAreaPoint);
      }
    }
    auto lessAreaCount = std::count_if(polygons.begin(), polygons.end(), [lessAreaPolygon](const Polygon& poly) {
      return AreaCalculator()(poly) < AreaCalculator()(lessAreaPolygon);
      });
    std::cout << std::fixed << std::fixed << std::setprecision(1) << lessAreaCount << "\n";
  }

  void intersections(const std::vector<Polygon>& polygons, const std::string& stringPolygon)
  {
    std::for_each(polygons.begin(), polygons.end(), PrintPolygon());
    Polygon intersectionsPolygon;
    unsigned long int numpoints = stringPolygon[0] - '0';
    for (size_t i = 1; i < stringPolygon.length(); i = i + 6)
    {
      Point intersectionsPoint;
      //if (stringPolygon.length() != (numpoints * 6 + 1)) break;
      if (stringPolygon[i] == ' ' && stringPolygon[i + 1] == '(' && stringPolygon[i + 3] == ';' && stringPolygon[i + 5] == ')')
      {
        intersectionsPoint.x = stringPolygon[i + 2] - '0';
        intersectionsPoint.y = stringPolygon[i + 4] - '0';
        intersectionsPolygon.points.push_back(intersectionsPoint);
      }
      else if (stringPolygon[i] == ' ' && stringPolygon[i + 1] == '(' && stringPolygon[i + 2] == '-')
      {
        intersectionsPoint.x = (stringPolygon[i + 3] - '0') * (-1);
        if (stringPolygon[i + 4] == ';')
        {
          if (stringPolygon[i + 5] == '-' && stringPolygon[i + 7] == ')')
          {
            intersectionsPoint.y = (stringPolygon[i + 6] - '0') * (-1);
            i = i + 2;
          }
          else if (stringPolygon[i + 6] == ')')
          {
            intersectionsPoint.y = stringPolygon[i + 5] - '0';
            i = i + 1;
          }
        }
        intersectionsPolygon.points.push_back(intersectionsPoint);
      }
      else if (stringPolygon[i] == ' ' && stringPolygon[i + 1] == '(' && stringPolygon[i + 3] == ';' && stringPolygon[i + 4] == '-' && stringPolygon[i + 6])
      {
        intersectionsPoint.x = stringPolygon[i + 2] - '0';
        intersectionsPoint.y = (stringPolygon[i + 5] - '0') * (-1);
        intersectionsPolygon.points.push_back(intersectionsPoint);
        i = i + 1;
      }
      sajfutdinov::PrintPolygon()(intersectionsPolygon);
    }
    if (intersectionsPolygon.points.size() != numpoints)
    {
      std::cerr << "<INVALID COMMAND>\n";
      return;
    }

    auto count = std::count_if(polygons.begin(), polygons.end(), [intersectionsPolygon](const Polygon& poly) {
      for (size_t i = 0; i < intersectionsPolygon.points.size(); ++i) {
        size_t next = (i + 1) % intersectionsPolygon.points.size();
        for (size_t j = 0; j < poly.points.size(); ++j) {
          size_t nextB = (j + 1) % poly.points.size();
          if (((intersectionsPolygon.points[next].x - intersectionsPolygon.points[i].x) * (poly.points[j].y - intersectionsPolygon.points[i].y) -
            (intersectionsPolygon.points[next].y - intersectionsPolygon.points[i].y) * (poly.points[j].x - intersectionsPolygon.points[i].x)) *
            ((intersectionsPolygon.points[next].x - intersectionsPolygon.points[i].x) * (poly.points[nextB].y - intersectionsPolygon.points[i].y) -
              (intersectionsPolygon.points[next].y - intersectionsPolygon.points[i].y) * (poly.points[nextB].x - intersectionsPolygon.points[i].x)) < 0 &&
            ((poly.points[nextB].x - poly.points[j].x) * (intersectionsPolygon.points[i].y - poly.points[j].y) -
              (poly.points[nextB].y - poly.points[j].y) * (intersectionsPolygon.points[i].x - poly.points[j].x)) *
            ((poly.points[nextB].x - poly.points[j].x) * (intersectionsPolygon.points[next].y - poly.points[j].y) -
              (poly.points[nextB].y - poly.points[j].y) * (intersectionsPolygon.points[next].x - poly.points[j].x)) < 0) {
            return true;
          }
        }
      }
      return false;
      });
    std::cout << count << "\n";
  }

}

int main(int argc, char* argv[]) {
  if (argc != 2)
  {
    std::cerr << "Bye";
    return 1;
  }

  std::vector<sajfutdinov::Polygon> polygons;
  std::ifstream input(argv[1]);
  if (!input.is_open())
  {
    std::cerr << "No file!";
    return 1;
  }

  std::string line = "";
  while (std::getline(input, line))
  {
    sajfutdinov::Polygon poly;
    char c = line[0];
    unsigned long int numpoints = 0;
    for (char a : "123456789")
    {
      if (c == a)
      {
        numpoints = c - '0';
        break;
      }
    }
    if (numpoints != 0)
    {
      for (size_t i = 1; i < 6 * numpoints - 1; i = i + 6)
      {
        if (line.length() != (numpoints * 6 + 1)) break;
        if (line[i] == ' ' && line[i + 1] == '(' && line[i + 3] == ';' && line[i + 5] == ')')
        {
          sajfutdinov::Point point;
          point.x = line[i + 2] - '0';
          point.y = line[i + 4] - '0';
          poly.points.push_back(point);
        }
      }
      if (poly.points.size() == numpoints) polygons.push_back(poly);
    }
  }

  while (std::getline(std::cin, line))
  {
    if (line.length() == 0) continue;
    if (line.length() > 4)
    {
      if (line.substr(0, 3) == "MAX")
      {
        std::string type = line.substr(4, line.length() - 1);
        sajfutdinov::calculateMax(polygons, type);
        continue;
      }
      if (line.substr(0, 3) == "MIN")
      {
        std::string type = line.substr(4, line.length() - 1);
        sajfutdinov::calculateMin(polygons, type);
        continue;
      }
    }
    if (line.length() > 5)
    {
      if (line.substr(0, 4) == "AREA")
      {
        std::string type = line.substr(5, line.length() - 1);
        sajfutdinov::calculateArea(polygons, type);
        continue;
      }
    }
    if (line.length() > 6)
    {
      if (line.substr(0, 5) == "COUNT")
      {
        std::string type = line.substr(6, line.length() - 1);
        if (type == "0" || type == "1" || type == "2")
        {
          std::cerr << "<INVALID COMMAND>\n";
          continue;
        }
        sajfutdinov::countPolygons(polygons, type);
        continue;
      }
    }
    if (line.length() > 9)
    {
      if (line.substr(0, 8) == "LESSAREA")
      {
        std::string stringPolygon = line.substr(9, line.length() - 1);
        sajfutdinov::lessArea(polygons, stringPolygon);
        continue;
      }
    }
    if (line.length() > 14)
    {
      if (line.substr(0, 13) == "INTERSECTIONS")
      {
        std::string stringPolygon = line.substr(14, line.length() - 1);
        sajfutdinov::intersections(polygons, stringPolygon);
        continue;
      }
    }
    std::cerr << "<INVALID COMMAND>\n";
  }

  return 0;
}
use
