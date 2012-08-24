/***************************************************************************
 tmsymbolrule.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber 
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

#include "tmsymbolrule.h"
#include "tmsymbolvectorline.h"
#include "tmsymbolvectorpoint.h"
#include "tmsymbolvectorline.h"
#include "tmsymbolvectorpolygon.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY (tmSymbolRuleArray);

tmSymbolVector  * tmSymbolRule::_NewSymbolVectorBasedOnSpatType(TM_GIS_SPATIAL_TYPES spatialtype) {
    tmSymbolVector * mySymbol = NULL;
    switch (spatialtype) {
        case LAYER_SPATIAL_LINE:
            mySymbol = (tmSymbolVector*) new tmSymbolVectorLine();
            break;
            
        case LAYER_SPATIAL_POINT:
            mySymbol = (tmSymbolVector*) new tmSymbolVectorPoint();
            break;
            
        case LAYER_SPATIAL_POLYGON:
            mySymbol = (tmSymbolVector*) new tmSymbolVectorPolygon();
            break;
            
        default:
            wxLogError(_("Unsupported layer spatial type!"));
            break;
    }
    wxASSERT(mySymbol);
    return mySymbol;
}



tmSymbolRule::tmSymbolRule(TM_GIS_SPATIAL_TYPES spatialtype, tmSymbolVector * symbol) {
    SetRuleName(wxEmptyString);
    SetActive(true);
    SetAttributFilter(wxEmptyString);
    m_SpatialType = spatialtype;
    if (symbol != NULL) {
        m_SymbolData = symbol;
    }
    else{
        m_SymbolData = _NewSymbolVectorBasedOnSpatType(GetSpatialType());
    }
    wxASSERT(m_SymbolData);
}



tmSymbolRule::~tmSymbolRule() {
    wxDELETE(m_SymbolData);
}



tmSymbolRule::tmSymbolRule(tmSymbolRule & source) {
    m_SpatialType = source.GetSpatialType();
    SetAttributFilter(source.GetAttributFilter());
    SetRuleName(source.GetRuleName());
    SetActive(source.IsActive());
    m_SymbolData = _NewSymbolVectorBasedOnSpatType(m_SpatialType);
    *m_SymbolData = *(source.GetSymbolData());
}



tmSymbolRule::tmSymbolRule(const tmSymbolRule & source) {
    m_SpatialType = source.GetSpatialType();
    SetAttributFilter(source.GetAttributFilter());
    SetRuleName(source.GetRuleName());
    SetActive(source.IsActive());
    m_SymbolData = _NewSymbolVectorBasedOnSpatType(m_SpatialType);
    *m_SymbolData = *(source.GetSymbolData());
}


tmSymbolRule & tmSymbolRule::operator = (const tmSymbolRule & source){
    m_SpatialType = source.GetSpatialType();
    SetAttributFilter(source.GetAttributFilter());
    SetRuleName(source.GetRuleName());
    SetActive(source.IsActive());
    m_SymbolData = _NewSymbolVectorBasedOnSpatType(m_SpatialType);
    *m_SymbolData = *(source.GetSymbolData());
    return * this;
}




wxBrush tmSymbolRule::GetBrush() {
    wxBrush myBrush = *wxBLACK_BRUSH;
    switch (GetSpatialType()) {
        case LAYER_SPATIAL_LINE:
        case LAYER_SPATIAL_POINT:
            break;

        case LAYER_SPATIAL_POLYGON:
        {
            tmSymbolVectorPolygon * mySPoly = (tmSymbolVectorPolygon*) GetSymbolData();
            myBrush.SetColour(mySPoly->GetColourWithTransparency(mySPoly->GetFillColour(), mySPoly->GetTransparency()));
            myBrush.SetStyle(mySPoly->GetFillStyle());
        }
            break;
            
        default:
            wxLogError(_("spatial layer type isn't supported"));
            break;
    }
    return myBrush;
}



wxPen tmSymbolRule::GetPen() {
    wxPen myPen = *wxBLACK_PEN;
    switch (GetSpatialType()) {
        case LAYER_SPATIAL_LINE:
        {
            tmSymbolVectorLine * mySLine = (tmSymbolVectorLine*) GetSymbolData();
            myPen.SetWidth(mySLine->GetWidth());
            myPen.SetColour(mySLine->GetColourWithTransparency(mySLine->GetColour(), mySLine->GetTransparency()));
            myPen.SetStyle(mySLine->GetShape());
        }
            break;
            
        case LAYER_SPATIAL_POINT:
        {
            tmSymbolVectorPoint * mySPoint = (tmSymbolVectorPoint*) GetSymbolData();
            myPen.SetWidth(mySPoint->GetRadius());
            myPen.SetColour(mySPoint->GetColourWithTransparency(mySPoint->GetColour(), mySPoint->GetTransparency()));
        }
            break;
            
        case LAYER_SPATIAL_POLYGON:
        {
            tmSymbolVectorPolygon * mySPoly = (tmSymbolVectorPolygon*) GetSymbolData();
            myPen.SetWidth(mySPoly->GetBorderWidth());
            myPen.SetColour(mySPoly->GetColourWithTransparency(mySPoly->GetBorderColour(), mySPoly->GetTransparency()));
        }
            break;
            
        default:
            wxLogError(_("spatial layer type isn't supported"));
            break;
    }
    return myPen;
}


void tmSymbolRule::SetActive(bool value) {
  m_Active = value;
}



void tmSymbolRule::SetAttributFilter(wxString value) {
  m_AttributFilter = value;
}



void tmSymbolRule::SetRuleName(wxString value) {
  m_RuleName = value;
}





