/////////////////////////////////////////////////////////////////////////////
// Name:        addspatiallayer.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Tue 21 Aug 14:06:29 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Tue 21 Aug 14:06:29 2007

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "addspatiallayer.h"

////@begin XPM images
////@end XPM images


/*!
 * ProjectDefLayersDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ProjectDefLayersDlg, wxDialog )


/*!
 * ProjectDefLayersDlg event table definition
 */

BEGIN_EVENT_TABLE( ProjectDefLayersDlg, wxDialog )

////@begin ProjectDefLayersDlg event table entries
////@end ProjectDefLayersDlg event table entries

END_EVENT_TABLE()


/*!
 * ProjectDefLayersDlg constructors
 */

ProjectDefLayersDlg::ProjectDefLayersDlg()
{
    Init();
}

ProjectDefLayersDlg::ProjectDefLayersDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * wxDialog creator
 */

bool ProjectDefLayersDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ProjectDefLayersDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ProjectDefLayersDlg creation
    return true;
}


/*!
 * ProjectDefLayersDlg destructor
 */

ProjectDefLayersDlg::~ProjectDefLayersDlg()
{
////@begin ProjectDefLayersDlg destruction
////@end ProjectDefLayersDlg destruction
}


/*!
 * Member initialisation
 */

void ProjectDefLayersDlg::Init()
{
////@begin ProjectDefLayersDlg member initialisation
    m_DlgPDL_Layer_Type = NULL;
    m_DlgPDL_Layer_Name = NULL;
    m_DlgPDL_Panel_Obj = NULL;
    m_DlgPDL_Object_List = NULL;
    m_DlgPDL_Panel_Fields = NULL;
    m_DlgPDL_Fields_List = NULL;
////@end ProjectDefLayersDlg member initialisation
}


/*!
 * Control creation for wxDialog
 */

void ProjectDefLayersDlg::CreateControls()
{    
////@begin ProjectDefLayersDlg content construction
    ProjectDefLayersDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Layer options"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer4->AddGrowableCol(1);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer type :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_DlgPDL_Layer_TypeStrings;
    m_DlgPDL_Layer_TypeStrings.Add(_("Point"));
    m_DlgPDL_Layer_TypeStrings.Add(_("Line"));
    m_DlgPDL_Layer_TypeStrings.Add(_("Polygon"));
    m_DlgPDL_Layer_Type = new wxChoice( itemDialog1, ID_DLGPDL_LAYER_TYPE, wxDefaultPosition, wxDefaultSize, m_DlgPDL_Layer_TypeStrings, 0 );
    m_DlgPDL_Layer_Type->SetStringSelection(_("Point"));
    itemFlexGridSizer4->Add(m_DlgPDL_Layer_Type, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgPDL_Layer_Name = new wxTextCtrl( itemDialog1, ID_DLGPDL_LAYER_NAME, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer4->Add(m_DlgPDL_Layer_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxNotebook* itemNotebook9 = new wxNotebook( itemDialog1, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    m_DlgPDL_Panel_Obj = new wxPanel( itemNotebook9, ID_DLGPDL_PANEL_OBJ, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(m_DlgPDL_Panel_Obj, wxID_ANY, _("Object description"));
    wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
    m_DlgPDL_Panel_Obj->SetSizer(itemStaticBoxSizer11);

    m_DlgPDL_Object_List = new ProjectDefLayersObjectList( m_DlgPDL_Panel_Obj, ID_DLGPDL_OBJ_LIST, wxSize(100, 200) );
    itemStaticBoxSizer11->Add(m_DlgPDL_Object_List, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer11->Add(itemBoxSizer13, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlatButton* itemFlatButton14 = new wxFlatButton( m_DlgPDL_Panel_Obj, ID_DLGPDL_OBJECT_ADD, _("+"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemFlatButton14->SetValue(false);
    itemBoxSizer13->Add(itemFlatButton14, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlatButton* itemFlatButton15 = new wxFlatButton( m_DlgPDL_Panel_Obj, ID_DLGPDL_OBJECT_REMOVE, _("-"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemFlatButton15->SetValue(false);
    itemBoxSizer13->Add(itemFlatButton15, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlatButton* itemFlatButton16 = new wxFlatButton( m_DlgPDL_Panel_Obj, ID_DLGPDL_OBJECT_IMPORT, _("Import"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlatButton16->SetValue(false);
    itemBoxSizer13->Add(itemFlatButton16, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    itemNotebook9->AddPage(m_DlgPDL_Panel_Obj, _("Objects"));

    m_DlgPDL_Panel_Fields = new wxPanel( itemNotebook9, ID_DLGPDL_PANEL_FIELDS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxStaticBox* itemStaticBoxSizer18Static = new wxStaticBox(m_DlgPDL_Panel_Fields, wxID_ANY, _("Optional layer fileds"));
    wxStaticBoxSizer* itemStaticBoxSizer18 = new wxStaticBoxSizer(itemStaticBoxSizer18Static, wxVERTICAL);
    m_DlgPDL_Panel_Fields->SetSizer(itemStaticBoxSizer18);

    m_DlgPDL_Fields_List = new ProjectDefLayersFieldsList( m_DlgPDL_Panel_Fields, ID_DLGPDL_FIELDS_LIST, wxSize(100, 100) );
    itemStaticBoxSizer18->Add(m_DlgPDL_Fields_List, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer18->Add(itemBoxSizer20, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlatButton* itemFlatButton21 = new wxFlatButton( m_DlgPDL_Panel_Fields, ID_DLGPDL_FIELD_ADD, _("+"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemFlatButton21->SetValue(false);
    itemBoxSizer20->Add(itemFlatButton21, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxFlatButton* itemFlatButton22 = new wxFlatButton( m_DlgPDL_Panel_Fields, ID_DLGPDL_FIELD_REMOVE, _("-"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemFlatButton22->SetValue(false);
    itemBoxSizer20->Add(itemFlatButton22, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    itemNotebook9->AddPage(m_DlgPDL_Panel_Fields, _("Fields"));

    itemBoxSizer2->Add(itemNotebook9, 1, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer23 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer23, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton24 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer23->AddButton(itemButton24);

    wxButton* itemButton25 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer23->AddButton(itemButton25);

    itemStdDialogButtonSizer23->Realize();

////@end ProjectDefLayersDlg content construction
}


/*!
 * Should we show tooltips?
 */

bool ProjectDefLayersDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ProjectDefLayersDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ProjectDefLayersDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ProjectDefLayersDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ProjectDefLayersDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ProjectDefLayersDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ProjectDefLayersDlg icon retrieval
}
