<?php
require 'conexion.php';
$IDplaca = 1;
$proceso = $_POST['proceso'];
$paso = $_POST['paso'];

$sql = "UPDATE menu SET menu = $proceso, dato = $paso WHERE IDplaca = $IDplaca";
if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}
?>