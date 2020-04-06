<?php
require 'conexion.php';
$IDplaca = 1;
$dato = $_POST['receta'];

$sql = "UPDATE menu SET menu = 1, dato = $dato WHERE IDplaca = $IDplaca";
if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}
?>