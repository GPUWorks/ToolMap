/////////////////////////////////////////////////////////////////////////////
// Name:        tocwindow_dlg.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Mon  3 Dec 18:27:07 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Mon 3 Dec 18:27:07 2007

#ifndef _TOCWINDOW_DLG_H_
#define _TOCWINDOW_DLG_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/treectrl.h"
#include "wx/tglbtn.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_TOCWINDOW_DLG 10035
#define ID_TREECTRL1 10127
#define ID_DLGTOC_ADD 10147
#define ID_DLGTOC_REMOVE 10145
#define SYMBOL_TOCWINDOW_DLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_TOCWINDOW_DLG_TITLE _("Table of content")
#define SYMBOL_TOCWINDOW_DLG_IDNAME ID_TOCWINDOW_DLG
#define SYMBOL_TOCWINDOW_DLG_SIZE wxSize(400, 300)
#define SYMBOL_TOCWINDOW_DLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * tocwindow_dlg class declaration
 */

class tocwindow_dlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( tocwindow_dlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    tocwindow_dlg();
    tocwindow_dlg( wxWindow* parent, wxWindowID id = SYMBOL_TOCWINDOW_DLG_IDNAME, const wxString& caption = SYMBOL_TOCWINDOW_DLG_TITLE, const wxPoint& pos = SYMBOL_TOCWINDOW_DLG_POSITION, const wxSize& size = SYMBOL_TOCWINDOW_DLG_SIZE, long style = SYMBOL_TOCWINDOW_DLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TOCWINDOW_DLG_IDNAME, const wxString& caption = SYMBOL_TOCWINDOW_DLG_TITLE, const wxPoint& pos = SYMBOL_TOCWINDOW_DLG_POSITION, const wxSize& size = SYMBOL_TOCWINDOW_DLG_SIZE, long style = SYMBOL_TOCWINDOW_DLG_STYLE );

    /// Destructor
    ~tocwindow_dlg();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin tocwindow_dlg event handler declarations

////@end tocwindow_dlg event handler declarations

////@begin tocwindow_dlg member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end tocwindow_dlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin tocwindow_dlg member variables
////@end tocwindow_dlg member variables
};

#endif
    // _TOCWINDOW_DLG_H_
