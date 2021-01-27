<?php
$host = "localhost";
$dbusername = "arduino";
$dbpassword = "IVSZ2e12";
$dbname = "cervecero";
// Create connection
$conn = new mysqli($host, $dbusername, $dbpassword, $dbname);

if (mysqli_connect_error()){
    die('Connect Error ('. mysqli_connect_errno() .') '
    . mysqli_connect_error());
    echo "No se pudo conectar con la BDD";
    }
?>