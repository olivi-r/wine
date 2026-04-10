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

#define COBJMACROS
#include "winstring.h"

#include "initguid.h"
#include "roapi.h"

#define WIDL_using_Windows_Data_Xml_Dom
#include "windows.data.xml.dom.h"

#include "wine/test.h"

#define check_interface( obj, iid ) check_interface_( __LINE__, obj, iid )
static void check_interface_( unsigned int line, void *obj, const IID *iid )
{
    IUnknown *iface = obj;
    IUnknown *unk;
    HRESULT hr;

    hr = IUnknown_QueryInterface( iface, iid, (void **)&unk );
    ok_(__FILE__, line)( hr == S_OK, "got hr %#lx.\n", hr );
    IUnknown_Release( unk );
}

static void test_XmlLoadSettings(void)
{
    static const WCHAR *class_name = RuntimeClass_Windows_Data_Xml_Dom_XmlLoadSettings;
    IXmlLoadSettings *load_settings;
    IActivationFactory *factory;
    boolean setting_bool;
    UINT32 setting_int;
    HSTRING str;
    HRESULT hr;
    LONG ref;

    hr = WindowsCreateString( class_name, wcslen( class_name ), &str );
    ok( hr == S_OK, "got hr %#lx.\n", hr );

    hr = RoGetActivationFactory( str, &IID_IActivationFactory, (void **)&factory );
    WindowsDeleteString( str );
    ok( hr == S_OK || broken( hr == REGDB_E_CLASSNOTREG ), "got hr %#lx.\n", hr );
    if (FAILED(hr))
    {
        win_skip( "%s runtimeclass not found, skipping tests.\n", wine_dbgstr_w( class_name ) );
        return;
    }

    check_interface( factory, &IID_IUnknown );
    check_interface( factory, &IID_IInspectable );
    check_interface( factory, &IID_IAgileObject );
    check_interface( factory, &IID_IActivationFactory );

    hr = IActivationFactory_ActivateInstance( factory, (IInspectable **)&load_settings );
    ok( hr == S_OK, "got hr %#lx.\n", hr );

    ref = IActivationFactory_Release( factory );
    ok( ref == 1, "got ref %ld.\n", ref );

    check_interface( load_settings, &IID_IUnknown );
    check_interface( load_settings, &IID_IInspectable );
    check_interface( load_settings, &IID_IAgileObject );
    check_interface( load_settings, &IID_IXmlLoadSettings );

    hr = IXmlLoadSettings_get_MaxElementDepth( load_settings, &setting_int );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( setting_int == 256, "got setting_int %u.\n", setting_int );
    hr = IXmlLoadSettings_put_MaxElementDepth( load_settings, 64 );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = IXmlLoadSettings_get_MaxElementDepth( load_settings, &setting_int );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( setting_int == 64, "got setting_int %u.\n", setting_int );

    hr = IXmlLoadSettings_get_ProhibitDtd( load_settings, &setting_bool );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( setting_bool, "got setting_bool %d.\n", setting_bool );
    hr = IXmlLoadSettings_put_ProhibitDtd( load_settings, FALSE );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = IXmlLoadSettings_get_ProhibitDtd( load_settings, &setting_bool );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !setting_bool, "got setting_bool %d.\n", setting_bool );

    hr = IXmlLoadSettings_get_ResolveExternals( load_settings, &setting_bool );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !setting_bool, "got setting_bool %d.\n", setting_bool );
    hr = IXmlLoadSettings_put_ResolveExternals( load_settings, TRUE );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = IXmlLoadSettings_get_ResolveExternals( load_settings, &setting_bool );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( setting_bool, "got setting_bool %d.\n", setting_bool );

    hr = IXmlLoadSettings_get_ValidateOnParse( load_settings, &setting_bool );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !setting_bool, "got setting_bool %d.\n", setting_bool );
    hr = IXmlLoadSettings_put_ValidateOnParse( load_settings, TRUE );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = IXmlLoadSettings_get_ValidateOnParse( load_settings, &setting_bool );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( setting_bool, "got setting_bool %d.\n", setting_bool );

    hr = IXmlLoadSettings_get_ElementContentWhiteSpace( load_settings, &setting_bool );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( setting_bool, "got setting_bool %d.\n", setting_bool );
    hr = IXmlLoadSettings_put_ElementContentWhiteSpace( load_settings, FALSE );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = IXmlLoadSettings_get_ElementContentWhiteSpace( load_settings, &setting_bool );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( !setting_bool, "got setting_bool %d.\n", setting_bool );

    ref = IXmlLoadSettings_Release( load_settings );
    ok( ref == 0, "got ref %ld.\n", ref );
}

START_TEST(runtime)
{
    HRESULT hr;

    hr = RoInitialize( RO_INIT_MULTITHREADED );
    ok( hr == S_OK, "RoInitialize failed, hr %#lx.\n", hr );

    test_XmlLoadSettings();

    RoUninitialize();
}
