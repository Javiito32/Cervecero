<?php
require '../conexion.php';
$IDplaca = $_GET['IDplaca'];
$receta = $_GET['receta'];
$proceso = $_GET['proceso'];
$pasoProceso = $_GET['pasoProceso'];
$estado = $_GET['estado'];
$tiempoRestante = $_GET['tiempoRestante'];
$porcentaje = $_GET['porcentaje'];
$sql = $conn->query("UPDATE info SET receta = $receta, proceso = $proceso, pasoProceso = $pasoProceso, estado = $estado, tiempoRestante=$tiempoRestante, porcentaje = $porcentaje WHERE IDplaca=$IDplaca");
?>