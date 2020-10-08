<?php
require '../conexion.php';
$mac = "'$_POST[mac]'";
$sql = $conn->query("SELECT IDplaca FROM placas WHERE mac=$mac");
if (mysqli_num_rows($sql) != 0) {
    $valores = mysqli_fetch_array($sql);
    echo "$valores[IDplaca]";
} else {
    echo "fallo";
}
?>