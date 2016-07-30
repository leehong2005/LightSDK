/*!
* @file DriversManager.cpp
* 
* @brief This file defines class DriversManager to manager system disk devices..
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/26
*/

#include "DriversManager.h"

USING_NAMESPACE_COMMON

DriversManager::DriversManager()
{
}

//////////////////////////////////////////////////////////////////////////

DriversManager::~DriversManager()
{
    ClearDriverInfos();
}

//////////////////////////////////////////////////////////////////////////

void DriversManager::BeginDetect()
{
    WCHAR szVolumeName[MAX_PATH] = { 0 };
    HANDLE hFindVolume = FindFirstVolume(szVolumeName, MAX_PATH);
    if (ISVALIDHANDLE(hFindVolume))
    {
        ClearDriverInfos();

        do
        {
            DRIVERINFO *pDriverInfo = new DRIVERINFO();
            ZeroMemory(pDriverInfo, sizeof(DRIVERINFO));
            _tcscpy_s(pDriverInfo->szVolumeName, MAX_PATH + 1, szVolumeName);

            GetDriverInfoFromVolumeName(pDriverInfo);
            m_vctDriverInfos.push_back(pDriverInfo);
            if ( (pDriverInfo->isRemovable || pDriverInfo->isUsbDisk) && !pDriverInfo->isFloppy )
            {
                m_vctRemovableInfos.push_back(pDriverInfo);
            }

        } while (FindNextVolume(hFindVolume, szVolumeName, MAX_PATH));

        FindVolumeClose(hFindVolume);
    }
}

//////////////////////////////////////////////////////////////////////////

const vector<DRIVERINFO*>* DriversManager::GetRemovableDrivers()
{
    return &m_vctRemovableInfos;
}

//////////////////////////////////////////////////////////////////////////

void DriversManager::GetDriverInfoFromVolumeName(IN OUT DRIVERINFO *pDriverInfo)
{
    if (NULL == pDriverInfo)
    {
        return;
    }

    GetVolumePathNamesForVolumeName(pDriverInfo->szVolumeName,
                                    pDriverInfo->szRootPath,
                                    MAX_PATH,
                                    NULL);

    pDriverInfo->isFloppy     = IsFloppy(pDriverInfo->szRootPath);
    pDriverInfo->dwDeviceType = GetDriveType(pDriverInfo->szVolumeName);
    pDriverInfo->isRemovable  = IsRemovableDriver(pDriverInfo->szRootPath);
    pDriverInfo->isUsbDisk    = IsUsbDisk(pDriverInfo->szRootPath);

    if ( (DRIVE_FIXED == pDriverInfo->dwDeviceType) || 
         (DRIVE_REMOVABLE == pDriverInfo->dwDeviceType) && 
         !pDriverInfo->isFloppy)
    {
        GetVolumeInformation(pDriverInfo->szRootPath,
                             pDriverInfo->szTitleName,
                             MAX_PATH + 1,
                             &(pDriverInfo->dwSerialNum),
                             &(pDriverInfo->dwMaxComLength),
                             &(pDriverInfo->dwFileSystemFlag),
                             pDriverInfo->szFileSystemName,
                             MAX_PATH + 1);
    }
}

//////////////////////////////////////////////////////////////////////////

void DriversManager::GetDriverInfoFromRootPath(IN OUT DRIVERINFO *pDriverInfo)
{
    if (NULL == pDriverInfo)
    {
        return;
    }

    GetVolumeNameForVolumeMountPoint(pDriverInfo->szRootPath,
                                     pDriverInfo->szVolumeName,
                                     MAX_PATH + 1);

    pDriverInfo->isFloppy     = IsFloppy(pDriverInfo->szRootPath);
    pDriverInfo->dwDeviceType = GetDriveType(pDriverInfo->szVolumeName);
    pDriverInfo->isRemovable  = IsRemovableDriver(pDriverInfo->szVolumeName);
    pDriverInfo->isUsbDisk    = IsUsbDisk(pDriverInfo->szRootPath);

    if ( (DRIVE_FIXED == pDriverInfo->dwDeviceType) || 
         (DRIVE_REMOVABLE == pDriverInfo->dwDeviceType) && 
         !pDriverInfo->isFloppy)
    {
        GetVolumeInformation(pDriverInfo->szRootPath,
                             pDriverInfo->szTitleName,
                             MAX_PATH + 1,
                             &(pDriverInfo->dwSerialNum),
                             &(pDriverInfo->dwMaxComLength),
                             &(pDriverInfo->dwFileSystemFlag),
                             pDriverInfo->szFileSystemName,
                             MAX_PATH + 1);
    }
}

//////////////////////////////////////////////////////////////////////////

void DriversManager::GetDriverInfoFromUnitMask(ULONG uMask, IN OUT DRIVERINFO *pDriverInfo)
{
    if (NULL == pDriverInfo)
    {
        return;
    }

    TCHAR driverLetter = FirstDriverFromMask(uMask);
    TCHAR szDriverRoot[4] = TEXT(" :\\");
    *szDriverRoot = driverLetter;

    _tcscpy_s(pDriverInfo->szRootPath, MAX_PATH, szDriverRoot);
    GetDriverInfoFromRootPath(pDriverInfo);
}

//////////////////////////////////////////////////////////////////////////

TCHAR DriversManager::FirstDriverFromMask(ULONG unitmask)
{
    TCHAR i = 0;

    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
        {
            break;
        }
        unitmask = unitmask >> 1;
    }

    return (i + TEXT('A'));
}

//////////////////////////////////////////////////////////////////////////

void DriversManager::ClearDriverInfos()
{
    int nSize = (int)m_vctDriverInfos.size();
    for (int i = 0; i < nSize; ++i)
    {
        SAFE_DELETE(m_vctDriverInfos[i]);
    }

    m_vctDriverInfos.clear();
}

//////////////////////////////////////////////////////////////////////////

BOOL DriversManager::IsRemovableDriver(IN LPCTSTR lpVolumeName)
{
    UINT uType = GetDriveType(lpVolumeName);
    return (DRIVE_REMOVABLE == uType);
}

//////////////////////////////////////////////////////////////////////////

BOOL DriversManager::IsUsbDisk(IN LPCTSTR lpRootPath)
{
    if ( (NULL == lpRootPath) || (0 == _tcslen(lpRootPath)) )
    {
        return FALSE;
    }

    BOOL isUsbDisk   = FALSE;
    UINT uType = GetDriveType(lpRootPath);
    if (DRIVE_FIXED == uType)
    {
        TCHAR szFileName[MAX_PATH] = { 0 };
        _stprintf_s(szFileName, L"\\\\?\\%c:", *lpRootPath);
        HANDLE hDevice = CreateFile(szFileName,
                                    GENERIC_READ,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    NULL, OPEN_EXISTING, NULL, NULL);

        if (ISVALIDHANDLE(hDevice))
        {
            PSTORAGE_DEVICE_DESCRIPTOR pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];
            pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;
            if (GetDisksProperty(hDevice, pDevDesc))
            {
                if (BusTypeUsb == pDevDesc->BusType)
                {
                    isUsbDisk = TRUE;
                }
            }

            SAFE_CLOSE_HANDLE(hDevice);
        }
    }

    return isUsbDisk;
}

//////////////////////////////////////////////////////////////////////////

BOOL DriversManager::IsFloppy(IN LPCTSTR lpDriverName)
{
    if (NULL == lpDriverName)
    {
        return FALSE;
    }

    TCHAR szVolumeName[MAX_PATH] = { 0 };
    if (GetVolumeNameForVolumeMountPoint(lpDriverName, szVolumeName, MAX_PATH))
    {
        if (IsRemovableDriver(szVolumeName))
        {
            TCHAR szName[MAX_PATH] = { 0 };
            TCHAR szDevice[3] = _T(" :");
            *szDevice = *lpDriverName;

            DWORD dwSize = QueryDosDevice(szDevice, szName, MAX_PATH);
            if (dwSize > 0)
            {
                LPCTSTR lpFloppy = _T("\\Device\\Floppy");
                return (0 == _tcsnicmp(szName, lpFloppy, _tcslen(lpFloppy)));
            }
        }
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

BOOL DriversManager::GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
    STORAGE_PROPERTY_QUERY    Query;    // input param for query
    DWORD dwOutBytes;                   // IOCTL output length
    BOOL bResult;                       // IOCTL return val

    // specify the query type
    Query.PropertyId = StorageDeviceProperty;
    Query.QueryType  = PropertyStandardQuery;

    // Query using IOCTL_STORAGE_QUERY_PROPERTY 
    bResult = ::DeviceIoControl(hDevice,                                // Device handle
                                IOCTL_STORAGE_QUERY_PROPERTY,           // Info of device property
                                &Query, sizeof(STORAGE_PROPERTY_QUERY), // Input data buffer
                                pDevDesc, pDevDesc->Size,               // Output data buffer
                                &dwOutBytes,                            // Out's length
                                (LPOVERLAPPED)NULL);

    return bResult;
}