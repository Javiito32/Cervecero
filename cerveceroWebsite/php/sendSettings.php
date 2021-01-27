<?php
require 'conexion.php';
require './phpMQTT/phpMQTT.php';

$server = 'localhost';     // change if necessary
$port = 1883;                     // change if necessary
$username = '';                   // set your username
$password = '';                   // set your password
$client_id = 'Server'; // make sure this is unique for connecting to sever - you could use uniqid()

$mqtt = new Bluerhinos\phpMQTT($server, $port, $client_id);

$IDplaca = $_POST['IDplaca'];
$setting = $_POST['setting'];

$sql = "UPDATE menu SET menu = 3, dato1 = $setting WHERE IDplaca = $IDplaca";

if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}

$sql = $conn->query("SELECT menu, dato1, dato2 FROM menu WHERE IDplaca=$IDplaca");

while ($valores = mysqli_fetch_array($sql)){

        $datos = array("menu" => $valores['menu'], "dato1" => $valores['dato1'], "dato2" => $valores['dato2']);
        //echo json_encode($datos), "\n";

        if ($mqtt->connect(true, NULL, $username, $password)) {

          $mqtt->publish('2/menu', json_encode($datos), 0, false);
          $mqtt->close();
        }
    }
?>