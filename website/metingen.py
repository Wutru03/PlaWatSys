#!/usr/bin/python3


#sudo chmod +x /usr/lib/cgi-bin/metingen.py


import mysql.connector
import cgi


form = cgi.FieldStorage()
plantId = form.getvalue("plantId")


#set to text for html/js
print("content-type: text/plain")
print("")


#connect to server
mydb = mysql.connector.connect(
  host="localhost",
  user="plawatsys",
  password="virtualwater",
  database="PlaWatSys"
)


#select info from db
mycursor = mydb.cursor()
mycursor.execute(f"SELECT datum, tijd, vocht FROM metingen WHERE plantId = {plantId} ORDER BY datum DESC, tijd DESC LIMIT 100")
myresult = mycursor.fetchall()


#output data
char="()'"
for i, x in enumerate(myresult):
    print(x[0],",",x[1],",",x[2],end="")
    if i == len(myresult) - 1:
        print( "", end="")
    else:
        print("\n",end="")


#disconnect
mydb.disconnect()
