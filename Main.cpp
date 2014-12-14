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
#include <climits>
#include <cstdlib>
#include <iostream>

#include "BongardGenerator.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr
        << "Usage: bongard_generator NUM_PCITURES DIRECTORY\n"
           "Create NUM_PICTURES Bongard pictures in the directory DIRECTORY\n";
    return 0;
  }

  boost::filesystem::path root(argv[2]);
  if (!boost::filesystem::exists(root)) {
    std::cerr << "Error: The path " << argv[2] << " does not exists.\n";
    return 0;
  }
  if (!boost::filesystem::is_directory(root)) {
    std::cerr << "Error: " << argv[2] << " is not a directory.\n";
    return 0;
  }

  char *pend;
  unsigned long long int num_pics = std::strtoull(argv[1], &pend, 10);
  if (num_pics == 0ULL || num_pics == ULLONG_MAX) {
    std::cerr << "Error: " << argv[1] << " is not a valid input number.\n";
    return 0;
  }

  /**
   * Change the other five parameters to custom the generator.
   */
  int min_num_elements = 4;
  int max_num_elements = 6;
  int min_length = 2;
  int max_length = 98;
  int min_encolse_pairs = 1;

  bongard::generator_params param(min_num_elements,
                                  max_num_elements,
                                  min_length,
                                  max_length,
                                  min_encolse_pairs);

  bongard::BongardGenerator gen(param, root.string());
  gen.generate(num_pics);

  return 0;
}
