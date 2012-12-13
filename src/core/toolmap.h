/***************************************************************************
								toolmap.h
				main file for toolmap 2, contain app and main frame
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

/*! \mainpage ToolMap 
 
 \section intro_sec Introduction
 
 This program is intended to be a full featured multi-layers GIS editor, implementing the
 SION Method. More informations on the method could be found here : http://www.crealp.ch
 
 This program was the result of a partnership between Swisstopo and the CREALP. For licence
 informations please refer to the corresponding section.
 
 \image html toolmap.jpg 
 
 \section doc Documentation topics
 To find our way in all the documentated class of the programm, We have sorted our
 documentation based on topics or part of the program. Some topics are presented here :
 
  
 - \subpage intro 
 - \subpage Prj_def "Project definition"
 - \subpage Prj_Edit_Obj "Object Definition"
 - \subpage Misc "Miscellaneous"

 \section Licence 
 
 This program is free software; you can redistribute it and/or modify 
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or 
 (at your option) any later version.
 
 \author Lucien Schreiber (c) CREALP 2007
 \date  Since november 2007

 
 \page intro Introduction 
 During ToolMap 2 startup, the followed process is described hereafter :
 - ToolMapApp::OnInit() function is called and create the program frame (see class ToolMapFrame)
 - ToolMapFrame object is then initialised and create both the menu bar and the toolbar.
		- ToolMapFrame::CreateToolMapMenu
		- ToolMapFrame::CreateToolMapToolBar
 
 Those two last function were copy-pasted from DialogBlocks (http://www.anthemion.co.uk/dialogblocks/)
 a graphical UI designer.
 
 The program is then started and mainly waiting for event.

 \page Misc "Miscellaneous"
 
 Here are presented some various stuff used in the ToolMap project but not 
 specificaly related to a category.
 
 \section ctrl Controls
 Some controls we have tweaked for Toolmap's need but probably usefull for others projects
 - \subpage tmColourPicker "Colour picker control"
 - \subpage tmSliderWithText "Slider with text control"
 - \subpage tmOptionButton "Option Button"
 
 \section lib Libraries
 Some code created by others that we use
 - \subpage wxSerialize "wxSerialize"
 - \subpage wxTreeMultiCtrl "wxTreeMultiCtrl"
 
 
 */


#ifndef TOOLMAP_H
#define TOOLMAP_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/sysopt.h>					// for dealing with systemoptions
#include <wx/toolbar.h>
#include "../gui/project_def_dlg.h"		// project definition dialog
#include "../gui/logwindow_dlg.h"		// log window definition
#include "../gui/tocwindow_dlg.h"		// Table of content (TOC) definition
//#include "../database/db_project.h"
#include "../gui/menumanager.h"			// for menu management (enable / diable)

// GIS Includes
#include "../gis/tmlayermanager.h"		// gis central manager

//#include "../gui/attribution_obj_type.h"
#include "../gui/main_panel.h"
#include "../gui/queries_panel.h"		// for queries panel
#include "projectmanager.h"
#include <wx/fileconf.h>		// for using file configuration even on windows

#include "../gui/openerror_dlg.h"	// open error dlg

#include "../gis/tmgisdata.h"	// for GIS data access
#include "../gis/tmscalectrl.h"	// for GIS scale control combo box

#include "../gis/tmattributionmanager.h"	// attribution manager
#include "tmexportmanager.h"				// export manager
#include "../gui/shortcut_panel.h"			// for shortcut panel
#include "../gui/snapping_panel.h"			// for snapping panel
#include "../gis/tmeditmanager.h"			// for editing manager
#include "../gui/tmtoolmanager.h"			// for tools


#include "tmwindowposition.h"			// saving window position

//// temp for testing 
#include "diroperation.h"
//// end of testing

#include "../gui/tmabout_dlg.h"
#include "../gui/tmcrash_dlg.h"


class MenuManager;
class ToolbarManager;
class ProjectManager;
class WebUpdateInformationBar;
class tmStatsManager;
class AttribObjType_PANEL;

#include <wx/mstream.h>


// items from menu project.
#define ID_MENU_NEW_PRJ_EMPTY		10300
#define ID_MENU_NEW_PRJ_EXISTING	10301
#define ID_MENU_OPEN_PRJ			10302
#define ID_MENU_RECENT				10303
#define ID_MENU_PRJ_BACKUP          10304
#define ID_MENU_PRJ_BACKUP_MANAGER  10305
#define ID_MENU_PRJ_SAVE_TEMPLATE	10306
#define ID_MENU_EXPORT              10307
#define ID_MENU_EXPORT_LAYER		10308
//#define ID_MENU_EXPORT_FULL			10309
#define ID_MENU_PRJ_EDIT            10309
#define ID_MENU_PRJ_DEF				10310
#define ID_MENU_OBJ_DEF				10311
#define ID_MENU_OBJ_ATTRIB_DEF		10312
#define ID_MENU_PRJ_SETTINGS		10313

#define ID_MENU_STATISTICS			10350

#define ID_MENU_ADD_SPATIAL_DATA 10022
#define ID_MENU_IMPORT_GIS_DATA 10112

//#define ID_MENU_ZOOM_PREVIOUS 10313
#define ID_MENU_ZOOM 10315
#define ID_MENU_PAN 10316
#define ID_MENU_ZOOM_FIT 10317
#define ID_MENU_ZOOM_GIVEN_SCALE 10318
#define ID_MENU_ZOOM_COORDINATE 10319
#define ID_MENU_ZOOM_NEXT_SCALE 10320
#define ID_MENU_PREVIOUS_SCALE 10321
#define ID_MENU_ZOOM_SELECTED_LAYER 10322

#define ID_MENU_SELECTION 10323

#define ID_MENU_UNDO 10179
//#define ID_MENU_REDO 10180
#define ID_MENU_DRAW 10176
#define ID_MENU_MODIFY 10177
#define ID_MENU_MODIFY_SHARED 10200
#define ID_MENU_CUT_LINES 10178
#define ID_MENU_EDIT_VERTEX_POS 10211
#define ID_MENU_MERGE_LINES 10003
#define ID_MENU_CREATE_INTERSECTIONS 10174
#define ID_MENU_FLIP_LINE 10004
#define ID_MENU_DELETE_OBJ 10122
#define ID_MENU_ORIENT_POINT 10190
#define ID_MENU_ADJUST_SNAPPING 10001
#define ID_MENU_ATTRIB_TYPES 10166
#define ID_MENU_ATTRIB_ATTRIBUTES 10175
#define ID_MENU_ATTRIB_BATCH 10167
#define ID_MENU_COPY_PASTE_ATTRIB 10231
#define ID_MENU_SHORTCUTS 10183
#define ID_MENU_CHECK_GEOM 10123
#define ID_MENU_TOOL_DANGLING 10048
#define ID_MENU_SELECT 10163
#define ID_MENU_SELECT_BY_OID 10162
#define ID_MENU_SELECT_NONE 10160
#define ID_MENU_SELECT_INVERSE 10161
#define ID_MENU_QUERIES 10019
//#define ID_MENU_QUERIES_RUN 10020
#define ID_MENUITEM11 10046
#define ID_MENUITEM12 10047
#define ID_MENU_TOC_WINDOW 10037
#define ID_MENU_INFO_WINDOW 10140
#define ID_MENU_LOG_WINDOW 10207
#define ID_MENU_CHECK_UPDATE 10125
#define ID_MENU_COMPONENTS 10126
#define ID_MENU_REPORT_BUG 10128
#define ID_MENU_ASK_NEW_FEATURE 10129
#define ID_MENU_USER_MANUAL 10130
#define ID_MENU_EXPORT_MODEL 10077

#define ID_TOOLBAR_ZOOM 10252


#define ID_CTXT_FULL_ATTRIB 21000
#define ID_CTXT_FILTER 21001
#define ID_CTXT_AUTODISPLAY_ATTRIB 21002
#define ID_CTXT_EMPTY_LIST_AFTER_ATTRIB 21003

const int ID_MENU_LAYOUT_DEFAULT = 15201;
const int ID_MENU_LAYOUT_VERTICAL = 15202;
const int ID_MENU_LAYOUT_HORIZONTAL = 15203;
const int ID_MENU_VALIDITY = 15204;


const wxString g_ProgName = _T("ToolMap");
const wxString g_ProgMajorVersion = _T("2.5");

// define perspectives for auimanager
enum tmPERSPECTIVE_MODE {
    tmPERSPECTIVE_MODE_DEFAULT = 0,
    tmPERSPECTIVE_MODE_VERTICAL,
    tmPERSPECTIVE_MODE_HORIZONTAL
};

/***************************************************************************//**
 @brief Implement the Application
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/
class ToolMapApp : public wxApp
{
private:
	void _RemoveLogFile();
	
public:
    ToolMapApp();
	virtual bool OnInit();
	virtual void OnFatalException();
	
};

DECLARE_APP(ToolMapApp);




/***************************************************************************//**
 @brief All frame's related stuff
 @details This implement the wxFrame class. This is where all stuff related to
 the graphical aspect of the main programme's frame is defined
 @author Lucien Schreiber (c) CREALP 2007
 @date 04 December 2007
 *******************************************************************************/
class ToolMapFrame: public wxFrame
{
private:
	wxAuiManager * m_AuiManager;
	AttribObjType_PANEL * m_AttribObjPanel;
	Main_PANEL * m_MainPanel;
	Queries_PANEL * m_QueriesPanel;
	Shortcuts_PANEL * m_ShortCutPanel;
	Snapping_PANEL * m_SnappingPanel; 
	
	ProjectManager * m_PManager;
	MenuManager * m_MManager;
	
	tmLayerManager * m_LayerManager;
	tmScaleCtrlCombo * m_ScaleCombo;
	
	tmAttributionManager * m_AttribManager;
	
	tmEditManager * m_EditManager;
	tmToolManager * m_ToolManager;
	WebUpdateInformationBar * m_InfoBar;
	
	tmStatsManager * m_StatManager;
    
    wxLogWindow * m_LogWindow;
	TocWindowDlgGen * m_TocWindow;
    wxArrayString m_Perspectives;
	
	void OnQuit(wxCommandEvent & event);
	void OnClose(wxCloseEvent & event);
	void OnAbout(wxCommandEvent& event);
	void OnNewProject(wxCommandEvent & event);
	void OnNewProjectExisting (wxCommandEvent & event);
	void OnOpenProject (wxCommandEvent & event);
	void OnOpenRecentProject (wxCommandEvent & event);
	void OnEditProjectObjects (wxCommandEvent & event);
	void OnEditProjectSettings (wxCommandEvent & event);
	void OnEditObjectAttributes (wxCommandEvent & event);
	void OnEditProject (wxCommandEvent & event);
	void OnExportProjectModel (wxCommandEvent & event);
	void OnLogWindow(wxCommandEvent & event);
	void OnTocWindow (wxCommandEvent & event);
	void OnShowObjectAttributionWindow (wxCommandEvent & event);
	void OnShowQueriesWindow (wxCommandEvent & event);
	void OnProjectBackup (wxCommandEvent & event);
    void OnProjectBackupManage (wxCommandEvent & event);
	void OnProjectSaveTemplate (wxCommandEvent & event);
	void OnToolChanged (wxCommandEvent & event);
	void OnZoomPrevious(wxCommandEvent & event);
	void OnZoomToSelectedLayer(wxCommandEvent & event);
	void OnExportSelected (wxCommandEvent & event);
	void OnExportAll (wxCommandEvent & event);
	void OnShowShortcutWindow (wxCommandEvent & event);
	void OnShowSnappingWindow (wxCommandEvent & event);
	void OnEditSwitch (wxCommandEvent & event);
	void OnEditDeleteSelected (wxCommandEvent & event);
	void OnEditUndo (wxCommandEvent & event);
	void OnCreateIntersections (wxCommandEvent & event);
	void OnEditVertexPosition (wxCommandEvent & event);
	void OnMergeSelectedLines (wxCommandEvent & event);
	void OnShowAAttributionWindow (wxCommandEvent & event);
	void OnAAttributionBatchWindow (wxCommandEvent & event);
	void OnShowInformationDialog (wxCommandEvent & event);
	//void OnMenuHighlightOn (wxMenuEvent & event);
	//void OnMenuHighligntOff(wxMenuEvent & event);
	void OnCheckUpdates (wxCommandEvent & event);
	void OnReportBug (wxCommandEvent & event);
	void OnComponentsInfo (wxCommandEvent & event);
	
	void OnImportGISData (wxCommandEvent & event);
	void OnUserManual(wxCommandEvent & event);
	void OnFlipLine (wxCommandEvent & event);
	void OnPreferences(wxCommandEvent & event);
    void OnRefreshView(wxCommandEvent & event);
	
	
	// selection
	void OnSelectNone (wxCommandEvent & event);
	void OnSelectByOid (wxCommandEvent & event);
	void OnSelectInvert (wxCommandEvent & event);
	
	// changing status function
	void OnUpdateSelection (wxCommandEvent & event);
    
	// GIS functions
	void OnAddGisData (wxCommandEvent & event);
	
	void OnShortcutAttributionDone (wxCommandEvent & event);
	
	// ToolsFunction
	void OnDanglingNodes(wxCommandEvent & event);
    void OnGeometryValidity (wxCommandEvent & event);
    
	void OnCloseManagedPane(wxAuiManagerEvent & event);
	
	void OnStatisticsUpdate(wxCommandEvent & event);
	void OnStatisticsDialog(wxCommandEvent & event);
	// child event function
	//void OnMenuZoomPreviousChange(wxCommandEvent & event);
	void _CheckUpdates(bool ismanual = false);
    
    // view layout
    void OnLayoutDefault (wxCommandEvent & event);
    void OnLayoutVertical (wxCommandEvent & event);
    void OnLayoutHorizontal (wxCommandEvent & event);
    
	// update menu functions
	void OnUpdateMenuProject(wxUpdateUIEvent & event);
	void OnUpdateMenuPreviousZoom (wxUpdateUIEvent & event);
	void OnUpdateMenuEditUndo (wxUpdateUIEvent & event);
	void OnUpdateMenuEditDraw (wxUpdateUIEvent & event);
	void OnUpdateMenuEditModify (wxUpdateUIEvent & event);
	void OnUpdateMenuEditDelete (wxUpdateUIEvent & event);
	void OnUpdateMenuEditMerge (wxUpdateUIEvent & event);
	void OnUpdateMenuEditPointOrient (wxUpdateUIEvent & event);
	void OnUpdateMenuEditClearSelection (wxUpdateUIEvent & event);
	void OnUpdateMenuEditQueryRun (wxUpdateUIEvent & event);
    void OnUpdateMenuEditQueryAdd (wxUpdateUIEvent & event);
	void OnUpdateMenuShowSnapping (wxUpdateUIEvent & event);
	void OnUpdateMenuShowShortcuts (wxUpdateUIEvent & event);
	void OnUpdateMenuShowQuery (wxUpdateUIEvent & event);
	void OnUpdateMenuShowTOC (wxUpdateUIEvent & event);
	void OnUpdateMenuShowLog (wxUpdateUIEvent & event);
	void OnUpdateMenuShowInfo (wxUpdateUIEvent & event);
	void OnUpdateMenuFlipLine (wxUpdateUIEvent & event);
	void OnUpdateMenuEditSharedNode (wxUpdateUIEvent & event);
	void OnUpdateStatisticsDialog (wxUpdateUIEvent & event);
    void OnUpdateGeometryValidity (wxUpdateUIEvent & event);
    
    void _CreateMenu();
	void _CreateToolBar();
    void _CreateAccelerators();
	void _LoadPreference(bool reload);
    void _CreatePerspectives();
    
	DECLARE_EVENT_TABLE();
	DECLARE_DYNAMIC_CLASS(ToolMapFrame)
    
    
public:
	ToolMapFrame();
	ToolMapFrame(wxFrame *frame, const wxString& title,
			 wxPoint  pos,  wxSize  size,
				 const wxString & name);
	~ToolMapFrame();
};




#endif
