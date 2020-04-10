<?php
require '../conexion.php';
$IDplaca = $_POST['IDplaca'];
$sql = $conn->query("UPDATE menu SET cancelar = 0 WHERE IDplaca = $IDplaca");