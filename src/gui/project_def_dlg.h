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

// comment doxygen


#ifndef PROJECT_DEF_DLG_H
#define PROJECT_DEF_DLG_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

////@begin includes
#include "wx/listctrl.h"
#include "wx/tglbtn.h"
#include "listgenreport.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class ProjectDefDLG;
class wxToggleButton;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DLGPD 10000
#define ID_DLGPD_PROJ_PATH 10001
#define ID_DLJPD_PROJ_PATH_BTN 10002
#define ID_DLGPD_PROJ_NAME 10003
#define ID_DLGPD_PROJ_UNIT 10156
#define ID_DLGPD_PROJ_PROJECTION 10119
#define ID_DLGPD_SPAT_MODEL_LIST 10004
#define ID_DLGPD_SPAT_MDL_ADD 10219
#define ID_DLJPD_SPAT_MDL_DEL 10006
#define SYMBOL_PROJECTDEFDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_PROJECTDEFDLG_TITLE _("Project Definition")
#define SYMBOL_PROJECTDEFDLG_IDNAME ID_DLGPD
#define SYMBOL_PROJECTDEFDLG_SIZE wxSize(400, 300)
#define SYMBOL_PROJECTDEFDLG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ProjectDefDLG class declaration
 */

class ProjectDefDLG: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ProjectDefDLG )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ProjectDefDLG();
    ProjectDefDLG( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFDLG_TITLE, const wxPoint& pos = SYMBOL_PROJECTDEFDLG_POSITION, const wxSize& size = SYMBOL_PROJECTDEFDLG_SIZE, long style = SYMBOL_PROJECTDEFDLG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_PROJECTDEFDLG_IDNAME, const wxString& caption = SYMBOL_PROJECTDEFDLG_TITLE, const wxPoint& pos = SYMBOL_PROJECTDEFDLG_POSITION, const wxSize& size = SYMBOL_PROJECTDEFDLG_SIZE, long style = SYMBOL_PROJECTDEFDLG_STYLE );

    /// Destructor
    ~ProjectDefDLG();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();


////@begin ProjectDefDLG member variables
    wxTextCtrl* m_DlgPD_Proj_Path;
    wxButton* m_DlgPd_Proj_Path_Btn;
    wxTextCtrl* m_DlgPd_Proj_Name;
    wxChoice* m_DlgPd_Proj_Unit;
    wxChoice* m_DlgPd_Proj_Projection;
    ProjectDefDLG * m_DlgPd_Stat_Model_List;
    wxToggleButton* m_DlgPd_Spat_Mdl_Add;
    wxToggleButton* m_DljPd_Spat_Mdl_Del;
////@end ProjectDefDLG member variables
};




class ProjectDefList : public ListGenReport
	{
	private:
		//virtual void OnDoubleClickItem(wxListEvent & event);
//		void OnMySelectionChange (wxListEvent & event);
		
		wxChoice * m_ChoiceToChange;
		
	public:
		static const int ID_PARAMLIST;

		ProjectDefList(wxWindow * parent, wxWindowID id);

		~ProjectDefList(); 

	//	int GetParamType (wxString myTextParam);
//		
//		wxString GetParamType (int imyType);
//
//		void AddItemToParamList(wxString myValue, wxString myComment, ACOMMENTPOSITION type);
//		void SetChoiceList (wxChoice * myChoiceList);
//		DECLARE_EVENT_TABLE();
};



#endif
