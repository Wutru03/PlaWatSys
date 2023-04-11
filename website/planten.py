#!/usr/bin/python3


#sudo chmod +x /usr/lib/cgi-bin/planten.py


import mysql.connector
mydb = mysql.connector.connect(
  host="localhost",
  user="plawatsys",
  password="virtualwater",
  database="PlaWatSys"
)


print("content-type: text/plain")
print("")


mycursor = mydb.cursor()
mycursor.execute("SELECT planten.plantId, soorten.naam, planten.alive FROM planten JOIN soorten ON planten.soortId = soorten.soortId order by alive desc, plantId asc")
myresult = mycursor.fetchall()


char="()'"
for i, x in enumerate(myresult):
    print(x[0],",",x[1],",",x[2],end="")
    if i == len(myresult) - 1:
        print( "", end="")
    else:
        print("\n",end="")
   
mydb.disconnect()
