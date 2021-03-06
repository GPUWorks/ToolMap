/////////////////////////////////////////////////////////////////////////////
// Name:        mergelines.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Thu 18 Oct 11:58:40 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Thu 18 Oct 11:58:40 2007

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

#include "mergelines.h"

////@begin XPM images
////@end XPM images


/*!
 * Mergelines type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Mergelines, wxDialog )


/*!
 * Mergelines event table definition
 */

BEGIN_EVENT_TABLE( Mergelines, wxDialog )

////@begin Mergelines event table entries
////@end Mergelines event table entries

END_EVENT_TABLE()


/*!
 * Mergelines constructors
 */

Mergelines::Mergelines()
{
    Init();
}

Mergelines::Mergelines( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * Mergelines creator
 */

bool Mergelines::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Mergelines creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end Mergelines creation
    return true;
}


/*!
 * Mergelines destructor
 */

Mergelines::~Mergelines()
{
////@begin Mergelines destruction
////@end Mergelines destruction
}


/*!
 * Member initialisation
 */

void Mergelines::Init()
{
////@begin Mergelines member initialisation
////@end Mergelines member initialisation
}


/*!
 * Control creation for Mergelines
 */

void Mergelines::CreateControls()
{    
////@begin Mergelines content construction
    // Generated by DialogBlocks, Thu 18 Oct 12:02:16 2007 (Personal Edition)

    Mergelines* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Choose the line with which other line will be merged"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxArrayString itemListBox4Strings;
    wxListBox* itemListBox4 = new wxListBox( itemDialog1, ID_LISTBOX6, wxDefaultPosition, wxDefaultSize, itemListBox4Strings, wxLB_SINGLE );
    itemBoxSizer2->Add(itemListBox4, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end Mergelines content construction
}


/*!
 * Should we show tooltips?
 */

bool Mergelines::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Mergelines::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Mergelines bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Mergelines bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Mergelines::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Mergelines icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Mergelines icon retrieval
}
