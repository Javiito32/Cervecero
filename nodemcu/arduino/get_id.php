<?php
require '../conexion.php';
$mac = "'$_GET[mac]'";
$sql = $conn->query("SELECT id FROM placas WHERE mac=$mac");
while ($valores = mysqli_fetch_array($sql)) {
    echo "$valores[id]";
}
?>