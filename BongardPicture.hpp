/**
 * Copyright 2014 UW-Madison, Database Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @author Qiang Zeng <qzeng@cs.wisc.edu>
 **/

#ifndef BONGARD_PICTURE_HPP_
#define BONGARD_PICTURE_HPP_

#include <cmath>
#include <cstdio>
#include <fstream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "Configurations.hpp"

namespace bongard {

/**
 * @brief Picture parameters
 */
struct picture_params {
  picture_params(int min_insides, std::mt19937 &gen,
                 std::uniform_int_distribution<> &coord_dist,
                 std::uniform_int_distribution<> &type_dist,
                 std::uniform_int_distribution<> &size_dist)
      : min_insides_(min_insides),
        gen_(gen),
        coord_dist_(coord_dist),
        type_dist_(type_dist),
        size_dist_(size_dist) {}

  /**
   * The minimum number of pairs of elements in which one should be inside of
   * the other.
   */
  const int min_insides_;
  std::mt19937 &gen_;
  std::uniform_int_distribution<> &coord_dist_;
  std::uniform_int_distribution<> &type_dist_;
  std::uniform_int_distribution<> &size_dist_;
};

struct Point {
  Point(int x, int y) : x_(x), y_(y) {}

  std::string string() const {
    std::ostringstream s;
    s << "(" << x_ << "," << y_ << ")";
    return s.str();
  }

  int x_;
  int y_;
};

/**
 * @brief The bounding rectangle
 */
class Rectangle {
 public:
  Rectangle();
  Rectangle(int x, int y, int width, int height);

  void set(int x, int y, int width, int height);

  /**
   *
   * @return The left-bottom point of the bounding rectangle.
   */
  const Point &getLeftBottom() const;
  /**
   *
   * @return The right-top point of the bounding rectangle.
   */
  const Point &getRightTop() const;

  /**
   * @brief Test whether this rectangle is inside of the \p other with an
   *        separate space of at least \p margin.
   *
   * @param other	The second rectangle to be checked with.
   * @param margin The minimum horizontal/vertical separate space between the
   *               two elements.
   * @return Whether this rectangle is inside of the \p other
   */
  bool isInside(const Rectangle &other, int margin = MARGIN) const;

  /**
   * @brief Test whether this rectangle is overlapped with the \p other with an
   *        separate space of at most \p margin.
   *
   * @param other The second rectangle to be checked with.
   * @param margin The minimum horizontal/vertical separate space between the
   *               two elements.
   * @return Whether this rectangle is overlapped with the \p other
   */
  bool isOverlapped(const Rectangle &other, int margin = MARGIN) const;

  /**
   * @brief Test whether this rectangle is on the east side of the \p other.
   *
   * @param other The second rectangle to be checked with.
   * @return Whether this rectangle is on the east side of the \p other.
   */
  bool isEastOf(const Rectangle &other) const;

  /**
   * @brief Test whether this rectangle is on the north side of the \p other.
   *
   * @param other The second rectangle to be checked with.
   * @return Whether this rectangle is on the north side of the \p other.
   */
  bool isNorthOf(const Rectangle &other) const;

 private:
  /** the left-bottom point */
  Point leftBottom_;

  /** the right-top point */
  Point rightTop_;

  /** the width of the rectangle */
  int width_;

  /** the height of the rectangle */
  int height_;
};

/**
 * @brief Base class of the Bongard element
 */
class BongardElement {
 public:
  /**
   * @brief Constructor.
   *
   * @param id The element id.
   */
  BongardElement(id_type id);

  virtual ~BongardElement();

  inline id_type getId() const { return id_; }

  void setId(id_type id);

  /**
   *
   * @return The minimum bounding rectangle.
   */
  const Rectangle &getOuterBoundRect() const;

  /**
   *
   * @return The maximum enclosed square.
   */
  const Rectangle &getInnerBoundRect() const;

  /**
   * @brief	Test if it is partially overlapped with the \p other.
   *
   * @param other The second element to be checked with.
   * @return True if this element is partially overlapped with the \p other.
   */
  bool conflict(const BongardElement *other) const;

  /**
   * @brief Test if it is overlapped with the \p other.
   *
   * @param other The second element to be checked with.
   * @return True if this element is overlapped with the \p other.
   */
  bool isOverlapped(const BongardElement *other) const;

  /**
   * @brief Test if it is inside of the \p other
   *
   * @param other The second element to be checked with.
   * @return True if this element is inside of the \p other.
   */
  bool isInside(const BongardElement *other) const;

  /**
   * @brief Test if it is on the east of the \p other
   *
   * @param other The second element to be checked with.
   * @return True if this element is on the east of the \p other.
   */
  bool isEastOf(const BongardElement *other) const;

  /**
   * @brief Test if it is on the north of the \p other
   *
   * @param other The second element to be checked with.
   * @return True if this element is on the north of the \p other.
   */
  bool isNorthOf(const BongardElement *other) const;

  /**
   * @brief Test boundary overflow.
   * @return True if this element exceed the picture boundary.
   */
  bool overflow() const;

  virtual void output(FILE *circle,
                      FILE *rec,
                      FILE *tri,
                      FILE *tri_up,
                      FILE *tri_down) const = 0;

  virtual std::string string() const = 0;

 protected:
  void setOuterBoundRect(int x, int y, int w, int h);
  void setInnerBoundRect(int x, int y, int w, int h);

 private:
  friend class BongardPicture;

  id_type id_;
  Rectangle outer_rect_;
  Rectangle inner_rect_;
};

/**
 * @brief circle element
 */
class BongardCircle : public BongardElement {
 public:
  /**
   * @brief Constructor.
   *
   * @param x	Left bottom x-coordinate.
   * @param y	Left bottom y-coordinate.
   * @param d Diameter.
   */
  BongardCircle(int x, int y, int d);

  void output(FILE *circle,
              FILE *rec,
              FILE *tri,
              FILE *tri_up,
              FILE *tri_down) const;

  std::string string() const;

 private:
  Point leftBottom_;
  int radius_;
};

/**
 * @brief square element
 */
class BongardSquare : public BongardElement {
 public:
  /**
   * @brief Constructor.
   *
   * @param x Left bottom x-coordinate.
   * @param y Left bottom y-coordinate.
   * @param w	Width.
   */
  BongardSquare(int x, int y, int w);

  void output(FILE *circle,
              FILE *rec,
              FILE *tri,
              FILE *tri_up,
              FILE *tri_down) const;

  std::string string() const;

 private:
  Point leftBottom_;
  int w_;
};

class BongardTriangle : public BongardElement {
 public:
  /**
   * @brief Constructor.
   *
   * @param x	Left bottom x-coordinate.
   * @param y	Left bottom y-coordinate.
   * @param d	Length of the horizontal edge.
   * @param orient True if the point is up.
   */
  BongardTriangle(int x, int y, int w, bool orient);

  void output(FILE *circle,
              FILE *rec,
              FILE *tri,
              FILE *tri_up,
              FILE *tri_down) const;

  std::string string() const;

 private:
  Point leftBottom_;
  int w_;
  bool point_orientation_;
};

/**
 * @brief Bongard picture
 */
class BongardPicture {
 public:
  /**
   * @brief Constructor.
   *
   * @param p Picture parameters.
   */
  BongardPicture(picture_params p);

  /**
   * @brief Assign IDs to itself and to its elements.
   *
   * @param pid Starting picture id.
   * @param eid	Starting element id.
   */
  void assignIDs(id_type &pid, id_type &eid);
  inline int size() const { return elems_.size(); }

  /**
   * @brief Create a picture with \p size elements.
   * @param size The number of elements in the picture.
   * @return True if a picture is successfully generated; otherwise, false.
   */
  bool createPicture(unsigned size);

  /**
   * @brief Randomly generate an element
   */
  BongardElement *createElement();

  /**
   * @brief Test if the element violates any restriction
   * @details An element should be partiall overlapped with another element in
   *          the same picture and should be fully enclosed in the picture.
   *
   * @param elem The checking element.
   * @return True if \p elem is valid.
   */
  bool isValid(BongardElement *elem) const;

 private:
  void populate();

  void output(FILE *elem,
              FILE *circle,
              FILE *rec,
              FILE *tri,
              FILE *tri_up,
              FILE *tri_down,
              FILE *inside,
              FILE *north,
              FILE *east);

  picture_params &p_;
  std::vector<std::unique_ptr<BongardElement>> elems_;
  std::vector<std::pair<BongardElement *, BongardElement *>> inside_;
  std::vector<std::pair<BongardElement *, BongardElement *>> north_;
  std::vector<std::pair<BongardElement *, BongardElement *>> east_;

  id_type id_;

  friend class BongardGenerator;
};

}  // namespace bongard

#endif /* BONGARD_PICTURE_HPP_ */
