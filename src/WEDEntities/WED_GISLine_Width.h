#ifndef WED_GISLINE_WIDTH_H
#define WED_GISLINE_WIDTH_H

#include "WED_GISLine.h"
#include "IGIS.h"

class	WED_GISLine_Width : public WED_GISLine, public virtual IGISLine_Width{

DECLARE_INTERMEDIATE(WED_GISLine_Width)

public:

	// IGISEntity
	virtual	GISClass_t		GetGISClass		(void				 ) const;

	// IGISLine_Width
	virtual	double	GetWidth (void		 ) const;
	virtual	void	SetWidth (double width)      ;
		

private:

	WED_PropDoubleText		width;
	
};

#endif /* WED_GISLINE_WIDTH_H */
