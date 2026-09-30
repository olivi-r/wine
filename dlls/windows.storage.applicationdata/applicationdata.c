/* WinRT Windows.Storage.ApplicationData ApplicationData Implementation
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
#include "roapi.h"

WINE_DEFAULT_DEBUG_CHANNEL(data);

struct application_data_container
{
    IApplicationDataContainer IApplicationDataContainer_iface;
    LONG ref;

    SRWLOCK lock;
    IMap_HSTRING_IInspectable *containers;
    IPropertySet *values;
    HSTRING name;
};

static inline struct application_data_container *impl_from_IApplicationDataContainer( IApplicationDataContainer *iface )
{
    return CONTAINING_RECORD( iface, struct application_data_container, IApplicationDataContainer_iface );
}

static HRESULT WINAPI application_data_container_QueryInterface( IApplicationDataContainer *iface, REFIID iid, void **out )
{
    struct application_data_container *impl = impl_from_IApplicationDataContainer( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IApplicationDataContainer ))
    {
        IInspectable_AddRef( (*out = &impl->IApplicationDataContainer_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI application_data_container_AddRef( IApplicationDataContainer *iface )
{
    struct application_data_container *impl = impl_from_IApplicationDataContainer( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI application_data_container_Release( IApplicationDataContainer *iface )
{
    struct application_data_container *impl = impl_from_IApplicationDataContainer( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref)
    {
        IMap_HSTRING_IInspectable_Release( impl->containers );
        IPropertySet_Release( impl->values );
        WindowsDeleteString( impl->name );
        free( impl );
    }
    return ref;
}

static HRESULT WINAPI application_data_container_GetIids( IApplicationDataContainer *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_container_GetRuntimeClassName( IApplicationDataContainer *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_container_GetTrustLevel( IApplicationDataContainer *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_container_get_Name( IApplicationDataContainer *iface, HSTRING *value )
{
    struct application_data_container *impl = impl_from_IApplicationDataContainer( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    return WindowsDuplicateString( impl->name, value );
}

static HRESULT WINAPI application_data_container_get_Locality( IApplicationDataContainer *iface, ApplicationDataLocality *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_container_get_Values( IApplicationDataContainer *iface, IPropertySet **value )
{
    struct application_data_container *impl = impl_from_IApplicationDataContainer( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_INVALIDARG;
    IPropertySet_AddRef( (*value = impl->values) );
    return S_OK;
}

static HRESULT WINAPI application_data_container_get_Containers( IApplicationDataContainer *iface, IMapView_HSTRING_ApplicationDataContainer **value )
{
    struct application_data_container *impl = impl_from_IApplicationDataContainer( iface );
    HRESULT hr;
    TRACE( "iface %p, value %p.\n", iface, value );
    AcquireSRWLockExclusive( &impl->lock );
    hr = IMap_HSTRING_IInspectable_GetView( impl->containers, (IMapView_HSTRING_IInspectable **)value );
    ReleaseSRWLockExclusive( &impl->lock );
    return hr;
}

static HRESULT application_data_container_create( HSTRING name, IApplicationDataContainer **instance );

static HRESULT WINAPI application_data_container_CreateContainer( IApplicationDataContainer *iface, HSTRING name, ApplicationDataCreateDisposition disposition, IApplicationDataContainer **value )
{
    struct application_data_container *impl = impl_from_IApplicationDataContainer( iface );
    BOOLEAN dummy;
    HRESULT hr;

    FIXME( "iface %p, name %s, disposition %d, value %p semi-stub\n", iface, debugstr_hstring( name ), disposition, value );

    if (!value) return E_INVALIDARG;

    AcquireSRWLockExclusive( &impl->lock );
    if (FAILED(hr = IMap_HSTRING_IInspectable_Lookup( impl->containers, name, (IInspectable **)value )))
    {
        if (FAILED(hr = application_data_container_create( name, value ))) goto end;
        hr = IMap_HSTRING_IInspectable_Insert( impl->containers, name, (IInspectable *)*value, &dummy );
    }

end:
    ReleaseSRWLockExclusive( &impl->lock );
    return hr;
}

static HRESULT WINAPI application_data_container_DeleteContainer( IApplicationDataContainer *iface, HSTRING name )
{
    FIXME( "iface %p, name %s stub!\n", iface, debugstr_hstring( name ) );
    return E_NOTIMPL;
}

static const struct IApplicationDataContainerVtbl application_data_container_vtbl =
{
    application_data_container_QueryInterface,
    application_data_container_AddRef,
    application_data_container_Release,
    /* IInspectable methods */
    application_data_container_GetIids,
    application_data_container_GetRuntimeClassName,
    application_data_container_GetTrustLevel,
    /* IApplicationDataContainer methods */
    application_data_container_get_Name,
    application_data_container_get_Locality,
    application_data_container_get_Values,
    application_data_container_get_Containers,
    application_data_container_CreateContainer,
    application_data_container_DeleteContainer,
};

static HRESULT application_data_container_create( HSTRING name, IApplicationDataContainer **instance )
{
    static const WCHAR *class = RuntimeClass_Windows_Foundation_Collections_PropertySet;
    struct application_data_container *impl;
    IInspectable *inspectable;
    HSTRING_HEADER header;
    HSTRING string;
    HRESULT hr;

    static const struct map_iids iids =
    {
        .map = &IID_IMap_HSTRING_IInspectable,
        .view = &IID_IMapView_HSTRING_ApplicationDataContainer,
        .iterable = &IID_IIterable_IKeyValuePair_HSTRING_ApplicationDataContainer,
        .iterator = &IID_IIterator_IKeyValuePair_HSTRING_ApplicationDataContainer,
        .pair = &IID_IKeyValuePair_HSTRING_ApplicationDataContainer,
    };

    TRACE( "names %s, instance %p.\n", debugstr_hstring( name ), instance );

    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->IApplicationDataContainer_iface.lpVtbl = &application_data_container_vtbl;
    impl->ref = 1;

    if (FAILED(hr = single_threaded_map_create( &iids, NULL, &inspectable )))
    {
        free( impl );
        return hr;
    }

    hr = IInspectable_QueryInterface( inspectable, &IID_IMap_HSTRING_IInspectable, (void **)&impl->containers );
    IInspectable_Release( inspectable );
    if (FAILED(hr))
    {
        free( impl );
        return hr;
    }

    if (FAILED(hr = WindowsDuplicateString( name, &impl->name )))
    {
        IMap_HSTRING_IInspectable_Release( impl->containers );
        free( impl );
        return hr;
    }

    WindowsCreateStringReference( class, wcslen( class ), &header, &string );
    if (FAILED(hr = RoActivateInstance( string, (IInspectable **)&impl->values )))
    {
        IMap_HSTRING_IInspectable_Release( impl->containers );
        free( impl );
        return hr;
    }

    *instance = &impl->IApplicationDataContainer_iface;
    TRACE( "created IApplicationDataContainer %p.\n", *instance );
    return S_OK;
}

struct application_data_statics
{
    IActivationFactory IActivationFactory_iface;
    IApplicationDataStatics IApplicationDataStatics_iface;
    LONG ref;
};

static inline struct application_data_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct application_data_statics, IActivationFactory_iface );
}

static HRESULT WINAPI factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct application_data_statics *impl = impl_from_IActivationFactory( iface );

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

    if (IsEqualGUID( iid, &IID_IApplicationDataStatics ))
    {
        *out = &impl->IApplicationDataStatics_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI factory_AddRef( IActivationFactory *iface )
{
    struct application_data_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI factory_Release( IActivationFactory *iface )
{
    struct application_data_statics *impl = impl_from_IActivationFactory( iface );
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

struct application_data
{
    IApplicationData IApplicationData_iface;
    LONG ref;

    IApplicationDataContainer *local_settings;
    IApplicationDataContainer *roaming_settings;
};

static inline struct application_data *impl_from_IApplicationData( IApplicationData *iface )
{
    return CONTAINING_RECORD( iface, struct application_data, IApplicationData_iface );
}

static HRESULT WINAPI application_data_QueryInterface( IApplicationData *iface, REFIID iid, void **out )
{
    struct application_data *impl = impl_from_IApplicationData( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IApplicationData ))
    {
        *out = &impl->IApplicationData_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI application_data_AddRef( IApplicationData *iface )
{
    struct application_data *impl = impl_from_IApplicationData( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI application_data_Release( IApplicationData *iface )
{
    struct application_data *impl = impl_from_IApplicationData( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );

    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );

    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI application_data_GetIids( IApplicationData *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_GetRuntimeClassName( IApplicationData *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_GetTrustLevel( IApplicationData *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_get_Version( IApplicationData *iface, UINT32 *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_SetVersionAsync( IApplicationData *iface, UINT32 version, IApplicationDataSetVersionHandler *handler,
                                                        IAsyncAction **operation )
{
    FIXME( "iface %p, version %d, handler %p, operation %p stub!\n", iface, version, handler, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_ClearAllAsync( IApplicationData *iface, IAsyncAction **operation )
{
    FIXME( "iface %p, operation %p stub!\n", iface, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_ClearAsync( IApplicationData *iface, ApplicationDataLocality locality, IAsyncAction **operation )
{
    FIXME( "iface %p, %d locality, operation %p stub!\n", iface, locality, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_get_LocalSettings( IApplicationData *iface, IApplicationDataContainer **value )
{
    struct application_data *impl = impl_from_IApplicationData( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_INVALIDARG;
    IApplicationDataContainer_AddRef( (*value = impl->local_settings) );
    return S_OK;
}

static HRESULT WINAPI application_data_get_RoamingSettings( IApplicationData *iface, IApplicationDataContainer **value )
{
    struct application_data *impl = impl_from_IApplicationData( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_INVALIDARG;
    IApplicationDataContainer_AddRef( (*value = impl->roaming_settings) );
    return S_OK;
}

static HRESULT WINAPI application_data_get_LocalFolder( IApplicationData *iface, IStorageFolder **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_get_RoamingFolder( IApplicationData *iface, IStorageFolder **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_get_TemporaryFolder( IApplicationData *iface, IStorageFolder **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_add_DataChanged( IApplicationData *iface, ITypedEventHandler_ApplicationData_IInspectable *handler,
                                                        EventRegistrationToken *token )
{
    FIXME( "iface %p, handler %p, token %p stub!\n", iface, handler, token );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_remove_DataChanged( IApplicationData *iface, EventRegistrationToken token )
{
    FIXME( "iface %p, token %#I64x stub!\n", iface, token.value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_SignalDataChanged( IApplicationData *iface )
{
    FIXME( "iface %p stub!\n", iface );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_data_get_RoamingStorageQuota( IApplicationData *iface, UINT64 *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static const struct IApplicationDataVtbl application_data_vtbl =
{
    application_data_QueryInterface,
    application_data_AddRef,
    application_data_Release,
    /* IInspectable methods */
    application_data_GetIids,
    application_data_GetRuntimeClassName,
    application_data_GetTrustLevel,
    /* IApplicationData methods */
    application_data_get_Version,
    application_data_SetVersionAsync,
    application_data_ClearAllAsync,
    application_data_ClearAsync,
    application_data_get_LocalSettings,
    application_data_get_RoamingSettings,
    application_data_get_LocalFolder,
    application_data_get_RoamingFolder,
    application_data_get_TemporaryFolder,
    application_data_add_DataChanged,
    application_data_remove_DataChanged,
    application_data_SignalDataChanged,
    application_data_get_RoamingStorageQuota,
};

DEFINE_IINSPECTABLE( application_data_statics, IApplicationDataStatics, struct application_data_statics, IActivationFactory_iface )

static IApplicationData *g_current;

BOOL WINAPI DllMain( HINSTANCE hinst, DWORD reason, void *reserved )
{
    TRACE( "hinst %p, reason %ld, reserved %p.\n", hinst, reason, reserved );
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            struct application_data *impl;
            HSTRING_HEADER header;
            HSTRING string;

            if (!(impl = calloc( 1, sizeof(*impl) ))) return FALSE;
            impl->IApplicationData_iface.lpVtbl = &application_data_vtbl;
            impl->ref = 1;

            WindowsCreateStringReference( L"localSettings", wcslen( L"localSettings" ), &header, &string );
            if (FAILED(application_data_container_create( string, &impl->local_settings )))
            {
                free( impl );
                return FALSE;
            }

            WindowsCreateStringReference( L"roamingSettings", wcslen( L"roamingSettings" ), &header, &string );
            if (FAILED(application_data_container_create( string, &impl->roaming_settings )))
            {
                IApplicationDataContainer_Release( impl->local_settings );
                free( impl );
                return FALSE;
            }

            g_current = &impl->IApplicationData_iface;
            break;
        }
        case DLL_PROCESS_DETACH:
            if (g_current) IApplicationData_Release( g_current );
    }

    return TRUE;
}

static HRESULT WINAPI application_data_statics_get_Current( IApplicationDataStatics *iface, IApplicationData **value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_INVALIDARG;
    IApplicationData_AddRef( (*value = g_current) );
    return S_OK;
}

static const struct IApplicationDataStaticsVtbl application_data_statics_vtbl =
{
    application_data_statics_QueryInterface,
    application_data_statics_AddRef,
    application_data_statics_Release,
    /* IInspectable methods */
    application_data_statics_GetIids,
    application_data_statics_GetRuntimeClassName,
    application_data_statics_GetTrustLevel,
    /* IApplicationDataStatics methods */
    application_data_statics_get_Current,
};

static struct application_data_statics application_data_statics =
{
    {&factory_vtbl},
    {&application_data_statics_vtbl},
    1,
};

IActivationFactory *application_data_factory = &application_data_statics.IActivationFactory_iface;
