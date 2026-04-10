/*
 * Copyright (c) 2026 Olivia Ryan
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

WINE_DEFAULT_DEBUG_CHANNEL(msxml6);

struct load_settings
{
    IXmlLoadSettings IXmlLoadSettings_iface;
    LONG ref;
    UINT32 max_element_depth;
    boolean prohibit_dtd;
    boolean resolve_externals;
    boolean validate_on_parse;
    boolean element_content_white_space;
};

static inline struct load_settings *impl_from_IXmlLoadSettings( IXmlLoadSettings *iface )
{
    return CONTAINING_RECORD( iface, struct load_settings, IXmlLoadSettings_iface );
}

static HRESULT WINAPI load_settings_QueryInterface( IXmlLoadSettings *iface, REFIID iid, void **out )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid(iid), out);

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IXmlLoadSettings ))
    {
        IInspectable_AddRef( *out = &impl->IXmlLoadSettings_iface );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI load_settings_AddRef( IXmlLoadSettings *iface )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI load_settings_Release( IXmlLoadSettings *iface )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI load_settings_GetIids( IXmlLoadSettings *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI load_settings_GetRuntimeClassName( IXmlLoadSettings *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI load_settings_GetTrustLevel( IXmlLoadSettings *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI load_settings_get_MaxElementDepth( IXmlLoadSettings *iface, UINT32 *value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = impl->max_element_depth;
    return S_OK;
}

static HRESULT WINAPI load_settings_put_MaxElementDepth( IXmlLoadSettings *iface, UINT32 value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %u.\n", iface, value );
    impl->max_element_depth = value;
    return S_OK;
}

static HRESULT WINAPI load_settings_get_ProhibitDtd( IXmlLoadSettings *iface, boolean *value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = impl->prohibit_dtd;
    return S_OK;
}

static HRESULT WINAPI load_settings_put_ProhibitDtd( IXmlLoadSettings *iface, boolean value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %d.\n", iface, value );
    impl->prohibit_dtd = value;
    return S_OK;
}

static HRESULT WINAPI load_settings_get_ResolveExternals( IXmlLoadSettings *iface, boolean *value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = impl->resolve_externals;
    return S_OK;
}

static HRESULT WINAPI load_settings_put_ResolveExternals( IXmlLoadSettings *iface, boolean value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %d.\n", iface, value );
    impl->resolve_externals = value;
    return S_OK;
}

static HRESULT WINAPI load_settings_get_ValidateOnParse( IXmlLoadSettings *iface, boolean *value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = impl->validate_on_parse;
    return S_OK;
}

static HRESULT WINAPI load_settings_put_ValidateOnParse( IXmlLoadSettings *iface, boolean value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %d.\n", iface, value );
    impl->validate_on_parse = value;
    return S_OK;
}

static HRESULT WINAPI load_settings_get_ElementContentWhiteSpace( IXmlLoadSettings *iface, boolean *value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    *value = impl->element_content_white_space;
    return S_OK;
}

static HRESULT WINAPI load_settings_put_ElementContentWhiteSpace( IXmlLoadSettings *iface, boolean value )
{
    struct load_settings *impl = impl_from_IXmlLoadSettings( iface );
    TRACE( "iface %p, value %d.\n", iface, value );
    impl->element_content_white_space = value;
    return S_OK;
}

static const struct IXmlLoadSettingsVtbl load_settings_vtbl =
{
    load_settings_QueryInterface,
    load_settings_AddRef,
    load_settings_Release,
    /* IInspectable methods */
    load_settings_GetIids,
    load_settings_GetRuntimeClassName,
    load_settings_GetTrustLevel,
    /* IXmlLoadSettings methods */
    load_settings_get_MaxElementDepth,
    load_settings_put_MaxElementDepth,
    load_settings_get_ProhibitDtd,
    load_settings_put_ProhibitDtd,
    load_settings_get_ResolveExternals,
    load_settings_put_ResolveExternals,
    load_settings_get_ValidateOnParse,
    load_settings_put_ValidateOnParse,
    load_settings_get_ElementContentWhiteSpace,
    load_settings_put_ElementContentWhiteSpace,
};

struct factory
{
    IActivationFactory IActivationFactory_iface;
    LONG ref;
};

static inline struct factory *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct factory, IActivationFactory_iface );
}

static HRESULT WINAPI factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct factory *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid(iid), out);

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IActivationFactory ))
    {
        IInspectable_AddRef( *out = &impl->IActivationFactory_iface );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI factory_AddRef( IActivationFactory *iface )
{
    struct factory *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI factory_Release( IActivationFactory *iface )
{
    struct factory *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
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
    struct load_settings *impl;

    TRACE( "iface %p, instance %p.\n", iface, instance );

    *instance = NULL;
    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;

    impl->IXmlLoadSettings_iface.lpVtbl = &load_settings_vtbl;
    impl->ref = 1;

    impl->max_element_depth = 256;
    impl->prohibit_dtd = TRUE;
    impl->resolve_externals = FALSE;
    impl->validate_on_parse = FALSE;
    impl->element_content_white_space = TRUE;

    *instance = (IInspectable *)&impl->IXmlLoadSettings_iface;
    return S_OK;
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

static struct factory factory =
{
    {&factory_vtbl},
    1,
};

IActivationFactory *xml_load_settings_factory = &factory.IActivationFactory_iface;
