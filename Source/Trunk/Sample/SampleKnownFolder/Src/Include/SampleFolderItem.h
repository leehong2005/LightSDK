#pragma once

class SampleFolderItem
{
public:

    SampleFolderItem();
    ~SampleFolderItem();

    void SetClipRect(LPRECT lprcClip);
    void Initialize(HBITMAP hBitmap, HICON hIcon, LPCTSTR lpDesc, LPRECT lpRect);
    void OnPaint(HDC hDC, BOOL drawBitmap = FALSE, int dy = 0);

public:

    RECT    m_rcPaint;
    HICON   m_hIcon;
    HBITMAP m_hItemIcon;
    RECT    m_rcClip;
    TCHAR   m_szDesc[MAX_PATH];
};
