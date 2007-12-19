/***************************************************************************
							project_def.h
						Project definition dialog
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

#include "project_def_dlg.h"


IMPLEMENT_DYNAMIC_CLASS( ProjectDefDLG, wxDialog )



BEGIN_EVENT_TABLE( ProjectDefDLG, wxDialog )
	EVT_FLATBUTTON(ID_DLGPD_SPAT_MDL_ADD, ProjectDefDLG::OnAddLayer)
	EVT_BUTTON(ID_DLGPD_PROJ_PATH_BTN, ProjectDefDLG::OnSelectProjectPath)
    EVT_FLATBUTTON(ID_DLGPD_SPAT_MDL_DEL,  ProjectDefDLG::OnRemoveLayer)
END_EVENT_TABLE()


void ProjectDefDLG::OnAddLayer(wxCommandEvent & event)
{
	// create a new object for storing fields value
	ProjectDefMemoryLayers * myMemLayersValue = new ProjectDefMemoryLayers();
	wxArrayString myListValues;

	
	m_LayerDialog = new ProjectDefLayersDlg (this);
	// transfert the data obj to the dialog, data will be 
	// filled during DataTransfer...
	m_LayerDialog->SetMemoryLayersObject(myMemLayersValue); 
	
	
	if (m_LayerDialog->ShowModal()==wxID_OK)
	{
		
		m_LayersArray.Add(myMemLayersValue);
		
		// prepare data for list representation
		myListValues.Add(myMemLayersValue->m_LayerName);
		myListValues.Add(PRJDEF_LAYERS_TYPE_STRING[myMemLayersValue->m_LayerType]);
		m_DlgPd_Stat_Model_List->EditDataToList(myListValues);
	}
	else
	{
		// we cancel so we don't need to keep
		// this Layers Object in memory
		delete myMemLayersValue;
		wxLogDebug(_T("Deleting Memory layers object not used"));
	}
	wxLogDebug(_T("Size of Layers array %d"), m_LayersArray.GetCount());
	delete m_LayerDialog;
	

}

void ProjectDefDLG::OnRemoveLayer (wxCommandEvent & event)
{
	// remove the object from the field array
	// if found and then remove the object from the list
	RemoveObjFromArray();
	m_DlgPd_Stat_Model_List->DeleteSelectedItem();
}

void ProjectDefDLG::RemoveObjFromArray()
{
	// if a corresponding item was found, remove it from the array
	int iItemIndex = FindObjInLayersArray(m_DlgPd_Stat_Model_List, m_LayersArray);
	if ( iItemIndex != -1)
	{
		m_LayersArray.RemoveAt(iItemIndex);
	}	
}

void ProjectDefDLG::OnSelectProjectPath (wxCommandEvent & event)
{
	wxDirDialog myDirDialog (this);
	if( myDirDialog.ShowModal() == wxID_OK)
	{
		m_DlgPD_Proj_Path->SetValue(myDirDialog.GetPath());
	}
}


ProjectDefDLG::ProjectDefDLG()
{
    Init();
}

ProjectDefDLG::ProjectDefDLG( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}



bool ProjectDefDLG::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();

    return true;
}




ProjectDefDLG::~ProjectDefDLG()
{

}



void ProjectDefDLG::Init()
{
    m_DlgPD_Proj_Path = NULL;
    m_DlgPd_Proj_Name = NULL;
    m_DlgPd_Proj_Unit = NULL;
    m_DlgPd_Proj_Projection = NULL;
    m_DlgPd_Stat_Model_List = NULL;
	m_LayerDialog = NULL;
}



void ProjectDefDLG::CreateControls()
{    
    ProjectDefDLG* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Project properties"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(4, 3, 0, 0);
    itemFlexGridSizer4->AddGrowableCol(1);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Project path :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgPD_Proj_Path = new wxTextCtrl( itemDialog1, ID_DLGPD_PROJ_PATH, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer4->Add(m_DlgPD_Proj_Path, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton * myDlgPd_Proj_Path_Btn = new wxButton( itemDialog1, ID_DLGPD_PROJ_PATH_BTN, _("..."), wxDefaultPosition, wxSize(50, -1), 0 );
    itemFlexGridSizer4->Add(myDlgPd_Proj_Path_Btn, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Project name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgPd_Proj_Name = new wxTextCtrl( itemDialog1, ID_DLGPD_PROJ_NAME, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(m_DlgPd_Proj_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer4->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Units"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_DlgPd_Proj_UnitStrings;
    m_DlgPd_Proj_UnitStrings.Add(_("Meters"));
    m_DlgPd_Proj_UnitStrings.Add(_("Kilometers"));
    m_DlgPd_Proj_UnitStrings.Add(_("Lat / Long"));
    m_DlgPd_Proj_Unit = new wxChoice( itemDialog1, ID_DLGPD_PROJ_UNIT, wxDefaultPosition, wxDefaultSize, m_DlgPd_Proj_UnitStrings, 0 );
    m_DlgPd_Proj_Unit->SetStringSelection(_("Meters"));
    itemFlexGridSizer4->Add(m_DlgPd_Proj_Unit, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer4->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemDialog1, wxID_STATIC, _("Projection :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_DlgPd_Proj_ProjectionStrings;
    m_DlgPd_Proj_ProjectionStrings.Add(_("No projection"));
    m_DlgPd_Proj_ProjectionStrings.Add(_("Swiss projection"));
    m_DlgPd_Proj_Projection = new wxChoice( itemDialog1, ID_DLGPD_PROJ_PROJECTION, wxDefaultPosition, wxDefaultSize, m_DlgPd_Proj_ProjectionStrings, 0 );
    m_DlgPd_Proj_Projection->SetStringSelection(_("No projection"));
    itemFlexGridSizer4->Add(m_DlgPd_Proj_Projection, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer16Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Define your spatial model"));
    wxStaticBoxSizer* itemStaticBoxSizer16 = new wxStaticBoxSizer(itemStaticBoxSizer16Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer16, 1, wxGROW|wxALL, 5);

    m_DlgPd_Stat_Model_List = new ProjectDefList( itemDialog1, ID_DLGPD_SPAT_MODEL_LIST, wxSize(-1,150),this);
    itemStaticBoxSizer16->Add(m_DlgPd_Stat_Model_List, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer16->Add(itemBoxSizer18, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlatButton * myFlatButton = new wxFlatButton(itemDialog1, ID_DLGPD_SPAT_MDL_ADD, wxFLATBUTTON_TEXT_ADD);
	itemBoxSizer18->Add(myFlatButton, 0,wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

	
	wxFlatButton * myFlatButton2 = new wxFlatButton(itemDialog1, ID_DLGPD_SPAT_MDL_DEL, wxFLATBUTTON_TEXT_REMOVE);
    itemBoxSizer18->Add(myFlatButton2, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer21 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer21, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton22 = new wxButton( itemDialog1, wxID_OK, _("&Create new project"), wxDefaultPosition, wxDefaultSize, 0 );
 	itemStdDialogButtonSizer21->AddButton(itemButton22);

    wxButton* itemButton23 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer21->AddButton(itemButton23);

    itemStdDialogButtonSizer21->Realize();
}


ProjectDefList::ProjectDefList(wxWindow * parent, wxWindowID  id, wxSize size, ProjectDefDLG * myParentDlg) 
	: ListGenReport(parent,id,size)
{
	m_ParentDlg = myParentDlg;
	
	// Create columns
	wxArrayString myColNames;
	wxArrayInt myColsWidths;
	
	myColNames.Add(_("Name"));
	myColNames.Add(_("Type"));
	
	myColsWidths.Add(250);
	myColsWidths.Add(80);
	
	CreateColumns(&myColNames, &myColsWidths);
	
	m_ChoiceToChange = NULL;
	
}

ProjectDefList::~ProjectDefList()
{

}

void ProjectDefList::OnPressBackSpace (wxListEvent & event)
{
	// delete selected item but also in the array
	m_ParentDlg->RemoveObjFromArray();
	DeleteSelectedItem();
}



