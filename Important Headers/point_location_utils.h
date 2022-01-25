#ifndef POINT_LOCATION_H
#define POINT_LOCATION_H

//-----------------------------------------------------------------------------
// Print the result of a point-location query.
//
template <typename Arrangement>
void print_point_location(const typename Arrangement::Point_2& q,
                          const CGAL::Object& obj)
{
  typename Arrangement::Vertex_const_handle    v;
  typename Arrangement::Halfedge_const_handle  e;
  typename Arrangement::Face_const_handle      f;

  std::cout << "The point (" << q << ") is located ";
  if (CGAL::assign(f, obj)) {                   // q is located inside a face
    if (f->is_unbounded())
      std::cout << "inside the unbounded face." << std::endl;
    else std::cout << "inside a bounded face." << std::endl;
  }
  else if (CGAL::assign(e, obj)) {              // q is located on an edge
    std::cout << "on an edge: " << e->curve() << std::endl;
  }
  else if (CGAL::assign(v, obj)) {              // q is located on a vertex
    if (v->is_isolated())
      std::cout << "on an isolated vertex: " << v->point() << std::endl;
    else std::cout << "on a vertex: " << v->point() << std::endl;
  }
  else CGAL_error_msg("Invalid object!");       // this should never happen
}

//-----------------------------------------------------------------------------
// Perform a point-location query and print the result.
//
template <typename Point_location>
void locate_point(const Point_location& pl,
                  const typename Point_location::Arrangement_2::Point_2& q)
{
  CGAL::Object    obj = pl.locate(q);   // perform the point-location query

  // Print the result.
  print_point_location<typename Point_location::Arrangement_2>(q, obj);
}

//-----------------------------------------------------------------------------
// Perform a vertical ray-shooting query and print the result.
//
template <typename Ray_shooting>
void shoot_vertical_ray(const Ray_shooting& vrs,
                        const typename Ray_shooting::Arrangement_2::Point_2& q)
{
  // Perform the vertical ray-shooting query.
  CGAL::Object    obj = vrs.ray_shoot_up(q);

  // Print the result.
  typename Ray_shooting::Arrangement_2::Vertex_const_handle    v;
  typename Ray_shooting::Arrangement_2::Halfedge_const_handle  e;
  typename Ray_shooting::Arrangement_2::Face_const_handle      f;

  std::cout << "Shooting up from (" << q << ") : ";
  if (CGAL::assign(e, obj))                     // we hit an edge
    std::cout << "hit an edge: " << e->curve() << std::endl;
  else if (CGAL::assign(v, obj))                // we hit a vertex
    std::cout << "hit " << ((v->is_isolated()) ? "an isolated" : "a")
              << " vertex: " << v->point() << std::endl;
  else if (CGAL::assign(f, obj)) {              // we did not hit anything
    CGAL_assertion(f->is_unbounded());
    std::cout << "hit nothing." << std::endl; 
  }
  else CGAL_error_msg("Invalid object!");       // this should never happen
}

//-----------------------------------------------------------------------------
// Construct the arrangement of segments needed for the point-location and
// the vertical ray-shooting examples.
// The function assumes that the arrangement is of line segments with integer
// coordinates.
//			
template <typename Arrangement> void construct_segments_arr(Arrangement& arr)
{
  typedef typename Arrangement::Point_2              Point_2;
  typedef typename Arrangement::X_monotone_curve_2   Segment_2;
  typedef typename Arrangement::Halfedge_handle      Halfedge_handle;

  Point_2    p0(3,2), p1(0,3), p2(2,5), p3(4,5), p4(6,3), p5(3,0);
  Segment_2  s1(p1, p2), s2(p2, p3), s3(p3, p4), s4(p4, p5), s5(p5, p1);

  arr.insert_in_face_interior(p0, arr.unbounded_face());

  Halfedge_handle e1 = arr.insert_in_face_interior(s1, arr.unbounded_face());
  Halfedge_handle e2 = arr.insert_from_left_vertex(s2, e1->target());
  Halfedge_handle e3 = arr.insert_from_left_vertex(s3, e2->target());
  Halfedge_handle e4 = arr.insert_from_right_vertex(s4, e3->target());
  Halfedge_handle e5 = arr.insert_at_vertices(s5, e4->target(), e1->source());
}

#endif
