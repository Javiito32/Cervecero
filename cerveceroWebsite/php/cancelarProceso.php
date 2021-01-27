<?php
 require 'conexion.php';
 require './phpMQTT/phpMQTT.php';
 
 $server = 'localhost';     // change if necessary
 $port = 1883;                     // change if necessary
 $username = '';                   // set your username
 $password = '';                   // set your password
 $client_id = 'Server'; // make sure this is unique for connecting to sever - you could use uniqid()
 
 $mqtt = new Bluerhinos\phpMQTT($server, $port, $client_id);


 $id_Placa = $_POST['idPlaca']; 
 $sql = "INSERT INTO logCancelaciones (id_Placa) VALUES ($id_placa)";

if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}

$json = array("menu" => 4, "dato1" => 0, "dato2" => 0);

if ($mqtt->connect(true, NULL, $username, $password)) {

    $mqtt->publish('2/menu', json_encode($json), 0, false);
    $mqtt->close();
}
?>