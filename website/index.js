var plantId = 1;
const portno = 51515;


var tableBody1 = document.getElementById("plantenBody");
var xhttp1 = new XMLHttpRequest();


xhttp1.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var data = this.responseText.split('\n');
        for (var i = 0; i < data.length; i++) {
            var row = tableBody1.insertRow(i);
            var cells = data[i].split(',');
            for (var j = 0; j < cells.length; j++) {
                var cell = row.insertCell(j);
                if (j === 2) {
                    cell.innerHTML = (cells[j] === " 1") ? "Yes" : "No";
                } else {
                    cell.innerHTML = cells[j];
                }
            }
            // Add event listener to each row
            row.addEventListener('click', function () {
                // Get value of first cell in clicked row
                plantId = parseInt(this.cells[0].innerHTML);


                console.log("Clicked row with plantId: " + plantId);
                reloadMetingen(plantId);
                reloadpomp(plantId);
            });
        }
    }
};


xhttp1.open("GET", "/cgi-bin/planten.py", true);
xhttp1.send();




var tableBody2 = document.getElementById("metingenBody");
var xhttp2 = new XMLHttpRequest();
xhttp2.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var data = this.responseText.split('\n');
        for (var i = 0; i < data.length; i++) {
            var row = tableBody2.insertRow(i);
            var cells = data[i].split(',');
            for (var j = 0; j < cells.length; j++) {
                var cell = row.insertCell(j);
                cell.innerHTML = cells[j];
            }
        }
    }
};
xhttp2.open("GET", "/cgi-bin/metingen.py?plantId=" + plantId, true);
xhttp2.send();


var tableBody3 = document.getElementById("pompBody");
var xhttp3 = new XMLHttpRequest();
xhttp3.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
        var data = this.responseText.split('\n');
        for (var i = 0; i < data.length; i++) {
            var row = tableBody3.insertRow(i);
            var cells = data[i].split(',');
            for (var j = 0; j < cells.length; j++) {
                var cell = row.insertCell(j);
                cell.innerHTML = cells[j];
            }
        }
    }
};
xhttp3.open("GET", "/cgi-bin/pomp.py?plantId=" + plantId, true);
xhttp3.send();


// var tableBody4 = document.getElementById("beschrijvingBody");
// var xhttp4 = new XMLHttpRequest();
// xhttp4.onreadystatechange = function () {
//     if (this.readyState == 4 && this.status == 200) {
//         var data = this.responseText.split('\n');
//         for (var i = 0; i < data.length; i++) {
//             var row = tableBody4.insertRow(i);
//             var cells = data[i].split(',');
//             for (var j = 0; j < cells.length; j++) {
//                 var cell = row.insertCell(j);
//                 cell.innerHTML = cells[j];
//             }
//         }
//     }
// };
// xhttp4.open("GET", "/cgi-bin/beschrijving.py?plantId=" + plantId, true);
// xhttp4.send();


function MN(portno, plantId) {
    var myDiv = document.getElementById("MNR");
    var xhttp5 = new XMLHttpRequest();
    xhttp5.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("MNR").innerHTML = this.responseText;
            myDiv.style.display = "block"; // show the div when the response is received
            setTimeout(function () {
                myDiv.style.display = "none"; // hide the div after 20 seconds
            }, 20000);
        }
    };
    xhttp5.open("GET", "/cgi-bin/plawatsys-socket.py?port=" + portno, true);
    xhttp5.send();
    reloadMetingen(plantId);
    reloadpomp(plantId);
}


function toggle(portno, plantId) {
    var myDiv = document.getElementById("toggleR");
    var xhttp5 = new XMLHttpRequest();
    xhttp5.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("toggleR").innerHTML = this.responseText;
            myDiv.style.display = "block"; // show the div when the response is received
            setTimeout(function () {
                myDiv.style.display = "none"; // hide the div after 20 seconds
            }, 10000);
            reloadPlant(plantId);
        }
    };
    xhttp5.open("GET", "/cgi-bin/toggle.py?port=" + portno + "&plantId=" + plantId, true);
    xhttp5.send();
}


function reloadPlant(plantId) {
    // Clear table rows
    tableBody1.innerHTML = '';


    // Update URL and fetch new data for each table
    xhttp1.open("GET", "/cgi-bin/planten.py", true);
    xhttp1.send();


}


function reloadMetingen(plantId) {
    // Clear table rows
    tableBody2.innerHTML = '';


    // Update URL and fetch new data for each table
    xhttp2.open("GET", "/cgi-bin/metingen.py?plantId=" + plantId, true);
    xhttp2.send();
}


function reloadpomp(plantId){
    // Clear table rows
    tableBody3.innerHTML = '';


    // Update URL and fetch new data for each table
    xhttp3.open("GET", "/cgi-bin/pomp.py?plantId=" + plantId, true);
    xhttp3.send();
}


// function reloadbeschrijving(plantId){
//     // Clear table rows
//     tableBody4.innerHTML = '';


//     // Update URL and fetch new data for each table
//     xhttp4.open("GET", "/cgi-bin/beschrijving.py?plantId=" + plantId, true);
//     xhttp4.send();
// }


// 8========D