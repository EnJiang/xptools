#include "WED_GISLine_Width.h"
#include "GISUtils.h"
#include "XESConstants.h"

START_CASTING(WED_GISLine_Width)
IMPLEMENTS_INTERFACE(IGISEntity)
IMPLEMENTS_INTERFACE(IGISPointSequence)
IMPLEMENTS_INTERFACE(IGISLine)
IMPLEMENTS_INTERFACE(IGISLine_Width)
INHERITS_FROM(WED_GISLine)
END_CASTING

WED_GISLine_Width::WED_GISLine_Width(WED_Archive * parent, int id) : 
	WED_GISLine(parent, id),
	width(this,"width","GIS_lines_heading", "width", 50.0)
{
}

WED_GISLine_Width::~WED_GISLine_Width()
{
}

GISClass_t		WED_GISLine_Width::GetGISClass		(void				 ) const
{
	return gis_Line_Width;
}

double	WED_GISLine_Width::GetWidth (void		 ) const
{
	return width.value;
}

void	WED_GISLine_Width::SetWidth (double w)
{
	if (w != width.value)
	{
		StateChanged();
		width.value = w;
	}
}

void	WED_GISLine_Width::GetCorners(Point2 corners[4]) const
{
	Point2		p1, p2;
	GetSource()->GetLocation(p1);
	GetTarget()->GetLocation(p2);
	Vector2	dir(p1,p2);	
	dir.dx *= cos((p1.y + p2.y) * 0.5 * DEG_TO_RAD);
	dir.normalize();
	Vector2 right(dir.perpendicular_cw());
	Point2 zero;
		
	corners[0] = zero - right * GetWidth() * 0.5;
	corners[1] = zero - right * GetWidth() * 0.5;
	corners[2] = zero + right * GetWidth() * 0.5;
	corners[3] = zero + right * GetWidth() * 0.5;
	
	MetersToLLE(p1, 1, corners  );
	MetersToLLE(p2, 2, corners+1);
	MetersToLLE(p1, 1, corners+3);
}
