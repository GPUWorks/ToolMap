/***************************************************************************
								scale.cpp
                   Deals with the scale properties
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

#include "scale.h"


bool ScaleTM::GetScaleFromLong(const long & scale, wxString & stringscale)
{
	stringscale = wxString::Format( _T("1:%ld"),scale);
	if (stringscale.Len() > 2)
	{
		return TRUE;
	}
	return FALSE;
}

long ScaleTM::GetScaleFromString (const wxString & scale)
{
	long myLongScale = -1;
	
	wxString myTempscale = scale.AfterFirst(':');
	// check that the scale string isn't empty
	if (!myTempscale.IsEmpty())
	{
		myTempscale.ToLong(&myLongScale);
	}
	return myLongScale;
	
}
