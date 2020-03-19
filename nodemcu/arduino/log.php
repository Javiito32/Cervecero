<?php
require '../conexion.php';
$proceso = "'". $_GET['proceso'] ."'";
$estado = "'". $_GET['estado'] ."'";
$sql = $conn->query("INSERT INTO log (time, proceso, estado) VALUES (now(),$proceso,$estado)");
echo"$sql";
?>