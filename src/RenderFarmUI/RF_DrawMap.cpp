/*
 * Copyright (c) 2004, Laminar Research.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include "RF_DrawMap.h"
#if APL
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <gl.h>
	#include <glu.h>
#endif
#include "ParamDefs.h"
#include "MapAlgs.h"
#include "DEMTables.h"
#include "XPLMGraphics.h"
#include "XPLMProcessing.h"
#include "XESConstants.h"
#include "ObjTables.h"
#include "XPLMDisplay.h"

#define DRAW_FACES 1
#define DRAW_FEATURES 1
#define DRAW_EDGES 1
#define DRAW_VERTICES 1
#define DRAW_FOOTPRINTS 1

#if APL
#define STDCALL_MACRO
#elif IBM
#define STDCALL_MACRO __stdcall
#endif

vector<float> *	gAccum;
int				gMode;
int				gCount;
float			gFirst[2];
float			gPrev[2];
float			gPrevPrev[2];

void PRECALC_Begin (GLenum mode)
{
	gMode = mode;
	gCount = 0;
}

void PRECALC_Vertex2fv (const GLfloat *v)
{
	if (gMode == GL_TRIANGLES)
	{
		gAccum->insert(gAccum->end(), v, v+2);
	}
	if (gMode == GL_TRIANGLE_FAN)
	{
		if (gCount == 0) 	{ 	gFirst[0] = v[0];  gFirst[1] = v[1]; }
		if (gCount > 1)		{ 	gAccum->insert(gAccum->end(), gFirst, gFirst+2);
								gAccum->insert(gAccum->end(), gPrev, gPrev+2);
								gAccum->insert(gAccum->end(), v, v+2); }
		gPrev[0] = v[0]; gPrev[1] = v[1];
	}
	if (gMode == GL_TRIANGLE_STRIP)
	{
		if (gCount > 1 && (gCount % 2)==1) {	gAccum->insert(gAccum->end(), gPrev, gPrev+2);
												gAccum->insert(gAccum->end(), gPrevPrev, gPrevPrev+2);
												gAccum->insert(gAccum->end(), v, v+2); }
		if (gCount > 1 && (gCount % 2)==0) {	gAccum->insert(gAccum->end(), gPrevPrev, gPrevPrev+2);
												gAccum->insert(gAccum->end(), gPrev, gPrev+2);
												gAccum->insert(gAccum->end(), v, v+2); }
		gPrevPrev[0] = gPrev[0]; gPrevPrev[1] = gPrev[1];
		gPrev[0] = v[0]; gPrev[1] = v[1];

	}
	++gCount;
}

void PRECALC_End (void)
{
}



void draw_he(Halfedge_handle e)
{
	Bezier2	b;
	b.p1 = cgal2ben(e->source()->point());
	b.p2 = cgal2ben(e->target()->point());
	b.c1=b.p1;
	b.c2=b.p2;

	if(e->source()->degree() == 2)
	{
		Vector2	vs(b.p1,b.p2);
		Halfedge_handle prev(e->twin()->next());
		Vector2 vp(cgal2ben(prev->source()->point()), cgal2ben(prev->target()->point()));
		if(!CGAL::collinear(prev->target()->point(),e->source()->point(),e->target()->point()))
		if(vs.dot(vp) < 0.0)
		{
			vs.normalize();
			vp.normalize();
			Vector2 n(vs+vp);
			n.normalize();
			vp*=-1.0;
			n*=-1.0;
			if(vp.left_turn(vs))
				n=n.perpendicular_ccw();
			else
				n=n.perpendicular_cw();

			b.c1 = b.p1 + n * (sqrt(Segment2(b.p1,b.p2).squared_length()) / 3.0);
		}
	}
	if(e->target()->degree() == 2)
	{
		Vector2	vs(b.p2,b.p1);
		Halfedge_handle next(e->next());
		Vector2 vn(cgal2ben(next->source()->point()), cgal2ben(next->target()->point()));
		if(!CGAL::collinear(e->source()->point(),e->target()->point(),next->target()->point()))
		if(vs.dot(vn) < 0.0)
		{
			vs.normalize();
			vn.normalize();
			Vector2 n(vs+vn);
			n.normalize();
			vn*=-1.0;
			n*=-1.0;
			if(vn.left_turn(vs))
				n=n.perpendicular_ccw();
			else
				n=n.perpendicular_cw();

			b.c2 = b.p2 + n * (sqrt(Segment2(b.p2,b.p1).squared_length()) / 3.0);
		}

	}

/*
	glVertex2f(b.p1.x_,b.p1.y_);
	glVertex2f(b.p2.x_,b.p2.y_);

	glVertex2f(b.p1.x_,b.p1.y_);
	glVertex2f(b.c1.x_,b.c1.y_);

	glVertex2f(b.p2.x_,b.p2.y_);
	glVertex2f(b.c2.x_,b.c2.y_);

*/
	float c = 10.0;
	if(b.p1 == b.c1 && b.p2 == b.c2)
	c=1.0;

	if(XPLMGetModifiers() & xplm_ShiftFlag)
		c=1.0;

	for(float s = 0.0; s < c; ++s)
	{
		Point2	p(b.midpoint(s/c));
		glVertex2f(p.x_,p.y_);
		p = b.midpoint((s+1.0)/c);
		glVertex2f(p.x_,p.y_);
	}

}

inline void SetColor(float c[3], float r, float g, float b)
{
	c[0] = r; c[1] = g; c[2] = b;
}

static void	SetColorForHalfedge(Pmwx::Halfedge_const_handle i, float color[3])
{
	int	terrainChange = !i->face()->data().TerrainMatch(i->twin()->face()->data());
	int	border = i->face()->is_unbounded() || i->twin()->face()->is_unbounded();
	int river = i->data().mParams.find(he_IsRiver) != i->data().mParams.end();
	int dryriver = i->data().mParams.find(he_IsDryRiver) != i->data().mParams.end();

	int	wet = i->face()->data().IsWater() || i->twin()->face()->data().IsWater();

//	bool beach = i->mTransition != 0;

	if (border)
		SetColor(color,1.0, 0.0, 1.0);

	int tp = NO_VALUE;
	for( GISNetworkSegmentVector::iterator r = i->data().mSegments.begin(); r != i->data().mSegments.end(); ++r)
	if(tp == NO_VALUE || r->mFeatType < tp)
		tp = r->mFeatType;
	for( GISNetworkSegmentVector::iterator r = i->twin()->data().mSegments.begin(); r != i->twin()->data().mSegments.end(); ++r)
	if(tp == NO_VALUE || r->mFeatType < tp)
		tp = r->mFeatType;

	if(tp != NO_VALUE)
	{
		SetColor(color,1.0, 0.0, 1.0);
		if (Road_IsTrain   (tp))	SetColor(color,0.5, 0.3, 0.1);
		if (Road_IsPowerline(tp))	SetColor(color,1.0, 1.0, 0.0);
		if (Road_IsHighway (tp))	SetColor(color,0.0, 1.0, 1.0);
		if (Road_IsMainDrag(tp))	SetColor(color,1.0, 0.0, 0.0);
		if (Road_IsLocal   (tp))	SetColor(color,0.7, 0.4, 0.4);
		if (Road_IsAccess  (tp))	SetColor(color,0.6, 0.6, 0.6);
		if (Road_IsWalkway (tp))	SetColor(color,0.4, 0.2, 0.1);
		if (Road_IsDam	   (tp))	SetColor(color,1.0, 1.0, 1.0);

	} else {
//		if (beach)
//			SetColor(color,0.8, 0.6, 0.2);
		 if (wet)
			SetColor(color,0.1, 0.3, 1.0);
		else if (terrainChange)
			SetColor(color,0.7, 0.7, 0.7);
		else if (river)
			SetColor(color,0.0, 0.0, 1.0);
		else if (dryriver)
			SetColor(color, 0.6, 0.6, 0.3);
		else
			SetColor(color,0.3, 0.3, 0.3);
	}
}

static	void	SetColorForFace(Pmwx::Face_const_handle f, float outColor[4])
{
	float red = 0.0;
	float green = 0.0;
	float blue = 0.0;

	RGBColor_t& rgbc = gEnumColors[f->data().mTerrainType];
	red = rgbc.rgb[0];
	green = rgbc.rgb[1];
	blue = rgbc.rgb[2];
#if 0

	switch(f->data().mTerrainType) {
	case NO_VALUE:							red = 1.0;  green = 0.0;	blue = 1.0; break;
	case terrain_Natural:					red = 0.0;	green = 0.0;	blue = 0.0;	break;
	case terrain_Water:						red = 0.0;	green = 0.0;	blue = 0.5;	break;

	case terrain_OutlayResidential:
	case terrain_OutlayResidentialHill:		red = 0.2;	green = 0.2;	blue = 0.2;
	case terrain_OutlayHighrise:
	case terrain_OutlayHighriseHill:		red = 0.0;	green = 0.4;	blue = 0.4; break;
	case terrain_Residential:
	case terrain_ResidentialHill:			red = 0.6;	green = 0.6;	blue = 0.0; break;
	case terrain_CommercialSprawl:
	case terrain_CommercialSprawlHill:		red = 0.0;	green = 0.6;	blue = 0.6; break;
	case terrain_Urban:
	case terrain_UrbanHill:					red = 0.8;	green = 0.8;	blue = 0.0; break;
	case terrain_Industrial:
	case terrain_IndustrialHill:			red = 0.8;	green = 0.5;	blue = 0.0;	break;
	case terrain_Downtown:
	case terrain_DowntownHill:				red = 1.0;	green = 1.0;	blue = 0.0;	break;

	case terrain_FarmTown:
	case terrain_FarmTownHill:				red = 0.2;	green = 0.5;	blue = 0.2;	break;
	case terrain_Farm:
	case terrain_FarmHill:					red = 0.0;	green = 0.5;	blue = 0.0;	break;
	case terrain_MixedFarm:
	case terrain_MixedFarmHill:				red = 0.0;	green = 0.3;	blue = 0.0;	break;

	case terrain_MilitaryBase:				red = 0.3;	green = 0.3;	blue = 0.3;	break;
	case terrain_TrailerPark:				red = 0.0;	green = 0.3;	blue = 0.0;	break;
	case terrain_Campground:				red = 0.0;	green = 0.3;	blue = 0.0;	break;
	case terrain_Marina:					red = 0.0;	green = 0.0;	blue = 0.8;	break;

	case terrain_GolfCourse:				red = 0.2;	green = 0.5;	blue = 0.0;	break;
	case terrain_Cemetary:					red = 0.2;	green = 0.5;	blue = 0.0;	break;
	case terrain_Airport:					red = 0.3;	green = 0.3;	blue = 0.3;	break;
	case terrain_AirportOuter:				red = 0.3;	green = 0.3;	blue = 0.2;	break;
	case terrain_Park:						red = 0.2;	green = 0.5;	blue = 0.0;	break;
	case terrain_ForestPark:				red = 0.2;	green = 0.4;	blue = 0.0;	break;
	}
#endif
	if (red == 0.0 && green == 0.0 && blue == 0.0)
	if (f->data().mAreaFeature.mFeatType != NO_VALUE)
		green = 0.5;

	if (red == 0.0 && green == 0.0 && blue == 0.0) outColor[3] = 0.0; else outColor[3] = 0.5;
	outColor[0] = red;
	outColor[1] = green;
	outColor[2] = blue;
}

void	PrecalcOGL(Pmwx&						ioMap, ProgressFunc inFunc)
{
	int total = ioMap.number_of_vertices() + ioMap.number_of_halfedges() + ioMap.number_of_faces();
	int ctr = 0;

	PROGRESS_START(inFunc, 0, 1, "Building preview of vector map...")

	for (Pmwx::Vertex_iterator v = ioMap.vertices_begin(); v != ioMap.vertices_end(); ++v, ++ctr)
	{
		PROGRESS_CHECK(inFunc, 0, 1, "Building preview of vector map...", ctr, total, 1000)
		v->data().mGL[0] = CGAL::to_double(v->point().x());
		v->data().mGL[1] = CGAL::to_double(v->point().y());
	}

	for (Pmwx::Halfedge_iterator h = ioMap.halfedges_begin(); h != ioMap.halfedges_end(); ++h, ++ctr)
	{
		PROGRESS_CHECK(inFunc, 0, 1, "Building preview of vector map...", ctr, total, 1000)
		h->data().mGL[0] = CGAL::to_double(h->source()->point().x());
		h->data().mGL[1] = CGAL::to_double(h->source()->point().y());
		h->data().mGL[2] = CGAL::to_double(h->target()->point().x());
		h->data().mGL[3] = CGAL::to_double(h->target()->point().y());
	}

	for (Pmwx::Face_iterator f = ioMap.faces_begin(); f != ioMap.faces_end(); ++f, ++ctr)
	if (!f->is_unbounded())
	{
		PROGRESS_CHECK(inFunc, 0, 1, "Building preview of vector map...", ctr, total, 1000)

		f->data().mGLTris.clear();
		gAccum = &f->data().mGLTris;

		GLUtriangulatorObj *tobj;   /* tessellation object */
		GLdouble v[3];              /* passed to gluTessVertex, prototype used 3d */

		vector<float>	storage;
		int needed = 0;

		Pmwx::Ccb_halfedge_circulator	i, stop;
		Pmwx::Hole_iterator holes;
		i = stop = f->outer_ccb();
		do {
			++i, ++needed;
		} while (i != stop);
		Pmwx::Hole_iterator hole;
		for (hole = f->holes_begin(); hole != f->holes_end(); ++hole)
		{
			i = stop = *hole;
			do {
				++i, ++needed;
			} while (i != stop);
		}

		storage.resize(needed * 2);
		float * vv = &*storage.begin();

		tobj = gluNewTess();
		gluTessCallback(tobj, GLU_BEGIN, (GLvoid (STDCALL_MACRO *)())PRECALC_Begin);
		gluTessCallback(tobj, GLU_VERTEX, (GLvoid (STDCALL_MACRO *)())PRECALC_Vertex2fv);
		gluTessCallback(tobj, GLU_END, (GLvoid (STDCALL_MACRO *)()) PRECALC_End);
		gluBeginPolygon(tobj);
		gluNextContour(tobj, GLU_EXTERIOR);

		int ctr = 0;
		i = stop = f->outer_ccb();
		do {
			vv[ctr*2  ] = v[0] = CGAL::to_double(i->source()->point().x());
			vv[ctr*2+1] = v[1] = CGAL::to_double(i->source()->point().y());
						  v[2] = 0.0;
			gluTessVertex(tobj, v, &vv[ctr*2]);
			++i, ++ctr;
		} while (i != stop);
		for (hole = f->holes_begin(); hole != f->holes_end(); ++hole)
		{
			gluNextContour(tobj, GLU_INTERIOR);
			i = stop = *hole;
			do {
				vv[ctr*2  ] = v[0] = CGAL::to_double(i->source()->point().x());
				vv[ctr*2+1] = v[1] = CGAL::to_double(i->source()->point().y());
							  v[2] = 0.0;
				gluTessVertex(tobj, v, &vv[ctr*2]);
				++i, ++ctr;
			} while (i != stop);
		}
		if (ctr != needed)
			printf("ERROR!\n");
		gluEndPolygon(tobj);
		gluDeleteTess(tobj);

	}

	PROGRESS_DONE(inFunc, 0, 1, "Building preview of vector map...")

	RecalcOGLColors(ioMap, inFunc);
}

void	RecalcOGLColors(Pmwx& ioMap, ProgressFunc inFunc)
{
	int total = ioMap.number_of_halfedges() + ioMap.number_of_faces();
	int ctr = 0;
	PROGRESS_START(inFunc, 0, 1, "Setting colors for vector map...")

	for (Pmwx::Halfedge_iterator h = ioMap.halfedges_begin(); h != ioMap.halfedges_end(); ++h, ++ctr)
	{
		PROGRESS_CHECK(inFunc, 0, 1, "Setting colors for vector map...", ctr, total, 1000)
		SetColorForHalfedge(h, h->data().mGLColor);
//		SetColorForHalfedge(h, h->twin()->data().mGLColor);
	}

	for (Pmwx::Face_iterator f = ioMap.faces_begin(); f != ioMap.faces_end(); ++f, ++ctr)
	if (!f->is_unbounded())
	{
		PROGRESS_CHECK(inFunc, 0, 1, "Setting colors for vector map...", ctr, total, 1000)
		SetColorForFace(f, f->data().mGLColor);
	}
	PROGRESS_DONE(inFunc, 0, 1, "Setting colors for vector map...")

}


void nonConvexPolygon( const Polygon2& p, const vector<Polygon2>& pp)
{
	glDisable(GL_CULL_FACE);


	GLUtriangulatorObj *tobj;   /* tessellation object */
	GLdouble v[3];              /* passed to gluTessVertex, prototype used 3d */

	GLenum err = glGetError();

	vector<float>	storage;
	int needed = p.size();
	for (int n = 0; n < pp.size(); ++n)
		needed += pp[n].size();
	storage.resize(needed * 2);
	float * vv = &*storage.begin();

	tobj = gluNewTess();
	gluTessCallback(tobj, GLU_BEGIN, (GLvoid (STDCALL_MACRO *)())glBegin);
	gluTessCallback(tobj, GLU_VERTEX, (GLvoid (STDCALL_MACRO *)())glVertex2fv);
	gluTessCallback(tobj, GLU_END, glEnd);
	gluBeginPolygon(tobj);
	gluNextContour(tobj, GLU_EXTERIOR);

	int ctr = 0;
	for (Polygon2::const_iterator i = p.begin(); i != p.end(); ++i, ++ctr)
	{
		/* send vertex for tessellation, it expects 3d array of double */
		vv[ctr*2] = v[0] = i->x();
		vv[ctr*2+1] = v[1] = i->y();
		v[2] = 0.0;

		gluTessVertex(tobj, v, &vv[ctr*2]);
	}

	for (vector<Polygon2>::const_iterator ii = pp.begin(); ii != pp.end(); ++ii)
	{
		gluNextContour(tobj, GLU_INTERIOR);
		for (Polygon2::const_iterator i = ii->begin(); i != ii->end(); ++i, ++ctr)
		{
			/* send vertex for tessellation, it expects 3d array of double */
			vv[ctr*2] = v[0] = i->x();
			vv[ctr*2+1] = v[1] = i->y();
			v[2] = 0.0;

			gluTessVertex(tobj, v, &vv[ctr*2]);
		}
	}

	gluEndPolygon(tobj);
	gluDeleteTess(tobj);
}

static void	MapMouseToCoord(
				double			mapWest,
				double			mapSouth,
				double			mapEast,
				double			mapNorth,
				double			screenLeft,
				double			screenBottom,
				double			screenRight,
				double			screenTop,
				double			inX,
				double			inY,
				double&			outLon,
				double&			outLat)
{
	double	mapWidth = mapEast - mapWest;
	double	mapHeight = mapNorth - mapSouth;
	double	screenWidth = screenRight - screenLeft;
	double	screenHeight = screenTop - screenBottom;
	outLon = mapWest + (inX - screenLeft) * mapWidth / screenWidth;
	outLat = mapSouth + (inY - screenBottom) * mapHeight / screenHeight;
}

void	RemapPolygonCoords(Polygon2& p,
				double			mapWest,
				double			mapSouth,
				double			mapEast,
				double			mapNorth,
				double			screenLeft,
				double			screenBottom,
				double			screenRight,
				double			screenTop)
{
	for (Polygon2::iterator v = p.begin(); v != p.end(); ++v)
	{
		double	xn, yn;
		MapMouseToCoord(mapWest, mapSouth, mapEast, mapNorth, screenLeft, screenBottom, screenRight, screenTop,
					v->x(), v->y(), xn, yn);
		*v = Point2(xn, yn);
	}
}

void	CirculatorToPoly(Pmwx::Ccb_halfedge_const_circulator circ, Polygon2& poly)
{
	Pmwx::Ccb_halfedge_const_circulator i = circ;
	do {
		poly.push_back(cgal2ben(i->source()->point()));
		++i;
	} while (i != circ);
}

void	FaceToScaledPoly(Pmwx::Face_const_handle	f, Polygon2& p, vector<Polygon2>& pp,
				double			mapWest,
				double			mapSouth,
				double			mapEast,
				double			mapNorth,
				double			screenLeft,
				double			screenBottom,
				double			screenRight,
				double			screenTop)
{
	if (f->is_unbounded()) return;
	pp.clear();
	CirculatorToPoly(f->outer_ccb(), p);
	for (Pmwx::Hole_const_iterator h = f->holes_begin(); h != f->holes_end(); ++h)
	{
		Polygon2 hole;
		CirculatorToPoly(Pmwx::Ccb_halfedge_const_circulator(*h), hole);
		pp.push_back(hole);
	}
	RemapPolygonCoords(p, mapWest, mapSouth, mapEast, mapNorth, screenLeft, screenBottom, screenRight, screenTop);
	for (vector<Polygon2>::iterator iter = pp.begin(); iter != pp.end(); ++iter)
	{
		RemapPolygonCoords(*iter, mapWest, mapSouth, mapEast, mapNorth, screenLeft, screenBottom, screenRight, screenTop);
	}
}


void	DrawMapBucketed(
				Pmwx& 	inMap,
				double			mapWest,
				double			mapSouth,
				double			mapEast,
				double			mapNorth,
//				double			screenLeft,
//				double			screenBottom,
//				double			screenRight,
//				double			screenTop,
				const set<Pmwx::Vertex_handle>&		vertexSel,
				const set<Pmwx::Halfedge_handle>&	edgeSel,
				const set<Pmwx::Face_handle>&		faceSel,
				const set<PointFeatureSelection>&	pointFeatureSel)
{
	XPLMSetGraphicsState(0, 0, 0,   0, 1,   0, 0);

	double	mapWidth = mapEast - mapWest;
	double	mapHeight = mapNorth - mapSouth;
//	double	screenWidth = screenRight - screenLeft;
//	double	screenHeight = screenTop - screenBottom;

	vector<Pmwx::Face_handle>		faces;
	vector<Pmwx::Halfedge_handle>	halfedges;
	vector<Pmwx::Vertex_handle>		vertices;
	FindFaceTouchesRectFast(inMap,Point2(mapWest, mapSouth), Point2(mapEast, mapNorth), faces);
	FindHalfedgeTouchesRectFast(inMap,Point2(mapWest, mapSouth), Point2(mapEast, mapNorth), halfedges);
	FindVerticesTouchesRect(inMap,Point2(mapWest, mapSouth), Point2(mapEast, mapNorth), vertices);

//	for (Pmwx::Face_iterator f = inMap.faces_begin(); f != inMap.faces_end(); ++f) faces.push_back(f);
//	for (Pmwx::Halfedge_iterator e = inMap.halfedges_begin(); e != inMap.halfedges_end(); ++e) halfedges.push_back(e);
//	for (Pmwx::Vertex_iterator v = inMap.vertices_begin(); v != inMap.vertices_end(); ++v) vertices.push_back(v);

//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glTranslated(screenLeft, screenBottom, 0.0);
//	glScaled(screenWidth / mapWidth, screenHeight / mapHeight, 1.0);
//	glTranslated(-mapWest, -mapSouth, 0.0);

	/******************************************************************************************
	 * DRAW FACES - SELECTED AND CONTAINING ZONING INFO
	 ******************************************************************************************/

#if DRAW_FACES
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLES);

	for (vector<Pmwx::Face_handle>::iterator fi = faces.begin(); fi != faces.end(); ++fi)
	{
		Pmwx::Face_handle f = *fi;
		if (!f->is_unbounded())
		{
			bool	sel = faceSel.count(f) > 0;
			bool	draw = sel || f->data().mGLColor[3] != 0.0;

			if (draw)
			{
				if (sel)
					glColor4f(f->data().mGLColor[0] * 0.5 + 0.5, f->data().mGLColor[1] * 0.5, f->data().mGLColor[2] * 0.5, 0.8);
				else
					glColor4fv(f->data().mGLColor);

				float * vs, * ve;
				vs = &*f->data().mGLTris.begin();
				ve = &*f->data().mGLTris.end();

				for (float * vv = vs; vv != ve; vv +=2)
				{
					glVertex2fv(vv);
				}
			}
		}
	}
	glEnd();
#endif

	/******************************************************************************************
	 * DRAW HALFEDGES - SELECTED AND ONES WITH TRANSPORTATION
	 ******************************************************************************************/

#if DRAW_EDGES
	glBegin(GL_LINES);
	int	width = 1;
	for (vector<Pmwx::Halfedge_handle>::iterator he = halfedges.begin(); he != halfedges.end(); ++he)
	{
		Pmwx::Halfedge_handle e = *he;
		{
			glColor3fv(e->data().mGLColor);
			int wantWidth = (edgeSel.find(e) != edgeSel.end() || edgeSel.find(e->twin()) != edgeSel.end()) ? 2 : 1;
			if (width != wantWidth)
			{
				glEnd();
				glLineWidth(wantWidth);
				width = wantWidth;
				glBegin(GL_LINES);
			}

			glVertex2fv(e->data().mGL);
			glVertex2fv(e->data().mGL+2);

//			draw_he(e);
		}
	}
	glEnd();
	glLineWidth(1);
#endif

	/******************************************************************************************
	 * DRAW POLYGON AND POINT OBJECTS INSIDE FACES
	 ******************************************************************************************/
#if DRAW_FEATURES
	for (vector<Pmwx::Face_handle>::iterator fi = faces.begin(); fi != faces.end(); ++fi)
	{
		Pmwx::Face_handle f = *fi;
		if (!f->is_unbounded())
		{
			bool	draw = faceSel.count(f);

			if (draw)
			{
				// THIS CODE DRAWS PT OBJECTS IN CYAN INSIDE ALL FACES
				glPointSize(3);
				glBegin(GL_POINTS);
				for (GISObjPlacementVector::const_iterator obj = f->data().mObjs.begin(); obj != f->data().mObjs.end(); ++obj)
				{
					double	x1 = CGAL::to_double(obj->mLocation.x());
					double	y1 = CGAL::to_double(obj->mLocation.y());
					glVertex2f(x1, y1);
				}
				glEnd();
				glPointSize(1);
			}
		}
	}
#endif

	/******************************************************************************************
	 * DRAW VERTICES
	 ******************************************************************************************/

#if DRAW_VERTICES
	if (!vertices.empty())
	{
		glPointSize(3);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		for (vector<Pmwx::Vertex_handle>::iterator v = vertices.begin(); v != vertices.end(); ++v)
		{
			if (vertexSel.find(*v) == vertexSel.end())
				continue;

			double	x1 = CGAL::to_double((*v)->point().x());
			double	y1 = CGAL::to_double((*v)->point().y());
//			x1 = screenLeft + ((x1 - mapWest) * screenWidth / mapWidth);
//			y1 = screenBottom + ((y1 - mapSouth) * screenHeight / mapHeight);
			glVertex2fv((*v)->data().mGL);
		}
		glEnd();
		glPointSize(1);
	}
#endif

	/******************************************************************************************
	 * DRAW BUILDING FOOTPRINTS ON SELECTED FACES
	 ******************************************************************************************/

#if DRAW_FOOTPRINTS
	glMatrixMode(GL_MODELVIEW);
	for (vector<Pmwx::Face_handle>::iterator fi = faces.begin(); fi != faces.end(); ++fi)
	{
		int n = 0;
		bool	 fsel = faceSel.find(*fi) != faceSel.end();
		if (!fsel) continue;
		for (int j = 0; j < (*fi)->data().mObjs.size(); ++j)
		{
			float shade = (float) (n % 10) / 20.0 + 0.1;
			++n;
			glColor4f(shade, shade, (*fi)->data().mObjs[j].mDerived ? 1.0 : 0.0, 1.0);

			double	x1 = CGAL::to_double((*fi)->data().mObjs[j].mLocation.x());
			double	y1 = CGAL::to_double((*fi)->data().mObjs[j].mLocation.y());
			double r = (*fi)->data().mObjs[j].mHeading;

			double	w = 0.5 * gRepTable[gRepFeatureIndex[(*fi)->data().mObjs[j].mRepType]].width_min;
			double	h = 0.5 * gRepTable[gRepFeatureIndex[(*fi)->data().mObjs[j].mRepType]].depth_min;

			float x_scale = /*(screenWidth / mapWidth) */ 1.0 /  (DEG_TO_MTR_LAT * cos (y1 * DEG_TO_RAD));
			float y_scale = /*(screenHeight / mapHeight) */ 1.0 /  (DEG_TO_MTR_LAT   );

//			x1 = screenLeft + ((x1 - mapWest) * screenWidth / mapWidth);
//			y1 = screenBottom + ((y1 - mapSouth) * screenHeight / mapHeight);

			glPushMatrix();
			glTranslatef(x1, y1, 0.0);
			glScalef(x_scale, y_scale, 1.0);
			glRotatef(r, 0, 0, -1);

			glBegin(GL_QUADS);
			glVertex2f(-w, -h);
			glVertex2f(-w,  h);
			glVertex2f( w,  h);
			glVertex2f( w, -h);
			glEnd();
			glPopMatrix();
		}

		for (int j = 0; j < (*fi)->data().mPolyObjs.size(); ++j)
		{
			float shade = (float) (n % 10) / 20.0 + 0.1;
			++n;
			glColor4f(shade, shade, (*fi)->data().mPolyObjs[j].mDerived ? 1.0 : 0.0, 1.0);

			Polygon_2 ccb((*fi)->data().mPolyObjs[j].mShape.outer_boundary());
			glBegin(GL_LINE_LOOP);
			for(int l = 0; l < ccb.size(); ++l)
				glVertex2f(CGAL::to_double(ccb[l].x()),CGAL::to_double(ccb[l].y()));
			glEnd();


			for (Polygon_with_holes_2::Hole_const_iterator h = (*fi)->data().mPolyObjs[j].mShape.holes_begin(); h != (*fi)->data().mPolyObjs[j].mShape.holes_end(); ++h)
			{
				glBegin(GL_LINE_LOOP);
				for(int l = 0; l < h->size(); ++l)
					glVertex2f(
						CGAL::to_double((*h)[l].x()),
						CGAL::to_double((*h)[l].y()));
				glEnd();
			}
		}
	}
#endif

	/******************************************************************************************
	 * DRAW POINT FEATURES ON SELECTED FACES AS WELL AS INDIVIDUAL SELECTED FEATURES
	 ******************************************************************************************/

#if DRAW_FEATURES
	glPointSize(3);
	glColor4f(0.0, 1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	for (vector<Pmwx::Face_handle>::iterator fi = faces.begin();
		fi != faces.end(); ++fi)
	{
		bool	 fsel = faceSel.find(*fi) != faceSel.end();
		for (int j = 0; j < (*fi)->data().mPointFeatures.size(); ++j)
		{
			bool	isel = pointFeatureSel.find(PointFeatureSelection(*fi, j)) != pointFeatureSel.end();
			if (isel || fsel)
				glColor4f(1.0, 1.0, 1.0, 1.0);
			else
				glColor4f(0.0, 0.6, 0.6, 1.0);

			if (isel)
			{
				glEnd();
				glPointSize(4);
				glBegin(GL_POINTS);
			}

			double	x1 = CGAL::to_double((*fi)->data().mPointFeatures[j].mLocation.x());
			double	y1 = CGAL::to_double((*fi)->data().mPointFeatures[j].mLocation.y());
//			x1 = screenLeft + ((x1 - mapWest) * screenWidth / mapWidth);
//			y1 = screenBottom + ((y1 - mapSouth) * screenHeight / mapHeight);
			glVertex2f(x1, y1);
			if (isel)
			{
				glEnd();
				glPointSize(3);
				glBegin(GL_POINTS);
			}
		}
	}
	glEnd();
	glPointSize(1);
#endif

//	glPopMatrix();

}






	/******************************************************************************************
	 * EXTRA JUNK CODe
	 ******************************************************************************************/



#if 0
			THIS CODE DRAWS THE SELECTED POLYGONS CCB IN CYAN AND HOLES IN GREEN, ALLOWING FOR TOPOLOGY DIAGNOSTICS!!

			glColor3f(0.0, 1.0, 1.0);
//			for (GISPolyObjPlacementVector::const_iterator poly = f->data().mPolyObjs.begin(); poly != f->data().mPolyObjs.end(); ++poly)
			if (faceSel.find(f) != faceSel.end())	// HACK
			{
				glLineWidth(3);	// HACK
//				p = poly->where;
//				RemapPolygonCoords(p, screenLeft, screenBottom, screenRight, screenTop, mapWest, mapSouth, mapEast, mapNorth);
				glBegin(GL_LINE_LOOP);
				for (Polygon2::Vertex_iterator v = p.begin(); v != p.end(); ++v)
				{
					glVertex2f(v->x), v->y));
				}
				glEnd();
				glLineWidth(1);	// HACK
			}

// COPY OF HACK

			glColor3f(0.0, 1.0, 0.3);
//			for (GISPolyObjPlacementVector::const_iterator poly = f->data().mPolyObjs.begin(); poly != f->data().mPolyObjs.end(); ++poly)
			for (vector<Polygon2>::iterator iter = pp.begin(); iter != pp.end(); ++iter)
			if (faceSel.find(f) != faceSel.end())	// HACK
			{
				glLineWidth(5);	// HACK
				p = *iter;
//				p = poly->where;
//				RemapPolygonCoords(p, screenLeft, screenBottom, screenRight, screenTop, mapWest, mapSouth, mapEast, mapNorth);
				glBegin(GL_LINE_LOOP);
				for (Polygon2::Vertex_iterator v = p.begin(); v != p.end(); ++v)
				{
					glVertex2f(v->x), v->y));
				}
				glEnd();
				glLineWidth(1);	// HACK
			}
#endif





void		FindFaceTouchesPt(Pmwx& inMap, const Point2& p, vector<Face_handle>& outIDs)
{
	outIDs.clear();
	for(Pmwx::Face_iterator f = inMap.faces_begin(); f != inMap.faces_end(); ++f)
	if(!f->is_unbounded())
	{
//		printf("Scanning poly...\n");
		Polygon2	pol;
		Pmwx::Ccb_halfedge_circulator circ = f->outer_ccb(), stop = f->outer_ccb();
		do {
			pol.push_back(cgal2ben(circ->target()->point()));
//			printf("  pt %lf,%lf\n",CGAL::to_double(circ->target()->point().x()),CGAL::to_double(circ->target()->point().y()));
		} while(++circ != stop);

		if(pol.inside(p))
		{
			bool in_hole = false;
			for(Pmwx::Hole_iterator h = f->holes_begin(); h != f->holes_end(); ++h)
			{
				Polygon2 polh;
				Pmwx::Ccb_halfedge_circulator circ = *h, stop = *h;
				do {
					polh.push_back(cgal2ben(circ->target()->point()));
		//			printf("  pt %lf,%lf\n",CGAL::to_double(circ->target()->point().x()),CGAL::to_double(circ->target()->point().y()));
				} while(--circ != stop);
				if(polh.inside(p))
				{
					in_hole = true;
						break;
				}
			}
			if(!in_hole)
				outIDs.push_back(f);
		}
	}
}

void		FindFaceTouchesRectFast(Pmwx& inMap, const Point2& p1, const Point2& p2, vector<Face_handle>& outIDs)
{
	outIDs.clear();
	Bbox2	sel(p1, p2);
	for(Pmwx::Face_iterator f = inMap.faces_begin(); f != inMap.faces_end(); ++f)
	if(!f->is_unbounded())
	{
		Bbox2	fb;
		Pmwx::Ccb_halfedge_circulator circ = f->outer_ccb(), stop = f->outer_ccb();
		do {
			fb += cgal2ben(circ->target()->point());
		} while(++circ != stop);
		if(sel.overlap(sel))
			outIDs.push_back(f);
	}
}

void		FindFaceFullyInRect(Pmwx& inMap, const Point2& p1, const Point2& p2, vector<Face_handle>& outIDs)
{
	outIDs.clear();
	Bbox2	sel(p1,p2);
	for(Pmwx::Face_iterator f = inMap.faces_begin(); f != inMap.faces_end(); ++f)
	if(!f->is_unbounded())
	{
		bool ok = true;
		Pmwx::Ccb_halfedge_circulator circ = f->outer_ccb(), stop = f->outer_ccb();
		do {
			if (!sel.contains(cgal2ben(circ->target()->point())))
			{
				ok=false;
				break;
			}
		} while(++circ != stop);
		if(ok)
			outIDs.push_back(f);
	}
}

void		FindHalfedgeTouchesRectFast(Pmwx& inMap, const Point2& p1, const Point2& p2, vector<Halfedge_handle>& outIDs)
{
	outIDs.clear();
	Bbox2 sel(p1,p2);
	for(Pmwx::Edge_iterator e = inMap.edges_begin(); e != inMap.edges_end(); ++e)
	{
		Bbox2	ebox(cgal2ben(e->source()->point()),cgal2ben(e->target()->point()));
		if(sel.overlap(ebox))
			outIDs.push_back(e);
	}
}

void		FindHalfedgeFullyInRect(Pmwx& inMap, const Point2& p1, const Point2& p2, vector<Halfedge_handle>& outIDs)
{
	outIDs.clear();
	Bbox2 sel(p1,p2);
	for(Pmwx::Edge_iterator e = inMap.edges_begin(); e != inMap.edges_end(); ++e)
	{
		Bbox2	ebox(cgal2ben(e->source()->point()),cgal2ben(e->target()->point()));
		if(sel.contains(ebox))
			outIDs.push_back(e);
	}
}


void		FindVerticesTouchesPt(Pmwx& inMap, const Point2& p, vector<Vertex_handle>& outIDs)
{
	Point_2 pp(ben2cgal(p));
	outIDs.clear();
	for(Pmwx::Vertex_iterator v = inMap.vertices_begin(); v != inMap.vertices_end(); ++v)
	if(pp==v->point())
		outIDs.push_back(v);
}

void		FindVerticesTouchesRect(Pmwx& inMap, const Point2& p1, const Point2& p2, vector<Vertex_handle>& outIDs)
{
	Bbox2 sel(p1,p2);
	outIDs.clear();
	for(Pmwx::Vertex_iterator v = inMap.vertices_begin(); v != inMap.vertices_end(); ++v)
	if(sel.contains(cgal2ben(v->point())))
		outIDs.push_back(v);
}
