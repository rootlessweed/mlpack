/**
 * @file averge_init.hpp
 * @author Sumedh Ghaisas
 *
 * Intialization rule for Alternating Matrix Factorization.
 *
 * This file is part of MLPACK 1.0.11.
 *
 * MLPACK is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * MLPACK is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details (LICENSE.txt).
 *
 * You should have received a copy of the GNU General Public License along with
 * MLPACK.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __MLPACK_METHODS_AMF_AVERAGE_INIT_HPP
#define __MLPACK_METHODS_AMF_AVERAGE_INIT_HPP

#include <mlpack/core.hpp>

namespace mlpack {
namespace amf {

/**
 * This initialization rule initializes matrix W and H to root of average of V
 * with uniform noise. Uniform noise is generated by Armadillo's 'randu' function.
 * To have a better effect lower bound of the matrix is subtracted from average
 * before dividing it by the factorization rank. This computed value is added
 * with the random noise.
 */
class AverageInitialization
{
 public:
  // Empty constructor required for the InitializeRule template
  AverageInitialization() { }

  template<typename MatType>
  inline static void Initialize(const MatType& V,
                                const size_t r,
                                arma::mat& W,
                                arma::mat& H)
  {
    size_t n = V.n_rows;
    size_t m = V.n_cols;

    double V_avg = 0;
    size_t count = 0;
    double min = DBL_MAX;
    for(typename MatType::const_row_col_iterator it = V.begin();it != V.end();it++)
    {
      if(*it != 0)
      {
        count++;
        V_avg += *it;
        if(*it < min) min = *it;
      }
    }
    V_avg = sqrt(((V_avg / (n * m)) - min) / r);

    // Intialize to random values.
    W.randu(n, r);
    H.randu(r, m);

    W = W + V_avg;
    H = H + V_avg;
  }
};

}; // namespace amf
}; // namespace mlpack

#endif