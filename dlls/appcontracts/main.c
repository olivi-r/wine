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
#include "private.h"

WINE_DEFAULT_DEBUG_CHANNEL(appcontracts);

HRESULT WINAPI DllGetClassObject( REFCLSID clsid, REFIID iid, void **out )
{
    FIXME( "clsid %s, iid %s, out %p stub!\n", debugstr_guid( clsid ), debugstr_guid( iid ), out );
    return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT WINAPI DllGetActivationFactory( HSTRING class, IActivationFactory **factory )
{
    const WCHAR *buffer = WindowsGetStringRawBuffer( class, NULL );

    TRACE( "class %s, factory %p.\n", debugstr_w( buffer ), factory );

    *factory = NULL;

    if (!wcscmp( buffer, RuntimeClass_Windows_ApplicationModel_AppService_AppServiceConnection ))
        IActivationFactory_QueryInterface( app_service_connection_factory, &IID_IActivationFactory, (void **)factory );

    if (*factory) return S_OK;
    return CLASS_E_CLASSNOTAVAILABLE;
}
