/***************************************************************************
								tmeditmanager.cpp
				Deals with editing data in construction layers
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
    email                : lucien.schreiber a"t crealp dot vs dot ch
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

#include "tmeditmanager.h"
#include "../core/toolmap.h"
#include "tmsymbolvectorline.h"
#include "tmsymbolvectorlinemultiple.h"
#include "tmsymbolvectorpoint.h"
#include "tmsymbolvectorpolygon.h"
#include "../core/tmstatsevent.h"
#include "../database/database_tm.h"		// for database
#include "tmrenderer.h"						// for GIS rendering


#include <wx/listimpl.cpp>
WX_DEFINE_LIST(wxRealPointList);


DEFINE_EVENT_TYPE(tmEVT_FOCUS_RENDERER);


BEGIN_EVENT_TABLE(tmEditManager, wxEvtHandler)
	EVT_COMMAND (wxID_ANY, tmEVT_SNAPPING_UPDATED, tmEditManager::OnSnappingChange)
	EVT_COMMAND (wxID_ANY, tmEVT_VIEW_REFRESHED, tmEditManager::OnViewUpdated)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_START, tmEditManager::OnEditStart)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_STOP, tmEditManager::OnEditStop)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_ENTER, tmEditManager::OnDrawFeatureValidate)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_CUT_LINE, tmEditManager::OnCutLines)
	EVT_COMMAND (wxID_ANY,tmEVT_EV_DISPLAY_VERTEX_COORD, tmEditManager::OnShowVertexPosition)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_CLICK, tmEditManager::OnModifySearch)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_MOVED, tmEditManager::OnModifyMove)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_UP, tmEditManager::OnModifyUp)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_CLICK, tmEditManager::OnDrawUp)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_MOVE, tmEditManager::OnDrawMove)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_DOWN, tmEditManager::OnDrawDown)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_ESC, tmEditManager::OnDrawFeatureEscape)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_MENU,  tmEditManager::OnModifyMenu)
	EVT_COMMAND (wxID_ANY, tmEVT_FOCUS_RENDERER, tmEditManager::OnSetRenderFocus)

	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_ORIENT_DOWN,tmEditManager::OnOrientedPtsDown)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_ORIENT_MOVE,tmEditManager::OnOrientedPtsMove)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_ORIENT_UP,tmEditManager::OnOrientedPtsUp)

	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_SHARED_DOWN, tmEditManager::OnEditSharedDown)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_SHARED_UP, tmEditManager::OnEditSharedUp)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_MODIFY_SHARED_MOVE, tmEditManager::OnEditSharedMove)

	EVT_MENU (tmEM_CONTEXTMENU_LINE_CANCEL,tmEditManager::OnDrawFeatureEscape)
	EVT_MENU (tmEM_CONTEXTMENU_LINE_SAVE, tmEditManager::OnDrawFeatureValidate)
	EVT_MENU (tmEM_CONTEXTMENU_VERTEX_INSERT,tmEditManager::OnMenuInsertVertex)
	EVT_MENU (tmEM_CONTEXTMENU_VERTEX_DELETE,tmEditManager::OnMenuDeleteVertex)

END_EVENT_TABLE()



/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
tmEditManager::tmEditManager(ToolMapFrame * parent,tmTOCCtrl * toc,
							 tmSelectedDataMemory * seldata,
							 tmRenderer * renderer,
							 tmGISScale * scale)
{
	InitMemberValues();
	m_ParentEvt = parent;
	m_TOC = toc;
	m_SelectedData = seldata;
	m_Renderer = renderer;
	m_Scale = scale;
	m_EditStarted = false;
	m_DrawLine.SetSymbology(*wxBLACK, 1);
    m_SelectionColour = *wxRED;
	

	m_ParentEvt->PushEventHandler(this);

	m_GISMemory = new tmGISDataVectorMemory();
    
    m_BezierActualP1 = wxPoint(0,0);
    m_BezierActualP2= wxPoint(0,0);
    m_BezierActualC1= wxPoint(0,0);
    m_BezierActualC2= wxPoint(0,0);
    m_BezierDrawControlPoints = false;
    m_BezierRefreshRect = wxRect(wxDefaultPosition, wxDefaultSize);
    m_BezierModifyIndexPoint = wxNOT_FOUND;
    m_BezierModifyIndexControl = wxNOT_FOUND;
    m_BezierSnappedPointsIndexes.Clear();
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
tmEditManager::~tmEditManager()
{
	m_ParentEvt->PopEventHandler(false);
	//m_ParentEvt->SetEventHandler(m_ParentEvt);
	delete m_GISMemory;
}



/***************************************************************************//**
 @brief Init pointer to default values
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::InitMemberValues()
{
	m_ParentEvt = NULL;
	m_TOC = NULL;
	m_SelectedData = NULL;
	m_Renderer = NULL;
	m_pDB = NULL;
	m_SnapMem = NULL;
	m_INSDELVertex = wxNOT_FOUND;
	m_INSVertexPos = wxRealPoint(-1,-1);
}



/***************************************************************************//**
 @brief User select edit tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::OnToolEdit ()
{
	wxASSERT (m_Renderer);
	m_Renderer->SetTool(tmTOOL_DRAW);
}


void tmEditManager::OnToolBezier() {
    m_Renderer->SetTool(tmTOOL_DRAW_BEZIER);
}


void tmEditManager::OnToolBezierModify(){
    m_Renderer->SetTool(tmTOOL_MODIFY_BEZIER);
    m_BezierActualP2 = wxDefaultPosition;
    m_Renderer->Refresh();
    m_Renderer->Update();
}




void tmEditManager::BezierClick(const wxPoint & mousepos){
    if (m_BezierPoints.GetCount() == m_BezierPointsControl.GetCount()) {
        wxRealPoint myPt = m_Scale->PixelToReal(mousepos);
        if(EMGetSnappingCoord(myPt)==true){
            m_BezierSnappedPointsIndexes.Add(m_BezierPoints.GetCount());
        }
        
        m_BezierPoints.push_back(new wxRealPoint(myPt));
        m_BezierActualP2 = mousepos;
        m_BezierDrawControlPoints = true;
    }
    else
    {
        m_BezierPointsControl.push_back(new wxRealPoint(m_Scale->PixelToReal(mousepos)));
        m_BezierActualC2 = mousepos;
        m_BezierDrawControlPoints = false;
    }
    m_Renderer->Refresh();
    m_Renderer->Update();
}



void tmEditManager::BezierMove (const wxPoint & mousepos){
    if (m_BezierPoints.GetCount() == 0) {
        return;
    }
    
    if (m_BezierPoints.GetCount() > m_BezierPointsControl.GetCount()) {
        m_BezierActualP2 = m_Scale->RealToPixel(*m_BezierPoints.back());
        m_BezierActualC2 = mousepos;
    }
    else {
        m_BezierActualP1 = m_Scale->RealToPixel(*m_BezierPoints.back());
        if (m_BezierPointsControl.GetCount() > 0){
            if (m_BezierPointsControl.GetCount() == 1){
                m_BezierActualC1 =  m_Scale->RealToPixel(*m_BezierPointsControl.front());
            }
            else {
                wxPoint myInvertedC1 = m_Scale->RealToPixel(*m_BezierPointsControl.back());
                m_BezierActualC1 = m_BezierActualP1 - (myInvertedC1 - m_BezierActualP1);
            }
        }
        m_BezierActualP2 = mousepos;
        m_BezierActualC2 = mousepos;
    }
    
    m_Renderer->RefreshRect(m_BezierRefreshRect);
    m_Renderer->Update();
}




void tmEditManager::BezierDraw (wxGCDC * dc){
    if (m_BezierPoints.GetCount() == 0) {
        return;
    }
    if (m_Renderer->GetTool() == tmTOOL_MODIFY_BEZIER) {
        return;
    }
    
    tmLayerProperties * myLayerProperties = m_TOC->GetEditLayer();
    wxASSERT(myLayerProperties);
    tmSymbolVectorLine * mySymbol = static_cast<tmSymbolVectorLine *>(myLayerProperties->GetSymbolRef());
    
    // draw existing bezier
    dc->SetPen(wxPen(m_SelectionColour, mySymbol->GetWidth()));
    wxGraphicsPath path = dc->GetGraphicsContext()->CreatePath();
    wxPoint myFirstPt (m_Scale->RealToPixel(*m_BezierPoints[0]));
    path.MoveToPoint(myFirstPt);
    for (unsigned int i = 1; i< m_BezierPointsControl.GetCount(); i++) {
        wxPoint myLastCPt (m_Scale->RealToPixel(*m_BezierPoints[i-1]) -  (m_Scale->RealToPixel(*m_BezierPointsControl[i-1]) - m_Scale->RealToPixel(*m_BezierPoints[i-1])));
        if (i == 1) {
            myLastCPt  = m_Scale->RealToPixel(*m_BezierPointsControl[i-1]);
        }
        wxPoint myPt (m_Scale->RealToPixel(* m_BezierPoints[i]));
        wxPoint myCPt1 (m_Scale->RealToPixel(*m_BezierPointsControl[i]));
        path.AddCurveToPoint(myLastCPt, myCPt1, myPt);
    }
    dc->GetGraphicsContext()->StrokePath(path);
    
    // draw nodes with different color based on snapping status
    int mySymbolNodeWidth = mySymbol->GetWidth();
    if (mySymbolNodeWidth <= 1) {
        mySymbolNodeWidth = 4;
    }
    else {
        mySymbolNodeWidth = 2.0 * mySymbolNodeWidth;
    }
    
    wxPen myNodeBlackPen = wxPen(*wxBLACK, mySymbolNodeWidth);
    wxPen myNodeGreenPen = wxPen(*wxGREEN, mySymbolNodeWidth);
    for (unsigned int i = 0; i< m_BezierPointsControl.GetCount(); i++) {
        if (m_BezierSnappedPointsIndexes.Index(i) == wxNOT_FOUND) {
            dc->SetPen(myNodeBlackPen);
        }
        else {
            dc->SetPen(myNodeGreenPen);
        }
        
        wxPoint myPt (m_Scale->RealToPixel(* m_BezierPoints[i]));
#ifdef __WXMSW__
        dc->DrawLine (myPt.x , myPt.y, myPt.x + 1, myPt.y + 1);
#else
        dc->DrawLine (myPt.x, myPt.y, myPt.x, myPt.y);
#endif
    }
    
#ifdef __WXMAC__
    dc->SetPen( *wxGREY_PEN );
#else
    dc->SetPen( wxPen( *wxLIGHT_GREY, 2, wxSOLID ) );
#endif
    // draw actual bezier when needed
    if (m_BezierActualP1 != wxPoint(0,0) && m_BezierActualC1 != wxPoint(0,0)){
        wxGraphicsPath path = dc->GetGraphicsContext()->CreatePath();
        path.MoveToPoint(m_BezierActualP1);
        path.AddCurveToPoint(m_BezierActualC1, m_BezierActualC2, m_BezierActualP2);
        dc->GetGraphicsContext()->StrokePath(path);
    }
    
    // draw bezier control when needed
    if (m_BezierDrawControlPoints == true && m_BezierActualC2 != wxPoint(0,0)) {
        dc->DrawLine(m_BezierActualP2, m_BezierActualC2);
        wxPoint myInvertedC2 = m_BezierActualP2 - (m_BezierActualC2 - m_BezierActualP2);
        dc->DrawLine(m_BezierActualP2, myInvertedC2);
        
        dc->SetPen(wxPen(*wxBLACK, 4));
#ifdef __WXMSW__
        dc->DrawLine (m_BezierActualC2.x , m_BezierActualC2.y, m_BezierActualC2.x + 1, m_BezierActualC2.y + 1);
        dc->DrawLine (m_BezierActualP2.x , m_BezierActualP2.y, m_BezierActualP2.x + 1, m_BezierActualP2.y + 1);
        dc->DrawLine (myInvertedC2.x , myInvertedC2.y, myInvertedC2.x + 1, myInvertedC2.y + 1);
#else
        dc->DrawLine (m_BezierActualC2.x, m_BezierActualC2.y, m_BezierActualC2.x, m_BezierActualC2.y);
        dc->DrawLine (m_BezierActualP2.x, m_BezierActualP2.y, m_BezierActualP2.x, m_BezierActualP2.y);
        dc->DrawLine (myInvertedC2.x, myInvertedC2.y, myInvertedC2.x, myInvertedC2.y);
#endif
    }
    
	
    // compute bounding box for refreshing. This is mainly to avoid flickering
	dc->ResetBoundingBox();
	if (m_BezierActualP1 != wxPoint(0,0) && m_BezierActualC1 != wxPoint(0,0)){
        dc->CalcBoundingBox(m_BezierActualP1.x, m_BezierActualP1.y);
        dc->CalcBoundingBox(m_BezierActualC1.x, m_BezierActualC1.y);
        // inverted C1 is never needed for bounding box
    }
    dc->CalcBoundingBox(m_BezierActualP2.x, m_BezierActualP2.y);
    dc->CalcBoundingBox(m_BezierActualC2.x, m_BezierActualC2.y);
    dc->CalcBoundingBox(m_BezierActualP2.x - (m_BezierActualC2.x - m_BezierActualP2.x) ,
                        m_BezierActualP2.y - (m_BezierActualC2.y - m_BezierActualP2.y));
    m_BezierRefreshRect = wxRect(wxPoint(dc->MinX(), dc->MaxY()), wxPoint(dc->MaxX(), dc->MinY()));
    m_BezierRefreshRect.Inflate(wxSize(3,3));
}



void tmEditManager::BezierClear(){
    m_BezierActualP1 = wxPoint(0,0);
    m_BezierActualP2= wxPoint(0,0);
    m_BezierActualC1= wxPoint(0,0);
    m_BezierActualC2= wxPoint(0,0);
    m_BezierDrawControlPoints = false;
    
    m_BezierPoints.DeleteContents(true);
    m_BezierPointsControl.DeleteContents(true);
    m_BezierPoints.Clear();
    m_BezierPointsControl.Clear();
    
    m_BezierRefreshRect = wxRect(wxDefaultPosition, wxDefaultSize);
    m_BezierSnappedPointsIndexes.Clear();
}



void tmEditManager::BezierModifyDraw(wxGCDC * dc){
    if (m_Renderer == NULL) {
        return;
    }
    if (m_Renderer->GetTool() != tmTOOL_MODIFY_BEZIER) {
        return;
    }
    if (m_BezierPoints.GetCount() < 2) {
        return;
    }
    wxASSERT(dc);
    
    tmLayerProperties * myLayerProperties = m_TOC->GetEditLayer();
    wxASSERT(myLayerProperties);
    tmSymbolVectorLine * mySymbol = static_cast<tmSymbolVectorLine *>(myLayerProperties->GetSymbolRef());
    
    // draw existing bezier
    dc->SetPen(wxPen(m_SelectionColour, mySymbol->GetWidth()));
    wxGraphicsPath path = dc->GetGraphicsContext()->CreatePath();
    wxPoint myFirstPt (m_Scale->RealToPixel(*m_BezierPoints[0]));
    path.MoveToPoint(myFirstPt);
    for (unsigned int i = 1; i< m_BezierPointsControl.GetCount(); i++) {
        wxPoint myLastCPt (m_Scale->RealToPixel(*m_BezierPoints[i-1]) -  (m_Scale->RealToPixel(*m_BezierPointsControl[i-1]) - m_Scale->RealToPixel(*m_BezierPoints[i-1])));
        if (i == 1) {
            myLastCPt  = m_Scale->RealToPixel(*m_BezierPointsControl[i-1]);
        }
        wxPoint myPt (m_Scale->RealToPixel(* m_BezierPoints[i]));
        wxPoint myCPt1 (m_Scale->RealToPixel(*m_BezierPointsControl[i]));
        path.AddCurveToPoint(myLastCPt, myCPt1, myPt);
    }
    dc->GetGraphicsContext()->StrokePath(path);
    
    // draw nodes
    dc->SetPen(wxPen(*wxBLACK, 2.0 * mySymbol->GetWidth()));
    for (unsigned int i = 1; i< m_BezierPointsControl.GetCount(); i++) {
        wxPoint myPt (m_Scale->RealToPixel(* m_BezierPoints[i]));
#ifdef __WXMSW__
        dc->DrawLine (myPt.x , myPt.y, myPt.x + 1, myPt.y + 1);
#else
        dc->DrawLine (myPt.x, myPt.y, myPt.x, myPt.y);
#endif
    }
    
    // draw controls
    wxPen myGreyPen (wxPen(*wxLIGHT_GREY, 2, wxSOLID));
#ifdef __WXMAC__
    myGreyPen = wxPen(*wxGREY_PEN);
#endif
    
    wxASSERT(m_BezierPointsControl.GetCount() == m_BezierPoints.GetCount());
    for (unsigned int i = 0; i< m_BezierPoints.GetCount(); i++) {
        wxPoint myPoint = m_Scale->RealToPixel(*m_BezierPoints[i]);
        wxPoint myControl = m_Scale->RealToPixel(*m_BezierPointsControl[i]);
        wxPoint myControlInverted = myPoint - (myControl - myPoint);
 
        dc->SetPen(myGreyPen);
        dc->DrawLine(myPoint, myControl);
        
        dc->SetPen(wxPen(*wxBLACK, 4));
#ifdef __WXMSW__
        dc->DrawLine (myPoint.x , myPoint.y, myPoint.x + 1, myPoint.y + 1);
        dc->DrawLine (myControl.x , myControl.y, myControl.x + 1, myControl.y + 1);
#else
        dc->DrawLine (myPoint.x, myPoint.y, myPoint.x, myPoint.y);
        dc->DrawLine (myControl.x, myControl.y, myControl.x, myControl.y);
#endif
        
        // special case for first and last loop. We draw only one
        // control instead of two.
        if (i == 0 || i == m_BezierPoints.GetCount() -1){
            continue;
        }
        dc->SetPen(myGreyPen);
        dc->DrawLine(myPoint, myControlInverted);
        
        dc->SetPen(wxPen(*wxBLACK, 4));
#ifdef __WXMSW__
        dc->DrawLine (myControlInverted.x , myControlInverted.y, myControlInverted.x + 1, myControlInverted.y + 1);
#else
        dc->DrawLine (myControlInverted.x, myControlInverted.y, myControlInverted.x, myControlInverted.y);
#endif
    }
    
    if (m_BezierModifyIndexControl == wxNOT_FOUND && m_BezierModifyIndexPoint == wxNOT_FOUND) {
        return;
    }

    // Compute Refreshing Rect
    dc->ResetBoundingBox();
    wxPointList myPoints;
    
    int myIndex = m_BezierModifyIndexControl;
    if (m_BezierModifyIndexControl == wxNOT_FOUND) {
        wxASSERT(m_BezierModifyIndexPoint != wxNOT_FOUND);
        myIndex = m_BezierModifyIndexPoint;
    }
    
    wxPoint myPt = m_Scale->RealToPixel(*m_BezierPoints[myIndex]);
    wxPoint myControl = m_Scale->RealToPixel(*m_BezierPointsControl[myIndex]);
    wxPoint myControlInverted = myPt - (myControl - myPt);
    myPoints.push_back(new wxPoint (myPt));
    
    if (myIndex != 0 && myIndex != m_BezierPoints.GetCount() -1){
        myPoints.push_back(new wxPoint (myControl));
        myPoints.push_back(new wxPoint (myControlInverted));
    }
    
    if (myIndex == 0 || myIndex == m_BezierPoints.GetCount() -1) {
        myPoints.push_back(new wxPoint(myControl));
    }

    if (myIndex != 0) {
        wxPoint myPt1 = m_Scale->RealToPixel(*m_BezierPoints[myIndex -1]);
        wxPoint myControl1 = m_Scale->RealToPixel(*m_BezierPointsControl[myIndex -1]);
        wxPoint myControlInverted1 = myPt1 - (myControl1 - myPt1);
        myPoints.push_back(new wxPoint (myPt1));
        myPoints.push_back(new wxPoint (myControlInverted1));
    }
    
    if (myIndex != m_BezierPoints.GetCount() -1) {
        wxPoint myPt2 = m_Scale->RealToPixel(*m_BezierPoints[myIndex + 1]);
        wxPoint myControl2 = m_Scale->RealToPixel(*m_BezierPointsControl[myIndex + 1]);
        myPoints.push_back(new wxPoint (myPt2));
        myPoints.push_back(new wxPoint (myControl2));
    }
    
    for (unsigned int i = 0; i< myPoints.GetCount(); i++) {
        dc->CalcBoundingBox(myPoints[i]->x, myPoints[i]->y);
    }
    myPoints.DeleteContents(true);
    myPoints.Clear();
     
    m_BezierRefreshRect = wxRect(wxPoint(dc->MinX(), dc->MaxY()), wxPoint(dc->MaxX(), dc->MinY()));
    m_BezierRefreshRect.Inflate(wxSize(3,3));
    /*dc->SetPen(myGreyPen);
    dc->SetBrush(*wxTRANSPARENT_BRUSH);
    dc->DrawRectangle(m_BezierRefreshRect);*/
}



// search point or control clicked
void tmEditManager::BezierModifyClickDown (const wxPoint & mousepos){
    if (m_BezierActualP2 != wxDefaultPosition) {
        return;
    }
    m_BezierActualP2 = mousepos;
    m_BezierActualC2 = wxDefaultPosition;
    m_BezierModifyIndexPoint = wxNOT_FOUND;
    m_BezierModifyIndexControl = wxNOT_FOUND;
    m_BezierModifyControlInverted = false;

    wxRect myRect (0,0,3,3);
    myRect = myRect.CentreIn(wxRect(m_BezierActualP2, wxSize(0,0)));
    
    for (unsigned int i = 0 ; i< m_BezierPointsControl.GetCount(); i++) {
        wxPoint myPt = m_Scale->RealToPixel(*m_BezierPointsControl[i]);
        if (myRect.Contains(myPt)){
            m_BezierModifyIndexControl = i;
            return;
        }
    }
    
    for (unsigned int i = 0 ; i< m_BezierPointsControl.GetCount(); i++) {
        wxPoint myControlPt = m_Scale->RealToPixel(*m_BezierPointsControl[i]);
        wxPoint myPt = m_Scale->RealToPixel(*m_BezierPoints[i]);
        wxPoint myControlPtInverted = myPt - (myControlPt - myPt);
        if (myRect.Contains(myControlPtInverted)){
            m_BezierModifyIndexControl = i;
            m_BezierModifyControlInverted = true;
            return;
        }
    }
    
    for (unsigned int i = 0 ; i< m_BezierPoints.GetCount(); i++) {
        wxPoint myPt = m_Scale->RealToPixel(*m_BezierPoints[i]);
        if (myRect.Contains(myPt)){
            m_BezierModifyIndexPoint = i;
            m_BezierActualC2 = m_Scale->RealToPixel(*m_BezierPointsControl[i]);
            return;
        }  
    }
}



void tmEditManager::BezierModifyClickMove (const wxPoint & mousepos){
    if (m_BezierModifyIndexControl != wxNOT_FOUND) {
        if (m_BezierModifyControlInverted == true) {
            wxPoint myPt = m_Scale->RealToPixel(*m_BezierPoints[m_BezierModifyIndexControl]);
            *m_BezierPointsControl[m_BezierModifyIndexControl] = m_Scale->PixelToReal(myPt - (mousepos - myPt));
        }
        else {
            *m_BezierPointsControl[m_BezierModifyIndexControl] = m_Scale->PixelToReal(mousepos);
        }
        
        m_Renderer->RefreshRect(m_BezierRefreshRect);
        m_Renderer->Update();
        return;
    }
    
    if (m_BezierModifyIndexPoint != wxNOT_FOUND) {
        *m_BezierPoints[m_BezierModifyIndexPoint] = m_Scale->PixelToReal(mousepos);
        
        // move control points too when moving point
        if (m_BezierActualC2 != wxDefaultPosition) {
            wxPoint myMovingPoint = mousepos - m_BezierActualP2;
            wxPoint myControlPx = m_BezierActualC2 + myMovingPoint;
            *m_BezierPointsControl[m_BezierModifyIndexPoint] = m_Scale->PixelToReal(myControlPx);
        }
        
        m_Renderer->RefreshRect(m_BezierRefreshRect);
        m_Renderer->Update();
        return;
    }
}



void tmEditManager::BezierModifyClickUp (const wxPoint & mousepos){
    m_BezierActualP2 = wxDefaultPosition;
    m_BezierActualC2 = wxDefaultPosition;
    m_BezierModifyIndexPoint = wxNOT_FOUND;
    m_BezierModifyIndexControl = wxNOT_FOUND;
}



void tmEditManager::DrawSnappingCircle (wxGCDC * dc){
    if (m_Renderer == NULL) {
        return;
    }
    wxASSERT(dc);
    
    switch (m_Renderer->GetTool()) {
        case tmTOOL_DRAW_BEZIER:
            if (m_BezierActualP2 != wxDefaultPosition) {
                dc->DrawCircle(m_BezierActualP2, 10);
                m_Renderer->Refresh();
                m_Renderer->Update();
            }
            break;
            
        default:
            break;
    }
}



/***************************************************************************//**
 @brief User select modify tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::OnToolModify ()
{
	wxASSERT (m_Renderer);
	if (IsModifictionAllowed()==false)
		return;
	
	m_Renderer->SetTool(tmTOOL_MODIFY);
	
	
}



void tmEditManager::OnToolEditShared(){
	wxASSERT(m_Renderer);
	m_Renderer->SetTool(tmTOOL_MODIFY_SHARED);
}


/***************************************************************************//**
 @brief Set tool to cut line tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmEditManager::OnToolCutLines ()
{
	wxASSERT(m_Renderer);
	m_Renderer->SetTool(tmTOOL_CUT_LINES);
}



/***************************************************************************//**
 @brief Set oriented point tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 September 2009
 *******************************************************************************/
void tmEditManager::OnToolOrientedPoint()
{
	wxASSERT(m_Renderer);
	
	if (IsModifictionAllowed()==false)
		return;
	
	if (IsLayerTypeSelected(TOC_NAME_POINTS)==false)
		return;
	
	m_OrientedPt.Create(m_pDB, m_SelectedData->GetSelectedUnique());
	if (m_OrientedPt.IsCorrectType()==false){
		wxLogWarning(_("No orientation attributes found, or point hasn't any kind"));	
		return;
	}
	m_Renderer->SetTool(tmTOOL_ORIENTED_POINTS);
	
}



/***************************************************************************//**
 @brief Called when snapping change
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::OnSnappingChange (wxCommandEvent & event)
{
	m_SnapMem = (tmSnappingMemory*) event.GetClientData();
	wxASSERT (m_SnapMem);
	DisplayRendererSnappingTolerence();
}



/***************************************************************************//**
 @brief Called when the view is updated
 @details After a zoom, pan or when loading the project
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 January 2009
 *******************************************************************************/
void tmEditManager::OnViewUpdated (wxCommandEvent & event)
{
	
	DisplayRendererSnappingTolerence();
	//wxLogDebug(_T("View updated"));
	
	wxClientDC myDC (m_Renderer);
	m_DrawLine.DrawEditReset(&myDC);
	
	if (IsDrawingAllowed()==true)
	{
	
		wxRealPoint myRPT;
		if (m_GISMemory->GetVertex(myRPT, -1))
		{
			wxPoint myPt = m_Scale->RealToPixel(myRPT);
			m_DrawLine.CreateVertex(myPt);
		}
		
		// draw memory line
		DrawMemoryData(true);
	}
	// update tools view
	wxCommandEvent evt(tmEVT_TM_UPDATE_TOOL_VIEW, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
}



/***************************************************************************//**
 @brief Display snapping if snapping enabled
 @details This function display a video inverse circle if snapping is enabled
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 January 2009
 *******************************************************************************/
void tmEditManager::DisplayRendererSnappingTolerence()
{
	if (m_SnapMem && m_Scale)
	{
		int iSnapRadius = 0;
		if (m_SnapMem->IsSnappingEnabled())
		{
			iSnapRadius = m_Scale->DistanceToReal(m_SnapMem->GetTolerence());
		}
		m_Renderer->ToogleSnapping(iSnapRadius);
	}
	//else
	//	wxLogDebug(_T("Snapping memory or scale not defined"));
}



/***************************************************************************//**
 @brief Checks that a layer is selected
 @details 
 @return  true if : a layer is defined as editing layer 
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsCorrectLayerSelected()
{
	wxASSERT(m_TOC);
	if (!m_TOC->GetEditLayer())
	{
		//if (m_Renderer->GetTool() == tmTOOL_DRAW) {
		//}
		
		return false;
	}
	
	
	// ensure no selection from external layer
	if (m_SelectedData->GetCount() > 0)
	{
		if (m_TOC->GetEditLayer()->GetID() != m_SelectedData->GetSelectedLayer())
		{
			m_SelectedData->Clear();
			wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
			m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
		}
		
	}
		
	return true;
}


/***************************************************************************//**
 @brief Check that a layer of type specified is selected
 @param layertype type of layer. Allowed values are : 
 - LAYER_SPATIAL_LINE
 - LAYER_SPATIAL_POINT
 - LAYER_SPATIAL_POLYGON
 @return  true if seleced layer is of specified type
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
bool tmEditManager::IsLayerTypeSelected (int layertype)
{
	wxASSERT(m_TOC);
	tmLayerProperties * myEditLayer = m_TOC->GetEditLayer();
	if (myEditLayer == NULL)
	{
		wxLogWarning(_("No editing layer selected. Define an edit layer"));
		return false;
	}
	
	
	if (m_TOC->GetEditLayer()->GetType() != layertype)
	{
		wxLogWarning(_("Layer isn't of correct type.") + 
					 wxString::Format(_("Please select a layer of type '%s'"),
					 TM_GIS_SPATIAL_TYPES_STRING[layertype].c_str()));
		return false;
	}
	
	return true;
	
}



/***************************************************************************//**
 @brief Checks that an object is selected
 @details 
 @return  true if an object is selected, false if no object or more than one
 objects are selected
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsObjectSelected()
{	
	if (m_SelectedData->GetCount() == 1){
		return true;	
	}
	return false;
}



/***************************************************************************//**
 @brief Check that minimum x objects are selected
 @param iNumbermin Number of minimum objects to be selected
 @return  true if minimum x objects are selected
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
bool tmEditManager::IsObjectMinNumberSelected (unsigned int iNumbermin)
{
	if (m_SelectedData->GetCount() >= iNumbermin)
		return true;
	
	wxLogDebug(_T("%d item selected, expected : %d"), m_SelectedData->GetCount(),
			   iNumbermin);
	return false;
}




/***************************************************************************//**
 @brief Are we ready for drawing
 @details 
 @return  True if all validation are ok for editing
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsDrawingAllowed()
{
	if (IsCorrectLayerSelected() && m_EditStarted)
		return true;
	
	return false;
}



/***************************************************************************//**
 @brief Are we ready for drawing
 @details 
 @return  True if all validation are ok for editing
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsModifictionAllowed()
{
	if (IsCorrectLayerSelected() == false){
		return false;
	}
	
	if ( IsObjectSelected() == false){
		return false;
	}
	return true;
}



bool tmEditManager::IsModificationBezierAllowed(){
    if (IsCorrectLayerSelected() == false){
        return false;
    }
    
    if (m_BezierPoints.GetCount() < 2) {
        return false;
    }
    
    if (m_BezierPoints.GetCount() != m_BezierPointsControl.GetCount()) {
        return false;
    }
    
    return true;
}


bool tmEditManager::IsMultipleModifictionAllowed(){
    if (IsCorrectLayerSelected() == false) {
        return false;
    }
    
    if (IsObjectMinNumberSelected(1) == false) {
        return false;
    }
    return true;
}



bool tmEditManager::IsLayerType(int layertype){
	wxASSERT(m_TOC);
	if (m_TOC->GetEditLayer() == NULL){
		return false;
	}
	
	if (m_TOC->GetEditLayer()->GetType() != layertype) {
		return false;
	}
	return true;
}



/***************************************************************************//**
 @brief Called when a click down is issued with Draw tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
void tmEditManager::OnDrawDown(wxCommandEvent & event)
{
	// get coordinate and dont forget to delete it
	wxPoint * myPxCoord = (wxPoint*) event.GetClientData();
	
	// check drawing allowed
	if (!IsDrawingAllowed())
	{
		delete myPxCoord;
		return;
	}
	
	if (m_SnapMem->IsSnappingEnabled()==true)
	{
		double iSnapRadius = m_Scale->DistanceToReal(m_SnapMem->GetTolerence());
		m_Renderer->DrawCircleVideoInverse(*myPxCoord, iSnapRadius);
		//m_Renderer->Update();
	}
	delete myPxCoord;
}


/***************************************************************************//**
 @brief Called when a click up is issued with Draw tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
void tmEditManager::OnDrawUp (wxCommandEvent & event)
{
	// get coordinate and dont forget to delete it
    wxPoint * myPxCoord = (wxPoint*) event.GetClientData();
	wxRealPoint myRealCoord = m_Scale->PixelToReal(*myPxCoord);
	
	// check drawing allowed
	if (!IsDrawingAllowed())
	{
		delete myPxCoord;
		return;
	}
	wxClientDC myDC (m_Renderer);
	m_DrawLine.DrawEditReset(&myDC);
	
	if (m_SnapMem->IsSnappingEnabled()==true)
	{
		//double iSnapRadius = m_Scale->DistanceToReal(m_SnapMem->GetTolerence());
		m_Renderer->DrawCircleVideoInverseClean();
		//m_Renderer->DrawCircleVideoInverse(*myPxCoord, iSnapRadius);
		m_Renderer->Update();
	}
	wxDELETE(myPxCoord);
	
	// snapping
	bool bSnapFound = EMGetSnappingCoord(myRealCoord);
	
	// add  line vertex
	if (m_TOC->GetEditLayer()->GetSpatialType() == LAYER_SPATIAL_LINE){	
    
        // feature #180 if snap isn't found, try to snap on line in edition
        if (bSnapFound == false && m_SnapMem->IsSnappingEnabled() == true) {
            wxRealPoint * myTempSnap = EMSearchLineMemorySnapping(myRealCoord);
            if (myTempSnap != NULL) {
                myRealCoord = *myTempSnap;
            }
            wxDELETE(myTempSnap);
        }
        
        wxPoint myNewPxCoord = m_Scale->RealToPixel(myRealCoord);
		bool bCreate = m_DrawLine.CreateVertex(myNewPxCoord);
		wxASSERT(bCreate);
		
		// ensure vertex isn't already present (double insert);
		wxRealPoint myLastVertex;
		bool bReturn = m_GISMemory->GetVertex(myLastVertex);
		if (bReturn == true && myLastVertex == myRealCoord) {
			wxLogMessage(_("Vertex : %.2f, %.2f not added, already existing!"),
						 myRealCoord.x, myRealCoord.y);
			DrawMemoryData(false);
			return;
		}
		
		AddLineVertex(myRealCoord);
		DrawMemoryData(false);
	}
	else // add point 
	{
		AddPointVertex(myRealCoord);
	}
	//delete myPxCoord;
}


/***************************************************************************//**
 @brief Called when a move is issued with Draw tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 4 May 2009
 *******************************************************************************/
void tmEditManager::OnDrawMove (wxCommandEvent & event)
{
	wxPoint * myPt = (wxPoint*) event.GetClientData();
	wxASSERT (myPt);
	m_LastMousePos = *myPt;
	
	if (m_DrawLine.IsOK() == false)
	{
		wxDELETE(myPt);
		return; 
	}
	wxClientDC dc (m_Renderer);
	bool BDraw = m_DrawLine.DrawEditPart(&dc);
	wxASSERT(BDraw);
	bool bSetVertex = m_DrawLine.SetVertex(*myPt);
	wxASSERT(bSetVertex);	
	wxDELETE(myPt);
}


void tmEditManager::OnOrientedPtsDown(wxCommandEvent & event)
{
	wxPoint * myPt = (wxPoint*) event.GetClientData();
	wxASSERT (myPt);
	
	m_OrientedPt.SetStartPoint(*myPt);
	m_DrawLine.CreateVertex(*myPt);
	
	delete myPt;
}


void tmEditManager::OnOrientedPtsMove (wxCommandEvent & event)
{
	wxPoint * myPt = (wxPoint*) event.GetClientData();
	wxASSERT (myPt);
	
	if (m_DrawLine.IsOK() == false)
	{
		delete myPt;
		return; 
	}
	
	wxClientDC dc (m_Renderer);
	bool BDraw = m_DrawLine.DrawEditPart(&dc);
	wxASSERT(BDraw);
	bool bSetVertex = m_DrawLine.SetVertex(*myPt);
	wxASSERT(bSetVertex);
	
	
	// display angle
	m_OrientedPt.SetEndPoint(*myPt);
	int myOrient = m_OrientedPt.GetOrientationInt();
	if (myOrient != wxNOT_FOUND)
	{
		wxCommandEvent evt2(tmEVT_LM_ANGLE_CHANGED, wxID_ANY);
		evt2.SetInt(myOrient);
		m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	}
	
	delete myPt;
}


void tmEditManager::OnOrientedPtsUp (wxCommandEvent & event)
{
	wxPoint * myPt = (wxPoint*) event.GetClientData();
	wxASSERT (myPt);
	
	if(m_DrawLine.IsOK() == false)
	{
		delete myPt;
		return;
	}
	wxClientDC myDC (m_Renderer);
	m_DrawLine.DrawEditReset(&myDC);
	
	m_OrientedPt.SetEndPoint(*myPt);
	bool bUpdate = m_OrientedPt.Update();
	wxASSERT(bUpdate);	
	
	// clearing status bar
	wxCommandEvent evt2(tmEVT_LM_ANGLE_CHANGED, wxID_ANY);
	evt2.SetInt(wxNOT_FOUND);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	
	
	m_DrawLine.ClearVertex();
	
	m_Renderer->Refresh(false);
	m_Renderer->Update();
	
	delete myPt;
}


/***************************************************************************//**
 @brief Add a vertex for line
 @details store the line vertex in memory
 @param pt the coordinate to store in memory
 @return  true if vertex was stored in memory
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
bool tmEditManager::AddLineVertex (const wxRealPoint & pt)
{
	bool bReturn = m_GISMemory->InsertVertex(pt, -1);
	//wxLogDebug(_T("Number of stored vertex : %d"), m_GISMemory->GetVertexCount());
	return bReturn;
}


/***************************************************************************//**
 @brief Draw and store the point into database
 @param pt the point (real)
 @return  true if the point was successfully added to the database
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
bool tmEditManager::AddPointVertex (const wxRealPoint & pt)
{
	bool bReturn = true;
	// draw the point
	tmDrawer myEditDrawer;
	myEditDrawer.InitDrawer(m_Renderer->GetBitmap(), 
							*m_Scale, m_Scale->GetWindowExtentReal());
	
	// get the symbology
	tmSymbolVectorPoint * mySymbol = (tmSymbolVectorPoint*) m_TOC->GetEditLayer()->GetSymbolRef();
	
	// draw the vertex in selected colour
	myEditDrawer.DrawEditVertex(pt, mySymbol->GetRadius(),
								*wxRED);

	// store the vertex into database
	long lpOid = StorePoint(pt);
	if (lpOid == -1)
	{
		wxLogError(_T("Error inserting point into database"));
		return false;
	}
	
	// draw the selected in normal colour (blue)
	tmGISDataVectorMemory myGISMem;
	myGISMem.CreateFeature();
	wxArrayLong * mySelArray = m_SelectedData->GetSelectedValues();
	if (mySelArray != NULL)
	{
		for (unsigned int k = 0; k<mySelArray->GetCount();k++)
		{
			
			wxRealPoint myRealPt;
			if(myGISMem.GetPointFromDatabase(m_pDB, mySelArray->Item(k),
											 m_TOC->GetEditLayer()->GetType())==false)
				break;
			
			bool bGet = myGISMem.GetVertex(myRealPt);
			wxASSERT(bGet);
			myEditDrawer.DrawEditVertex(myRealPt, mySymbol->GetRadius()+1,
										mySymbol->GetColour());
			
		}
		delete mySelArray;
	}
	
	
	// select the last inserted oid
	m_SelectedData->SetLayerID(m_TOC->GetEditLayer()->GetID());
	m_SelectedData->SetSelected(lpOid);
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
	
	//TODO: Remove this temp code
	wxLogDebug(_T("Selected OID = %ld"), lpOid);
	
	
	m_Renderer->Refresh();
	m_Renderer->Update();
	return bReturn;
}



/***************************************************************************//**
 @brief Save the passed point to the database
 @param pt the point (real)
 @return  the OID of the stored point, -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
long tmEditManager::StorePoint (const wxRealPoint & pt)
{
	// get table
	tmLayerProperties * layerprop = m_TOC->GetEditLayer();
	if (layerprop == NULL)
		return -1;
	
	tmGISDataVectorMemory myTempPoint;
	myTempPoint.CreateFeature();
	myTempPoint.InsertVertex(pt);
	long lid = myTempPoint.SavePointToDatabase(m_pDB, layerprop->GetType());
	
	return lid;
}


/***************************************************************************//**
 @brief Store the line in memory into database
 @details @warning No verification are done internally, make all checks before
 calling this function
 @return the OID of the line inserted or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
long tmEditManager::StoreLine ()
{
	tmLayerProperties * layerprop = m_TOC->GetEditLayer();
	if (layerprop == NULL)
		return -1;
	
	return m_GISMemory->SaveLineToDatabase(m_pDB, layerprop->GetType());
}



/***************************************************************************//**
 @brief Update line in database
 @details This function works only if tmGISDataVectorMemory object contain a
 valid OID
 @param bool true if updating was successfull
 @author Lucien Schreiber (c) CREALP 2009
 @date 29 April 2009
 *******************************************************************************/
bool tmEditManager::UpdateLine()
{
	tmLayerProperties * layerprop = m_TOC->GetEditLayer();
	wxASSERT (layerprop);
	if (layerprop == NULL)
		return false;
	
	bool bContainOID = m_GISMemory->IsUpdating();
	wxASSERT(bContainOID);
	if (bContainOID == false)
		return false;
	
	return m_GISMemory->UpdateLineToDatabase(m_pDB, layerprop->GetType());
}


bool tmEditManager::UpdatePoint()
{
	tmLayerProperties * layerprop = m_TOC->GetEditLayer();
	wxASSERT (layerprop);
	if (layerprop == NULL)
		return false;
	
	bool bContainOID = m_GISMemory->IsUpdating();
	wxASSERT(bContainOID);
	if (bContainOID == false)
		return false;
	
	return m_GISMemory->UpdatePointToDatabase(m_pDB, layerprop->GetType());
}


/***************************************************************************//**
 @brief Directly draw the last segment
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmEditManager::DrawLastSegment ()
{
	wxRealPoint LastRealPt(0,0), LastLastRealPt(0,0);
	
	// init a drawer 
	tmDrawer myEditDrawer;
	myEditDrawer.InitDrawer(m_Renderer->GetBitmap(), 
							*m_Scale, m_Scale->GetWindowExtentReal());
	
	// get the symbology
	tmSymbolVectorLine * mySymbol = (tmSymbolVectorLine*) m_TOC->GetEditLayer()->GetSymbolRef();
	
	// get two last vertex 
	m_GISMemory->GetVertex(LastRealPt, -1);
	m_GISMemory->GetVertex(LastLastRealPt, m_GISMemory->GetVertexCount()-2);
	
	myEditDrawer.DrawEditSegment(LastLastRealPt,
								LastRealPt, 
								mySymbol->GetWidth());
	m_Renderer->Refresh();
	m_Renderer->Update();
}



/***************************************************************************//**
 @brief Draw again the line in editing when image reloaded
 @author Lucien Schreiber (c) CREALP 2009
 @date 04 February 2009
 *******************************************************************************/
void tmEditManager::DrawEditBitmapLine ()
{
	// check edit memory data for drawing
	int iNbVertexMemory = m_GISMemory->GetVertexCount();
	if (iNbVertexMemory == 0)
		return;
	
	// init a drawer 
	tmDrawer myEditDrawer;
	myEditDrawer.InitDrawer(m_Renderer->GetBitmap(), 
							*m_Scale, m_Scale->GetWindowExtentReal());
	// get the symbology
	tmSymbolVectorLine * mySymbol = (tmSymbolVectorLine*) m_TOC->GetEditLayer()->GetSymbolRef();

	
	
	// get all vertex 
	wxArrayRealPoints myRealPts;
	wxRealPoint myActualPoint;
	for (int i = 0; i<iNbVertexMemory; i++)
	{
		m_GISMemory->GetVertex(myActualPoint, i);
		myRealPts.Add(myActualPoint);
	}
	
	myEditDrawer.DrawEditLine(myRealPts,
							  mySymbol->GetWidth());
	
	m_Renderer->Refresh();
	m_Renderer->Update();

}


void tmEditManager::DrawMemoryData(bool refresh)
{
	// check edit memory data for drawing
	int iNbVertexMemory = m_GISMemory->GetVertexCount();

	
	// Removed because flickering under Windows
	if (refresh == true) {
		m_Renderer->Refresh();
		m_Renderer->Update();
	}

	
	if (iNbVertexMemory <= 1){
		return;
	}
	
	// init a drawer 
	tmDrawer myEditDrawer;
	myEditDrawer.InitDrawer(m_Renderer->GetBitmap(), 
							*m_Scale, m_Scale->GetWindowExtentReal());
	
	wxClientDC dc(m_Renderer);
	myEditDrawer.DrawMemoryData(m_GISMemory, m_TOC->GetEditLayer(), &dc);
	
}



/***************************************************************************//**
 @brief Called when TOC start editing
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmEditManager::OnEditStart (wxCommandEvent & event)
{
    wxConfigBase * myConfig =  wxConfigBase::Get(false);
    wxASSERT(myConfig);
    myConfig->SetPath("GENERAL");
	wxString mySelColorText = myConfig->Read("selection_color", wxEmptyString);
    myConfig->SetPath("..");
	
	if (mySelColorText != wxEmptyString) {
		m_SelectionColour.Set(mySelColorText);
	}
    
    m_GISMemory->CreateFeature();
	m_EditStarted = true;	
	event.Skip();
}


/***************************************************************************//**
 @brief Called when TOC stop editing
 @author Lucien Schreiber (c) CREALP 2009
 @date 03 February 2009
 *******************************************************************************/
void tmEditManager::OnEditStop (wxCommandEvent & event)
{
	m_GISMemory->DestroyFeature();
	m_EditStarted = false;
	event.Skip();
}



/***************************************************************************//**
 @brief Called when user press enter / TAB in the #tmRenderer
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmEditManager::OnDrawFeatureValidate (wxCommandEvent & event)
{
	if (IsDrawingAllowed() == false)
		return;
	
	wxClientDC myDC (m_Renderer);
	m_DrawLine.DrawEditReset(&myDC);
	
	// minimum 2 vertex for saving line into database
	if (m_GISMemory->GetVertexCount() < 2) 
	{
		m_GISMemory->DestroyFeature();
		m_GISMemory->CreateFeature();
		return;
	}
	
	m_DrawLine.ClearVertex();
	
	long lid = wxNOT_FOUND;
	// UPDATING
	if (m_GISMemory->IsUpdating())
	{
		lid = m_SelectedData->GetSelectedUnique();
		bool bUpdate = UpdateLine();
		wxASSERT(bUpdate);
	}
	else // CREATING NEW
	{
		lid = StoreLine();
		if (lid == -1)
		{
			wxLogDebug(_T("Line not saved into database"));
			return;
		}
		
	}

	wxLogDebug(_T("Line saved : OID = %ld"), lid);
	
	// Clear memory
	m_GISMemory->DestroyFeature();
	m_GISMemory->CreateFeature();
	
	// set selection
	m_SelectedData->SetLayerID(m_TOC->GetEditLayer()->GetID());
	m_SelectedData->SetSelected(lid);
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);

}


/***************************************************************************//**
 @brief Called when user press ESC in the #tmRenderer
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmEditManager::OnDrawFeatureEscape (wxCommandEvent & event)
{
	if (IsDrawingAllowed() == false)
		return;
	
	wxClientDC myDC (m_Renderer);
	m_DrawLine.DrawEditReset(&myDC);
	m_DrawLine.ClearVertex();
    
	// Clear memory
	m_GISMemory->DestroyFeature();
	m_GISMemory->CreateFeature();
	
    BezierClear();
    
	m_Renderer->Refresh();
	m_Renderer->Update();

}


/***************************************************************************//**
 @brief Called if user try to cut lines
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void tmEditManager::OnCutLines (wxCommandEvent & event)
{
	// get mouse position
	wxPoint * mypCutPos = (wxPoint*) event.GetClientData();
	wxPoint myCutPos = wxPoint(*mypCutPos);	
	delete mypCutPos;
	
	// some checks (1 object selected)
	if (IsModifictionAllowed() == false)
	{
		wxLogDebug(_T("Unable to cut line, select a line first"));
		return;
	}
	if (m_TOC->GetEditLayer()->GetSpatialType() != LAYER_SPATIAL_LINE){
		return;
    }
		
	// Get the selected layer 
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (!mySelLayer){
		return;
    }
	
	// display radius 
	int icutRadius = tmSELECTION_DIAMETER;
	if (m_SnapMem->IsSnappingEnabled()){
		icutRadius = m_SnapMem->GetTolerence();
		double dRadius = m_Scale->DistanceToReal(icutRadius);
		icutRadius = int(dRadius + 0.5);
	}

	m_Renderer->DrawCircleVideoInverse(myCutPos, icutRadius);
	m_Renderer->Update();
	wxMilliSleep(200);
	m_Renderer->DrawCircleVideoInverseClean();
	
	bool bCut = mySelLayer->CutLineAtVertex(m_SelectedData->GetSelectedUnique(),
											m_Scale->PixelToReal(myCutPos),
											tmSELECTION_DIAMETER, m_TOC->GetEditLayer()->GetType());	
	wxDELETE(mySelLayer);
	if (bCut == false){
		return;
    }
	
	// copy attribution only if layer is lines (not frame!)
    if (m_TOC->GetEditLayer()->GetType() == TOC_NAME_LINES) {        
        wxCommandEvent attribevt1(tmEVT_AM_COPY_ATTRIBUTION, wxID_ANY);
        attribevt1.SetExtraLong(m_SelectedData->GetSelectedUnique());
        wxArrayLong * myTempArray = new wxArrayLong(); 
        myTempArray->Add(m_pDB->DataBaseGetLastInsertedID());
        attribevt1.SetClientData(myTempArray);
        m_ParentEvt->GetEventHandler()->AddPendingEvent(attribevt1);
	}
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	
	// change cursor
	m_Renderer->SetTool(tmTOOL_SELECT);
}



/***************************************************************************//**
 @brief Called to display vertex position
 @details Respond to tmEVT_EV_DISPLAY_VERTEX_COORD event.
 @author Lucien Schreiber (c) CREALP 2009
 @date 24 February 2009
 *******************************************************************************/
void tmEditManager::OnShowVertexPosition (wxCommandEvent & event)
{
	wxRealPoint * myPt = (wxRealPoint*) event.GetClientData();
	if (myPt == NULL)
		return;
	
	wxLogDebug(_T("Showing vertex @ %.*f / %.*f"), 2, myPt->x, 2, myPt->y);
	
	// convert to px
	wxPoint myPxPt = m_Scale->RealToPixel(*myPt);
	delete myPt;
	
	m_Renderer->Refresh();
	m_Renderer->Update();
		
	m_Renderer->DrawCircleVideoInverse(myPxPt, 7);
	wxMilliSleep(200);
	m_Renderer->DrawCircleVideoInverseClean();
}


/***************************************************************************//**
 @brief Called to when modify is clicked down for searching vertex
 @details Respond to tmEVT_EM_MODIFY_CLICK event.
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 April 2009
 *******************************************************************************/
void tmEditManager::OnModifySearch (wxCommandEvent & event)
{
	// getting point
	wxPoint * myTempPt = (wxPoint*) event.GetClientData();
	wxASSERT (myTempPt);
	wxRealPoint myRPt = m_Scale->PixelToReal(*myTempPt);

	bool bSearch = false;
	
	if (m_TOC->GetEditLayer()->GetSpatialType() == LAYER_SPATIAL_LINE)
	{
		bSearch = EMModifySearchLine(myRPt);
	}
	else
	{
		bSearch = EMModifySearchPoint(myRPt);
	}

	delete myTempPt;
	if (bSearch == false)
	{
		wxLogDebug(_T("No Vertex Found"));
		m_Renderer->StopModifyEvent();
	}
}




bool tmEditManager::EMModifySearchPoint(const wxRealPoint & pt)
{
	// load point
	if(m_GISMemory->GetVertexCount() == 0)
	{
		long myActualSel = m_SelectedData->GetSelectedUnique();
		tmLayerProperties * mypLayerProp = m_TOC->GetEditLayer();
		wxASSERT(myActualSel != wxNOT_FOUND);
		wxASSERT(mypLayerProp);
		bool bCopy = m_GISMemory->GetPointFromDatabase(m_pDB, myActualSel,
													   mypLayerProp->GetType());
		wxASSERT(bCopy);
		m_GISMemory->SetOID(myActualSel);
	}
		
	// searching if point was correctly clicked
	if (m_GISMemory->SearchPoint(pt, tmSELECTION_DIAMETER, m_Scale->GetPixelSize())==false)
		return false;
	
	wxRealPoint myStoredRPt;
	bool bGetPoint = m_GISMemory->GetVertex(myStoredRPt);
	wxASSERT(bGetPoint);
	wxPoint myPoint = m_Scale->RealToPixel(myStoredRPt);
	bool bCreateDrawerLine = m_DrawLine.CreateVertex(myPoint);
	wxASSERT(bCreateDrawerLine);
	return true;
}




bool tmEditManager::EMModifySearchLine(const wxRealPoint & pt)
{
	// Load Line from DB -> Memory (if needed)
	if( m_GISMemory->GetVertexCount() == 0)
	{
		long myActualSel = m_SelectedData->GetSelectedUnique();
		tmLayerProperties * mypLayerProp = m_TOC->GetEditLayer();
		wxASSERT(myActualSel != wxNOT_FOUND);
		wxASSERT(mypLayerProp);
		bool bCopy = m_GISMemory->GetLineFromDatabase(m_pDB, myActualSel,
													  mypLayerProp->GetType());
		wxASSERT(bCopy);
		m_GISMemory->SetOID(myActualSel);
	}
	
	
	wxRealPoint topleft, bottomright;
	
	// searching vertex
	int iIndex = wxNOT_FOUND;
	if (m_GISMemory->SearchVertex(pt, iIndex, tmSELECTION_DIAMETER, m_Scale->GetPixelSize())==false)
		return false;
	
	// creating invert-video drawing
	wxPoint * myLeft = NULL;
	wxPoint * myRight = NULL;
	wxPoint myPoint;
	
	wxRealPoint myPtToConvert;
	bool bGetPoint = m_GISMemory->GetVertex(myPtToConvert, iIndex);
	wxASSERT(bGetPoint);
	myPoint = m_Scale->RealToPixel(myPtToConvert);
	
	if (m_GISMemory->GetVertex(myPtToConvert, iIndex + 1)==true)
	{
		myRight = new wxPoint(m_Scale->RealToPixel(myPtToConvert));
	}
	
	if (iIndex -1 >= 0)
	{
		if (m_GISMemory->GetVertex(myPtToConvert, iIndex -1)==true)
		{
			myLeft = new wxPoint (m_Scale->RealToPixel(myPtToConvert));
		}
	}
	
	
	// converting
	bool bCreateDrawerLine = m_DrawLine.CreateVertex(myPoint, myLeft, myRight, iIndex);
	wxASSERT(bCreateDrawerLine);
	
	if (myLeft != NULL)
		delete myLeft;
	
	if (myRight != NULL)
		delete myRight;
	return true;
}


void tmEditManager::OnModifyMove (wxCommandEvent & event)
{
	wxPoint * myPt = (wxPoint*) event.GetClientData();
	wxASSERT (myPt);
	
	//if (m_TOC->GetEditLayer()->m_LayerType == LAYER_SPATIAL_LINE)
	//{
	wxClientDC dc (m_Renderer);
	bool BDraw = m_DrawLine.DrawEditPart(&dc);
	wxASSERT(BDraw);
	bool bSetVertex = m_DrawLine.SetVertex(*myPt);
	wxASSERT(bSetVertex);
	
	//BDraw = m_DrawLine.DrawEditPart(&dc);
	//wxASSERT(BDraw);
	/*}
	else
	{
		tmSymbolVectorPoint * mySymbol = (tmSymbolVectorPoint*) m_TOC->GetEditLayer()->m_LayerSymbol;
		int iRadius = mySymbol->GetRadius();
		
		m_Renderer->Refresh();
		m_Renderer->Update();
		wxClientDC dc(m_Renderer);
		
		wxASSERT(mySymbol);
		//dc.DrawRectangle(MyRefreshRect);
		dc.DrawCircle(*myPt,iRadius );
	}*/
	
	wxDELETE(myPt);
}


void tmEditManager::OnModifyUp (wxCommandEvent & event)
{
	wxPoint * myPt = (wxPoint*) event.GetClientData();
	wxASSERT (myPt);
	
	wxClientDC myDC (m_Renderer);
	m_DrawLine.DrawEditReset(&myDC);
	
	//check snapping
	wxRealPoint myRPt = m_Scale->PixelToReal(*myPt);
	bool bSnappingFound = EMGetSnappingCoord(myRPt);
	
	if (m_TOC->GetEditLayer()->GetSpatialType() == LAYER_SPATIAL_LINE)
	{
		bool bSetVertex = m_DrawLine.SetVertex(*myPt);
		wxASSERT(bSetVertex);
		
		bool BSave = m_GISMemory->SetVertex(myRPt, m_DrawLine.GetVertexIndex());
		wxASSERT(BSave);
		

		DrawMemoryData(true);
		if (bSnappingFound)
			EMDrawSnappingStatus(*myPt);
	}
	else
	{
		bool BSave = m_GISMemory->SetVertex(myRPt, 0);
		wxASSERT(BSave);
		
		m_DrawLine.ClearVertex();
		bool bUpdate =UpdatePoint();
		wxASSERT(bUpdate);
		
		if (bSnappingFound)
			EMDrawSnappingStatus(*myPt);
		
		// Clear memory
		m_GISMemory->DestroyFeature();
		m_GISMemory->CreateFeature();
		
		// update display
		wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
		m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	}

	delete myPt;
}



void tmEditManager::EMCreateMenu(wxMenu & menu)
{
	// cleaning first
	wxMenuItemList items = menu.GetMenuItems();
	wxMenuItemList::iterator iter;
    for (iter = items.begin(); iter != items.end(); ++iter)
	{
		wxMenuItem *current = *iter;
		menu.Destroy(current);
	}
	wxASSERT(menu.GetMenuItemCount() == 0);
	
	menu.Append(tmEM_CONTEXTMENU_VERTEX_INSERT, _("Insert vertex"), _("Insert a vertex"));
	menu.Append(tmEM_CONTEXTMENU_VERTEX_DELETE, _("Delete selected vertex"),
				_("Delete the selected vertex"));
	menu.AppendSeparator();
	menu.Append(tmEM_CONTEXTMENU_LINE_SAVE, _("Apply modifications\tTAB"), 
				_("Apply modifications)"));
	menu.Append(tmEM_CONTEXTMENU_VERTEX_INSERT, _("Cancel modifications\tESC"),
				_("Cancel modifications"));
}


void tmEditManager::EMGetMenuLine(wxMenu & menu)
{
	EMCreateMenu(menu);
	menu.Enable(tmEM_CONTEXTMENU_VERTEX_INSERT, true);
	menu.Enable(tmEM_CONTEXTMENU_VERTEX_DELETE, false);
}


void tmEditManager::EMGetMenuVertex(wxMenu & menu)
{
	EMCreateMenu(menu);
	menu.Enable(tmEM_CONTEXTMENU_VERTEX_INSERT, false);
	menu.Enable(tmEM_CONTEXTMENU_VERTEX_DELETE, true);
}


bool tmEditManager::EMLoadModifyData()
{
	if (IsModifictionAllowed()==false)
		return false;
	
	// load line if needed
	if( m_GISMemory->GetVertexCount() == 0)
	{
		long myActualSel = m_SelectedData->GetSelectedUnique();
		tmLayerProperties * mypLayerProp = m_TOC->GetEditLayer();
		wxASSERT(myActualSel != wxNOT_FOUND);
		wxASSERT(mypLayerProp);
		bool bCopy = m_GISMemory->GetLineFromDatabase(m_pDB, myActualSel,
													  mypLayerProp->GetType());
		wxASSERT(bCopy);
		m_GISMemory->SetOID(myActualSel);
	}
	return true;
}



void tmEditManager::OnModifyMenu (wxCommandEvent & event)
{
	// get coordinate and dont forget to delete it
	wxPoint * myPxCoord = (wxPoint*) event.GetClientData();
	wxRealPoint myRPT = m_Scale->PixelToReal(*myPxCoord);
	
	if (EMLoadModifyData()==false)
	{
		delete myPxCoord;
		return;
	}
	
	wxRealPoint topleft, bottomright;
	
	wxMenu myPopupMenu;
	m_INSDELVertex = wxNOT_FOUND;
	m_INSVertexPos = wxRealPoint(-1,-1);
	if (m_GISMemory->IsIntersectingGeometry(myRPT,m_INSDELVertex, tmSELECTION_DIAMETER, m_Scale->GetPixelSize())==true)
	{
		m_INSVertexPos = myRPT;
		EMGetMenuLine(myPopupMenu);
		
		if (m_GISMemory->SearchVertex(myRPT, m_INSDELVertex, tmSELECTION_DIAMETER, m_Scale->GetPixelSize())==true)
		{
			EMGetMenuVertex(myPopupMenu);
		}
		m_Renderer->PopupMenu(&myPopupMenu, *myPxCoord);
	}
	delete myPxCoord;	
}



void tmEditManager::OnMenuInsertVertex(wxCommandEvent & event)
{
	if (m_INSDELVertex == wxNOT_FOUND || m_INSVertexPos == wxRealPoint(-1,-1))
	{
		wxFAIL_MSG(_T("Error inserting vertex"));
		return;
	}
	
	wxLogDebug(_T("Inserting vertex after %d"), m_INSDELVertex);
	m_GISMemory->InsertVertex(m_INSVertexPos, m_INSDELVertex+1);
	DrawMemoryData(true);
}


void tmEditManager::OnMenuDeleteVertex(wxCommandEvent & event)
{
	if (m_INSDELVertex == wxNOT_FOUND)
	{
		wxFAIL_MSG(_T("Error deleting vertex"));
		return;
	}
	wxLogDebug(_T("Deleting vertex @ %d"), m_INSDELVertex);
	
	m_GISMemory->RemoveVertex(m_INSDELVertex);
	DrawMemoryData(true);
}




void tmEditManager::EMDrawSnappingStatus (const wxPoint & pt)
{
	double iSnapRadius = m_Scale->DistanceToReal(m_SnapMem->GetTolerence());
	
	m_Renderer->DrawCircleVideoInverse(pt, iSnapRadius);
	m_Renderer->Update();
	wxMilliSleep(150);
	m_Renderer->DrawCircleVideoInverseClean();
}



/***************************************************************************//**
 @brief Search function
 @details This function is widly inspired from #tmLayerManager
 @param screenpt Coordinate of the clicked point
 @return  true if an object was found
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 February 2009
 *******************************************************************************/
bool tmEditManager::SelectedSearch (const wxPoint & screenpt)
{
	wxASSERT(IsDrawingAllowed());
	tmLayerProperties * pLayerprop = m_TOC->GetEditLayer();
	wxASSERT(pLayerprop);
	
	// create rect from point
	int myRadius = tmSELECTION_DIAMETER / 2;
	wxRect myClickRect = wxRect (screenpt.x - myRadius,
								 screenpt.y - myRadius,
								 tmSELECTION_DIAMETER,
								 tmSELECTION_DIAMETER);
	
	tmRealRect myClickReal = m_Scale->PixelsToReal(myClickRect);
	tmGISData * myLayerData = tmGISData::LoadLayer(pLayerprop);
	wxASSERT (myLayerData);
	if (!myLayerData)
		return false;
	
	
	
	wxArrayLong * myArray = myLayerData->SearchData(myClickReal, pLayerprop->GetType());
	if (!myArray)
		return false;
	
	int myArrayCount = myArray->GetCount();
	if (myArrayCount == 0)
	{
		wxLogDebug(_T("No data found at specified coordinates"));
		return false;
	}
	wxLogDebug(_T("Number of features selected : %d"), myArrayCount);
	
	
	m_SelectedData->SetLayerID(pLayerprop->GetID());
	m_SelectedData->Clear();
	m_SelectedData->AddSelected(myArray);
	myArray->Clear();
	delete myArray;
	
	return true;
}




bool tmEditManager::EMGetSnappingCoord (wxRealPoint & pt)
{
	// Snapping may be disabled using space key
	if (m_SnapMem->IsSnappingEnabled()==false)
		return false;
	
	
	//bool bReturn = false;
	wxRealPoint * mySnapCoord = NULL;
	mySnapCoord = EMIterateAllSnappingLayers(pt);
		
	//TODO: Remove this temp logging code
	if (mySnapCoord)
	{
		wxLogDebug(_T("Point found for snapping @ : %.*f, %.*f"),
				   2, mySnapCoord->x, 
				   2, mySnapCoord->y);
	}	
	// END of temp logging code
		
	
	if (mySnapCoord)
	{
		pt.x = mySnapCoord->x;
		pt.y = mySnapCoord->y;
		wxDELETE(mySnapCoord);
		return true;
	}
	wxDELETE(mySnapCoord);
	return false;
}
	




/***************************************************************************//**
 @brief Iterate all layers in snapping memory
 @details Try to get the snapped coordinate for the clicked point
 @param clickedpoint The real coordinate of the clicked point
 @return  a valid wxRealPoint if snapped point found, null otherwise (don't
 forget to delete)
 @author Lucien Schreiber (c) CREALP 2009
 @date 30 January 2009
 *******************************************************************************/
wxRealPoint * tmEditManager::EMIterateAllSnappingLayers(const wxRealPoint & clickedpoint)
{
	long myLayerId = 0;
	int mySnapStatus = tmSNAPPING_OFF;
	tmLayerProperties * myActualLayer = NULL;	
    wxArrayRealPoints mySnapPts;
	for (unsigned int i = 0; i< m_SnapMem->GetCount(); i++){
		m_SnapMem->GetSnappingInfo(i, myLayerId, mySnapStatus);
		myActualLayer = m_TOC->GetLayerById(myLayerId);
		if (!myActualLayer)
			break;
		
		// search snapping for that layer
		tmGISData * myActualGISData = tmGISData::LoadLayer(myActualLayer);
		if (!myActualGISData){
			break;
        }
				
		myActualGISData->GetSnapCoord(clickedpoint, m_SnapMem->GetTolerence(), 
                                      mySnapPts,mySnapStatus);
		wxDELETE(myActualGISData);
    }
    
    wxLogMessage("%ld snapping points found!", mySnapPts.GetCount());    
    if (mySnapPts.GetCount() == 0) {
        return NULL;
    }
    
    if (mySnapPts.GetCount() == 1) {
        return new wxRealPoint(mySnapPts.Item(0));
    }
    
    // compute closest point!
    int myMinItemIndex = 0;
    double myMinDistance = 0;
    for (unsigned int p = 0; p<mySnapPts.GetCount(); p++) {  
        wxRealPoint mySubstrPtr = mySnapPts.Item(p) - clickedpoint;
        double myDistance = fabs(sqrt((mySubstrPtr.x * mySubstrPtr.x) + (mySubstrPtr.y * mySubstrPtr.y)));
        if (p == 0) {
            myMinDistance = myDistance;
        }
        else{
            if(myDistance < myMinDistance){
                myMinDistance = myDistance;
                myMinItemIndex = p;
            }
        }
    }
    return new wxRealPoint(mySnapPts.Item(myMinItemIndex));    
}



wxRealPoint * tmEditManager::EMSearchLineMemorySnapping (const wxRealPoint & clickedpoint){
    // get snapping info for layer in edition (line or frame)
    long myLayerId = 0;
    int mySnapStatus = tmSNAPPING_OFF;
    bool bFoundLayer = false;
    for (unsigned int i = 0; i< m_SnapMem->GetCount(); i++){
        m_SnapMem->GetSnappingInfo(i, myLayerId, mySnapStatus);
        if (m_TOC->GetEditLayer()->GetID() == myLayerId) {
            bFoundLayer = true;
            break;
        }
    }
    
    if (bFoundLayer == false) {
        return NULL;
    }
    
    // check all possible points in memory data
    wxArrayRealPoints mySnapPts;
    m_GISMemory->GetSnapCoord(clickedpoint, m_SnapMem->GetTolerence(), mySnapPts, mySnapStatus);
    if (mySnapPts.GetCount() == 0) {
        return NULL;
    }
    
    // compute closest point!
    int myMinItemIndex = 0;
    double myMinDistance = 0;
    for (unsigned int p = 0; p<mySnapPts.GetCount(); p++) {  
        wxRealPoint mySubstrPtr = mySnapPts.Item(p) - clickedpoint;
        double myDistance = fabs(sqrt((mySubstrPtr.x * mySubstrPtr.x) + (mySubstrPtr.y * mySubstrPtr.y)));
        if (p == 0) {
            myMinDistance = myDistance;
        }
        else{
            if(myDistance < myMinDistance){
                myMinDistance = myDistance;
                myMinItemIndex = p;
            }
        }
    }
    return new wxRealPoint(mySnapPts.Item(myMinItemIndex));
}



/***************************************************************************//**
 @brief Delete the geometry and attribution of selected object
 @param Clearselection true if we sould clear selection ids, if you need to
 delete more stuff (attribution for exemple) set to false
 @return true if geometry are deleted
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
bool tmEditManager::DeleteSelected(bool Clearselection)
{
	// make some checks 
	if (!IsDrawingAllowed() || m_SelectedData->GetCount() <= 0){
		return false;
    }
	
	// delete ids from database
	wxArrayLong * mySelectedIds = m_SelectedData->GetSelectedValues();
	if (mySelectedIds == NULL){
		return false;
	}

    wxBusyCursor wait;
    for (unsigned int i = 0; i< mySelectedIds->GetCount(); i++) {
        wxArrayLong myFeatures = m_pDB->GetObjectsFromFeature(mySelectedIds->Item(i),
                                                              m_TOC->GetEditLayer()->GetType());
        for (unsigned int f = 0; f < myFeatures.GetCount(); f++) {
            m_pDB->DeleteAdvancedAttribution(mySelectedIds->Item(i), 
                                             m_pDB->GetSelectedLayerId(myFeatures.Item(f)));
        }
    }
	m_pDB->DeleteAttribution(mySelectedIds, m_TOC->GetEditLayer()->GetType());
	m_pDB->DeleteGeometry(mySelectedIds, m_TOC->GetEditLayer()->GetType());
    delete mySelectedIds;
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	return true;
}



/***************************************************************************//**
 @brief Remove the last vertex from memory
 @details This function will only work while editing lines, calling this for
 points wont work
 @return  true if the last vertex was successfully removed
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
bool tmEditManager::UndoLastVertex ()
{
	// zero vertex or not in drawing mode
	if (!IsDrawingAllowed() || m_GISMemory->GetVertexCount() == 0)
		return false;
	
	// one vertex, destroying feature 
	if (m_GISMemory->GetVertexCount() == 1) {
		wxCommandEvent evt;
		OnDrawFeatureEscape(evt);
		return true;
	}
	
	
	// remove last vertex
	m_GISMemory->RemoveVertex(-1);
	wxRealPoint myPreviousRPT;
	bool bGet = m_GISMemory->GetVertex(myPreviousRPT, -1);
	wxASSERT(bGet);
	wxPoint myPreviousPT = m_Scale->RealToPixel(myPreviousRPT);
	m_DrawLine.CreateVertex(myPreviousPT);
	
	
	DrawMemoryData(true);

	// display last segment (in video inverse)
	wxCommandEvent evt(tmEVT_EM_DRAW_MOVE, wxID_ANY);
	wxPoint * myClickedPos = new wxPoint(m_LastMousePos.x,
										 m_LastMousePos.y);
	evt.SetClientData(myClickedPos);
	OnDrawMove(evt);
	return true;
}


bool tmEditManager::HasLastVertex(){
	wxASSERT(m_GISMemory);
	if (m_GISMemory->GetVertexCount() > 0) {
		return true;
	}
	return false;
}



/***************************************************************************//**
 @brief Number of selected features
 @return  the number of features selected
 @author Lucien Schreiber (c) CREALP 2009
 @date 12 May 2009
 *******************************************************************************/
int tmEditManager::GetSelectionCount()
{
	return m_SelectedData->GetCount();
}


/***************************************************************************//**
 @brief Create intersections
 @details between the selected line and all crossed lines.
 @return  true if all works, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 09 February 2009
 *******************************************************************************/
bool tmEditManager::CreateIntersections ()
{
	// checks (editing mode and 1 line selected)
    if (IsObjectSelected() == false) {
        return false;
    }
    
    if (m_TOC->GetEditLayer()->GetSpatialType() != LAYER_SPATIAL_LINE) {
        return false;
    }
 
	// Get the Layer (Line MySQL) 
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (!mySelLayer){
		return false;
	}
		
	OGRFeature * myFeature = mySelLayer->GetFeatureByOID(m_SelectedData->GetSelectedUnique());
	wxASSERT(myFeature);
	
	
	OGRLineString * myOGRSelLine = (OGRLineString*) myFeature->GetGeometryRef();
	wxASSERT(myOGRSelLine);
	
	// Get all line crossing
	wxArrayLong * myLinesCrossing = mySelLayer->SearchIntersectingGeometry(myOGRSelLine);
	if (myLinesCrossing == NULL || myLinesCrossing->GetCount() == 0)
	{
		OGRFeature::DestroyFeature(myFeature);
		wxLogWarning(_T("No intersections found"));
		return false;
	}
	
	OGRMultiLineString LinesCrossing;
	for (unsigned int j = 0; j<myLinesCrossing->GetCount();j++)
	{
		OGRFeature * myTempFeature = mySelLayer->GetFeatureByOID(myLinesCrossing->Item(j));
		wxASSERT(myTempFeature);
		LinesCrossing.addGeometry(myTempFeature->GetGeometryRef());
		OGRFeature::DestroyFeature(myTempFeature);
	}
			
	// for selected lines, compute all intersections (with all lines)
	wxArrayLong myInsertedIDs1;
	OGRMultiLineString selectedsegments;
	mySelLayer->CutLineMultiple(myOGRSelLine, &LinesCrossing, selectedsegments);
	mySelLayer->SplitGeometry (&selectedsegments, m_SelectedData->GetSelectedUnique(),
							   myInsertedIDs1, m_TOC->GetEditLayer()->GetType());
		
	// add attributions for new segment of selected line
    // only if lines (not frame)
    if (m_TOC->GetEditLayer()->GetType() == TOC_NAME_LINES) {
        wxCommandEvent attribevt1(tmEVT_AM_COPY_ATTRIBUTION, wxID_ANY);
        attribevt1.SetExtraLong(m_SelectedData->GetSelectedUnique());
        wxArrayLong * myTempArray = new wxArrayLong(myInsertedIDs1); 
        attribevt1.SetClientData(myTempArray);
        m_ParentEvt->GetEventHandler()->AddPendingEvent(attribevt1);
    }
		
	// compute intersections for other lines
	OGRMultiLineString myRes1;
	OGRMultiLineString myRes2;
	wxArrayLong myInsertedIDs2;
	wxCommandEvent attribevt2(tmEVT_AM_COPY_ATTRIBUTION, wxID_ANY);
	// for all crossing line, compute intersections
	for (unsigned int i = 0 ; i< myLinesCrossing->GetCount(); i++)
	{
		OGRLineString * myCrossedL = (OGRLineString*) LinesCrossing.getGeometryRef(i);
		if (myCrossedL)
		{
			mySelLayer->CutLineGeometry(myOGRSelLine, myCrossedL, 
										myRes1,	myRes2);
			mySelLayer->SplitGeometry(&myRes2, myLinesCrossing->Item(i),
									  myInsertedIDs2,  m_TOC->GetEditLayer()->GetType());
			myRes1.empty();
			myRes2.empty();
			
			// send attribution message
            if (m_TOC->GetEditLayer()->GetType() == TOC_NAME_LINES) {
                attribevt2.SetExtraLong(myLinesCrossing->Item(i));
                wxArrayLong * myTempArray2 = new wxArrayLong(myInsertedIDs2); 
                attribevt2.SetClientData(myTempArray2);
                m_ParentEvt->GetEventHandler()->AddPendingEvent(attribevt2);
            }	
            myInsertedIDs2.Clear();

		}
	}
	OGRFeature::DestroyFeature(myFeature);
	wxDELETE(myLinesCrossing);
	wxDELETE(mySelLayer);
	
    // Check lines for error (two vertex overlapping)
    wxArrayLong myErrLines;
    myErrLines = _GetErrorLines(myInsertedIDs1);
    if (myErrLines.GetCount() > 0) {
        wxLogDebug("Incorrect lines found, removing them!");
        m_SelectedData->Clear();
        m_SelectedData->AddSelected(&myErrLines);
        DeleteSelected();
        for (unsigned int i = 0; i< myErrLines.GetCount(); i++) {
            myInsertedIDs1.Remove(myErrLines.Item(i));
        }
    }
    
	// add segment to selection
	m_SelectedData->AddSelected(&myInsertedIDs1);
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
	
	
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	
	// send statistics
	wxCommandEvent statevt(tmEVT_STAT_INTERSECTION, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(statevt);
	
	return true;	
}



wxArrayLong tmEditManager::_GetErrorLines(wxArrayLong linetocheck){
    wxArrayLong myResults;
    wxASSERT(myResults.GetCount() == 0);
    
    // Get the Layer (Line MySQL) 
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (!mySelLayer){
        wxLogError(_("Checking intersected lines failed! Unable to get the 'in edition' layer!"));
		return myResults;
	}
    
    for (long i = 0; i < linetocheck.GetCount(); i++) {
        OGRFeature * myFeature = mySelLayer->GetFeatureByOID(linetocheck.Item(i));
        if (myFeature == NULL) {
            wxLogError(_("Unable to get feature : %ld! Checking intersected lines failed"), linetocheck.Item(i));
            continue;
        }
        OGRLineString * myOGRSelLine = (OGRLineString*) myFeature->GetGeometryRef();
        wxASSERT(myOGRSelLine);
        
        // line error, only one vertex!
        if (myOGRSelLine->getNumPoints() == 1) {
            myResults.Add(linetocheck.Item(i));
            OGRFeature::DestroyFeature(myFeature);
            continue;
        }
        
        OGRPoint p1;
        OGRPoint p2;
        
        myOGRSelLine->getPoint(0, &p1);
        myOGRSelLine->getPoint(1, &p2);
        
        // line error 2 vertex overlapping
        if (p1.Equals(&p2) && myOGRSelLine->getNumPoints() == 2) {
            myResults.Add(linetocheck.Item(i));
        }
        
        OGRFeature::DestroyFeature(myFeature);
    }
    return myResults;
}




/***************************************************************************//**
 @brief Display a dialog for editing vertex position
 @details Checks are done internally for ensuring that :
 - An object is selected
 - Editing layer is selected
 @return  true if Editing vertex is allowed, false otherwise
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
bool tmEditManager::EditVertexPosition ()
{
	// validation
	if (IsModifictionAllowed()==false)
		return false;
	
	// get the editing layers
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (!mySelLayer)
		return false;
	
	// getting geometry info
	long lSelectedOID = m_SelectedData->GetSelectedUnique();
	OGRFeature * myFeature = mySelLayer->GetFeatureByOID(lSelectedOID);
	wxASSERT(myFeature);
	OGRGeometry * myGeom = myFeature->GetGeometryRef(); 
	wxASSERT(myGeom);
	OGRwkbGeometryType myType =  wkbFlatten ( myGeom->getGeometryType());
	
	// preparing dialog and dialog data
	EditVertexDLG myDlg (m_Renderer);
	myDlg.m_SelectedOID = lSelectedOID;
	myDlg.m_LayerType = m_TOC->GetEditLayer()->GetSpatialType();
	OGRLineString * myLine = NULL;
	OGRPoint * myPt = NULL;
	
	switch (myType)
	{
		case wkbPoint:
			myPt = (OGRPoint*) myGeom;
			myDlg.m_VertexPts.Add(wxRealPoint(myPt->getX(), myPt->getY()));
			break;
			
		case wkbLineString:
			myLine = (OGRLineString*) myGeom;
			for (int i = 0; i< myLine->getNumPoints();i++)
				myDlg.m_VertexPts.Add(wxRealPoint(myLine->getX(i), myLine->getY(i)));
			break;
		
		default:
			OGRFeature::DestroyFeature(myFeature);
			return false;
			break;
	}
	
	// displaying dialog
	if(myDlg.ShowModal() == wxID_OK)
	{
		// apply modification
		if (myPt)
		{
			wxASSERT (myDlg.m_VertexPts.GetCount() == 1);
			myPt->setX(myDlg.m_VertexPts.Item(0).x);
			myPt->setY(myDlg.m_VertexPts.Item(0).y);
		}
		else if (myLine)
		{
			myLine->empty();
			for (unsigned int j = 0; j < myDlg.m_VertexPts.GetCount(); j++)
			{
				myLine->addPoint(myDlg.m_VertexPts.Item(j).x,
								 myDlg.m_VertexPts.Item(j).y);	
			}
		}
		mySelLayer->UpdateGeometry(myGeom, myDlg.m_SelectedOID);
	}
	OGRFeature::DestroyFeature(myFeature);
	wxDELETE(mySelLayer);
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	
	return true;
}





/***************************************************************************//**
 @brief Merge selected lines
 @details  This function Checks the following rules:
 - Minimum two objects of type lines should been selected.
 - Editing mode should be turned on.
 Merging is done without user input if all selected lines share the same attribution.
 If a line is different, a dialog-box is displayed allowing the user to choice
 the attribution he want to keep for the new merged line.
 @return  true if merging was successfull or allowed, see above
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
bool tmEditManager::MergeSelectedLines ()
{
	//verifications
    if (m_TOC->GetEditLayer()->GetType() != TOC_NAME_LINES && m_TOC->GetEditLayer()->GetType() != TOC_NAME_FRAME) {
        return false;
    }
    
    if (IsObjectMinNumberSelected(2) == false) {
        return false;
    }
	
	
	// getting selections ids
	wxArrayLong * mySelectedIDs = m_SelectedData->GetSelectedValues();
	wxASSERT (mySelectedIDs);
	
	// merge lines
	tmGISDataVector * myLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	wxASSERT (myLayer);
	OGRGeometry * myMergeGeom = NULL;
	OGRMultiLineString * mySelGeom = 
				(OGRMultiLineString*) myLayer->GetGeometryColByOID (mySelectedIDs);
	bool bMergeSuccess = myLayer->LinesMerge(mySelGeom, &myMergeGeom);

	
	OGRGeometryFactory::destroyGeometry(mySelGeom);
	if (bMergeSuccess == false)
	{
		wxMessageBox(_("Selected lines are not adjacent"),
					 _("Merge failed"), wxOK | wxICON_ERROR);
		return false;
	}
	
    // don't search attribution for frame.
    long myKeptOid = wxNOT_FOUND;
	if (m_TOC->GetEditLayer()->GetType() == TOC_NAME_LINES) {
        // get attributions
        tmAttributionBasicArray myAttributions;
        tmAttributionDataLine myAttrib (m_SelectedData->GetSelectedValues(), m_pDB);
        if (myAttrib.GetInfoBasicArray(myAttributions) == false)
        {
            wxLogDebug(_T("Unable to get attributions from selected lines"));
            return false;
        }
        
        //FIXME: remove, this is temp logging code
        for (unsigned int k = 0; k<myAttributions.GetCount();k++)
        {
            wxString sTmp = wxString::Format(_T("Values for OID : %ld are : "), 
                                             myAttributions.Item(k).m_Oid);
            for (unsigned int m = 0; m< myAttributions.Item(k).m_Values.GetCount() ;m++)
            {
                sTmp.Append(wxString::Format(_T("%ld, "), myAttributions.Item(k).m_Values.Item(m)));
            }
            
            wxLogDebug(sTmp);
        }
        //END 
        
        // compare attributions
        int iLineToKeep = 0;
        wxArrayString myConcatenedAttrib;
        if (tmAttributionData::IsAttributionSimilar(myAttributions) == false)
        {
            // get attribution name 
            if (!myAttrib.GetConcatenedBasicName(myAttributions, myConcatenedAttrib))
            {
                wxLogError(_T("Error getting concatenated basic values"));
                return false;
            }
            
            // display dialog
            wxSingleChoiceDialog myDlg (m_ParentEvt,
                                        _("Select the attribution to be kept"),
                                        _("Merge lines"),
                                        myConcatenedAttrib);
            if (myDlg.ShowModal()==wxID_CANCEL)
                return false;
            
            iLineToKeep = myDlg.GetSelection();
        }
        
        // remove lines
        myKeptOid = myAttributions.Item(iLineToKeep).m_Oid;
        wxLogDebug(_T("We keep : OID %ld"), myKeptOid);
        mySelectedIDs->RemoveAt(iLineToKeep);
    }
    else{
        // arbitriary keep line 0!
        myKeptOid = mySelectedIDs->Item(0);
        mySelectedIDs->RemoveAt(0);
    }
    
	m_pDB->DeleteGeometry(mySelectedIDs, m_TOC->GetEditLayer()->GetType());
	m_pDB->DeleteAttribution(mySelectedIDs, m_TOC->GetEditLayer()->GetType());

	// update number of selected features
	m_SelectedData->Clear();
	m_SelectedData->SetSelected(myKeptOid);
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
    m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);
	
	// update geometry
	wxASSERT (myMergeGeom);
	myLayer->UpdateGeometry(myMergeGeom, myKeptOid);
	OGRGeometryFactory::destroyGeometry(myMergeGeom);
		
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
		
	return true;
}


void tmEditManager::OnSetRenderFocus (wxCommandEvent & event)
{
	wxASSERT(m_Renderer);
	m_Renderer->SetFocus();
}



void tmEditManager::OnEditSharedDown (wxCommandEvent & event){
	wxLogMessage(_("Searching shared node"));
	wxPoint * myTempPt = (wxPoint*) event.GetClientData();
	wxASSERT (myTempPt);
	wxPoint myLocalPt = *myTempPt;
	wxDELETE(myTempPt);
	
	if (SelectedSearch(myLocalPt) == false) {
		return;
	}
	
	// update display and selection
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	wxCommandEvent evt(tmEVT_SELECTION_DONE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt);

	// update display imediatly
	//m_ParentEvt->ReloadLayerNow();

	OGRPoint myPt;
	wxRealPoint myRealPt = m_Scale->PixelToReal(myLocalPt);
	myPt.setX(myRealPt.x);
	myPt.setY(myRealPt.y);
	OGRGeometry * myRect = myPt.Buffer(tmSELECTION_DIAMETER / 2.0);
	wxASSERT(myRect);
	
	// clear shared nodes
	m_SharedNodes.Clear();
	
	// search end nodes
	wxArrayLong * mySelIds = m_SelectedData->GetSelectedValues();
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (mySelLayer == NULL){
		return;
	}
	
	for (unsigned int i = 0; i<mySelIds->GetCount(); i++) 
	{
		OGRFeature * myFeature = mySelLayer->GetFeatureByOID(mySelIds->Item(i));
		wxASSERT(myFeature);
		OGRLineString * myLine = (OGRLineString*) myFeature->GetGeometryRef(); 
		wxASSERT(myLine);
		OGRPoint myNode;
		int myTotalPoints = myLine->getNumPoints();
		myLine->getPoint(0, &myNode);
		int myNodeID = wxNOT_FOUND;
		int myNodePreviousID = wxNOT_FOUND;
		bool bFoundNode = false;
		
		// search start node
		if (myNode.Within(myRect)) {
			wxLogMessage(_("Found node : 1"));
			myNodeID = 0;
			myNodePreviousID = 1;
			wxRealPoint myCoord (myLine->getX(myNodeID), myLine->getY(myNodeID));
			wxRealPoint myPreviousCoord (myLine->getX(myNodePreviousID), myLine->getY(myNodePreviousID));
			tmSharedNodeEdit mySharedNode (mySelIds->Item(i),
										   myNodeID,
										   m_Scale->RealToPixel(myCoord),
										   m_Scale->RealToPixel(myPreviousCoord));
			m_SharedNodes.Add(mySharedNode);
			bFoundNode = true;
		}
		
		// search end node
		myLine->getPoint(myTotalPoints -1, &myNode);
		if (myNode.Within(myRect)) {
			wxLogMessage(_("Found node : %d"), myTotalPoints -2);
			myNodeID = myTotalPoints -1;
			myNodePreviousID = myTotalPoints -2;
			wxRealPoint myCoord (myLine->getX(myNodeID), myLine->getY(myNodeID));
			wxRealPoint myPreviousCoord (myLine->getX(myNodePreviousID), myLine->getY(myNodePreviousID));
			tmSharedNodeEdit mySharedNode (mySelIds->Item(i),
										   myNodeID,
										   m_Scale->RealToPixel(myCoord),
										   m_Scale->RealToPixel(myPreviousCoord));
			m_SharedNodes.Add(mySharedNode);
			bFoundNode = true;
		}
		OGRFeature::DestroyFeature(myFeature);
		
		if (bFoundNode == false) {
			wxLogError(_("No Node found for line : %ld"), mySelIds->Item(i));
		}
	}
    
    if (m_SharedNodes.GetCount() < 2) {
        m_SharedNodes.Clear();
    }
	OGRGeometryFactory::destroyGeometry(myRect);
}



void tmEditManager::OnEditSharedUp (wxCommandEvent & event){
	wxPoint * myTempPt = (wxPoint*) event.GetClientData();
	wxASSERT (myTempPt);
	
	
	if (m_SharedNodes.GetCount() == 0) {
		wxDELETE(myTempPt);
		return;
	}
	
	
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (mySelLayer == NULL){
		m_SharedNodes.Clear();
		wxDELETE(myTempPt);
		return;
	}
	
	// clear DC
	{
		wxClientDC dc( m_Renderer );
		wxDCOverlay overlaydc(m_OverlaySharedNodes, &dc );
		overlaydc.Clear();
	}
	m_OverlaySharedNodes.Reset();
	
	
	wxRealPoint myNewCoord = m_Scale->PixelToReal(*myTempPt);
	for (unsigned int i = 0; i<m_SharedNodes.GetCount(); i++) {
		OGRFeature * myFeature = mySelLayer->GetFeatureByOID(m_SharedNodes.Item(i).GetLineID());
		wxASSERT(myFeature);
		OGRLineString * myLine = (OGRLineString*) myFeature->GetGeometryRef(); 
		wxASSERT(myLine);
		
		myLine->setPoint(m_SharedNodes.Item(i).GetVertexID(), myNewCoord.x, myNewCoord.y, 0);
		mySelLayer->UpdateGeometry(myLine, m_SharedNodes.Item(i).GetLineID());
		OGRFeature::DestroyFeature(myFeature);
	}
		
	m_SharedNodes.Clear();
	wxDELETE(myTempPt);
	
	// update display with msg
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);	
}



void tmEditManager::OnEditSharedMove(wxCommandEvent & event){
	wxPoint * myTempPt = (wxPoint*) event.GetClientData();
	wxASSERT (myTempPt);
	
	wxClientDC myDC (m_Renderer);
	wxDCOverlay myOverlayDC (m_OverlaySharedNodes, &myDC);
	myOverlayDC.Clear();
#ifdef __WXMAC__
	myDC.SetPen( *wxGREY_PEN );
#else
	myDC.SetPen( wxPen( *wxLIGHT_GREY, 2, wxSOLID ) );
#endif
		
	for (unsigned int i = 0; i<m_SharedNodes.GetCount(); i++) {
		m_SharedNodes.Item(i).SetCoordVertex(*myTempPt);
		m_SharedNodes.Item(i).DrawLine(&myDC);
	}
	wxDELETE(myTempPt);
	
}




bool tmEditManager::FlipLine(){
	if (IsLayerTypeSelected(LAYER_SPATIAL_LINE) == false){
		return false;
	}
	
	// Get the Layer (Line MySQL) 
	tmGISDataVector * mySelLayer = (tmGISDataVector*) tmGISData::LoadLayer(m_TOC->GetEditLayer());
	if (mySelLayer == NULL){
		return false;
	}
	
    wxArrayLong * mySelected = m_SelectedData->GetSelectedValues();
    wxASSERT(mySelected);
    wxBusyCursor myBusyCursor;
    
    for (unsigned int f = 0; f< mySelected->GetCount(); f++) {
        OGRFeature * myFeature = mySelLayer->GetFeatureByOID(mySelected->Item(f));
        wxASSERT(myFeature);
        
        // don't delete, internally geometry.
        OGRLineString * myOGRSelLine = (OGRLineString*) myFeature->GetGeometryRef();
        wxASSERT(myOGRSelLine);
        
        OGRLineString * myTmpLine = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
        wxASSERT(myTmpLine);
        for (int i = myOGRSelLine->getNumPoints()-1; i >= 0; i--) {
            OGRPoint myPoint;
            myOGRSelLine->getPoint(i, &myPoint);
            myTmpLine->addPoint(&myPoint);
        }
        wxLogMessage(_("Line  %ld flipped"), myFeature->GetFID());
        mySelLayer->UpdateGeometry(myTmpLine, myFeature->GetFID());
        OGRGeometryFactory::destroyGeometry(myTmpLine);
        OGRFeature::DestroyFeature(myFeature);
    }
	wxDELETE(mySelLayer);
	
    //wxNotificationMessage myMessage( _("Flipping Done!"), wxString::Format(_("%ld line(s) flipped!"), mySelected->GetCount()),m_ParentEvt);
    //myMessage.Show();
	// update display
	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	m_ParentEvt->GetEventHandler()->AddPendingEvent(evt2);
	return true;
}













tmSharedNodeEdit::tmSharedNodeEdit(long lineid, int vertexid, 
								   const wxPoint & coord,
								   const wxPoint & coordprevious) {
	m_LineID = lineid;
	m_VertexID = vertexid;
	m_CoordVertex = coord;
	m_CoordVertexPrevious = coordprevious;
}

tmSharedNodeEdit::~tmSharedNodeEdit() {
}


void tmSharedNodeEdit::DrawLine(wxClientDC * dc, wxPoint * point) {
	wxASSERT(dc);
	wxPoint myTempPoint = m_CoordVertex;
	if (point) {
		myTempPoint = *point;
	}
	dc->DrawLine(m_CoordVertexPrevious, myTempPoint);	
}


#include <wx/arrimpl.cpp> // This is a magic incantation which must be done!
WX_DEFINE_OBJARRAY(tmArraySharedNodes);







