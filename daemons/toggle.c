#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>


void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con)); // print error
    mysql_close(con);                          // close connection
    exit(1);
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "incorrect args");
        exit(1);
    }


    MYSQL *con;
    if (!(con = mysql_init(0)))
    {
        fprintf(stderr, "unable to initialize connection struct\n");
        exit(1);
    }


    // Connect to the database
    if (!mysql_real_connect(
            con,            // Connection
            "localhost",    // Host
            "plawatsys",    // User account
            "virtualwater", // User password
            "PlaWatSys",    // Default database
            0,              // Port number
            NULL,           // Path to socket file
            0               // Additional options
            ))
    {
        // Report the failed-connection error & close the handle
        fprintf(stderr, "Error connecting to Server: %s\n", mysql_error(con)); // print error
        mysql_close(con);                                                      // close connection
        exit(1);                                                               // exit program
    }


    int id = atoi(argv[1]);
    char query[255];
    sprintf(query, "SELECT alive FROM planten WHERE plantId = %d", id);
    if (mysql_query(con, query)) // get list of plants
    {
        finish_with_error(con);
    }
    MYSQL_RES *result = mysql_store_result(con); // store result
    if (result == NULL)                          // check if empty
    {
        finish_with_error(con);
    }
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) // for every row in result
    {
        if (row[0] == NULL)
        {
            mysql_free_result(result);
            sprintf(query, "UPDATE planten SET alive = true WHERE plantId = %d", id);
            if (mysql_query(con, query)) // get list of plants
            {
                finish_with_error(con);
            }
            result = mysql_use_result(con);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
            }
            mysql_free_result(result);
        }
        else
        {
            mysql_free_result(result);


            sprintf(query, "UPDATE planten SET alive = null WHERE plantId = %d", id);
            if (mysql_query(con, query)) // get list of plants
            {
                finish_with_error(con);
            }
            result = mysql_use_result(con);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
            }
            mysql_free_result(result);
        }
    }


    mysql_free_result(result);


    mysql_close(con); // close connection
    return 0;
}
