/**
 * char.c
 *
 * Copyright (c) 1999, 2000, 2001
 *	Lu-chuan Kung and Kang-pen Chen.
 *	All rights reserved.
 *
 * Copyright (c) 2004, 2005, 2006, 2008
 *	libchewing Core Team. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

/** 
 * @file char.c
 * @brief word data file
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "global-private.h"
#include "chewing-definition.h"
#include "char-private.h"
#include "private.h"
#include "plat_mmap.h"

#ifdef USE_BINARY_DATA
static uint16* arrPhone = NULL;
static int *begin = NULL;
static char *phone_data_buf = NULL;
static int phone_num;
#else
static uint16 arrPhone[ PHONE_NUM + 1 ];
static int begin[ PHONE_NUM + 1 ];
#endif
static FILE *dictfile;
static int end_pos;

static char *fgettab( char *buf, int maxlen, FILE *fp )
{
	int i;

	for ( i = 0; i < maxlen; i++ ) {
		buf[ i ] = (char) fgetc( fp );
		if ( feof( fp ) )
			break;
		if ( buf[ i ] == '\t' )
			break;
	}
	if ( feof( fp ) )
		return 0;
	buf[ i ] = '\0';
	return buf;
}

static int CompUint16( const uint16 *pa, const uint16 *pb )
{
	return ( (*pa) - (*pb) );
}

static void TerminateChar()
{
	if ( dictfile )
		fclose( dictfile );
#ifdef USE_BINARY_DATA
	if( phone_data_buf )
		free( phone_data_buf );
#endif
}

int InitChar( const char *prefix )
{
	char filename[ 100 ];

#ifdef USE_BINARY_DATA
	long file_size;
	int indexfile;
	struct stat file_stat;
#else
	FILE *indexfile;
	int i;
#endif

	sprintf( filename, "%s" PLAT_SEPARATOR "%s", prefix, CHAR_FILE );
	dictfile = fopen( filename, "r" );

	sprintf( filename, "%s" PLAT_SEPARATOR "%s", prefix, CHAR_INDEX_FILE );

#ifdef USE_BINARY_DATA
	indexfile = open( filename, O_RDONLY );
	if ( ! dictfile || -1 == indexfile )
		return 0;
	fstat( indexfile, &file_stat );
	file_size = (long)file_stat.st_size;

	phone_num = file_size / (sizeof(int) + sizeof(uint16));
	phone_data_buf = malloc( file_size );

	if ( !phone_data_buf ||
	     !read( indexfile, phone_data_buf, file_size ) ) {
		close( indexfile );
		return 0;
	}

	begin = (int *) phone_data_buf;
	arrPhone = (uint16 *) (begin + phone_num);

	close(indexfile);
#else
	indexfile = fopen( filename, "r" );

	if ( ! dictfile || ! indexfile )
		return 0;

	for ( i = 0; i <= PHONE_NUM; i++ )
		fscanf( indexfile, "%hu %d", &arrPhone[ i ], &begin[ i ] );
	fclose( indexfile );
#endif
	addTerminateService( TerminateChar );
	return 1;
}

static void Str2Word( Word *wrd_ptr )
{
	char buf[ 1000 ];
	uint16 sh;

	fgettab( buf, 1000, dictfile );
	/* only read 6 bytes to wrd_ptr->word avoid buffer overflow */
	sscanf( buf, "%hu %6[^ ]", &sh, wrd_ptr->word );
	assert( wrd_ptr->word != '\0' );
}

int GetCharFirst( Word *wrd_ptr, uint16 phoneid )
{
	uint16 *pinx;

	pinx = (uint16 *) bsearch(
#ifdef USE_BINARY_DATA
		&phoneid, arrPhone, phone_num,
#else
		&phoneid, arrPhone, PHONE_NUM, 
#endif
		sizeof( uint16 ), (CompFuncType) CompUint16 );
	if ( ! pinx )
		return 0;

	fseek( dictfile, begin[ pinx - arrPhone ], SEEK_SET );
	end_pos = begin[ pinx - arrPhone + 1 ];
	Str2Word( wrd_ptr );
	return 1;
}

int GetCharNext( Word *wrd_ptr )
{
	if ( ftell( dictfile ) >= end_pos )
		return 0;
	Str2Word( wrd_ptr );
	return 1;
}

