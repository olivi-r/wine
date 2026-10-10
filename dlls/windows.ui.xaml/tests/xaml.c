/*
 * Copyright (C) 2025 Mohamad Al-Jaf
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
#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winstring.h"

#include "roapi.h"

#define WIDL_using_Windows_Foundation
#define WIDL_using_Windows_Foundation_Collections
#define WIDL_using_Windows_UI
#define WIDL_using_Windows_UI_Xaml
#include "windows.ui.xaml.h"

#include "wine/test.h"

#define check_interface( obj, iid, is_broken ) check_interface_( __LINE__, obj, iid, is_broken )
static void check_interface_( unsigned int line, void *obj, const IID *iid, BOOL is_broken )
{
    IUnknown *iface = obj;
    IUnknown *unk;
    HRESULT hr;

    hr = IUnknown_QueryInterface( iface, iid, (void **)&unk );
    ok_(__FILE__, line)( hr == S_OK || broken( is_broken && hr == E_NOINTERFACE ), "got hr %#lx.\n", hr );
    if (SUCCEEDED(hr))
        IUnknown_Release( unk );
}

struct inspectable
{
    IInspectable IInspectable_iface;
    LONG ref;
};

static inline struct inspectable *impl_from_IInspectable( IInspectable *iface )
{
    return CONTAINING_RECORD( iface, struct inspectable, IInspectable_iface );
}

static HRESULT WINAPI inspectable_QueryInterface( IInspectable *iface, REFIID iid, void **out )
{
    struct inspectable *impl = impl_from_IInspectable( iface );

    trace( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ))
    {
        IInspectable_AddRef( (*out = &impl->IInspectable_iface) );
        return S_OK;
    }

    return E_NOINTERFACE;
}

static ULONG WINAPI inspectable_AddRef( IInspectable *iface )
{
    struct inspectable *impl = impl_from_IInspectable( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    trace( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI inspectable_Release( IInspectable *iface )
{
    struct inspectable *impl = impl_from_IInspectable( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    trace( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI inspectable_GetIids( IInspectable *iface, ULONG *iid_count, IID **iids )
{
    trace( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI inspectable_GetRuntimeClassName( IInspectable *iface, HSTRING *class_name )
{
    trace( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI inspectable_GetTrustLevel( IInspectable *iface, TrustLevel *trust_level )
{
    trace( "iface %p, trust_level %p stub!\n", iface, trust_level );
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

static void test_Application(void)
{
    static const WCHAR *application_statics_name = L"Windows.UI.Xaml.Application";
    IApplication *application = (void *)0xdeadbeef;
    IInspectable *inner = (void *)0xdeadbeef;
    IApplicationFactory *application_factory;
    IApplicationStatics *application_statics;
    IActivationFactory *activation_factory;
    struct inspectable *inspectable;
    HSTRING str;
    HRESULT hr;
    ULONG ref;

    inspectable = calloc( 1, sizeof(*inspectable) );
    ok( !!inspectable, "got impl %p.\n", inspectable );
    if (!inspectable)
    {
        skip( "Failed to create inspectable, skipping tests.\n" );
        return;
    }

    inspectable->IInspectable_iface.lpVtbl = &inspectable_vtbl;
    inspectable->ref = 1;

    hr = WindowsCreateString( application_statics_name, wcslen( application_statics_name ), &str );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = RoGetActivationFactory( str, &IID_IActivationFactory, (void **)&activation_factory );
    ok( hr == S_OK || broken( hr == REGDB_E_CLASSNOTREG ), "got hr %#lx.\n", hr );
    WindowsDeleteString( str );
    if (hr == REGDB_E_CLASSNOTREG)
    {
        win_skip( "%s runtimeclass not registered, skipping tests.\n", wine_dbgstr_w( application_statics_name ) );
        goto end;
    }

    check_interface( activation_factory, &IID_IUnknown, FALSE );
    check_interface( activation_factory, &IID_IInspectable, FALSE );
    check_interface( activation_factory, &IID_IAgileObject, FALSE );

    hr = IActivationFactory_QueryInterface( activation_factory, &IID_IApplicationStatics, (void **)&application_statics);
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = IApplicationStatics_get_Current( application_statics, &application );
    ok( hr == E_UNEXPECTED, "got hr %#lx.\n", hr );
    ok( application == (void *)0xdeadbeef, "got application %p.\n", application );

    hr = IActivationFactory_QueryInterface( activation_factory, &IID_IApplicationFactory, (void **)&application_factory );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = IApplicationFactory_CreateInstance( application_factory, NULL, NULL, NULL );
    ok( hr == E_INVALIDARG, "got hr %#lx.\n", hr );
    hr = IApplicationFactory_CreateInstance( application_factory, NULL, &inner, NULL );
    ok( hr == E_INVALIDARG, "got hr %#lx.\n", hr );
    ok( inner == (void *)0xdeadbeef, "got inner %p.\n", inner );
    hr = IApplicationFactory_CreateInstance( application_factory, &inspectable->IInspectable_iface, NULL, NULL );
    ok( hr == E_INVALIDARG, "got hr %#lx.\n", hr );
    hr = IApplicationFactory_CreateInstance( application_factory, &inspectable->IInspectable_iface, &inner, NULL );
    ok( hr == E_INVALIDARG, "got hr %#lx.\n", hr );
    ok( inner == (void *)0xdeadbeef, "got inner %p.\n", inner );

    hr = IApplicationFactory_CreateInstance( application_factory, NULL, NULL, &application );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( application != (void *)0xdeadbeef, "got application %p.\n", application );
    ref = IApplication_Release( application );
    ok( ref == 1, "got ref %ld.\n", ref );

    application = (void *)0xdeadbeef;
    hr = IApplicationStatics_get_Current( application_statics, &application );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( application != (void *)0xdeadbeef, "got application %p.\n", application );
    ref = IApplication_Release( application );
    ok( ref == 1, "got ref %ld.\n", ref );
    ref = IApplication_Release( application );
    ok( ref == 0, "got ref %ld.\n", ref );
    application = (void *)0xdeadbeef;
    hr = IApplicationStatics_get_Current( application_statics, &application );
    ok( hr == E_UNEXPECTED, "got hr %#lx.\n", hr );
    ok( application == (void *)0xdeadbeef, "got application %p.\n", application );

    hr = IApplicationFactory_CreateInstance( application_factory, &inspectable->IInspectable_iface, &inner, &application );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( application != (void *)0xdeadbeef, "got application %p.\n", application );
    ok( inspectable->ref == 3, "got ref %ld.\n", inspectable->ref );
    ref = IApplication_Release( application );
    ok( ref == 2, "got ref %ld.\n", ref );
    ok( inspectable->ref == 2, "got ref %ld.\n", inspectable->ref );

    ref = IInspectable_Release( inner );
    ok( ref == 0, "got ref %ld.\n", ref );
    ok( inspectable->ref == 2, "got ref %ld.\n", inspectable->ref );

    // ref = IApplication_Release( application );
    // ok( ref == 20, "got ref %ld.\n", ref );
    // ref = IInspectable_Release( inspectable->IInspectable_inner );
    // ok( ref == 20, "got ref %ld.\n", ref );

    ref = IApplicationFactory_Release( application_factory );
    ok( ref == 3, "got ref %ld.\n", ref );
    ref = IApplicationStatics_Release( application_statics );
    ok( ref == 2, "got ref %ld.\n", ref );
    ref = IActivationFactory_Release( activation_factory );
    ok( ref == 1, "got ref %ld.\n", ref );

end:
    ref = IInspectable_Release( &inspectable->IInspectable_iface );
    ok( ref == 1, "got ref %ld.\n", ref );
    trace("end\n");
}

static void test_ColorHelper(void)
{
    static const WCHAR *color_helper_statics_name = L"Windows.UI.ColorHelper";
    IColorHelperStatics *color_helper_statics = (void *)0xdeadbeef;
    IActivationFactory *factory = (void *)0xdeadbeef;
    Color color;
    HSTRING str;
    HRESULT hr;

    hr = WindowsCreateString( color_helper_statics_name, wcslen( color_helper_statics_name ), &str );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    hr = RoGetActivationFactory( str, &IID_IActivationFactory, (void **)&factory );
    WindowsDeleteString( str );
    ok( hr == S_OK || broken( hr == REGDB_E_CLASSNOTREG ), "got hr %#lx.\n", hr );
    if (hr == REGDB_E_CLASSNOTREG)
    {
        win_skip( "%s runtimeclass not registered, skipping tests.\n", wine_dbgstr_w( color_helper_statics_name ) );
        return;
    }

    check_interface( factory, &IID_IUnknown, FALSE );
    check_interface( factory, &IID_IInspectable, FALSE );
    check_interface( factory, &IID_IAgileObject, TRUE /* Missing on Windows older than 1809v2 */ );

    hr = IActivationFactory_QueryInterface( factory, &IID_IColorHelperStatics, (void **)&color_helper_statics );
    ok( hr == S_OK, "got hr %#lx.\n", hr );

    memset( &color, 0, sizeof( color ) );
    hr = IColorHelperStatics_FromArgb( color_helper_statics, 255, 255, 255, 255, NULL );
    ok( hr == E_POINTER, "got hr %#lx.\n", hr );
    hr = IColorHelperStatics_FromArgb( color_helper_statics, 255, 255, 255, 255, &color );
    ok( hr == S_OK, "got hr %#lx.\n", hr );
    ok( color.A == 255 && color.R == 255 && color.G == 255 && color.B == 255,
        "got color.A = %u, color.R = %u, color.G = %u, color.B = %u,\n", color.A, color.R, color.G, color.B );

    IColorHelperStatics_Release( color_helper_statics );
    IActivationFactory_Release( factory );
}

START_TEST(xaml)
{
    HRESULT hr;

    hr = RoInitialize( RO_INIT_MULTITHREADED );
    ok( hr == S_OK, "RoInitialize failed, hr %#lx\n", hr );

    test_Application();
    test_ColorHelper();

    RoUninitialize();
}
