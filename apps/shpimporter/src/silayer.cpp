/***************************************************************************
 silayer.cpp
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

#include "silayer.h"
#include "siparam.h"

siLayer::siLayer(const wxString & layerpath, DataBase * database) {
    wxASSERT(database);
    m_Database = database;
    m_LayerNameIn.SetPath(layerpath);
    m_LayerIndexOut = wxNOT_FOUND;
    m_LayerType = SILAYER_TYPE_UNKNOWN;
}



siLayer::~siLayer() {
}


int siLayer::_GetEmptyBlockStop(int startpos) {
    wxTextFile myFile (m_RuleFileName.GetFullPath());
    if (myFile.Open() == false) {
        return wxNOT_FOUND;
    }
    
    int iStopPos = wxNOT_FOUND;
    siParam myParam;
    for (unsigned int i = startpos; i< myFile.GetLineCount(); i++) {
        if (myParam.IsEmpty(myFile.GetLine(i))== true) {
            iStopPos = i;
            break;
        }
    }
    myFile.Close();
    return iStopPos;
}



bool siLayer::_LoadRuleIntoArray(int start, int stop, wxArrayString * array) {
    if (array == NULL) {
        return false;
    }
    array->Clear();
    
    if (stop < start) {
        wxLogError(_("Unable to load data into array, start and stop incorrect!"));
        return false;
    }
    
    wxTextFile myFile (m_RuleFileName.GetFullPath());
    if (myFile.Open() == false) {
        return false;
    }
    
    for (unsigned int i = start; i< stop ; i++) {
        array->Add(myFile.GetLine(i));
    }
    myFile.Close();
    return true;
}



bool siLayer::LoadFromFile(const wxString & filename) {
    m_RuleFileName = wxFileName(filename);
    wxTextFile myFile (filename);
    if(myFile.Open()==false){
        wxLogError(_("Unable to open: %s"), filename);
        return false;
    }
    
    // getting parameters
    bool bError = false;
    siParam myParam;
    wxString myLayerInTxt = myParam.GetParam(myFile.GetLine(0), _T("LAYER_IN"), bError);
    if (bError == true) {
        myFile.Close();
        wxLogError(_("Unable to get LAYER_IN"));
        return false;
    }
    
    wxString myLayerOutTxt = myParam.GetParam(myFile.GetLine(1), _T("LAYER_OUT"), bError);
    if (bError == true) {
        myFile.Close();
        wxLogError(_("Unable to get LAYER_OUT"));
        return false;
    }
    
    wxString myLayerOutTypeTxt = myParam.GetParam(myFile.GetLine(2), _T("LAYER_OUT_TM"), bError);
    if (bError == true) {
        myFile.Close();
        wxLogError(_("Unable to get LAYER_OUT_TM"));
        return false;
    }
    myFile.Close();
    
    // processing and checking parameters
    m_LayerNameIn.SetName(myLayerInTxt);
    m_LayerNameIn.SetExt(_T("shp"));
    if(m_LayerNameIn.Exists() == false){
        wxLogError(_("Layer didn't exists"));
        return false;
    }
    
    m_LayerIndexOut = wxNOT_FOUND;
    if(m_Database->DataBaseQuery(wxString::Format(_T("SELECT LAYER_INDEX FROM thematic_layers WHERE LAYER_NAME =\"%s\""), myLayerOutTxt))==false){
        return false;
    }
    if(m_Database->DataBaseGetNextResult(m_LayerIndexOut)==false){
        return false;
    }
    m_Database->DataBaseClearResults();
    
    if (myLayerOutTypeTxt == _T("generic_points")) {
        m_LayerType = SILAYER_TYPE_POINT;
    } else if (myLayerOutTypeTxt == _T("generic_lines")){
        m_LayerType = SILAYER_TYPE_LINE;
    }else if (myLayerOutTypeTxt == _T("generic_labels")){
        m_LayerType = SILAYER_TYPE_POLYGON;
    }
    wxLogMessage(_("Layer in is: %s (ID: %ld)"), m_LayerNameIn.GetFullName(), m_LayerIndexOut);
    
    // load kind
    int iKindStart = 4;
    int iKindStop = _GetEmptyBlockStop(iKindStart);
    wxLogMessage(_("Kind found starting from %d to %d"), iKindStart, iKindStop);
    
    wxArrayString myKindDef;
    if (_LoadRuleIntoArray(iKindStart, iKindStop, &myKindDef)==false) {
        return false;
    }
    
    if(m_Kind.LoadFromArray(myKindDef,m_Database)==false){
        wxLogError(_("Loading Kind failed!"));
        return false;
    }
    
    return true;
}



bool siLayer::Process() {
    return false;
}


const siKind siLayer::GetKindRef() {
    return m_Kind;
}

