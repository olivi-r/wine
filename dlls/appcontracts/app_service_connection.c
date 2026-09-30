/*
 * Copyright (C) 2026 Olivia Ryan
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

WINE_DEFAULT_DEBUG_CHANNEL(appcontracts);

struct app_service_connection
{
    IAppServiceConnection IAppServiceConnection_iface;
    LONG ref;
};

static inline struct app_service_connection *impl_from_IAppServiceConnection( IAppServiceConnection *iface )
{
    return CONTAINING_RECORD( iface, struct app_service_connection, IAppServiceConnection_iface );
}

static HRESULT WINAPI app_service_connection_QueryInterface( IAppServiceConnection *iface, REFIID iid, void **out )
{
    struct app_service_connection *impl = impl_from_IAppServiceConnection( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IAppServiceConnection ))
    {
        IInspectable_AddRef( (*out = &impl->IAppServiceConnection_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI app_service_connection_AddRef( IAppServiceConnection *iface )
{
    struct app_service_connection *impl = impl_from_IAppServiceConnection( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI app_service_connection_Release( IAppServiceConnection *iface )
{
    struct app_service_connection *impl = impl_from_IAppServiceConnection( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI app_service_connection_GetIids( IAppServiceConnection *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_GetRuntimeClassName( IAppServiceConnection *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_GetTrustLevel( IAppServiceConnection *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_get_AppServiceName( IAppServiceConnection *iface, HSTRING *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_put_AppServiceName( IAppServiceConnection *iface, HSTRING value )
{
    FIXME( "iface %p, value %s stub!\n", iface, debugstr_hstring( value ) );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_get_PackageFamilyName( IAppServiceConnection *iface, HSTRING *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_put_PackageFamilyName( IAppServiceConnection *iface, HSTRING value )
{
    FIXME( "iface %p, value %s stub!\n", iface, debugstr_hstring( value ) );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_OpenAsync( IAppServiceConnection *iface, IAsyncOperation_AppServiceConnectionStatus **operation )
{
    FIXME( "iface %p, operation %p stub!\n", iface, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_SendMessageAsync( IAppServiceConnection *iface, IPropertySet *message, IAsyncOperation_AppServiceResponse **operation )
{
    FIXME( "iface %p, message %p, operation %p stub!\n", iface, message, operation );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_add_RequestReceived( IAppServiceConnection *iface, ITypedEventHandler_AppServiceConnection_AppServiceRequestReceivedEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_remove_RequestReceived( IAppServiceConnection *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_add_ServiceClosed( IAppServiceConnection *iface, ITypedEventHandler_AppServiceConnection_AppServiceClosedEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI app_service_connection_remove_ServiceClosed( IAppServiceConnection *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static const struct IAppServiceConnectionVtbl app_service_connection_vtbl =
{
    app_service_connection_QueryInterface,
    app_service_connection_AddRef,
    app_service_connection_Release,
    /* IInspectable methods */
    app_service_connection_GetIids,
    app_service_connection_GetRuntimeClassName,
    app_service_connection_GetTrustLevel,
    /* IAppServiceConnection methods */
    app_service_connection_get_AppServiceName,
    app_service_connection_put_AppServiceName,
    app_service_connection_get_PackageFamilyName,
    app_service_connection_put_PackageFamilyName,
    app_service_connection_OpenAsync,
    app_service_connection_SendMessageAsync,
    app_service_connection_add_RequestReceived,
    app_service_connection_remove_RequestReceived,
    app_service_connection_add_ServiceClosed,
    app_service_connection_remove_ServiceClosed,
};

struct app_service_connection_statics
{
    IActivationFactory IActivationFactory_iface;
    LONG ref;
};

static inline struct app_service_connection_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct app_service_connection_statics, IActivationFactory_iface );
}

static HRESULT WINAPI activation_factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct app_service_connection_statics *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IActivationFactory ))
    {
        IInspectable_AddRef( (*out = &impl->IActivationFactory_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI activation_factory_AddRef( IActivationFactory *iface )
{
    struct app_service_connection_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI activation_factory_Release( IActivationFactory *iface )
{
    struct app_service_connection_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    return ref;
}

static HRESULT WINAPI activation_factory_GetIids( IActivationFactory *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI activation_factory_GetRuntimeClassName( IActivationFactory *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI activation_factory_GetTrustLevel( IActivationFactory *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI activation_factory_ActivateInstance( IActivationFactory *iface, IInspectable **instance )
{
    struct app_service_connection *impl;

    TRACE( "iface %p, instance %p.\n", iface, instance );

    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->IAppServiceConnection_iface.lpVtbl = &app_service_connection_vtbl;
    impl->ref = 1;

    *instance = (IInspectable *)&impl->IAppServiceConnection_iface;
    return S_OK;
}

static const struct IActivationFactoryVtbl activation_factory_vtbl =
{
    activation_factory_QueryInterface,
    activation_factory_AddRef,
    activation_factory_Release,
    /* IInspectable methods */
    activation_factory_GetIids,
    activation_factory_GetRuntimeClassName,
    activation_factory_GetTrustLevel,
    /* IActivationFactory methods */
    activation_factory_ActivateInstance,
};

static struct app_service_connection_statics app_service_connection_statics =
{
    {&activation_factory_vtbl},
    1,
};

IActivationFactory *app_service_connection_factory = &app_service_connection_statics.IActivationFactory_iface;
