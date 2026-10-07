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
#include "appmodel.h"
#define COBJMACROS
#include "appxpackaging.h"
#include "xmllite.h"

WINE_DEFAULT_DEBUG_CHANNEL(appxpackaging);

struct appx_manifest_reader
{
    IAppxManifestReader IAppxManifestReader_iface;
    IAppxManifestPackageId IAppxManifestPackageId_iface;
    LONG ref;

    APPX_PACKAGE_ARCHITECTURE arch;
    PACKAGE_VERSION version;
    WCHAR name[PACKAGE_NAME_MAX_LENGTH + 1];
    WCHAR publisher[PACKAGE_PUBLISHER_MAX_LENGTH + 1];
    WCHAR resource[PACKAGE_RESOURCEID_MAX_LENGTH + 1];
};

static inline struct appx_manifest_reader *impl_from_IAppxManifestReader( IAppxManifestReader *iface )
{
    return CONTAINING_RECORD( iface, struct appx_manifest_reader, IAppxManifestReader_iface );
}

static HRESULT WINAPI appx_manifest_reader_QueryInterface( IAppxManifestReader *iface, REFIID iid, void **out )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestReader( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IAppxManifestReader ))
    {
        IClassFactory_AddRef( (*out = &impl->IAppxManifestReader_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI appx_manifest_reader_AddRef( IAppxManifestReader *iface )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestReader( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI appx_manifest_reader_Release( IAppxManifestReader *iface )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestReader( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI appx_manifest_reader_GetPackageId( IAppxManifestReader *iface, IAppxManifestPackageId **value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestReader( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    return IAppxManifestPackageId_QueryInterface( &impl->IAppxManifestPackageId_iface,
                                                  &IID_IAppxManifestPackageId, (void **)value );
}

static HRESULT WINAPI appx_manifest_reader_GetProperties( IAppxManifestReader *iface, IAppxManifestProperties **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_manifest_reader_GetPackageDependencies( IAppxManifestReader *iface,
                                                                   IAppxManifestPackageDependenciesEnumerator **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_manifest_reader_GetCapabilities( IAppxManifestReader *iface, APPX_CAPABILITIES *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_manifest_reader_GetResources( IAppxManifestReader *iface,
                                                         IAppxManifestResourcesEnumerator **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_manifest_reader_GetDeviceCapabilities( IAppxManifestReader *iface,
                                                                  IAppxManifestDeviceCapabilitiesEnumerator **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_manifest_reader_GetPrerequisite( IAppxManifestReader *iface,
                                                            const WCHAR *name, UINT64 *value )
{
    FIXME( "iface %p, name %s, value %p stub!\n", iface, debugstr_w( name ), value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_manifest_reader_GetApplications( IAppxManifestReader *iface,
                                                            IAppxManifestApplicationsEnumerator **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI appx_manifest_reader_GetStream( IAppxManifestReader *iface, IStream **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static const struct IAppxManifestReaderVtbl appx_manifest_reader_vtbl =
{
    appx_manifest_reader_QueryInterface,
    appx_manifest_reader_AddRef,
    appx_manifest_reader_Release,
    /* IAppxManifestReader methods */
    appx_manifest_reader_GetPackageId,
    appx_manifest_reader_GetProperties,
    appx_manifest_reader_GetPackageDependencies,
    appx_manifest_reader_GetCapabilities,
    appx_manifest_reader_GetResources,
    appx_manifest_reader_GetDeviceCapabilities,
    appx_manifest_reader_GetPrerequisite,
    appx_manifest_reader_GetApplications,
    appx_manifest_reader_GetStream,
};

static inline struct appx_manifest_reader *impl_from_IAppxManifestPackageId( IAppxManifestPackageId *iface )
{
    return CONTAINING_RECORD( iface, struct appx_manifest_reader, IAppxManifestPackageId_iface );
}

static HRESULT WINAPI appx_manifest_package_id_QueryInterface( IAppxManifestPackageId *iface, REFIID iid, void **out )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IAppxManifestPackageId ))
    {
        IClassFactory_AddRef( (*out = &impl->IAppxManifestPackageId_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI appx_manifest_package_id_AddRef( IAppxManifestPackageId *iface )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    return IAppxManifestReader_AddRef( &impl->IAppxManifestReader_iface );
}

static ULONG WINAPI appx_manifest_package_id_Release( IAppxManifestPackageId *iface )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    return IAppxManifestReader_Release( &impl->IAppxManifestReader_iface );
}

static HRESULT WINAPI appx_manifest_package_id_GetName( IAppxManifestPackageId *iface, WCHAR **value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    if (!wcslen( impl->name )) *value = NULL;
    else
    {
        *value = CoTaskMemAlloc( (wcslen( impl->name ) + 1) * sizeof(WCHAR) );
        wcscpy( *value, impl->name );
    }
    return S_OK;
}

static HRESULT WINAPI appx_manifest_package_id_GetArchitecture( IAppxManifestPackageId *iface,
                                                                APPX_PACKAGE_ARCHITECTURE *value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    *value = impl->arch;
    return S_OK;
}

static HRESULT WINAPI appx_manifest_package_id_GetPublisher( IAppxManifestPackageId *iface, WCHAR **value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    if (!wcslen( impl->publisher )) *value = NULL;
    else
    {
        *value = CoTaskMemAlloc( (wcslen( impl->publisher ) + 1) * sizeof(WCHAR) );
        wcscpy( *value, impl->publisher );
    }
    return S_OK;
}

static HRESULT WINAPI appx_manifest_package_id_GetVersion( IAppxManifestPackageId *iface, UINT64 *value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    *value = impl->version.Version;
    return S_OK;
}

static HRESULT WINAPI appx_manifest_package_id_GetResourceId( IAppxManifestPackageId *iface, WCHAR **value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    if (!wcslen( impl->resource )) *value = NULL;
    else
    {
        *value = CoTaskMemAlloc( (wcslen( impl->resource ) + 1) * sizeof(WCHAR) );
        wcscpy( *value, impl->resource );
    }
    return S_OK;
}

static HRESULT WINAPI appx_manifest_package_id_ComparePublisher( IAppxManifestPackageId *iface,
                                                                 const WCHAR *other, BOOL *value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    TRACE( "iface %p, other %s, value %p.\n", iface, debugstr_w( other ), value );
    if (!other || !value) return E_POINTER;
    *value = !wcscmp( impl->publisher, other );
    return S_OK;
}

static HRESULT WINAPI appx_manifest_package_id_GetPackageFullName( IAppxManifestPackageId *iface, WCHAR **value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    UINT32 length = 0;

    const PACKAGE_ID id =
    {
        .processorArchitecture = impl->arch,
        .resourceId = impl->resource,
        .publisher = impl->publisher,
        .version = impl->version,
        .name = impl->name,
    };

    TRACE( "iface %p, value %p.\n", iface, value );

    if (!value) return E_POINTER;
    PackageFullNameFromId( &id, &length, NULL );
    *value = CoTaskMemAlloc( length * sizeof(WCHAR) );
    return HRESULT_FROM_WIN32( PackageFullNameFromId( &id, &length, *value ));
}

static HRESULT WINAPI appx_manifest_package_id_GetPackageFamilyName( IAppxManifestPackageId *iface, WCHAR **value )
{
    struct appx_manifest_reader *impl = impl_from_IAppxManifestPackageId( iface );
    UINT32 length = 0;

    const PACKAGE_ID id =
    {
        .publisher = impl->publisher,
        .name = impl->name,
    };

    TRACE( "iface %p, value %p.\n", iface, value );

    if (!value) return E_POINTER;
    PackageFamilyNameFromId( &id, &length, NULL );
    *value = CoTaskMemAlloc( length * sizeof(WCHAR) );
    return HRESULT_FROM_WIN32( PackageFamilyNameFromId( &id, &length, *value ));
}

static const struct IAppxManifestPackageIdVtbl appx_manifest_package_id_vtbl =
{
    appx_manifest_package_id_QueryInterface,
    appx_manifest_package_id_AddRef,
    appx_manifest_package_id_Release,
    /* IAppxManifestPackageId methods */
    appx_manifest_package_id_GetName,
    appx_manifest_package_id_GetArchitecture,
    appx_manifest_package_id_GetPublisher,
    appx_manifest_package_id_GetVersion,
    appx_manifest_package_id_GetResourceId,
    appx_manifest_package_id_ComparePublisher,
    appx_manifest_package_id_GetPackageFullName,
    appx_manifest_package_id_GetPackageFamilyName,
};

static const struct
{
    UINT32 code;
    const WCHAR *name;
}
arch_names[] =
{
    {PROCESSOR_ARCHITECTURE_INTEL,   L"x86"},
    {PROCESSOR_ARCHITECTURE_ARM,     L"arm"},
    {PROCESSOR_ARCHITECTURE_AMD64,   L"x64"},
    {PROCESSOR_ARCHITECTURE_NEUTRAL, L"neutral"},
    {PROCESSOR_ARCHITECTURE_ARM64,   L"arm64"},
};

static HRESULT appx_manifest_reader_create( IStream *input_stream, IAppxManifestReader **manifest_reader )
{
    WCHAR arch_str[PACKAGE_ARCHITECTURE_MAX_LENGTH + 1] = {};
    WCHAR version_str[PACKAGE_VERSION_MAX_LENGTH + 1] = {};
    WCHAR publisher[PACKAGE_PUBLISHER_MAX_LENGTH + 1] = {};
    WCHAR resource[PACKAGE_RESOURCEID_MAX_LENGTH + 1] = {};
    WCHAR name[PACKAGE_NAME_MAX_LENGTH + 1] = {};
    struct appx_manifest_reader *impl;
    IXmlReader *xml_reader = NULL;
    PACKAGE_VERSION version;
    XmlNodeType type;
    UINT32 arch = -1;
    HRESULT hr;
    WCHAR *ptr;

    TRACE( "input_stream %p, manifest_reader %p.\n", input_stream, manifest_reader );

    if (!input_stream || !manifest_reader) return E_POINTER;
    if (FAILED(hr = CreateXmlReader( &IID_IXmlReader, (void **)&xml_reader, NULL ))) return hr;
    if (FAILED(hr = IXmlReader_SetInput( xml_reader, (IUnknown *)input_stream )) ||
        FAILED(hr = IXmlReader_GetNodeType( xml_reader, &type ))) goto end;
    while (!IXmlReader_IsEOF( xml_reader ))
    {
        const WCHAR *node, *namespace;
        UINT num_attrs;

        if (FAILED(hr = IXmlReader_GetNodeType( xml_reader, &type ))) goto end;
        if (type != XmlNodeType_Element) goto loop;
        if (FAILED(hr = IXmlReader_GetNamespaceUri( xml_reader, &namespace, NULL )) ||
            FAILED(hr = IXmlReader_GetAttributeCount( xml_reader, &num_attrs )) ||
            FAILED(hr = IXmlReader_GetLocalName( xml_reader, &node, NULL ))) goto end;
        if (wcscmp( namespace, L"http://schemas.microsoft.com/appx/manifest/foundation/windows10" ) ||
            wcscmp( node, L"Identity" )) goto loop;
        for (UINT i = 0; i < num_attrs; i++)
        {
            const WCHAR *value;
            if (FAILED(hr = IXmlReader_MoveToNextAttribute( xml_reader )) ||
                FAILED(hr = IXmlReader_GetLocalName( xml_reader, &node, NULL )) ||
                FAILED(hr = IXmlReader_GetValue( xml_reader, &value, NULL ))) goto end;
            if (!wcscmp( node, L"ProcessorArchitecture" )) wcscpy( arch_str, value );
            if (!wcscmp( node, L"ResourceId" )) wcscpy( resource, value );
            if (!wcscmp( node, L"Publisher" )) wcscpy( publisher, value );
            if (!wcscmp( node, L"Version" )) wcscpy( version_str, value );
            if (!wcscmp( node, L"Name" )) wcscpy( name, value );
            TRACE( "name %s, value %s, hr %#lx\n", debugstr_w( node ), debugstr_w( value ), hr );
        }

    loop:
        if (FAILED(hr = IXmlReader_Read( xml_reader, NULL ))) goto end;
        if (hr == S_FALSE)
        {
            /* eof */
            hr = S_OK;
            break;
        }
    }

    for (UINT i = 0; i < ARRAY_SIZE( arch_names ); i++)
    {
        if (wcscmp( arch_str, arch_names[i].name )) continue;
        arch = arch_names[i].code;
    }

    version.Major = wcstol( version_str, NULL, 10 );
    if (!(ptr = wcschr( version_str, L'.' ))) return APPX_E_INVALID_MANIFEST;
    version.Minor = wcstol( ++ptr, NULL, 10 );
    if (!(ptr = wcschr( ptr, L'.' ))) return APPX_E_INVALID_MANIFEST;
    version.Build = wcstol( ++ptr, NULL, 10 );
    if (!(ptr = wcschr( ptr, L'.' ))) return APPX_E_INVALID_MANIFEST;
    version.Revision = wcstol( ++ptr, NULL, 10 );

    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->IAppxManifestReader_iface.lpVtbl = &appx_manifest_reader_vtbl;
    impl->IAppxManifestPackageId_iface.lpVtbl = &appx_manifest_package_id_vtbl;
    impl->ref = 1;

    impl->arch = arch;
    impl->version = version;
    wcscpy( impl->name, name );
    wcscpy( impl->resource, resource );
    wcscpy( impl->publisher, publisher );

    *manifest_reader = &impl->IAppxManifestReader_iface;
end:
    IXmlReader_Release( xml_reader );
    return hr;
}

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
    TRACE( "iface %p, input_stream %p, value %p.\n", iface, input_stream, value );
    return appx_manifest_reader_create( input_stream, value );
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
