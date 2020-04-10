<?php
require 'conexion.php';
$IDplaca = 1;
$receta = $_POST['receta'];

$sql = "UPDATE menu SET menu = 1, dato1 = $receta WHERE IDplaca = $IDplaca";
if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}
?>