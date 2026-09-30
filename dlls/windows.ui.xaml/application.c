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

WINE_DEFAULT_DEBUG_CHANNEL(xaml);

struct application
{
    IInspectable IInspectable_iface;
    IApplication IApplication_iface;
    IInspectable *IInspectable_outer;
    LONG ref;
};

static inline struct application *impl_from_IInspectable( IInspectable *iface )
{
    return CONTAINING_RECORD( iface, struct application, IInspectable_iface );
}

static HRESULT WINAPI inspectable_QueryInterface( IInspectable *iface, REFIID iid, void **out )
{
    struct application *impl = impl_from_IInspectable( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ))
    {
        IInspectable_AddRef( (*out = &impl->IInspectable_iface) );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_IApplication ))
    {
        IInspectable_AddRef( (*out = &impl->IApplication_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI inspectable_AddRef( IInspectable *iface )
{
    struct application *impl = impl_from_IInspectable( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI inspectable_Release( IInspectable *iface )
{
    struct application *impl = impl_from_IInspectable( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI inspectable_GetIids( IInspectable *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI inspectable_GetRuntimeClassName( IInspectable *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI inspectable_GetTrustLevel( IInspectable *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static const struct IInspectableVtbl inspectable_vtbl =
{
    inspectable_QueryInterface,
    inspectable_AddRef,
    inspectable_Release,
    /* IInspectable methods */
    inspectable_GetIids,
    inspectable_GetRuntimeClassName,
    inspectable_GetTrustLevel,
};

DEFINE_IINSPECTABLE_OUTER( application, IApplication, struct application, IInspectable_outer )

static HRESULT WINAPI application_get_Resources( IApplication *iface, IResourceDictionary **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_put_Resources( IApplication *iface, IResourceDictionary *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_get_DebugSettings( IApplication *iface, IDebugSettings **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_get_RequestedTheme( IApplication *iface, ApplicationTheme *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_put_RequestedTheme( IApplication *iface, ApplicationTheme value )
{
    FIXME( "iface %p, value %d stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_add_UnhandledException( IApplication *iface, IUnhandledExceptionEventHandler *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_remove_UnhandledException( IApplication *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_add_Suspending( IApplication *iface, ISuspendingEventHandler *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_remove_Suspending( IApplication *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_add_Resuming( IApplication *iface, IEventHandler_IInspectable *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_remove_Resuming( IApplication *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static const struct IApplicationVtbl application_vtbl =
{
    application_QueryInterface,
    application_AddRef,
    application_Release,
    /* IInspectable methods */
    application_GetIids,
    application_GetRuntimeClassName,
    application_GetTrustLevel,
    /* IApplication methods */
    application_get_Resources,
    application_put_Resources,
    application_get_DebugSettings,
    application_get_RequestedTheme,
    application_put_RequestedTheme,
    application_add_UnhandledException,
    application_remove_UnhandledException,
    application_add_Suspending,
    application_remove_Suspending,
    application_add_Resuming,
    application_remove_Resuming,
};

struct application_statics
{
    IActivationFactory IActivationFactory_iface;
    IApplicationFactory IApplicationFactory_iface;
    LONG ref;
};

static inline struct application_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct application_statics, IActivationFactory_iface );
}

static HRESULT WINAPI activation_factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct application_statics *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IActivationFactory ))
    {
        IInspectable_AddRef( (*out = &impl->IActivationFactory_iface) );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_IApplicationFactory ))
    {
        IInspectable_AddRef( (*out = &impl->IApplicationFactory_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI activation_factory_AddRef( IActivationFactory *iface )
{
    struct application_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI activation_factory_Release( IActivationFactory *iface )
{
    struct application_statics *impl = impl_from_IActivationFactory( iface );
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
    FIXME( "iface %p, instance %p stub!\n", iface, instance );
    return E_NOTIMPL;
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

DEFINE_IINSPECTABLE( application_factory, IApplicationFactory, struct application_statics, IActivationFactory_iface )

static HRESULT WINAPI application_factory_CreateInstance( IApplicationFactory *iface, IInspectable *base_interface, IInspectable **inner_interface, IApplication **value )
{
    struct application *impl;

    TRACE( "iface %p, base_interface %p, inner_interface %p, value %p.\n", iface, base_interface, inner_interface, value );

    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->IInspectable_iface.lpVtbl = &inspectable_vtbl;
    impl->IApplication_iface.lpVtbl = &application_vtbl;
    impl->IInspectable_outer = base_interface ? base_interface : &impl->IInspectable_iface;
    impl->ref = 1;

    if (inner_interface) IInspectable_AddRef( (*inner_interface = &impl->IInspectable_iface) );
    *value = &impl->IApplication_iface;
    return S_OK;
}

static const struct IApplicationFactoryVtbl application_factory_vtbl =
{
    application_factory_QueryInterface,
    application_factory_AddRef,
    application_factory_Release,
    /* IInspectable methods */
    application_factory_GetIids,
    application_factory_GetRuntimeClassName,
    application_factory_GetTrustLevel,
    /* IApplicationFactory methods */
    application_factory_CreateInstance,
};

static struct application_statics application_statics =
{
    {&activation_factory_vtbl},
    {&application_factory_vtbl},
    1,
};

IActivationFactory *application_factory = &application_statics.IActivationFactory_iface;
