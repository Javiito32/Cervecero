<?php
require '../conexion.php';
$IDplaca = $_POST['IDplaca'];

//echo $date->;
$sql = $conn->query("SELECT receta, time, proceso, pasoProceso, tiempoRestante, estado FROM log WHERE IDplaca=$IDplaca ORDER BY TIME DESC LIMIT 1 ");

while ($payload = mysqli_fetch_array($sql)) {

$json = array('receta' => $payload['receta'], 'tiempoRestante' => $payload['tiempoRestante'], 'proceso' => $payload['proceso'], 'pasoProceso' => $payload['pasoProceso'], 'estado' => $payload['estado']);
echo json_encode($json);
     
}

?>