/////////////////////////////////////////////////////////////////////////////
// Name:        projectproperties.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Mon 27 Aug 15:48:36 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Mon 27 Aug 15:48:36 2007

#ifndef _PROJECTPROPERTIES_H_
#define _PROJECTPROPERTIES_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/notebook.h"
#include "wx/tglbtn.h"
#include "wx/statline.h"
////@end includes
#include <wx/filepicker.h>

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxNotebook;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_PROJECTPROPERTIES 10132
#define ID_DLGPS_NOTEBOOK 10141
#define ID_DLGPS_PANEL_SETTINGS 10142
#define ID_DLGPS_EXPORT_TYPE_CHOICE 10000
#define ID_DLGPS_EXPORT_PATH 10001
#define ID_DLGPS_BACKUP_PATH 10003
#define ID_DLGPS_PANEL_SCALE 10133
#define ID_DLGPS_SCALE_LIST 10134
#define ID_DLGPS_BTN_SCALE_ADD 10239
#define ID_DLGPS_BTN_SCALE_DEL 10240
#define ID_DLGPS_SCALE_ORDER_CHOICE 10135
#define SYMBOL_PROJECTPROPERTIESDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTPROPERTIESDLG_TITLE _("Project settings")
#define SYMBOL_PROJECTPROPERTIESDLG_IDNAME ID_PROJECTPROPERTIES
#define SYMBOL_PROJECTPROPERTIESDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTPROPERTIESDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ProjectPropertiesDLG class declaration
 */

class ProjectPropertiesDLG: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ProjectPropertiesDLG )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ProjectPropertiesDLG();
    ProjectPropertiesDLG( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTPROPERTIESDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTPROPERTIESDLG_TITLE, const wxPoint& pos = SYMBOL_PROJECTPROPERTIESDLG_POSITION, const wxSize& size = SYMBOL_PROJECTPROPERTIESDLG_SIZE, long style = SYMBOL_PROJECTPROPERTIESDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTPROPERTIESDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTPROPERTIESDLG_TITLE, const wxPoint& pos = SYMBOL_PROJECTPROPERTIESDLG_POSITION, const wxSize& size = SYMBOL_PROJECTPROPERTIESDLG_SIZE, long style = SYMBOL_PROJECTPROPERTIESDLG_STYLE );

    /// Destructor
    ~ProjectPropertiesDLG();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin ProjectPropertiesDLG event handler declarations

////@end ProjectPropertiesDLG event handler declarations

////@begin ProjectPropertiesDLG member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ProjectPropertiesDLG member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin ProjectPropertiesDLG member variables
    wxNotebook* m_DLGPS_Notebook;
    wxChoice* m_DLGPS_Export_Type_Choice;
    wxTextCtrl* m_DLGPS_Export_Path;
    wxTextCtrl* m_DLGPS_Backup_Path;
    wxListBox* m_DLGPS_Scale_List;
    wxChoice* m_DLGPS_Scale_Order_Choice;
////@end ProjectPropertiesDLG member variables
};

#endif
    // _PROJECTPROPERTIES_H_
