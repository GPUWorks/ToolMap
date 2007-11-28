/////////////////////////////////////////////////////////////////////////////
// Name:        importlog.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Thu 30 Aug 14:52:41 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Thu 30 Aug 14:52:41 2007

#ifndef _IMPORTLOG_H_
#define _IMPORTLOG_H_

/*!
 * Includes
 */

////@begin includes
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
#define ID_IMPORTLOG 10157
#define ID_TEXTCTRL18 10158
#define ID_BUTTON24 10159
#define SYMBOL_IMPORTLOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_IMPORTLOG_TITLE _("Log window")
#define SYMBOL_IMPORTLOG_IDNAME ID_IMPORTLOG
#define SYMBOL_IMPORTLOG_SIZE wxSize(400, 300)
#define SYMBOL_IMPORTLOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ImportLog class declaration
 */

class ImportLog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ImportLog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ImportLog();
    ImportLog( wxWindow* parent, wxWindowID id = SYMBOL_IMPORTLOG_IDNAME, const wxString& caption = SYMBOL_IMPORTLOG_TITLE, const wxPoint& pos = SYMBOL_IMPORTLOG_POSITION, const wxSize& size = SYMBOL_IMPORTLOG_SIZE, long style = SYMBOL_IMPORTLOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_IMPORTLOG_IDNAME, const wxString& caption = SYMBOL_IMPORTLOG_TITLE, const wxPoint& pos = SYMBOL_IMPORTLOG_POSITION, const wxSize& size = SYMBOL_IMPORTLOG_SIZE, long style = SYMBOL_IMPORTLOG_STYLE );

    /// Destructor
    ~ImportLog();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ImportLog event handler declarations

////@end ImportLog event handler declarations

////@begin ImportLog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ImportLog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ImportLog member variables
////@end ImportLog member variables
};

#endif
    // _IMPORTLOG_H_
