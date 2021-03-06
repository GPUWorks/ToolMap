/***************************************************************************
 information_dlg.h
 Display informations dialog for layers and features
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


#ifndef _INFORMATION_DLG_H_
#define _INFORMATION_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


#include <wx/html/htmlwin.h>
#include <wx/notebook.h>
#include "../components/wxtreemultictrl/wxTreeMultiCtrl.h"    // for wxTreeMultiCtrl
#include "../components/wxtreemultictrl/TreeMultiItemWindow.h"

class tmTOCCtrl;

class tmSelectedDataMemory;

class tmSelectionInfoCtrl;

class tmLayerInfoCtrl;

class PrjDefMemManage;


const int ID_INFORMATION_DLG = 22401;


class InformationDLG : public wxDialog
{
private:
    tmTOCCtrl *m_TOC;
    tmSelectedDataMemory *m_Selected;
    tmSelectionInfoCtrl *m_SelCtrl;
    tmLayerInfoCtrl *m_LayerCtrl;


    void _CreateControls();

    void OnCloseDlg(wxCloseEvent &event);


DECLARE_EVENT_TABLE();
public:
    InformationDLG(wxWindow *parent, tmTOCCtrl *toc,
                   tmSelectedDataMemory *sel, wxWindowID id,
                   const wxString &title = _("Informations"),
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxSTAY_ON_TOP);

    virtual ~InformationDLG();

    virtual bool TransferDataToWindow();

    void UpdateLayer();

    void UpdateSelection();

    void SetProject(PrjDefMemManage *project);

};


/***************************************************************************//**
Layer informations (wxHtmlWindow)
author Lucien Schreiber (c) CREALP 2010
date 01 mars 2010
*******************************************************************************/
class tmLayerInfoCtrl : public wxHtmlWindow
{
public:

    tmLayerInfoCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);

    virtual ~tmLayerInfoCtrl();

    void UpdateLayer(tmTOCCtrl *toc);

};


/***************************************************************************//**
Selection Control
author Lucien Schreiber (c) CREALP 2010
date 02 mars 2010
*******************************************************************************/
const int ID_POPUP_MOVE_TO = 18500;
const int ID_POPUP_ZOOM_TO = 18501;
const int ID_POPUP_REMOVE_FROM_SEL = 18502;
const int ID_POPUP_ONLY_THIS_OBJ = 18503;


class tmSelectionInfoCtrl : public wxTreeMultiCtrl
{
private:
    tmSelectedDataMemory *m_Selected;
    long m_ClickedItemID;
    PrjDefMemManage *m_Project;
    tmTOCCtrl *m_Toc;


    bool _GetItemByMousePos(wxTreeMultiItem &item, const wxPoint &position);

    void _DeleteAllInfos(const wxTreeMultiItem &dontdelete);

    wxMenu *_CreatePopupMenu();

    void _CreateInfoControl(const wxTreeMultiItem &item,
                            const wxArrayString &header,
                            const wxArrayString &values);

    void _UpdateSelection();

    bool _GetData(long oid, wxArrayString &header, wxArrayString &values);


    // event function
    void OnItemLeftClick(wxMouseEvent &event);

    void OnItemRightClick(wxMouseEvent &event);

    void OnPopupSelectionThis(wxCommandEvent &event);

    void OnPopupSelectionRemove(wxCommandEvent &event);

    void OnPopupMove(wxCommandEvent &event);

    void OnPopupZoom(wxCommandEvent &event);

    void OnPopupCopy(wxCommandEvent &event);


DECLARE_EVENT_TABLE();

public:
    tmSelectionInfoCtrl(wxWindow *window, wxWindowID id,
                        tmSelectedDataMemory *sel, tmTOCCtrl *toc,
                        const wxPoint &pos = wxDefaultPosition,
                        const wxSize &size = wxDefaultSize,
                        long style = wxTMC_BG_ADJUST_ALL | wxTMC_SPAN_WIDTH);


    ~tmSelectionInfoCtrl();

    void UpdateSelection();

    void SetProject(PrjDefMemManage *project);


};


#endif
