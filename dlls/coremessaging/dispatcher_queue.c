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

WINE_DEFAULT_DEBUG_CHANNEL(messaging);

struct dispatcher_queue
{
    IDispatcherQueue IDispatcherQueue_iface;
    LONG ref;
};

static inline struct dispatcher_queue *impl_from_IDispatcherQueue( IDispatcherQueue *iface )
{
    return CONTAINING_RECORD( iface, struct dispatcher_queue, IDispatcherQueue_iface );
}

static HRESULT WINAPI dispatcher_queue_QueryInterface( IDispatcherQueue *iface, REFIID iid, void **out )
{
    struct dispatcher_queue *impl = impl_from_IDispatcherQueue( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IDispatcherQueue ))
    {
        IInspectable_AddRef( (*out = &impl->IDispatcherQueue_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI dispatcher_queue_AddRef( IDispatcherQueue *iface )
{
    struct dispatcher_queue *impl = impl_from_IDispatcherQueue( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI dispatcher_queue_Release( IDispatcherQueue *iface )
{
    struct dispatcher_queue *impl = impl_from_IDispatcherQueue( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
    if (!impl) free( impl );
    return ref;
}

static HRESULT WINAPI dispatcher_queue_GetIids( IDispatcherQueue *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_GetRuntimeClassName( IDispatcherQueue *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_GetTrustLevel( IDispatcherQueue *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_CreateTimer( IDispatcherQueue *iface, IDispatcherQueueTimer **result )
{
    FIXME( "iface %p, result %p stub!\n", iface, result );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_TryEnqueue( IDispatcherQueue *iface, IDispatcherQueueHandler *handler, BOOLEAN *result )
{
    FIXME( "iface %p, handler %p, result %p stub!\n", iface, handler, result );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_TryEnqueueWithPriority( IDispatcherQueue *iface, DispatcherQueuePriority priority, IDispatcherQueueHandler *handler, BOOLEAN *result )
{
    FIXME( "iface %p, priority %d, handler %p, result %p stub!\n", iface, priority, handler, result );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_add_ShutdownStarting( IDispatcherQueue *iface, ITypedEventHandler_DispatcherQueue_DispatcherQueueShutdownStartingEventArgs *handler, EventRegistrationToken *token )
{
    FIXME( "iface %p, handler %p, token %p stub!\n", iface, handler, token );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_remove_ShutdownStarting( IDispatcherQueue *iface, EventRegistrationToken token )
{
    FIXME( "iface %p, token %p stub!\n", iface, &token );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_add_ShutdownCompleted( IDispatcherQueue *iface, ITypedEventHandler_DispatcherQueue_IInspectable *handler, EventRegistrationToken *token )
{
    FIXME( "iface %p, handler %p, token %p stub!\n", iface, handler, token );
    return E_NOTIMPL;
}

static HRESULT WINAPI dispatcher_queue_remove_ShutdownCompleted( IDispatcherQueue *iface, EventRegistrationToken token )
{
    FIXME( "iface %p, token %p stub!\n", iface, &token );
    return E_NOTIMPL;
}

static const struct IDispatcherQueueVtbl dispatcher_queue_vtbl =
{
    dispatcher_queue_QueryInterface,
    dispatcher_queue_AddRef,
    dispatcher_queue_Release,
    /* IInspectable methods */
    dispatcher_queue_GetIids,
    dispatcher_queue_GetRuntimeClassName,
    dispatcher_queue_GetTrustLevel,
    /* IDispatcherQueue methods */
    dispatcher_queue_CreateTimer,
    dispatcher_queue_TryEnqueue,
    dispatcher_queue_TryEnqueueWithPriority,
    dispatcher_queue_add_ShutdownStarting,
    dispatcher_queue_remove_ShutdownStarting,
    dispatcher_queue_add_ShutdownCompleted,
    dispatcher_queue_remove_ShutdownCompleted,
};

struct dispatcher_queue_statics
{
    IActivationFactory IActivationFactory_iface;
    IDispatcherQueueStatics IDispatcherQueueStatics_iface;
    LONG ref;
};

static inline struct dispatcher_queue_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct dispatcher_queue_statics, IActivationFactory_iface );
}

static HRESULT WINAPI activation_factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct dispatcher_queue_statics *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IActivationFactory ))
    {
        IInspectable_AddRef( (*out = &impl->IActivationFactory_iface) );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_IDispatcherQueueStatics ))
    {
        IInspectable_AddRef( (*out = &impl->IDispatcherQueueStatics_iface) );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI activation_factory_AddRef( IActivationFactory *iface )
{
    struct dispatcher_queue_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI activation_factory_Release( IActivationFactory *iface )
{
    struct dispatcher_queue_statics *impl = impl_from_IActivationFactory( iface );
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

DEFINE_IINSPECTABLE( dispatcher_queue_statics, IDispatcherQueueStatics, struct dispatcher_queue_statics, IActivationFactory_iface )

static HRESULT WINAPI dispatcher_queue_statics_GetForCurrentThread( IDispatcherQueueStatics *iface, IDispatcherQueue **result )
{
    struct dispatcher_queue *impl;

    TRACE( "iface %p, result %p.\n", iface, result );

    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->IDispatcherQueue_iface.lpVtbl = &dispatcher_queue_vtbl;
    impl->ref = 1;

    *result = &impl->IDispatcherQueue_iface;
    return S_OK;
}

static const struct IDispatcherQueueStaticsVtbl dispatcher_queue_statics_vtbl =
{
    dispatcher_queue_statics_QueryInterface,
    dispatcher_queue_statics_AddRef,
    dispatcher_queue_statics_Release,
    /* IInspectable methods */
    dispatcher_queue_statics_GetIids,
    dispatcher_queue_statics_GetRuntimeClassName,
    dispatcher_queue_statics_GetTrustLevel,
    /* IDispatcherQueueStatics methods */
    dispatcher_queue_statics_GetForCurrentThread,
};

static struct dispatcher_queue_statics dispatcher_queue_statics =
{
    {&activation_factory_vtbl},
    {&dispatcher_queue_statics_vtbl},
    1,
};

IActivationFactory *dispatcher_queue_factory = &dispatcher_queue_statics.IActivationFactory_iface;
