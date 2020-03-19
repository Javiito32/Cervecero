<?php
require '../conexion.php';
if (isset($_GET['reset'])) {
    $sql = $conn->query("UPDATE info SET fallo = 0 WHERE id=1");
    
}
$sql = $conn->query("SELECT fallo FROM info WHERE id=1");
while ($valores = mysqli_fetch_array($sql)) {
    echo "$valores[fallo]";
}
?>