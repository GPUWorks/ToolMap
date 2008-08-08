/////////////////////////////////////////////////////////////////////////////
// Name:        linessymbology.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Wed 22 Aug 14:09:53 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Wed 22 Aug 14:09:53 2007

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

#include "linessymbology.h"

////@begin XPM images
////@end XPM images


/*!
 * Linessymbology type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Linessymbology, wxDialog )


/*!
 * Linessymbology event table definition
 */

BEGIN_EVENT_TABLE( Linessymbology, wxDialog )

////@begin Linessymbology event table entries
////@end Linessymbology event table entries

END_EVENT_TABLE()


/*!
 * Linessymbology constructors
 */

Linessymbology::Linessymbology()
{
    Init();
}

Linessymbology::Linessymbology( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * Linessymbology creator
 */

bool Linessymbology::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Linessymbology creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end Linessymbology creation
    return true;
}


/*!
 * Linessymbology destructor
 */

Linessymbology::~Linessymbology()
{
////@begin Linessymbology destruction
////@end Linessymbology destruction
}


/*!
 * Member initialisation
 */

void Linessymbology::Init()
{
////@begin Linessymbology member initialisation
////@end Linessymbology member initialisation
}


/*!
 * Control creation for Linessymbology
 */

void Linessymbology::CreateControls()
{    
////@begin Linessymbology content construction
    // Generated by DialogBlocks, Thu 7 Aug 15:28:35 2008 (Personal Edition)

    Linessymbology* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxNotebook* itemNotebook3 = new wxNotebook( itemDialog1, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    wxPanel* itemPanel4 = new wxPanel( itemNotebook3, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);

    wxTextCtrl* itemTextCtrl6 = new wxTextCtrl( itemPanel4, ID_TEXTCTRL3, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer5->Add(itemTextCtrl6, 1, wxGROW|wxALL, 5);

    itemNotebook3->AddPage(itemPanel4, _("Informations"));

    wxPanel* itemPanel7 = new wxPanel( itemNotebook3, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);

    wxNotebook* itemNotebook9 = new wxNotebook( itemPanel7, ID_NOTEBOOK5, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    wxPanel* itemPanel10 = new wxPanel( itemNotebook9, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(3, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
    itemPanel10->SetSizer(itemFlexGridSizer11);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel10, wxID_STATIC, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton13 = new wxButton( itemPanel10, ID_BUTTON12, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemButton13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel10, wxID_STATIC, _("Shape :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice15Strings;
    itemChoice15Strings.Add(_("----------"));
    itemChoice15Strings.Add(_("..............."));
    wxChoice* itemChoice15 = new wxChoice( itemPanel10, ID_CHOICE4, wxDefaultPosition, wxDefaultSize, itemChoice15Strings, 0 );
    itemFlexGridSizer11->Add(itemChoice15, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel10, wxID_STATIC, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSpinCtrl* itemSpinCtrl17 = new wxSpinCtrl( itemPanel10, ID_SPINCTRL3, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer11->Add(itemSpinCtrl17, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook9->AddPage(itemPanel10, _("Unique"));

    wxPanel* itemPanel18 = new wxPanel( itemNotebook9, ID_PANEL12, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    itemPanel18->SetSizer(itemBoxSizer19);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer20, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel18, wxID_STATIC, _("Field :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice22Strings;
    wxChoice* itemChoice22 = new wxChoice( itemPanel18, ID_CHOICE5, wxDefaultPosition, wxDefaultSize, itemChoice22Strings, 0 );
    itemBoxSizer20->Add(itemChoice22, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxListCtrl* itemListCtrl23 = new wxListCtrl( itemPanel18, ID_LISTCTRL5, wxDefaultPosition, wxSize(100, 100), wxLC_REPORT );
    itemBoxSizer19->Add(itemListCtrl23, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer19->Add(itemBoxSizer24, 0, wxGROW|wxALL, 5);
    wxToggleButton* itemToggleButton25 = new wxToggleButton( itemPanel18, ID_TOGGLEBUTTON16, _("+"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton25->SetValue(false);
    itemBoxSizer24->Add(itemToggleButton25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxToggleButton* itemToggleButton26 = new wxToggleButton( itemPanel18, ID_TOGGLEBUTTON17, _("-"), wxDefaultPosition, wxSize(40, -1), 0 );
    itemToggleButton26->SetValue(false);
    itemBoxSizer24->Add(itemToggleButton26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxToggleButton* itemToggleButton27 = new wxToggleButton( itemPanel18, ID_TOGGLEBUTTON18, _("Add all"), wxDefaultPosition, wxDefaultSize, 0 );
    itemToggleButton27->SetValue(false);
    itemBoxSizer24->Add(itemToggleButton27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer28 = new wxFlexGridSizer(2, 2, 0, 0);
    itemFlexGridSizer28->AddGrowableCol(1);
    itemBoxSizer19->Add(itemFlexGridSizer28, 0, wxGROW, 5);
    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel18, wxID_STATIC, _("Shape :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText29, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice30Strings;
    itemChoice30Strings.Add(_("----------"));
    itemChoice30Strings.Add(_("..............."));
    wxChoice* itemChoice30 = new wxChoice( itemPanel18, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, itemChoice30Strings, 0 );
    itemFlexGridSizer28->Add(itemChoice30, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel18, wxID_STATIC, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSpinCtrl* itemSpinCtrl32 = new wxSpinCtrl( itemPanel18, ID_SPINCTRL, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer28->Add(itemSpinCtrl32, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook9->AddPage(itemPanel18, _("Discrete"));

    wxPanel* itemPanel33 = new wxPanel( itemNotebook9, ID_PANEL13, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer34 = new wxFlexGridSizer(5, 2, 0, 0);
    itemFlexGridSizer34->AddGrowableCol(1);
    itemPanel33->SetSizer(itemFlexGridSizer34);

    wxStaticText* itemStaticText35 = new wxStaticText( itemPanel33, wxID_STATIC, _("Field :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText35, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice36Strings;
    wxChoice* itemChoice36 = new wxChoice( itemPanel33, ID_CHOICE6, wxDefaultPosition, wxDefaultSize, itemChoice36Strings, 0 );
    itemFlexGridSizer34->Add(itemChoice36, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText37 = new wxStaticText( itemPanel33, wxID_STATIC, _("Starting color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText37, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton38 = new wxButton( itemPanel33, ID_BUTTON15, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemButton38, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText39 = new wxStaticText( itemPanel33, wxID_STATIC, _("Ending color"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText39, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton40 = new wxButton( itemPanel33, ID_BUTTON16, _("Button"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemButton40, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText41 = new wxStaticText( itemPanel33, wxID_STATIC, _("Shape :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText41, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice42Strings;
    wxChoice* itemChoice42 = new wxChoice( itemPanel33, ID_CHOICE7, wxDefaultPosition, wxDefaultSize, itemChoice42Strings, 0 );
    itemFlexGridSizer34->Add(itemChoice42, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText43 = new wxStaticText( itemPanel33, wxID_STATIC, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer34->Add(itemStaticText43, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSpinCtrl* itemSpinCtrl44 = new wxSpinCtrl( itemPanel33, ID_SPINCTRL4, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer34->Add(itemSpinCtrl44, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook9->AddPage(itemPanel33, _("Continuous"));

    itemBoxSizer8->Add(itemNotebook9, 1, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer45Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer45 = new wxStaticBoxSizer(itemStaticBoxSizer45Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer45, 0, wxGROW|wxALL, 5);
    wxSlider* itemSlider46 = new wxSlider( itemPanel7, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemStaticBoxSizer45->Add(itemSlider46, 1, wxGROW|wxALL, 5);

    wxTextCtrl* itemTextCtrl47 = new wxTextCtrl( itemPanel7, ID_TEXTCTRL10, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer45->Add(itemTextCtrl47, 0, wxGROW|wxALL, 5);

    itemNotebook3->AddPage(itemPanel7, _("Line Symbology"));

    itemBoxSizer2->Add(itemNotebook3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer48 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer48, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton49 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer48->AddButton(itemButton49);

    wxButton* itemButton50 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer48->AddButton(itemButton50);

    itemStdDialogButtonSizer48->Realize();

////@end Linessymbology content construction
}


/*!
 * Should we show tooltips?
 */

bool Linessymbology::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Linessymbology::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Linessymbology bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Linessymbology bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Linessymbology::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Linessymbology icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Linessymbology icon retrieval
}
