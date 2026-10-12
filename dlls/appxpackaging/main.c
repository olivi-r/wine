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

struct factory
{
    IClassFactory IClassFactory_iface;
    IAppxEncryptionFactory IAppxEncryptionFactory_iface;
    LONG ref;
};

static inline struct factory *impl_from_IClassFactory( IClassFactory *iface )
{
    return CONTAINING_RECORD( iface, struct factory, IClassFactory_iface );
}

static HRESULT WINAPI class_factory_QueryInterface( IClassFactory *iface, REFIID iid, void **out )
{
    struct factory *impl = impl_from_IClassFactory( iface );

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
    struct factory *impl = impl_from_IClassFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI class_factory_Release( IClassFactory *iface )
{
    struct factory *impl = impl_from_IClassFactory( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    return ref;
}

static HRESULT WINAPI class_factory_CreateInstance( IClassFactory *iface, IUnknown *outer, REFIID iid, void **out )
{
    struct factory *impl = impl_from_IClassFactory( iface );
    TRACE( "iface %p, outer %p, iid %s, out %p.\n", iface, outer, debugstr_guid( iid ), out );
    if (outer) return CLASS_E_NOAGGREGATION;
    if (IsEqualGUID( iid, &IID_IAppxEncryptionFactory ))
        return IAppxEncryptionFactory_QueryInterface( &impl->IAppxEncryptionFactory_iface, iid, out );
    return E_NOINTERFACE;
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

static inline struct factory *impl_from_IAppxEncryptionFactory( IAppxEncryptionFactory *iface )
{
    return CONTAINING_RECORD( iface, struct factory, IAppxEncryptionFactory_iface );
}

static HRESULT WINAPI appx_encryption_factory_QueryInterface( IAppxEncryptionFactory *iface, REFIID iid, void **out )
{
    struct factory *impl = impl_from_IAppxEncryptionFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IAppxEncryptionFactory ))
    {
        IAppxEncryptionFactory_AddRef( (*out = &impl->IAppxEncryptionFactory_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI appx_encryption_factory_AddRef( IAppxEncryptionFactory *iface )
{
    struct factory *impl = impl_from_IAppxEncryptionFactory( iface );
    return IClassFactory_AddRef( &impl->IClassFactory_iface );
}

static ULONG WINAPI appx_encryption_factory_Release( IAppxEncryptionFactory *iface )
{
    struct factory *impl = impl_from_IAppxEncryptionFactory( iface );
    return IClassFactory_Release( &impl->IClassFactory_iface );
}

static HRESULT WINAPI appx_encryption_factory_EncryptPackage( IAppxEncryptionFactory *iface, IStream *input_stream,
                          IStream *output_stream, const APPX_ENCRYPTED_PACKAGE_SETTINGS *settings,
                          const APPX_KEY_INFO *key_info, const APPX_ENCRYPTED_EXEMPTIONS *exempted_files )
{
    FIXME( "iface %p, input_stream %p, output_stream %p, settings %p, key_info %p, exempted_files %p stub!\n",
           iface, input_stream, output_stream, settings, key_info, exempted_files );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_encryption_factory_DecryptPackage( IAppxEncryptionFactory *iface, IStream *input_stream,
                          IStream *output_stream, const APPX_KEY_INFO *key_info )
{
    FIXME( "iface %p, input_stream %p, output_stream %p, key_info %p stub!\n",
           iface, input_stream, output_stream, key_info );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_encryption_factory_CreateEncryptedPackageWriter( IAppxEncryptionFactory *iface,
                          IStream *output_stream, IStream *manifest_stream,
                          const APPX_ENCRYPTED_PACKAGE_SETTINGS *settings, const APPX_KEY_INFO *key_info,
                          const APPX_ENCRYPTED_EXEMPTIONS *exempted_files, IAppxEncryptedPackageWriter **value )
{
    FIXME( "iface %p, output_stream %p, manifest_stream %p, settings %p, key_info %p, exempted_files %p, value %p stub!\n",
           iface, output_stream, manifest_stream, settings, key_info, exempted_files, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_encryption_factory_CreateEncryptedPackageReader( IAppxEncryptionFactory *iface,
                          IStream *input_stream, const APPX_KEY_INFO *key_info, IAppxPackageReader **value )
{
    FIXME( "iface %p, input_stream %p, key_info %p, value %p stub!\n", iface, input_stream, key_info, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_encryption_factory_EncryptBundle( IAppxEncryptionFactory *iface, IStream *input_stream,
                          IStream *output_stream, const APPX_ENCRYPTED_PACKAGE_SETTINGS *settings,
                          const APPX_KEY_INFO *key_info, const APPX_ENCRYPTED_EXEMPTIONS *exempted_files )
{
    FIXME( "iface %p, input_stream %p, output_stream %p, settings %p, key_info %p, exempted_files %p stub!\n",
           iface, input_stream, output_stream, settings, key_info, exempted_files );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_encryption_factory_DecryptBundle( IAppxEncryptionFactory *iface, IStream *input_stream,
                          IStream *output_stream, const APPX_KEY_INFO *key_info )
{
    FIXME( "iface %p, input_stream %p, output_stream %p, key_info %p stub!\n",
           iface, input_stream, output_stream, key_info );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_encryption_factory_CreateEncryptedBundleWriter( IAppxEncryptionFactory *iface,
                          IStream *output_stream, UINT64 bundle_version,
                          const APPX_ENCRYPTED_PACKAGE_SETTINGS *settings, const APPX_KEY_INFO *key_info,
                          const APPX_ENCRYPTED_EXEMPTIONS *exempted_files, IAppxEncryptedBundleWriter **value )
{
    FIXME( "iface %p, output_stream %p, bundle_version %llu, settings %p, key_info %p, exempted_files %p, value %p stub!\n",
           iface, output_stream, bundle_version, settings, key_info, exempted_files, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_encryption_factory_CreateEncryptedBundleReader( IAppxEncryptionFactory *iface,
                          IStream *input_stream, const APPX_KEY_INFO *key_info, IAppxBundleReader **value )
{
    FIXME( "iface %p, input_stream %p, key_info %p, value %p stub!\n", iface, input_stream, key_info, value );
    return E_NOTIMPL;
}

static const struct IAppxEncryptionFactoryVtbl appx_encryption_factory_vtbl =
{
    appx_encryption_factory_QueryInterface,
    appx_encryption_factory_AddRef,
    appx_encryption_factory_Release,
    /* IAppxEncryptionFactory methods */
    appx_encryption_factory_EncryptPackage,
    appx_encryption_factory_DecryptPackage,
    appx_encryption_factory_CreateEncryptedPackageWriter,
    appx_encryption_factory_CreateEncryptedPackageReader,
    appx_encryption_factory_EncryptBundle,
    appx_encryption_factory_DecryptBundle,
    appx_encryption_factory_CreateEncryptedBundleWriter,
    appx_encryption_factory_CreateEncryptedBundleReader,
};

static struct factory factory =
{
    {&class_factory_vtbl},
    {&appx_encryption_factory_vtbl},
    1,
};

HRESULT WINAPI DllGetClassObject( REFCLSID clsid, REFIID iid, void **out )
{
    TRACE( "clsid %s, iid %s, out %p.\n", debugstr_guid( clsid ), debugstr_guid( iid ), out );
    if (IsEqualGUID( clsid, &CLSID_AppxEncryptionFactory ))
        return IClassFactory_QueryInterface( &factory.IClassFactory_iface, iid, out );
    return CLASS_E_CLASSNOTAVAILABLE;
}
