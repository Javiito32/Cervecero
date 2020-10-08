<?php
require 'conexion.php';
$idPlaca = $_POST['idPlaca'];
$proceso = $_POST['proceso'];
$paso = $_POST['paso'];

$sql = "UPDATE menu SET menu = 2, dato1 = $proceso, dato2 = $paso WHERE IDplaca = $idPlaca";
if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}
?>