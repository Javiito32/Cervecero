<?php
require '../conexion.php';
$mac = "'$_GET[mac]'";
$sql = $conn->query("SELECT IDplaca FROM placas WHERE mac=$mac");
while ($valores = mysqli_fetch_array($sql)) {
    echo "$valores[IDplaca]";
}
?>