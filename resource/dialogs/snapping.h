/////////////////////////////////////////////////////////////////////////////
// Name:        snapping.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 31 Aug 11:12:00 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 31 Aug 11:12:00 2007

#ifndef _SNAPPING_H_
#define _SNAPPING_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/spinctrl.h"
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
#define ID_SNAPPING 10124
#define ID_SPINCTRL1 10002
#define ID_CHECKLISTBOX1 10003
#define ID_TOGGLEBUTTON21 10005
#define ID_TOGGLEBUTTON22 10241
#define ID_TOGGLEBUTTON25 10227
#define SYMBOL_SNAPPING_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SNAPPING_TITLE _("Snapping")
#define SYMBOL_SNAPPING_IDNAME ID_SNAPPING
#define SYMBOL_SNAPPING_SIZE wxSize(400, 300)
#define SYMBOL_SNAPPING_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * Snapping class declaration
 */

class Snapping: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( Snapping )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Snapping();
    Snapping( wxWindow* parent, wxWindowID id = SYMBOL_SNAPPING_IDNAME, const wxString& caption = SYMBOL_SNAPPING_TITLE, const wxPoint& pos = SYMBOL_SNAPPING_POSITION, const wxSize& size = SYMBOL_SNAPPING_SIZE, long style = SYMBOL_SNAPPING_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SNAPPING_IDNAME, const wxString& caption = SYMBOL_SNAPPING_TITLE, const wxPoint& pos = SYMBOL_SNAPPING_POSITION, const wxSize& size = SYMBOL_SNAPPING_SIZE, long style = SYMBOL_SNAPPING_STYLE );

    /// Destructor
    ~Snapping();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Snapping event handler declarations

////@end Snapping event handler declarations

////@begin Snapping member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Snapping member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin Snapping member variables
////@end Snapping member variables
};

#endif
    // _SNAPPING_H_
