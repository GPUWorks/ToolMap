/////////////////////////////////////////////////////////////////////////////
// Name:        dlg_obj_attribution.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 10 Aug 18:15:18 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 10 Aug 18:15:18 2007

#ifndef _DLG_OBJ_ATTRIBUTION_H_
#define _DLG_OBJ_ATTRIBUTION_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/datectrl.h"
#include "wx/dateevt.h"
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
#define ID_DLG_OBJ_ATTRIBUTION 10039
#define ID_NOTEBOOK2 10040
#define ID_PANEL4 10041
#define ID_NOTEBOOK3 10042
#define ID_PANEL5 10043
#define ID_CHECKLISTBOX4 10045
#define ID_PANEL6 10044
#define ID_CHECKLISTBOX3 10063
#define ID_PANEL7 10057
#define ID_CHECKLISTBOX2 10060
#define ID_TEXTCTRL6 10062
#define ID_BUTTON9 10061
#define ID_PANEL8 10058
#define ID_CHECKLISTBOX1 10059
#define ID_PANEL23 10139
#define ID_CHOICE13 10203
#define ID_TEXTCTRL19 10204
#define ID_CHOICE14 10205
#define ID_DATEPICKERCTRL 10214
#define ID_TEXTCTRL21 10206
#define ID_CHECKBOX7 10209
#define ID_SEARCHCTRL1 10053
#define ID_CHECKBOX9 10054
#define ID_BUTTON7 10055
#define ID_BUTTON8 10056
#define ID_CHECKBOX5 10136
#define SYMBOL_ATTRIBOBJTYPE_PANEL_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE _("Object types")
#define SYMBOL_ATTRIBOBJTYPE_PANEL_IDNAME ID_DLG_OBJ_ATTRIBUTION
#define SYMBOL_ATTRIBOBJTYPE_PANEL_SIZE wxSize(450, 300)
#define SYMBOL_ATTRIBOBJTYPE_PANEL_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * AttribObjType_PANEL class declaration
 */

class AttribObjType_PANEL: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( AttribObjType_PANEL )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AttribObjType_PANEL();
    AttribObjType_PANEL( wxWindow* parent, wxWindowID id = SYMBOL_ATTRIBOBJTYPE_PANEL_IDNAME, const wxString& caption = SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE, const wxPoint& pos = SYMBOL_ATTRIBOBJTYPE_PANEL_POSITION, const wxSize& size = SYMBOL_ATTRIBOBJTYPE_PANEL_SIZE, long style = SYMBOL_ATTRIBOBJTYPE_PANEL_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ATTRIBOBJTYPE_PANEL_IDNAME, const wxString& caption = SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE, const wxPoint& pos = SYMBOL_ATTRIBOBJTYPE_PANEL_POSITION, const wxSize& size = SYMBOL_ATTRIBOBJTYPE_PANEL_SIZE, long style = SYMBOL_ATTRIBOBJTYPE_PANEL_STYLE );

    /// Destructor
    ~AttribObjType_PANEL();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AttribObjType_PANEL event handler declarations

////@end AttribObjType_PANEL event handler declarations

////@begin AttribObjType_PANEL member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AttribObjType_PANEL member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AttribObjType_PANEL member variables
////@end AttribObjType_PANEL member variables
};

#endif
    // _DLG_OBJ_ATTRIBUTION_H_
