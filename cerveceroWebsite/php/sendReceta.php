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
$receta = $_POST['receta'];

$sql = "UPDATE menu SET menu = 1, dato1 = $receta WHERE IDplaca = $IDplaca";

if ($conn->query($sql)) {

    echo "Okay";

} else {
    
    echo "Fail";
}

$json = array("menu" => 1, "dato1" => $receta, "dato2" => 0);

if ($mqtt->connect(true, NULL, $username, $password)) {

    $mqtt->publish('2/menu', json_encode($json), 0, false);
    $mqtt->close();
}


?>