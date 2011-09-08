/***************************************************************************
 pdfdocument.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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


#include "pdfdocument.h"
#include "prjdefmemmanage.h"

bool PdfDocument::_GenerateTitle() {
	m_pdf.SetFont("Helvetica", "B", m_FontSize + 4);
	m_pdf.Cell(0, m_LineSpacing, 
			   wxString::Format(_("'%s' Data Model"),m_prjName.Upper()),
			   wxPDF_BORDER_BOTTOM, 1);
	m_pdf.SetFont("Helvetica", "I", m_FontSize);
	
	
	
	
	m_pdf.Cell(m_pdf.GetPageWidth() / 2.0 - m_pdf.GetLeftMargin() , m_LineSpacing,
			   wxString::Format(_("Generated by ToolMap on: %s"),
								wxDateTime::Now().FormatISOCombined(' ')),
			   wxPDF_BORDER_NONE, 0);
	
	m_pdf.Cell(m_pdf.GetPageWidth() / 2.0 - m_pdf.GetRightMargin(), m_LineSpacing,
			   wxString::Format(_("Page : %d / {nb}"), m_pdf.PageNo()),
			   wxPDF_BORDER_NONE, 0, wxPDF_ALIGN_RIGHT);
	m_pdf.Ln();
	return true;
}



PdfDocument::PdfDocument(PrjDefMemManage * project) {
	wxASSERT(project);
	m_prjName = project->m_PrjName;
	m_PaperSize = wxPAPER_A4;
	m_PaperOrientation = wxPORTRAIT;
	m_FontSize = 10; 
	m_LineSpacing = 6;
	
	// adding layers (copy of them)
	for (int i = 0; i< project->GetCountLayers(); i++) {
		PdfLayer * myLayer = new PdfLayer(this, project->m_PrjLayerArray.Item(i));
		wxASSERT(myLayer);
		m_pdfLayers.Add(myLayer);
	}
	wxLogDebug("%ld PDF Layers added", m_pdfLayers.GetCount());
}



PdfDocument::~PdfDocument() {
	// manually clearing array of layers, Clear() or Empty() didn't work
	unsigned int iCount = m_pdfLayers.GetCount();
	for (unsigned int i = 0; i<iCount; i++)
	{
		PdfLayer * myLayer = m_pdfLayers.Item(0);
		wxDELETE(myLayer);
		m_pdfLayers.RemoveAt(0);
	}
	wxASSERT(m_pdfLayers.GetCount()==0);
}



bool PdfDocument::Generate(const wxFileName & filename) {

	m_pdf.AliasNbPages();
	
	for (unsigned int i = 0; i<m_pdfLayers.GetCount(); i++) {
		m_pdf.AddPage(m_PaperOrientation, m_PaperSize);
		_GenerateTitle();
		m_pdf.Ln();
		m_pdfLayers.Item(i)->Generate(&m_pdf);
	}
	m_pdf.SaveAsFile(filename.GetFullPath());
	wxLogMessage(_("Data model exported to: '%s'"), filename.GetFullName());
	return true;
}



void PdfDocument::SetFontSize(int value) {
	m_FontSize = value;
}



void PdfDocument::SetLineSpacing(double value) {
	m_LineSpacing = value;
}