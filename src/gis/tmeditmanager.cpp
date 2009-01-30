/***************************************************************************
								tmeditmanager.cpp
				Deals with editing data in construction layers
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

#include "tmeditmanager.h"


BEGIN_EVENT_TABLE(tmEditManager, wxEvtHandler)
	EVT_COMMAND (wxID_ANY, tmEVT_SNAPPING_UPDATED, tmEditManager::OnSnappingChange)
	EVT_COMMAND (wxID_ANY, tmEVT_VIEW_REFRESHED, tmEditManager::OnViewUpdated)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_DRAW_CLICK, tmEditManager::OnDrawClicked)
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
tmEditManager::tmEditManager(wxWindow * parent,tmTOCCtrl * toc,
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

	m_ParentEvt->PushEventHandler(this);

}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
tmEditManager::~tmEditManager()
{
	m_ParentEvt->PopEventHandler(false);
	m_ParentEvt->SetEventHandler(m_ParentEvt);
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



/***************************************************************************//**
 @brief User select modify tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 January 2009
 *******************************************************************************/
void tmEditManager::OnToolModify ()
{
	wxASSERT (m_Renderer);
	m_Renderer->SetTool(tmTOOL_MODIFY);
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
	wxLogDebug(_T("View updated"));
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
	else
		wxLogDebug(_T("Snapping memory or scale not defined"));
}



/***************************************************************************//**
 @brief Checks that a layer is selected
 @details 
 @return  true if : a layer is selected and this selected layer is <
 TOC_NAME_NOT_GENERIC
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
bool tmEditManager::IsCorrectLayerSelected()
{
	wxASSERT(m_TOC);
	tmLayerProperties * tmFirstSelected =  m_TOC->GetSelectionLayer();
	if (!tmFirstSelected)
	{
		wxLogMessage(_("No layer selected, select a layer for drawing"));
		return false;
	}	
	
	if (tmFirstSelected->m_LayerType >= TOC_NAME_NOT_GENERIC)
	{
		wxLogMessage(_("Selected layer isn't editable, select an other layer"));
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
	if (m_SelectedData->GetCount() == 1)
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
bool tmEditManager::IsDrawingAllowed()
{
	return IsCorrectLayerSelected();
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
	if (IsCorrectLayerSelected() && IsObjectSelected())
		return true;
		
	return false;
}



/***************************************************************************//**
 @brief Called when a click is issued with Draw tool
 @author Lucien Schreiber (c) CREALP 2009
 @date 28 January 2009
 *******************************************************************************/
void tmEditManager::OnDrawClicked (wxCommandEvent & event)
{
	// get coordinate and dont forget to delete it
	wxPoint * myPxCoord = (wxPoint*) event.GetClientData();
	wxRealPoint myRealCoord = m_Scale->PixelToReal(*myPxCoord);
	wxRealPoint * mySnapCoord = NULL;
	
	
	// check drawing allowed
	if (IsDrawingAllowed())
		wxLogDebug(_T("Adding data to layer"));
	

	// check snapping if needed
	//TODO: add support for stopping snapping with keyboard 
	if (m_SnapMem->IsSnappingEnabled())
	{
		mySnapCoord = IterateAllSnappingLayers(myRealCoord);
		
		//TODO: Remove this temp logging code
		if (mySnapCoord)
			wxLogDebug(_T("Point found for snapping @ : %.*f, %.*f"),
					   2, mySnapCoord->x, 
					   2, mySnapCoord->y);
		else
			wxLogDebug(_T("No snapping found"));
		// END of temp logging code
	}
	
	if (mySnapCoord)
		delete mySnapCoord;
	delete myPxCoord;
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
wxRealPoint * tmEditManager::IterateAllSnappingLayers(const wxRealPoint & clickedpoint)
{
	long myLayerId = 0;
	int mySnapStatus = tmSNAPPING_OFF;
	tmLayerProperties * myActualLayer = NULL;
	
	wxRealPoint * myReturnedSnappedPoint = NULL;
	wxRealPoint mySnapPoint;
	
	for (unsigned int i = 0; i< m_SnapMem->GetCount(); i++)
	{
		m_SnapMem->GetSnappingInfo(i, myLayerId, mySnapStatus);
		myActualLayer = m_TOC->GetLayerById(myLayerId);
		if (!myActualLayer)
			break;
		
		// search snapping for that layer
		tmGISData * myActualGISData = tmGISData::LoadLayer(myActualLayer);
		if (!myActualGISData)
			break;
				
		if (myActualGISData->GetSnapCoord(clickedpoint,
									  m_SnapMem->GetTolerence(),
									  mySnapPoint,mySnapStatus))
		{
			myReturnedSnappedPoint = new wxRealPoint(mySnapPoint);
			delete myActualGISData;
			break;
		}
		delete myActualGISData;
	}
	return myReturnedSnappedPoint;
}

