<?php
require 'conexion.php';
$idPlaca = $_POST['idPlaca'];
$receta = $_POST['receta'];

$sql = "UPDATE menu SET menu = 1, dato1 = $receta WHERE IDplaca = $idPlaca";
if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}
?>