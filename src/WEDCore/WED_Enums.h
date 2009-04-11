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

ENUM_DOMAIN(Airport_Type, "Airport Type")
	ENUM(type_Airport,	"Airport", apt_airport)
	ENUM(type_Heliport,	"Heliport",apt_heliport)
	ENUM(type_Seaport,	"Seaport", apt_seaport)

ENUM_DOMAIN(Edge_Lights, "Runway Edge Light Types")

	ENUM(edge_None, "None", apt_edge_none)
	ENUM(edge_LIRL, "LIRL", apt_edge_LIRL)
	ENUM(edge_MIRL,	"MIRL",	apt_edge_MIRL)
	ENUM(edge_HIRL,	"HIRL", apt_edge_HIRL)

ENUM_DOMAIN(REIL_Lights,"Runway End Identifier Lights")
	ENUM(reil_None,"None",apt_reil_none)
	ENUM(reil_Omni,"Omnidirectional REIL", apt_reil_omni)
	ENUM(reil_Uni,"Unidirectional REIL",apt_reil_uni)

ENUM_DOMAIN(Heli_Lights,"Helipad Edge Lights")
	ENUM(heli_None,"None",apt_heli_edge_none)
	ENUM(heli_Yellow,"Yellow",apt_heli_edge_yellow)
//	ENUM(heli_White,"White", 2)
//	ENUM(heli_Red,"Red",3)

ENUM_DOMAIN(Sign_Size,"Sign Size")
	ENUM(size_SmallTaxi,"Small Taxiway", apt_sign_small)
	ENUM(size_MediumTaxi,"Medium Taxiway", apt_sign_medium)
	ENUM(size_LargeTaxi,"Large Taxiway", apt_sign_large)
	ENUM(size_LargeRemaining,"Large Distance Remaining",apt_sign_large_distance)
	ENUM(size_SmallRemaining,"Small Distance Remaining", apt_sign_small_distance)

ENUM_DOMAIN(Sign_Style,"Sign Style")
	ENUM(style_Default,"Default",apt_sign_style_default)

ENUM_DOMAIN(Surface_Type,"Surface Type")
	ENUM(surf_Asphalt,"Asphalt",apt_surf_asphalt)
	ENUM(surf_Concrete,"Concrete",apt_surf_concrete)
	ENUM(surf_Grass,"Grass",apt_surf_grass)
	ENUM(surf_Dirt,"Dirt",apt_surf_dirt)
	ENUM(surf_Gravel,"Gravel",apt_surf_gravel)
	ENUM(surf_Lake,"Dry Lakebed",apt_surf_dry_lake)
	ENUM(surf_Water,"Water",apt_surf_water)
	ENUM(surf_Snow,"Snow",apt_surf_ice)
	ENUM(surf_Trans,"Transparent",apt_surf_transparent)

ENUM_DOMAIN(Light_Fixt,"Light Fixtures")
	ENUM(light_VASI,"VASI", apt_gls_vasi)
	ENUM(light_PAPI_Left,"PAPI (Left Side)",apt_gls_papi_left)
	ENUM(light_PAPI_Right,"PAPI (Right Side)",apt_gls_papi_right)
	ENUM(light_PAPI20,"PAPI (20 degrees)",apt_gls_papi_20)
	ENUM(light_VASI_Tri,"VASI (Tri-Color)",apt_gls_vasi_tricolor)
	ENUM(light_WigWag,"Wig-Wags",apt_gls_wigwag)

ENUM_DOMAIN(Light_App,"Approach Lights")
	ENUM(app_None,		"None",						apt_app_none)
	ENUM(app_ALSFI,		"ALSF-I",					apt_app_ALSFI)
	ENUM(app_ALSFII,	"ALSF-II",					apt_app_ALSFII)
	ENUM(app_Calvert,	"Calvert",					apt_app_CALVERTI)
	ENUM(app_Calvert2,	"Calvert-II",				apt_app_CALVERTII)

	ENUM(app_SSALR,		"SSALR",					apt_app_SSALR)
	ENUM(app_SSALF,		"SSALF",					apt_app_SSALF)
	ENUM(app_SALS,		"SALS",						apt_app_SALS)

	ENUM(app_MALSR,		"MALSR",					apt_app_MALSR)
	ENUM(app_MALSF,		"MALSF",					apt_app_MALSF)
	ENUM(app_MALS,		"MALS",						apt_app_MALS)

	ENUM(app_ODALS,		"ODALS",					apt_app_ODALS)
	ENUM(app_RAIL,		"RAIL",						apt_app_RAIL)

ENUM_DOMAIN(Shoulder_Type,"Shoulder Type")
	ENUM(shoulder_None,			"None",				apt_shoulder_none)
	ENUM(shoulder_Asphalt,		"Asphalt",			apt_shoulder_asphalt)
	ENUM(shoulder_Concrete,		"Concrete",			apt_shoulder_concrete)

ENUM_DOMAIN(Runway_Markings,"Runway Markings")
	ENUM(mark_None,				"None",						apt_mark_none)
	ENUM(mark_Visual,			"Visual",					apt_mark_visual)
	ENUM(mark_NonPrecis,		"Non-Precision (US)",		apt_mark_non_precision)
	ENUM(mark_Precis,			"Precision (US)",			apt_mark_precision)
	ENUM(mark_NonPrecisUK,		"Non-Precision (UK)",		apt_mark_non_precision_UK)
	ENUM(mark_PrecisUK,			"Precision (UK)",			apt_mark_precision_UK)

ENUM_DOMAIN(Helipad_Markings,"Helipad Markings")
	ENUM(heli_Mark_Default,		"Default",					apt_mark_heli_default)

ENUM_DOMAIN(Airport_Beacon,"Airport Beacon Type")
	ENUM(beacon_Airport,		"Airport",					apt_beacon_airport)
	ENUM(beacon_Seaport,		"Seaport",					apt_beacon_seaport)
	ENUM(beacon_Heliport,		"Heliport",					apt_beacon_heliport)
	ENUM(beacon_MilitaryAirport,"Military Airport",			apt_beacon_military)

ENUM_DOMAIN(LinearFeature, "Linear Markings")
	ENUM(line_SolidYellow,		"Solid Yellow",					apt_line_solid_yellow)
	ENUM(line_BrokenYellow,		"Broken Yellow",				apt_line_broken_yellow)
	ENUM(line_DoubleSolidYellow,"Double Solid Yellow",			apt_line_double_solid_yellow)
	ENUM(line_RunwayHold,		"Runway Hold",					apt_line_runway_hold)
	ENUM(line_OtherHold,		"Other Hold",					apt_line_other_hold)
	ENUM(line_ILSHold,			"ILS Hold",						apt_line_ils_hold)
	ENUM(line_ILSCriticalCenter,"ILS Critical Centerline",		apt_line_ils_center)
	ENUM(line_WideBrokenSingle,	"Wide Broken Single Yellow",	apt_line_wide_broken_yellow)
	ENUM(line_WideBrokenDouble,	"Wide Broken Double Yellow",	apt_line_wide_double_broken_yellow)

	ENUM(line_SolidWhite,		"Solid White (Roadway)",				apt_line_solid_white)
	ENUM(line_Chequered,		"Chequered White (Roadway)",			apt_line_chequered_white)
	ENUM(line_BrokenWhite,		"Broken White (Roadway)",				apt_line_broken_white)

	ENUM(line_BSolidYellow,		"Solid Yellow (Black)",					apt_line_Bsolid_yellow)
	ENUM(line_BBrokenYellow,	"Broken Yellow (Black)",				apt_line_Bbroken_yellow)
	ENUM(line_BDoubleSolidYellow,"Double Solid Yellow (Black)",			apt_line_Bdouble_solid_yellow)
	ENUM(line_BRunwayHold,		"Runway Hold (Black)",					apt_line_Brunway_hold)
	ENUM(line_BOtherHold,		"Other Hold (Black)",					apt_line_Bother_hold)
	ENUM(line_BILSHold,			"ILS Hold (Black)",						apt_line_Bils_hold)
	ENUM(line_BILSCriticalCenter,"ILS Critical Centerline (Black)",		apt_line_Bils_center)
	ENUM(line_BWideBrokenSingle,"Wide Broken Single Yellow (Black)",	apt_line_Bwide_broken_yellow)
	ENUM(line_BWideBrokenDouble,"Wide Broken Double Yellow (Black)",	apt_line_Bwide_double_broken_yellow)

	ENUM(line_TaxiCenter,		"Taxiway Centerline Lights (Green)",	apt_light_taxi_centerline)
	ENUM(line_TaxiEdge,			"Taxiway Edge Lights (Blue)",			apt_light_taxi_edge)
	ENUM(line_HoldLights,		"Hold Short Lights (Amber)",			apt_light_hold_short)
	ENUM(line_HoldLightsPulse,	"Hold Short Lights (Pulsing Amber)",	apt_light_hold_short_flash)
	ENUM(line_HoldShortCenter,	"Hold Short Centerline Lights (Amber/Green)",apt_light_hold_short_centerline)
	ENUM(line_BoundaryEdge,		"Boundary Edge Lights (Red)",			apt_light_bounary)

ENUM_DOMAIN(ATCFrequency, "ATC Frequency")
	ENUM(atc_AWOS,				"AWOS",									apt_freq_awos)
	ENUM(atc_CTAF,				"CTAF",									apt_freq_ctaf)
	ENUM(atc_Delivery,			"Clearance Delivery",					apt_freq_del)
	ENUM(atc_Ground,			"Ground Control",						apt_freq_gnd)
	ENUM(atc_Tower,				"Tower",								apt_freq_twr)
	ENUM(atc_Approach,			"Approach",								apt_freq_app)
	ENUM(atc_Departure,			"Departure",							apt_freq_dep)

ENUM_DOMAIN(ExclusionTypes, "Exclusion Types")
	ENUM(exclude_Obj,			"Objects",								0)
	ENUM(exclude_Fac,			"Facades",								1)
	ENUM(exclude_For,			"Forests",								2)
	ENUM(exclude_Bch,			"Beaches",								3)
	ENUM(exclude_Net,			"Road Networks",						4)
	ENUM(exclude_Lin,			"Lines",								5)
	ENUM(exclude_Pol,			"Draped Polygons",						6)
	ENUM(exclude_Str,			"String",								7)
