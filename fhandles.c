// JvW 21-05-2002  initial version, derived from SHMEMMON.C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <os2.h>

#define MAX_HANDLES     1024

#define NOT_OPENED     ((HFILE) (0xffffffff))

#define DUMMY_FILE     "_-test-_"

#define TPRINTF(txt)   printf("%s",txt); \
                       if (log) {fprintf(log, "%s",txt); fflush(log);}


// Show usage of the program
void fhandUsage
(
   char               *program                  // IN    program name
);

int main (int argc, char *argv[]);

/*****************************************************************************/
// Main function, entry point
/*****************************************************************************/
int main (int argc, char *argv[])
{
   APIRET              rc;
   ULONG               size = 0;
   ULONG               ivt;                     // interval time in minutes
   char                line[ 128];
   HFILE              *handles = NULL;          // array of handles

   if ( (argc == 1) ||                          // no argument
       ((argc  > 1) &&                          // at least one argument
        (argv[1][0] != '?') &&                  // and no explicit
        (argv[1][1] != '?') ))                  // help request
   {
      FILE            *log = NULL;              // optional logfile
      FILE            *dum = NULL;              // dummy logfile

      if ((dum = fopen( DUMMY_FILE, "w")) != NULL)
      {
         fclose( dum);                          // create empty dummy file

         if (argc > 1)                          // logfile specified
         {
            if ((log = fopen( argv[1], "a")) == NULL)
            {
               printf( "\nCannot append to file '%s', logging disabled\n",
                          argv[1]);
            }
         }
         sprintf( line, "\n%s 1.1  Show available handles.  "
                 "(c) 2002 - 2004  Fsys Software; Jan van Wijk\n\n", argv[0]);
         TPRINTF( line);

         if ((handles = calloc( MAX_HANDLES, sizeof(HFILE))) != NULL)
         {
            ULONG      i;
            ULONG      allocated = 0;
            char       tstamp[80];
            time_t     tt = time( &tt);         // current date/time
            ULONG      act;                     // action taken

            strftime( tstamp, 80, "%A %d-%m-%Y %H:%M:%S", localtime( &tt));

            for (i = 0; i < MAX_HANDLES; i++)
            {
               handles[i] = NOT_OPENED;
            }

            for (i = 0; i < MAX_HANDLES; i++)
            {

               if (DosOpen((PSZ) DUMMY_FILE,    // open dummy file
                            &(handles[i]),      // file handle
                            &act,               // action taken
                            0,                  // filesize
                            FILE_NORMAL,        // no attributes
                            FILE_OPEN,          // OpenFlag
                            OPEN_ACCESS_READONLY | // try just reading it
                            OPEN_SHARE_DENYNONE, // OpenMode (allow sharing)
                            0) == 0)            // reserved
               {
                  allocated++;
               }
            }
            for (i = 0; i < MAX_HANDLES; i++)
            {
               if (handles[i] != NOT_OPENED)
               {
                  DosClose( handles[i]);        // Free each handle
               }
            }
            sprintf( line, "%s - Available: % 5lu filehandles\n",
                     tstamp, allocated);
            TPRINTF( line);

            free( handles);
         }
         else
         {
            printf( "\nError allocating normal memory\n");
         }
         if (log)
         {
            fclose( log);
         }
         unlink( DUMMY_FILE);                   // delete dummy file
      }
      else
      {
         printf( "\nCannot create testfile '%s'\n", argv[0]);
      }
   }
   else
   {
      fhandUsage( argv[0]);
   }
   return(0);
}                                               // end 'main'
/*---------------------------------------------------------------------------*/


/*****************************************************************************/
// Show usage of the program
/*****************************************************************************/
void fhandUsage
(
   char               *program                  // IN    program name
)
{
   printf( "\nUsage: %s  [logfile]\n", program);

   printf( "\nParameters:     logfile     Path and filename for logfile\n");
   printf( "\nWhen specified, output will be APPENDED to logfile.\n");

}                                               // end 'fhandUsage'
/*---------------------------------------------------------------------------*/
