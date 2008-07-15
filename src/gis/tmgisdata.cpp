/***************************************************************************
								tmgisdata.cpp
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

#include "tmgisdata.h"
#include "tmgisdatavector.h"
#include "tmgisdataraster.h"

/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
tmGISData::tmGISData()
{
	InitMemberValue();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
tmGISData::~tmGISData()
{

}



/***************************************************************************//**
 @brief Init class members
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
void tmGISData::InitMemberValue()
{
	m_ShortFileName = _T("");
}



/***************************************************************************//**
 @brief Init the GDAL / OGR driver
 @details Call this function during program startup to allow all GDAL / OGR
 drivers to be loaded.
 @param bRaster Should we load all GDAL rasters drivers
 @param bVector Should we load all OGR vector drivers
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
void tmGISData::InitGISDrivers (bool bRaster, bool bVector)
{
	if (bRaster)
	{
		tmGISDataRaster::InitGISDriversRaster();
		wxLogDebug(_T("GIS Raster drivers loaded"));
	}
	
	if (bVector)
	{
		tmGISDataVector::InitGISDriversVector();
		wxLogDebug(_T("GIS Vector drivers loaded"));
	}
	
	
	wxLogMessage(_("loaded GDAL version : %s"), wxString::FromAscii(GDAL_RELEASE_NAME).c_str());
	wxLogMessage(_("loaded GEOS version : %s"), wxString::FromAscii(GEOS_VERSION).c_str());
}



/***************************************************************************//**
 @brief Return all supported formats
 @details Return the wildcards of all supported formats for loading.
 @param wxString A String containing all supported formats as wildcards that one
 may use in a dialog box for file selection
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
wxString tmGISData::GetAllSupportedGISFormatsWildcards()
{
	wxString myReturnedWildcard = _T("");
	
	myReturnedWildcard.Append(tmGISDataRaster::GetAllRasterGISFormatsWildcards());
	myReturnedWildcard.Append(_T("|"));
	myReturnedWildcard.Append(tmGISDataVector::GetAllVectorGISFormatsWildcards());
	
	return myReturnedWildcard;
}



/***************************************************************************//**
 @brief Create an object of the specified format
 @details Allowed values for gis_format_index are those from the
 #tmGISDATA_RASTER_TYPES and from the #tmGISDATA_VECTOR_TYPES
 @param gis_format_index zero based index of the item type to create
 @return  pointer to an object of the correct type or NULL if something's going
 wrong
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
tmGISData * tmGISData::CreateGISBasedOnType (const int & gis_format_index)
{
	// create pointer depending on the selected format
	if (gis_format_index < tmGISVECTOR_OFFSET)
		return tmGISDataRaster::CreateGISRasterBasedOnType(gis_format_index);
	else
		return tmGISDataVector::CreateGISVectorBasedOnType(gis_format_index);
}



/***************************************************************************//**
 @brief Generic open code
 @details Mainly init member values used by all GIS class
 @param filename path and name of the GIS data to open
 @return  Allways TRUE
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
bool tmGISData::Open (const wxString & filename)
{
	wxFileName myFilename (filename);
	m_ShortFileName = myFilename.GetFullName();
	
	return TRUE;
}



