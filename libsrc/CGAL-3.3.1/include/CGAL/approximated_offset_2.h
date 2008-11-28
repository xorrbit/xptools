// Copyright (c) 2006  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal/branches/CGAL-3.3-branch/Minkowski_sum_2/include/CGAL/approximated_offset_2.h $
// $Id$
//
// Author(s)     : Ron Wein   <wein@post.tau.ac.il>

#ifndef CGAL_APPROXIMATED_OFFSET_H
#define CGAL_APPROXIMATED_OFFSET_H

#include <CGAL/Minkowski_sum_2/Approx_offset_base_2.h>
#include <CGAL/Minkowski_sum_2/Offset_conv_2.h>
#include <CGAL/Minkowski_sum_2/Offset_decomp_2.h>

CGAL_BEGIN_NAMESPACE

/*!
 * Approximate the offset of a given simple polygon by a given radius,
 * using the convolution method.
 * Note that as the input polygon may not be convex, its offset may not be 
 * simply connected. The result is therefore represented as a polygon with
 * holes.
 * \param pgn The polygon.
 * \param r The offset radius.
 * \param eps The approximation-error bound.
 * \return The approximated offset polygon.
 */
template <class Kernel, class Container>
typename Gps_circle_segment_traits_2<Kernel>::Polygon_with_holes_2
approximated_offset_2 (const Polygon_2<Kernel, Container>& pgn,
                       const typename Kernel::FT& r,
                       const double& eps)
{
  typedef Approx_offset_base_2<Kernel, Container>            Base;
  typedef Offset_by_convolution_2<Base>                      Approx_offset_2;
  typedef typename Approx_offset_2::Offset_polygon_2         Offset_polygon_2;

  Base                                               base (eps);
  Approx_offset_2                                    approx_offset (base);
  Offset_polygon_2                                   offset_bound;
  std::list<Offset_polygon_2>                        offset_holes;

  approx_offset (pgn, r,
                 offset_bound, std::back_inserter(offset_holes));

  return (typename Gps_circle_segment_traits_2<Kernel>::Polygon_with_holes_2
          (offset_bound, offset_holes.begin(), offset_holes.end()));
}

/*!
 * Approximate the offset of a given simple polygon by a given radius,
 * by decomposing it to convex sub-polygons and computing the union of their
 * offsets.
 * Note that as the input polygon may not be convex, its offset may not be 
 * simply connected. The result is therefore represented as a polygon with
 * holes.
 * \param pgn The polygon.
 * \param r The offset radius.
 * \param eps The approximation-error bound.
 * \param decomp A functor for decomposing polygons.
 * \return The approximated offset polygon.
 */
template <class Kernel, class Container, class DecompositionStrategy>
typename Gps_circle_segment_traits_2<Kernel>::Polygon_with_holes_2
approximated_offset_2 (const Polygon_2<Kernel, Container>& pgn,
                       const typename Kernel::FT& r,
                       const double& eps,
                       const DecompositionStrategy&)
{
  typedef Approx_offset_base_2<Kernel, Container>            Base;
  typedef Offset_by_decomposition_2<Base, DecompositionStrategy>
                                                             Approx_offset_2;
  typedef typename Approx_offset_2::Offset_polygon_2         Offset_polygon_2;

  Base                                               base (eps);
  Approx_offset_2                                    approx_offset (base);
  Offset_polygon_2                                   offset_bound;
  std::list<Offset_polygon_2>                        offset_holes;

  approx_offset (pgn, r,
                 offset_bound, std::back_inserter(offset_holes));

  return (typename Gps_circle_segment_traits_2<Kernel>::Polygon_with_holes_2
          (offset_bound, offset_holes.begin(), offset_holes.end()));
}

CGAL_END_NAMESPACE

#endif