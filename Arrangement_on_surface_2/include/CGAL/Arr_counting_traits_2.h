// Copyright (c) 2005  Tel-Aviv University (Israel).
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
// $URL$
// $Id$
//
// Author(s)     : Efi Fogel    <efif@post.tau.ac.il>

#ifndef CGAL_ARR_COUNTING_TRAITS_H
#define CGAL_ARR_COUNTING_TRAITS_H

/*! \file
 * A counting traits-class for the arrangement package.
 * This is a meta-traits class. It is parameterized with another traits class
 * and inherits from it. For each traits method it maintains a counter that
 * counts the number of invokations into the method.
 */

#include <iostream>
#include <string.h>

#include <CGAL/basic.h>
#include <CGAL/Arr_enums.h>

CGAL_BEGIN_NAMESPACE

/*! \class 
 * A model of the ArrangementTraits_2 concept that counts the methods invoked.
 */
template <class Base_traits>
class Arr_counting_traits_2 : public Base_traits {
public:
  enum Operation_id {
    COMPARE_X_OP = 0,
    COMPARE_XY_OP,
    CONSTRUCT_MIN_VERTEX_OP,
    CONSTRUCT_MAX_VERTEX_OP,
    IS_VERTICAL_OP,
    COMPARE_Y_AT_X_OP,
    EQUAL_POINTS_OP,
    EQUAL_CURVES_OP,
    COMPARE_Y_AT_X_LEFT_OP,
    COMPARE_Y_AT_X_RIGHT_OP,
    MAKE_X_MONOTONE_OP,
    SPLIT_OP,
    INTERSECT_OP,
    ARE_MERGEABLE_OP,
    MERGE_OP,
    CONSTRUCT_OPPOSITE_OP,
    COMPARE_ENDPOINTS_XY_OP,
    BOUNDARY_IN_X_OP,
    BOUNDARY_IN_Y_OP,
    COMPARE_X_NEAR_BOUNDARY_OP,
    COMPARE_Y_NEAR_BOUNDARY_OP,
    COMPARE_X_ON_IDENTIFICATION_OP,
    COMPARE_Y_ON_IDENTIFICATION_OP,
    NUMBER_OF_OPERATIONS
  };

  typedef Base_traits                           Base;
  typedef Arr_counting_traits_2<Base>           Self;

  /*! Construct default */
  Arr_counting_traits_2() : Base()
  {
    clear_counters();
    increment();
  }

  /*! Construct copy */
  Arr_counting_traits_2(Arr_counting_traits_2 & other) : Base(other)
  {
    clear_counters();
    increment();
  }
  
  /*! Obtain the counter of the given operation */
  unsigned int count(Operation_id id) const
  { return m_counters[id]; }

  unsigned int count_compare_x() const
  { return m_counters[COMPARE_X_OP]; }
  
  unsigned int count_compare_xy() const
  { return m_counters[COMPARE_XY_OP]; }

  unsigned int count_construct_min_vertex() const
  { return m_counters[CONSTRUCT_MIN_VERTEX_OP]; }
  
  unsigned int count_construct_max_vertex() const
  { return m_counters[CONSTRUCT_MAX_VERTEX_OP]; }

  unsigned int count_is_vertical() const
  { return m_counters[IS_VERTICAL_OP]; }
  
  unsigned int count_compare_y_at_x() const
  { return m_counters[COMPARE_Y_AT_X_OP]; }
  
  unsigned int count_equal_points() const
  { return m_counters[EQUAL_POINTS_OP]; }
  
  unsigned int count_equal_curves() const
  { return m_counters[EQUAL_CURVES_OP]; }
  
  unsigned int count_compare_y_at_x_left() const
  { return m_counters[COMPARE_Y_AT_X_LEFT_OP]; }
  
  unsigned int count_compare_y_at_x_right() const
  { return m_counters[COMPARE_Y_AT_X_RIGHT_OP]; }
  
  unsigned int count_make_x_monotone() const
  { return m_counters[MAKE_X_MONOTONE_OP]; }
  
  unsigned int count_split() const
  { return m_counters[SPLIT_OP]; }
  
  unsigned int count_intersect() const
  { return m_counters[INTERSECT_OP]; }
  
  unsigned int count_are_mergeable() const
  { return m_counters[ARE_MERGEABLE_OP]; }
  
  unsigned int count_merge() const
  { return m_counters[MERGE_OP]; }
  
  unsigned int count_construct_opposite() const
  { return m_counters[CONSTRUCT_OPPOSITE_OP]; }
  
  unsigned int count_compare_endpoints_xy() const
  { return m_counters[COMPARE_ENDPOINTS_XY_OP]; }
  
  unsigned int count_parameter_space_in_x() const
  { return m_counters[BOUNDARY_IN_X_OP]; }
  
  unsigned int count_parameter_space_in_y() const
  { return m_counters[BOUNDARY_IN_Y_OP]; }

  unsigned int count_compare_x_near_boundary() const
  { return m_counters[COMPARE_X_NEAR_BOUNDARY_OP]; }

  unsigned int count_compare_y_near_boundary() const
  { return m_counters[COMPARE_Y_NEAR_BOUNDARY_OP]; }

  unsigned int count_compare_x_on_identification() const
  { return m_counters[COMPARE_X_ON_IDENTIFICATION_OP]; }

  unsigned int count_compare_y_on_identification() const
  { return m_counters[COMPARE_Y_ON_IDENTIFICATION_OP]; }
  
  /// \name Types and functors inherited from the base
  //@{

  // Traits types:
  typedef typename Base::Has_left_category      Has_left_category;
  typedef typename Base::Has_merge_category     Has_merge_category;
  typedef typename Base::Has_boundary_category  Has_boundary_category;
  typedef typename Base::Boundary_category      Boundary_category;
  
  typedef typename Base::Point_2                Point_2;
  typedef typename Base::X_monotone_curve_2     X_monotone_curve_2;
  typedef typename Base::Curve_2                Curve_2;

  /*! A functor that compares the x-coordinates of two points */
  class Compare_x_2 {
  private:
    typename Base::Compare_x_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_x_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_x_2_object()), m_counter(counter) {}

    /*! Operate */
    Comparison_result operator()(const Point_2 & p1, const Point_2 & p2) const
    { ++m_counter; return m_object(p1, p2); }
  };

  /*! A functor that compares two points lexigoraphically: by x, then by y. */
  class Compare_xy_2 {
  private:
    typename Base::Compare_xy_2 m_object;
    mutable unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_xy_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_xy_2_object()), m_counter(counter) {}

    /*! Operate */
    Comparison_result operator()(const Point_2 & p1, const Point_2 & p2) const
    { ++m_counter; return m_object(p1, p2); }
  };

  /*! A functor that obtains the left endpoint of an x-monotone curve. */
  class Construct_min_vertex_2 {
  private:
    typename Base::Construct_min_vertex_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Construct_min_vertex_2(const Base * base, unsigned int & counter) :
      m_object(base->construct_min_vertex_2_object()), m_counter(counter) {}

    /*! Operate */
    const Point_2 operator()(const X_monotone_curve_2 & xc) const
    { ++m_counter; return m_object(xc); }
  };

  /*! A functor that obtains the right endpoint of an x-monotone curve. */
  class Construct_max_vertex_2 {
  private:
    typename Base::Construct_max_vertex_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Construct_max_vertex_2(const Base * base, unsigned int & counter) :
      m_object(base->construct_max_vertex_2_object()), m_counter(counter) {}

    /*! Operate */
    const Point_2 operator()(const X_monotone_curve_2 & xc) const
    { ++m_counter; return m_object(xc); }
  };
  
  /*! A functor that checks whether a given x-monotone curve is vertical. */
  class Is_vertical_2 {
  private:
    typename Base::Is_vertical_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Is_vertical_2(const Base * base, unsigned int & counter) :
      m_object(base->is_vertical_2_object()), m_counter(counter) {}

    /*! Operate */
    bool operator()(const X_monotone_curve_2 & xc) const
    { ++m_counter; return m_object(xc); }
  };
  
  /*! A functor that compares the y-coordinates of a point and an
   * x-monotone curve at the point x-coordinate.
   */
  class Compare_y_at_x_2 {
  private:
    typename Base::Compare_y_at_x_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_y_at_x_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_y_at_x_2_object()), m_counter(counter) {}

    /*! Operate */
    Comparison_result operator()(const Point_2 & p,
                                 const X_monotone_curve_2 & xc) const
    { ++m_counter; return m_object(p, xc); }
  };
  
  /*! A functor that checks whether two points and two x-monotone curves are
   * identical.
   */
  class Equal_2 {
  private:
    typename Base::Equal_2 m_object;
    unsigned int & m_counter1;
    unsigned int & m_counter2;

  public:
    /*! Construct */
    Equal_2(const Base * base,
            unsigned int & counter1, unsigned int & counter2) :
      m_object(base->equal_2_object()),
      m_counter1(counter1), m_counter2(counter2)
    {}

    /*! Operate */
    bool operator()(const X_monotone_curve_2 & xc1,
                    const X_monotone_curve_2 & xc2) const
    { ++m_counter1; return m_object(xc1, xc2); }

    /*! Operate */
    bool operator()(const Point_2 & p1, const Point_2 & p2) const
    { ++m_counter2; return m_object(p1, p2); }    
  };

  /*! A functor that compares compares the y-coordinates of two x-monotone
   * curves immediately to the left of their intersection point.
   */
  class Compare_y_at_x_left_2 {
  private:
    typename Base::Compare_y_at_x_left_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_y_at_x_left_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_y_at_x_left_2_object()), m_counter(counter) {}

    /*! Operate */
    Comparison_result operator()(const X_monotone_curve_2 & xc1,
                                 const X_monotone_curve_2 & xc2,
                                 const Point_2 & p) const
    { ++m_counter; return m_object(xc1, xc2, p); }
  };

  /*! A functor that compares compares the y-coordinates of two x-monotone
   * curves immediately to the right of their intersection point.
   */
  class Compare_y_at_x_right_2 {
  private:
    typename Base::Compare_y_at_x_right_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_y_at_x_right_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_y_at_x_right_2_object()), m_counter(counter) {}

    /*! Operate */
    Comparison_result operator()(const X_monotone_curve_2 & xc1,
                                 const X_monotone_curve_2 & xc2,
                                 const Point_2 & p) const
    { ++m_counter; return m_object(xc1, xc2, p); }
  };
  
  /*! A functor that divides a curve into x-monotone curves. */
  class Make_x_monotone_2 {
  private:
    typename Base::Make_x_monotone_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Make_x_monotone_2(Base * base, unsigned int & counter) :
      m_object(base->make_x_monotone_2_object()), m_counter(counter) {}

    /*! Operate */
    template<class OutputIterator>
    OutputIterator operator()(const Curve_2 & cv, OutputIterator oi)
    { ++m_counter; return m_object(cv, oi); }
  };

  /*! A functor that splits an arc at a point. */
  class Split_2 {
  private:
    typename Base::Split_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Split_2(Base * base, unsigned int & counter) :
      m_object(base->split_2_object()), m_counter(counter) {}

    /*! Operate */
    void operator()(const X_monotone_curve_2 & xc, const Point_2 & p,
                    X_monotone_curve_2 & xc1, X_monotone_curve_2 & xc2)
    { ++m_counter; m_object(xc, p, xc1, xc2); }
  };

  /*! A functor that computes intersections between x-monotone curves. */
  class Intersect_2 {
  private:
    typename Base::Intersect_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Intersect_2(Base * base, unsigned int & counter) :
      m_object(base->intersect_2_object()), m_counter(counter) {}

    /*! Operate */
    template<class OutputIterator>
    OutputIterator operator()(const X_monotone_curve_2 & xc1,
                              const X_monotone_curve_2 & xc2,
                              OutputIterator oi)
    { ++m_counter; return m_object(xc1, xc2, oi); }
  };

  /*! A functor that tests whether two x-monotone curves can be merged. */
  class Are_mergeable_2 {
  private:
    typename Base::Are_mergeable_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Are_mergeable_2(const Base * base, unsigned int & counter) :
      m_object(base->are_mergeable_2_object()), m_counter(counter) {}

    /*! Operate */
    bool operator()(const X_monotone_curve_2 & xc1,
                    const X_monotone_curve_2 & xc2) const
    { ++m_counter; return m_object(xc1, xc2); }
  };

  /*! A functor that merges two x-monotone curves into one. */
  class Merge_2 {
  private:
    typename Base::Merge_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Merge_2(Base * base, unsigned int & counter) :
      m_object(base->merge_2_object()), m_counter(counter) {}

    /*! Operate */
    void operator()(const X_monotone_curve_2 & xc1,
                    const X_monotone_curve_2 & xc2,
                    X_monotone_curve_2 & xc)
    { ++m_counter; m_object(xc1, xc2, xc); }
  };

  /*! A fnuctor that constructs an opposite x-monotone curve. */
  class Construct_opposite_2 {
  private:
    typename Base::Construct_opposite_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Construct_opposite_2(const Base * base, unsigned int & counter) :
      m_object(base->construct_opposite_2_object()), m_counter(counter) {}

    /*! Operate */
    X_monotone_curve_2 operator()(const X_monotone_curve_2 & xc)
    { ++m_counter; return m_object(xc); }
  };

  /*! A functor that compares the two endpoints of an x-monotone curve
   * lexigoraphically.
   */
  class Compare_endpoints_xy_2 {
  private:
    typename Base::Compare_endpoints_xy_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_endpoints_xy_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_endpoints_xy_2_object()), m_counter(counter) {}

    /*! Operate */
    Comparison_result operator()(const X_monotone_curve_2 & xc)
    { ++m_counter; return m_object(xc); }
  };

  /*! A functor that determines whether an endpoint of an x-monotone curve lies
   * on a boundary of the parameter space along the x axis.
   */
  class Parameter_space_in_x_2 {
  private:
    typename Base::Parameter_space_in_x_2 m_object;
    mutable unsigned int & m_counter;

  public:
    /*! Construct */
    Parameter_space_in_x_2(const Base * base, unsigned int & counter) :
      m_object(base->parameter_space_in_x_2_object()), m_counter(counter) {}

    /*! Operate */
    Arr_parameter_space operator()(const X_monotone_curve_2 & xc,
                             Arr_curve_end ce) const
    { ++m_counter; return m_object(xc, ce); }
  };

  /*! A functor that determines whether an endpoint of an x-monotone arc lies
   * on a boundary of the parameter space along the y axis.
   */
  class Parameter_space_in_y_2 {
  private:
    typename Base::Parameter_space_in_y_2 m_object;
    mutable unsigned int & m_counter;

  public:
    /*! Construct */
    Parameter_space_in_y_2(const Base * base, unsigned int & counter) :
      m_object(base->parameter_space_in_y_2_object()), m_counter(counter) {}

    /*! Operate */
    Arr_parameter_space operator()(const X_monotone_curve_2 & xc,
                             Arr_curve_end ce) const
    { ++m_counter; return m_object(xc, ce); }

    /*! Operate */
    Arr_parameter_space operator()(const Point_2 & p) const
    { ++m_counter; return m_object(p); }
  };

  /*! A functor that compares the x-coordinates of curve ends near the
   * boundary of the parameter space.
   */
  class Compare_x_near_boundary_2 {
  private:
    typename Base::Compare_x_near_boundary_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_x_near_boundary_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_x_near_boundary_2_object()), m_counter(counter) {}

    /*! Operate */
    Comparison_result operator()(const Point_2 & p,
                                 const X_monotone_curve_2 & xc,
                                 Arr_curve_end ce) const
    { ++m_counter; return m_object(p, xc, ce); }

    /*! Operate */
    Comparison_result operator()(const X_monotone_curve_2 & xc1,
                                 Arr_curve_end ce1,
                                 const X_monotone_curve_2 & xc2,
                                 Arr_curve_end ce2) const
    { ++m_counter; return m_object(xc1, ce1, xc2, ce2); }
  };

  /*! A functor that compares the y-coordinates of curve ends near the
   * boundary of the parameter space.
   */
  class Compare_y_near_boundary_2 {
  private:
    typename Base::Compare_y_near_boundary_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_y_near_boundary_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_y_near_boundary_2_object()), m_counter(counter) {}

    /*! Operate */
    Comparison_result operator()(const X_monotone_curve_2 & xc1,
                                 const X_monotone_curve_2 & xc2, 
                                 Arr_curve_end ce) const
    { ++m_counter; return m_object(xc1, xc2, ce); }
  };

  /*! A functor that compares the x-coordinate of two given points
   * that lie on the horizontal identification curve.
   */
  class Compare_x_on_identification_2 {
  private:
    typename Base::Compare_x_on_identification_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_x_on_identification_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_x_on_identification_2_object()),
      m_counter(counter)
    {}

    /*! Operate */
    Comparison_result operator()(const Point_2 & p1,
                                 const Point_2 & p2) const
    { ++m_counter; return m_object(p1, p2); }
  };

  /*! A functor that compares the y-coordinate of two given points
   * that lie on the vertical identification curve.
   */
  class Compare_y_on_identification_2 {
  private:
    typename Base::Compare_y_on_identification_2 m_object;
    unsigned int & m_counter;

  public:
    /*! Construct */
    Compare_y_on_identification_2(const Base * base, unsigned int & counter) :
      m_object(base->compare_y_on_identification_2_object()),
      m_counter(counter)
    {}

    /*! Operate */
    Comparison_result operator()(const Point_2 & p1,
                                 const Point_2 & p2) const
    { ++m_counter; return m_object(p1, p2); }
  };
  //@}

  /// \name Obtain the appropriate functor
  //@{
  Compare_x_2 compare_x_2_object() const
  { return Compare_x_2(this, m_counters[COMPARE_X_OP]); }
  
  Compare_xy_2 compare_xy_2_object() const
  { return Compare_xy_2(this, m_counters[COMPARE_XY_OP]); }

  Construct_min_vertex_2 construct_min_vertex_2_object() const
  { return Construct_min_vertex_2(this, m_counters[CONSTRUCT_MIN_VERTEX_OP]); }

  Construct_max_vertex_2 construct_max_vertex_2_object() const
  { return Construct_max_vertex_2(this, m_counters[CONSTRUCT_MAX_VERTEX_OP]); }
  
  Parameter_space_in_x_2 parameter_space_in_x_2_object() const
  { return Parameter_space_in_x_2(this, m_counters[BOUNDARY_IN_X_OP]); }

  Parameter_space_in_y_2 parameter_space_in_y_2_object() const
  { return Parameter_space_in_y_2(this, m_counters[BOUNDARY_IN_Y_OP]); }  

  Is_vertical_2 is_vertical_2_object() const
  { return Is_vertical_2(this, m_counters[IS_VERTICAL_OP]); }
  
  Compare_y_at_x_2 compare_y_at_x_2_object() const
  { return Compare_y_at_x_2(this, m_counters[COMPARE_Y_AT_X_OP]); }
  
  Equal_2 equal_2_object() const
  {
    return Equal_2(this, m_counters[EQUAL_POINTS_OP],
                   m_counters[EQUAL_CURVES_OP]);
  }

  Compare_y_at_x_left_2 compare_y_at_x_left_2_object() const
  { return Compare_y_at_x_left_2(this, m_counters[COMPARE_Y_AT_X_LEFT_OP]); }

  Compare_y_at_x_right_2 compare_y_at_x_right_2_object() const
  { return Compare_y_at_x_right_2(this, m_counters[COMPARE_Y_AT_X_RIGHT_OP]); }
  
  Make_x_monotone_2 make_x_monotone_2_object()
  { return Make_x_monotone_2(this, m_counters[MAKE_X_MONOTONE_OP]); }

  Split_2 split_2_object()
  { return Split_2(this, m_counters[SPLIT_OP]); }

  Intersect_2 intersect_2_object()
  { return Intersect_2(this, m_counters[INTERSECT_OP]); }

  Are_mergeable_2 are_mergeable_2_object() const
  { return Are_mergeable_2(this, m_counters[ARE_MERGEABLE_OP]); }

  Merge_2 merge_2_object()
  { return Merge_2(this, m_counters[MERGE_OP]); }

  Construct_opposite_2 construct_opposite_2_object() const
  { return Construct_opposite_2(this, m_counters[CONSTRUCT_OPPOSITE_OP]); }

  Compare_endpoints_xy_2 compare_endpoints_xy_2_object() const
  { return Compare_endpoints_xy_2(this, m_counters[COMPARE_ENDPOINTS_XY_OP]); }

  //@}

  /*! Increment the construction counter
   * \param doit indicates whethet to actually inceremnt the counter or not
   * \return the counter at the end of the operation
   */
  static unsigned int increment(bool doit = true)
  {
    static unsigned int counter = 0;
    if (doit) ++counter;
    return counter;
  }

  /*! Clean all operation counters */
  void clear_counters()
  { memset(m_counters, 0, sizeof(m_counters)); }

private:
  /*! The operation counters */
  mutable unsigned int m_counters[NUMBER_OF_OPERATIONS];
};

template <class Out_stream, class Base_traits>
inline
Out_stream & operator<<(Out_stream & os,
                        const Arr_counting_traits_2<Base_traits> & traits)
{
  typedef Arr_counting_traits_2<Base_traits>            Traits;
  unsigned int sum = 0;
  unsigned int i;
  for (i = 0; i < Traits::NUMBER_OF_OPERATIONS; ++i)
    sum += traits.count(static_cast<typename Traits::Operation_id>(i));
  os << "# of COMPARE_X operation = "
     << traits.count_compare_x() << std::endl
     << "# of COMPARE_XY operation = "
     << traits.count_compare_xy() << std::endl
     << "# of CONSTRUCT_MIN_VERTEX operation = "
     << traits.count_construct_min_vertex() << std::endl
     << "# of CONSTRUCT_MAX_VERTEX operation = "
     << traits.count_construct_max_vertex() << std::endl
     << "# of IS_VERTICAL operation = "
     << traits.count_is_vertical() << std::endl
     << "# of COMPARE_Y_AT_X operation = "
     << traits.count_compare_y_at_x() << std::endl
     << "# of EQUAL_POINTS operation = "
     << traits.count_equal_points() << std::endl
     << "# of EQUAL_CURVES operation = "
     << traits.count_equal_curves() << std::endl
     << "# of COMPARE_Y_AT_X_LEFT operation = "
     << traits.count_compare_y_at_x_left() << std::endl
     << "# of COMPARE_Y_AT_X_RIGHT operation = "
     << traits.count_compare_y_at_x_right() << std::endl
     << "# of MAKE_X_MONOTONE operation = "
     << traits.count_make_x_monotone() << std::endl
     << "# of SPLIT operation = "
     << traits.count_split() << std::endl
     << "# of INTERSECT operation = "
     << traits.count_intersect() << std::endl
     << "# of ARE_MERGEABLE operation = "
     << traits.count_are_mergeable() << std::endl
     << "# of MERGE operation = "
     << traits.count_merge() << std::endl
     << "# of CONSTRUCT_OPPOSITE operation = "
     << traits.count_construct_opposite() << std::endl
     << "# of COMPARE_ENDPOINTS_XY operation = "
     << traits.count_compare_endpoints_xy() << std::endl
     << "# of BOUNDARY_IN_X operation = "
     << traits.count_parameter_space_in_x() << std::endl
     << "# of BOUNDARY_IN_Y operation = "
     << traits.count_parameter_space_in_y() << std::endl
     << "# of COMPARE_X_NEAR_BOUNDARY operation = "
     << traits.count_compare_x_near_boundary() << std::endl
     << "# of COMPARE_Y_NEAR_BOUNDARY operation = "
     << traits.count_compare_y_near_boundary() << std::endl
     << "# of COMPARE_X_ON_IDENTIFICATION operation = "
     << traits.count_compare_x_on_identification() << std::endl
     << "# of COMPARE_Y_ON_IDENTIFICATION operation = "
     << traits.count_compare_y_on_identification() << std::endl
     << "total # = " << sum << std::endl
     << "# of traits constructed = " << Traits::increment(false)
     << std::endl;
  return os;
}

CGAL_END_NAMESPACE

#endif
