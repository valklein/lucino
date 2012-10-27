#include "lucene.h"
#include "lcn_index.h"
#include "field.h"
#include "fs_field.h"

static void
lcn_field_update_field_type( lcn_field_t *field )
{
    (void) lcn_field_type_init( &(field->field_type));

    if ( ( 0 != (LCN_FIELD_INDEXED & field->flags ) ))
    {
        lcn_field_type_set_indexed( &(field->field_type), LCN_TRUE );
    }

    if ( ( 0 != (LCN_FIELD_TOKENIZED & field->flags ) ) )
    {
        lcn_field_type_set_tokenized( &(field->field_type), LCN_TRUE );
    }

    if ( 0 != (LCN_FIELD_STORED & field->flags ) )
    {
        lcn_field_type_set_stored( &(field->field_type), LCN_TRUE );
    }

    if ( ( 0 != (LCN_FIELD_FIXED_SIZE & field->flags )))
    {
        lcn_field_type_set_fixed_size( &(field->field_type), LCN_TRUE );
    }

    if ( ( 0 != (LCN_FIELD_BINARY & field->flags ) ))
    {
        lcn_field_type_set_binary( &(field->field_type), LCN_TRUE );
    }

    if ( ( 0 != (LCN_FIELD_STORE_TERM_VECTOR & field->flags )))
    {
        lcn_field_type_set_store_term_vectors( &(field->field_type), LCN_TRUE );
    }

    if ( ( 0 != (LCN_FIELD_STORE_POSITION_WITH_TV & field->flags )))
    {
        lcn_field_type_set_store_term_vector_positions( &(field->field_type), LCN_TRUE );
    }

    if( ( 0 != (LCN_FIELD_STORE_OFFSET_WITH_TV & field->flags )))
    {
        lcn_field_type_set_store_term_vector_offsets( &(field->field_type), LCN_TRUE );
    }

    if (( 0 != (LCN_FIELD_OMIT_NORMS & field->flags ) ))
    {
        lcn_field_type_set_omit_norms( &(field->field_type), LCN_TRUE );
    }
}

const char *
lcn_field_name( lcn_field_t *field )
{
    return field->name;
}

apr_status_t
lcn_field_int_value( lcn_field_t *field,
                     unsigned int *val )
{
    apr_status_t s = APR_SUCCESS;

    do
    {
        LCNASSERTM( lcn_field_is_fixed_size( field ), LCN_ERR_INVALID_ARGUMENT, field->name );
        LCNCE( field->accessor->get_int_value( field->accessor, val, field->doc_id ));
    }
    while(0);

    return s;
}


apr_status_t
lcn_field_binary_value( lcn_field_t *field,
                        char **value,
                        unsigned int *len,
                        apr_pool_t *pool )
{
    apr_status_t s = APR_SUCCESS;

    do
    {
        if ( field->is_lazy )
        {
            field->accessor->get_binary_value( field->accessor,
                                               value,
                                               len,
                                               field->doc_id,
                                               pool );
        }
        else
        {
            LCNPV( *value = (char*) apr_palloc( pool, field->size * sizeof(char)), APR_ENOMEM );
            memcpy( *value, field->value, field->size );
            *len = field->size;
        }
    }
    while(0);

    return s;
}

const char *
lcn_field_value( lcn_field_t *field )
{
    return field->value;
}

lcn_bool_t
lcn_field_is_indexed( lcn_field_t *field )
{
    return lcn_field_type_is_indexed( field->field_type );
}

lcn_bool_t
lcn_field_is_tokenized( lcn_field_t *field )
{
    return lcn_field_type_is_tokenized( field->field_type );
}

lcn_bool_t
lcn_field_is_stored( lcn_field_t *field )
{
    return lcn_field_type_is_stored( field->field_type );
}

lcn_bool_t
lcn_field_is_fixed_size( lcn_field_t *field )
{
    return lcn_field_type_is_fixed_size( field->field_type );
}

const char *
lcn_field_default_value( lcn_field_t *field )
{
    return  field->default_value;
}

lcn_bool_t
lcn_field_is_binary( lcn_field_t *field )
{
    return lcn_field_type_is_binary( field->field_type );
}

lcn_bool_t
lcn_field_is_term_vector_stored( lcn_field_t *field )
{
    return lcn_field_type_is_store_term_vectors( field->field_type );
}

lcn_bool_t
lcn_field_store_position_with_term_vector( lcn_field_t *field )
{
    return lcn_field_type_is_store_term_vector_positions( field->field_type );
}

lcn_bool_t
lcn_field_store_offset_with_term_vector( lcn_field_t *field )
{
    return lcn_field_type_is_store_term_vector_offsets( field->field_type );
}

lcn_bool_t
lcn_field_omit_norms( lcn_field_t *field )
{
    return lcn_field_type_is_omit_norms( field->field_type );
}

unsigned int
lcn_field_size( lcn_field_t *field )
{
    if ( lcn_field_is_binary( field ) || lcn_field_is_fixed_size( field ))
    {
        return field->size;
    }

    return strlen( field->value );
}

float
lcn_field_get_boost( lcn_field_t *field )
{
    return field->boost;
}

void
lcn_field_set_analyzer( lcn_field_t *field, lcn_analyzer_t *analyzer )
{
    field->analyzer = analyzer;
}

apr_status_t
lcn_field_get_analyzer( lcn_field_t *field,
                        lcn_analyzer_t **analyzer )
{
    if ( NULL != field->analyzer )
    {
        *analyzer = field->analyzer;
        return APR_SUCCESS;
    }

    return LCN_ERR_FIELD_ANALYZER_NOT_INITIALIZED;
}

apr_status_t
lcn_field_create_impl( lcn_field_t **field,
                       const char *name,
                       const char *value,
                       unsigned int flags,
                       unsigned int copy_value,
                       unsigned int value_size,
                       apr_pool_t *pool )
{
    apr_status_t s;

    do
    {
        LCNPV( *field = (lcn_field_t*) apr_pcalloc( pool, sizeof(lcn_field_t) ), APR_ENOMEM );

        (*field)->boost = (float) 1.0;
        (*field)->name = lcn_atom_get_str ( name );

        if ( LCN_FIELD_VALUE_COPY == copy_value )
        {
            if ( NULL != value )
            {
                LCNPV( (*field)->value = (char*) apr_palloc( pool, sizeof(char*) * (value_size + 1) ), APR_ENOMEM );
                memcpy( (*field)->value, value, (size_t) value_size );
                ((*field)->value)[ value_size ] = '\0';
            }
        }
        else if ( LCN_FIELD_NO_VALUE_COPY == copy_value )
        {
            (*field)->value = (char*)value;
        }
        else
        {
            return LCN_ERR_FIELD_UNKNOWN_COPY_VALUE_FLAG;
        }

        (*field)->flags = flags;

        lcn_field_update_field_type( *field );

        if ( copy_value == LCN_FIELD_VALUE_COPY )
        {
            lcn_field_type_set_copy_value( &((*field)->field_type ), LCN_TRUE );
        }

        (*field)->analyzer = NULL;

        if ( !lcn_field_is_indexed( *field ) && lcn_field_omit_norms( *field ) )
        {
            s = LCN_ERR_FIELD_INFO_OMIT_NORMS_ON_UNINDEXED;
        }

    }
    while(0);

    return s;
}

apr_status_t
lcn_field_create_impl_ft( lcn_field_t **field,
                          const char *name,
                          const char *value,
                          unsigned int value_size,
                          lcn_field_type_t ft,
                          apr_pool_t *pool )
{
    apr_status_t s;

    do
    {
        LCNPV( *field = (lcn_field_t*) apr_pcalloc( pool, sizeof(lcn_field_t) ), APR_ENOMEM );

        (*field)->boost = (float) 1.0;
        (*field)->name = lcn_atom_get_str ( name );

        if ( lcn_field_type_is_copy_value( ft ) )
        {
            if ( NULL != value )
            {
                LCNPV( (*field)->value = (char*) apr_palloc( pool, sizeof(char*) * (value_size + 1) ), APR_ENOMEM );
                memcpy( (*field)->value, value, (size_t) value_size );
                ((*field)->value)[ value_size ] = '\0';
            }
        }
        else
        {
            (*field)->value = (char*)value;
        }

        (*field)->field_type = ft;
        (*field)->analyzer = NULL;

        if ( lcn_field_type_is_binary( ft ) )
        {
            (*field)->size = value_size;
        }

        if ( !lcn_field_type_is_indexed( ft ) && lcn_field_type_is_omit_norms( ft ))
        {
            s = LCN_ERR_FIELD_INFO_OMIT_NORMS_ON_UNINDEXED;
        }
    }
    while(0);

    return s;
}


unsigned int
lcn_field_flags( lcn_field_t* field )
{
    return field->flags;
}

apr_status_t
lcn_field_flags_set( lcn_field_t* field, unsigned int flags )
{
    field->flags = flags;

    lcn_field_update_field_type( field );

    if ( !lcn_field_is_indexed( field ) && lcn_field_omit_norms( field ) )
    {
        return LCN_ERR_FIELD_INFO_OMIT_NORMS_ON_UNINDEXED;
    }

    return APR_SUCCESS;
}

apr_status_t
lcn_field_create( lcn_field_t **field,
                  const char *name,
                  const char *value,
                  unsigned int flags,
                  unsigned int copy_value,
                  apr_pool_t *pool )
{
    apr_status_t s;
    LCNCR( lcn_field_create_impl( field, name, value,
                                  flags, copy_value,
                                  strlen( value ), pool ) );
    return s;
}

apr_status_t
lcn_field_create_binary( lcn_field_t **field,
                         const char *name,
                         const char *value,
                         unsigned int copy_value,
                         unsigned int size,
                         apr_pool_t *pool )
{
    apr_status_t s;

    LCNCR( lcn_field_create_impl( field,
                                  name,
                                  value,
                                  LCN_FIELD_BINARY | LCN_FIELD_STORED,
                                  copy_value,
                                  size,
                                  pool ) );
    (*field)->size = size;

    return s;
}

apr_status_t
lcn_field_create_fixed_size( lcn_field_t **field,
                             const char *name,
                             const char *value,
                             const char *default_value,
                             unsigned int size,
                             apr_pool_t *pool )
{
    apr_status_t s;

    do
    {
        unsigned int bytes = (size>>3) + ((size&7) ? 1 : 0 );

        LCNCR( lcn_field_create_impl( field,
                                      name,
                                      value,
                                      0 /* flags */,
                                      LCN_FIELD_VALUE_COPY,
                                      bytes,
                                      pool ) );

        (*field)->flags |= LCN_FIELD_FIXED_SIZE;
        (*field)->flags |= LCN_FIELD_STORED;
        (*field)->flags |= LCN_FIELD_BINARY;

        lcn_field_update_field_type( *field );

        if ( NULL != default_value )
        {
            LCNPV( (*field)->default_value = (char*) apr_palloc( pool, bytes ),APR_ENOMEM );
            memcpy( (*field)->default_value, default_value, bytes );

            if ( NULL != value )
            {
                memcpy( (*field)->value, value, bytes );
            }
        }

        (*field)->size = size;
    }
    while(0);

    return s;
}

apr_status_t
lcn_field_create_fixed_size_by_ints( lcn_field_t **field,
                                     const char *name,
                                     unsigned int val,
                                     unsigned int default_value,
                                     unsigned int size,
                                     apr_pool_t *pool )
{
    apr_status_t s;

    char value[ sizeof(unsigned int) ];
    char defval[ sizeof( unsigned int )];

    LCNCR( lcn_fs_field_int_to_char( val, value, size ) );
    LCNCR( lcn_fs_field_int_to_char( default_value, defval, size ) );

    return lcn_field_create_fixed_size( field, name, value, defval, size, pool );
}


apr_status_t
lcn_field_create_binary_ft( lcn_field_t **field,
                            const char *name,
                            const char *value,
                            unsigned int value_size,
                            lcn_field_type_t ft,
                            apr_pool_t *pool )
{
    apr_status_t s;

    LCNCR( lcn_field_create_impl_ft( field, name, value, value_size, ft, pool ));

    return s;
}

apr_status_t
lcn_field_create_ft( lcn_field_t **field,
                     const char *name,
                     const char *value,
                     lcn_field_type_t ft,
                     apr_pool_t *pool )
{
    apr_status_t s;

    LCNCR( lcn_field_create_impl_ft( field, name, value, strlen( value ), ft, pool ));

    return s;
}
