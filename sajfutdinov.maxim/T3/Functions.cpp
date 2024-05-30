#include "functions.h"

double sajfutdinov::AreaCalculator::operator()(const Polygon& poly) const {
  double area = 0.0;
  size_t n = poly.points.size();
  for (size_t i = 0; i < n; ++i) {
    size_t j = (i + 1) % n;
    area += (poly.points[i].x + poly.points[j].x) * (poly.points[i].y - poly.points[j].y);
  }
  return std::abs(area) / 2.0;
}

void sajfutdinov::calculateArea(const std::vector<Polygon>& polygons, const std::string& type) {
  int count = 0;
  double totalArea = 0.0;
  if (type == "0" || type == "1" || type == "2")
  {
    std::cout << INVALID_COMMAND;
    return;
  }
  for (const auto& poly : polygons) {
    if (type == "MEAN")
    {
      if (polygons.size() > 0)
      {
        totalArea += AreaCalculator()(poly);
        count++;
        continue;
      }
      else
      {
        std::cout << INVALID_COMMAND;
        return;
      }
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
  if (type == "MEAN" && count == 0)
  {
    std::cout << INVALID_COMMAND;
    return;
  }
  std::cout << std::fixed << std::setprecision(1) << (type == "MEAN" ? totalArea / count : totalArea) << "\n";
}

void sajfutdinov::calculateMax(const std::vector<Polygon>& polygons, const std::string& type) {
  if ((type != "AREA") && (type != "VERTEXES"))
  {
    std::cout << INVALID_COMMAND;
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
    std::cout << std::fixed << std::setprecision(1) << AreaCalculator()(*maxIt) << "\n";
  }
  else if (type == "VERTEXES") {
    std::cout << std::fixed << std::setprecision(1) << maxIt->points.size() << "\n";
  }
}

void sajfutdinov::calculateMin(const std::vector<Polygon>& polygons, const std::string& type) {
  if ((type != "AREA") && (type != "VERTEXES"))
  {
    std::cout << INVALID_COMMAND;
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
    std::cout << std::fixed << std::setprecision(1) << AreaCalculator()(*minIt) << "\n";
  }
  else if (type == "VERTEXES") {
    std::cout << std::fixed << std::setprecision(1) << minIt->points.size() << "\n";
  }
}

void sajfutdinov::countPolygons(const std::vector<Polygon>& polygons, const std::string& type) {
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
    if (numpoints == 1 || numpoints == 2 || numpoints == 0)
    {
      std::cout << INVALID_COMMAND;
      return;
    }
    auto count = std::count_if(polygons.begin(), polygons.end(), [numpoints](const Polygon& poly) {
      return poly.points.size() == numpoints;
      });
    std::cout << count << "\n";
  }
  else
  {
    std::cout << INVALID_COMMAND;
    return;
  }
}

void sajfutdinov::lessArea(const std::vector<Polygon>& polygons, const std::string& strPoly)
{
  Polygon lessAreaPolygon;
  unsigned long int numpoints = strPoly[0] - '0';
  if (numpoints == 1 || numpoints == 2 || numpoints == 0)
  {
    std::cout << INVALID_COMMAND;
    return;
  }
  for (size_t i = 1; i < strPoly.length(); i = i + 6)
  {
    Point lessAreaPoint;
    if (strPoly[i] == ' ' && strPoly[i + 1] == '(' && strPoly[i + 3] == ';' && strPoly[i + 5] == ')')
    {
      lessAreaPoint.x = strPoly[i + 2] - '0';
      lessAreaPoint.y = strPoly[i + 4] - '0';
      lessAreaPolygon.points.push_back(lessAreaPoint);
    }
    else if (strPoly[i] == ' ' && strPoly[i + 1] == '(' && strPoly[i + 2] == '-')
    {
      lessAreaPoint.x = (strPoly[i + 3] - '0') * (-1);
      if (strPoly[i + 4] == ';')
      {
        if (strPoly[i + 5] == '-' && strPoly[i + 7] == ')')
        {
          lessAreaPoint.y = (strPoly[i + 6] - '0') * (-1);
          i = i + 2;
        }
        else if (strPoly[i + 6] == ')')
        {
          lessAreaPoint.y = strPoly[i + 5] - '0';
          i = i + 1;
        }
      }
      lessAreaPolygon.points.push_back(lessAreaPoint);
    }
    else if (strPoly[i] == ' ' && strPoly[i + 1] == '(' && strPoly[i + 3] == ';' && strPoly[i + 4] == '-' && strPoly[i + 6] == ')')
    {
      lessAreaPoint.x = strPoly[i + 2] - '0';
      lessAreaPoint.y = (strPoly[i + 5] - '0') * (-1);
      lessAreaPolygon.points.push_back(lessAreaPoint);
      i = i + 1;
    }
  }
  if (lessAreaPolygon.points.size() != numpoints)
  {
    std::cout << INVALID_COMMAND;
    return;
  }
  auto lessAreaCount = std::count_if(polygons.begin(), polygons.end(), [lessAreaPolygon](const Polygon& poly) {
    return AreaCalculator()(poly) < AreaCalculator()(lessAreaPolygon);
    });
  std::cout << std::fixed << std::setprecision(1) << lessAreaCount << "\n";
}

double sajfutdinov::cross(const Point& O, const Point& A, const Point& B) {
  return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

bool sajfutdinov::isPointOnSegment(const Point& p, const Point& a, const Point& b) {
  return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
    std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

bool sajfutdinov::doSegmentsIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
  double d1 = cross(q1, q2, p1);
  double d2 = cross(q1, q2, p2);
  double d3 = cross(p1, p2, q1);
  double d4 = cross(p1, p2, q2);

  if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
    ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
    return true;
  }

  if (d1 == 0 && isPointOnSegment(p1, q1, q2)) return true;
  if (d2 == 0 && isPointOnSegment(p2, q1, q2)) return true;
  if (d3 == 0 && isPointOnSegment(q1, p1, p2)) return true;
  if (d4 == 0 && isPointOnSegment(q2, p1, p2)) return true;

  return false;
}

bool sajfutdinov::isPointInsidePolygon(const Point& p, const Polygon& poly) {
  int n = poly.points.size();
  int inf = 1e5;
  Point extreme(inf, p.y);
  int count = 0, i = 0;

  do {
    int next = (i + 1) % n;
    if (doSegmentsIntersect(poly.points[i], poly.points[next], p, extreme)) {
      if (cross(poly.points[i], p, poly.points[next]) == 0) {
        return isPointOnSegment(p, poly.points[i], poly.points[next]);
      }
      count++;
    }
    i = next;
  } while (i != 0);

  return count % 2 == 1;
}

bool sajfutdinov::doPolygonsIntersect(const Polygon& poly1, const Polygon& poly2) {
  for (size_t i = 0; i < poly1.points.size(); ++i) {
    for (size_t j = 0; j < poly2.points.size(); ++j) {
      size_t next_i = (i + 1) % poly1.points.size();
      size_t next_j = (j + 1) % poly2.points.size();
      if (doSegmentsIntersect(poly1.points[i], poly1.points[next_i], poly2.points[j], poly2.points[next_j])) {
        return true;
      }
    }
  }
  for (const Point& point : poly1.points)
  {
    if (isPointInsidePolygon(point, poly2)) return true;
  }
  for (const Point& point : poly2.points)
  {
    if (isPointInsidePolygon(point, poly1)) return true;
  }
  return false;
}


void sajfutdinov::intersections(const std::vector<Polygon>& polygons, const std::string& strPoly)
{
  Polygon intrscPoly;
  unsigned long int numpoints = strPoly[0] - '0';
  if (numpoints == 1 || numpoints == 2 || numpoints == 0)
  {
    std::cout << INVALID_COMMAND;
    return;
  }
  for (size_t i = 1; i < strPoly.length(); i = i + 6)
  {
    Point intersectionsPoint;
    if (strPoly[i] == ' ' && strPoly[i + 1] == '(' && strPoly[i + 3] == ';' && strPoly[i + 5] == ')')
    {
      intersectionsPoint.x = strPoly[i + 2] - '0';
      intersectionsPoint.y = strPoly[i + 4] - '0';
      intrscPoly.points.push_back(intersectionsPoint);
    }
    else if (strPoly[i] == ' ' && strPoly[i + 1] == '(' && strPoly[i + 2] == '-')
    {
      intersectionsPoint.x = (strPoly[i + 3] - '0') * (-1);
      if (strPoly[i + 4] == ';')
      {
        if (strPoly[i + 5] == '-' && strPoly[i + 7] == ')')
        {
          intersectionsPoint.y = (strPoly[i + 6] - '0') * (-1);
          i = i + 2;
        }
        else if (strPoly[i + 6] == ')')
        {
          intersectionsPoint.y = strPoly[i + 5] - '0';
          i = i + 1;
        }
      }
      intrscPoly.points.push_back(intersectionsPoint);
    }
    else if (strPoly[i] == ' ' && strPoly[i + 1] == '(' && strPoly[i + 3] == ';' && strPoly[i + 4] == '-' && strPoly[i + 6] == ')')
    {
      intersectionsPoint.x = strPoly[i + 2] - '0';
      intersectionsPoint.y = (strPoly[i + 5] - '0') * (-1);
      intrscPoly.points.push_back(intersectionsPoint);
      i = i + 1;
    }
  }
  if (intrscPoly.points.size() != numpoints)
  {
    std::cout << INVALID_COMMAND;
    return;
  }

  auto count = std::count_if(polygons.begin(), polygons.end(), [intrscPoly](const Polygon& poly) {
    return doPolygonsIntersect(intrscPoly, poly);
    });
  std::cout << count << "\n";
}
