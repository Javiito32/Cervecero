<?php
require '../conexion.php';
$IDplaca = $_POST['IDplaca'];

//echo $date->;
$sql = $conn->query("SELECT receta, time, proceso, pasoProceso, tiempoRestante, estado FROM log WHERE IDplaca=$IDplaca ORDER BY TIME DESC LIMIT 1 ");
while ($valores = mysqli_fetch_array($sql)) {

    echo "receta=$valores[receta];tiempoRestante=$valores[tiempoRestante];proceso=$valores[proceso];pasoProceso=$valores[pasoProceso];estado=$valores[estado]";
}
?>