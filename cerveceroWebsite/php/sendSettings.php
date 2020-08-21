<?php
require 'conexion.php';
$IDplaca = 1;
$setting = $_POST['setting'];

$sql = "UPDATE menu SET menu = 3, dato1 = $setting WHERE IDplaca = $IDplaca";
if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}
?>