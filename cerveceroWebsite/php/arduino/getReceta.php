<?php
require '../conexion.php';
$IDreceta = $_POST['IDreceta'];
$sql = $conn->query("SELECT * FROM recetas WHERE id=$IDreceta");
while ($valores = mysqli_fetch_array($sql)) {
    $datos = array("nombre" => $valores['nombre'], "tempMacer" => $valores['tempMacer'], "tiempoMacer" => $valores['tiempoMacer'], "tempCoc" => $valores['tempCoc'], "tiempoCoc" => $valores['tiempoCoc'], "tempFermen" => $valores['tempFermen'], "tiempoFermen" => $valores['tiempoFermen']);
    echo json_encode($datos);
    //echo "nombre=$valores[nombre];tempMacer=$valores[tempMacer];tiempoMacer=$valores[tiempoMacer];tempCoc=$valores[tempCoc];tiempoCoc=$valores[tiempoCoc];tempFermen=$valores[tempFermen];tiempoFermen=$valores[tiempoFermen]";
}
?>