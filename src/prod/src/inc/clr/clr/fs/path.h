// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#ifndef _clr_fs_Path_h_
#define _clr_fs_Path_h_

#include "clrtypes.h"
#include "cor.h" // SELECTANY

#include "strsafe.h"

#include "clr/str.h"

#ifndef LONG_FORMAT_PATH_PREFIX
    #define LONG_FORMAT_PATH_PREFIX W("\\\\?\\")
#endif

namespace clr
{
    namespace fs
    {
        // This list taken from ndp/clr/src/bcl/system/io/path.cs
        SELECTANY WCHAR const g_rgInvalidPathChars[] =
            { W('"'), W('<'), W('>'), W('|'), W('\0'), (WCHAR)1, (WCHAR)2, (WCHAR)3, (WCHAR)4, (WCHAR)5, (WCHAR)6,
              (WCHAR)7, (WCHAR)8, (WCHAR)9, (WCHAR)10, (WCHAR)11, (WCHAR)12, (WCHAR)13, (WCHAR)14,
              (WCHAR)15, (WCHAR)16, (WCHAR)17, (WCHAR)18, (WCHAR)19, (WCHAR)20, (WCHAR)21, (WCHAR)22,
              (WCHAR)23, (WCHAR)24, (WCHAR)25, (WCHAR)26, (WCHAR)27, (WCHAR)28, (WCHAR)29, (WCHAR)30,
              (WCHAR)31 };

        class Path
        {
        public:
            //-----------------------------------------------------------------------------------------
            static inline bool
            Exists(
                LPCWSTR wzPath)
            {
                DWORD attrs = WszGetFileAttributes(wzPath);
                return (attrs != INVALID_FILE_ATTRIBUTES);
            }

            //-----------------------------------------------------------------------------------------
            // Returns true if wzPath represents a long format path (i.e., prefixed with '\\?\').
            static inline bool
            HasLongFormatPrefix(LPCWSTR wzPath)
            {
                _ASSERTE(!clr::str::IsNullOrEmpty(wzPath)); // Must check this first.
                return wcscmp(wzPath, LONG_FORMAT_PATH_PREFIX) == 0;
            }

            //-----------------------------------------------------------------------------------------
            static inline bool
            HasUncPrefix(LPCWSTR wzPath)
            {
                _ASSERTE(!clr::str::IsNullOrEmpty(wzPath)); // Must check this first.
                return wzPath[0] != W('\0') && wzPath[0] == W('\\')
                    && wzPath[1] != W('\0') && wzPath[1] == W('\\')
                    && wzPath[2] != W('\0') && wzPath[2] != W('?');
            }

            //-----------------------------------------------------------------------------------------
            static inline bool
            HasDrivePrefix(LPCWSTR wzPath)
            {
                _ASSERTE(!clr::str::IsNullOrEmpty(wzPath)); // Must check this first.
                return wzPath[0] != W('\0')
                    && wzPath[1] != W('\0') && wzPath[1] == W(':')
                    && ((wzPath[0] >= W('a') && wzPath[0] <= W('z')) ||
                        (wzPath[0] >= W('A') && wzPath[0] <= W('Z')));
            }

            //-----------------------------------------------------------------------------------------
            // Returns true if wzPath represents a relative path.
            static inline bool
            IsRelative(LPCWSTR wzPath)
            {
                _ASSERTE(!clr::str::IsNullOrEmpty(wzPath)); // Must check this first.
                return !HasLongFormatPrefix(wzPath)
                    && !HasUncPrefix(wzPath)
                    && (!HasDrivePrefix(wzPath) || wzPath[2] != W('\\'));
            }

            //-----------------------------------------------------------------------------------------
            // Combines two path parts. wzPathLeft must be a directory path and may be either absolute
            // or relative. wzPathRight may be a directory or file path and must be relative. The
            // result is placed in wzBuffer and the number of chars written is placed in pcchBuffer on
            // success; otherwise an error HRESULT is returned.
            static HRESULT
            Combine(LPCWSTR wzPathLeft, LPCWSTR wzPathRight, __out DWORD *pcchBuffer, __out_ecount(*pcchBuffer) LPWSTR wzBuffer)
            {
                STATIC_CONTRACT_NOTHROW;

                HRESULT hr = S_OK;

                if (clr::str::IsNullOrEmpty(wzPathLeft) || clr::str::IsNullOrEmpty(wzPathRight) || pcchBuffer == nullptr)
                    return E_INVALIDARG;

                LPWSTR wzBuf = wzBuffer;
                size_t cchBuf = *pcchBuffer;

                IfFailRet(StringCchCopyExW(wzBuf, cchBuf, wzPathLeft, &wzBuf, &cchBuf, STRSAFE_NULL_ON_FAILURE));
                IfFailRet(StringCchCatExW(wzBuf, cchBuf, wzBuf[-1] == W('\\') ? W("") : W("\\"), &wzBuf, &cchBuf, STRSAFE_NULL_ON_FAILURE));
                IfFailRet(StringCchCatExW(wzBuf, cchBuf, wzPathRight, &wzBuf, &cchBuf, STRSAFE_NULL_ON_FAILURE));

                return S_OK;
            }

            //-----------------------------------------------------------------------------------------
            // Checks if the path provided is valid within the specified constraints.
            // ***NOTE: does not yet check for invalid path characters.
            static bool
            IsValid(LPCWSTR wzPath, DWORD cchPath, bool fAllowLongFormat)
            {
                if (clr::str::IsNullOrEmpty(wzPath))
                    return false;

                bool fIsLongFormat = HasLongFormatPrefix(wzPath);

                if (fIsLongFormat && !fAllowLongFormat)
                    return false;

                if (!fIsLongFormat && cchPath > _MAX_PATH)
                    return false;

                return true;
            }
        };
    }
}

#endif // _clr_fs_Path_h_
