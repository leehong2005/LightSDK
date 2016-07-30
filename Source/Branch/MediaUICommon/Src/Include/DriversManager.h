/*!
* @file DriversManager.h
* 
* @brief This file defines class DriversManager to manager system disk devices..
* 
* Copyright (C) 2010, Toshiba Corporation.
* 
* @author Li Hong
* @date 2011/02/26
*/

#ifdef __cplusplus
#ifndef _DRIVERSMANAGER_H_
#define _DRIVERSMANAGER_H_

#include "Common.h"
#include "CommonMacro.h"
#include <WinIoCtl.h>

BEGIN_NAMESPACE_COMMON

/*!
* @brief The driver information.
*/
typedef struct _DRIVERINFO
{
    TCHAR   szRootPath[MAX_PATH];               // The root path of a driver, for example, "H:\\".
    TCHAR   szVolumeName[MAX_PATH + 1];         // The volume name, "\\?\Volume{GUID}\"
    TCHAR   szTitleName[MAX_PATH + 1];          // Title name, may be empty.
    TCHAR   szFileSystemName[MAX_PATH + 1];     // File system text, such as "FAT32".
    BOOL    isFloppy;                           // Is floppy or not.
    BOOL    isRemovable;                        // Is Removable or not.
    BOOL    isUsbDisk;                          // Is Usb fixed disk.
    DWORD   dwDeviceType;                       // Device type. such as DRIVE_UNKNOWN, DRIVE_FIXED.
    DWORD   dwSerialNum;                        // Serial number.
    DWORD   dwMaxComLength;                     // Maximum component length.
    DWORD   dwFileSystemFlag;                   // File system flag.

} DRIVERINFO, *LPDRIVERINFO;

/*!
* @brief The DriversManager class is used to get infos of system disk devices.
*/
class CLASS_DECLSPEC DriversManager
{
public:

    /*!
    * @brief The constructor functions.
    */
    DriversManager();

    /*!
    * @brief The destructor functions.
    */
    ~DriversManager();

    /*!
    * @brief Call this method to detect all disk on this computer.
    */
    void BeginDetect();

    /*!
    * @brief Call this method to get all removable driver names, this function should be 
    *        called after calling to BeginDetect function.
    *
    * @return vctDriverNames     [I/ ] The path of driver name is with a trailing backslash, "\", such as "D:\".
    */
    const vector<DRIVERINFO*>* GetRemovableDrivers();

    /*!
    * @brief Get driver's information.
    *
    * @param pDriverInfo        [I/O] The input output driver information.
    */
    void GetDriverInfoFromVolumeName(IN OUT DRIVERINFO *pDriverInfo);

    /*!
    * @brief Get driver's information.
    *
    * @param pDriverInfo        [I/O] The input output driver information.
    */
    void GetDriverInfoFromRootPath(IN OUT DRIVERINFO *pDriverInfo);

    /*!
    * @brief Get driver's information.
    *
    * @param uMask              [I/O] The device mask.
    * @param pDriverInfo        [I/O] The input output driver information.
    */
    void GetDriverInfoFromUnitMask(ULONG uMask, IN OUT DRIVERINFO *pDriverInfo);

private:

    /*!
    * @brief Get driver from mask.
    *
    * @param unitmask           [I/ ] The driver mask.
    *
    * @return The driver letter. such as A, C.
    */
    TCHAR FirstDriverFromMask(ULONG unitmask);

    /*!
    * @brief Clear driver information list and release all allocated memory.
    */
    void ClearDriverInfos();

    /*!
    * @brief Indicates a specified volume name is removable driver.
    *
    * @param lpVolumeName       [I/ ] This name is a unique volume name of the form "\\?\Volume{GUID}\" where GUID is 
    *                                 the GUID that identifies the volume.
    */
    BOOL IsRemovableDriver(IN LPCTSTR lpVolumeName);

    /*!
    * @brief Indicates a specified root path is USB disk driver or not.
    *
    * @param lpRootPath     [I/ ] The root path, such as "A:\".
    *
    * @return TRUE if the driver is USB disk, FALSE otherwise.
    */
    BOOL IsUsbDisk(IN LPCTSTR lpRootPath);

    /*!
    * @brief Indicates a specified volume name is removable floppy.
    *
    * @param lpDriverName       [I/ ] The name of driver such as "A:\" or "A:".
    */
    BOOL IsFloppy(IN LPCTSTR lpDriverName);

    /*!
    * @brief Get the property of disk.
    *
    * @param hDevice            [I/ ] The handle of device.
    * @param pDevDesc           [I/ ] Pointer to PSTORAGE_DEVICE_DESCRIPTOR structure.
    */
    BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc);

private:

    vector<DRIVERINFO*> m_vctDriverInfos;           // The driver information.
    vector<DRIVERINFO*> m_vctRemovableInfos;        // The removable driver information, excluded Floppy.
};

END_NAMESPACE

#endif // _DRIVERSMANAGER_H_
#endif // __cplusplus
