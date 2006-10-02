// PatcherGUIDlg.h : header file
//

#pragma once


// CPatcherGUIDlg dialog
class CPatcherGUIDlg : public CDialog                                      ,
                       private GUCEF::PATCHER::CPatchSetParserEventHandler
{
// Construction
public:
	CPatcherGUIDlg(CWnd* pParent = NULL);	// standard constructor

    CListBox m_listBox;
    
// Dialog Data
	enum { IDD = IDD_PATCHERGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
private:
    
    virtual void OnPatchSetStart( const GUCEF::CORE::CString& patchSetName );
    
    virtual void OnEnterLocalDir( const GUCEF::CORE::CString& localPath );
    
    virtual void OnLocalFileOK( const GUCEF::CORE::CString& localPath ,
                                const GUCEF::CORE::CString& localFile );

    virtual void OnLocalFileNotFound( const GUCEF::CORE::CString& localPath ,
                                      const GUCEF::CORE::CString& localFile );

    virtual void OnLocalFileDifference( const GUCEF::CORE::CString& localPath ,
                                        const GUCEF::CORE::CString& localFile );

    virtual void OnNewSourceRequired( const TSourceInfo& sourceInfo );
    
    virtual void OnLeaveLocalDir( const GUCEF::CORE::CString& localPath );
    
    virtual void OnPatchSetEnd( const GUCEF::CORE::CString& patchSetName );	
};
