#include "lcn_util.h"
#include "segment_infos.h"
#include "directory.h"
#include "index_file_names.h"
#include "index_input.h"
#include "io_context.h"

/********************************************************
 *                                                      *
 * Functions of segment_info                            *
 *                                                      *
 * constructor is inlined in lcn_segment_infos_add_info *
 *                                                      *
 ********************************************************/

const char *
lcn_segment_info_name( lcn_segment_info_t *segment_info )
{
    return segment_info->name;
}

lcn_directory_t *
lcn_segment_info_directory( lcn_segment_info_t *segment_info )
{
    return segment_info->directory;
}

unsigned int
lcn_segment_info_doc_count( lcn_segment_info_t *segment_info )
{
    return segment_info->doc_count;
}

apr_status_t
lcn_segment_info_has_deletions( lcn_segment_info_t *segment_info,
                                lcn_bool_t *has_deletions )
{
    apr_status_t s;
    char *del_file;
    apr_pool_t *pool = NULL;

    do
    {
        LCNCE( apr_pool_create( &pool, segment_info->directory->pool ));
        LCNPV( del_file = apr_pstrcat( pool, segment_info->name, ".del", NULL ), APR_ENOMEM );
        LCNCE( lcn_directory_file_exists ( segment_info->directory, del_file, has_deletions ) );
    }
    while(0);

    if ( NULL != pool )
    {
        apr_pool_destroy( pool );
    }

    return s;
}

/********************************************************
 *                                                      *
 * Functions of segment_infos                           *
 *                                                      *
 * read current version not implemented. is a conv.     *
 * function                                             *
 *                                                      *
 ********************************************************/

apr_status_t
lcn_segment_infos_create( lcn_segment_infos_t **segment_infos,
                          apr_pool_t *pool )
{
    apr_status_t s;

    do
    {
        LCNPV( *segment_infos = apr_pcalloc( pool, sizeof(lcn_segment_infos_t)), APR_ENOMEM );
        LCNCE( apr_pool_create( &((*segment_infos)->subpool), pool ) );
        LCNCE( lcn_list_create( &((*segment_infos)->list), 10, (*segment_infos)->subpool ) );
        (*segment_infos)->pool = pool;
    }
    while(0);

    return s;
}

void
lcn_segment_infos_remove( lcn_segment_infos_t *segment_infos, unsigned int i )
{
    lcn_list_remove( segment_infos->list, i );
}

unsigned int
lcn_segment_infos_size( lcn_segment_infos_t *segment_infos )
{
    return lcn_list_size( segment_infos->list );
}

apr_uint64_t
lcn_segment_infos_version( lcn_segment_infos_t *segment_infos )
{
    return segment_infos->version;
}

int
lcn_segment_infos_format( lcn_segment_infos_t *segment_infos )
{
    return segment_infos->format;
}

apr_status_t
lcn_segment_infos_get_next_name( lcn_segment_infos_t *segment_infos,
                                 char **seg_name,
                                 apr_pool_t *pool )
{
    apr_status_t s;

    do
    {
        char buf[10];
        buf[0] = '_';
        lcn_itoa36( segment_infos->counter, buf + 1 );
        LCNPV( *seg_name = apr_pstrdup( pool, buf ), APR_ENOMEM );
        segment_infos->counter++;
    }
    while(0);

    return s;
}

apr_status_t
lcn_segment_infos_get( lcn_segment_infos_t *segment_infos,
                       lcn_segment_info_t **segment_info,
                       unsigned int nth )
{
    *segment_info = lcn_list_get( segment_infos->list, nth );
    return APR_SUCCESS;
}

apr_status_t
lcn_segment_infos_add_info ( lcn_segment_infos_t *segment_infos,
                             lcn_directory_t *directory,
                             const char *name,
                             unsigned int count )
{
    apr_status_t s;

    do
    {
        lcn_segment_info_t *segment_info;
        apr_pool_t *pool = lcn_list_pool( segment_infos->list );

        LCNPV( segment_info = (lcn_segment_info_t*) apr_pcalloc( pool, sizeof(lcn_segment_info_t) ),
               APR_ENOMEM );

        LCNCE( lcn_list_add( segment_infos->list, segment_info ) );
        LCNPV( segment_info->name = apr_pstrdup( pool, name ), APR_ENOMEM );

        segment_info->doc_count = count;
        segment_info->directory = directory;

        if ( 0 == (segment_infos->counter % 1000))
        {
            unsigned int lsize = lcn_list_size( segment_infos->list );
            unsigned int i;
            lcn_list_t *list;
            apr_pool_t *p;

            LCNCE( apr_pool_create( &p, segment_infos->pool ));
            LCNCE( lcn_list_create( &list, 100, p ));

            for (i = 0; i < lsize; i++ )
            {
                lcn_segment_info_t *si = (lcn_segment_info_t*) lcn_list_get( segment_infos->list, i );
                lcn_segment_info_t *new_si;

                LCNPV( new_si = (lcn_segment_info_t*) apr_pcalloc( p, sizeof(lcn_segment_info_t) ),
                       APR_ENOMEM );

                LCNCE( lcn_list_add( list, new_si ) );
                LCNPV( new_si->name = apr_pstrdup( p, si->name ), APR_ENOMEM );

                new_si->doc_count = si->doc_count;
                new_si->directory = si->directory;
            }

            if ( s )
            {
                break;
            }

            apr_pool_destroy( segment_infos->subpool );
            segment_infos->subpool = p;
            segment_infos->list = list;
        }
    }
    while(0);

    return s;
}


apr_status_t
lcn_segment_infos_write( lcn_segment_infos_t *segment_infos,
                         lcn_directory_t *dir )
{
    apr_status_t s;
    apr_pool_t *pool = NULL;
    lcn_ostream_t *os = NULL;

    do
    {

        unsigned int i = 0;
        unsigned int size = lcn_segment_infos_size( segment_infos );

        LCNCE( apr_pool_create( &pool, dir->pool ));
        LCNCE( lcn_directory_create_output( dir, &os, "segments.new", pool ) );
        LCNCE( lcn_ostream_write_int( os, LCN_SEGMENT_INFOS_FORMAT ));
        LCNCE( lcn_ostream_write_long( os, ++(segment_infos->version) ));
        LCNCE( lcn_ostream_write_int( os, segment_infos->counter ));
        LCNCE( lcn_ostream_write_int( os, size ));

        for( i = 0; i < size; i++ )
        {
            lcn_segment_info_t *info;
            LCNCE( lcn_segment_infos_get( segment_infos, &info, i ) );
            LCNCE( lcn_ostream_write_string( os, info->name ) );
            LCNCE( lcn_ostream_write_int( os, info->doc_count ));
        }

        if ( s )
        {
            break;
        }

        LCNCE( lcn_ostream_close( os ) );
        os = NULL;

        LCNCE( lcn_directory_rename_file( dir, "segments.new", LCN_INDEX_FILE_NAMES_SEGMENTS ) );
    }
    while(0);

    if ( NULL != os )
    {
        (void) lcn_ostream_close( os );
    }

    if ( NULL != pool )
    {
        apr_pool_destroy( pool );
    }

    return s;
}


apr_status_t
lcn_segment_infos_has_separate_norms( lcn_segment_info_t *segment_info,
                                      lcn_bool_t *flag,
                                      apr_pool_t *pool)
{
    apr_status_t s = APR_SUCCESS;
    apr_pool_t *child_pool;
    *flag = LCN_FALSE;

    do
    {
        lcn_list_t *file_list;
        char *pattern;
        int i;

        LCNCE( apr_pool_create( &child_pool, pool ));
        LCNCE( lcn_directory_list(segment_info->directory, &file_list, child_pool) );
        LCNPV( pattern = apr_pstrcat(child_pool, segment_info->name, ".s", NULL ), LCN_ERR_NULL_PTR);

        unsigned int pattern_len = strlen(pattern);

        for ( i = 0; i < lcn_list_size( file_list ); i++ )
        {
            char *file_name = lcn_list_get( file_list, i );

            if( lcn_string_starts_with(file_name,pattern)  && isdigit( file_name[pattern_len] ) )
            {
                *flag = LCN_TRUE;
            }
        }
    }
    while(0);

    if ( NULL != child_pool )
    {
        apr_pool_destroy( child_pool );
    }

    return s;
}


/**
 * Lucene 4.0
 */

static apr_status_t
generation_from_segments_file_name(char *file_name, apr_int64_t *gen)
{
    if ( 0 == strcmp( file_name, LCN_INDEX_FILE_NAMES_SEGMENTS ))
    {
        *gen = 0;
        return APR_SUCCESS;
    }

    if ( 0 == strncmp( file_name, LCN_INDEX_FILE_NAMES_SEGMENTS, strlen( LCN_INDEX_FILE_NAMES_SEGMENTS )) &&
         0 != strcmp( file_name, LCN_INDEX_FILE_NAMES_SEGMENTS_GEN ))
    {
        char *end = NULL;
        apr_int64_t generation = apr_strtoi64( file_name + 1 + strlen(LCN_INDEX_FILE_NAMES_SEGMENTS),  &end,
                                               LCN_INDEX_FILE_NAMES_MAX_RADIX );

        if ( end == (file_name + strlen(file_name)))
        {
            *gen = generation;
            return APR_SUCCESS;
        }
    }

    return LCN_ERR_SEGMENT_INFOS_INVALID_SEGMENTS_FILE_NAME;
}

static apr_status_t
get_last_commit_generation( lcn_list_t *files, apr_int64_t *gen )
{
    apr_status_t s = APR_SUCCESS;
    apr_int64_t max = -1;
    int i;

    do
    {
        if ( NULL == files || lcn_list_size( files ) == 0 )
        {
            *gen = -1;
            break;
        }

        for( i = 0; i < lcn_list_size( files ); i++ )
        {
            char *file_name = (char*) lcn_list_get( files, i );

            if ( 0 == strncmp( file_name, LCN_INDEX_FILE_NAMES_SEGMENTS, strlen( LCN_INDEX_FILE_NAMES_SEGMENTS )) &&
                 0 != strcmp( file_name, LCN_INDEX_FILE_NAMES_SEGMENTS_GEN ))
            {
                apr_int64_t generation;

                LCNCE( generation_from_segments_file_name(file_name, &generation));

                if (generation > max)
                {
                    max = generation;
                }
            }
        }

        *gen = max;
    }
    while(0);

    return s;
}

/**
 * List the directory and use the highest
 * segments_N file.  This method works well as long
 * as there is no stale caching on the directory
 * contents (NOTE: NFS clients often have such stale caching):
 */
static apr_status_t
find_segments_file( lcn_directory_t *directory,
                    char **segments_file,
                    apr_pool_t *pool )
{
    apr_status_t s = APR_SUCCESS;

    do
    {
        lcn_bool_t exists;
        lcn_list_t *file_list;
        apr_int64_t gen_a, gen_b;
        lcn_index_input_t *gen_input = NULL;

        /* first check for old format (still 2.4) */

        LCNCE( lcn_directory_file_exists ( directory, LCN_INDEX_FILE_NAMES_SEGMENTS, &exists ));


        if ( LCN_TRUE == exists )
        {
            *segments_file = apr_pstrdup( pool, LCN_INDEX_FILE_NAMES_SEGMENTS );
            break;
        }

        /* now assume we are Lucene 4.0 */

        LCNCE( lcn_directory_list( directory, &file_list, pool ) );
        LCNCE( get_last_commit_generation( file_list, &gen_a ));

        /**
         * Also open segments.gen and read its
         * contents.  Then we take the larger of the two
         * gens.  This way, if either approach is hitting
         * a stale cache (NFS) we have a better chance of
         * getting the right generation.
         */
        gen_b = -1;
#if 0
        LCNCE( lcn_directory_open_input_( directory,
                                          LCN_INDEX_FILE_NAMES_SEGMENTS,
                                          LCN_IO_CONTEXT_READONCE,
                                          &gen_input,
                                          pool ));
#endif
        //genInput = directory.openInput(IndexFileNames.SEGMENTS_GEN, IOContext.READONCE);

       *segments_file = NULL;
    }
    while(0);

    return APR_SUCCESS;
}


apr_status_t
lcn_segment_infos_read_directory( lcn_segment_infos_t *segment_infos,
                                  lcn_directory_t *dir )
{
    apr_status_t s = APR_SUCCESS;
    apr_pool_t *cp = NULL;
    lcn_istream_t *is = NULL;

    do
    {
        int format, size, i;
        char *segments_file;

        LCNCE( apr_pool_create( &cp, segment_infos->pool ));

        segment_infos->generation = segment_infos->last_generation = -1;

        /**
         * WARNING: we simplify a lot here. Should be completed.
         */

        LCNCE( find_segments_file( dir, &segments_file, cp ));

        LCNASSERT( NULL != segments_file, LCN_ERR_NOT_REGULAR_FILE );

        if ( 0 == strcmp( LCN_INDEX_FILE_NAMES_SEGMENTS, segments_file ))
        {
            /* execute old code here (to delete as soon as possible)  */

            if (( s = lcn_directory_open_input( dir, &is, LCN_INDEX_FILE_NAMES_SEGMENTS, cp )))
            {
                break;
            }

            LCNCE( lcn_istream_read_int( is, &format ) );

            if ( format < 0 )  /* file contains explicit format info */
            {
                int counter;
                LCNASSERT( format >= LCN_SEGMENT_INFOS_FORMAT, LCN_ERR_SEGMENT_INFOS_UNKNOWN_FILE_FORMAT );
                LCNCE( lcn_istream_read_ulong( is, &(segment_infos->version) ));
                LCNCE( lcn_istream_read_int( is, &counter ) );
                segment_infos->counter = counter;
                segment_infos->format = format;
            }

            LCNCE( lcn_istream_read_int( is, (int*)&size ));

            for( i = 0; i < size; i++ )
            {
                char *name;
                int doc_count;
                unsigned int len;

                LCNCE( lcn_istream_read_string( is, &name, &len, lcn_list_pool( segment_infos->list )));
                LCNCE( lcn_istream_read_int( is, (int*)&doc_count ) );
                LCNCE( lcn_segment_infos_add_info( segment_infos, dir, name, doc_count ));
            }

            dir->segments_format = segment_infos->format;
            dir->segments_format_is_init = LCN_TRUE;
        }
        else
        {
        /* Lucene 4.0 */
        }
    }
    while(0);

    if ( NULL != is )
    {
        apr_status_t stat = lcn_istream_close( is );
        s = ( s ? s : stat );
    }

    if ( NULL != cp )
    {
        apr_pool_destroy( cp );
    }

    return s;
}
