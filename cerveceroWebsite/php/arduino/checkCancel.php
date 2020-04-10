<?php
require '../conexion.php';
$IDplaca = $_POST['IDplaca'];
$sql = $conn->query("SELECT cancelar FROM menu WHERE IDplaca = $IDplaca");
while ($valores = mysqli_fetch_array($sql)) {
    echo "$valores[cancelar]";
}

?>