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

#include "initguid.h"
#include "wine/debug.h"
#define COBJMACROS
#include "appxpackaging.h"

WINE_DEFAULT_DEBUG_CHANNEL(appxpackaging);

struct appx_factory
{
    IClassFactory IClassFactory_iface;
    IAppxFactory IAppxFactory_iface;
    LONG ref;
};

static inline struct appx_factory *impl_from_IClassFactory( IClassFactory *iface )
{
    return CONTAINING_RECORD( iface, struct appx_factory, IClassFactory_iface );
}

static HRESULT WINAPI class_factory_QueryInterface( IClassFactory *iface, REFIID iid, void **out )
{
    struct appx_factory *impl = impl_from_IClassFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IClassFactory ))
    {
        IClassFactory_AddRef( (*out = &impl->IClassFactory_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI class_factory_AddRef( IClassFactory *iface )
{
    struct appx_factory *impl = impl_from_IClassFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI class_factory_Release( IClassFactory *iface )
{
    struct appx_factory *impl = impl_from_IClassFactory( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    return ref;
}

static HRESULT WINAPI class_factory_CreateInstance( IClassFactory *iface, IUnknown *outer, REFIID iid, void **out )
{
    struct appx_factory *impl = impl_from_IClassFactory( iface );
    TRACE( "iface %p, outer %p, iid %s, out %p.\n", iface, outer, debugstr_guid( iid ), out );
    if (outer) return CLASS_E_NOAGGREGATION;
    return IAppxFactory_QueryInterface( &impl->IAppxFactory_iface, iid, out );
}

static HRESULT WINAPI class_factory_LockServer( IClassFactory *iface, BOOL lock )
{
    FIXME( "iface %p, lock %d stub!\n", iface, lock );
    return E_NOTIMPL;
}

static const struct IClassFactoryVtbl class_factory_vtbl =
{
    class_factory_QueryInterface,
    class_factory_AddRef,
    class_factory_Release,
    /* IClassFactory methods */
    class_factory_CreateInstance,
    class_factory_LockServer,
};

static inline struct appx_factory *impl_from_IAppxFactory( IAppxFactory *iface )
{
    return CONTAINING_RECORD( iface, struct appx_factory, IAppxFactory_iface );
}

static HRESULT WINAPI appx_factory_QueryInterface( IAppxFactory *iface, REFIID iid, void **out )
{
    struct appx_factory *impl = impl_from_IAppxFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IAppxFactory ))
    {
        IAppxFactory_AddRef( (*out = &impl->IAppxFactory_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI appx_factory_AddRef( IAppxFactory *iface )
{
    struct appx_factory *impl = impl_from_IAppxFactory( iface );
    return IClassFactory_AddRef( &impl->IClassFactory_iface );
}

static ULONG WINAPI appx_factory_Release( IAppxFactory *iface )
{
    struct appx_factory *impl = impl_from_IAppxFactory( iface );
    return IClassFactory_Release( &impl->IClassFactory_iface );
}

static HRESULT WINAPI appx_factory_CreatePackageWriter( IAppxFactory *iface, IStream *output_stream,
                                                        APPX_PACKAGE_SETTINGS *settings, IAppxPackageWriter **value )
{
    FIXME( "iface %p, output_stream %p, settings %p, value %p stub!\n", iface, output_stream, settings, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_factory_CreatePackageReader( IAppxFactory *iface, IStream *input_stream,
                                                        IAppxPackageReader **value )
{
    FIXME( "iface %p, input_stream %p, value %p stub!\n", iface, input_stream, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_factory_CreateManifestReader( IAppxFactory *iface, IStream *input_stream,
                                                         IAppxManifestReader **value )
{
    FIXME( "iface %p, input_stream %p, value %p stub!\n", iface, input_stream, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_factory_CreateBlockMapReader( IAppxFactory *iface, IStream *input_stream,
                                                         IAppxBlockMapReader **value )
{
    FIXME( "iface %p, input_stream %p, value %p stub!\n", iface, input_stream, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_factory_CreateValidatedBlockMapReader( IAppxFactory *iface, IStream *input_stream,
                                                                  const WCHAR *signature_file_name,
                                                                  IAppxBlockMapReader **value )
{
    FIXME( "iface %p, input_stream %p, signature_file_name %s, value %p stub!\n",
           iface, input_stream, debugstr_w( signature_file_name ), value );
    return E_NOTIMPL;
}

static const struct IAppxFactoryVtbl appx_factory_vtbl =
{
    appx_factory_QueryInterface,
    appx_factory_AddRef,
    appx_factory_Release,
    /* IAppxFactory methods */
    appx_factory_CreatePackageWriter,
    appx_factory_CreatePackageReader,
    appx_factory_CreateManifestReader,
    appx_factory_CreateBlockMapReader,
    appx_factory_CreateValidatedBlockMapReader,
};

static struct appx_factory appx_factory =
{
    {&class_factory_vtbl},
    {&appx_factory_vtbl},
    1,
};

static IClassFactory *class_factory = &appx_factory.IClassFactory_iface;

HRESULT WINAPI DllGetClassObject( REFCLSID clsid, REFIID iid, void **out )
{
    TRACE( "clsid %s, iid %s, out %p.\n", debugstr_guid( clsid ), debugstr_guid( iid ), out );

    if (IsEqualGUID( clsid, &CLSID_AppxFactory ))
        return IClassFactory_QueryInterface( class_factory, iid, out );

    return CLASS_E_CLASSNOTAVAILABLE;
}
