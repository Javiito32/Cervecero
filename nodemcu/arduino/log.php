<?php
require '../conexion.php';
$proceso = $_GET['proceso'];
$estado = $_GET['estado'];
$mac = "'$_GET[placaID]'";
$sql = $conn->query("INSERT INTO log (time, placaID, proceso, estado) VALUES (now(),$mac,$proceso,$estado)");
echo"$sql";
?>