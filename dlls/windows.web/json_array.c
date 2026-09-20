/* WinRT Windows.Data.Json.JsonArray Implementation
 *
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

struct json_array
{
    IJsonArray IJsonArray_iface;
    IJsonValue IJsonValue_iface;
    LONG ref;
    IInspectable *inner;
};

static inline struct json_array *impl_from_IJsonArray( IJsonArray *iface )
{
    return CONTAINING_RECORD( iface, struct json_array, IJsonArray_iface );
}

static HRESULT WINAPI json_array_QueryInterface( IJsonArray *iface, REFIID iid, void **out )
{
    struct json_array *impl = impl_from_IJsonArray( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid( iid ), out );

    if (IsEqualGUID( iid, &IID_IUnknown ) ||
        IsEqualGUID( iid, &IID_IInspectable ) ||
        IsEqualGUID( iid, &IID_IAgileObject ) ||
        IsEqualGUID( iid, &IID_IJsonArray ))
    {
        *out = &impl->IJsonArray_iface;
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

static ULONG WINAPI json_array_AddRef( IJsonArray *iface )
{
    struct json_array *impl = impl_from_IJsonArray( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI json_array_Release( IJsonArray *iface )
{
    struct json_array *impl = impl_from_IJsonArray( iface );
    ULONG ref = InterlockedDecrement( &impl->ref );

    TRACE( "iface %p, ref %lu.\n", iface, ref );

    if (!ref)
    {
        IInspectable_Release( impl->inner );
        free( impl );
    }
    return ref;
}

static HRESULT WINAPI json_array_GetIids( IJsonArray *iface, ULONG *iid_count, IID **iids )
{
    FIXME( "iface %p, iid_count %p, iids %p stub!\n", iface, iid_count, iids );
    return E_NOTIMPL;
}

static HRESULT WINAPI json_array_GetRuntimeClassName( IJsonArray *iface, HSTRING *class_name )
{
    FIXME( "iface %p, class_name %p stub!\n", iface, class_name );
    return E_NOTIMPL;
}

static HRESULT WINAPI json_array_GetTrustLevel( IJsonArray *iface, TrustLevel *trust_level )
{
    FIXME( "iface %p, trust_level %p stub!\n", iface, trust_level );
    return E_NOTIMPL;
}

static HRESULT WINAPI json_array_GetObjectAt( IJsonArray *iface, UINT32 index, IJsonObject **value )
{
    struct json_array *impl = impl_from_IJsonArray( iface );
    IVector_IJsonValue *vector;
    IJsonValue *json_value;
    HRESULT hr;

    TRACE( "iface %p, index %u, value %p\n", iface, index, value );

    if (!value) return E_INVALIDARG;
    if (FAILED(hr = IInspectable_QueryInterface( impl->inner, &IID_IVector_IJsonValue, (void **)&vector ))) return hr;
    hr = IVector_IJsonValue_GetAt( vector, index, &json_value );
    IVector_IJsonValue_Release( vector );
    if (FAILED(hr)) return hr;
    hr = IJsonValue_GetObject( json_value, value );
    IJsonValue_Release( json_value );
    return hr;
}

static HRESULT WINAPI json_array_GetArrayAt( IJsonArray *iface, UINT32 index, IJsonArray **value )
{
    struct json_array *impl = impl_from_IJsonArray( iface );
    IVector_IJsonValue *vector;
    IJsonValue *json_value;
    HRESULT hr;

    TRACE( "iface %p, index %u, value %p\n", iface, index, value );

    if (!value) return E_INVALIDARG;
    if (FAILED(hr = IInspectable_QueryInterface( impl->inner, &IID_IVector_IJsonValue, (void **)&vector ))) return hr;
    hr = IVector_IJsonValue_GetAt( vector, index, &json_value );
    IVector_IJsonValue_Release( vector );
    if (FAILED(hr)) return hr;
    hr = IJsonValue_GetArray( json_value, value );
    IJsonValue_Release( json_value );
    return hr;
}

static HRESULT WINAPI json_array_GetStringAt( IJsonArray *iface, UINT32 index, HSTRING *value )
{
    struct json_array *impl = impl_from_IJsonArray( iface );
    IVector_IJsonValue *vector;
    IJsonValue *json_value;
    HRESULT hr;

    TRACE( "iface %p, index %u, value %p\n", iface, index, value );

    if (!value) return E_INVALIDARG;
    if (FAILED(hr = IInspectable_QueryInterface( impl->inner, &IID_IVector_IJsonValue, (void **)&vector ))) return hr;
    hr = IVector_IJsonValue_GetAt( vector, index, &json_value );
    IVector_IJsonValue_Release( vector );
    if (FAILED(hr)) return hr;
    hr = IJsonValue_GetString( json_value, value );
    IJsonValue_Release( json_value );
    return hr;
}

static HRESULT WINAPI json_array_GetNumberAt( IJsonArray *iface, UINT32 index, DOUBLE *value )
{
    struct json_array *impl = impl_from_IJsonArray( iface );
    IVector_IJsonValue *vector;
    IJsonValue *json_value;
    HRESULT hr;

    TRACE( "iface %p, index %u, value %p\n", iface, index, value );

    if (!value) return E_INVALIDARG;
    if (FAILED(hr = IInspectable_QueryInterface( impl->inner, &IID_IVector_IJsonValue, (void **)&vector ))) return hr;
    hr = IVector_IJsonValue_GetAt( vector, index, &json_value );
    IVector_IJsonValue_Release( vector );
    if (FAILED(hr)) return hr;
    hr = IJsonValue_GetNumber( json_value, value );
    IJsonValue_Release( json_value );
    return hr;
}

static HRESULT WINAPI json_array_GetBooleanAt( IJsonArray *iface, UINT32 index, boolean *value )
{
    struct json_array *impl = impl_from_IJsonArray( iface );
    IVector_IJsonValue *vector;
    IJsonValue *json_value;
    HRESULT hr;

    TRACE( "iface %p, index %u, value %p\n", iface, index, value );

    if (!value) return E_INVALIDARG;
    if (FAILED(hr = IInspectable_QueryInterface( impl->inner, &IID_IVector_IJsonValue, (void **)&vector ))) return hr;
    hr = IVector_IJsonValue_GetAt( vector, index, &json_value );
    IVector_IJsonValue_Release( vector );
    if (FAILED(hr)) return hr;
    hr = IJsonValue_GetBoolean( json_value, value );
    IJsonValue_Release( json_value );
    return hr;
}

static const struct IJsonArrayVtbl json_array_vtbl =
{
    json_array_QueryInterface,
    json_array_AddRef,
    json_array_Release,
    /* IInspectable methods */
    json_array_GetIids,
    json_array_GetRuntimeClassName,
    json_array_GetTrustLevel,
    /* IJsonArray methods */
    json_array_GetObjectAt,
    json_array_GetArrayAt,
    json_array_GetStringAt,
    json_array_GetNumberAt,
    json_array_GetBooleanAt,
};

DEFINE_IINSPECTABLE( json_value, IJsonValue, struct json_array, IJsonArray_iface )

static HRESULT WINAPI json_value_get_ValueType( IJsonValue *iface, JsonValueType *value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    if (!value) return E_POINTER;
    *value = JsonValueType_Array;
    return S_OK;
}

static HRESULT WINAPI json_value_Stringify( IJsonValue *iface, HSTRING *value )
{
    IVectorView_IJsonValue *view;
    IVector_IJsonValue *vector;
    UINT32 length = 2, size;
    HSTRING_BUFFER handle;
    WCHAR *buffer, *ptr;
    HSTRING *children;
    HRESULT hr;

    TRACE( "iface %p, value %p.\n", iface, value );

    if (FAILED(hr = IJsonValue_QueryInterface( iface, &IID_IVector_IJsonValue, (void **)&vector ))) return hr;
    hr = IVector_IJsonValue_GetView( vector, &view );
    IVector_IJsonValue_Release( vector );
    if (FAILED(hr)) return hr;
    if (FAILED(hr = IVectorView_IJsonValue_get_Size( view, &size )))
    {
        IVectorView_IJsonValue_Release( view );
        return hr;
    }

    if (!(children = calloc( size, sizeof(*children) )))
    {
        IVectorView_IJsonValue_Release( view );
        return E_OUTOFMEMORY;
    }

    length += size ? size - 1 : 0;
    for (UINT32 i = 0; i < size; i++)
    {
        IJsonValue *json_value;
        if (FAILED(hr = IVectorView_IJsonValue_GetAt( view, i, &json_value )))
        {
            IVectorView_IJsonValue_Release( view );
            for (UINT32 j = 0; j < i; j++) WindowsDeleteString( children[j] );
            return hr;
        }

        hr = IJsonValue_Stringify( json_value, &children[i] );
        IJsonValue_Release( json_value );
        if (FAILED(hr))
        {
            IVectorView_IJsonValue_Release( view );
            for (UINT32 j = 0; j < i; j++) WindowsDeleteString( children[j] );
            return hr;
        }

        length += WindowsGetStringLen( children[i] );
    }

    IVectorView_IJsonValue_Release( view );
    if (FAILED(hr = WindowsPreallocateStringBuffer( length, &buffer, &handle )))
    {
        for (UINT32 i = 0; i < size; i++) WindowsDeleteString( children[i] );
        return hr;
    }

    ptr = buffer;
    *(ptr++) = '[';
    for (UINT32 i = 0; i < size; i++)
    {
        UINT32 child_length;
        const WCHAR *child = WindowsGetStringRawBuffer( children[i], &child_length );
        memcpy( ptr, child, child_length * sizeof(WCHAR) );
        ptr += child_length;
        *(ptr++) = ',';
    }
    buffer[length - 1] = ']';

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
    TRACE( "iface %p, value2 %p.\n", iface, value );
    if (!value) return E_INVALIDARG;
    return IJsonValue_QueryInterface( iface, &IID_IJsonArray, (void **)value );
}

static HRESULT WINAPI json_value_GetObject( IJsonValue *iface, IJsonObject **value )
{
    TRACE( "iface %p, value %p.\n", iface, value );
    return E_ILLEGAL_METHOD_CALL;
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

struct json_array_statics
{
    IActivationFactory IActivationFactory_iface;
    LONG ref;
};

static inline struct json_array_statics *impl_from_IActivationFactory( IActivationFactory *iface )
{
    return CONTAINING_RECORD( iface, struct json_array_statics, IActivationFactory_iface );
}

static HRESULT WINAPI factory_QueryInterface( IActivationFactory *iface, REFIID iid, void **out )
{
    struct json_array_statics *impl = impl_from_IActivationFactory( iface );

    TRACE( "iface %p, iid %s, out %p.\n", iface, debugstr_guid(iid), out);

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
    struct json_array_statics *impl = impl_from_IActivationFactory( iface );
    ULONG ref = InterlockedIncrement( &impl->ref );
    TRACE( "iface %p, ref %lu.\n", iface, ref );
    return ref;
}

static ULONG WINAPI factory_Release( IActivationFactory *iface )
{
    struct json_array_statics *impl = impl_from_IActivationFactory( iface );
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
    struct json_array *impl;
    HRESULT hr;

    static const struct vector_iids iids =
    {
        .vector = &IID_IVector_IJsonValue,
        .view = &IID_IVectorView_IJsonValue,
        .iterable = &IID_IIterable_IJsonValue,
        .iterator = &IID_IIterator_IJsonValue,
    };

    TRACE( "iface %p, instance %p.\n", iface, instance );

    *instance = NULL;
    if (!(impl = calloc( 1, sizeof(*impl) ))) return E_OUTOFMEMORY;

    impl->IJsonArray_iface.lpVtbl = &json_array_vtbl;
    impl->IJsonValue_iface.lpVtbl = &json_value_vtbl;
    impl->ref = 1;

    if (FAILED(hr = vector_create( &iids, (IInspectable *)&impl->IJsonArray_iface, &impl->inner )))
    {
        free( impl );
        return hr;
    }

    *instance = (IInspectable *)&impl->IJsonArray_iface;
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

static struct json_array_statics json_array_statics =
{
    {&factory_vtbl},
    1,
};

IActivationFactory *json_array_factory = &json_array_statics.IActivationFactory_iface;
