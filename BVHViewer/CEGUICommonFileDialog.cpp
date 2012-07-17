#include "CEGUIHashMapTemplates.h"
#include "CEGUICommonFileDialog.h"
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUICombobox.h>
#include <direct.h>
#include <io.h>
 
namespace CEGUI
{
    /*************************************************************************
        CommonFileDialogInput::Getters & Setters
    *************************************************************************/
    uint CommonFileDialogInput::getHandle (void)
    {
        return _uHandle;
    }
 
    String CommonFileDialogInput::getFileName (void)
    {
        return _szFileName;
    }
 
    String CommonFileDialogInput::getFilter (void)
    {
        return _szFilter;
    }
 
    String CommonFileDialogInput::getDefaultExtension (void)
    {
        return _szDefaultExtension;
    }
 
    String CommonFileDialogInput::getTitle (void)
    {
        return _szTitle;
    }
 
    bool CommonFileDialogInput::getOpenFileDialog (void)
    {
        return _bOpenFileDialog;
    }
 
    bool CommonFileDialogInput::getDoPrompt (void)
    {
        return _bDoPrompt;
    }
 
    String CommonFileDialogInput::getPromptText (void)
    {
        return _szPromptText;
    }
 
    void CommonFileDialogInput::setHandle (uint uHandle)
    {
        _uHandle = uHandle;
    }
 
    void CommonFileDialogInput::setFilter (const String szFilter)
    {
        _szFilter = szFilter;
    }
 
    void CommonFileDialogInput::setDefaultExtension (const String szDefaultExtension)
    {
        _szDefaultExtension = szDefaultExtension;
    }
 
    void CommonFileDialogInput::setFileName (const String szFileName)
    {
        _szFileName = szFileName;
    }
 
    void CommonFileDialogInput::setTitle (const String szTitle)
    {
        _szTitle = szTitle;
    }
 
    void CommonFileDialogInput::setOpenFileDialog (const bool bOpenFileDialog)
    {
        _bOpenFileDialog = bOpenFileDialog;
    }
 
    void CommonFileDialogInput::setDoPrompt (const bool bDoPrompt)
    {
        _bDoPrompt = bDoPrompt;
    }
 
    void CommonFileDialogInput::setPromptText (const String szPromptText)
    {
        _szPromptText = szPromptText;
    }
 
    /*************************************************************************
        CommonFileDialogOutput::Getters & Setters
    *************************************************************************/
    uint CommonFileDialogOutput::getHandle (void)
    {
        return _uHandle;
    }
 
    String CommonFileDialogOutput::getFullQualifiedFileName (void)
    {
        return _szFullQualifiedFileName;
    }
 
    String CommonFileDialogOutput::getDrive (void)
    {
        return _szDrive;
    }
 
    String CommonFileDialogOutput::getAbsolutePath (void)
    {
        return _szAbsolutePath;
    }
 
    String CommonFileDialogOutput::getRelativePath (void)
    {
        return _szRelativePath;
    }
 
    String CommonFileDialogOutput::getFileName (void)
    {
        return _szFileName;
    }
 
    CommonFileDialogAction CommonFileDialogOutput::getAction (void)
    {
        return _action;
    }
 
    bool CommonFileDialogOutput::isFileExisting (void)
    {
        return _bFileExisting;
    }
 
    void CommonFileDialogOutput::setHandle (const uint uHandle)
    {
        _uHandle = uHandle;
    }
 
    void CommonFileDialogOutput::setFullQualifiedFileName (const String szFullQualifiedFileName)
    {
        _szFullQualifiedFileName = szFullQualifiedFileName;
    }
 
    void CommonFileDialogOutput::setDrive (const String szDrive)
    {
        _szDrive = szDrive;
    }
 
    void CommonFileDialogOutput::setAbsolutePath (const String szAbsolutePath)
    {
        _szAbsolutePath = szAbsolutePath;
    }
 
    void CommonFileDialogOutput::setRelativePath (const String szRelativePath)
    {
        _szRelativePath = szRelativePath;
    }
 
    void CommonFileDialogOutput::setFileName (const String szFileName)
    {
        _szFileName = szFileName;
    }
 
    void CommonFileDialogOutput::setAction (CommonFileDialogAction action)
    {
        _action = action;
    }
 
    void CommonFileDialogOutput::setFileExisting (bool bFileExisting)
    {
        _bFileExisting = bFileExisting;
    }
 
    /*************************************************************************
        Singleton
    *************************************************************************/
    CommonFileDialog::_InstPtr CommonFileDialog::sm_ptr;
 
    /*************************************************************************
        CCommonFileDialog::CCommonFileDialog
    *************************************************************************/
    CCommonFileDialog::CCommonFileDialog(void)
    {
        // Load the window layout, initialise the widget attributes and subscribe to events
        _mWindow = WindowManager::getSingleton().loadWindowLayout((utf8*)NAME_LAYOUT_COMMON_FILE_DIALOG.c_str());
        _mBtnBack = (PushButton*)WindowManager::getSingleton().getWindow((utf8*)NAME_PUSHBUTTON_BACK.c_str());
        _mBtnOk = (PushButton*)WindowManager::getSingleton().getWindow((utf8*)NAME_PUSHBUTTON_OK.c_str());
        _mBtnCancel = (PushButton*)WindowManager::getSingleton().getWindow((utf8*)NAME_PUSHBUTTON_CANCEL.c_str());
        _mCmbFile = (Combobox*)WindowManager::getSingleton().getWindow((utf8*)NAME_COMBOBOX_FILE.c_str());
        _mCmbDrives = (Combobox*)WindowManager::getSingleton().getWindow((utf8*)NAME_COMBOBOX_DRIVE.c_str());
        _mCmbExtensions = (Combobox*)WindowManager::getSingleton().getWindow((utf8*)NAME_COMBOBOX_EXTENSION.c_str());
        _mLbxFiles = (Listbox*)WindowManager::getSingleton().getWindow((utf8*)NAME_LISTBOX_FILES.c_str());
 
        _mWindowPromptLoad = (FrameWindow*)WindowManager::getSingleton().getWindow((utf8*)NAME_FRAMEWINDOW_PROMPT_LOAD.c_str());
        _mStPromptLoad = (Window*)WindowManager::getSingleton().getWindow((utf8*)NAME_STATICTEXT_PROMPT_LOAD.c_str());
        _mBtnPromptLoadOk = (PushButton*)WindowManager::getSingleton().getWindow((utf8*)NAME_PUSHBUTTON_OK_PROMPT_LOAD.c_str());
 
        _mWindowPromptSave = (FrameWindow*)WindowManager::getSingleton().getWindow((utf8*)NAME_FRAMEWINDOW_PROMPT_SAVE.c_str());
        _mStPromptSave = (Window*)WindowManager::getSingleton().getWindow((utf8*)NAME_STATICTEXT_PROMPT_SAVE.c_str());
        _mBtnPromptSaveOk = (PushButton*)WindowManager::getSingleton().getWindow((utf8*)NAME_PUSHBUTTON_OK_PROMPT_SAVE.c_str());
        _mBtnPromptSaveCancel = (PushButton*)WindowManager::getSingleton().getWindow((utf8*)NAME_PUSHBUTTON_CANCEL_PROMPT_SAVE.c_str());
 
        // Add a drive combobox
        _determineDrives ();
        vector<String>::iterator pos;
        for (pos = _drives.begin(); pos < _drives.end(); pos++)
            _mCmbDrives->addItem(new ListboxTextItem (*pos));
 
        if (_mWindow)
        {
            _mWindow->hide();
            _mWindow->disable();
            _mWindow->subscribeEvent(FrameWindow::EventCloseClicked, Event::Subscriber(&CCommonFileDialog::_handleClose, this));
        }
 
        if (_mBtnBack)
        {
            _mBtnBack->setText ("<<"); // Must set it here, otherwise the xml parser gets confused
            _mBtnBack->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CCommonFileDialog::_handleBack, this));
        }
 
        if (_mBtnCancel)
            _mBtnCancel->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CCommonFileDialog::_handleCancel, this));
 
        if (_mBtnOk)
            _mBtnOk->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CCommonFileDialog::_handleOk, this));
 
        if (_mCmbExtensions)
            _mCmbExtensions->subscribeEvent(Combobox::EventListSelectionAccepted, Event::Subscriber(&CCommonFileDialog::_handleFilterSelect, this));
 
        if (_mCmbDrives)
        {
            _mCmbDrives->setText(_getCurrentDrive ());
            _mCmbDrives->subscribeEvent(Combobox::EventListSelectionAccepted, Event::Subscriber(&CCommonFileDialog::_handleDriveSelect, this));
        }
 
        if (_mLbxFiles)
        {
            _mLbxFiles->setSortingEnabled (true);
            _mLbxFiles->subscribeEvent(Listbox::EventMouseClick, Event::Subscriber(&CCommonFileDialog::_handleFileSelect, this));
            _mLbxFiles->subscribeEvent(Listbox::EventMouseDoubleClick, Event::Subscriber(&CCommonFileDialog::_handleDirectorySelect, this));
        }
 
        if (_mWindowPromptLoad)
        {
            _mWindowPromptLoad->disable();
            _mWindowPromptLoad->hide();
        }
 
        if (_mBtnPromptLoadOk)
            _mBtnPromptLoadOk->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CCommonFileDialog::_handlePromptLoadOk, this));
 
        if (_mWindowPromptSave)
        {
            _mWindowPromptSave->disable();
            _mWindowPromptSave->hide();
        }
 
        if (_mBtnPromptSaveOk)
            _mBtnPromptSaveOk->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CCommonFileDialog::_handlePromptSaveOk, this));
 
        if (_mBtnPromptSaveCancel)
            _mBtnPromptSaveCancel->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CCommonFileDialog::_handlePromptSaveCancel, this));
    }
 
    /*************************************************************************
        CCommonFileDialog::~CCommonFileDialog
        Delete the _Path objects. Use the vector, because iterating through
        the hash_map gives problems
    *************************************************************************/
    CCommonFileDialog::~CCommonFileDialog (void)
    {
        deleteAllEntriesFromHashMap (&_currentPaths);
    }
 
    /*************************************************************************
        CCommonFileDialog::_disableDialog
        Disables all interaction with the dialog controls
    *************************************************************************/
    void CCommonFileDialog::_disableDialog (void)
    {
        if (_mBtnBack)
            _mBtnBack->disable();
        if (_mBtnOk)
            _mBtnOk->disable();
        if (_mBtnCancel)
            _mBtnCancel->disable();
        if (_mCmbFile)
            _mCmbFile->disable();
        if (_mCmbDrives)
            _mCmbDrives->disable();
        if (_mCmbExtensions)
            _mCmbExtensions->disable();
        if (_mLbxFiles)
            _mLbxFiles->disable();
    }
 
    /*************************************************************************
        CCommonFileDialog::_enableDialog
        Enables all interaction with the dialog controls
    *************************************************************************/
    void CCommonFileDialog::_enableDialog (void)
    {
        if (_mBtnBack)
            _mBtnBack->enable();
        if (_mBtnOk)
            _mBtnOk->enable();
        if (_mBtnCancel)
            _mBtnCancel->enable();
        if (_mCmbFile)
            _mCmbFile->enable();
        if (_mCmbDrives)
            _mCmbDrives->enable();
        if (_mCmbExtensions)
            _mCmbExtensions->enable();
        if (_mLbxFiles)
            _mLbxFiles->enable();
    }
 
    /*************************************************************************
        CCommonFileDialog::_closeWindow
        Actually closes the window
    *************************************************************************/
    void CCommonFileDialog::_closeWindow (void)
    {
        if (_mWindow)
        {
            _mWindow->disable();
            _mWindow->hide();
        }
 
        _closePromptWindowLoad ();
        _closePromptWindowSave ();
    }
 
    /*************************************************************************
        CCommonFileDialog::_closePromptWindowLoad
    *************************************************************************/
    void CCommonFileDialog::_closePromptWindowLoad (void)
    {
        if (_mWindowPromptLoad)
        {
            _mWindowPromptLoad->disable();
            _mWindowPromptLoad->hide();
        }
        _enableDialog ();
    }
 
    /*************************************************************************
        CCommonFileDialog::_closePromptWindowSave
    *************************************************************************/
    void CCommonFileDialog::_closePromptWindowSave (void)
    {
        if (_mWindowPromptSave)
        {
            _mWindowPromptSave->disable();
            _mWindowPromptSave->hide();
        }
        _enableDialog ();
    }
 
    /*************************************************************************
        CCommonFileDialog::_openPromptWindowLoad
        Opens the prompt window in case of loading
    *************************************************************************/
    void CCommonFileDialog::_openPromptWindowLoad (void)
    {
        if (_mWindowPromptLoad)
        {
            _disableDialog();
            _mWindowPromptLoad->show();
            _mWindowPromptLoad->enable();
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::_openPromptWindowSave
        Opens the prompt window in case of saving
    *************************************************************************/
    void CCommonFileDialog::_openPromptWindowSave (void)
    {
        if (_mWindowPromptSave)
        {
            _disableDialog();
            _mWindowPromptSave->show();
            _mWindowPromptSave->enable();
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::_handleClose
        Close the window
    *************************************************************************/
    bool CCommonFileDialog::_handleClose (const CEGUI::EventArgs& e)
    {
        _prefillOutputAndClearInput ();
        _currentOutput.setAction (ACTION_WINDOW_CLOSED);
        _closeWindow ();
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_handleCancel
        Cancel Loading/saving and close the window
    *************************************************************************/
    bool CCommonFileDialog::_handleCancel (const CEGUI::EventArgs& e)
    {
        _prefillOutputAndClearInput ();
        _currentOutput.setAction (ACTION_CANCEL);
        _closeWindow ();
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_okExecute
    *************************************************************************/
    void CCommonFileDialog::_okExecute (void)
    {
        _prefillOutputAndClearInput ();
        _Path* path = _getPathOfSelectedDrive ();
        if (path)
        {
            _currentOutput.setFullQualifiedFileName (    path->szAbsolutePath + 
                                                        SEPARATOR + 
                                                        _getCurrentFile());
            _currentOutput.setAbsolutePath (path->szAbsolutePath);
            _currentOutput.setRelativePath (path->szRelativePath);
        }
 
        _currentOutput.setFileName (_getCurrentFile());
        _currentOutput.setFileExisting (_isFileExisting(_getCurrentFile()));
        _currentOutput.setAction (ACTION_OK);
 
        // Add the filename to the combobox (= history)
        _addFileNameToHistory (_getCurrentFile());
 
        _closeWindow ();
    }
 
    /*************************************************************************
        CCommonFileDialog::_handleOk
        Executed when Ok has been pressed.
        An output object is created with the current settings or in case 
        prompt has been set, the prompt window is displayed
    *************************************************************************/
    bool CCommonFileDialog::_handleOk (const CEGUI::EventArgs& e)
    {
        if (_currentInput.getDoPrompt() &&
            _currentInput.getOpenFileDialog() &&
            !_isFileExisting(_getCurrentFile()))
        {
                _openPromptWindowLoad();
                return true;
        }
 
        if (_currentInput.getDoPrompt() &&
            !_currentInput.getOpenFileDialog() &&
            _isFileExisting(_getCurrentFile()))
        {
                _openPromptWindowSave();
                return true;
        }
 
        _okExecute ();
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_handleBack
        One step back in the directory tree
    *************************************************************************/
    bool CCommonFileDialog::_handleBack (const CEGUI::EventArgs& e)
    {
        _adjustPathOfSelectedDrive (BRACKET_OPEN + ".." + BRACKET_CLOSED);
        _fillListbox ();
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_handleDriveSelect
        Executed when a drive is selected
    *************************************************************************/
    bool CCommonFileDialog::_handleDriveSelect (const CEGUI::EventArgs& e)
    {
        _fillListbox ();
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_handleFilterSelect
        Executed when a filter (extension) is selected.
    *************************************************************************/
    bool CCommonFileDialog::_handleFilterSelect (const CEGUI::EventArgs& e)
    {
        _fillListbox ();
 
        if (!_currentInput.getOpenFileDialog())
        {
            // Dialog is open for 'Save As'. Only then the extension of the file is changed after selecting an extension
            _FilterElement* filterElement = _getCurrentFilterElement();
            if (filterElement)
            {
                // Get the extension without the "*."
                String szExt = _stripFileExtension(filterElement->szExtension);
 
                // Get the filename without the extension and set it in the combobox
                String szFileName = _stripFileExtension(_getCurrentFile(), false);
                String szRes = szFileName + "." + szExt;
                _setCurrentFile (szRes);
            }
        }
 
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_handleFileSelect
        Executed when a filename (in the listbox) is selected
    *************************************************************************/
    bool CCommonFileDialog::_handleFileSelect (const CEGUI::EventArgs& e)
    {
        if (_mLbxFiles)
        {
            ListboxTextItem* item = static_cast<ListboxTextItem*>(_mLbxFiles->getFirstSelectedItem ());
            if (item)
            {
                // Return if a directory is selected
                if (item->getText ().length() > 0)
                    if (item->getText ()[0] == BRACKET_OPEN_ASCII)
                        return true;
 
                if (_mCmbFile)
                    _mCmbFile->setText(item->getText());
            }
        }
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_handleDirectorySelect
        Executed when a directory (in the listbox) is selected
    *************************************************************************/
    bool CCommonFileDialog::_handleDirectorySelect (const CEGUI::EventArgs& e)
    {
        if (_mLbxFiles)
        {
            ListboxTextItem* item = static_cast<ListboxTextItem*>(_mLbxFiles->getFirstSelectedItem ());
            if (item)
            {
                // Return if no directory is selected
                if (item->getText ().length() > 0)
                    if (item->getText ()[0] != BRACKET_OPEN_ASCII)
                        return true;
 
                // Determine new directory
                _adjustPathOfSelectedDrive (item->getText ());
                _fillListbox ();
            }
        }
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_handlePromptLoadOk
        Executed when the Ok button in the prompt load window has been pushed
    *************************************************************************/
    bool CCommonFileDialog::_handlePromptLoadOk (const CEGUI::EventArgs& e)
    {
        _closePromptWindowLoad ();
        return true;
    }
 
 
    /*************************************************************************
        CCommonFileDialog::_handlePromptSaveOk
        Executed when the Ok button in the prompt save window has been pushed
    *************************************************************************/
    bool CCommonFileDialog::_handlePromptSaveOk (const CEGUI::EventArgs& e)
    {
        _okExecute ();
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_handlePromptSaveCancel
        Executed when the Cancel button in the prompt save window has been pushed
    *************************************************************************/
    bool CCommonFileDialog::_handlePromptSaveCancel (const CEGUI::EventArgs& e)
    {
        _closePromptWindowSave ();
        return true;
    }
 
    /*************************************************************************
        CCommonFileDialog::_isFileExisting
        Determines whether a file exists in the selected path
    *************************************************************************/
    bool CCommonFileDialog::_isFileExisting (const String szFileName)
    {
        String szPathPattern = _getPathOfSelectedDrive()->szAbsolutePath;
        szPathPattern = szPathPattern + SEPARATOR + "*";
        String szName = "";
        bool found = false;
        struct _finddata_t c_file;
        long hFile;
 
        if( (hFile = _findfirst(szPathPattern.c_str(), &c_file )) != -1L)
        {
            szName = String(c_file.name);
            if ((!(c_file.attrib & _A_SUBDIR)) && strcmp(c_file.name, szFileName.c_str()) == 0)
                found = true;
 
            // Get the other ones
            while (_findnext( hFile, &c_file ) == 0 && !found)
            {
                szName = String(c_file.name);
                if ((!(c_file.attrib & _A_SUBDIR)) && strcmp(c_file.name, szFileName.c_str()) == 0)
                    found = true;
            }
        }
 
        _findclose( hFile );
        return found;
    }
 
    /*************************************************************************
        CCommonFileDialog::_prefillOutputAndClearInput
        Fills the output object with generic data and clear the input data
    *************************************************************************/
    void CCommonFileDialog::_prefillOutputAndClearInput (void)
    {
        _currentOutput.setHandle (_currentInput.getHandle());
        _currentOutput.setFullQualifiedFileName ("");
        _currentOutput.setDrive (_getCurrentDrive());
        _currentOutput.setAbsolutePath ("");
        _currentOutput.setRelativePath ("");
        _currentOutput.setFileName ("");
 
        _currentInput.setDefaultExtension ("");
        _currentInput.setFileName ("");
        _currentInput.setFilter ("");
        _currentInput.setHandle (0);
        _currentInput.setTitle ("");
    }
 
    /*************************************************************************
        CCommonFileDialog::_determineDrives
        Determine all active drives
    *************************************************************************/
    void CCommonFileDialog::_determineDrives (void)
    {
        _drives.clear();
        String szDrive = "A:";
        ulong uDriveMask = _getdrives();
        if (uDriveMask != 0)
        {
            while (uDriveMask)
            {
                if (uDriveMask & 1)
                    _drives.push_back (szDrive);
 
                ++szDrive[0];
                uDriveMask >>= 1;
            }
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::_stripBrackets
        Strip the left and right bracket of a string
    *************************************************************************/
    String CCommonFileDialog::_stripBrackets(const String szItem)
    {
        String szNewItem = "";
        if (szItem.length() > 1)
        {
            szNewItem.assign (szItem, 1, szItem.length() - 2);
        }
        return szNewItem;
    }
 
    /*************************************************************************
        CCommonFileDialog::_stripDrive
        Returns the drive of a full qualified path
    *************************************************************************/
    String CCommonFileDialog::_stripDrive(const String szItem)
    {
        String szDrive;
        if (szItem.length() > 1)
        {
            return szDrive.assign (szItem, 0, 2);
        }
        else
        {
            return "";
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::_stripFileExtension
        Strips the extension of a file and returns either:
        - The extension (returnRightPart = true; the default)
        - The filename without extension (returnRightPart = false)
    *************************************************************************/
    String CCommonFileDialog::_stripFileExtension(const String szItem, bool bReturnRightPart)
    {
        if (szItem.length() == 0)
            return "";
 
        String szExt;
        String::size_type pos = szItem.rfind (".", szItem.length());
        if (pos != string::npos)
        {
            if (bReturnRightPart)
            {
                pos++;
                return szExt.assign (szItem, pos, szItem.length() - pos);
            }
            else
            {
                return szExt.assign (szItem, 0, pos);
            }
        }
 
        return "";
    }
 
    /*************************************************************************
        CCommonFileDialog::_matchItem (String)
        Determines whether an item (directory or filename) matches the
        selected filter element.
    *************************************************************************/
    bool CCommonFileDialog::_matchItem (const String szItem)
    {
        // If the item doesn't have an extension, false is returned
        if (szItem.length() == 0)
            return false;
 
        // Return true if it is a directory
        if (szItem.length() > 0)
            if (szItem[0] == BRACKET_OPEN_ASCII)
                return true;
 
        // Return true if all files should be displayed
        _FilterElement* filterElement = _getCurrentFilterElement();
        if (filterElement)
        {
            if (filterElement->szExtension == EXTENSION_ALL)
                return true;
 
            // Do they match?
            String szExt = "*." + _stripFileExtension (szItem);
            if (szExt == filterElement->szExtension)
                return true;
        }
 
        return false;
    }
 
    /*************************************************************************
        CCommonFileDialog::_getWorkingPath
        Get the current working directory (full qualified)
    *************************************************************************/
    String CCommonFileDialog::_getWorkingPath (void)
    {
        char currentPath[_MAX_PATH];
        if ( _getcwd(currentPath, _MAX_PATH ) != NULL )
        {
            return currentPath;
        }
 
        return "";
    }
 
    /*************************************************************************
        CCommonFileDialog::_getCurrentDrive
        Get the current drive. This can be:
        - The selected drive
        - If no drive selected, the drive of the working directory is returned
    *************************************************************************/
    String CCommonFileDialog::_getCurrentDrive (void)
    {
        // Return the selected drive (if available)
        if (_mCmbDrives)
        {
            ListboxTextItem* item = static_cast <ListboxTextItem*>(_mCmbDrives->getSelectedItem());
            if (item)
            {
                return item->getText();
            }
        }
 
        // If no drive was selected, use the one from the working directory
        return _stripDrive (_getWorkingPath());
    }
 
    /*************************************************************************
        CCommonFileDialog::_getCurrentFilterElement
        Get the current filter element. This can be:
        - The selected one from the extensions combobox
        - If no one selected, the first one from the list is returned
    *************************************************************************/
    CCommonFileDialog::_FilterElement* CCommonFileDialog::_getCurrentFilterElement (void)
    {
        // Return the selected one  (if available)
        if (_mCmbExtensions)
        {
            _FilterElement* item = static_cast <_FilterElement*>(_mCmbExtensions->getSelectedItem());
            if (item)
            {
                return item;
            }
 
            // If no one was selected, use the one which is presented
            String szText = _mCmbExtensions->getText ();
            uint uNumberOfEntries = _mCmbExtensions->getItemCount();
            for (uint uPos = 0; uPos < uNumberOfEntries; uPos++)
            {
                _FilterElement* item = static_cast <_FilterElement*>(_mCmbExtensions->getListboxItemFromIndex (uPos));
                if (item)
                {
                    if (szText == item->szDescription)
                        return item;
                }
            }
 
            // If nothing displayed, use the first from the list
            if (uNumberOfEntries)
            {
                _FilterElement* item = static_cast <_FilterElement*>(_mCmbExtensions->getListboxItemFromIndex (0));
                if (item)
                    return item;
            }
        }
 
        return NULL;
    }
 
    /*************************************************************************
        CCommonFileDialog::_setCurrentFile
        Set a filename in the files combobox.
    *************************************************************************/
    void CCommonFileDialog::_setCurrentFile (const String szNewFileName)
    {
        if (_mCmbFile)
        {
            // Change the currently displayed text
            _mCmbFile->setText(szNewFileName);
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::_getCurrentFile
        Get the filename from the combobox. If no filename selected, the
        currently displayed one is returned.
    *************************************************************************/
    String CCommonFileDialog::_getCurrentFile (void)
    {
        // Return the selected file from the combobox (if available)
        if (_mCmbFile)
        {
            return _mCmbFile->getText();
        }
 
        return "";
    }
 
    /*************************************************************************
        CCommonFileDialog::_addFileNameToHistory
        Insert the filename to the filename combobox (at the beginning), which
        serves as a history of all loaded and saved files. If the combobox
        contains too many items, the last one will be removed.
        Note, that the combobox currently only contains filenames, without
        path.
    *************************************************************************/
    void CCommonFileDialog::_addFileNameToHistory (const String szFileName)
    {
        _mCmbFile->insertItem (new ListboxTextItem (szFileName.c_str()), NULL);
        uint uNumberOfItems = _mCmbFile->getItemCount ();
        if (uNumberOfItems > MAX_NUMBER_OF_HISTORY_ITEMS)
            _mCmbFile->removeItem (_mCmbFile->getListboxItemFromIndex (--uNumberOfItems));
    }
 
    /*************************************************************************
        CCommonFileDialog::_adjustPathOfSelectedDrive
        Changes the directory of the selected drive.
    *************************************************************************/
    void CCommonFileDialog::_adjustPathOfSelectedDrive (const String szDirectoryChange)
    {
        // Set absolute path
        _Path* path = _getPathOfSelectedDrive ();
        String szDir = _stripBrackets (szDirectoryChange);
        if (szDir == "..")
        {
            // Go one step back
            String::size_type pos = path->szAbsolutePath.rfind (SEPARATOR, path->szAbsolutePath.length());
            if (pos != string::npos)
            {
                path->szAbsolutePath.assign (path->szAbsolutePath, 0, pos);
            }
        }
        else
        {
            if (szDir == ".")
            {
                // Go to the root
                path->szAbsolutePath = _getCurrentDrive();
            }
            else
            {
                // Must be a directory step, update the path
                path->szAbsolutePath = path->szAbsolutePath + SEPARATOR + szDir;
            }
        }
 
        // Set relative path
        // Only in case the drive of the working path is identical to the drive of the absolute path
        String szDrive = _getCurrentDrive();
        String szCurrentPath = _getWorkingPath();
        if (szDrive == _stripDrive(szCurrentPath))
        {
            path->szRelativePath = "";
            String szBranchWorkingPath = "";
            String szBranchAbsolutePath = "";
            String szRelative = "";
            uint uLenCurrent = szCurrentPath.length();
            uint uLenAbs = path->szAbsolutePath.length();
            uint u = 0;
 
            // Determine where the the working path and the absolute path are branching
            while (szCurrentPath[u] == path->szAbsolutePath[u] && u < uLenCurrent && u < uLenAbs)
                u++;
 
            // Go back until the first separator is detected
            if (u < uLenCurrent)
                while (szCurrentPath[u] != SEPARATOR[0] && u >= 0)
                    u--;
 
            // Get the branch of the working path
            szBranchWorkingPath.assign (szCurrentPath, u, szCurrentPath.length() - u);
 
            // Get the branch of the absolute path (increase counter by 1 to prevent duplicate separators)
            if (u < uLenAbs)
                u++;
            szBranchAbsolutePath.assign (path->szAbsolutePath, u, path->szAbsolutePath.length() - u);
 
            // The amount of separators determines the steps back.
            uint uLenBranchWorkingPath = szBranchWorkingPath.length();
            for (u = 0; u < uLenBranchWorkingPath; u++)
            {
                if (szBranchWorkingPath[u] == SEPARATOR[0])
                    szRelative = szRelative + ".." + SEPARATOR;
            }
 
            // Concatenate the 2 parts
            path->szRelativePath = szRelative + szBranchAbsolutePath;
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::_getPathOfSelectedDrive
        Returns the path of the selected drive.
    *************************************************************************/
    CCommonFileDialog::_Path* CCommonFileDialog::_getPathOfSelectedDrive (void)
    {
        String szDrive = _getCurrentDrive();
        _Path* path = getEntryFromHashMap (string(szDrive.c_str()), &_currentPaths);
        if (!path)
        {
            // No path found, add a new one
            path = new _Path ("");
            String szCurrentPath = _getWorkingPath();
            if (szDrive == _stripDrive(szCurrentPath))
                path->szAbsolutePath = szCurrentPath;
            else
                path->szAbsolutePath = szDrive;
 
            addEntryToHashMap (string(szDrive.c_str()), path, &_currentPaths);
        }
 
        return path;
    }
 
    /*************************************************************************
        CCommonFileDialog::_fillListbox
        Fill the listbox with files
    *************************************************************************/
    void CCommonFileDialog::_fillListbox  (void)
    {
        String szName;
        String szPathPattern;
 
        if (_mLbxFiles)
        {
            szPathPattern = _getPathOfSelectedDrive()->szAbsolutePath;
            szPathPattern = szPathPattern + SEPARATOR + "*";
            _mLbxFiles->resetList();
            struct _finddata_t c_file;
            long hFile;
 
            if( (hFile = _findfirst( szPathPattern.c_str(), &c_file )) == -1L)
            {
                // Nothing in directory
            }
            else
            {
                szName = String(c_file.name);
                if (c_file.attrib & _A_SUBDIR)
                    szName = BRACKET_OPEN + szName + BRACKET_CLOSED;
 
                // Add it to the listbox if it passes the filter
                if (_matchItem(szName))
                    _mLbxFiles->addItem(new ListboxTextItem((utf8*)szName.c_str()));
            }
 
            // Get the other ones
            while( _findnext( hFile, &c_file ) == 0 )
            {
                szName = String(c_file.name);
                if (c_file.attrib & _A_SUBDIR)
                    szName = BRACKET_OPEN + szName + BRACKET_CLOSED;
 
                // Add it to the listbox if it passes the filter
                if (_matchItem(szName))
                    _mLbxFiles->addItem(new ListboxTextItem((utf8*)szName.c_str()));
            }
 
            _findclose( hFile );
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::openDialog
        Call the file dialog
    *************************************************************************/
    void CCommonFileDialog::openDialog (CommonFileDialogInput& input)
    {
        if (_mWindow)
        {
            // Store the input
            bool bDefaultExtensionSet = false;
            _currentInput = input;
 
            // Parse the filter and fill the filter elements (extensions) in the combobox
            _filterParser.parseFilter (input.getFilter());
            vector<_Filter*> filter = _filterParser.getParsedFilter();
            if (_mCmbExtensions && filter.size() > 0)
            {
                // Loop through the filter and add the elements to the combobox
                _mCmbExtensions->resetList ();
                vector<CCommonFileDialog::_Filter*>::iterator pos;
                for (pos = filter.begin(); pos < filter.end(); pos++)
                {
                    if (*pos)
                    {
                        _mCmbExtensions->addItem(new _FilterElement ((*pos)->szDescription, (*pos)->szExtension));
 
                        // Was the default extension part of the element. If yes, then show it
                        if (input.getDefaultExtension() == (*pos)->szExtension)
                        {
                            _mCmbExtensions->setText((*pos)->szDescription);
                            bDefaultExtensionSet = true;
                        }
                    }
                }
 
                // If the default extension in the input object isn't available; set the first in the list
                if (!bDefaultExtensionSet)
                {
                    _FilterElement* filterElement = _getCurrentFilterElement ();
                    if (filterElement)
                        _mCmbExtensions->setText(filterElement->szDescription);
                }
 
                // Fill the list
                _fillListbox ();
            }
 
            // Set the title
            if (input.getTitle() != "")
            {
                _mWindow->setText((CEGUI::utf8*)input.getTitle().c_str());
                if (_currentInput.getOpenFileDialog())
                    _mWindowPromptLoad->setText((CEGUI::utf8*)input.getTitle().c_str());
                else
                    _mWindowPromptSave->setText((CEGUI::utf8*)input.getTitle().c_str());
            }
            else
            {
                if (_currentInput.getOpenFileDialog())
                {
                    _mWindowPromptLoad->setText((CEGUI::utf8*)DEFAULT_TEXT_DIALOG_LOAD.c_str());
                    _mWindow->setText((CEGUI::utf8*)DEFAULT_TEXT_DIALOG_LOAD.c_str());
                }
                else
                {
                    _mWindowPromptSave->setText((CEGUI::utf8*)DEFAULT_TEXT_DIALOG_SAVE.c_str());
                    _mWindow->setText((CEGUI::utf8*)DEFAULT_TEXT_DIALOG_SAVE.c_str());
                }
            }
 
            // Set the text in the prompt window
            if (input.getDoPrompt())
            {
                if (input.getPromptText() != "")
                {
                    if (_currentInput.getOpenFileDialog())
                        _mStPromptLoad->setText((CEGUI::utf8*)input.getPromptText().c_str());
                    else
                        _mStPromptSave->setText((CEGUI::utf8*)input.getPromptText().c_str());
                }
                else
                {
                    if (_currentInput.getOpenFileDialog())
                        _mStPromptLoad->setText((CEGUI::utf8*)DEFAULT_TEXT_PROMPT_LOAD.c_str());
                    else
                        _mStPromptSave->setText((CEGUI::utf8*)DEFAULT_TEXT_PROMPT_SAVE.c_str());
                }
            }
 
            // Display the (default) filename
            _mCmbFile->setText((CEGUI::utf8*)input.getFileName().c_str());
 
            // Show the dialog
            _mWindow->show();
            _mWindow->enable();
            _enableDialog();
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::getResult
        Returns the output object after the Ok button has been pressed
    *************************************************************************/
    CommonFileDialogOutput CCommonFileDialog::getResult (void)
    {
        return _currentOutput;
    }
 
    /*************************************************************************
        CCommonFileDialog::_FilterParser::__deleteFilter
        Delete the vector with filter elements
    *************************************************************************/
    void CCommonFileDialog::_FilterParser::__deleteFilter()
    {
        vector<_Filter*>::iterator pos;
        for (pos = __filter.begin(); pos < __filter.end(); pos++)
        {
            if (*pos)
                delete *pos;
        }
 
        __filter.clear();
    }
 
    /*************************************************************************
        CCommonFileDialog::_FilterParser::parseFilter
        Parse the filter string. The string is divided and the parsed data
        is added to _Filter objects
    *************************************************************************/
    void CCommonFileDialog::_FilterParser::parseFilter(const String szFilter)
    {
        __deleteFilter ();
        _Filter* filter;
        String szPart;
        bool push = false;
        String::size_type idxStart = 0;
        String::size_type idxEnd = szFilter.find ("|", idxStart);
        while (idxEnd != String::npos)
        {
            szPart.assign (szFilter, idxStart, idxEnd - idxStart);
            if (!push)
            {
                filter = new _Filter();
                if (filter)
                    filter->szDescription = szPart;
                push = true;
            }
            else
            {
                if (filter)
                {
                    filter->szExtension = szPart;
                    __filter.push_back (filter);
                    push = false;
                }
            }
 
            idxStart = ++idxEnd;
            idxEnd = szFilter.find ("|", idxStart);
        }
    }
 
    /*************************************************************************
        CCommonFileDialog::_FilterParser::getParsedFilter
        Return the parsed filter as a vector with Filter pointers
    *************************************************************************/
    vector<CCommonFileDialog::_Filter*> CCommonFileDialog::_FilterParser::getParsedFilter(void)
    {
        return __filter;
    }
}