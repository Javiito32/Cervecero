<?php
require 'conexion.php';

$sql = $conn->query("SELECT * FROM info WHERE id=$_GET[id]");
while ($valores = mysqli_fetch_array($sql)) {
    echo "temp=$valores[temp];hum=$valores[hum]";
}
?>