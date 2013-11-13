/***************************************************************************
 tmcoordconvert
 -------------------
 copyright            : (C) 2013 Lucien Schreiber 
 email                : lucien.schreiber at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tmcoordconvert.h"

// EPSG 3857, google and bing projection system
wxString tmCoordConvert::m_ProjTextGoogle = _T("+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +wktext  +no_defs");

// ESPG 21781, CH1903 / LV03 Swiss projection
wxString tmCoordConvert::m_ProjTextSwiss = _T("+proj=somerc +lat_0=46.95240555555556 +lon_0=7.439583333333333 +k_0=1 +x_0=600000 +y_0=200000 +ellps=bessel +towgs84=674.4,15.1,405.3,0,0,0,0 +units=m +no_defs");

// ESPG 4326, WGS projection
wxString tmCoordConvert::m_ProjTextWGS84 = _T(" +proj=longlat +datum=WGS84 +no_defs  ");



OGRSpatialReference * tmCoordConvert::_CreateSpatialRef(PRJDEF_PROJ_TYPE proj) {
    OGRSpatialReference * mySpatRef = new OGRSpatialReference();
    switch (proj) {
        case PROJ_SWISSPROJ:
            mySpatRef->importFromProj4(m_ProjTextSwiss.mb_str(wxConvUTF8));
            break;
            
            case PROJ_WORLDWGS84:
            mySpatRef->importFromProj4(m_ProjTextWGS84.mb_str(wxConvUTF8));
            break;
            
        default:
            wxLogError(_("Unsupported projection!"));
            wxDELETE (mySpatRef);
            break;
    }
    return mySpatRef;
}



OGRSpatialReference * tmCoordConvert::_CreateSpatialRefGoogle() {
    OGRSpatialReference * mySpatRef = new OGRSpatialReference();
    mySpatRef->importFromProj4(m_ProjTextGoogle.mb_str(wxConvUTF8));
    return mySpatRef;
}


wxRealPoint tmCoordConvert::_Transform ( OGRSpatialReference * refin, OGRSpatialReference * refout, const wxRealPoint & in){
    if (refin == NULL || refout == NULL) {
        wxLogError(_("Empty projection reference found!"));
        return wxRealPoint (wxNOT_FOUND, wxNOT_FOUND);
    }
    
    double myPtX = in.x;
    double myPtY = in.y;
    
    OGRCoordinateTransformation * myTransform = OGRCreateCoordinateTransformation (refin , refout);
    myTransform->Transform(1, &myPtX, &myPtY, NULL);
    
    wxRealPoint myOutPt (myPtX, myPtY);
    return myOutPt;
}


tmCoordConvert::tmCoordConvert(PRJDEF_PROJ_TYPE projtype) {
    m_ProjType = projtype;
}



tmCoordConvert::~tmCoordConvert() {
}



wxRealPoint tmCoordConvert::GetPointWGS(const wxRealPoint & in) {
    if (m_ProjType == PROJ_WORLDWGS84) {
        return in;
    }
    
    OGRSpatialReference * myInRef = _CreateSpatialRef(m_ProjType);
    wxASSERT(myInRef);
    OGRSpatialReference * myOutRef = _CreateSpatialRef(PROJ_WORLDWGS84);
    wxASSERT(myOutRef);
    return _Transform(myInRef, myOutRef, in);
}



wxRealPoint tmCoordConvert::GetPointGoogle(const wxRealPoint & in) {
    return _Transform(_CreateSpatialRef(m_ProjType), _CreateSpatialRefGoogle(), in);
}
