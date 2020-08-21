<?php
require '../conexion.php';
$IDplaca = $_POST['IDplaca'];
$receta = $_POST['receta'];
$proceso = $_POST['proceso'];
$pasoProceso = $_POST['pasoProceso'];
$estado = $_POST['estado'];
$tiempoRestante = $_POST['tiempoRestante'];
$porcentaje = $_POST['porcentaje'];
$sql = $conn->query("UPDATE info SET receta = $receta, proceso = $proceso, pasoProceso = $pasoProceso, estado = $estado, tiempoRestante=$tiempoRestante, porcentaje = $porcentaje WHERE IDplaca=$IDplaca");
?>