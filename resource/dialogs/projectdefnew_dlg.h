/////////////////////////////////////////////////////////////////////////////
// Name:        projectdefnew_dlg.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 16 May 22:18:04 2008
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 16 May 22:18:04 2008

#ifndef _PROJECTDEFNEW_DLG_H_
#define _PROJECTDEFNEW_DLG_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
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
#define ID_PROJECTDEFNEW 10018
#define ID_TEXTCTRL1 10114
#define ID_TEXTCTRL2 10115
#define SYMBOL_PROJECTDEFNEW_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxDIALOG_MODAL|wxTAB_TRAVERSAL
#define SYMBOL_PROJECTDEFNEW_TITLE _("New project")
#define SYMBOL_PROJECTDEFNEW_IDNAME ID_PROJECTDEFNEW
#define SYMBOL_PROJECTDEFNEW_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFNEW_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ProjectDefNew class declaration
 */

class ProjectDefNew: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ProjectDefNew )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ProjectDefNew();
    ProjectDefNew( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFNEW_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFNEW_TITLE, const wxPoint& pos = SYMBOL_PROJECTDEFNEW_POSITION, const wxSize& size = SYMBOL_PROJECTDEFNEW_SIZE, long style = SYMBOL_PROJECTDEFNEW_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFNEW_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFNEW_TITLE, const wxPoint& pos = SYMBOL_PROJECTDEFNEW_POSITION, const wxSize& size = SYMBOL_PROJECTDEFNEW_SIZE, long style = SYMBOL_PROJECTDEFNEW_STYLE );

    /// Destructor
    ~ProjectDefNew();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ProjectDefNew event handler declarations

////@end ProjectDefNew event handler declarations

////@begin ProjectDefNew member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ProjectDefNew member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ProjectDefNew member variables
////@end ProjectDefNew member variables
};

#endif
    // _PROJECTDEFNEW_DLG_H_
