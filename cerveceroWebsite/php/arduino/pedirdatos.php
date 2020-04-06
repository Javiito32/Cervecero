<?php
require '../conexion.php';

$sql = $conn->query("SELECT * FROM recetas WHERE id=$_GET[id]");
while ($valores = mysqli_fetch_array($sql)) {
    echo "nombre=$valores[nombre];tempMacer=$valores[tempMacer];tiempoMacer=$valores[tiempoMacer];tempCoc=$valores[tempCoc];tiempoCoc=$valores[tiempoCoc];tempFermen=$valores[tempFermen];tiempoFermen=$valores[tiempoFermen]";
}
?>