/* WinRT Windows.UI.Core.CoreWindow Implementation
 *
 * Copyright 2025 Zhiyi Zhang for CodeWeavers
 * Copyright 2026 Olivia Ryan
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

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(ui);

struct coredispatcher
{
    ICoreDispatcher ICoreDispatcher_iface;
    ICoreAcceleratorKeys ICoreAcceleratorKeys_iface;
    ICoreDispatcherWithTaskPriority ICoreDispatcherWithTaskPriority_iface;
    LONG ref;
};

static inline struct coredispatcher *impl_from_ICoreDispatcher( ICoreDispatcher *iface )
{
    return CONTAINING_RECORD( iface, struct coredispatcher, ICoreDispatcher_iface );
}

static HRESULT WINAPI coredispatcher_QueryInterface( ICoreDispatcher *iface, REFIID iid, void **out )
{
    struct coredispatcher *impl = impl_from_ICoreDispatcher( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_ICoreDispatcher ))
    {
        *out = &impl->ICoreDispatcher_iface;
        ICoreDispatcher_AddRef( &impl->ICoreDispatcher_iface );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_ICoreAcceleratorKeys ))
    {
        *out = &impl->ICoreAcceleratorKeys_iface;
        ICoreAcceleratorKeys_AddRef( &impl->ICoreAcceleratorKeys_iface );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_ICoreDispatcherWithTaskPriority ))
    {
        *out = &impl->ICoreDispatcherWithTaskPriority_iface;
        ICoreDispatcherWithTaskPriority_AddRef( &impl->ICoreDispatcherWithTaskPriority_iface );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI coredispatcher_AddRef( ICoreDispatcher *iface )
{
    struct coredispatcher *impl = impl_from_ICoreDispatcher( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI coredispatcher_Release( ICoreDispatcher *iface )
{
    struct coredispatcher *impl = impl_from_ICoreDispatcher( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI coredispatcher_GetIids( ICoreDispatcher *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_GetRuntimeClassName( ICoreDispatcher *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_GetTrustLevel( ICoreDispatcher *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_get_HasThreadAccess( ICoreDispatcher *iface, BOOLEAN *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_ProcessEvents( ICoreDispatcher *iface, CoreProcessEventsOption options )
{
    FIXME( "iface %p, options %d stub!\n", iface, options );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_RunAsync( ICoreDispatcher *iface, CoreDispatcherPriority priority, IDispatchedHandler *callback, IAsyncAction **action )
{
    FIXME( "iface %p, priority %d, callback %p, action %p stub!\n", iface, priority, callback, action );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_RunIdleAsync( ICoreDispatcher *iface, IIdleDispatchedHandler *callback, IAsyncAction **action )
{
    FIXME( "iface %p, callback %p, action %p stub!\n", iface, callback, action );
    return E_NOTIMPL;
}

static const struct ICoreDispatcherVtbl coredispatcher_vtbl =
{
    coredispatcher_QueryInterface,
    coredispatcher_AddRef,
    coredispatcher_Release,
    /* IInspectable methods */
    coredispatcher_GetIids,
    coredispatcher_GetRuntimeClassName,
    coredispatcher_GetTrustLevel,
    /* ICoreDispatcher methods */
    coredispatcher_get_HasThreadAccess,
    coredispatcher_ProcessEvents,
    coredispatcher_RunAsync,
    coredispatcher_RunIdleAsync,
};

DEFINE_IINSPECTABLE( accelerator_keys, ICoreAcceleratorKeys, struct coredispatcher, ICoreDispatcher_iface )

static HRESULT WINAPI accelerator_keys_eventadd_AcceleratorKeyActivated( ICoreAcceleratorKeys *iface, ITypedEventHandler_CoreDispatcher_AcceleratorKeyEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI accelerator_keys_eventremove_AcceleratorKeyActivated( ICoreAcceleratorKeys *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static const struct ICoreAcceleratorKeysVtbl accelerator_keys_vtbl =
{
    accelerator_keys_QueryInterface,
    accelerator_keys_AddRef,
    accelerator_keys_Release,
    /* IInspectable methods */
    accelerator_keys_GetIids,
    accelerator_keys_GetRuntimeClassName,
    accelerator_keys_GetTrustLevel,
    /* ICoreAcceleratorKeys methods */
    accelerator_keys_eventadd_AcceleratorKeyActivated,
    accelerator_keys_eventremove_AcceleratorKeyActivated,
};

DEFINE_IINSPECTABLE( coredispatcher_priority, ICoreDispatcherWithTaskPriority, struct coredispatcher, ICoreDispatcher_iface )

static HRESULT WINAPI coredispatcher_priority_get_CurrentPriority( ICoreDispatcherWithTaskPriority *iface, CoreDispatcherPriority *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_priority_put_CurrentPriority( ICoreDispatcherWithTaskPriority *iface, CoreDispatcherPriority value )
{
    FIXME( "iface %p, value %d stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_priority_ShouldYield( ICoreDispatcherWithTaskPriority *iface, BOOLEAN *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_priority_ShouldYieldWithArg( ICoreDispatcherWithTaskPriority *iface, CoreDispatcherPriority priority, BOOLEAN *value )
{
    FIXME( "iface %p, priority %d, value %p stub!\n", iface, priority, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI coredispatcher_priority_StopProcessEvents( ICoreDispatcherWithTaskPriority *iface )
{
    FIXME( "iface %p stub!\n", iface );
    return E_NOTIMPL;
}

static const struct ICoreDispatcherWithTaskPriorityVtbl coredispatcher_priority_vtbl =
{
    coredispatcher_priority_QueryInterface,
    coredispatcher_priority_AddRef,
    coredispatcher_priority_Release,
    /* IInspectable methods */
    coredispatcher_priority_GetIids,
    coredispatcher_priority_GetRuntimeClassName,
    coredispatcher_priority_GetTrustLevel,
    /* ICoreDispatcherWithTaskPriority methods */
    coredispatcher_priority_get_CurrentPriority,
    coredispatcher_priority_put_CurrentPriority,
    coredispatcher_priority_ShouldYield,
    coredispatcher_priority_ShouldYieldWithArg,
    coredispatcher_priority_StopProcessEvents,
};

struct corewindow
{
    ICoreWindow ICoreWindow_iface;
    LONG ref;
};

static inline struct corewindow *impl_from_ICoreWindow( ICoreWindow *iface )
{
    return CONTAINING_RECORD( iface, struct corewindow, ICoreWindow_iface );
}

static HRESULT WINAPI corewindow_QueryInterface( ICoreWindow *iface, REFIID iid, void **out )
{
    struct corewindow *impl = impl_from_ICoreWindow( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_ICoreWindow ))
    {
        *out = &impl->ICoreWindow_iface;
        ICoreWindow_AddRef( &impl->ICoreWindow_iface );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI corewindow_AddRef( ICoreWindow *iface )
{
    struct corewindow *impl = impl_from_ICoreWindow( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI corewindow_Release( ICoreWindow *iface )
{
    struct corewindow *impl = impl_from_ICoreWindow( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    if (!ref) free( impl );
    return ref;
}

static HRESULT WINAPI corewindow_GetIids( ICoreWindow *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_GetRuntimeClassName( ICoreWindow *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_GetTrustLevel( ICoreWindow *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_get_AutomationHostProvider( ICoreWindow *iface, IInspectable **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_get_Bounds( ICoreWindow *iface, Rect *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_get_CustomProperties( ICoreWindow *iface, IPropertySet **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_get_Dispatcher( ICoreWindow *iface, ICoreDispatcher **value )
{
    struct coredispatcher *impl;

    TRACE( "iface %p, value %p.\n", iface, value );

    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->ICoreDispatcher_iface.lpVtbl = &coredispatcher_vtbl;
    impl->ICoreAcceleratorKeys_iface.lpVtbl = &accelerator_keys_vtbl;
    impl->ICoreDispatcherWithTaskPriority_iface.lpVtbl = &coredispatcher_priority_vtbl;
    impl->ref = 1;

    *value = &impl->ICoreDispatcher_iface;
    return S_OK;
}

static HRESULT WINAPI corewindow_get_FlowDirection( ICoreWindow *iface, CoreWindowFlowDirection *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_put_FlowDirection( ICoreWindow *iface, CoreWindowFlowDirection value )
{
    FIXME( "iface %p, value %p stub!\n", iface, &value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_get_IsInputEnabled( ICoreWindow *iface, BOOLEAN *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_put_IsInputEnabled( ICoreWindow *iface, BOOLEAN value )
{
    FIXME( "iface %p, value %d stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_get_PointerCursor( ICoreWindow *iface, ICoreCursor **value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_put_PointerCursor( ICoreWindow *iface, ICoreCursor *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_get_PointerPosition( ICoreWindow *iface, Point *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_get_Visible( ICoreWindow *iface, BOOLEAN *value )
{
    FIXME( "iface %p, value %p stub!\n", iface, value );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_Activate( ICoreWindow *iface )
{
    FIXME( "iface %p stub!\n", iface );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_Close( ICoreWindow *iface )
{
    FIXME( "iface %p stub!\n", iface );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_GetAsyncKeyState( ICoreWindow *iface, VirtualKey key, CoreVirtualKeyStates *state )
{
    FIXME( "iface %p key, %d, state %p stub!\n", iface, key, state );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_GetKeyState( ICoreWindow *iface, VirtualKey key, CoreVirtualKeyStates *state )
{
    FIXME( "iface %p key, %d, state %p stub!\n", iface, key, state );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_ReleasePointerCapture( ICoreWindow *iface )
{
    FIXME( "iface %p stub!\n", iface );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_SetPointerCapture( ICoreWindow *iface )
{
    FIXME( "iface %p stub!\n", iface );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_Activated( ICoreWindow *iface, ITypedEventHandler_CoreWindow_WindowActivatedEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_Activated( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_AutomationProviderRequested( ICoreWindow *iface, ITypedEventHandler_CoreWindow_AutomationProviderRequestedEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_AutomationProviderRequested( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_CharacterReceived( ICoreWindow *iface, ITypedEventHandler_CoreWindow_CharacterReceivedEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_CharacterReceived( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_Closed( ICoreWindow *iface, ITypedEventHandler_CoreWindow_CoreWindowEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_Closed( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_InputEnabled( ICoreWindow *iface, ITypedEventHandler_CoreWindow_InputEnabledEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_InputEnabled( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_KeyDown( ICoreWindow *iface, ITypedEventHandler_CoreWindow_KeyEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_KeyDown( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_KeyUp( ICoreWindow *iface, ITypedEventHandler_CoreWindow_KeyEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_KeyUp( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_PointerCaptureLost( ICoreWindow *iface, ITypedEventHandler_CoreWindow_PointerEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_PointerCaptureLost( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_PointerEntered( ICoreWindow *iface, ITypedEventHandler_CoreWindow_PointerEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_PointerEntered( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_PointerExited( ICoreWindow *iface, ITypedEventHandler_CoreWindow_PointerEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_PointerExited( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_PointerMoved( ICoreWindow *iface, ITypedEventHandler_CoreWindow_PointerEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_PointerMoved( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_PointerPressed( ICoreWindow *iface, ITypedEventHandler_CoreWindow_PointerEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_PointerPressed( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_PointerReleased( ICoreWindow *iface, ITypedEventHandler_CoreWindow_PointerEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_PointerReleased( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_TouchHitTesting( ICoreWindow *iface, ITypedEventHandler_CoreWindow_TouchHitTestingEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_TouchHitTesting( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_PointerWheelChanged( ICoreWindow *iface, ITypedEventHandler_CoreWindow_PointerEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_PointerWheelChanged( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_SizeChanged( ICoreWindow *iface, ITypedEventHandler_CoreWindow_WindowSizeChangedEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_SizeChanged( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventadd_VisibilityChanged( ICoreWindow *iface, ITypedEventHandler_CoreWindow_VisibilityChangedEventArgs *handler, EventRegistrationToken *cookie )
{
    FIXME( "iface %p, handler %p, cookie %p stub!\n", iface, handler, cookie );
    return E_NOTIMPL;
}

static HRESULT WINAPI corewindow_eventremove_VisibilityChanged( ICoreWindow *iface, EventRegistrationToken cookie )
{
    FIXME( "iface %p, cookie %p stub!\n", iface, &cookie );
    return E_NOTIMPL;
}

static const struct ICoreWindowVtbl corewindow_vtbl =
{
    corewindow_QueryInterface,
    corewindow_AddRef,
    corewindow_Release,
    /* IInspectable methods */
    corewindow_GetIids,
    corewindow_GetRuntimeClassName,
    corewindow_GetTrustLevel,
    /* ICoreWindow methods */
    corewindow_get_AutomationHostProvider,
    corewindow_get_Bounds,
    corewindow_get_CustomProperties,
    corewindow_get_Dispatcher,
    corewindow_get_FlowDirection,
    corewindow_put_FlowDirection,
    corewindow_get_IsInputEnabled,
    corewindow_put_IsInputEnabled,
    corewindow_get_PointerCursor,
    corewindow_put_PointerCursor,
    corewindow_get_PointerPosition,
    corewindow_get_Visible,
    corewindow_Activate,
    corewindow_Close,
    corewindow_GetAsyncKeyState,
    corewindow_GetKeyState,
    corewindow_ReleasePointerCapture,
    corewindow_SetPointerCapture,
    corewindow_eventadd_Activated,
    corewindow_eventremove_Activated,
    corewindow_eventadd_AutomationProviderRequested,
    corewindow_eventremove_AutomationProviderRequested,
    corewindow_eventadd_CharacterReceived,
    corewindow_eventremove_CharacterReceived,
    corewindow_eventadd_Closed,
    corewindow_eventremove_Closed,
    corewindow_eventadd_InputEnabled,
    corewindow_eventremove_InputEnabled,
    corewindow_eventadd_KeyDown,
    corewindow_eventremove_KeyDown,
    corewindow_eventadd_KeyUp,
    corewindow_eventremove_KeyUp,
    corewindow_eventadd_PointerCaptureLost,
    corewindow_eventremove_PointerCaptureLost,
    corewindow_eventadd_PointerEntered,
    corewindow_eventremove_PointerEntered,
    corewindow_eventadd_PointerExited,
    corewindow_eventremove_PointerExited,
    corewindow_eventadd_PointerMoved,
    corewindow_eventremove_PointerMoved,
    corewindow_eventadd_PointerPressed,
    corewindow_eventremove_PointerPressed,
    corewindow_eventadd_PointerReleased,
    corewindow_eventremove_PointerReleased,
    corewindow_eventadd_TouchHitTesting,
    corewindow_eventremove_TouchHitTesting,
    corewindow_eventadd_PointerWheelChanged,
    corewindow_eventremove_PointerWheelChanged,
    corewindow_eventadd_SizeChanged,
    corewindow_eventremove_SizeChanged,
    corewindow_eventadd_VisibilityChanged,
    corewindow_eventremove_VisibilityChanged,
};

struct corewindow_statics
{
    IActivationFactory IActivationFactory_iface;
    ICoreWindowStatic ICoreWindowStatic_iface;
    LONG ref;
};

static inline struct corewindow_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct corewindow_statics, IActivationFactory_iface );
}

static HRESULT WINAPI factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct corewindow_statics *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IActivationFactory ))
    {
        *out = &impl->IActivationFactory_iface;
        IActivationFactory_AddRef( &impl->IActivationFactory_iface );
        return S_OK;
    }
    else if (IsEqualGUID( iid, &IID_ICoreWindowStatic ))
    {
        *out = &impl->ICoreWindowStatic_iface;
        ICoreWindowStatic_AddRef( &impl->ICoreWindowStatic_iface );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI factory_AddRef( IActivationFactory *iface )
{
    struct corewindow_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI factory_Release( IActivationFactory *iface )
{
    struct corewindow_statics *impl = impl_from_IActivationFactory( iface );
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
    FIXME( "iface %p, instance %p.\n", iface, instance );
    return E_NOTIMPL;
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

DEFINE_IINSPECTABLE( corewindow_static, ICoreWindowStatic, struct corewindow_statics, IActivationFactory_iface )

static HRESULT STDMETHODCALLTYPE corewindow_static_GetForCurrentThread( ICoreWindowStatic *iface, ICoreWindow **windows )
{
    struct corewindow *impl;

    TRACE( "iface %p, windows %p.\n", iface, windows );

    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->ICoreWindow_iface.lpVtbl = &corewindow_vtbl;
    impl->ref = 1;

    *windows = &impl->ICoreWindow_iface;
    return S_OK;
}

static const struct ICoreWindowStaticVtbl corewindow_static_vtbl =
{
    corewindow_static_QueryInterface,
    corewindow_static_AddRef,
    corewindow_static_Release,
    /* IInspectable methods */
    corewindow_static_GetIids,
    corewindow_static_GetRuntimeClassName,
    corewindow_static_GetTrustLevel,
    /* ICoreWindowStatic methods */
    corewindow_static_GetForCurrentThread
};

static struct corewindow_statics corewindow_statics =
{
    {&factory_vtbl},
    {&corewindow_static_vtbl},
    1,
};

IActivationFactory *corewindow_factory = &corewindow_statics.IActivationFactory_iface;
