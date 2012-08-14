#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/app.h>
#include <wx/cmdline.h>
#include <wx/dir.h>

#include "tmProjectMerge.h"

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, "h", "help", "show this help message",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_SWITCH, "m", "merge", "Merge Slave into Master" },
    { wxCMD_LINE_SWITCH, "v", "verbose", "Be more verbose" },
    { wxCMD_LINE_NONE }
};



bool CopyDir(wxString from, wxString to) {
	wxString SLASH = wxFILE_SEP_PATH;
    
	// append a slash if there is not one (for easier parsing)
	// because who knows what people will pass to the function.
	if (to[to.length()-1] != SLASH) {
		to += SLASH;
	}
	// for both dirs
	if (from[from.length()-1] != SLASH) {
		from += SLASH;
	}
	
	// first make sure that the source dir exists
	if(!wxDir::Exists(from)) {
		wxLogError(from + " does not exist.  Can not copy directory.");
	}
	else {
		// check on the destination dir
		// if it doesn't exist...
		if(!wxDir::Exists(to)) {
			// if it doesn't get created
			if(!wxFileName::Mkdir(to, 0777, wxPATH_MKDIR_FULL)) {
				// Send an error
				wxLogError(to + " could not be created.");
				// And exit gracefully
				return false;
			}
		}
		
		// The directories to traverse
		wxArrayString myDirs;
		myDirs.Add("");
		
		// loop through each directory.. storing all sub directories
		// and copying over all files.. the final iteration of one loop
		// should begin an iteration for any subdirectories discovered
		// on the previous pass
		// (rather than pragma, unsigned int will shut the MS compiler up)
		for (unsigned int i = 0; i < myDirs.size(); i++) {
			
			// get the next directory
			wxDir nextDir(from + myDirs[i]);
			
			// check that it exists in destination form
			if(!wxDir::Exists(to + myDirs[i])) {
				// if it doesn't, then create it
				if(!wxFileName::Mkdir(to + myDirs[i], 0777, wxPATH_MKDIR_FULL)) {
					// If it doesn't create, error
					wxLogError(to + myDirs[i] + " could not be created.");
					// And exit gracefully
					return false;
				}
			}
			
			// get the first file in the next directory
			wxString nextFile;
			bool process = nextDir.GetFirst(&nextFile);
			
			// and while there are still files to process
			while (process) {
				
				// If this file is a directory
				if(wxDir::Exists(from+nextFile)) {
					// then append it for creation/copying
					myDirs.Add(nextFile + SLASH);   // only add the difference
				}
				else {
					
					// otherwise just go ahead and copy the file over
					if(!wxCopyFile(from + myDirs[i] + nextFile,
								   to   + myDirs[i] + nextFile)) {
						// error if we couldn't
						wxLogError("Could not copy " +
								   from + myDirs[i] + nextFile + " to "
								   + to + myDirs[i] + nextFile);
					}
				}
				// and get the next file
				process = nextDir.GetNext(&nextFile);
			}
		}
		return true;
	}
	return false;
}




int main(int argc, char **argv)
{
    // debugging string for OSX
    // this is needed for viewing string content with Xcode !!
    wxString myTest = _T("Test debugging");
    myTest.ToUTF8().data();
    myTest.Len();
    
    wxApp::CheckBuildOptions(WX_BUILD_OPTIONS_SIGNATURE, "program");
    wxInitializer initializer;
    if ( !initializer )
    {
        fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }
    
    wxString myLogoTxt = _T("*\n* ToolMerge \n* Checking and Merging ToolMap projects \n* (c) Copyright 2012 Lucien Schreiber - CREALP . All Rights Reserved. \n*\n");
    wxCmdLineParser parser(cmdLineDesc, argc, argv);
    parser.AddParam(_T("[master project path]"), wxCMD_LINE_VAL_STRING);
    parser.AddParam(_T("[slave project path]"), wxCMD_LINE_VAL_STRING);
    parser.SetLogo(myLogoTxt);
    
    if (parser.Parse() != 0) {
        return 0;
    }
    
    bool beVerbose = parser.Found("verbose");
    
    // cmd line is correct !!
    wxPrintf(myLogoTxt);
    wxString myMasterFileName = parser.GetParam(0);
    wxString mySlaveFileName = parser.GetParam(1);
    
    if (beVerbose) {
        wxPrintf(_("Master: '%s'\nSlave:  '%s'\n"), myMasterFileName, mySlaveFileName);
    }
    
    // TODO: Remove this temp code (process on backup)
    wxString myMasterFileNameBkp = myMasterFileName + _("_bkp");
    wxString mySlaveFileNameBkp = mySlaveFileName + _("_bkp");
    
    if (wxDir::Exists(myMasterFileNameBkp)) {
        wxFileName myDir (myMasterFileNameBkp, _T(""));
        myDir.Rmdir(wxPATH_RMDIR_RECURSIVE);
    }
    if (wxDir::Exists(mySlaveFileNameBkp)) {
        wxFileName myDir (mySlaveFileNameBkp, _T(""));
        myDir.Rmdir(wxPATH_RMDIR_RECURSIVE);
    }
    if (CopyDir(myMasterFileName, myMasterFileNameBkp)==false) {
        wxLogError(_("Creating Master Backup before processing failed!"));
    }
    if (CopyDir(mySlaveFileName, mySlaveFileNameBkp)==false) {
        wxLogError(_("Creating Slave Backup before processing failed!"));
    }
    
    
    bool bCheckOk = false;
    tmProjectMerge myCheckMerger(myMasterFileNameBkp, mySlaveFileNameBkp);
    myCheckMerger.SetVerbose(beVerbose);
    // checking here
    if (beVerbose) {
        wxPrintf(_("\nCHECKING...\n"));
    }
    wxStopWatch sw;
    if(myCheckMerger.CheckSimilar()==true){
        wxPrintf(_("OK projects are similar\n"));
        bCheckOk = true;
    }
    else {
        wxPrintf(_("Checking FAILED! see bellow\n"));
        wxArrayString myErrors = myCheckMerger.GetErrors();
        for (unsigned int i = 0; i< myErrors.GetCount(); i++) {
            wxPrintf(myErrors.Item(i) + _T("\n"));
        }
    }
    
    if (beVerbose) {
        wxPrintf(_("Checking projects in %ld [ms]\n"), sw.Time());
    }
    
    if (parser.Found("merge") && bCheckOk == false) {
        wxPrintf(_("Merging not allowed, projects are different!\n"));
        return 1;
    }
    
    
    sw.Start(0);
    if (parser.Found("merge")) {
        // merging here
        if (beVerbose) {
            wxPrintf(_("\nMERGING...\n"));
        }
        
        if(myCheckMerger.MergeIntoMaster()==true){
            wxPrintf(_("OK Project Merged into '%s' in %ld [ms]"), myMasterFileNameBkp, sw.Time());
        }
        else{
            wxPrintf(_("Merge FAILED! see bellow\n"));
            wxArrayString myErrors = myCheckMerger.GetErrors();
            for (unsigned int i = 0; i< myErrors.GetCount(); i++) {
                wxPrintf(myErrors.Item(i) + _T("\n"));
            }
        }
    }
    
    return 0;
}
