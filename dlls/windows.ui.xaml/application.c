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
    IApplication2 IApplication2_iface;
    IApplication3 IApplication3_iface;
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

    if (IsEqualGUID( iid, &IID_IApplication2 ))
    {
        IInspectable_AddRef( (*out = &impl->IApplication2_iface) );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_IApplication3 ))
    {
        IInspectable_AddRef( (*out = &impl->IApplication3_iface) );
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

DEFINE_IINSPECTABLE_OUTER( application2, IApplication2, struct application, IInspectable_outer )

static HRESULT WINAPI application2_get_FocusVisualKind( IApplication2 *iface, FocusVisualKind *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application2_put_FocusVisualKind( IApplication2 *iface, FocusVisualKind value )
{
    FIXME( "iface %p, value %d stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application2_get_RequiresPointerMode( IApplication2 *iface, ApplicationRequiresPointerMode *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application2_put_RequiresPointerMode( IApplication2 *iface, ApplicationRequiresPointerMode value )
{
    FIXME( "iface %p, value %d stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application2_add_LeavingBackground( IApplication2 *iface, ILeavingBackgroundEventHandler *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI application2_remove_LeavingBackground( IApplication2 *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI application2_add_EnteredBackground( IApplication2 *iface, IEnteredBackgroundEventHandler *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI application2_remove_EnteredBackground( IApplication2 *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static const struct IApplication2Vtbl application2_vtbl =
{
    application2_QueryInterface,
    application2_AddRef,
    application2_Release,
    /* IInspectable methods */
    application2_GetIids,
    application2_GetRuntimeClassName,
    application2_GetTrustLevel,
    /* IApplication2 methods */
    application2_get_FocusVisualKind,
    application2_put_FocusVisualKind,
    application2_get_RequiresPointerMode,
    application2_put_RequiresPointerMode,
    application2_add_LeavingBackground,
    application2_remove_LeavingBackground,
    application2_add_EnteredBackground,
    application2_remove_EnteredBackground,
};

DEFINE_IINSPECTABLE_OUTER( application3, IApplication3, struct application, IInspectable_outer )

static HRESULT WINAPI application3_get_HighContrastAdjustment( IApplication3 *iface, ApplicationHighContrastAdjustment *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application3_put_HighContrastAdjustment( IApplication3 *iface, ApplicationHighContrastAdjustment value )
{
    FIXME( "iface %p, value %d stub!\n", iface, value );
    return E_NOTIMPL;
}

static const struct IApplication3Vtbl application3_vtbl =
{
    application3_QueryInterface,
    application3_AddRef,
    application3_Release,
    /* IInspectable methods */
    application3_GetIids,
    application3_GetRuntimeClassName,
    application3_GetTrustLevel,
    /* IApplication2 methods */
    application3_get_HighContrastAdjustment,
    application3_put_HighContrastAdjustment,
};

struct application_statics
{
    IActivationFactory IActivationFactory_iface;
    IApplicationFactory IApplicationFactory_iface;
    IApplicationStatics IApplicationStatics_iface;
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

    if (IsEqualGUID( iid, &IID_IApplicationStatics ))
    {
        IInspectable_AddRef( (*out = &impl->IApplicationStatics_iface) );
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
    impl->IApplication2_iface.lpVtbl = &application2_vtbl;
    impl->IApplication3_iface.lpVtbl = &application3_vtbl;
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

DEFINE_IINSPECTABLE( application_statics, IApplicationStatics, struct application_statics, IActivationFactory_iface )

static HRESULT WINAPI application_statics_get_Current( IApplicationStatics *iface, IApplication **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_statics_Start( IApplicationStatics *iface, IApplicationInitializationCallback *callback )
{
    FIXME( "iface %p, callback %p stub!\n", iface, callback );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_statics_LoadComponent( IApplicationStatics *iface, IInspectable *component, IUriRuntimeClass *resource_locator )
{
    FIXME( "iface %p, component %p, resource_locator %p stub!\n", iface, component, resource_locator );
    return E_NOTIMPL;
}

static HRESULT WINAPI application_statics_LoadComponentWithResourceLocation( IApplicationStatics *iface, IInspectable *component, IUriRuntimeClass *resource_locator, ComponentResourceLocation component_resource_location )
{
    FIXME( "iface %p, component %p, resource_locator %p, component_resource_location %d stub!\n", iface, component, resource_locator, component_resource_location );
    return E_NOTIMPL;
}

static const struct IApplicationStaticsVtbl application_statics_vtbl =
{
    application_statics_QueryInterface,
    application_statics_AddRef,
    application_statics_Release,
    /* IInspectable methods */
    application_statics_GetIids,
    application_statics_GetRuntimeClassName,
    application_statics_GetTrustLevel,
    /* IApplicationStatics methods */
    application_statics_get_Current,
    application_statics_Start,
    application_statics_LoadComponent,
    application_statics_LoadComponentWithResourceLocation,
};

static struct application_statics application_statics =
{
    {&activation_factory_vtbl},
    {&application_factory_vtbl},
    {&application_statics_vtbl},
    1,
};

IActivationFactory *application_factory = &application_statics.IActivationFactory_iface;
