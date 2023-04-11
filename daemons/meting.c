#include <stdio.h>
#include <mariadb/mysql.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


#define inputPin 16
#define outputPin 26


// #define DEBUG
// #define RAND


void finish_with_error(MYSQL *con);
int plantCount(MYSQL *con);
void plantIDs(MYSQL *con, int[]);


int getMax(MYSQL *con, int);
void DBInMeting(MYSQL *con, int, int);
int getV();


void DBInWater(MYSQL *con, int);
void water();


void initialize();
void closePins();
int readInput();
int getVData();
int binaryToDecimal(int binary);


int main()
{


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


    initialize();


#ifdef RAND
    srand(time(NULL));
#endif


    int count = plantCount(con);
    int IDs[count];
    plantIDs(con, IDs);


    for (int i = 0; i < count; i++)
    {


        int vmax = getMax(con, IDs[i]);


        int vocht = getV();


        sleep(1);
        if (vmax > vocht)
        {
            DBInWater(con, IDs[i]);
            water();
        }


        DBInMeting(con, IDs[i], vocht);
    }


    mysql_close(con); // close connection
    closePins();
    return 0;
}


void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con)); // print error
    mysql_close(con);                          // close connection
    exit(1);
}


int plantCount(MYSQL *con)
{
    if (mysql_query(con, "SELECT plantId from planten WHERE alive = 1")) // get list of plants
    {
        finish_with_error(con);
    }
    MYSQL_RES *PCresult = mysql_store_result(con); // store result
    if (PCresult == NULL)                          // check if empty
    {
        finish_with_error(con);
    }
    int i = 0;
    MYSQL_ROW PCrow;
    while ((PCrow = mysql_fetch_row(PCresult))) // for every row in result
    {
        i++;
    }
    return i;
    mysql_free_result(PCresult);
}


void plantIDs(MYSQL *con, int array[])
{


    if (mysql_query(con, "SELECT plantId from planten WHERE alive = 1")) // get list of plants
    {
        finish_with_error(con);
    }
    MYSQL_RES *Presult = mysql_store_result(con); // store result
    if (Presult == NULL)                          // check if empty
    {
        finish_with_error(con);
    }
    int i = 0;
    MYSQL_ROW Prow;
    while ((Prow = mysql_fetch_row(Presult))) // for every row in result
    {
        array[i] = atoi(Prow[0]);
        i++;
    }
    mysql_free_result(Presult);
}


int getMax(MYSQL *con, int ID)
{
    int max;
    char query[255];
    sprintf(query, "SELECT soorten.maxVocht FROM soorten JOIN planten ON soorten.soortId = planten.soortId where planten.plantId = %d", ID);


    if (mysql_query(con, query)) // get maxv for plant
    {
        finish_with_error(con);
    }
    MYSQL_RES *MAXresult = mysql_store_result(con); // store result
    if (MAXresult == NULL)                          // check if empty
    {
        finish_with_error(con);
    }


    MYSQL_ROW row = mysql_fetch_row(MAXresult);
    if (row != NULL)
    {
        max = atoi(row[0]);
    }


    mysql_free_result(MAXresult);


#ifdef DEBUG
    printf("vmax: %d \n", max);
#endif


    return max;
}


void DBInMeting(MYSQL *con, int ID, int vocht)
{


    char query[255];
    sprintf(query, "INSERT INTO metingen(plantId, datum, tijd, vocht) VALUES(%d,CURRENT_DATE(),CURRENT_TIME(),%d)", ID, vocht);


    #ifdef DEBUG
    printf("%d \n", vocht);
    printf("%s \n", query);
    #endif


    #ifndef DEBUG
        if (mysql_query(con, query))
        {
            finish_with_error(con);
        }
    #endif
}


int getV()
{
    int vocht;
#ifdef RAND
    vocht = rand() % 101;
    printf("vocht: %d \n", vocht);
#endif


#ifndef RAND
    int fd = open("/sys/class/gpio/gpio26/value", O_WRONLY);


    // starting bit
    write(fd, "1", 1);
    usleep(800);


    // command bit
    write(fd, "0", 1);


    vocht = getVData();
    // vocht = 100 - vocht;
#endif
    return vocht;
}


void DBInWater(MYSQL *con, int ID)
{
    char query[255];
    sprintf(query, "INSERT INTO pomp(plantId, datum, tijd) VALUES(%d,CURRENT_DATE(),CURRENT_TIME())", ID);


#ifdef DEBUG
    printf("%s \n", query);
#endif


#ifndef DEBUG
    if (mysql_query(con, query))
    {
        finish_with_error(con);
    }
#endif
}


void water()
{
    int fd = open("/sys/class/gpio/gpio26/value", O_WRONLY);


    // starting bit
    write(fd, "1", 1);
    usleep(800);


    // command bit
    write(fd, "1", 1);
    usleep(1000);
    write(fd, "0", 1);


    while (readInput() == 0)
    {
    }
}


void initialize()
{
    int fd;


    fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "16", 3);
    write(fd, "26", 3);
    close(fd);
    sleep(1);


    fd = open("/sys/class/gpio/gpio16/direction", O_WRONLY);
    write(fd, "in", 3);
    close(fd);


    fd = open("/sys/class/gpio/gpio26/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);
}


void closePins()
{
    int fd;
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fd, (char *)outputPin, 3);
    write(fd, (char *)inputPin, 3);
    close(fd);
}


int readInput()
{
    int fd;
    char value;
    fd = open("/sys/class/gpio/gpio16/value", O_RDONLY);
    read(fd, &value, 1);
    close(fd);


    int intValue = atoi(&value);
    return intValue;
}


int getVData()
{


    while (readInput() == 0)
    {
    }


    usleep(1000);


    int binary = 0;


    for (int i = 0; i < 8; i++)
    {


        int bit = readInput();
        binary = binary * 10 + bit;
#ifdef DEBUG
        printf("%d", bit);
#endif
        usleep(5000);
    }
#ifdef DEBUG
    printf("\n");
#endif
    int decimal = binaryToDecimal(binary);
    return decimal;
}


int binaryToDecimal(int binary)
{
    int decimal = 0, base = 1;
    while (binary > 0)
    {
        decimal += (binary % 10) * base;
        binary /= 10;
        base *= 2;
    }
    return decimal;
}
