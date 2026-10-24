# gucefCORE file system access

An abstraction over "a file system" for code that browses and changes files without caring where they live: the file system of
the operating system, the virtual file system of gucefVFS, or anything else that implements the interface (a test fake, a remote
store). The first user is the file dialogs of gucefGUI.

## 1. Pieces

| piece | module | what it is |
|---|---|---|
| `gucefCORE_EFileSystemAccessResult.h` | CORE | the result codes of file system operations; a header of its own that C code can read |
| `IFileSystemIterator` | CORE | the interface level of a directory iterator: `FindFirst / FindNext / FindClose`, entry type, name, path, metadata |
| `CFileSystemIterator` | CORE | the iterator over a directory of the operating system, an `IFileSystemIterator` |
| `CFileSystemEntryListIterator` | CORE | an iterator over entries read before (a listing in memory) |
| `IFileSystemAccess` | CORE | the file system: operations, special directories, async variants |
| `CNativeFileSystemAccess` | CORE | the file system of the operating system (local disks, network shares, anything the OS has mounted) |
| `CVfsFileSystemAccess` | VFS | the virtual file system of `CVfsGlobal` |
| `CGlobPattern` | CORE | glob matching (`*`, `?`, `[a-z]`, `[!x]`, `{a,b}`, `**/`) for searches |

## 2. Result codes

`TFileSystemAccessResult` (`enum EFileSystemAccessResult`) follows the convention of `gucefCORE_ETypes.h`: the enum sits in
`GUCEF::CORE` only for C++, the values carry the prefix `FILESYSTEMACCESS_` and fixed numbers, so the same header serves C code.
`FileSystemAccessResultToString()` is callable from C.

| value | meaning |
|---|---|
| `SUCCESS` (0) | done |
| `PARTIAL_SUCCESS` | an operation on several entries did some of them |
| `NOT_FOUND` / `PARENT_NOT_FOUND` | the entry, or the directory it should go in, is not there |
| `ALREADY_EXISTS` | the target exists and may not be replaced |
| `NOT_A_DIRECTORY` / `NOT_A_FILE` | the entry is of the other kind |
| `DIRECTORY_NOT_EMPTY` | a directory to delete has contents and deleting them was not asked |
| `ACCESS_DENIED` | the entry or its directory is read-only for the user |
| `READ_ONLY` | the place cannot be written at all (e.g. a VFS mount that is not writeable) |
| `INVALID_PATH` / `INVALID_NAME` | the path cannot be used; a new name has a separator or is empty |
| `SAME_PATH` | source and target are the same |
| `CROSS_DEVICE`, `IN_USE`, `NO_SPACE`, `TOO_LONG` | reserved for providers that can tell |
| `NOT_SUPPORTED` | the provider cannot do this |
| `QUEUEING_FAILED`, `CANCELLED`, `TIMEOUT` | async operations |
| `FAILED` | the operation failed for a reason the provider cannot tell |

The CORE file functions (`MoveFile`, `DeleteDir`, ...) return `bool`; the native provider maps them: checks before a call
give the specific codes, a call that still fails gives `FAILED`.

## 3. `IFileSystemAccess`

Derives from `CTSharedPtrCreator< IFileSystemAccess, MT::CMutex >`: providers are made with `GUCEF_NEW` and handed around as
`TFileSystemAccessPtr` (`( GUCEF_NEW CNativeFileSystemAccess() )->CreateSharedPtr()`).

Synchronous operations run on the calling thread and return a `TFileSystemAccessResult`:

| operation | |
|---|---|
| `TryCreateDirectory( path, createParents )` | |
| `TryDeleteDirectory( path, deleteContents )` | `DIRECTORY_NOT_EMPTY` without `deleteContents` |
| `TryMoveDirectory( oldPath, newPath )` | |
| `TryMoveFile( oldPath, newPath, overwrite )` | |
| `TryMoveFiles( files, destinationDir, overwrite, perFileResults )` | `PARTIAL_SUCCESS` when some failed; one result per file |
| `TryRenameFile( path, newName )` | in the same directory; the name has no separator |
| `TryDeleteFile( path )` | |
| `TryGetMetaData( path, metaData )` | `CResourceMetaData` |
| `TryGetEntryType( path, type )` | none, file or directory |
| `TryGetFileSystemIterator( dirPath, iterator )` | the interface level of the provider's iterator; walk it with `FindFirst( dirPath )` / `FindNext()` |
| `ListAvailableSpecialDirs( dirs )`, `TryResolveSpecialDir( id, path )` | the special directories the provider knows, as paths of the provider |
| `GetDisplayName()`, `GetRootPath()`, `IsWriteable( path )` | |

**Async variants** take the same arguments plus a `CFutureResult&` and an optional thread pool name. They return
`FILESYSTEMACCESS_SUCCESS` when the operation was queued (or `QUEUEING_FAILED`); the result of the operation itself is read
from the future when it is ready, with the static `TryGetAsyncResult`, `TryGetAsyncIterator`, `TryGetAsyncMetaData`,
`TryGetAsyncEntryType` and `TryGetAsyncPerFileResults`.

They are virtual with a default implementation: a task of the CORE task manager (`CAsyncFileSystemAccessOperation`, registered
by `CCoreGlobal`) that runs the synchronous operation on a thread of the pool. The task data holds a shared pointer to the
provider, so the provider lives until the task ended. A provider with a native async API can override a variant.

`TryGetFileSystemIteratorAsync` reads the whole directory on the pool thread and hands back a `CFileSystemEntryListIterator`,
so listing a large or slow directory never blocks the caller.

A pool without worker threads (a build without threads, such as WebAssembly without pthreads, or a pool set to 0 threads) only
runs a queued operation when a thread pitches in with `CThreadPool::CarryOutQueuedTasksIfAny()`; the file system dialogs of
gucefGUI do that from the GUI thread when their pool has no threads.

Thread safety: a provider is used from the calling thread and, through the async variants, from pool threads. The providers here
keep no state between calls (the operating system and the VFS do their own locking).

## 4. Providers

**`CNativeFileSystemAccess`** routes to the CORE file functions (`CreateDirs`, `DeleteDir`, `MoveFile`, `DeleteFile`,
`GetFileMetaData`, `FileExists`, `DirExists`, `CFileSystemIterator`, `TryResolveSpecialDir`, `ListAvailableSpecialDirs`). A
directory is moved through `MoveFile`, whose rename also moves directories. Paths are resolved with `RelativePath`, so
`$TEMPDIR$` and friends work. The root is `/` on Linux; on Windows the root `""` lists the drives.

**`CVfsFileSystemAccess`** routes to `CVFS`: moving and renaming to `MoveFile`, `DeleteFile`, `GetFileMetaData`, `FileExists` /
`DirExists`, and the directory operations `CreateDirectory` / `DeleteDirectory` / `MoveDirectory` of the VFS
(archives that cannot do them say "not supported"). The listing merges the directories and files of every mount that covers the
directory, and the mount points below it, without duplicates. The VFS has no special directories of its own: these are stubs
(`TryResolveSpecialDirToVfsPath`, `ListAvailableSpecialDirs`).

## 5. Special directories

`CORE::ListAvailableSpecialDirs( TSpecialDirSet& )` gives the ids `TryResolveSpecialDir` can resolve on the platform, so
callers do not have to try every id. The table mirrors the branches of `TryResolveSpecialDir`; a test keeps the two in step.
`SpecialDirToString()` gives a stable name for each id.
