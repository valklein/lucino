#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <apr_getopt.h>
#include "lucene.h"
#include "lcn_index.h"
#include "indexinfo_func.h"
#include "cJSON.h"

/* FILE* lcn_log_stream; */

#define LIST_FIELDS  (1)
#define TERM_ENUM    (2)
#define SEARCH_DOCS  (3)

static void
print_help(void)
{
    printf( "\nUsage:\n\n"
            "indexwriter [options] <inputfile> <index_directory>\n\n"
            "\toptions:\n\n"
        );
}


static void
add_field_to_document(const cJSON* field, const lcn_document_t* document, apr_pool_t* main_pool)
{
	apr_pool_t* pool;
	apr_pool_create(&pool, main_pool);
	do {

	} while(0);
	apr_pool_destroy(pool);
}

static void
create_index_from_input(const char* input, const lcn_index_writer_t *index_writer, apr_pool_t* pool)
{
	cJSON *json = cJSON_Parse(input);
	//
	cJSON* documents = NULL;
	cJSON* fields = NULL;
	cJSON* document = NULL;
	cJSON* field = NULL;
	cJSON* field_name = NULL;

	if (json == NULL) {
		const char* err_ptr = cJSON_GetErrorPtr();
		if (err_ptr!=NULL) {
			printf("error: %s", err_ptr);
		}
	}

	documents = cJSON_GetObjectItemCaseSensitive(json, "documents");

	cJSON_ArrayForEach(document, documents) {
		apr_pool_t* child_pool;
		apr_pool_create(&child_pool, pool);

		fields = cJSON_GetObjectItemCaseSensitive(document, "fields");
		cJSON_ArrayForEach(field, fields) {
			apr_status_t s;
			field_name = cJSON_GetObjectItemCaseSensitive(field, "name");
			printf("tada: %s", field_name->valuestring);
			do {
				lcn_document_t* document;
				LCNCE( lcn_document_create( &document, child_pool) );
				// main part
				add_field_to_document(field, document, child_pool);
			} while(0);
			if (s!=APR_SUCCESS) {
				printf("error occured\n");
			}
		}
		apr_pool_destroy(child_pool);
	} 
}

static apr_status_t
create_index(const char* inputfile_name, const char* index_path, apr_pool_t* main_pool)
{
	apr_status_t s;
	do {
	apr_pool_t* pool;
	char *input;
	unsigned int length;
	lcn_index_writer_t* index_writer;
	
	apr_pool_create(&pool, main_pool);

	// read inputfile
	LCNCE( lcn_string_from_file( &input, inputfile_name, &length, main_pool ) );

	printf("%s", input);	
 
	// create index writer
	LCNCE( lcn_index_writer_create_by_path( &index_writer, index_path, LCN_TRUE, pool ) );

	// write index
	create_index_from_input(input, index_writer, pool);

	// close index writer
	LCNCE( lcn_index_writer_close( index_writer ) );
	
	apr_pool_destroy(pool);

	} while(0);
	return s;
}

/*main
 *
 *@agc
 *@argv
 *
 *@return 0
*/
int main( int argc, char** argv )
{
    apr_status_t s;
    apr_pool_t *pool;

    static const apr_getopt_option_t long_options[] =
        {
            {"help",               1,     FALSE },
            {"inputfile",		2,	TRUE},
            {"outputdir",		3,	TRUE},
            { NULL,                0,     0     }
    };


        char *index_path = NULL;
        char *inputfile_name = NULL;
        apr_getopt_t* os;
        int optch;
        const char* optarg;
        
	apr_initialize();
        apr_pool_create( &pool, NULL);
        lcn_atom_init( pool );
        lcn_log_stream = stderr;

        apr_getopt_init( &os, pool, argc, (const char* const*)argv ) ;

        while ( APR_SUCCESS == (s = apr_getopt_long( os, long_options, &optch, &optarg ) ))
        {
            switch( optch )
            {
            case 1:
                print_help();
                break;
            case 2:
                inputfile_name = apr_pstrdup( pool, (char*) optarg );
                break;
            case 3:
                index_path = apr_pstrdup( pool, (char *) optarg );
                break;
            default:
        	break;
    	    }
        }

	create_index(inputfile_name, index_path, pool);


    apr_pool_destroy( pool );
    atexit( apr_terminate );

    return 0;
}
