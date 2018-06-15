/***************************************************************************
 tmimportdatawiz.cpp
 
 -------------------
 copyright            : (C) 2018 TERRANUM Pascal Horton
 email                : pascal.horton at terranum dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "tmimportdatawiz.h"
#include "../gis/tmimport.h"
#include "../gis/tmimportgis.h"
#include "../gis/tmimportcsv.h"
#include "../core/projectmanager.h"

ImportDataWizard::ImportDataWizard(wxWindow *window, wxWindowID id, ProjectManager *prjManager) :
        tmWizardImport::tmWizardImport(window, id, _("Import data")),
        m_Import(nullptr),
        m_PrjManager(prjManager)
{
    m_Import = new tmImportGIS();
    this->Connect( wxID_ANY, wxEVT_WIZARD_BEFORE_PAGE_CHANGED, wxWizardEventHandler( ImportDataWizard::OnWizardBeforePageChanged ) );
}

ImportDataWizard::~ImportDataWizard()
{
    wxDELETE(m_Import);
    this->Disconnect( wxID_ANY, wxEVT_WIZARD_BEFORE_PAGE_CHANGED, wxWizardEventHandler( ImportDataWizard::OnWizardBeforePageChanged ) );
}

int ImportDataWizard::GetPageIndex(wxWizardPage* page)
{
    for (int i = 0; i < m_pages.size(); ++i) {
        if (m_pages[i] == page) {
            return i;
        }
    }
    wxFAIL;

    return wxNOT_FOUND;
}

void ImportDataWizard::OnWizardBeforePageChanged(wxWizardEvent &event)
{
    auto *page = dynamic_cast<wxWizardPageSimple *> (event.GetPage());

    switch (GetPageIndex(page)) {
        case (0): { // Leaving selection the file type (SHP or CSV)

            // Change the file picker accordingly to the type of file.
            wxDELETE(m_Import);
            wxASSERT(m_radioBtnSHP->GetValue() != m_radioBtnCSV->GetValue());
            ResetFilePicker(page);
            EnableNextButton(false);

            break;
        }
        case (1): { // Leaving File picker page

            // If going back, enable the Next button again
            if (!event.GetDirection()) {
                EnableNextButton();
                return;
            }

            if (m_Import->GetFileType() == tmIMPORT_TYPE_SHP) {
                // Shapefiles do not need the definition of X/Y
                page->SetNext(m_pages[3]);
            } else {
                page->SetNext(m_pages[2]);
                SetXYColumnsOptions();
            }

            SetTargetsOptions();

            break;
        }
        case (2):{ // Leaving choice of X/Y columns

            wxASSERT(m_Import->GetFileType() == tmIMPORT_TYPE_CSV);

            // If going back
            if (!event.GetDirection()) {
                return;
            }

            GetXYColumnsSelection();

            break;
        }
        case (3):{ // Leaving choice of target

            // If going back, shapefiles do not need the definition of X/Y
            if (!event.GetDirection()) {
                if (m_Import->GetFileType() == tmIMPORT_TYPE_SHP) {
                    page->SetPrev(m_pages[1]);
                } else {
                    page->SetPrev(m_pages[2]);
                }
                return;
            }

            GetTargetSelection();

            // If frame, terminate
            if(m_Import->GetTarget() == TOC_NAME_FRAME) {
                page->SetNext(nullptr);
                return;
            }

            SetLayerOptions();

            break;
        }
        case (4):{ // Leaving choice of layer

            // If going back
            if (!event.GetDirection()) {
                return;
            }

            GetLayerSelection();
            SetAttributeOptions();

            break;
        }
        case (5):{ // Leaving attributes matching

            // If going back
            if (!event.GetDirection()) {
                return;
            }

            GetAttributeSelection();

            // If no enumeration, terminate
            if(!m_Import->HasEnumAttributes()) {
                page->SetNext(nullptr);
                return;
            }

            SetEnumerationOptions();

            break;
        }
        case (6):{

            // If going back
            if (!event.GetDirection()) {
                return;
            }

            break;
        }
        default:
            wxLogError(_("Page index not recognized (%d)."), GetPageIndex(page));
    }

}

void ImportDataWizard::SetEnumerationOptions() const
{
    // Get list of attributes from project
    ProjectDefMemoryLayers *layer = nullptr;
    PrjDefMemManage* prjDefMem = m_PrjManager->GetMemoryProjectDefinition();
    layer = prjDefMem->GetActiveLayer();
    wxASSERT(layer);

	bool skipPage = true;

    for (int i = 0; i < m_Import->GetAttributesCount(); ++i) {
        if(!m_Import->AttributeIsEnum(i)) {
            continue;
        }

        // Extract unique values from the file
        wxString attNameInFile = m_Import->GetAttributeNameInFile(i);
        wxArrayString attValuesInFile;
        m_Import->GetExistingAttributeValues(attNameInFile, attValuesInFile);

        if (attValuesInFile.IsEmpty()) {
            continue;
        }

		skipPage = false;

        // Extract enumerations from the project
        wxString attNameInDB = m_Import->GetAttributeNameInDB(i);
        wxArrayString attValuesInDB;
        for (int j = 0; j < prjDefMem->GetCountFields(); j++) {
            ProjectDefMemoryFields *fieldObj = prjDefMem->GetNextField();
            wxASSERT(fieldObj);
            if (fieldObj->m_Fieldname.IsSameAs(attNameInDB)) {
                for (unsigned int k = 0; k < fieldObj->m_pCodedValueArray.GetCount(); k++) {
                    ProjectDefMemoryFieldsCodedVal *myVal = fieldObj->m_pCodedValueArray[k];
                    wxASSERT(myVal);
                    attValuesInDB.Add(myVal->m_ValueName);
                }

                break;
            }
        }

        // Add a row for the attribute names
        wxStaticText *textAttNameDB = new wxStaticText(m_scrolledWindow2, wxID_ANY,
                                                       m_Import->GetAttributeNameInDB(i), wxDefaultPosition,
                                                       wxDefaultSize, 0);
        textAttNameDB->SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL,
                                       false, wxEmptyString));
        textAttNameDB->Wrap(-1);
        m_fgSizerEnums->Add(textAttNameDB, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        wxStaticText *textAttNameFile = new wxStaticText(m_scrolledWindow2, wxID_ANY,
                                                         m_Import->GetAttributeNameInFile(i), wxDefaultPosition,
                                                         wxDefaultSize, 0);
        textAttNameFile->SetFont(wxFont( 11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL,
                                          false, wxEmptyString ) );
        textAttNameFile->Wrap(-1);
        m_fgSizerEnums->Add(textAttNameFile, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        // Fill lists
        for (int j = 0; j < attValuesInFile.GetCount(); ++j) {
            wxStaticText *textAttValue = new wxStaticText(m_scrolledWindow2, wxID_ANY, attValuesInFile.Item(j),
                                                          wxDefaultPosition, wxDefaultSize, 0);
            textAttValue->Wrap(-1);
            m_fgSizerEnums->Add(textAttValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

            wxChoice *choiceAttValue = new wxChoice(m_scrolledWindow2, wxID_ANY, wxDefaultPosition,
                                                    wxDefaultSize, attValuesInDB, 0);
            choiceAttValue->SetSelection(0);
            m_fgSizerEnums->Add(choiceAttValue, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

            // Find potential match
            for (int k = 1; k < attValuesInDB.GetCount(); ++k) {
                if (attValuesInDB.Item(k).IsSameAs(attValuesInFile.Item(j), false)) {
                    choiceAttValue->SetSelection(k);
                }
            }
        }
    }

    // If no enum values to map, terminate
    if (skipPage) {
        auto *page = dynamic_cast<wxWizardPageSimple *> (GetCurrentPage());
        page->SetNext(nullptr);
        return;
    }
}

void ImportDataWizard::GetAttributeSelection() const
{
    m_Import->ClearAttributes();

    // Get list of attributes from project
    ProjectDefMemoryLayers *layer = nullptr;
    PrjDefMemManage* prjDefMem = m_PrjManager->GetMemoryProjectDefinition();
	layer = prjDefMem->GetActiveLayer();
	wxASSERT(layer);

    for (int i = 0; i < m_fgSizerAttributes->GetItemCount(); i += 2) {
        wxWindow* itemFile = m_fgSizerAttributes->GetItem(i)->GetWindow();
        auto textFile = dynamic_cast<wxStaticText *>(itemFile);
        wxString fileAttribute = textFile->GetLabel();

        wxWindow* itemDB = m_fgSizerAttributes->GetItem(i + 1)->GetWindow();
        auto choiceDB = dynamic_cast<wxChoice *>(itemDB);
        wxString dbAttribute = choiceDB->GetString(choiceDB->GetSelection());

        if (!dbAttribute.IsSameAs("-", false) && !dbAttribute.IsEmpty()) {

            // Get type
            PRJDEF_FIELD_TYPE type;
            for (int j = 0; j < prjDefMem->GetCountFields(); j++) {
                ProjectDefMemoryFields *fieldObj = prjDefMem->GetNextField();
                if (fieldObj->m_Fieldname.IsSameAs(dbAttribute)) {
                    type = fieldObj->m_FieldType;
                    break;
                }
            }
            m_Import->AddAttribute(fileAttribute, dbAttribute, type);
        }
    }

    m_fgSizerEnums->Clear(true);

}

void ImportDataWizard::SetAttributeOptions() const
{

    if (m_fgSizerAttributes->GetRows() == 0) {
        // List attributes from file
        wxArrayString fieldListFile;
        m_Import->GetFieldNames(fieldListFile);

        // Remove some fields
        for (unsigned int k = 0; k < fieldListFile.GetCount(); ++k) {
            wxString item = fieldListFile.Item(k);
            if(item.IsSameAs("TM_OID", false) || item.IsSameAs("OBJ_CD", false) || item.IsSameAs("OBJ_DESC", false) ||
               item.IsSameAs("x", false) || item.IsSameAs("y", false) || item.IsSameAs("id", false)) {
                fieldListFile.RemoveAt(k);
                k--;
            }
        }

        // If no attribute, terminate
        if (fieldListFile.IsEmpty()) {
            auto *page = dynamic_cast<wxWizardPageSimple *> (GetCurrentPage());
            page->SetNext(nullptr);
            return;
        }

        // List attributes from project
        ProjectDefMemoryLayers *layer = nullptr;
        PrjDefMemManage* prjDefMem = m_PrjManager->GetMemoryProjectDefinition();
        layer = prjDefMem->FindLayer(m_Import->GetLayerName());
        wxArrayString fieldListPrj;
        for (int j = 0; j < prjDefMem->GetCountFields(); j++) {
            ProjectDefMemoryFields *fieldObj = prjDefMem->GetNextField();
            fieldListPrj.Add(fieldObj->m_Fieldname);
        }
        fieldListPrj.Insert("-", 0);

        // Fill lists
        for (int i = 0; i < fieldListFile.GetCount(); ++i) {
            wxStaticText* textAttribute = new wxStaticText( m_scrolledWindow1, wxID_ANY, fieldListFile.Item(i), wxDefaultPosition, wxDefaultSize, 0 );
            textAttribute->Wrap( -1 );
            m_fgSizerAttributes->Add( textAttribute, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

            wxChoice* choiceAttribute = new wxChoice( m_scrolledWindow1, wxID_ANY, wxDefaultPosition, wxDefaultSize, fieldListPrj, 0 );
            choiceAttribute->SetSelection( 0 );
            m_fgSizerAttributes->Add( choiceAttribute, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

            // Find potential match
            for (int j = 1; j < fieldListPrj.GetCount(); ++j) {
                if (fieldListPrj.Item(j).IsSameAs(fieldListFile.Item(i), false)) {
                    choiceAttribute->SetSelection( j );
                }
            }
        }
    }

}

void ImportDataWizard::GetLayerSelection() const
{
    // If changed, clear attributes
    if (! m_choiceLayer->GetStringSelection().IsSameAs(m_Import->GetLayerName())) {
        m_fgSizerAttributes->Clear(true);
    }

    m_Import->SetLayerName(m_choiceLayer->GetStringSelection());
}

void ImportDataWizard::SetLayerOptions() const
{
    m_choiceLayer->Clear();
    m_choiceLayer->Append(m_PrjManager->GetDatabase()->GetLayerNameByType(m_Import->GetTarget()));
    m_choiceLayer->SetSelection(0);
}

void ImportDataWizard::GetTargetSelection() const
{
    wxArrayInt supportedTargets = m_Import->GetTargetSupported();
    wxASSERT(supportedTargets.GetCount() == m_choiceTarget->GetCount());
    int myTarget = supportedTargets.Item(m_choiceTarget->GetSelection());
    m_Import->SetTarget((TOC_GENERIC_NAME) myTarget);
}

void ImportDataWizard::SetTargetsOptions() const
{
    wxArrayString supportedTargetsName = m_Import->GetTargetSupportedName();
    m_choiceTarget->Clear();
    m_choiceTarget->Append(supportedTargetsName);
    m_choiceTarget->SetSelection(0);
}

void ImportDataWizard::GetXYColumnsSelection() const
{
    wxASSERT(m_Import->GetFileType() == tmIMPORT_TYPE_CSV);

    tmImportCSV *importCSV = (tmImportCSV *) m_Import;
    if (m_choiceX->GetCount() > 0 && m_choiceY->GetCount() > 0) {
        importCSV->SetColumn(m_choiceX->GetSelection(), m_choiceY->GetSelection());
    } else {
        importCSV->SetColumn(wxNOT_FOUND, wxNOT_FOUND);
    }
}

void ImportDataWizard::SetXYColumnsOptions() const
{
    wxASSERT(m_Import->GetFileType() == tmIMPORT_TYPE_CSV);

    // Fill X/Y lists
    tmImportCSV *importCSV = (tmImportCSV *) m_Import;
    wxArrayString cols;
    importCSV->ListColumns(cols);
    importCSV->GuessXYcolumns(cols);
    for (unsigned int i = 0; i < cols.GetCount(); i++) {
        wxString colText = wxString::Format(_("Column %d: %s"), i + 1, cols.Item(i).c_str());
        cols.Item(i) = colText;
    }

    m_choiceX->Clear();
    m_choiceY->Clear();
    m_choiceX->Append(cols);
    m_choiceY->Append(cols);

    int xSelection = 0;
    int ySelection = 0;
    if (importCSV->GetColumnX() != wxNOT_FOUND && importCSV->GetColumnY() != wxNOT_FOUND) {
        xSelection = importCSV->GetColumnX();
        ySelection = importCSV->GetColumnY();
    }
    m_choiceX->SetSelection(xSelection);
    m_choiceY->SetSelection(ySelection);
}

void ImportDataWizard::ResetFilePicker(const wxWizardPageSimple *page)
{
    wxFilePickerCtrl *filePicker = NULL;

    m_infoValueCtrl1->SetLabel(wxEmptyString);
    m_infoValueCtrl2->SetLabel(wxEmptyString);

    if (m_radioBtnSHP->GetValue()) {

        m_Import = new tmImportGIS();
        filePicker = new wxFilePickerCtrl(page->GetNext(), wxID_ANY, wxEmptyString, _("Select a SHP file"),
                                          _("ESRI's shapefile (*.shp)|*.shp"));
        m_infoLabelCtrl1->SetLabel(_("Geometry type:"));
        m_infoLabelCtrl2->SetLabel(_("Number of features:"));

    } else if (m_radioBtnCSV->GetValue()) {

        m_Import = new tmImportCSV();
        filePicker = new wxFilePickerCtrl(page->GetNext(), wxID_ANY, wxEmptyString, _("Select a CSV file"),
                                          _("Semi-colon separeted file (*.csv)|*.csv"));
        m_infoLabelCtrl1->SetLabel(_("Number of columns:"));
        m_infoLabelCtrl2->SetLabel(_("Number of rows:"));

    }

    m_sizerFilePath->Replace(m_filePicker, filePicker);
    page->GetNext()->RemoveChild(m_filePicker);
    m_filePicker->Disconnect(wxEVT_COMMAND_FILEPICKER_CHANGED,
                             wxFileDirPickerEventHandler(ImportDataWizard::OnFileChanged), NULL, this);
    wxDELETE(m_filePicker);
    m_filePicker = filePicker;
    m_filePicker->Connect(wxEVT_COMMAND_FILEPICKER_CHANGED,
                          wxFileDirPickerEventHandler(ImportDataWizard::OnFileChanged), NULL, this);
    page->GetNext()->Layout();
}

void ImportDataWizard::OnWizardFinished(wxWizardEvent &event)
{

}

void ImportDataWizard::EnableNextButton(bool state)
{
    SetControlEnable(wxID_FORWARD, state);
}

void ImportDataWizard::EnableBackButton(bool state)
{
    SetControlEnable(wxID_BACKWARD, state);
}

void ImportDataWizard::SetControlEnable(int id, bool state)
{
    wxWindow *win = wxWindow::FindWindowById(id);
    if (win) win->Enable(state);
}

void ImportDataWizard::OnFileChanged(wxFileDirPickerEvent &event)
{
    wxASSERT(m_Import);

    if (!m_Import->Open(event.GetPath())) {
        return;
    }

    m_infoValueCtrl2->SetLabel(wxString::Format(_T("%ld"), m_Import->GetFeatureCount()));

    if (m_Import->GetFileType() == tmIMPORT_TYPE_SHP) {
        m_infoValueCtrl1->SetLabel(TM_GIS_SPATIAL_TYPES_STRING[m_Import->GetGeometryType()]);
    } else if (m_Import->GetFileType() == tmIMPORT_TYPE_CSV) {
        m_infoValueCtrl1->SetLabel(wxString::Format(_T("%d"), m_Import->GetFieldCount()));
    } else {
        wxFAIL;
    }

    if (m_Import->GetFileType() == tmIMPORT_TYPE_SHP) {
        if (m_Import->GetGeometryType() > LAYER_SPATIAL_POINT) {
            wxLogError(_("Unable to import shapefiles of type : '%s'"),
                       TM_GIS_SPATIAL_TYPES_STRING[m_Import->GetGeometryType()].c_str());
            return;
        }
    }

    EnableNextButton();
}