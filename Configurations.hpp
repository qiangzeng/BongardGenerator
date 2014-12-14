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

#ifndef CONFIGURATIONS_HPP_
#define CONFIGURATIONS_HPP_

/**
 * @typedef the id type for pictures and elements
 */
typedef long long unsigned int id_type;

/** The chunk size of pictures stored in one fold */
#define CUTOFF 1000000

/** Print the progress whenever a multiplier number of picture is generated. */
#define PRINT_GRAN 5000

/** The length and the width of the picture. */
#define BOUND 100

/** The minimum horizontal/vertical separate space between two elements and
 * between the element and the picture boundary.
 */
#define MARGIN 2

/** The maximum number of trials in generating an element. */
#define MAX_TRY 100000000

#endif /* CONFIGURATIONS_HPP_ */
