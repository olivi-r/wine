/* WinRT Windows.ApplicationModel Package Implementation
 *
 * Copyright (C) 2023 Mohamad Al-Jaf
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "private.h"
#include "wine/debug.h"

#include "appmodel.h"
#include "pathcch.h"

WINE_DEFAULT_DEBUG_CHANNEL(model);

struct package_statics
{
    IActivationFactory IActivationFactory_iface;
    IPackageStatics IPackageStatics_iface;
    LONG ref;
};

static inline struct package_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct package_statics, IActivationFactory_iface );
}

static HRESULT WINAPI factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct package_statics *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IActivationFactory ))
    {
        *out = &impl->IActivationFactory_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_IPackageStatics ))
    {
        *out = &impl->IPackageStatics_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI factory_AddRef( IActivationFactory *iface )
{
    struct package_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI factory_Release( IActivationFactory *iface )
{
    struct package_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    return ref;
}

static HRESULT WINAPI factory_GetIids( IActivationFactory *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI factory_GetRuntimeClassName( IActivationFactory *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI factory_GetTrustLevel( IActivationFactory *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI factory_ActivateInstance( IActivationFactory *iface, IInspectable **instance )
{
    FIXME( "iface %p, instance %p stub!\n", iface, instance );
    return E_NOTIMPL;
}

static const struct IActivationFactoryVtbl factory_vtbl =
{
    factory_QueryInterface,
    factory_AddRef,
    factory_Release,
    /* IInspectable methods */
    factory_GetIids,
    factory_GetRuntimeClassName,
    factory_GetTrustLevel,
    /* IActivationFactory methods */
    factory_ActivateInstance,
};

struct storage_folder
{
    IStorageFolder IStorageFolder_iface;
    IStorageItem IStorageItem_iface;
    LONG ref;
};

static inline struct storage_folder *impl_from_IStorageFolder( IStorageFolder *iface )
{
    return CONTAINING_RECORD( iface, struct storage_folder, IStorageFolder_iface );
}

static HRESULT WINAPI storage_folder_QueryInterface( IStorageFolder *iface, REFIID iid, void **out )
{
    struct storage_folder *impl = impl_from_IStorageFolder( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IStorageFolder ))
    {
        *out = &impl->IStorageFolder_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_IStorageItem ))
    {
        *out = &impl->IStorageItem_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI storage_folder_AddRef( IStorageFolder *iface )
{
    struct storage_folder *impl = impl_from_IStorageFolder( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI storage_folder_Release( IStorageFolder *iface )
{
    struct storage_folder *impl = impl_from_IStorageFolder( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );

    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );

    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI storage_folder_GetIids( IStorageFolder *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_GetRuntimeClassName( IStorageFolder *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_GetTrustLevel( IStorageFolder *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_CreateFileAsyncOverloadDefaultOptions( IStorageFolder *iface, HSTRING name,
                                                                            IAsyncOperation_StorageFile **operation )
{
    FIXME( "iface %p, name %s, operation %p stub!\n", iface, debugstr_hstring(name), operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_CreateFileAsync( IStorageFolder *iface, HSTRING name, CreationCollisionOption options,
                                                      IAsyncOperation_StorageFile **operation )
{
    FIXME( "iface %p, name %s, options %d, operation %p stub!\n", iface, debugstr_hstring(name), options, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_CreateFolderAsyncOverloadDefaultOptions( IStorageFolder *iface, HSTRING name,
                                                                              IAsyncOperation_StorageFolder **operation )
{
    FIXME( "iface %p, name %s, operation %p stub!\n", iface, debugstr_hstring(name), operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_CreateFolderAsync( IStorageFolder *iface, HSTRING name, CreationCollisionOption options,
                                                        IAsyncOperation_StorageFolder **operation )
{
    FIXME( "iface %p, name %s, options %d, operation %p stub!\n", iface, debugstr_hstring(name), options, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_GetFileAsync( IStorageFolder *iface, HSTRING name, IAsyncOperation_StorageFile **operation )
{
    FIXME( "iface %p, name %s, operation %p stub!\n", iface, debugstr_hstring(name), operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_GetFolderAsync( IStorageFolder *iface, HSTRING name, IAsyncOperation_StorageFolder **operation )
{
    FIXME( "iface %p, name %s, operation %p stub!\n", iface, debugstr_hstring(name), operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_GetItemAsync( IStorageFolder *iface, HSTRING name, IAsyncOperation_IStorageItem **operation )
{
    FIXME( "iface %p, name %s, operation %p stub!\n", iface, debugstr_hstring(name), operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_GetFilesAsyncOverloadDefaultOptionsStartAndCount( IStorageFolder *iface,
                                                                                       IAsyncOperation_IVectorView_StorageFile **operation )
{
    FIXME( "iface %p, operation %p stub!\n", iface, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_GetFoldersAsyncOverloadDefaultOptionsStartAndCount( IStorageFolder *iface,
                                                                                         IAsyncOperation_IVectorView_StorageFolder **operation )
{
    FIXME( "iface %p, operation %p stub!\n", iface, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_folder_GetItemsAsyncOverloadDefaultStartAndCount( IStorageFolder *iface,
                                                                                IAsyncOperation_IVectorView_IStorageItem **operation )
{
    FIXME( "iface %p, operation %p stub!\n", iface, operation );
    return E_NOTIMPL;
}

static const struct IStorageFolderVtbl storage_folder_vtbl =
{
    storage_folder_QueryInterface,
    storage_folder_AddRef,
    storage_folder_Release,
    /* IInspectable methods */
    storage_folder_GetIids,
    storage_folder_GetRuntimeClassName,
    storage_folder_GetTrustLevel,
    /* IStorageFolder methods */
    storage_folder_CreateFileAsyncOverloadDefaultOptions,
    storage_folder_CreateFileAsync,
    storage_folder_CreateFolderAsyncOverloadDefaultOptions,
    storage_folder_CreateFolderAsync,
    storage_folder_GetFileAsync,
    storage_folder_GetFolderAsync,
    storage_folder_GetItemAsync,
    storage_folder_GetFilesAsyncOverloadDefaultOptionsStartAndCount,
    storage_folder_GetFoldersAsyncOverloadDefaultOptionsStartAndCount,
    storage_folder_GetItemsAsyncOverloadDefaultStartAndCount,
};

DEFINE_IINSPECTABLE( storage_item, IStorageItem, struct storage_folder, IStorageFolder_iface )

static HRESULT WINAPI storage_item_RenameAsyncOverloadDefaultOptions( IStorageItem *iface, HSTRING name, IAsyncAction **operation )
{
    FIXME( "iface %p, name %s, operation %p stub!\n", iface, debugstr_hstring(name), operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_item_RenameAsync( IStorageItem *iface, HSTRING name, NameCollisionOption option, IAsyncAction **operation )
{
    FIXME( "iface %p, name %s, option %d, operation %p stub!\n", iface, debugstr_hstring(name), option, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_item_DeleteAsyncOverloadDefaultOptions( IStorageItem *iface, IAsyncAction **operation )
{
    FIXME( "iface %p, operation %p stub!\n", iface, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_item_DeleteAsync( IStorageItem *iface, StorageDeleteOption option, IAsyncAction **operation )
{
    FIXME( "iface %p, option %d, operation %p stub!\n", iface, option, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_item_GetBasicPropertiesAsync( IStorageItem *iface, IAsyncOperation_BasicProperties **operation )
{
    FIXME( "iface %p, operation %p stub!\n", iface, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_item_get_Name( IStorageItem *iface, HSTRING *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_item_get_Path( IStorageItem *iface, HSTRING *value )
{
    WCHAR buffer[MAX_PATH];
    HRESULT hr;

    TRACE( "iface %p, value %p\n", iface, value );

    if (!GetModuleFileNameW( NULL, buffer, MAX_PATH )) return HRESULT_FROM_WIN32( GetLastError() );
    if (FAILED( hr = PathCchRemoveFileSpec( buffer, ARRAY_SIZE(buffer) ) )) return hr;

    return WindowsCreateString( buffer, wcslen(buffer), value );
}

static HRESULT WINAPI storage_item_get_Attributes( IStorageItem *iface, FileAttributes *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_item_get_DateCreated( IStorageItem *iface, DateTime *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI storage_item_IsOfType( IStorageItem *iface, StorageItemTypes type, boolean *value )
{
    FIXME( "iface %p, type %d, value %p stub!\n", iface, type, value );
    return E_NOTIMPL;
}

static const struct IStorageItemVtbl storage_item_vtbl =
{
    storage_item_QueryInterface,
    storage_item_AddRef,
    storage_item_Release,
    /* IInspectable methods */
    storage_item_GetIids,
    storage_item_GetRuntimeClassName,
    storage_item_GetTrustLevel,
    /* IStorageItem methods */
    storage_item_RenameAsyncOverloadDefaultOptions,
    storage_item_RenameAsync,
    storage_item_DeleteAsyncOverloadDefaultOptions,
    storage_item_DeleteAsync,
    storage_item_GetBasicPropertiesAsync,
    storage_item_get_Name,
    storage_item_get_Path,
    storage_item_get_Attributes,
    storage_item_get_DateCreated,
    storage_item_IsOfType,
};

struct package_id
{
    IPackageId IPackageId_iface;
    LONG ref;
    IPackage *package;
    const PACKAGE_ID *id;
};

static inline struct package_id *impl_from_IPackageId( IPackageId *iface )
{
    return CONTAINING_RECORD( iface, struct package_id, IPackageId_iface );
}

static HRESULT WINAPI package_id_QueryInterface( IPackageId *iface, REFIID iid, void **out )
{
    struct package_id *impl = impl_from_IPackageId( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IPackageId ))
    {
        IInspectable_AddRef( (*out = &impl->IPackageId_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI package_id_AddRef( IPackageId *iface )
{
    struct package_id *impl = impl_from_IPackageId( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI package_id_Release( IPackageId *iface )
{
    struct package_id *impl = impl_from_IPackageId( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref)
    {
        IPackage_Release( impl->package );
        free( impl );
    }
    return ref;
}

static HRESULT WINAPI package_id_GetIids( IPackageId *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_id_GetRuntimeClassName( IPackageId *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_id_GetTrustLevel( IPackageId *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_id_get_Name( IPackageId *iface, HSTRING *value )
{
    struct package_id *impl = impl_from_IPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( impl->id->name, wcslen( impl->id->name ), value );
}

static HRESULT WINAPI package_id_get_Version( IPackageId *iface, PackageVersion *value )
{
    struct package_id *impl = impl_from_IPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_INVALIDARG;
    value->Major = impl->id->version.Major;
    value->Minor = impl->id->version.Minor;
    value->Build = impl->id->version.Build;
    value->Revision = impl->id->version.Revision;
    return S_OK;
}

static HRESULT WINAPI package_id_get_Architecture( IPackageId *iface, ProcessorArchitecture *value )
{
    struct package_id *impl = impl_from_IPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_INVALIDARG;
    *value = impl->id->processorArchitecture;
    return S_OK;
}

static HRESULT WINAPI package_id_get_ResourceId( IPackageId *iface, HSTRING *value )
{
    struct package_id *impl = impl_from_IPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( impl->id->resourceId, wcslen( impl->id->resourceId ), value );
}

static HRESULT WINAPI package_id_get_Publisher( IPackageId *iface, HSTRING *value )
{
    struct package_id *impl = impl_from_IPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( impl->id->publisher, wcslen( impl->id->publisher ), value );
}

static HRESULT WINAPI package_id_get_PublisherId( IPackageId *iface, HSTRING *value )
{
    struct package_id *impl = impl_from_IPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsCreateString( impl->id->publisherId, wcslen( impl->id->publisherId ), value );
}

static HRESULT WINAPI package_id_get_FullName( IPackageId *iface, HSTRING *value )
{
    struct package_id  *impl = impl_from_IPackageId( iface );
    HSTRING_BUFFER handle;
    UINT32 size = 0;
    WCHAR *buffer;
    HRESULT hr;

    TRACE( "iface %p, value %p.\n", iface, value );

    PackageFullNameFromId( impl->id, &size, NULL );
    if (FAILED(hr = WindowsPreallocateStringBuffer( size - 1, &buffer, &handle )))
        return hr;

    PackageFullNameFromId( impl->id, &size, buffer );
    if (FAILED(hr = WindowsPromoteStringBuffer( handle, value )))
        WindowsDeleteStringBuffer( handle );

    return hr;
}

static HRESULT WINAPI package_id_get_FamilyName( IPackageId *iface, HSTRING *value )
{
    struct package_id  *impl = impl_from_IPackageId( iface );
    HSTRING_BUFFER handle;
    UINT32 size = 0;
    WCHAR *buffer;
    HRESULT hr;

    TRACE( "iface %p, value %p.\n", iface, value );

    PackageFamilyNameFromId( impl->id, &size, NULL );
    if (FAILED(hr = WindowsPreallocateStringBuffer( size - 1, &buffer, &handle )))
        return hr;

    PackageFamilyNameFromId( impl->id, &size, buffer );
    if (FAILED(hr = WindowsPromoteStringBuffer( handle, value )))
        WindowsDeleteStringBuffer( handle );

    return hr;
}

static const struct IPackageIdVtbl package_id_vtbl =
{
    package_id_QueryInterface,
    package_id_AddRef,
    package_id_Release,
    /* IInspectable methods */
    package_id_GetIids,
    package_id_GetRuntimeClassName,
    package_id_GetTrustLevel,
    /* IPackageId methods */
    package_id_get_Name,
    package_id_get_Version,
    package_id_get_Architecture,
    package_id_get_ResourceId,
    package_id_get_Publisher,
    package_id_get_PublisherId,
    package_id_get_FullName,
    package_id_get_FamilyName,
};

struct package
{
    IPackage IPackage_iface;
    LONG ref;
    PACKAGE_ID *id;
};

static inline struct package *impl_from_IPackage( IPackage *iface )
{
    return CONTAINING_RECORD( iface, struct package, IPackage_iface );
}

static HRESULT WINAPI package_QueryInterface( IPackage *iface, REFIID iid, void **out )
{
    struct package *impl = impl_from_IPackage( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IPackage ))
    {
        *out = &impl->IPackage_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI package_AddRef( IPackage *iface )
{
    struct package *impl = impl_from_IPackage( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI package_Release( IPackage *iface )
{
    struct package *impl = impl_from_IPackage( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );

    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );

    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI package_GetIids( IPackage *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_GetRuntimeClassName( IPackage *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_GetTrustLevel( IPackage *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_get_Id( IPackage *iface, IPackageId **value )
{
    struct package *impl = impl_from_IPackage( iface );
    struct package_id *id_impl;

    TRACE( "iface %p, value %p.\n", iface, value );

    if (!value) return E_INVALIDARG;
    if (!(id_impl = calloc( 1, sizeof(*id_impl) ))) return E_OUTOFMEMORY;
    id_impl->IPackageId_iface.lpVtbl = &package_id_vtbl;
    id_impl->ref = 1;

    IPackage_AddRef( (id_impl->package = iface) );
    id_impl->id = impl->id;

    *value = &id_impl->IPackageId_iface;
    return S_OK;
}

static HRESULT WINAPI package_get_InstalledLocation( IPackage *iface, IStorageFolder **value )
{
    struct storage_folder *impl;

    TRACE( "iface %p, value %p\n", iface, value );

    if (!value) return E_INVALIDARG;
    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;

    impl->IStorageFolder_iface.lpVtbl = &storage_folder_vtbl;
    impl->IStorageItem_iface.lpVtbl = &storage_item_vtbl;
    impl->ref = 1;

    *value = &impl->IStorageFolder_iface;
    TRACE( "created IStorageFolder %p.\n", *value );
    return S_OK;
}

static HRESULT WINAPI package_get_IsFramework( IPackage *iface, boolean *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI package_get_Dependencies( IPackage *iface, IVectorView_Package **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static const struct IPackageVtbl package_vtbl =
{
    package_QueryInterface,
    package_AddRef,
    package_Release,
    /* IInspectable methods */
    package_GetIids,
    package_GetRuntimeClassName,
    package_GetTrustLevel,
    /* IPackage methods */
    package_get_Id,
    package_get_InstalledLocation,
    package_get_IsFramework,
    package_get_Dependencies,
};

DEFINE_IINSPECTABLE( package_statics, IPackageStatics, struct package_statics, IActivationFactory_iface )

static HRESULT WINAPI package_statics_get_Current( IPackageStatics *iface, IPackage **value )
{
    struct package *impl;
    UINT32 size = 0;

    TRACE( "iface %p, value %p\n", iface, value );

    if (!value) return E_INVALIDARG;

    if (GetCurrentPackageId( &size, NULL ) == APPMODEL_ERROR_NO_PACKAGE)
        return E_NOTIMPL;

    if (!(impl = calloc( 1, sizeof(*impl) + size ))) return E_OUTOFMEMORY;

    impl->IPackage_iface.lpVtbl = &package_vtbl;
    impl->ref = 1;

    GetCurrentPackageId( &size, (BYTE *)(impl->id = (PACKAGE_ID *)&impl[1]) );

    *value = &impl->IPackage_iface;
    TRACE( "created IPackage %p.\n", *value );
    return S_OK;
}

static const struct IPackageStaticsVtbl package_statics_vtbl =
{
    package_statics_QueryInterface,
    package_statics_AddRef,
    package_statics_Release,
    /* IInspectable methods */
    package_statics_GetIids,
    package_statics_GetRuntimeClassName,
    package_statics_GetTrustLevel,
    /* IPackageStatics methods */
    package_statics_get_Current,
};

static struct package_statics package_statics =
{
    {&factory_vtbl},
    {&package_statics_vtbl},
    1,
};

IActivationFactory *package_factory = &package_statics.IActivationFactory_iface;
