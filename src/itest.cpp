#include <irods/irods_client_api_table.hpp>
#include <irods/irods_pack_table.hpp>
#include <irods/rods.h>
#include <irods/rodsClient.h>
#include <irods/irods_random.hpp>
#include <irods/rcMisc.h>

#include <string>
#include <iostream>



void usage( char *prog);

int 
main(int argc, char **argv){

    signal(SIGPIPE, SIG_IGN);
    
    rodsArguments_t myRodsArgs;

    int status = parseCmdLineOpt( argc, argv, "h", 0, &myRodsArgs );
    if ( status ) {
        printf( "Use -h for help\n" );
        exit( 1 );
    }
    if ( myRodsArgs.help == True ) {
        usage( argv[0] );
        exit( 0 );
    }

    rodsEnv myEnv;
    status = getRodsEnv( &myEnv);

    if (status < 0) {
        rodsLogError(LOG_ERROR, status, "main: getRodsEnv error");
        exit(1);
    }

    irods::api_entry_table&  api_tbl = irods::get_client_api_table();
    irods::pack_entry_table& pk_tbl  = irods::get_pack_table();
    init_api_table( api_tbl, pk_tbl );

    rErrMsg_t errMsg;
    rcComm_t *conn;

    conn = rcConnect(
        myEnv.rodsHost,
        myEnv.rodsPort,
        myEnv.rodsUserName,
        myEnv.rodsZone,
        0, &errMsg );

    if (conn == NULL) {
        exit(2);
    }

    status = clientLogin(conn);

    if ( status != 0 ) {
        exit(3);
    }

    irods::ticket::administration.creatReadTicket(conn, "/tempZone/home/public", "testTicket");

    exit(0);
}

void usage( char *prog) {
    printf("This was a test command I created to make a new ticket!\n");
    printf("Usage: %s [-h] \n", prog);
    printf(" -h this help\n");
    printReleaseInfo("itest");
}