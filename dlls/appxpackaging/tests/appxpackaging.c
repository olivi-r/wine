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

#define COBJMACROS
#include "initguid.h"
#include "appxpackaging.h"
#include "shlwapi.h"
#include "wine/test.h"

#define check_interface( iface, iid ) check_interface_( __LINE__, iface, iid )
static void check_interface_( unsigned int line, void *iface, REFIID iid )
{
    IUnknown *unknown = iface, *out;
    HRESULT hr;

    hr = IUnknown_QueryInterface( unknown, iid, (void **)&out );
    ok_(__FILE__, line)( hr == S_OK, "got hr %#lx.\n", hr );
    if (hr == S_OK) IUnknown_Release( out );
}

static void test_AppxFactory(void)
{
    IAppxManifestReader *manifest_reader = (void *)0xdeadbeef;
    IAppxManifestPackageId *manifest_package_id;
    APPX_PACKAGE_ARCHITECTURE arch;
    IClassFactory *class_factory;
    IAppxFactory *appx_factory;
    IStream *stream;
    UINT64 version;
    HRSRC resource;
    HGLOBAL hglob;
    WCHAR *string;
    HRESULT hr;
    int cmp;

    hr = CoGetClassObject( &CLSID_AppxFactory, CLSCTX_INPROC_SERVER, NULL, &IID_IClassFactory, (void **)&class_factory );
    ok( hr == S_OK || broken( hr == CLASS_E_CLASSNOTAVAILABLE ), "got hr %#lx.\n", hr );
    if (hr != S_OK)
    {
        skip( "Failed to create IClassFactory, skipping tests.\n" );
        return;
    }

    check_interface( class_factory, &IID_IUnknown );
    check_interface( class_factory, &IID_IClassFactory );

    hr = IClassFactory_CreateInstance( class_factory, (void *)0xdeadbeef, &IID_IAppxFactory, (void **)&appx_factory );
    ok( hr == CLASS_E_NOAGGREGATION, "got hr %#lx.\n", hr );
    if (SUCCEEDED(hr)) IAppxFactory_Release( appx_factory );
    hr = IClassFactory_CreateInstance( class_factory, NULL, &IID_IAppxFactory, (void **)&appx_factory );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    IClassFactory_Release( class_factory );
    if (hr != S_OK)
    {
        skip( "Failed to create IAppxFactory, skipping tests.\n" );
        return;
    }

    check_interface( appx_factory, &IID_IUnknown );
    check_interface( appx_factory, &IID_IAppxFactory );

    resource = FindResourceW( NULL, L"appxmanifest.xml", (const WCHAR *)RT_RCDATA );
    hglob = LoadResource( NULL, resource );
    stream = SHCreateMemStream( hglob, SizeofResource( NULL, resource ) );
    ok( !!stream, "got stream %p.\n", stream );
    GlobalFree( hglob );
    if (!stream)
    {
        skip( "Failed to create IStream, skipping tests %#lx.\n", GetLastError() );
        return;
    }

    hr = IAppxFactory_CreateManifestReader( appx_factory, NULL, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxFactory_CreateManifestReader( appx_factory, stream, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxFactory_CreateManifestReader( appx_factory, NULL, &manifest_reader );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    ok( manifest_reader == (void *)0xdeadbeef, "got manifest_reader %p.\n", manifest_reader );
    hr = IAppxFactory_CreateManifestReader( appx_factory, stream, &manifest_reader );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    IAppxFactory_Release( appx_factory );
    IStream_Release( stream );
    if (hr != S_OK)
    {
        skip( "Failed to create IAppxManifestReader, skipping tests.\n" );
        return;
    }

    hr = IAppxManifestReader_GetPackageId( manifest_reader, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxManifestReader_GetPackageId( manifest_reader, &manifest_package_id );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    IAppxManifestReader_Release( manifest_reader );
    if (hr != S_OK)
    {
        skip( "Failed to create IAppxManifestPackageId, skipping tests.\n" );
        return;
    }

    check_interface( manifest_package_id, &IID_IUnknown );
    check_interface( manifest_package_id, &IID_IAppxManifestPackageId );

    hr = IAppxManifestPackageId_GetName( manifest_package_id, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxManifestPackageId_GetName( manifest_package_id, &string );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !!string, "got string %p.\n", string );
    if (hr == S_OK && string)
    {
        cmp = wcscmp( string, L"WineTest" );
        ok( !cmp, "expected %s, got %s.\n", debugstr_w( L"WineTest" ), debugstr_w( string ) );
        CoTaskMemFree( string );
    }

    hr = IAppxManifestPackageId_GetArchitecture( manifest_package_id, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxManifestPackageId_GetArchitecture( manifest_package_id, &arch );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( arch == 11, "got arch %d.\n", arch );

    hr = IAppxManifestPackageId_GetPublisher( manifest_package_id, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxManifestPackageId_GetPublisher( manifest_package_id, &string );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !!string, "got string %p.\n", string );
    if (hr == S_OK && string)
    {
        cmp = wcscmp( string, L"CN=Wine, O=The Wine Project, C=US" );
        ok( !cmp, "expected %s, got %s.\n", debugstr_w( L"CN=Wine, O=The Wine Project, C=US" ), debugstr_w( string ) );
        CoTaskMemFree( string );
    }

    hr = IAppxManifestPackageId_GetVersion( manifest_package_id, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxManifestPackageId_GetVersion( manifest_package_id, &version );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( version == 0x000100020003abcd, "got version %#llx.\n", version );

    hr = IAppxManifestPackageId_GetResourceId( manifest_package_id, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxManifestPackageId_GetResourceId( manifest_package_id, &string );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !string, "got string %p.\n", string );

    hr = IAppxManifestPackageId_GetPackageFullName( manifest_package_id, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxManifestPackageId_GetPackageFullName( manifest_package_id, &string );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !!string, "got string %p.\n", string );
    if (hr == S_OK && string)
    {
        cmp = wcscmp( string, L"WineTest_1.2.3.43981_neutral__1dd93kt3jvn14" );
        ok( !cmp, "expected %s, got %s.\n", debugstr_w( L"WineTest_1.2.3.43981_neutral__1dd93kt3jvn14" ), debugstr_w( string ) );
        CoTaskMemFree( string );
    }

    hr = IAppxManifestPackageId_GetPackageFamilyName( manifest_package_id, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IAppxManifestPackageId_GetPackageFamilyName( manifest_package_id, &string );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !!string, "got string %p.\n", string );
    if (hr == S_OK && string)
    {
        cmp = wcscmp( string, L"WineTest_1dd93kt3jvn14" );
        ok( !cmp, "expected %s, got %s.\n", debugstr_w( L"WineTest_1dd93kt3jvn14" ), debugstr_w( string ) );
        CoTaskMemFree( string );
    }

    IAppxManifestPackageId_Release( manifest_package_id );
}

START_TEST(appxpackaging)
{
    HRESULT hr;

    hr = CoInitialize( NULL );
    ok( hr == S_OK, "got hr %#lx.\n", hr );

    test_AppxFactory();

    CoUninitialize();
}
