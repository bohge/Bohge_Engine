/*****************************************************************************/
/*                                                                           */
/*  (triangle.h)                                                             */
/*                                                                           */
/*  Include file for programs that call Triangle.                            */
/*                                                                           */
/*  Accompanies Triangle Version 1.6                                         */
/*  July 28, 2005                                                            */
/*                                                                           */
/*  Copyright 1996, 2005                                                     */
/*  Jonathan Richard Shewchuk                                                */
/*  2360 Woolsey #H                                                          */
/*  Berkeley, California  94705-1927                                         */
/*  jrs@cs.berkeley.edu                                                      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  How to call Triangle from another program                                */
/*                                                                           */
/*                                                                           */
/*  If you haven't read Triangle's instructions (run "triangle -h" to read   */
/*  them), you won't understand what follows.                                */
/*                                                                           */
/*  Triangle must be compiled into an object file (triangle.o) with the      */
/*  TRILIBRARY symbol defined (generally by using the -DTRILIBRARY compiler  */
/*  switch).  The makefile included with Triangle will do this for you if    */
/*  you run "make trilibrary".  The resulting object file can be called via  */
/*  the procedure triangulate().                                             */
/*                                                                           */
/*  If the size of the object file is important to you, you may wish to      */
/*  generate a reduced version of triangle.o.  The REDUCED symbol gets rid   */
/*  of all features that are primarily of research interest.  Specifically,  */
/*  the -DREDUCED switch eliminates Triangle's -i, -F, -s, and -C switches.  */
/*  The CDT_ONLY symbol gets rid of all meshing algorithms above and beyond  */
/*  constrained Delaunay triangulation.  Specifically, the -DCDT_ONLY switch */
/*  eliminates Triangle's -r, -q, -a, -u, -D, -Y, -S, and -s switches.       */
/*                                                                           */
/*  IMPORTANT:  These definitions (TRILIBRARY, REDUCED, CDT_ONLY) must be    */
/*  made in the makefile or in triangle.c itself.  Putting these definitions */
/*  in this file (triangle.h) will not create the desired effect.            */
/*                                                                           */
/*                                                                           */
/*  The calling convention for triangulate() follows.                        */
/*                                                                           */
/*      void triangulate(triswitches, in, out, vorout)                       */
/*      char *triswitches;                                                   */
/*      struct triangulateio *in;                                            */
/*      struct triangulateio *out;                                           */
/*      struct triangulateio *vorout;                                        */
/*                                                                           */
/*  `triswitches' is a string containing the command line switches you wish  */
/*  to invoke.  No initial dash is required.  Some suggestions:              */
/*                                                                           */
/*  - You'll probably find it convenient to use the `z' switch so that       */
/*    points (and other items) are numbered from zero.  This simplifies      */
/*    indexing, because the first item of any type always starts at index    */
/*    [0] of the corresponding array, whether that item's number is zero or  */
/*    one.                                                                   */
/*  - You'll probably want to use the `Q' (quiet) switch in your final code, */
/*    but you can take advantage of Triangle's printed output (including the */
/*    `V' switch) while debugging.                                           */
/*  - If you are not using the `q', `a', `u', `D', `j', or `s' switches,     */
/*    then the output points will be identical to the input points, except   */
/*    possibly for the boundary markers.  If you don't need the boundary     */
/*    markers, you should use the `N' (no nodes output) switch to save       */
/*    memory.  (If you do need boundary markers, but need to save memory, a  */
/*    good nasty trick is to set out->pointlist equal to in->pointlist       */
/*    before calling triangulate(), so that Triangle overwrites the input    */
/*    points with identical copies.)                                         */
/*  - The `I' (no iteration numbers) and `g' (.off file output) switches     */
/*    have no effect when Triangle is compiled with TRILIBRARY defined.      */
/*                                                                           */
/*  `in', `out', and `vorout' are descriptions of the input, the output,     */
/*  and the Voronoi output.  If the `v' (Voronoi output) switch is not used, */
/*  `vorout' may be NULL.  `in' and `out' may never be NULL.                 */
/*                                                                           */
/*  Certain fields of the input and output structures must be initialized,   */
/*  as described below.                                                      */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*  The `triangulateio' structure.                                           */
/*                                                                           */
/*  Used to pass data into and out of the triangulate() procedure.           */
/*                                                                           */
/*                                                                           */
/*  Arrays are used to store points, triangles, markers, and so forth.  In   */
/*  all cases, the first item in any array is stored starting at index [0].  */
/*  However, that item is item number `1' unless the `z' switch is used, in  */
/*  which case it is item number `0'.  Hence, you may find it easier to      */
/*  index points (and triangles in the neighbor list) if you use the `z'     */
/*  switch.  Unless, of course, you're calling Triangle from a Fortran       */
/*  program.                                                                 */
/*                                                                           */
/*  Description of fields (except the `numberof' fields, which are obvious): */
/*                                                                           */
/*  `pointlist':  An array of point coordinates.  The first point's x        */
/*    coordinate is at index [0] and its y coordinate at index [1], followed */
/*    by the coordinates of the remaining points.  Each point occupies two   */
/*    REALs.                                                                 */
/*  `pointattributelist':  An array of point attributes.  Each point's       */
/*    attributes occupy `numberofpointattributes' REALs.                     */
/*  `pointmarkerlist':  An array of point markers; one int per point.        */
/*                                                                           */
/*  `trianglelist':  An array of triangle corners.  The first triangle's     */
/*    first corner is at index [0], followed by its other two corners in     */
/*    counterclockwise order, followed by any other nodes if the triangle    */
/*    represents a nonlinear element.  Each triangle occupies                */
/*    `numberofcorners' ints.                                                */
/*  `triangleattributelist':  An array of triangle attributes.  Each         */
/*    triangle's attributes occupy `numberoftriangleattributes' REALs.       */
/*  `trianglearealist':  An array of triangle area constraints; one REAL per */
/*    triangle.  Input only.                                                 */
/*  `neighborlist':  An array of triangle neighbors; three ints per          */
/*    triangle.  Output only.                                                */
/*                                                                           */
/*  `segmentlist':  An array of segment endpoints.  The first segment's      */
/*    endpoints are at indices [0] and [1], followed by the remaining        */
/*    segments.  Two ints per segment.                                       */
/*  `segmentmarkerlist':  An array of segment markers; one int per segment.  */
/*                                                                           */
/*  `holelist':  An array of holes.  The first hole's x and y coordinates    */
/*    are at indices [0] and [1], followed by the remaining holes.  Two      */
/*    REALs per hole.  Input only, although the pointer is copied to the     */
/*    output structure for your convenience.                                 */
/*                                                                           */
/*  `regionlist':  An array of regional attributes and area constraints.     */
/*    The first constraint's x and y coordinates are at indices [0] and [1], */
/*    followed by the regional attribute at index [2], followed by the       */
/*    maximum area at index [3], followed by the remaining area constraints. */
/*    Four REALs per area constraint.  Note that each regional attribute is  */
/*    used only if you select the `A' switch, and each area constraint is    */
/*    used only if you select the `a' switch (with no number following), but */
/*    omitting one of these switches does not change the memory layout.      */
/*    Input only, although the pointer is copied to the output structure for */
/*    your convenience.                                                      */
/*                                                                           */
/*  `edgelist':  An array of edge endpoints.  The first edge's endpoints are */
/*    at indices [0] and [1], followed by the remaining edges.  Two ints per */
/*    edge.  Output only.                                                    */
/*  `edgemarkerlist':  An array of edge markers; one int per edge.  Output   */
/*    only.                                                                  */
/*  `normlist':  An array of normal vectors, used for infinite rays in       */
/*    Voronoi diagrams.  The first normal vector's x and y magnitudes are    */
/*    at indices [0] and [1], followed by the remaining vectors.  For each   */
/*    finite edge in a Voronoi diagram, the normal vector written is the     */
/*    zero vector.  Two REALs per edge.  Output only.                        */
/*                                                                           */
/*                                                                           */
/*  Any input fields that Triangle will examine must be initialized.         */
/*  Furthermore, for each output array that Triangle will write to, you      */
/*  must either provide space by setting the appropriate pointer to point    */
/*  to the space you want the data written to, or you must initialize the    */
/*  pointer to NULL, which tells Triangle to allocate space for the results. */
/*  The latter option is preferable, because Triangle always knows exactly   */
/*  how much space to allocate.  The former option is provided mainly for    */
/*  people who need to call Triangle from Fortran code, though it also makes */
/*  possible some nasty space-saving tricks, like writing the output to the  */
/*  same arrays as the input.                                                */
/*                                                                           */
/*  Triangle will not free() any input or output arrays, including those it  */
/*  allocates itself; that's up to you.  You should free arrays allocated by */
/*  Triangle by calling the trifree() procedure defined below.  (By default, */
/*  trifree() just calls the standard free() library procedure, but          */
/*  applications that call triangulate() may replace trimalloc() and         */
/*  trifree() in triangle.c to use specialized memory allocators.)           */
/*                                                                           */
/*  Here's a guide to help you decide which fields you must initialize       */
/*  before you call triangulate().                                           */
/*                                                                           */
/*  `in':                                                                    */
/*                                                                           */
/*    - `pointlist' must always point to a list of points; `numberofpoints'  */
/*      and `numberofpointattributes' must be properly set.                  */
/*      `pointmarkerlist' must either be set to NULL (in which case all      */
/*      markers default to zero), or must point to a list of markers.  If    */
/*      `numberofpointattributes' is not zero, `pointattributelist' must     */
/*      point to a list of point attributes.                                 */
/*    - If the `r' switch is used, `trianglelist' must point to a list of    */
/*      triangles, and `numberoftriangles', `numberofcorners', and           */
/*      `numberoftriangleattributes' must be properly set.  If               */
/*      `numberoftriangleattributes' is not zero, `triangleattributelist'    */
/*      must point to a list of triangle attributes.  If the `a' switch is   */
/*      used (with no number following), `trianglearealist' must point to a  */
/*      list of triangle area constraints.  `neighborlist' may be ignored.   */
/*    - If the `p' switch is used, `segmentlist' must point to a list of     */
/*      segments, `numberofsegments' must be properly set, and               */
/*      `segmentmarkerlist' must either be set to NULL (in which case all    */
/*      markers default to zero), or must point to a list of markers.        */
/*    - If the `p' switch is used without the `r' switch, then               */
/*      `numberofholes' and `numberofregions' must be properly set.  If      */
/*      `numberofholes' is not zero, `holelist' must point to a list of      */
/*      holes.  If `numberofregions' is not zero, `regionlist' must point to */
/*      a list of region constraints.                                        */
/*    - If the `p' switch is used, `holelist', `numberofholes',              */
/*      `regionlist', and `numberofregions' is copied to `out'.  (You can    */
/*      nonetheless get away with not initializing them if the `r' switch is */
/*      used.)                                                               */
/*    - `edgelist', `edgemarkerlist', `normlist', and `numberofedges' may be */
/*      ignored.                                                             */
/*                                                                           */
/*  `out':                                                                   */
/*                                                                           */
/*    - `pointlist' must be initialized (NULL or pointing to memory) unless  */
/*      the `N' switch is used.  `pointmarkerlist' must be initialized       */
/*      unless the `N' or `B' switch is used.  If `N' is not used and        */
/*      `in->numberofpointattributes' is not zero, `pointattributelist' must */
/*      be initialized.                                                      */
/*    - `trianglelist' must be initialized unless the `E' switch is used.    */
/*      `neighborlist' must be initialized if the `n' switch is used.  If    */
/*      the `E' switch is not used and (`in->numberofelementattributes' is   */
/*      not zero or the `A' switch is used), `elementattributelist' must be  */
/*      initialized.  `trianglearealist' may be ignored.                     */
/*    - `segmentlist' must be initialized if the `p' or `c' switch is used,  */
/*      and the `P' switch is not used.  `segmentmarkerlist' must also be    */
/*      initialized under these circumstances unless the `B' switch is used. */
/*    - `edgelist' must be initialized if the `e' switch is used.            */
/*      `edgemarkerlist' must be initialized if the `e' switch is used and   */
/*      the `B' switch is not.                                               */
/*    - `holelist', `regionlist', `normlist', and all scalars may be ignored.*/
/*                                                                           */
/*  `vorout' (only needed if `v' switch is used):                            */
/*                                                                           */
/*    - `pointlist' must be initialized.  If `in->numberofpointattributes'   */
/*      is not zero, `pointattributelist' must be initialized.               */
/*      `pointmarkerlist' may be ignored.                                    */
/*    - `edgelist' and `normlist' must both be initialized.                  */
/*      `edgemarkerlist' may be ignored.                                     */
/*    - Everything else may be ignored.                                      */
/*                                                                           */
/*  After a call to triangulate(), the valid fields of `out' and `vorout'    */
/*  will depend, in an obvious way, on the choice of switches used.  Note    */
/*  that when the `p' switch is used, the pointers `holelist' and            */
/*  `regionlist' are copied from `in' to `out', but no new space is          */
/*  allocated; be careful that you don't free() the same array twice.  On    */
/*  the other hand, Triangle will never copy the `pointlist' pointer (or any */
/*  others); new space is allocated for `out->pointlist', or if the `N'      */
/*  switch is used, `out->pointlist' remains uninitialized.                  */
/*                                                                           */
/*  All of the meaningful `numberof' fields will be properly set; for        */
/*  instance, `numberofedges' will represent the number of edges in the      */
/*  triangulation whether or not the edges were written.  If segments are    */
/*  not used, `numberofsegments' will indicate the number of boundary edges. */
/*                                                                           */
/*****************************************************************************/
#include "Predefine.h"
#include <string>
#define ANSI_DECLARATORS
#define SINGLE
#define VOID void

#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

void trifree(VOID *memptr);

struct triangulateio {
  REAL *pointlist;                                               /* In / out *///输入的三角型坐标点p[0]是X p[1]是y，以此类推
  REAL *pointattributelist;                                      /* In / out */
  int *pointmarkerlist;                                          /* In / out */
  int numberofpoints;                                            /* In / out *///输入点的个数，一个点是两个float
  int numberofpointattributes;                                   /* In / out */

  int *trianglelist;                                             /* In / out *///输出三角型坐标的位置
  REAL *triangleattributelist;                                   /* In / out */
  REAL *trianglearealist;                                         /* In only */
  int *neighborlist;                                             /* Out only */
  int numberoftriangles;                                         /* In / out *///输出三角型的个数
  int numberofcorners;                                           /* In / out *///每个三角型包含的索引数
  int numberoftriangleattributes;                                /* In / out */

  int *segmentlist;                                              /* In / out *///边界，s[0]到s[1]组成的边界,设置一个按顺序排列的边界以绘制一个凹多边形
  int *segmentmarkerlist;                                        /* In / out */
  int numberofsegments;                                          /* In / out *///边界个数

  REAL *holelist;                        /* In / pointer to array copied out *///坑洞，不会被填充的区域
  int numberofholes;                                      /* In / copied out *///区域顶点数

  REAL *regionlist;                      /* In / pointer to array copied out */
  int numberofregions;                                    /* In / copied out */

  int *edgelist;                                                 /* Out only */
  int *edgemarkerlist;            /* Not used with Voronoi diagram; out only */
  REAL *normlist;                /* Used only with Voronoi diagram; out only */
  int numberofedges;                                             /* Out only */
	triangulateio()
		:pointlist(NULL),
		pointattributelist(NULL),
		pointmarkerlist(NULL),
		numberofpoints(0),
		numberofpointattributes(0),

		trianglelist(NULL),
		triangleattributelist(NULL),
		trianglearealist(NULL),
		neighborlist(NULL),
		numberoftriangles(0),
		numberofcorners(0),
		numberoftriangleattributes(0),

		segmentlist(NULL),
		segmentmarkerlist(NULL),
		numberofsegments(0),

		holelist(NULL),
		numberofholes(0),

		regionlist(NULL),
		numberofregions(0),

		edgelist(NULL),
		edgemarkerlist(NULL),
		normlist(NULL),
		numberofedges(0)
	{}
	~triangulateio()
	{
		trifree(pointlist);
		trifree(pointattributelist);
		trifree(pointmarkerlist);

		trifree(trianglelist);
		trifree(triangleattributelist);
		trifree(trianglearealist);
		trifree(neighborlist);

		trifree(segmentlist);
		trifree(segmentmarkerlist);

		trifree(holelist);

		trifree(regionlist);

		trifree(edgelist);
		trifree(edgemarkerlist);
		trifree(normlist);
	}
};

#ifdef ANSI_DECLARATORS
void triangulate(char *, struct triangulateio *, struct triangulateio *, struct triangulateio *);
#else /* not ANSI_DECLARATORS */
void triangulate();
void trifree();
#endif /* not ANSI_DECLARATORS */



////例子
///*****************************************************************************/
///*                                                                           */
///*  (tricall.c)                                                              */
///*                                                                           */
///*  Example program that demonstrates how to call Triangle.                  */
///*                                                                           */
///*  Accompanies Triangle Version 1.6                                         */
///*  July 19, 1996                                                            */
///*                                                                           */
///*  This file is placed in the public domain (but the file that it calls     */
///*  is still copyrighted!) by                                                */
///*  Jonathan Richard Shewchuk                                                */
///*  2360 Woolsey #H                                                          */
///*  Berkeley, California  94705-1927                                         */
///*  jrs@cs.berkeley.edu                                                      */
///*                                                                           */
///*****************************************************************************/
//
///* If SINGLE is defined when triangle.o is compiled, it should also be       */
///*   defined here.  If not, it should not be defined here.                   */
//
///* #define SINGLE */
//
//#ifdef SINGLE
//#define REAL float
//#else /* not SINGLE */
//#define REAL double
//#endif /* not SINGLE */
//
//#include <stdio.h>
//#include <stdlib.h>
//#include "triangle.h"
//
///*****************************************************************************/
///*                                                                           */
///*  report()   Print the input or output.                                    */
///*                                                                           */
///*****************************************************************************/
//
//void report(io, markers, reporttriangles, reportneighbors, reportsegments,
//            reportedges, reportnorms)
//struct triangulateio *io;
//int markers;
//int reporttriangles;
//int reportneighbors;
//int reportsegments;
//int reportedges;
//int reportnorms;
//{
//  int i, j;
//
//  for (i = 0; i < io->numberofpoints; i++) {
//    printf("Point %4d:", i);
//    for (j = 0; j < 2; j++) {
//      printf("  %.6g", io->pointlist[i * 2 + j]);
//    }
//    if (io->numberofpointattributes > 0) {
//      printf("   attributes");
//    }
//    for (j = 0; j < io->numberofpointattributes; j++) {
//      printf("  %.6g",
//             io->pointattributelist[i * io->numberofpointattributes + j]);
//    }
//    if (markers) {
//      printf("   marker %d\n", io->pointmarkerlist[i]);
//    } else {
//      printf("\n");
//    }
//  }
//  printf("\n");
//
//  if (reporttriangles || reportneighbors) {
//    for (i = 0; i < io->numberoftriangles; i++) {
//      if (reporttriangles) {
//        printf("Triangle %4d points:", i);
//        for (j = 0; j < io->numberofcorners; j++) {
//          printf("  %4d", io->trianglelist[i * io->numberofcorners + j]);
//        }
//        if (io->numberoftriangleattributes > 0) {
//          printf("   attributes");
//        }
//        for (j = 0; j < io->numberoftriangleattributes; j++) {
//          printf("  %.6g", io->triangleattributelist[i *
//                                         io->numberoftriangleattributes + j]);
//        }
//        printf("\n");
//      }
//      if (reportneighbors) {
//        printf("Triangle %4d neighbors:", i);
//        for (j = 0; j < 3; j++) {
//          printf("  %4d", io->neighborlist[i * 3 + j]);
//        }
//        printf("\n");
//      }
//    }
//    printf("\n");
//  }
//
//  if (reportsegments) {
//    for (i = 0; i < io->numberofsegments; i++) {
//      printf("Segment %4d points:", i);
//      for (j = 0; j < 2; j++) {
//        printf("  %4d", io->segmentlist[i * 2 + j]);
//      }
//      if (markers) {
//        printf("   marker %d\n", io->segmentmarkerlist[i]);
//      } else {
//        printf("\n");
//      }
//    }
//    printf("\n");
//  }
//
//  if (reportedges) {
//    for (i = 0; i < io->numberofedges; i++) {
//      printf("Edge %4d points:", i);
//      for (j = 0; j < 2; j++) {
//        printf("  %4d", io->edgelist[i * 2 + j]);
//      }
//      if (reportnorms && (io->edgelist[i * 2 + 1] == -1)) {
//        for (j = 0; j < 2; j++) {
//          printf("  %.6g", io->normlist[i * 2 + j]);
//        }
//      }
//      if (markers) {
//        printf("   marker %d\n", io->edgemarkerlist[i]);
//      } else {
//        printf("\n");
//      }
//    }
//    printf("\n");
//  }
//}
//
///*****************************************************************************/
///*                                                                           */
///*  main()   Create and refine a mesh.                                       */
///*                                                                           */
///*****************************************************************************/
//
//int main()
//{
//  struct triangulateio in, mid, out, vorout;
//
//  /* Define input points. */
//
//  in.numberofpoints = 4;
//  in.numberofpointattributes = 1;
//  in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
//  in.pointlist[0] = 0.0;
//  in.pointlist[1] = 0.0;
//  in.pointlist[2] = 1.0;
//  in.pointlist[3] = 0.0;
//  in.pointlist[4] = 1.0;
//  in.pointlist[5] = 10.0;
//  in.pointlist[6] = 0.0;
//  in.pointlist[7] = 10.0;
//  in.pointattributelist = (REAL *) malloc(in.numberofpoints *
//                                          in.numberofpointattributes *
//                                          sizeof(REAL));
//  in.pointattributelist[0] = 0.0;
//  in.pointattributelist[1] = 1.0;
//  in.pointattributelist[2] = 11.0;
//  in.pointattributelist[3] = 10.0;
//  in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));
//  in.pointmarkerlist[0] = 0;
//  in.pointmarkerlist[1] = 2;
//  in.pointmarkerlist[2] = 0;
//  in.pointmarkerlist[3] = 0;
//
//  in.numberofsegments = 0;
//  in.numberofholes = 0;
//  in.numberofregions = 1;
//  in.regionlist = (REAL *) malloc(in.numberofregions * 4 * sizeof(REAL));
//  in.regionlist[0] = 0.5;
//  in.regionlist[1] = 5.0;
//  in.regionlist[2] = 7.0;            /* Regional attribute (for whole mesh). */
//  in.regionlist[3] = 0.1;          /* Area constraint that will not be used. */
//
//  printf("Input point set:\n\n");
//  report(&in, 1, 0, 0, 0, 0, 0);
//
//  /* Make necessary initializations so that Triangle can return a */
//  /*   triangulation in `mid' and a voronoi diagram in `vorout'.  */
//
//  mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
//  /* Not needed if -N switch used or number of point attributes is zero: */
//  mid.pointattributelist = (REAL *) NULL;
//  mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
//  mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
//  /* Not needed if -E switch used or number of triangle attributes is zero: */
//  mid.triangleattributelist = (REAL *) NULL;
//  mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
//  /* Needed only if segments are output (-p or -c) and -P not used: */
//  mid.segmentlist = (int *) NULL;
//  /* Needed only if segments are output (-p or -c) and -P and -B not used: */
//  mid.segmentmarkerlist = (int *) NULL;
//  mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
//  mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */
//
//  vorout.pointlist = (REAL *) NULL;        /* Needed only if -v switch used. */
//  /* Needed only if -v switch used and number of attributes is not zero: */
//  vorout.pointattributelist = (REAL *) NULL;
//  vorout.edgelist = (int *) NULL;          /* Needed only if -v switch used. */
//  vorout.normlist = (REAL *) NULL;         /* Needed only if -v switch used. */
//
//  /* Triangulate the points.  Switches are chosen to read and write a  */
//  /*   PSLG (p), preserve the convex hull (c), number everything from  */
//  /*   zero (z), assign a regional attribute to each element (A), and  */
//  /*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
//  /*   neighbor list (n).                                              */
//
//  triangulate("pczAevn", &in, &mid, &vorout);
//
//  printf("Initial triangulation:\n\n");
//  report(&mid, 1, 1, 1, 1, 1, 0);
//  printf("Initial Voronoi diagram:\n\n");
//  report(&vorout, 0, 0, 0, 0, 1, 1);
//
//  /* Attach area constraints to the triangles in preparation for */
//  /*   refining the triangulation.                               */
//
//  /* Needed only if -r and -a switches used: */
//  mid.trianglearealist = (REAL *) malloc(mid.numberoftriangles * sizeof(REAL));
//  mid.trianglearealist[0] = 3.0;
//  mid.trianglearealist[1] = 1.0;
//
//  /* Make necessary initializations so that Triangle can return a */
//  /*   triangulation in `out'.                                    */
//
//  out.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
//  /* Not needed if -N switch used or number of attributes is zero: */
//  out.pointattributelist = (REAL *) NULL;
//  out.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
//  /* Not needed if -E switch used or number of triangle attributes is zero: */
//  out.triangleattributelist = (REAL *) NULL;
//
//  /* Refine the triangulation according to the attached */
//  /*   triangle area constraints.                       */
//
//  triangulate("prazBP", &mid, &out, (struct triangulateio *) NULL);
//
//  printf("Refined triangulation:\n\n");
//  report(&out, 0, 1, 0, 0, 0, 0);
//
//  /* Free all allocated arrays, including those allocated by Triangle. */
//
//  free(in.pointlist);
//  free(in.pointattributelist);
//  free(in.pointmarkerlist);
//  free(in.regionlist);
//  free(mid.pointlist);
//  free(mid.pointattributelist);
//  free(mid.pointmarkerlist);
//  free(mid.trianglelist);
//  free(mid.triangleattributelist);
//  free(mid.trianglearealist);
//  free(mid.neighborlist);
//  free(mid.segmentlist);
//  free(mid.segmentmarkerlist);
//  free(mid.edgelist);
//  free(mid.edgemarkerlist);
//  free(vorout.pointlist);
//  free(vorout.pointattributelist);
//  free(vorout.edgelist);
//  free(vorout.normlist);
//  free(out.pointlist);
//  free(out.pointattributelist);
//  free(out.trianglelist);
//  free(out.triangleattributelist);
//
//  return 0;
//}








//
//
//
//
////德劳内三角测试
//#include "ISceneNode.h"
//#include "RendBuffer.h"
//#include "triangle.h"
//#include "CV.h"
//
//class Delaunay : public ISceneNode
//{
//private:
//	int				m_Size;
//	RendBuffer*		m_pRendBuffer;
//	Matrix44f		mat;
//public:
//	Delaunay()
//		:m_pRendBuffer(NULL),
//			m_Size(0)
//	{
//	}
//	void MakeMesh()
//	{
//		//vertex[0] = VertexColor( vector4f(0,0,-20), Color::BloodRed );
//		//vertex[1] = VertexColor( vector4f(10,0,-20), Color::BloodRed );
//		//vertex[2] = VertexColor( vector4f(0,0,0), Color::BloodRed );
//		//vertex[3] = VertexColor( vector4f(10,0,10), Color::BloodRed );
//		//vertex[4] = VertexColor( vector4f(-20,0,0), Color::BloodRed );
//		//vertex[5] = VertexColor( vector4f(-20,0,10), Color::BloodRed );
//
//		/* Define input points. */
//
//		float *contourArray = NULL;
//		int contourArraySize = 0;
//
//		ContourDLL_getContour( &contourArray, contourArraySize );
//		if( 0 != contourArraySize )
//		{
//			struct triangulateio in, mid;
//
//			vector<vector<float> > vvf;
//			in.numberofpoints = contourArraySize / 2;
//			int vvfIndex = -1;
//			for ( int i = 0 ; i < contourArraySize ; i ++ )
//			{
//				if( 9999.0 == contourArray[i] )
//				{
//					vvf.push_back( vector<float>() );
//					i ++;
//					vvfIndex++;
//				}
//				else
//				{
//					vvf[vvfIndex].push_back(contourArray[i]);
//				}
//			}
//			
//			in.numberofpoints = vvf[0].size()/2;
//			if( in.numberofpoints > 3 )
//			{
//				SAFE_DELETE(m_pRendBuffer);
//				m_pRendBuffer = NEW RendBuffer( VertexColor::Layout(), Device::LINES );
//				in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
//				for (int i = 0 ; i < in.numberofpoints ; i ++)
//				{
//					in.pointlist[i*2+0] = vvf[0][i*2] * 20.0;
//					in.pointlist[i*2+1] = - vvf[0][i*2+1] * 20.0;
//				}
//				in.segmentlist = NEW int[in.numberofpoints * 2];
//				in.numberofsegments = in.numberofpoints;
//				for ( int i = 0 ; i < in.numberofpoints; i ++ )
//				{
//					in.segmentlist[i*2] = i;
//					int end = i+1;
//					if ( end >= in.numberofpoints )
//					{
//						end = 0;
//					}
//					in.segmentlist[i*2+1] = end;
//				}
//				triangulate((char*)"pz", &in, &mid, NULL);
//
//				VertexColor* vertex = NEW VertexColor[mid.numberofpoints];
//				for( int i = 0 ; i < mid.numberofpoints ; i ++ )
//				{
//					vertex[i] = VertexColor( vector4f( mid.pointlist[i*2],0,mid.pointlist[i*2+1] ), Color::BloodRed );
//				}
//				m_pRendBuffer->MakeVertexBuffer( vertex, mid.numberofpoints, sizeof(VertexColor), Device::STATIC );
//
//				std::vector<uint> index;
//				for (int i = 0 ; i < mid.numberoftriangles; i ++)
//				{
//					int t1 = mid.trianglelist[i*mid.numberofcorners + 0];
//					int t2 = mid.trianglelist[i*mid.numberofcorners + 1];
//					int t3 = mid.trianglelist[i*mid.numberofcorners + 2];
//					index.push_back(t1);
//					index.push_back(t2);
//					index.push_back(t2);
//					index.push_back(t3);
//					index.push_back(t3);
//					index.push_back(t1);
//				}
//				m_pRendBuffer->MakeIndiesBuffer( &index[0], index.size(), Device::STATIC );
//				SAFE_DELETE_ARRAY(vertex);			
//			}
//		}
//
//		//in.numberofpoints = 8;
//		//in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
//		//in.pointlist[0] = 0.0;
//		//in.pointlist[1] = -20;
//
//		//in.pointlist[2] = -10.0;
//		//in.pointlist[3] = -20.0;
//
//		//in.pointlist[4] = -10.0;
//		//in.pointlist[5] = 10.0;
//
//		//in.pointlist[6] = 30.0;
//		//in.pointlist[7] = 10.0;
//
//		//in.pointlist[8] = 30.0;
//		//in.pointlist[9] = -10.0;
//
//		//in.pointlist[10]= 20.0;
//		//in.pointlist[11]= -10.0;
//
//
//		//in.pointlist[12]= 20.0;
//		//in.pointlist[13]= 0.0;
//
//		//in.pointlist[14]= 0.0;
//		//in.pointlist[15]= 0.0;
//
//
//		//in.segmentlist = NEW int[in.numberofpoints * 2];
//		//in.numberofsegments = in.numberofpoints;
//		//for ( int i = 0 ; i < in.numberofpoints; i ++ )
//		//{
//		//	in.segmentlist[i*2] = i;
//		//	int end = i+1;
//		//	if ( end >= in.numberofpoints )
//		//	{
//		//		end = 0;
//		//	}
//		//	in.segmentlist[i*2+1] = end;
//		//}
//		//
//
//		////in.numberofpointattributes = 1;
//		////in.pointattributelist = (REAL *) malloc(in.numberofpoints *
//		////	in.numberofpointattributes *
//		////	sizeof(REAL));
//		////in.pointattributelist[0] = 0.0;
//		////in.pointattributelist[1] = 1.0;
//		////in.pointattributelist[2] = 11.0;
//		////in.pointattributelist[3] = 10.0;
//		////in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));
//		////in.pointmarkerlist[0] = 0;
//		////in.pointmarkerlist[1] = 2;
//		////in.pointmarkerlist[2] = 0;
//		////in.pointmarkerlist[3] = 0;
//
//		////in.numberofsegments = 0;
//		////in.numberofholes = 0;
//		////in.numberofregions = 1;
//		////in.regionlist = (REAL *) malloc(in.numberofregions * 4 * sizeof(REAL));
//		////in.regionlist[0] = 0.5;
//		////in.regionlist[1] = 5.0;
//		////in.regionlist[2] = 7.0;            /* Regional attribute (for whole mesh). */
//		////in.regionlist[3] = 0.1;          /* Area constraint that will not be used. */
//		/////* Make necessary initializations so that Triangle can return a */
//		/////*   triangulation in `mid' and a voronoi diagram in `vorout'.  */
//
//		//mid.pointlist = (REAL *) NULL;            /* Not needed if -N switch used. */
//		///* Not needed if -N switch used or number of point attributes is zero: */
//		//mid.pointattributelist = (REAL *) NULL;
//		//mid.pointmarkerlist = (int *) NULL; /* Not needed if -N or -B switch used. */
//		//mid.trianglelist = (int *) NULL;          /* Not needed if -E switch used. */
//		///* Not needed if -E switch used or number of triangle attributes is zero: */
//		//mid.triangleattributelist = (REAL *) NULL;
//		//mid.neighborlist = (int *) NULL;         /* Needed only if -n switch used. */
//		///* Needed only if segments are output (-p or -c) and -P not used: */
//		//mid.segmentlist = (int *) NULL;
//		///* Needed only if segments are output (-p or -c) and -P and -B not used: */
//		//mid.segmentmarkerlist = (int *) NULL;
//		//mid.edgelist = (int *) NULL;             /* Needed only if -e switch used. */
//		//mid.edgemarkerlist = (int *) NULL;   /* Needed if -e used and -B not used. */
//
//
//		///* Triangulate the points.  Switches are chosen to read and write a  */
//		///*   PSLG (p), preserve the convex hull (c), number everything from  */
//		///*   zero (z), assign a regional attribute to each element (A), and  */
//		///*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
//		///*   neighbor list (n).                                              */
//
//		//triangulate((char*)"pz", &in, &mid, NULL);
//
//		//VertexColor* vertex = NEW VertexColor[mid.numberofpoints];
//		//for( int i = 0 ; i < mid.numberofpoints ; i ++ )
//		//{
//		//	vertex[i] = VertexColor( vector4f( mid.pointlist[i*2],0,mid.pointlist[i*2+1] ), Color::BloodRed );
//		//}
//		//m_pRendBuffer->MakeVertexBuffer( vertex, mid.numberofpoints, sizeof(VertexColor), Device::STATIC );
//
//		//std::vector<uint> index;
//		//for (int i = 0 ; i < mid.numberoftriangles; i ++)
//		//{
//		//	int t1 = mid.trianglelist[i*mid.numberofcorners + 0];
//		//	int t2 = mid.trianglelist[i*mid.numberofcorners + 1];
//		//	int t3 = mid.trianglelist[i*mid.numberofcorners + 2];
//		//	index.push_back(t1);
//		//	index.push_back(t2);
//		//	index.push_back(t2);
//		//	index.push_back(t3);
//		//	index.push_back(t3);
//		//	index.push_back(t1);
//		//}
//		//m_pRendBuffer->MakeIndiesBuffer( &index[0], index.size(), Device::STATIC );
//		//SAFE_DELETE_ARRAY(vertex);
//	}
//	bool isInside(float* xa, float* ya, float cx, float cy)
//	{
//		return true;
//		float anglesum = 0.0f;
//		vector2f c(cx, cy);
//		c.NormalizeSelf();
//		for ( int i = 0 ; i < m_Size ; i ++)
//		{
//			vector2f e(xa[i], ya[i]);
//			e.NormalizeSelf();
//			vector2f l = e - c;
//			anglesum += l.m_x*1;
//		}
//		if ( 0 == anglesum )
//		{
//			return true;
//		}
//		return false;
//	}
//	void Render(engine& engine) const
//	{
//		if( NULL != m_pRendBuffer )
//		{
//			ShapesShader& ss = engine.GetShaderManage()->GetShader<ShapesShader>(ShaderManage::ShapesShader);
//			ss.SetParamTransform( &mat );
//			engine.GetDevice()->Draw( *m_pRendBuffer, ss, NULL );
//		}
//	}
//};
//Delaunay* d;