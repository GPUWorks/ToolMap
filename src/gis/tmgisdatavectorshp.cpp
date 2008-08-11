/***************************************************************************
								tmgisdatavectorshp.cpp
                    class for dealing with vector SHP data
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

#include "tmgisdatavectorshp.h"



tmGISDataVectorSHP::tmGISDataVectorSHP()
{
	m_Datasource = NULL;
	m_Layer = NULL;
}



tmGISDataVectorSHP::~tmGISDataVectorSHP()
{
	// safe destroy the datasource if needed
	if (m_Datasource)
		OGRDataSource::DestroyDataSource(m_Datasource);
}



bool tmGISDataVectorSHP::Open (const wxString & filename, bool bReadWrite)
{
	// init parent member values
	tmGISData::Open(filename, bReadWrite);
	
	// convert utf wxString into char *
	//const char* ascii_str = 
	char * buffer = new char [filename.Length()+2];
	strcpy(buffer, (const char*)filename.mb_str(wxConvUTF8));
	
	// open the shapefile and return true if success
	m_Datasource = OGRSFDriverRegistrar::Open(buffer, FALSE );
	if( m_Datasource==NULL)
	{
		wxLogDebug(_T("Unable to open shp : %s"), filename.c_str());
		return FALSE;
	}
	
	
	
	m_Layer = m_Datasource->GetLayer(0);
	wxASSERT (m_Layer);
	
	delete buffer;
	return TRUE;
}



tmRealRect tmGISDataVectorSHP::GetMinimalBoundingRectangle()
{
	wxASSERT(m_Layer);
	
	OGREnvelope myEnveloppe;
	OGRErr myError = m_Layer->GetExtent(&myEnveloppe, TRUE);
	if (myError == OGRERR_FAILURE)
	{
		wxLogDebug(_T("Unable to compute extend for %s"), GetShortFileName().c_str());
		return tmRealRect(0,0,0,0);
	}
	
	return tmRealRect(myEnveloppe.MinX, myEnveloppe.MinY,
					  myEnveloppe.MaxX, myEnveloppe.MaxY);

}




TM_GIS_SPATIAL_TYPES tmGISDataVectorSHP::GetSpatialType ()
{
	OGRGeometry *poGeometry;
	OGRFeature *poFeature;
	
	wxASSERT(m_Layer);
	
	// computing features count, not able to know the
	// spatial type if no features are present.
	if (m_Layer->GetFeatureCount () <= 0)
	{
		wxLogError(_("Unable to add the %s layer, layer is empty"),
				   GetShortFileName().c_str());
		return LAYER_SPATIAL_UNKNOWN;
	}
	
	// computing layer type (point, line, polygon or unknown)
	if ((poFeature = m_Layer->GetNextFeature()) == NULL)
	{
		wxLogError(_("Unable to read feature from : %s, layer may be corrupted"),
				   GetShortFileName().c_str());
		return LAYER_SPATIAL_UNKNOWN;
	}
		
	
	poGeometry = poFeature->GetGeometryRef();
	if( poGeometry != NULL)
		{	
			switch (wkbFlatten(poGeometry->getGeometryType()))
			{
				case wkbLineString:
					return LAYER_SPATIAL_LINE;
					break;
				case wkbPoint:
					return LAYER_SPATIAL_POINT;
					break;
				case wkbPolygon:
					return LAYER_SPATIAL_POLYGON;
					break;
				default:
					return LAYER_SPATIAL_UNKNOWN;
					break;
			}
		}
	
	
	wxLogDebug(_T("Error getting spatial layer type for : %s"), GetShortFileName().c_str());
	return LAYER_ERR;
}



