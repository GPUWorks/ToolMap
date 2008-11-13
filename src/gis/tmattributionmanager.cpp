/***************************************************************************
							tmattributionmanager.cpp
							Manage the attribution
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

#include "tmattributionmanager.h"


BEGIN_EVENT_TABLE(tmAttributionManager, wxEvtHandler)
	EVT_COMMAND (wxID_ANY, tmEVT_SELECTION_DONE, tmAttributionManager::OnSelection)
	EVT_COMMAND (wxID_ANY, tmEVT_ATTRIBUTION_BTN_PRESSED, tmAttributionManager::OnAttributeBtn)
	EVT_COMMAND (wxID_ANY, tmEVT_INFO_BTN_PRESSED, tmAttributionManager::OnInfoBtn)
	EVT_COMMAND (wxID_ANY, tmEVT_QUERY_RUN, tmAttributionManager::OnRunQuery)
END_EVENT_TABLE()


/***************************************************************************//**
 @brief Constructor
 @param parent Adress of the parent, used for event processing
 @param toc Adress of a #tmTOCCtrl
 @param panel Adress of a #AttribObjType_PANEL object
 @param selection Adress of a #tmSelectedDataMemory object
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
tmAttributionManager::tmAttributionManager(wxWindow * parent,
										   tmTOCCtrl * toc, 
										   AttribObjType_PANEL * panel,
										   tmSelectedDataMemory * selection)
{
	m_Parent = parent;
	m_TOC = toc;
	m_Panel = panel;
	m_SelData = selection;
	
	m_pDB = NULL;
	m_pLayerProperties = NULL;
	m_Parent->PushEventHandler(this);
}



/***************************************************************************//**
 @brief Destructor
 @details Doesn't do anything actually
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
tmAttributionManager::~tmAttributionManager()
{
	UnInitAttributionManager();
	m_Parent->PopEventHandler(false);
	m_Parent->SetEventHandler(m_Parent);
}



/***************************************************************************//**
 @brief Init attribution manager
 @details This function should be the first called for beeing able to use the
 attibution manager
 @return  true if Database, TOC, selection data, and panel are valid
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
bool tmAttributionManager::InitAttributionManager(DataBaseTM * pDb)
{
	m_pDB = pDb;
	m_pLayerProperties = NULL;
	
	// check validity for all objects
	return IsAttributionManagerReady();
}



/***************************************************************************//**
 @brief Uninit attribution manager
 @details Mark the attribution manager as not valid.
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
void tmAttributionManager::UnInitAttributionManager()
{
	m_pDB = NULL;
	m_pLayerProperties = NULL;
}



/***************************************************************************//**
 @brief Checks all #tmAttributionManager object for validity
 @return  true if all objects are valid, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
bool tmAttributionManager::IsAttributionManagerReady()
{
	if (!m_Parent)
		return false;
	
	if (!m_Panel)
		return false;
	
	if (!m_SelData)
		return false;
	
	if(!m_TOC)
		return false;
	
	if (!m_pDB)
		return false;
	
	return true;
}



/***************************************************************************//**
 @brief Called when selection was made
 @details This is called by the #tmRenderer when a selection (either by point or
 rectangle) was made. This event is also used by #tmLayerManager::OnSelection
 @param event Contain data about the status of the shift key and the rectangle
 size. 
 - Use event.GetInt() for getting the status of the shift key
 - Use event.GetClientObject() for getting the wxRect.
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmAttributionManager::OnSelection (wxCommandEvent & event)
{
	int iSelFeatureCount = m_SelData->GetCount();
	
	// some verifications :
	//	A layer must be selected
	//	A construction layer must be selected (< TOC_NAME_FRAME)
	m_pLayerProperties = m_TOC->GetSelectionLayer();
	if (!m_pLayerProperties)
		return;
	
	if(m_pLayerProperties->m_LayerType >= TOC_NAME_FRAME)
		return;
	
	m_Panel->SetAttributeBtn(iSelFeatureCount);
	m_Panel->SetInfoBtn(iSelFeatureCount);
	TOC_GENERIC_NAME mySelType = static_cast<TOC_GENERIC_NAME> (m_pLayerProperties->m_LayerType);
	m_Panel->SetVisibleNotebook(mySelType);
	
	// if auto display attribute is checked 
	if (m_Panel->IsAutoDisplayAttributeChecked() && iSelFeatureCount == 1)
	{
		wxCommandEvent evt (tmEVT_INFO_BTN_PRESSED, wxID_ANY);
		m_Parent->GetEventHandler()->AddPendingEvent(evt);
	}
	
	
}



/***************************************************************************//**
 @brief Called when user press attribute
 @details This event function is called by the #AttribObjType_PANEL when user
 press the attribute Button.
 @param event 
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 November 2008
 *******************************************************************************/
void tmAttributionManager::OnAttributeBtn (wxCommandEvent & event)
{
	// verification : 
	//		Selected notebook page is same type as
	//		selected layer in TOC
	wxASSERT(m_pLayerProperties);
	if (m_pLayerProperties->m_LayerType != m_Panel->GetVisibleNotebook())
	{
		wxLogDebug(_("Something incorrect, selected panel doesn't correspond to the selected layer"));
		return;
	}
	
	wxArrayLong  * mySelObjArray = m_SelData->GetSelectedValues();
	//int myLayerIndex = m_SelData->GetSelectedLayer();
	
	// create attribution object based on type
	tmAttributionData * myAttrib = CreateAttributionData(m_pLayerProperties->m_LayerType);
	myAttrib->Create(mySelObjArray, m_pDB);
	if(!myAttrib->SetAttributeBasic(m_Panel))
	{
		wxLogMessage(_("Unable to attribute those data"));
	}
	
	delete myAttrib;

}



/***************************************************************************//**
 @brief Called when user press the info button
 @details This function is called by the #AttribObjType_PANEL
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 November 2008
 *******************************************************************************/
void tmAttributionManager::OnInfoBtn (wxCommandEvent & event)
{
	wxLogDebug(_T("Getting info"));
	
	// create attribution object based on type
	wxASSERT(m_pLayerProperties);
	wxArrayLong  * mySelObjArray = m_SelData->GetSelectedValues();
	tmAttributionData * myAttrib = CreateAttributionData(m_pLayerProperties->m_LayerType);
	myAttrib->Create(mySelObjArray, m_pDB);
	if (!myAttrib->GetInfoBasic(m_Panel))
	{
		wxLogError(_("Error getting informations for objects"));
	}
	
	delete myAttrib;
	
	
}




/***************************************************************************//**
 @brief Create an object of #tmAttributionData type
 @details This function may be used for getting a valid pointer on one of the
 #tmAttributionData derived class.
 @param type one of the following : 
 - TOC_NAME_LINES 
 - TOC_NAME_LABELS
 - TOC_NAME_POINTS 
 - TOC_NAME_ANNOTATION
 @return  A valid pointer of type #tmAttributionData class
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionData * tmAttributionManager::CreateAttributionData (int type)
{
	tmAttributionData * myAttrib = NULL;
	switch (type)
	{
		case TOC_NAME_LINES:
			myAttrib = new tmAttributionDataLine();
			break;
			
		case TOC_NAME_POINTS:
			myAttrib = new tmAttributionDataPoint();
			break;
			
		case TOC_NAME_LABELS:
			myAttrib = new tmAttributionDataLabel();
			break;
			
		default:
			myAttrib = new tmAttributionData();
			break;
	}
	return myAttrib;
}



/***************************************************************************//**
 @brief Received when user select run Query
 @details This function receive the query from the #Queries_PANEL. The query may
 be retreived from GetString() function
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 November 2008
 *******************************************************************************/
void tmAttributionManager::OnRunQuery (wxCommandEvent & event)
{
	wxASSERT(m_pDB);
	wxASSERT(m_TOC);
	wxASSERT(m_SelData);
	
	wxLogDebug(_T("Running query"));
	
	// getting the query
	wxString myQuery = event.GetString();
	
	
	// getting the layer ID
	int myLayerType = event.GetInt();
	int myLayerID = m_pDB->GetLayerID(static_cast<TOC_GENERIC_NAME> (myLayerType));
	if (myLayerID == wxNOT_FOUND)
	{
		wxASSERT_MSG (0, _T("Error getting layer ID"));
		return;
	}
	// Selecting layer in TOC
	m_TOC->SetSelectedLayer(myLayerID);
		
	
	
	// passing the query
	if (!m_pDB->DataBaseQuery(myQuery))
	{
		wxString szError = wxString::Format(_("Error running the query. Bellow is the error rapported by MySQL\n%s"),
											m_pDB->DataBaseGetLastError().c_str());
		wxMessageBox(szError,
					 _("Error running the query"),
					 wxICON_EXCLAMATION | wxOK ,
					 m_Parent);
		return;
	}
	
	// query succeed, getting results
	wxArrayLong myResults;
	long myTempRes = -1;
	while (1)
	{
		myTempRes = m_pDB->DataBaseGetNextResultAsLong();
		if (myTempRes == -1)
			break;
		myResults.Add(myTempRes);
	}
	
	// clear selection
	m_SelData->Clear();
	m_SelData->SetLayerID(myLayerID);
	
	m_SelData->AddSelected(&myResults);
	
	// update display
	wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
	
	// update attribution panel
	wxCommandEvent tmpevt;
	OnSelection(tmpevt);
	
}
