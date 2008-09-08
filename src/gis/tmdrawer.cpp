/***************************************************************************
								tmdrawer.cpp
						Draws GIS data into bitmap
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

#include "tmdrawer.h"


tmDrawer::tmDrawer()
{
	m_bmp = NULL;
	m_IsInitialised = FALSE;
}



tmDrawer::~tmDrawer()
{

}



void tmDrawer::InitDrawer(wxBitmap * bitmap, tmGISScale & scale)
{
	m_bmp = bitmap;
	m_scale = scale;
	
	if (m_bmp && m_bmp->IsOk())
		m_IsInitialised = TRUE;
	else
	{
		m_IsInitialised = FALSE;
		fprintf(stderr, "%s line %d : Error initing drawer \n ", __FUNCTION__, __LINE__);
	}
}


/***************************************************************************//**
 @brief Draw the max extent
 @details Draw a red rectangle corresponding to the maximum extent of all
 visible layers
 @param bitmap pointer to a valid bitmap
 @param scale actual values of scale and zoom
 @return  TRUE if bitmap drawn, FALSE if bitmap not valid
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool tmDrawer::DrawExtentIntoBitmap (int width, const wxColour & col)
{
	// check for initialisation
	if(!m_IsInitialised)
	{
		wxString sFunction = wxString::FromAscii(__FUNCTION__);
		wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
													   sFunction.c_str(), __LINE__); 
		wxString sErrMsg = wxString::Format(_T("%s Drawer not initialised"),
											sFunctionLineError.c_str());
		
		wxASSERT_MSG(0,sErrMsg);
	}
	
	
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	
	wxPen myPen (col, width);
	temp_dc.SetPen(myPen);
	temp_dc.SetBackground(*wxWHITE);
	
	
	tmRealRect myRealExt = m_scale.GetMaxLayersExtent();
	
	wxPoint pts[5];
	pts[0] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_min));
	pts[1] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_max, myRealExt.y_min));
	pts[2] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_max, myRealExt.y_max));
	pts[3] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_max));
	pts[4] = m_scale.RealToPixel(wxRealPoint(myRealExt.x_min, myRealExt.y_min));
	
	temp_dc.DrawLines(5, pts);
	
	
	wxBitmap nullbmp;
	temp_dc.SelectObject(nullbmp);
	
	return TRUE;
}



bool tmDrawer::Draw (tmLayerProperties * itemProp)
{
	switch (itemProp->m_LayerSpatialType)
	{
		case LAYER_SPATIAL_LINE:
			DrawLines(itemProp);
			break;
		default:
			return FALSE;
			break;
	}
	
	return TRUE;	
}


bool tmDrawer::DrawLines(tmLayerProperties * itemProp)
{
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(*m_bmp);
	
	wxPen myPen (wxColour(*wxBLUE),2);
	temp_dc.SetPen(myPen);
	temp_dc.SetBackground(*wxWHITE);
	
	
	//tmRealRect myRealExt = m_scale.GetMaxLayersExtent();
	
	wxPoint pts[2];
	pts[0] = wxPoint(50,50);
	pts[1] = wxPoint(200,200);

	
	temp_dc.DrawLines(2, pts);
	wxLogDebug(_T("Lines drawn"));
	
	
	wxBitmap nullbmp;
	temp_dc.SelectObject(nullbmp);
	
	return TRUE;
	
	
}
