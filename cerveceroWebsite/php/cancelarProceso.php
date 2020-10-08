<?php
 require 'conexion.php';
 $IDplaca = $_POST['idPlaca']; 
 $sql = "UPDATE menu SET cancelar = 1 WHERE IDplaca=$IDplaca";
if ($conn->query($sql)) {
    echo "Okay";

} else {
    echo "Fail";
}
?>