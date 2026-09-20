/* WinRT Windows.Data.Json.JsonObject Implementation
 *
 * Copyright (C) 2024 Mohamad Al-Jaf
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
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(web);

struct json_object
{
    IJsonObject IJsonObject_iface;
    IJsonValue IJsonValue_iface;
    LONG ref;
    IInspectable *inner;
};

static inline struct json_object *impl_from_IJsonObject( IJsonObject *iface )
{
    return CONTAINING_RECORD( iface, struct json_object, IJsonObject_iface );
}

static HRESULT WINAPI json_object_QueryInterface( IJsonObject *iface, REFIID iid, void **out )
{
    struct json_object *impl = impl_from_IJsonObject( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IJsonObject ))
    {
        *out = &impl->IJsonObject_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    if (IsEqualGUID( iid, &IID_IJsonValue ))
    {
        *out = &impl->IJsonValue_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    if (SUCCEEDED(IInspectable_QueryInterface( impl->inner, iid, out ))) return S_OK;

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI json_object_AddRef( IJsonObject *iface )
{
    struct json_object *impl = impl_from_IJsonObject( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI json_object_Release( IJsonObject *iface )
{
    struct json_object *impl = impl_from_IJsonObject( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );

    TRACE( "iface %p, ref %lu.\n", iface, ref );

    if (!ref)
    {
        IInspectable_Release( impl->inner );
        free( impl );
    }
    return ref;
}

static HRESULT WINAPI json_object_GetIids( IJsonObject *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI json_object_GetRuntimeClassName( IJsonObject *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI json_object_GetTrustLevel( IJsonObject *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI json_object_GetNamedValue( IJsonObject *iface, HSTRING name, IJsonValue **value )
{
    struct json_object *impl = impl_from_IJsonObject( iface );
    IMap_HSTRING_IJsonValue *map;
    boolean exists;
    HRESULT hr;

    TRACE( "iface %p, name %s, value %p.\n", iface, debugstr_hstring( name ), value );

    if (!value) return E_POINTER;
    if (FAILED(hr = IInspectable_QueryInterface( impl->inner, &IID_IMap_HSTRING_IJsonValue, (void **)&map )))
        return hr;

    hr = IMap_HSTRING_IJsonValue_HasKey( map, name, &exists );
    if (FAILED(hr) || !exists)
    {
        IMap_HSTRING_IJsonValue_Release( map );
        return WEB_E_JSON_VALUE_NOT_FOUND;
    }

    hr = IMap_HSTRING_IJsonValue_Lookup( map, name, value );
    IMap_HSTRING_IJsonValue_Release( map );
    return hr;
}

static HRESULT WINAPI json_object_SetNamedValue( IJsonObject *iface, HSTRING name, IJsonValue *value )
{
    struct json_object *impl = impl_from_IJsonObject( iface );
    IMap_HSTRING_IJsonValue *map;
    boolean dummy;
    HRESULT hr;

    TRACE( "iface %p, name %s, value %p.\n", iface, debugstr_hstring( name ), value );

    if (FAILED(hr = IInspectable_QueryInterface( impl->inner, &IID_IMap_HSTRING_IJsonValue, (void **)&map )))
        return hr;

    hr = IMap_HSTRING_IJsonValue_Insert( map, name, value, &dummy );
    IMap_HSTRING_IJsonValue_Release( map );
    return hr;
}

static HRESULT WINAPI json_object_GetNamedObject( IJsonObject *iface, HSTRING name, IJsonObject **value )
{
    IJsonValue *internal_value;
    JsonValueType value_type;
    HRESULT hr;

    TRACE( "iface %p, name %s, value %p.\n", iface, debugstr_hstring( name ), value );

    if (!value) return E_POINTER;
    if (FAILED(hr = IJsonObject_GetNamedValue( iface, name, &internal_value )))
        return hr;

    IJsonValue_Release( internal_value );
    IJsonValue_get_ValueType( internal_value, &value_type );
    if (value_type != JsonValueType_Object) return E_ILLEGAL_METHOD_CALL;

    return IJsonValue_GetObject( internal_value, value );
}

static HRESULT WINAPI json_object_GetNamedArray( IJsonObject *iface, HSTRING name, IJsonArray **value )
{
    IJsonValue *internal_value;
    JsonValueType value_type;
    HRESULT hr;

    TRACE( "iface %p, name %s, value %p.\n", iface, debugstr_hstring( name ), value );

    if (!value) return E_POINTER;
    if (FAILED(hr = IJsonObject_GetNamedValue( iface, name, &internal_value )))
        return hr;

    IJsonValue_Release( internal_value );
    IJsonValue_get_ValueType( internal_value, &value_type );
    if (value_type != JsonValueType_Array) return E_ILLEGAL_METHOD_CALL;

    return IJsonValue_GetArray( internal_value, value );
}

static HRESULT WINAPI json_object_GetNamedString( IJsonObject *iface, HSTRING name, HSTRING *value )
{
    IJsonValue *internal_value;
    JsonValueType value_type;
    HRESULT hr;

    TRACE( "iface %p, name %s, value %p.\n", iface, debugstr_hstring( name ), value );

    if (!value) return E_POINTER;
    if (FAILED(hr = IJsonObject_GetNamedValue( iface, name, &internal_value )))
        return hr;

    IJsonValue_Release( internal_value );
    IJsonValue_get_ValueType( internal_value, &value_type );
    if (value_type != JsonValueType_String) return E_ILLEGAL_METHOD_CALL;

    return IJsonValue_GetString( internal_value, value );
}

static HRESULT WINAPI json_object_GetNamedNumber( IJsonObject *iface, HSTRING name, DOUBLE *value )
{
    IJsonValue *internal_value;
    JsonValueType value_type;
    HRESULT hr;

    TRACE( "iface %p, name %s, value %p.\n", iface, debugstr_hstring( name ), value );

    if (!value) return E_POINTER;
    if (FAILED(hr = IJsonObject_GetNamedValue( iface, name, &internal_value )))
        return hr;

    IJsonValue_Release( internal_value );
    IJsonValue_get_ValueType( internal_value, &value_type );
    if (value_type != JsonValueType_Number) return E_ILLEGAL_METHOD_CALL;

    return IJsonValue_GetNumber( internal_value, value );
}

static HRESULT WINAPI json_object_GetNamedBoolean( IJsonObject *iface, HSTRING name, boolean *value )
{
    IJsonValue *internal_value;
    JsonValueType value_type;
    HRESULT hr;

    TRACE( "iface %p, name %s, value %p.\n", iface, debugstr_hstring( name ), value );

    if (!value) return E_POINTER;
    if (FAILED(hr = IJsonObject_GetNamedValue( iface, name, &internal_value )))
        return hr;

    IJsonValue_Release( internal_value );
    IJsonValue_get_ValueType( internal_value, &value_type );
    if (value_type != JsonValueType_Boolean) return E_ILLEGAL_METHOD_CALL;

    return IJsonValue_GetBoolean( internal_value, value );
}

static const struct IJsonObjectVtbl json_object_vtbl =
{
    json_object_QueryInterface,
    json_object_AddRef,
    json_object_Release,
    /* IInspectable methods */
    json_object_GetIids,
    json_object_GetRuntimeClassName,
    json_object_GetTrustLevel,
    /* IJsonObject methods */
    json_object_GetNamedValue,
    json_object_SetNamedValue,
    json_object_GetNamedObject,
    json_object_GetNamedArray,
    json_object_GetNamedString,
    json_object_GetNamedNumber,
    json_object_GetNamedBoolean,
};

DEFINE_IINSPECTABLE( json_value, IJsonValue, struct json_object, IJsonObject_iface )

static HRESULT WINAPI json_value_get_ValueType( IJsonValue *iface, JsonValueType *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    *value = JsonValueType_Object;
    return S_OK;
}

static HRESULT WINAPI json_value_Stringify( IJsonValue *iface, HSTRING *value )
{
    IIterable_IKeyValuePair_HSTRING_IJsonValue *iterable;
    IIterator_IKeyValuePair_HSTRING_IJsonValue *iterator;
    IKeyValuePair_HSTRING_IJsonValue **items;
    UINT32 dummy, length = 2, size;
    IMap_HSTRING_IJsonValue *map;
    HSTRING_BUFFER handle;
    WCHAR *buffer, *ptr;
    HSTRING *children;
    HRESULT hr;

    TRACE( "iface %p, value %p.\n", iface, value );

    if (FAILED(hr = IJsonValue_QueryInterface( iface, &IID_IMap_HSTRING_IJsonValue, (void **)&map ))) return hr;
    hr = IMap_HSTRING_IJsonValue_get_Size( map, &size );
    IMap_HSTRING_IJsonValue_Release( map );
    if (FAILED(hr)) return hr;
    length += size ? 2 * size - 1 : 0;
    if (FAILED(hr = IJsonValue_QueryInterface( iface, &IID_IIterable_IKeyValuePair_HSTRING_IJsonValue, (void **)&iterable ))) return hr;
    hr = IIterable_IKeyValuePair_HSTRING_IJsonValue_First( iterable, &iterator );
    IIterable_IKeyValuePair_HSTRING_IJsonValue_Release( iterable );
    if (FAILED(hr)) return hr;
    if (!(items = calloc( size, sizeof(*items) + 2 * sizeof(*children) )))
    {
        IIterator_IKeyValuePair_HSTRING_IJsonValue_Release( iterator );
        return E_OUTOFMEMORY;
    }

    children = (HSTRING *)(items + size);
    hr = IIterator_IKeyValuePair_HSTRING_IJsonValue_GetMany( iterator, size, items, &dummy );
    IIterator_IKeyValuePair_HSTRING_IJsonValue_Release( iterator );
    if (FAILED(hr))
    {
        free( items );
        return hr;
    }

    for (UINT32 i = 0; i < size; i++)
    {
        HSTRING escaped_key, key, value;
        IJsonValue *json_value;

        if (FAILED(hr = IKeyValuePair_HSTRING_IJsonValue_get_Key( items[i], &key ))) goto failed;
        hr = escape_string( key, &escaped_key );
        WindowsDeleteString( key );
        if (FAILED(hr)) goto failed;
        if (FAILED(hr = IKeyValuePair_HSTRING_IJsonValue_get_Value( items[i], &json_value )))
        {
            WindowsDeleteString( escaped_key );
            goto failed;
        }

        hr = IJsonValue_Stringify( json_value, &value );
        IJsonValue_Release( json_value );
        if (FAILED(hr))
        {
            WindowsDeleteString( escaped_key );
            goto failed;
        }

        length += WindowsGetStringLen( (children[2 * i] = escaped_key) );
        length += WindowsGetStringLen( (children[2 * i + 1] = value) );
        IKeyValuePair_HSTRING_IJsonValue_Release( items[i] );
        continue;

    failed:
        for (UINT32 j = i; j < size; j++) IKeyValuePair_HSTRING_IJsonValue_Release( items[j] );
        for (UINT32 j = 0; j < i; j++)
        {
            WindowsDeleteString( children[2 * j] );
            WindowsDeleteString( children[2 * j + 1] );
        }
        free( items );
        return hr;
    }

    if (FAILED(hr = WindowsPreallocateStringBuffer( length, &buffer, &handle )))
    {
        for (UINT32 i = 0; i < size; i++)
        {
            WindowsDeleteString( children[2 * i] );
            WindowsDeleteString( children[2 * i + 1] );
        }
        free( items );
        return hr;
    }

    ptr = buffer;
    *(ptr++) = '{';
    for (UINT32 i = 0; i < size; i++)
    {
        UINT32 child_length;
        const WCHAR *child = WindowsGetStringRawBuffer( children[2 * i], &child_length );
        memcpy( ptr, child, child_length * sizeof(WCHAR) );
        ptr += child_length;
        *(ptr++) = ':';
        child = WindowsGetStringRawBuffer( children[2 * i + 1], &child_length );
        memcpy( ptr, child, child_length * sizeof(WCHAR) );
        ptr += child_length;
        *(ptr++) = ',';
    }

    buffer[length - 1] = '}';
    hr = WindowsPromoteStringBuffer( handle, value );
    if (FAILED(hr)) WindowsDeleteStringBuffer( handle );
    return hr;
}

static HRESULT WINAPI json_value_GetString( IJsonValue *iface, HSTRING *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return E_ILLEGAL_METHOD_CALL;
}

static HRESULT WINAPI json_value_GetNumber( IJsonValue *iface, DOUBLE *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return E_ILLEGAL_METHOD_CALL;
}

static HRESULT WINAPI json_value_GetBoolean( IJsonValue *iface, BOOLEAN *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return E_ILLEGAL_METHOD_CALL;
}

static HRESULT WINAPI json_value_GetArray( IJsonValue *iface, IJsonArray **value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return E_ILLEGAL_METHOD_CALL;
}

static HRESULT WINAPI json_value_GetObject( IJsonValue *iface, IJsonObject **value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    return IJsonValue_QueryInterface( iface, &IID_IJsonObject, (void **)value );
}

static const struct IJsonValueVtbl json_value_vtbl =
{
    json_value_QueryInterface,
    json_value_AddRef,
    json_value_Release,
    /* IInspectable methods */
    json_value_GetIids,
    json_value_GetRuntimeClassName,
    json_value_GetTrustLevel,
    /* IJsonValue methods */
    json_value_get_ValueType,
    json_value_Stringify,
    json_value_GetString,
    json_value_GetNumber,
    json_value_GetBoolean,
    json_value_GetArray,
    json_value_GetObject,
};

struct json_object_statics
{
    IActivationFactory IActivationFactory_iface;
    LONG ref;
};

static inline struct json_object_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct json_object_statics, IActivationFactory_iface );
}

static HRESULT WINAPI factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct json_object_statics *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IActivationFactory ))
    {
        *out = &impl->IActivationFactory_iface;
        IInspectable_AddRef( *out );
        return S_OK;
    }

    FIXME( "%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid( iid ) );
    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI factory_AddRef( IActivationFactory *iface )
{
    struct json_object_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p increasing refcount to %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI factory_Release( IActivationFactory *iface )
{
    struct json_object_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );
    TRACE( "iface %p decreasing refcount to %lu.\n", iface, ref );
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
    struct json_object *impl;
    HRESULT hr;

    static const struct map_iids iids =
    {
        .map = &IID_IMap_HSTRING_IJsonValue,
        .view = &IID_IMapView_HSTRING_IJsonValue,
        .iterable = &IID_IIterable_IKeyValuePair_HSTRING_IJsonValue,
        .iterator = &IID_IIterator_IKeyValuePair_HSTRING_IJsonValue,
        .pair = &IID_IKeyValuePair_HSTRING_IJsonValue,
    };

    TRACE( "iface %p, instance %p.\n", iface, instance );

    *instance = NULL;
    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;
    impl->IJsonObject_iface.lpVtbl = &json_object_vtbl;
    impl->IJsonValue_iface.lpVtbl = &json_value_vtbl;
    impl->ref = 1;

    if (FAILED(hr = multi_threaded_map_create( &iids, (IInspectable *)&impl->IJsonObject_iface, &impl->inner )))
    {
        free( impl );
        return hr;
    }

    *instance = (IInspectable *)&impl->IJsonObject_iface;
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

static struct json_object_statics json_object_statics =
{
    {&factory_vtbl},
    1,
};

IActivationFactory *json_object_factory = &json_object_statics.IActivationFactory_iface;
