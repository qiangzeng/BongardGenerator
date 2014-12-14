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

#include <cmath>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>

#include "BongardPicture.hpp"

namespace bongard {

BongardPicture::BongardPicture(picture_params p) : p_(p), id_(-1) {}

bool BongardPicture::isValid(BongardElement *check) const {
  if (check->overflow()) {
    return false;
  }
  for (auto &elem : elems_) {
    if (elem->conflict(check)) {
      return false;
    }
  }
  return true;
}

BongardElement *BongardPicture::createElement() {
  int size = p_.size_dist_(p_.gen_);
  int x = p_.coord_dist_(p_.gen_);
  int y = p_.coord_dist_(p_.gen_);
  int fig = p_.type_dist_(p_.gen_);

  if (fig == 1) return new BongardSquare(x, y, size);
  if (fig == 2) return new BongardCircle(x, y, size);
  if (fig == 3) return new BongardTriangle(x, y, size, 0);
  if (fig == 4) return new BongardTriangle(x, y, size, 1);

  return NULL;
}

bool BongardPicture::createPicture(unsigned size) {
  while (elems_.size() < size) {
    int error_trial = 0;
    while (true) {
      std::unique_ptr<BongardElement> elem(createElement());
      if (isValid(elem.get())) {
        elems_.emplace_back(elem.release());
        break;
      } else {
        error_trial++;
        if (error_trial > MAX_TRY) {
          return false;
        }
      }
    }
  }

  populate();
  return inside_.size() >= p_.min_insides_;
}

void BongardPicture::populate() {
  for (size_t i = 0; i < elems_.size(); i++) {
    BongardElement *elem1 = elems_[i].get();
    for (size_t j = i + 1; j < elems_.size(); j++) {
      BongardElement *elem2 = elems_[j].get();
      if (elem1->isInside(elem2)) {
        inside_.emplace_back(elem1, elem2);
      } else if (elem2->isInside(elem1)) {
        inside_.emplace_back(elem2, elem1);
      } else {
        if (elem1->isEastOf(elem2)) {
          east_.emplace_back(elem1, elem2);
        } else if (elem2->isEastOf(elem1)) {
          east_.emplace_back(elem2, elem1);
        }

        if (elem1->isNorthOf(elem2)) {
          north_.emplace_back(elem1, elem2);
        } else {
          north_.emplace_back(elem2, elem1);
        }
      }
    }
  }
}

bool BongardElement::conflict(const BongardElement *other) const {
  if (isInside(other)) return false;
  if (isInside(other)) return false;
  return isOverlapped(other);
}

bool BongardElement::isOverlapped(const BongardElement *other) const {
  return getOuterBoundRect().isOverlapped(other->getOuterBoundRect());
}

bool BongardElement::isInside(const BongardElement *other) const {
  return getOuterBoundRect().isInside(other->getInnerBoundRect());
}

bool BongardElement::isEastOf(const BongardElement *other) const {
  return getOuterBoundRect().isEastOf(other->getOuterBoundRect());
}

bool BongardElement::isNorthOf(const BongardElement *other) const {
  return getOuterBoundRect().isNorthOf(other->getOuterBoundRect());
}

bool BongardElement::overflow() const {
  auto &tp = getOuterBoundRect().getRightTop();
  return tp.x_ >= BOUND || tp.y_ >= BOUND;
}

bool Rectangle::isInside(const Rectangle &other, int margin) const {
  return leftBottom_.x_ >= other.leftBottom_.x_ + margin &&
         leftBottom_.y_ >= other.leftBottom_.y_ + margin &&
         rightTop_.x_ <= other.rightTop_.x_ - margin &&
         rightTop_.y_ <= other.rightTop_.y_ - margin;
}

bool Rectangle::isOverlapped(const Rectangle &other, int margin) const {
  return leftBottom_.x_ <= other.rightTop_.x_ + margin &&
         rightTop_.x_ + margin >= other.leftBottom_.x_ &&
         leftBottom_.y_ <= other.rightTop_.y_ + margin &&
         rightTop_.y_ + margin >= other.leftBottom_.y_;
}

bool Rectangle::isEastOf(const Rectangle &other) const {
  return leftBottom_.x_ > other.rightTop_.x_;
}

bool Rectangle::isNorthOf(const Rectangle &other) const {
  return leftBottom_.y_ > other.rightTop_.y_;
}

const Rectangle &BongardElement::getOuterBoundRect() const {
  return outer_rect_;
}

const Rectangle &BongardElement::getInnerBoundRect() const {
  return inner_rect_;
}

void BongardElement::setOuterBoundRect(int x, int y, int w, int h) {
  outer_rect_.set(x, y, w, h);
}

void BongardElement::setInnerBoundRect(int x, int y, int w, int h) {
  inner_rect_.set(x, y, w, h);
}

BongardCircle::BongardCircle(int x, int y, int diameter)
    : BongardElement(-1), leftBottom_(x, y), radius_(diameter / 2) {
  setOuterBoundRect(x, y, diameter, diameter);
  setInnerBoundRect(x + radius_ - std::sqrt(2) / 2.0 * radius_,
                    y + radius_ - std::sqrt(2) / 2.0 * radius_,
                    std::sqrt(2) * radius_, std::sqrt(2) * radius_);
}

BongardSquare::BongardSquare(int x, int y, int w)
    : BongardElement(-1), leftBottom_(x, y), w_(w) {
  setOuterBoundRect(x, y, w, w);
  setInnerBoundRect(x, y, w, w);
}

BongardTriangle::BongardTriangle(int x, int y, int width, bool up)
    : BongardElement(-1), leftBottom_(x, y), w_(width), point_orientation_(up) {
  int height = (int)std::ceil((float)(width * sqrt(3) / 2.0));
  setOuterBoundRect(x, y, width, height);

  int inner_width = height * width / (height + width);
  if (up) {
    setInnerBoundRect(x + (width - inner_width) / 2, y, inner_width,
                      inner_width);
  } else {
    setInnerBoundRect(x + (width - inner_width) / 2, y + height - inner_width,
                      inner_width, inner_width);
  }
}

Rectangle::Rectangle()
    : leftBottom_(0, 0), rightTop_(0, 0), width_(0), height_(0) {}

Rectangle::Rectangle(int x, int y, int width, int height)
    : leftBottom_(x, y),
      rightTop_(x + width, y + height),
      width_(width),
      height_(height) {}

void Rectangle::set(int x, int y, int width, int height) {
  leftBottom_.x_ = x;
  leftBottom_.y_ = y;
  rightTop_.x_ = x + width;
  rightTop_.y_ = y + width;
  width_ = width;
  height_ = height;
}

const Point &Rectangle::getLeftBottom() const { return leftBottom_; }
const Point &Rectangle::getRightTop() const { return rightTop_; }

BongardElement::~BongardElement() {}

BongardElement::BongardElement(id_type id) : id_(id) {}

void BongardElement::setId(id_type id) { id_ = id; }

void BongardPicture::assignIDs(id_type &pid, id_type &eid) {
  id_ = pid++;
  for (auto &elm : elems_) {
    elm->setId(eid++);
  }
}

void BongardPicture::output(FILE *elem, FILE *circle, FILE *rec, FILE *tri,
                            FILE *tri_up, FILE *tri_down, FILE *inside,
                            FILE *north, FILE *east) {
  for (auto &e : elems_) {
    fprintf(elem, "%llu|%llu\n", id_, e->id_);
    e->output(circle, rec, tri, tri_up, tri_down);
  }

  for (auto &pair : inside_) {
    fprintf(inside, "%llu|%llu\n", pair.first->id_, pair.second->id_);
  }

  for (auto &pair : north_) {
    fprintf(north, "%llu|%llu\n", pair.first->id_, pair.second->id_);
  }

  for (auto &pair : east_) {
    fprintf(east, "%llu|%llu\n", pair.first->id_, pair.second->id_);
  }
}

void BongardCircle::output(FILE *circle, FILE *rec, FILE *tri, FILE *tri_up,
                           FILE *tri_down) const {
  fprintf(circle, "%llu\n", getId());
}

void BongardSquare::output(FILE *circle, FILE *rec, FILE *tri, FILE *tri_up,
                           FILE *tri_down) const {
  fprintf(rec, "%llu\n", getId());
}

void BongardTriangle::output(FILE *circle, FILE *rec, FILE *tri, FILE *tri_up,
                             FILE *tri_down) const {
  fprintf(tri, "%llu\n", getId());
  if (point_orientation_) {
    fprintf(tri_up, "%llu\n", getId());
  } else {
    fprintf(tri_down, "%llu\n", getId());
  }
}

std::string BongardCircle::string() const {
  std::ostringstream str;
  str << "Circle(" << leftBottom_.string() << ", " << radius_ << ")";
  return str.str();
}

std::string BongardSquare::string() const {
  std::ostringstream str;
  str << "Square(" << leftBottom_.string() << ", " << w_ << ")";
  return str.str();
}

std::string BongardTriangle::string() const {
  std::ostringstream str;
  str << "Triangle(" << leftBottom_.string() << ", " << w_ << ")";
  return str.str();
}

}  // namespace bongard
