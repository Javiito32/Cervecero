<?php
require '../conexion.php';

$sql = $conn->query("SELECT * FROM recetas WHERE id=$_GET[id]");
while ($valores = mysqli_fetch_array($sql)) {
    echo "name=$valores[nombre];tempMacer=$valores[temp];timeMacer=$valores[hum]";
}
?>