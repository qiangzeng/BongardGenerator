/**Copyright 2014 UW-Madison, Database Group
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

#ifndef BONGARDGENERATOR_HPP_
#define BONGARDGENERATOR_HPP_

#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <memory>
#include <boost/filesystem.hpp>

#include "BongardPicture.hpp"

namespace bongard {

/**
 * @brief The generator parameters
 */
struct generator_params {
	/**
	 * @brief Constructor
	 *
	 * @param min_num_elems The minimum number of elements (shape objects) in a picture
	 * @param max_num_elems	The maximum number of elements in a picture
	 * @param min_size	The minimum size of an element
	 * @param max_size	The maximum size of an element
	 * @param min_insides	The minimum number of pairs of elements for which one is enclosed in the other
	 */
	generator_params(int min_num_elems, int max_num_elems, int min_size,
			int max_size, int min_insides) :
			min_num_elems_(min_num_elems), max_num_elems_(max_num_elems), min_size_(
					min_size), max_size_(max_size), min_insides_(min_insides) {

	}
	const int min_num_elems_;
	const int max_num_elems_;
	const int min_size_;
	const int max_size_;
	const int min_insides_;
};

/**
 * @brief A data generator for the Bongard problem.
 **/
class BongardGenerator {
public:
	/**
	 * @brief Constructor
	 *
	 * @param p 	 generator parameters
	 * @param root_path 	the output root path
	 * @param pid	the beginning picture id
	 * @param eid	the beginning picutre element id
	 * @param fold_id	the beginning fold id
	 */
	BongardGenerator(generator_params p, std::string root_path, id_type pid = 0,
			id_type eid = 0, int fold_id = 0);

	/**
	 * @brief Randomly generate pictures
	 * @details The size (number of elements) of each picture, and the size and the location of each picture element are
	 * uniformly generated from the ranges specified in the generator parameters. The shape of an element is uniformly
	 * chosen in circles, point-up/point-down triangles and squares. Each picture cannot have two elements that are
	 * partially overlapped and every element should be enclosed in the bounding box of the picture.
	 *
	 * @param num the number of picures to be generated
	 */
	void generate(id_type num);
private:
	void output();

	int fold_id_;
	boost::filesystem::path root_path_;
	id_type pid_;
	id_type eid_;

	std::vector<std::unique_ptr<BongardPicture> > pictures_;

	generator_params p_;
	std::random_device rd_;
	std::mt19937 gen_;
};

} /* namespace bongard */

#endif /* BONGARDGENERATOR_HPP_ */
