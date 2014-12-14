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

#include <boost/filesystem.hpp>
#include <cstdio>
#include <iostream>
#include <memory>
#include <random>
#include <string>

#include "BongardGenerator.hpp"
#include "Configurations.hpp"

namespace bongard {

BongardGenerator::BongardGenerator(generator_params p,
                                   const std::string &root_path,
                                   id_type pid,
                                   id_type eid,
                                   int fold_id)
    : fold_id_(fold_id),
      root_path_(root_path),
      pid_(pid),
      eid_(eid),
      p_(p),
      gen_(rd_()) {}

void BongardGenerator::generate(id_type num) {
  std::uniform_int_distribution<> size_dis(p_.min_num_elems_,
                                           p_.max_num_elems_);
  std::uniform_int_distribution<> area_rand(p_.min_size_, p_.max_size_);
  std::uniform_int_distribution<> coord_rand(MARGIN, BOUND - MARGIN);
  std::uniform_int_distribution<> type_rand(1, 4);

  picture_params pp(p_.min_insides_, gen_, coord_rand, type_rand, area_rand);

  std::cout << "To generate " << num << " pictures.\n";

  while (pid_ < num) {
    std::unique_ptr<BongardPicture> picture(new BongardPicture(pp));
    int size = size_dis(gen_);
    int error_try = 0;
    while (!picture->createPicture(size)) {
      error_try++;
      picture.reset(new BongardPicture(pp));
      if (error_try % 10000 == 0) {
        std::cout << "Error trials: " << error_try << std::endl;
      }
    }

    picture->assignIDs(pid_, eid_);
    pictures_.emplace_back(picture.release());

    if (pictures_.size() == CUTOFF) {
      output();
      pictures_.clear();
    }

    if (pid_ % (PRINT_GRAN) == 0) {
      std::cout << "Generated " << pid_ << " pictures.\n" << std::flush;
    }
  }
  if (!pictures_.empty()) output();
}

void BongardGenerator::output() {
  boost::filesystem::path dir = root_path_ / std::to_string(fold_id_++);

  if (!boost::filesystem::create_directory(dir)) {
    std::cout << "The files in the directory " << dir.string()
              << " will be rewritten." << std::endl;
  } else {
    std::cout << "Created directory " << dir.string() << "." << std::endl;
  }

  const std::string elem_rel = "element";
  const std::string circle_rel = "circle";
  const std::string rec_rel = "rectangle";
  const std::string tri_rel = "triangle";
  const std::string tri_down_rel = "triangle_down";
  const std::string tri_up_rel = "triangle_up";
  const std::string east_rel = "east";
  const std::string north_rel = "north";
  const std::string inside = "inside";

  FILE *elem_file = fopen((dir / elem_rel).c_str(), "w");
  FILE *rect_file = fopen((dir / rec_rel).c_str(), "w");
  FILE *circle_file = fopen((dir / circle_rel).c_str(), "w");
  FILE *tri_file = fopen((dir / tri_rel).c_str(), "w");
  FILE *tri_up_file = fopen((dir / tri_up_rel).c_str(), "w");
  FILE *tri_down_file = fopen((dir / tri_down_rel).c_str(), "w");
  FILE *east_file = fopen((dir / east_rel).c_str(), "w");
  FILE *north_file = fopen((dir / north_rel).c_str(), "w");
  FILE *inside_file = fopen((dir / inside).c_str(), "w");

  for (const std::unique_ptr<BongardPicture> &picture : pictures_) {
    picture->output(elem_file, circle_file, rect_file, tri_file, tri_up_file,
                    tri_down_file, inside_file, north_file, east_file);
  }

  FILE *summary = fopen((dir / "summary").c_str(), "w");
  fprintf(summary,
          "#elements: [%d, %d]; #size: [%d, %d]; #min_insides: %d; "
          "max_pid: %llu max_eid: %llu\n",
          p_.min_num_elems_, p_.max_num_elems_, p_.min_size_, p_.max_size_,
          p_.min_insides_, pid_, eid_);

  std::cout << "Output " << pictures_.size() << " pictures." << std::endl;

  fclose(summary);
  fclose(elem_file);
  fclose(rect_file);
  fclose(circle_file);
  fclose(tri_file);
  fclose(tri_up_file);
  fclose(tri_down_file);
  fclose(east_file);
  fclose(north_file);
  fclose(inside_file);
}

}  // namespace bongard
