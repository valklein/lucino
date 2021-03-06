#include "test_all.h"
#include "lcn_term_enum.h"
#include "lcn_search.h"

static void
test_indexing_terms( CuTest* tc )
{
    apr_pool_t *pool;
    lcn_directory_t *dir;
    lcn_document_t *doc;
    lcn_field_t *field, *field1;
    lcn_field_type_t indexed_type = {0};
    lcn_index_writer_t *index_writer;

    apr_pool_create( &pool, main_pool );

    LCN_TEST( lcn_field_type_set_indexed( &indexed_type, LCN_TRUE ));

    LCN_TEST( lcn_fs_directory_create( &dir, "new_dir", LCN_TRUE, pool ) );
    delete_files( tc, "new_dir" );
    LCN_TEST( lcn_index_writer_create_by_path( &index_writer,
                                               "new_dir",
                                               LCN_TRUE,
                                               pool ));

    LCN_TEST( lcn_document_create( &doc, pool ));
    LCN_TEST( lcn_field_create( &field, "text", "a", &indexed_type,pool ));
    LCN_TEST( lcn_field_create( &field1, "text", "b", &indexed_type, pool ));
    LCN_TEST( lcn_document_add_field( doc, field ));
    LCN_TEST( lcn_document_add_field( doc, field1 ));
    LCN_TEST( lcn_index_writer_add_document( index_writer, doc ));

    lcn_index_writer_close( index_writer ) ;
    lcn_index_writer_optimize( index_writer ) ;

    {
        lcn_term_t *term;
        lcn_query_t *query;
        lcn_searcher_t *searcher;
        lcn_hits_t *hits;

        LCN_TEST( lcn_term_create( &term, "text", "a", LCN_TERM_TEXT_COPY, pool ));
        LCN_TEST( lcn_term_query_create( &query, term, pool ));
        LCN_TEST( lcn_index_searcher_create_by_path( &searcher, "new_dir", pool ));
        LCN_TEST( lcn_searcher_search( searcher, &hits, query, NULL, pool ));

        CuAssertIntEquals( tc, 1, lcn_hits_length( hits ) );

        LCN_TEST( lcn_term_create( &term, "text", "b", LCN_TERM_TEXT_COPY, pool ));
        LCN_TEST( lcn_term_query_create( &query, term, pool ));
        LCN_TEST( lcn_searcher_search( searcher, &hits, query, NULL, pool ));

        CuAssertIntEquals( tc, 1, lcn_hits_length( hits ) );
    }

    apr_pool_destroy( pool );
}

CuSuite *
make_multiple_fields_suite (void)
{
    CuSuite *s= CuSuiteNew();
    SUITE_ADD_TEST(s, test_indexing_terms );
    return s;
}
