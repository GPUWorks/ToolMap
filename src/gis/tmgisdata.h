/***************************************************************************
								tmgisdata.h
                    Main class for dealing with GIS data
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
    email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// comment doxygen


#ifndef _TM_GISDATA_H_
#define _TM_GISDATA_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#include "gdal_priv.h"		// GDAL ACCES C++
#include "ogrsf_frmts.h"	// OGR accessing
#include "geos_c.h"			// GEOS accessing
#include <wx/filename.h>	// for dealing with filename class
#include "tmgisscale.h"		// for dealing with scale and real rectangle.




/***************************************************************************//**
 @brief Main class for dealing with GIS data
 @details This class may be used for accessing GIS data using GDAL / OGR driver
 see also : http://www.gdal.org/
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
class tmGISData : public wxObject
	{
	private:
		wxString		m_ShortFileName;
				
		void InitMemberValue();
		
	
	protected:
	public:
		// ctor and dtor
		tmGISData();
		~tmGISData();
		
		// static functions for init
		static void InitGISDrivers (bool bRaster = TRUE, bool bVector = TRUE);
		static wxString GetAllSupportedGISFormatsWildcards();
		static wxArrayString GetAllSupportedGISFormatsExtensions();
		static tmGISData * CreateGISBasedOnType (const int & gis_format_index);
		static tmGISData * CreateGISBasedOnExt (const wxString & extension);
		
		
		// gis function
		virtual bool Open (const wxString & filename, bool bReadWrite = TRUE);
		virtual tmRealRect GetMinimalBoundingRectangle(){return tmRealRect(0,0,0,0);}
		
		
		// misc function
		wxString GetShortFileName (){return m_ShortFileName;}
		
		
	};





#endif
