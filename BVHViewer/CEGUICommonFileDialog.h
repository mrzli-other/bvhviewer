#ifndef _CEGUI_COMMON_FILE_DIALOG_H_
#define _CEGUI_COMMON_FILE_DIALOG_H_
 
#include "CEGUISingletonX.h"
#include "CEGUIHashMapTemplates.h"
#include <CEGUIBase.h>
#include <CEGUIWindow.h>
#include <CEGUIWindowManager.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIListboxTextItem.h>
#include <elements/CEGUIListbox.h>
 
#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4251)
#endif
 
using namespace std;
 
namespace CEGUI
{
    const String NAME_LAYOUT_COMMON_FILE_DIALOG                = "CommonFileDialog.layout";
    const String NAME_FRAMEWINDOW_COMMON_FILE_DIALOG        = "CommonFileDialog";
    const String NAME_PUSHBUTTON_BACK                        = "CommonFileDialog/BackButton";
    const String NAME_PUSHBUTTON_OK                            = "CommonFileDialog/OkButton";
    const String NAME_PUSHBUTTON_CANCEL                        = "CommonFileDialog/CancelButton";
    const String NAME_COMBOBOX_FILE                            = "CommonFileDialog/FileCombobox";
    const String NAME_COMBOBOX_EXTENSION                    = "CommonFileDialog/ExtensionCombobox";
    const String NAME_COMBOBOX_DRIVE                        = "CommonFileDialog/DriveCombobox";
    const String NAME_LISTBOX_FILES                            = "CommonFileDialog/Listbox";
    const String NAME_FRAMEWINDOW_PROMPT_LOAD                = "CommonFileDialog/PromptLoad";
    const String NAME_STATICTEXT_PROMPT_LOAD                = "CommonFileDialog/PromptLoad/Text";
    const String NAME_PUSHBUTTON_OK_PROMPT_LOAD                = "CommonFileDialog/PromptLoad/OkButton";
    const String NAME_FRAMEWINDOW_PROMPT_SAVE                = "CommonFileDialog/PromptSave";
    const String NAME_STATICTEXT_PROMPT_SAVE                = "CommonFileDialog/PromptSave/Text";
    const String NAME_PUSHBUTTON_OK_PROMPT_SAVE                = "CommonFileDialog/PromptSave/OkButton";
    const String NAME_PUSHBUTTON_CANCEL_PROMPT_SAVE            = "CommonFileDialog/PromptSave/CancelButton";
 
    const String DEFAULT_TEXT_DIALOG_LOAD                    = "Load";
    const String DEFAULT_TEXT_DIALOG_SAVE                    = "Save As";
    const String DEFAULT_TEXT_PROMPT_LOAD                    = "Cannot find the requested file.";
    const String DEFAULT_TEXT_PROMPT_SAVE                    = "The file already exist. Replace it?";
 
    const String EXTENSION_ALL                                = "*.*";
    const String EXTENSION_PRESENTATION_DEFAULT                = "All files (*.*)|*.*|";
    const String BRACKET_OPEN                                = "<";
    const String BRACKET_CLOSED                                = ">";
    const ulong BRACKET_OPEN_ASCII                            = 0x03C;
    const String SEPARATOR                                    = "\\";
    const uint MAX_NUMBER_OF_HISTORY_ITEMS                    = 12;
 
    const enum CommonFileDialogAction
    {
        ACTION_OK,
        ACTION_CANCEL,
        ACTION_WINDOW_CLOSED
    };
 
    /*************************************************************************
        Class: CommonFileDialogInput
    *************************************************************************/
    class CommonFileDialogInput
    {
        private:
            uint _uHandle;                                        // Handle, that is supplied by the calling function. This
                                                                // handle serves as an identifier that determines the 'caller'.
                                                                // It will be added to the output object 'CommonFileDialogOutput'
                                                                // after the Ok button in the CommonFileDialog has been pressen,
                                                                // so the triggered function (registered by means of ´subscribe´)
                                                                // is able to identify who has originally called the dialog.
                                                                // Default: 0 (no handle)
 
            String _szFilter;                                    // Filter for allowed extensions
                                                                // Default: "All files|*.*|"
                                                                // The filter is build according to the windows common file dialog filter string.
                                                                // An example: The filter
                                                                //
                                                                //    "HTML Files (*.htm)|*.htm|Active Server Pages (*.asp)|*.asp|Perl Script (*.pl)|*.pl|All files|*.*|"
                                                                //    (don't forget the last | or otherwise the last element will not be parsed)
                                                                //
                                                                // produces a list that looks like this in the combobox:
                                                                //
                                                                //    HTML Files (*.htm)
                                                                //    Active Server Pages (*.asp)
                                                                //    Perl Script (*.pl)
                                                                //    All files
 
            String _szDefaultExtension;                            // Default Extension
                                                                // Default: *.*
 
            String _szFileName;                                    // Filename
                                                                // Default: "" (no filename)
 
            String _szTitle;                                    // Title of the window.
                                                                // Default title is "Load" in case _bOpenFileDialog is TRUE
                                                                // Default title is "Save as" in case _bOpenFileDialog is FALSE
 
            bool _bOpenFileDialog;                                // TRUE for FileLoad, FALSE for FileSaveAs
                                                                // Default: TRUE
 
            bool _bDoPrompt;                                    // TRUE for prompting (are you sure?), FALSE for non-prompting
                                                                // Default: TRUE
            String _szPromptText;                                // Define text in the prompt window
                                                                // Default for Load: "Cannot find the requested file."
                                                                // Default for Save: "The file already exist. Replace it?"
 
        protected:
        public:
            CommonFileDialogInput (    const uint uHandle = 0,
                                    const String szFileName = "",
                                    const String szFilter = EXTENSION_PRESENTATION_DEFAULT,
                                    const String szDefaultExtension = EXTENSION_ALL,
                                    const String szTitle = "",
                                    bool bOpenFileDialog = true,
                                    bool bDoPrompt = true,
                                    const String szPromptText = "") : 
                                    _uHandle (uHandle),
                                    _szFileName (szFileName),
                                    _szFilter (szFilter),
                                    _szDefaultExtension (szDefaultExtension),
                                    _szTitle (szTitle),
                                    _bOpenFileDialog (bOpenFileDialog),
                                    _bDoPrompt (bDoPrompt),
                                    _szPromptText (szPromptText){};
 
            virtual ~CommonFileDialogInput(void){};
            uint getHandle (void);
            String getFileName (void);
            String getFilter (void);
            String getDefaultExtension (void);
            String getTitle (void);
            bool getOpenFileDialog (void);
            bool getDoPrompt (void);
            String getPromptText (void);
 
            void setHandle (const uint);
            void setFileName (const String);
            void setFilter (const String);
            void setDefaultExtension (const String);
            void setTitle (const String);
            void setOpenFileDialog (const bool);
            void setDoPrompt (const bool);
            void setPromptText (const String);
 
    };
 
    /*************************************************************************
        Class: CommonFileDialogOutput
    *************************************************************************/
    class CommonFileDialogOutput
    {
        private:
            uint _uHandle;
            String _szFullQualifiedFileName;                        // File name including drive and (absolute) path.
            String _szDrive;                                        // The selected drive.
            String _szAbsolutePath;                                    // The absolute path, with drive and without filename.
            String _szRelativePath;                                    // The relative path (only set if the working directory
                                                                    // is in the selected drive).
            String _szFileName;                                        // File name.
            CommonFileDialogAction _action;                            // Determines how the dialog was ended.
            bool _bFileExisting;                                    // TRUE if file exists
                                                                    // FALSE if file doesn't exist
 
        protected:
        public:
            CommonFileDialogOutput (const uint uHandle = 0,
                                    const String szFullQualifiedFileName = "",
                                    const String szDrive = "",
                                    const String szAbsolutePath = "",
                                    const String szRelativePath = "",
                                    const String szFileName = "",
                                    const bool bFileExisting = false) : 
                                    _uHandle (uHandle),
                                    _szFullQualifiedFileName (szFullQualifiedFileName),
                                    _szDrive (szDrive),
                                    _szAbsolutePath (szAbsolutePath),
                                    _szRelativePath (szRelativePath),
                                    _szFileName (szFileName),
                                    _bFileExisting(bFileExisting){};
 
            virtual ~CommonFileDialogOutput(void){};
            uint getHandle (void);
            String getFullQualifiedFileName (void);
            String getDrive (void);
            String getAbsolutePath (void);
            String getRelativePath (void);
            String getFileName (void);
            CommonFileDialogAction getAction (void);
            bool isFileExisting (void);
            void setHandle (const uint);
            void setFullQualifiedFileName (const String);
            void setDrive (const String);
            void setAbsolutePath (const String);
            void setRelativePath (const String);
            void setFileName (const String);
            void setAction (CommonFileDialogAction);
            void setFileExisting (const bool);
    };
 
    /*************************************************************************
        Class: CCommonFileDialog
    *************************************************************************/
    class CCommonFileDialog
    {
        private:
            // ------------------ Inner class that represents a filter element for extension combobox ------------------
            class _FilterElement : public ListboxTextItem
            {
                private:
                public:
                    _FilterElement(    const String& szDescription,
                                    const String& szExtension) :    szDescription(szDescription),
                                                                    szExtension(szExtension),
                                                                    ListboxTextItem(szDescription) {};
                    String szDescription;                            // Description presented in the combobox
                    String szExtension;                                // File extension
            };
 
            // ------------------ Inner class that represents drive, path and filename ------------------
            // made a small change to the original posting here because of an error 
            // (exchanged szRelativePath with szAbsolutePath in the constructor --- Cheers, Ben811 )
 
            class _Path : public ListboxTextItem
            {
                private:
                protected:
                public:
                    _Path(const String& szAbsolutePath) :
                        szAbsolutePath(szAbsolutePath),
                        szRelativePath(""),
                        ListboxTextItem(szAbsolutePath) {};
                    virtual ~_Path(void){};
                    String szAbsolutePath;
                    String szRelativePath;
            };
 
            // ------------------ Inner class representing a filter element in a more appropriate way ------------------
            class _Filter
            {
                private:
                protected:
                public:
                    _Filter(void) {};
                    virtual ~_Filter(void){};
                    String szDescription;                            // Description presented in the combobox
                    String szExtension;                                // File extension
            };
 
            // ------------------ Inner class that parses the filter ------------------
            class _FilterParser
            {
                private:
                    vector<_Filter*>__filter;                        // Parsed filter
                    void __deleteFilter();
                protected:
                public:
                    _FilterParser(void) {};
                    virtual ~_FilterParser(void){__deleteFilter();};
                    void parseFilter(const String);
                    vector<_Filter*> getParsedFilter(void);
            };
            _FilterParser _filterParser;                            // Object that contains the parsed filter
 
            // Open/close/enable/disable windows and widgets
            void _disableDialog (void);
            void _enableDialog (void);
            void _closeWindow (void);
            void _closePromptWindowLoad (void);
            void _closePromptWindowSave (void);
            void _openPromptWindowLoad (void);
            void _openPromptWindowSave (void);
 
            // Handlers
            bool _handleClose (const CEGUI::EventArgs&);
            bool _handleCancel (const CEGUI::EventArgs&);
            void _okExecute (void);
            bool _handleOk (const CEGUI::EventArgs&);
            bool _handleBack (const CEGUI::EventArgs&);
            bool _handleDriveSelect (const CEGUI::EventArgs&);
            bool _handleFilterSelect (const CEGUI::EventArgs&);
            bool _handleFileSelect (const CEGUI::EventArgs&);
            bool _handleDirectorySelect (const CEGUI::EventArgs&);
            bool _handlePromptLoadOk (const CEGUI::EventArgs&);
            bool _handlePromptSaveOk (const CEGUI::EventArgs&);
            bool _handlePromptSaveCancel (const CEGUI::EventArgs&);
 
            // Other private functions
            bool _isFileExisting (const String);
            void _prefillOutputAndClearInput (void);
            void _determineDrives (void);
            String _getWorkingPath (void);
            String _getCurrentDrive (void);
            _FilterElement* _getCurrentFilterElement (void);
            void _setCurrentFile (const String);
            String _getCurrentFile (void);
            void _addFileNameToHistory (const String);
            void _adjustPathOfSelectedDrive (const String);
            _Path* _getPathOfSelectedDrive (void);
            String _stripBrackets(const String);
            String _stripDrive(const String);
            String _stripFileExtension(    const String, bool returnRightPart = true);
            bool _matchItem (const String);
            void _fillListbox (void);
 
            // Widgets
            Window* _mWindow;                                        // Main dialog window
            PushButton* _mBtnBack;                                    // Back button
            PushButton* _mBtnOk;                                    // Ok button
            PushButton* _mBtnCancel;                                // Cancel button
            Combobox* _mCmbFile;                                    // Combobox, containing the file to be loaded or
                                                                    // saved, including the history of the last ´x´
                                                                    // loaded/saved filenames
            Combobox* _mCmbDrives;                                    // Combobox, containing all active drives
            Combobox* _mCmbExtensions;                                // Combobox, that contains the filter elements (extensions)
            Listbox* _mLbxFiles;                                    // Listbox, that represents the files and (sub)directories
                                                                    // of the current path
            FrameWindow* _mWindowPromptLoad;                        // Prompt window, displayed in case file to load doesn't exists
            Window* _mStPromptLoad;                                // Message in the prompt window
            PushButton* _mBtnPromptLoadOk;                            // Ok button
            FrameWindow* _mWindowPromptSave;                        // Prompt window, displayed in case file to save already exists
            Window* _mStPromptSave;                                // Message in the prompt window
            PushButton* _mBtnPromptSaveOk;                            // Ok button
            PushButton* _mBtnPromptSaveCancel;                        // Cancel button
 
            // Other private attributes
            vector<String> _drives;                                    // Vector containing all drives
            CommonFileDialogInput _currentInput;                    // The input data, set after each call (function 'openDialog') is stored
            CommonFileDialogOutput _currentOutput;                    // The output data
            hash_map<string, _Path*> _currentPaths;                    // Map that contains the last selected dir's for each particular drive
 
        protected:
        public:
            CCommonFileDialog(void);
            virtual ~CCommonFileDialog(void);
            Window* getWindow() {return _mWindow;};
            void openDialog (CommonFileDialogInput&);
            CommonFileDialogOutput getResult (void);
    };
    typedef CEGUISingletonX<CCommonFileDialog> CommonFileDialog;
};
#endif