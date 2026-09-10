//------------------------------------------------------------------------------
//
// File Name:	IOSystem.cpp
// Author(s):	Jonathan Sandquist (jonathan.s)
// Course:		GAM200F25
// Project:		Card Critters
// Purpose:		System for unifying IO operations and directories.
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include <Windows.h>
//#include <windef.h>
//#include <minwinbase.h>
#include <winnt.h>

#include <shlobj.h>
#include <filesystem>

#include "IOSystem.h"
#include "BaseSystem.h"

#include "StreamCommon.h"

//------------------------------------------------------------------------------
// External Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Namespace Declarations:
//------------------------------------------------------------------------------

namespace Nightjar
{
    //--------------------------------------------------------------------------
    // Class Implementation:
    //--------------------------------------------------------------------------

#pragma region Class Implementation

    class IOSystem_ SYSTEM_FINAL : public BaseSystem
    {
        friend IOSystem;

        SYSTEM_COMMON(IOSystem_);

        // Public Constants, Enums, and Structs:
    public:
        static constexpr string_view_t DirAssets{ DIR_ASSETS };
        static constexpr string_view_t DirData{ DIR_DATA };

        typedef std::function<void(string_view_arg)> FileChangeCallback;

        // Constructors/Destructors:
    private:
        IOSystem_(void);

        // All systems need a virtual destructor to have their destructor called 
        ~IOSystem_(void) override;

        // Public Static Functions:
    public:

        // Public Functions:
    public:

        // Public Event Handlers
    public:

        // Private Constants, Enums, and Structs:
    private:
        static const StreamKey LocalSubdirectory;

        struct DirectoryWatcher
        {
            // Sources referenced when making this struct:
            // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-readdirectorychangesw (and related windows api documentation)
            // https://gist.github.com/nickav/a57009d4fcc3b527ed0f5c9cf30618f8 as an example of ReadDirectoryChangesW
            // https://qualapps.blogspot.com/2010/05/understanding-readdirectorychangesw.html for getting an understanding (did not visit part 2 at time of this writing)
        public:
            string_t directory_;
            vector_t<FILE_NOTIFY_INFORMATION> fileNotifications_;
            HANDLE dataHandle_;
            OVERLAPPED overlapped_;

            DirectoryWatcher();

            void SetDirectory(string_view_arg directory);

            bool Init();

            void Update();

            void Exit();

        private:
            bool PerformRequest();
            void ProcessChanges();
        };

        // Private Static Functions:
    private:

        // Private Functions:
    private:
        /// <summary>
        /// Initializes the system.
        /// </summary>
        /// <returns>True if initialization succeeded, false otherwise.</returns>
        bool Init(void) override;

        /// <summary>
        /// Updates the system.
        /// </summary>
        void Update(void) override;

        /// <summary>
        /// Exits and shuts down the system.
        /// </summary>
        void Exit(void) override;

        // Private Static Variables:
    private:
        static IOSystem_* instance;

        // Private Variables:
    private:
        string_t exec_, userdata_, temp_;
        string_t assets_, data_;
        vector_t<std::pair<CallbackID, FileChangeCallback>> changeCallbacks_;
        DirectoryWatcher* assetsWatcher_, * dataWatcher_;
    };

#pragma endregion Class Implementation

    //--------------------------------------------------------------------------
    // Public Constants:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Static Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Public Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Private Constants:
    //--------------------------------------------------------------------------

    constexpr StreamKey IOSystem_::LocalSubdirectory = "DigiPen/CardCritters/";

    //--------------------------------------------------------------------------
    // Private Static Variables:
    //--------------------------------------------------------------------------

    IOSystem_* IOSystem_::instance = nullptr;

#define inst IOSystem_::instance

    //--------------------------------------------------------------------------
    // Private Variables:
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Constructors/Destructors:
    //--------------------------------------------------------------------------

#pragma region Constructors

    IOSystem_::IOSystem_(void)
        : BaseSystem("IOSystem", LoopFlags::HasUpdate)
        , exec_()
        , userdata_()
        , temp_()
        , assets_("Assets/")
        , data_("Data/")
        , changeCallbacks_()
        , assetsWatcher_(new DirectoryWatcher())
        , dataWatcher_(new DirectoryWatcher())
    {
        // Raise an assert if this system has already been created.
        assert(instance == nullptr);

        // Store this system's instance for use by static functions.
        instance = this;
    }

    IOSystem_::DirectoryWatcher::DirectoryWatcher()
        : directory_()
        , fileNotifications_()
        , dataHandle_(nullptr)
        , overlapped_()
    {
    }

    //--------------------------------------------------------------------------

    IOSystem_::~IOSystem_(void)
    {
        if (dataWatcher_ != nullptr)
        {
            delete dataWatcher_;
            dataWatcher_ = nullptr;
        }
    }

#pragma endregion Constructors

    //--------------------------------------------------------------------------
    // Public Static Functions:
    //--------------------------------------------------------------------------

#pragma region Public Static Functions

    BaseSystem* IOSystem::GetInstance(void)
    {
        return inst != nullptr ? inst : new IOSystem_();
    }

    const string_t& IOSystem::GetAssetsPath(void)
    {
        GetInstance();
        return inst->assets_;
    }

    string_t IOSystem::GetAssetsPath(string_view_arg subdirectory)
    {
        GetInstance();
        string_t tmp{ inst->assets_ };
        tmp += subdirectory;
        tmp += '/';
        return tmp;
    }

    const string_t& IOSystem::GetDataPath(void)
    {
        GetInstance();
        return inst->data_;
    }

    string_t IOSystem::GetDataPath(string_view_arg subdirectory)
    {
        GetInstance();
        string_t tmp{ inst->data_ };
        tmp += subdirectory;
        tmp += '/';
        return tmp;
    }

    const string_t& IOSystem::GetExecutablePath(void)
    {
        GetInstance();
        if (inst->exec_.empty())
        {
            wstring_t buffer(MAX_PATH, '\0');
            unsigned loopPrevention = 5;
            while (loopPrevention > 0)
            {
                DWORD size = GetModuleFileNameW(NULL, buffer.data(), static_cast<DWORD>(buffer.size()));
                if (size == 0)
                {
                    assert(false);
                    throw std::runtime_error("Failed to get executable path, path is empty!");
                }
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER || size == buffer.size())
                {
                    buffer.resize(buffer.size() * 2);
                    continue;
                }
                if (size < buffer.size())
                {
                    buffer.resize(size);
                    break;
                }
                --loopPrevention;
            }
            if (loopPrevention <= 0)
            {
                assert(false);
                throw std::runtime_error("Failed to get executable path, path is too long!");
            }

            inst->exec_ = StringWideToMulti(buffer);
            ValidatePathSeparators(inst->exec_);
            size_t lastDir = inst->exec_.find_last_of('/');
            if (lastDir != string_t::npos)
            {
                inst->exec_.resize(lastDir);
            }
        }
        return inst->exec_;
    }

    const string_t& IOSystem::GetUserDataPath(void)
    {
        GetInstance();
        if (inst->userdata_.empty())
        {
            TCHAR szPath[MAX_PATH];
            HRESULT hr = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);

            if (!SUCCEEDED(hr))
            {
                assert(false);
                throw std::runtime_error("Failed to get user data path!");
            }

            inst->userdata_ = StringWideToMulti(szPath);

            ValidatePathSeparators(inst->userdata_);
            if (inst->userdata_.back() != '/')
            {
                inst->userdata_ += '/';
            }
            inst->userdata_ += IOSystem_::LocalSubdirectory;
            std::filesystem::create_directories(inst->userdata_);
        }
        return inst->userdata_;
    }

    const string_t& IOSystem::GetTemporaryPath(void)
    {
        GetInstance();
        if (inst->temp_.empty())
        {
            inst->temp_ = std::filesystem::temp_directory_path().string();
            ValidatePathSeparators(inst->temp_);
        }
        return inst->temp_;
    }

    CallbackID IOSystem::RegisterChangeCallback(const FileChangeCallback& callback)
    {
        static CallbackID callbackCounter{ 1 };
        CallbackID id = callbackCounter++;
        inst->changeCallbacks_.push_back({ id, callback });
        return id;
    }

    bool IOSystem::UnregisterChangeCallback(const CallbackID& callbackID)
    {
        size_t count = std::erase_if(inst->changeCallbacks_, [callbackID](const auto& entry) { return entry.first == callbackID; });
        return (bool)count;
    }

    bool IOSystem::ReadFileAsString(const string_t& filename, string_t& buffer)
    {
        std::ifstream inFileStream(filename.c_str());
        if (!inFileStream.is_open())
        {
            return false;
        }
        std::stringstream bufferStream;
        bufferStream << inFileStream.rdbuf();
        inFileStream.close();
        buffer = bufferStream.str();
        return true;
    }

    bool IOSystem::FileExists(string_view_arg path)
    {
        return std::filesystem::status(path).type() == std::filesystem::file_type::regular;
    }

    void IOSystem::ForceDirectory(string_view_arg path)
    {
        std::filesystem::create_directories(path);
    }

    void IOSystem::Destroy(string_view_arg path, bool preserveDirectory, bool preserveRoot)
    {
        if (preserveDirectory)
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    std::filesystem::remove(entry);
                }
            }
        }
        else if (preserveRoot)
        {
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                std::filesystem::remove_all(entry);
            }
        }
        else
        {
            std::filesystem::remove_all(path);
        }
    }

    bool IOSystem::Copy(string_view_arg from, string_view_arg to, bool overwrite)
    {
        std::filesystem::copy_options options = overwrite ? std::filesystem::copy_options::overwrite_existing : std::filesystem::copy_options::none;
        return std::filesystem::copy_file(from, to, options);
    }

    bool IOSystem::Move(string_view_arg from, string_view_arg to, bool overwrite)
    {
        if (!overwrite && std::filesystem::status(to).type() != std::filesystem::file_type::not_found)
        {
            return false;
        }
        std::error_code err;
        std::filesystem::rename(from, to, err);
        return err == std::error_code{};
    }

    bool IOSystem::Rename(string_view_arg from, string_view_arg to)
    {
        if (std::filesystem::status(to).type() != std::filesystem::file_type::not_found)
        {
            return false;
        }
        std::error_code err;
        std::filesystem::rename(from, to, err);
        return err == std::error_code{};
    }

    void IOSystem::ValidatePathSeparators(string_t& path)
    {
        for (auto& c : path)
        {
            if (c == '\\')
            {
                c = '/';
            }
        }
    }

    string_t IOSystem::ConvertPathSeparators(string_view_arg path)
    {
        string_t temp(path);
        ValidatePathSeparators(temp);
        return temp;
    }

#pragma endregion Public Static Functions

    //--------------------------------------------------------------------------
    // Public Functions:
    //--------------------------------------------------------------------------

#pragma region Public Functions

#pragma endregion Public Functions

    //--------------------------------------------------------------------------
    // Private Static Functions:
    //--------------------------------------------------------------------------

#pragma region Private Static Functions

#pragma endregion Private Static Functions

    //--------------------------------------------------------------------------
    // Private Functions:
    //--------------------------------------------------------------------------

#pragma region Private Functions

    bool IOSystem_::Init(void)
    {
        MemoryManager::RegisterCategory(StreamAllocator::memId, StreamAllocator::memName);

        IOSystem::GetUserDataPath();
        IOSystem::ForceDirectory(userdata_);
        IOSystem::GetTemporaryPath();

        assetsWatcher_->SetDirectory(assets_);
        if (!assetsWatcher_->Init())
        {
            return false;
        }

        dataWatcher_->SetDirectory(data_);
        if (!dataWatcher_->Init())
        {
            return false;
        }

        // Return true if the initialization completed successfully.
        return true;
    }

    void IOSystem_::Update(void)
    {
        assetsWatcher_->Update();
        dataWatcher_->Update();
    }

    void IOSystem_::Exit(void)
    {
        if (assetsWatcher_ != nullptr)
        {
            assetsWatcher_->Exit();
            SafeDelete(assetsWatcher_);
        }
        if (dataWatcher_ != nullptr)
        {
            dataWatcher_->Exit();
            SafeDelete(dataWatcher_);
        }
    }

    void IOSystem_::DirectoryWatcher::SetDirectory(string_view_arg directory)
    {
        directory_ = directory;
    }

    bool IOSystem_::DirectoryWatcher::Init()
    {
        // Referenced: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
        dataHandle_ = CreateFile(
            StringMultiToWide(directory_).c_str(),                  // lpFileName
            FILE_LIST_DIRECTORY,                                    // dwDesiredAccess
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // dwShareMode
            NULL,                                                   // lpSecurityAttributes
            OPEN_EXISTING,                                          // dwCreationDisposition
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,      // dwFlagsAndAttributes
            NULL                                                    // hTemplateFile
        );

        if (dataHandle_ == INVALID_HANDLE_VALUE)
        {
            auto error = GetLastError();
            TraceError("IOSystem directory monitor failed setup for handle: {}", error);
            dataHandle_ = NULL;
            return false;
        }

        fileNotifications_.resize(20);

        // Referenced: https://gist.github.com/nickav/a57009d4fcc3b527ed0f5c9cf30618f8
        overlapped_.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

        PerformRequest();

        return true;
    }

    void IOSystem_::DirectoryWatcher::Update()
    {
        // Referenced: https://gist.github.com/nickav/a57009d4fcc3b527ed0f5c9cf30618f8
        DWORD result = WaitForSingleObject(overlapped_.hEvent, 0);
        while (result == WAIT_OBJECT_0)
        {
            DWORD bytes_transferred;
            GetOverlappedResult(dataHandle_, &overlapped_, &bytes_transferred, FALSE);

            ProcessChanges();

            PerformRequest();

            result = WaitForSingleObject(overlapped_.hEvent, 0);
        }
    }

    void IOSystem_::DirectoryWatcher::Exit()
    {
        if (overlapped_.hEvent != NULL && overlapped_.hEvent != INVALID_HANDLE_VALUE)
        {
            CloseHandle(overlapped_.hEvent);
            overlapped_.hEvent = NULL;
        }
        if (dataHandle_ != NULL && dataHandle_ != INVALID_HANDLE_VALUE)
        {
            CloseHandle(dataHandle_);
            dataHandle_ = NULL;
        }
    }

    bool IOSystem_::DirectoryWatcher::PerformRequest()
    {
        // Referenced: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-readdirectorychangesw
        constexpr DWORD changeFilter =
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE |
            //FILE_NOTIFY_CHANGE_LAST_ACCESS |
            FILE_NOTIFY_CHANGE_CREATION |
            //FILE_NOTIFY_CHANGE_SECURITY |
            0;
        DWORD bufferSize = (DWORD)(sizeof(FILE_NOTIFY_INFORMATION) * fileNotifications_.size());
        bool changed = ReadDirectoryChangesW(
            dataHandle_,            // hDirectory
            &fileNotifications_[0], // lpBuffer
            bufferSize,             // nBufferLength
            TRUE,                   // bWatchSubtree
            changeFilter,           // dwNotifyFilter
            NULL,                   // lpBytesReturned
            &overlapped_,           // lpOverlapped
            NULL                    // lpCompletionRoutine
        );

        return changed;
    }

    void IOSystem_::DirectoryWatcher::ProcessChanges()
    {
        // Referenced: https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-file_notify_information
        PFILE_NOTIFY_INFORMATION fileChange, fileChangeNext = &fileNotifications_[0];
        do
        {
            fileChange = fileChangeNext;
            std::wstring_view fileNameW{ fileChange->FileName, fileChange->FileNameLength / sizeof(wchar_t) };
            string_t fileName = directory_ + StringWideToMulti(fileNameW);
            switch (fileChange->Action)
            {
            case FILE_ACTION_ADDED:
                TraceInfo("File Added: {}", fileName);
                break;
            case FILE_ACTION_REMOVED:
                TraceInfo("File Removed: {}", fileName);
                break;
            case FILE_ACTION_MODIFIED:
                TraceInfo("File Modified: {}", fileName);
                break;
            case FILE_ACTION_RENAMED_OLD_NAME:
                TraceInfo("File Renamed Old: {}", fileName);
                break;
            case FILE_ACTION_RENAMED_NEW_NAME:
                TraceInfo("File Renamed New: {}", fileName);
                break;
            }

            for (const auto& [callbackID, callback] : instance->changeCallbacks_)
            {
                callback(fileName);
            }

            fileChangeNext = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(reinterpret_cast<char*>(fileChange) + fileChange->NextEntryOffset);
        } while (fileChange != fileChangeNext);
    }

#pragma endregion Private Functions

}	// namespace
