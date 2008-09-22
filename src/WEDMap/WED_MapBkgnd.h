/* 
 * Copyright (c) 2007, Laminar Research.
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

#ifndef WED_MAPBKGND_H
#define WED_MAPBKGND_H

#include "WED_MapLayer.h"

class	WED_MapBkgnd : public WED_MapLayer {
public:

						 WED_MapBkgnd(GUI_Pane * host, WED_MapZoomerNew * zoomer, IResolver * resolver);
	virtual				~WED_MapBkgnd();

	virtual	void		DrawVisualization(intptr_t inCurrent, GUI_GraphState * g);
	virtual	void		DrawStructure	 (intptr_t inCurrent, GUI_GraphState * g);

	virtual	void		GetCaps(intptr_t& draw_ent_v, intptr_t& draw_ent_s, intptr_t& cares_about_sel);
		
};
		

#endif
