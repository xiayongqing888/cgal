// Copyright (c) 1997  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Andreas Fabri, Ron Wein, Julien Hazebrouck


// Descriptions of the file format can be found at
// http://www.autodesk.com/techpubs/autocad/acad2000/dxf/
// http://www.tnt.uni-hannover.de/soft/compgraph/fileformats/docs/DXF.ascii
// 
// Descriptions of the file format can be found at
// http://www.autodesk.com/techpubs/autocad/acad2000/dxf/
// http://www.tnt.uni-hannover.de/soft/compgraph/fileformats/docs/DXF.ascii

#ifndef CGAL_IO_DXF_VARIANT_READER_H
#define CGAL_IO_DXF_VARIANT_READER_H

#include <CGAL/IO/Dxf_reader.h>
#include <iostream>
#include <string>
#include <list>
#include <boost/variant.hpp>



namespace CGAL {


template<class CK,class Circular_arc_2, class Line_arc_2, class OutputIterator>
  OutputIterator variant_load(std::istream& is, OutputIterator res)
{
  typedef typename CK::FT FT;
  typedef typename CK::Circular_arc_point_2 Circular_arc_point_2;    
  typedef typename CK::Root_for_circles_2_2 Root_for_circles_2_2;
  typedef typename CK::Line_2  Line_2;
  typedef typename CK::Point_2 Point_2;
  typedef typename CK::Circle_2 Circle_2;
  typedef typename boost::variant< Circular_arc_2, Line_arc_2 >        Arc;
  typedef std::list<std::pair<Point_2, double> > Polygon;
  typedef std::list<Polygon> Polygons;
  typedef std::list<Circle_2> Circles;

  Polygons polygons;
  Circles circles;
  CGAL::Dxf_reader<CK> reader;
  
  reader(is, polygons, circles);

  std::cout << "Read " << polygons.size() << " polygons, and " 
	    << circles.size() << " circles" << std::endl;
  
  for(typename Circles::iterator it = circles.begin(); it != circles.end(); it++){
    Arc arc = *it;
    *res++ = arc;
  }
  
  Point_2 first_point;
  Point_2 ps;
  Point_2 pt ;
  Point_2 center;
  FT bulge;
  for(typename Polygons::iterator it = polygons.begin(); it != polygons.end(); it++){
    typename Polygon::iterator pit = it->begin();
    first_point = pit->first;
    ps = pit->first;
    bulge = pit->second;
    pit++;
    do{
      //std::cerr << "bulge = " << to_double(bulge) << std::endl;
      if(bulge == FT(0)){
	Arc arc = Line_arc_2(ps, pit->first);

	if(ps != pit->first){     
	  //std::cerr << "Line_arc_2 " << std::endl;
	  // std::cout << arc << std::endl;
	  *res++ = arc;
	}
	ps = pit->first;
	bulge = pit->second;
      } else {
	pt = pit->first;
        bulge = (pit->second);
        const FT sqr_bulge = CGAL::square(bulge);
        const FT common = (FT(1) - sqr_bulge) / (FT(4)*bulge);
        const FT x_coord = ((ps.x() + pt.x())/FT(2)) + common*(ps.y() - pt.y());
        const FT y_coord = ((ps.y() + pt.y())/FT(2)) + common*(pt.x() - ps.x());

        const FT sqr_rad = CGAL::squared_distance(ps, pt) * (FT(1)/sqr_bulge + FT(2) + sqr_bulge) / FT(16); 

	Root_for_circles_2_2 rootxy = Root_for_circles_2_2(ps.x(), ps.y());
	Circular_arc_point_2 cps = Circular_arc_point_2(rootxy);
	Circular_arc_point_2 cpt = Circular_arc_point_2(rootxy);
	Circular_arc_2 arc = Circular_arc_2(Circle_2(Point_2(x_coord, y_coord), sqr_rad), cps, cpt);
	//std::cerr << "arc with center: "  << to_double( x_coord) << "  " << to_double(y_coord) << " and radius " << sqrt(to_double(sqr_rad)) << std::endl;
	//std::cerr << "source: " << to_double(ps.x()) << ", " << to_double(ps.y()) << " target: " << to_double(pt.x()) << ", " << to_double(pt.y()) << std::endl << std::endl;

	//std::cerr << "arc with center: "  << x_coord << "  " <<y_coord << " and radius " << sqrt(to_double(sqr_rad)) << std::endl;
	//std::cerr << "source: " << ps.x() << ", " << ps.y() << " target: " << pt.x() << ", " << pt.y() << std::endl << std::endl;
	*res++ = arc;
      }
      ps = pt;
      bulge = pit->second;
      pit++;
    }while(pit != it->end());

    if(bulge == FT(0)){
      if(ps != first_point){
	Arc arc = Line_arc_2(ps, first_point);      
	//std::cerr << "Line_arc_2" << std::endl;
	*res++ = arc;
      }
    } else {
      pt = first_point;
      const FT sqr_bulge = CGAL::square(bulge);
      const FT common = (FT(1) - sqr_bulge) / (FT(4)*bulge);
      const FT x_coord = ((ps.x() + pt.x())/FT(2)) + common*(ps.y() - pt.y());
      const FT y_coord = ((ps.y() + pt.y())/FT(2)) + common*(pt.x() - ps.x());
      
      const FT sqr_rad = CGAL::squared_distance(ps, pt) * (FT(1)/sqr_bulge + FT(2) + sqr_bulge) / FT(16); 
      
	Root_for_circles_2_2 rootxy =  Root_for_circles_2_2(ps.x(), ps.y());
	Circular_arc_point_2 cps = Circular_arc_point_2(rootxy);
	Circular_arc_point_2 cpt = Circular_arc_point_2(rootxy);
      Circular_arc_2 arc = Circular_arc_2(Circle_2(Point_2(x_coord, y_coord), sqr_rad), cps, cpt);

      //std::cerr << "arc with center: "  << to_double(x_coord) << "  " << to_double(y_coord) << " and radius " << sqrt(to_double(sqr_rad)) << std::endl;	
      //std::cerr << "source: " << to_double(ps.x()) << ", " << to_double(ps.y()) << " target: " << to_double(pt.x()) << ", " << to_double(pt.y()) << std::endl << std::endl;

      //std::cerr << "arc with center: "  << x_coord << "  " << y_coord << " and radius " << sqrt(to_double(sqr_rad)) << std::endl;	
      //std::cerr << "source: " << ps.x() << ", " << ps.y() << " target: " << pt.x() << ", " << pt.y() << std::endl << std::endl;
      *res++ = arc;
    }
  }
  std::cout << " Loaded" << std::endl;
  
  return res;
}

}// namespace CGAL

#endif // CGAL_IO_DXF_VARIANT_READER_H
